⚙️ TEACH//PENDANT
Teach the arm. Precisely.

An interactive industrial robotic-arm teach pendant project built
around a physical controller prototype, Raspberry Pi Pico, OLED
interface, buttons, joystick input, USB HID/Serial communication, and a
browser-based teaching interface.

This repository also contains a polished interactive showcase that
demonstrates the complete concept: operator input → controller →
command transport → teaching interface → robot workflow.
<p align="center">
<strong>{=html}🤖 Industrial Robot Teaching
Interface</strong>{=html}<br>{=html} <em>{=html}Physical hardware
• Embedded control • Web HMI • Point teaching</em>{=html}
</p>
🚀 What is TEACH//PENDANT?
TEACH//PENDANT is a robotics control and teaching interface designed
around the workflow used when manually positioning a robotic arm and
recording useful positions.

Instead of treating the project as only a collection of buttons and
microcontroller code, the system is organized around a clear operator
workflow:
┌──────────────────┐
│  Physical Input  │
│ Buttons + Joystick│
└────────┬─────────┘
         ↓
┌──────────────────┐
│ Raspberry Pi Pico│
│ Controller / UI  │
└────────┬─────────┘
         ↓
┌──────────────────┐
│ USB HID / Serial │
│ Command Transport│
└────────┬─────────┘
         ↓
┌──────────────────┐
│ Teaching Web HMI │
│ Jog / Points / UI│
└────────┬─────────┘
         ↓
┌──────────────────┐
│ Robot Workflow   │
│ Arm + Taught Pts │
└──────────────────┘
The project also evolved from an earlier ESP-NOW based prototype,
making the repository a record of the system's development rather than
just the final interface.
✨ Highlights
  Feature                             Description
  🎮 Joint Control                Individual J1--J6 controls for
                                      robotic-arm simulation

  🕹️ Joystick Input               Interactive joystick for intuitive
                                      jogging

  🧭 Cartesian Mode               Demonstrates Cartesian-style X/Y/Z
                                      control

  📍 Point Teaching               Capture and display taught
                                      positions

  🏠 Home Position                Return the simulated arm to a known
                                      pose

  📦 Pick / Place                 Demonstration poses for common
                                      robot workflows

  🛑 Stop Control                 Explicit motion-stop state in the
                                      simulator

  🖥️ OLED Interface               Physical pendant uses an OLED for
                                      operator feedback

  🔌 USB HID / Serial             Controller-to-computer
                                      communication concept

  📡 ESP-NOW Prototype            Earlier wireless prototype
                                      architecture

  🌐 Responsive Web UI            Desktop, tablet and mobile friendly

  ⚡ Zero Framework Demo          Showcase page runs with plain
                                  HTML/CSS/JavaScript
🦾 The Interactive Simulator
The showcase includes an industrial-style robotic-arm simulator rather
than a static illustration.

You can interact with:
- J1 --- Base
- J2 --- Shoulder
- J3 --- Elbow
- J4 --- Wrist
- J5 --- Wrist Pitch
- J6 --- Tool
- Joystick
- HOME
- PICK
- PLACE
- JOINT / CARTESIAN
- TEACH POINT
- STOP
The simulator also exposes:
Motion State
Active Axis
TCP Coordinates
Safety State
Last Command
Taught Points
Try this sequence
HOME
  ↓
PICK
  ↓
TEACH POINT
  ↓
PLACE
  ↓
TEACH POINT
This gives a simple visual demonstration of the idea behind robot
teaching.
🧠 Command Concept
The project uses compact command tokens to represent operator actions.

Examples:
+j1    → positive Joint 1
-j1    → negative Joint 1
0j1    → stop Joint 1

+cx    → positive Cartesian X
-cz    → negative Cartesian Z
0cx    → stop Cartesian X

@p1    → taught point 1
The exact command set can be extended as the robot-side controller
evolves.
🔧 Hardware Prototype
The physical pendant prototype combines:
- Raspberry Pi Pico
- OLED display
- Multiple push buttons
- Joystick input
- Wiring / prototyping board
- USB connection
- Additional control electronics depending on the prototype revision
The project went through more than one hardware revision.
Prototype evolution
Earlier Prototype
ESP-based controller
        │
        ▼
    ESP-NOW
        │
        ▼
