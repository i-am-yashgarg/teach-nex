#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keyboard.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int OLED_SDA = 4;
const int OLED_SCL = 5;
const int MODE_BTN = 15;
const int TEST_BTN = 18; // Point-save button
const int ACTION_BTNS[12] = {0, 1, 2, 3, 6, 7, 8, 9, 10, 11, 13, 12};
const int LED_PIN = LED_BUILTIN;

bool isCartesianMode = false;
bool lastModeState = HIGH;
unsigned long modeDebounceTime = 0;
const unsigned long MODE_DEBOUNCE_DELAY = 50;
bool lastTestBtnState = HIGH;
unsigned long testBtnDebounceTime = 0;
const unsigned long TEST_BTN_DEBOUNCE_DELAY = 50;

// =======================================================
// Point naming counter
// =======================================================
int currentPointNum = 1;
const int MAX_POINT_NUM = 999; // wrap-around safety, adjust as needed

// STATE TRACKING: lastReading array removed, using Lockout Debounce
bool buttonState[12] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
unsigned long lastDebounceTime[12] = {0};
const unsigned long BTN_DEBOUNCE_DELAY = 1;

unsigned long last10HzTick = 0;

// =======================================================
// HID Output Report callback
// =======================================================
void onKeyboardLED(bool numLock, bool capsLock, bool scrollLock, bool compose, bool kana, void* cbData) {
  digitalWrite(LED_PIN, capsLock ? HIGH : LOW);

  display.fillRect(0, 56, SCREEN_WIDTH, 8, SSD1306_BLACK);
  display.setTextSize(1);
  display.setCursor(0, 56);
  display.print(capsLock ? "PC->LED: ON" : "PC->LED: OFF");
  display.display();
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Mode:");

  display.setTextSize(2);
  display.setCursor(10, 12);
  display.print(isCartesianMode ? "CARTESIAN" : "JOINTS");

  display.setTextSize(1);
  display.setCursor(0, 36);
  display.print("Current Point:");

  display.setTextSize(2);
  display.setCursor(10, 48);
  display.print("@p" + String(currentPointNum));

  display.display();
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Wire.setSDA(OLED_SDA);
  Wire.setSCL(OLED_SCL);
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;);
  }
  display.setRotation(2);

  pinMode(MODE_BTN, INPUT_PULLUP);
  pinMode(TEST_BTN, INPUT_PULLUP);
  for (int i = 0; i < 12; i++) {
    pinMode(ACTION_BTNS[i], INPUT_PULLUP);
  }

  Keyboard.begin();
  Keyboard.onLED(onKeyboardLED); 

  updateDisplay();
}

void sendHIDCommand(int buttonIndex, bool isPressed) {
  String command = "";
  
  // Determine if this is a positive button (0-5) or negative button (6-11)
  bool isPositive = (buttonIndex < 6);
  
  // Normalize the index so it is always 0-5, regardless of positive/negative
  int normalizedIndex = isPositive ? buttonIndex : (buttonIndex - 6);

  if (!isCartesianMode) {
    // === JOINT MODE ===
    String axis = String(normalizedIndex + 1); // Gives "1" to "6"
    
    if (isPressed) {
      command = (isPositive ? "+j" : "-j") + axis; // e.g., +j1 or -j1
    } else {
      command = "0j" + axis;                       // e.g., 0j1
    }
    
  } else {
    // === CARTESIAN MODE ===
    char cartAxes[] = {'x', 'y', 'z', 'r', 'p', 'w'};
    String axis = String(cartAxes[normalizedIndex]); // Gives "x", "y", etc.
    
    if (isPressed) {
      command = (isPositive ? "+c" : "-c") + axis; // e.g., +cx or -cx
    } else {
      command = "0c" + axis;                       // e.g., 0cx
    }
  }

  Keyboard.print(command);
  Keyboard.write('\n');
}

void loop() {
  unsigned long currentTime = millis();

  // =======================================================
  // Mode button (single click)
  // =======================================================
  bool currentModeState = digitalRead(MODE_BTN);
  if (currentModeState != lastModeState) {
    if (currentTime - modeDebounceTime > MODE_DEBOUNCE_DELAY) {
      if (currentModeState == LOW) {
        isCartesianMode = !isCartesianMode;
        updateDisplay();
      }
      modeDebounceTime = currentTime;
    }
  }
  lastModeState = currentModeState;

  // =======================================================
  // Point-save button (single click) - sends "@pN" then increments
  // =======================================================
  bool currentTestBtnState = digitalRead(TEST_BTN);
  if (currentTestBtnState != lastTestBtnState) {
    if (currentTime - testBtnDebounceTime > TEST_BTN_DEBOUNCE_DELAY) {
      if (currentTestBtnState == LOW) {
        // Send the current point name to the system
        Keyboard.print("@p" + String(currentPointNum));
        Keyboard.write('\n');

        // Move to the next point name
        currentPointNum++;
        if (currentPointNum > MAX_POINT_NUM) {
          currentPointNum = 1; // wrap around if you want, or remove this if not needed
        }

        // Update the screen to show the new current point
        updateDisplay();
      }
      testBtnDebounceTime = currentTime;
    }
  }
  lastTestBtnState = currentTestBtnState;

  // =======================================================
  // Action Buttons: Zero-Latency Debounce (Instant Press/Release)
  // =======================================================
  for (int i = 0; i < 12; i++) {
    bool reading = digitalRead(ACTION_BTNS[i]);

    // If the hardware reads differently than our recorded state
    if (reading != buttonState[i]) {
      // Only accept the change if the cooldown period has passed
      if (currentTime - lastDebounceTime[i] > BTN_DEBOUNCE_DELAY) {
        
        buttonState[i] = reading;            // Update the state
        lastDebounceTime[i] = currentTime;   // Start the cooldown timer

        if (buttonState[i] == HIGH) {
          // BUTTON RELEASED: Stop instantly
          sendHIDCommand(i, false);
        } else {
          // BUTTON PRESSED: Move instantly
          sendHIDCommand(i, true);
          
          // Reset the 10Hz clock so it doesn't double-fire immediately after an initial press
          last10HzTick = currentTime; 
        }
      }
    }
  }

  // =======================================================
  // 10Hz Continuous Action Button Poll (While holding)
  // =======================================================
  if (currentTime - last10HzTick >= 100) {
    last10HzTick = currentTime;
    
    for (int i = 0; i < 12; i++) {
      if (buttonState[i] == LOW) { 
        sendHIDCommand(i, true);
      }
    }
  }
}