ESP32 receiver
        │
        ▼
 Serial communication
        │
        ▼
 Raspberry Pi Pico
        │
        ▼
 Current pendant workflow
The repository is therefore not just a UI demo --- it documents the
transition from an early wireless prototype toward a more structured
controller architecture.
🏗️ Project Architecture
1. Physical Layer
The operator interacts with:
Buttons
Joystick
OLED
The controls represent:
- Joint jogging
- Cartesian jogging
- Mode switching
- Point operations
- Robot workflow actions
2. Embedded Layer
The Raspberry Pi Pico acts as the pendant controller.

Responsibilities include:
Read input
   ↓
Interpret operator action
   ↓
Update OLED
   ↓
Generate control command
3. Communication Layer
The project explores:
USB HID
Serial
ESP-NOW
depending on the prototype/revision.
4. Web HMI
The browser interface provides:
Robot visualisation
Joint controls
Joystick
Teaching points
Status
Commands
5. Robot Layer
The final command path can be connected to the robotic-arm control stack
for:
Jogging
Point teaching
Position playback
Robot workflows
🎨 Showcase Website
The included index.html is a standalone project showcase.

It contains:
- Industrial robot visualisation
- Interactive simulator
- Architecture section
- Physical prototype section
- Software/command section
- Responsive design
- GitHub navigation
Run locally
No build system is required.
git clone https://github.com/i-am-yashgarg/teach-nex.git
cd teach-nex
Then open:
index.html
in a browser.

Or use a local server:
python3 -m http.server 8000
Open:
http://localhost:8000
🌐 Deploy to Netlify
Because the showcase is a static HTML/CSS/JavaScript site, it can be
deployed directly to Netlify.
Option 1 --- Drag & Drop
Upload the project folder containing:
index.html
to Netlify.
Option 2 --- Git-based deployment
Connect this repository:
https://github.com/i-am-yashgarg/teach-nex.git
No Node.js build step is required for the standalone showcase.
📁 Suggested Repository Structure
As the project grows, a clean structure could look like:
teach-nex/
│
├── index.html
│
├── firmware/
│   ├── pico/
│   └── esp32/
│
├── web/
│   ├── js/
│   └── assets/
│
├── hardware/
│   ├── wiring/
│   └── prototype/
│
├── docs/
│   ├── architecture/
│   └── setup/
│
└── README.md
🧪 Current Demonstration vs Physical Robot
Important distinction:
The browser simulator is a visual demonstration of the control
workflow. It is not presented as a live connection to a physical
robot.

The physical hardware and robot-control stack are separate from the
static showcase unless an actual communication bridge is connected.

This keeps the public demo honest while still making the project
understandable and interactive.
🔮 Roadmap
The next logical upgrades are:
- [ ] True 3D robot-arm visualization
- [ ] Proper six-axis kinematics
- [ ] Orbit / zoom camera
- [ ] Live TCP trajectory
- [ ] Taught-point playback
- [ ] Real-time Pico connection
- [ ] Web Serial / WebUSB integration
- [ ] Robot status telemetry
- [ ] Joint-limit visualization
- [ ] Emergency-stop state visualization
- [ ] Real robot controller integration
- [ ] Calibration workflow
- [ ] Recorded motion sequences
🛠️ Technology
Embedded
Raspberry Pi Pico
OLED
Push Buttons
Joystick
USB HID / Serial
Web
HTML5
CSS3
JavaScript
SVG
Prototype Communication
ESP-NOW
ESP32
Serial
USB
The showcase intentionally avoids unnecessary frontend frameworks so
that the demonstration remains lightweight and easy to deploy.
🎯 Why This Project?
Industrial robots are powerful, but teaching them still requires
interfaces that make manual positioning and point recording
understandable for the operator.

TEACH//PENDANT explores a practical question:
Can a low-cost custom controller provide a clear and extensible
interface for teaching a robotic arm?

The project combines:

Embedded systems + robotics + human-machine interface + web
technology

into one system.
👨‍💻 Author
Yash Garg

GitHub:

https://github.com/i-am-yashgarg/teach-nex
