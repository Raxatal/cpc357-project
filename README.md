<h1>Smart Drainage Blockage Monitoring System</h1>

<p>
This project presents an Internet of Things (IoT) based Smart Drainage Blockage Monitoring System
developed as part of <strong>CPC357: IoT Architecture & Smart Applications</strong>.
The system is designed to address urban drainage challenges by providing real-time monitoring,
cloud-based data storage, and web-based visualisation, supporting the goals of
<strong>UN Sustainable Development Goal (SDG) 11 – Sustainable Cities and Communities</strong>.
</p>

<hr>

<h2>Project Overview</h2>

<p>
Urban drainage blockages can lead to flash floods, infrastructure damage, and public safety risks.
Traditional inspection methods are manual and reactive, often identifying problems only after
failures occur. This project proposes a smart, automated solution that continuously monitors
drainage conditions using IoT sensors.
</p>

<p>
The system uses an ultrasonic distance sensor to detect water level changes that indicate blockage
conditions, and a flow sensor to measure water flow rate for analytical insights. Sensor data is
processed by a microcontroller and transmitted via Wi-Fi to a Firebase Realtime Database. A web
dashboard displays real-time readings, system status, and historical analytics.
</p>

<hr>

<h2>System Features</h2>
<ul>
  <li>Real-time blockage detection using distance thresholds</li>
  <li>Flow rate monitoring for trend analysis and maintenance insights</li>
  <li>Visual and audible alerts using LEDs and an active buzzer</li>
  <li>Cloud-based data storage with Firebase Realtime Database</li>
  <li>Web dashboard for live monitoring and historical analytics</li>
</ul>

<hr>

<p>
<strong>Integrated_System_v3.ino</strong> contains the main firmware that integrates sensor readings,
blockage detection logic, alert handling, and cloud communication.
</p>

<p>
<strong>FlowCalibration.ino</strong> is a supporting sketch used during the experimental calibration
process to determine the appropriate flow sensor calibration factor based on the physical setup.
</p>

<hr>

<h2>System Architecture</h2>

<img width="560" height="896" alt="CPC357-CPC357PSA drawio" src="https://github.com/user-attachments/assets/acc77ce4-f092-4a98-9adc-898a6b0308da" />

<hr>

<h2>Hardware Requirements</h2>
<ul>
  <li>Microcontroller with Wi-Fi capability (e.g., ESP32 / Maker Feather S3)</li>
  <li>Ultrasonic Distance Sensor (HC-SR04)</li>
  <li>Water Flow Sensor (YF-S401)</li>
  <li>Red LED (Critical alert)</li>
  <li>Yellow LED (Partial blockage alert)</li>
  <li>Active Buzzer Module</li>
  <li>Resistors (for LEDs)</li>
  <li>Submersible USB water pump with tubing</li>
  <li>Breadboard and jumper wires</li>
  <li>USB power source</li>
</ul>

<hr>

<h2>Software Requirements and Dependencies</h2>
<ul>
  <li>Arduino IDE</li>
  <li>Board support package for selected microcontroller</li>
  <li>Wi-Fi library (built-in for ESP32)</li>
  <li>Firebase Realtime Database</li>
  <li>Web browser (Chrome, Edge, or Firefox)</li>
  <li>Code editor (Visual Studio Code recommended)</li>
</ul>

<hr>

<h2>Setup and Installation Instructions</h2>

<h3>1. Hardware Setup</h3>

<img width="442" height="445" alt="Picture1" src="https://github.com/user-attachments/assets/6ec63535-d70d-4276-86cf-2c1f49be8708" />

<pre>
  <b>Power Connections (The Rails)</b>
Maker GND -> Breadboard Negative Power Rail (left)
Breadboard Negative Power Rail (left) -> Breadboard Negative Power Rail (right)
Maker 3V3 -> Breadboard Positive Power Rail (left)
Maker USB -> Breadboard Positive Power Rail (right) 
Reminder: Do not connect Left and Right Red rails together
  <b>Ultrasonic Sensor (HC-SR04)</b>
Ultrasonic Sensor (via Qwiic Cable) -> Maker Feather Port (labeled 21/14)
  <b>Water Flow Sensor (YF-S401)</b>
Flow Sensor Red Wire -> Breadboard Positive Power Rail (right) (5V)
Flow Sensor Black Wire -> Breadboard Negative Power Rail (right)
Flow Sensor Yellow Wire -> Maker Pin A2
  <b>LEDs & Buzzer (Alerts)
  Yellow LED (Warning)</b>
Yellow LED Long Leg (+) -> Breadboard Row 15
Breadboard Row 15 -> Maker Pin A4
Yellow LED Short Leg (-) -> Breadboard Row 16
Resistor (from Row 16) -> Breadboard Negative Power Rail (left)
  <b>Red LED (Critical)</b>
Red LED Long Leg (+) -> Breadboard Row 20
Breadboard Row 20 -> Maker Pin A5
Red LED Short Leg (-) -> Breadboard Row 21
Resistor (from Row 21) -> Breadboard Negative Power Rail (left)
  <b>Buzzer Module</b>
Buzzer VCC -> Breadboard Row 25 -> Breadboard Positive Power Rail (left) (3.3V)
Buzzer I/O -> Breadboard Row 26 -> Maker Pin 47
Buzzer GND -> Breadboard Row 27 -> Breadboard Negative Power Rail (left)
</pre>

<h3>2. Firmware Setup</h3>
<ul>
  <li>Open <strong>Integrated_System_v3.ino</strong> using Arduino IDE.</li>
  <li>Select the correct board and COM port.</li>
  <li>Update Wi-Fi credentials and Firebase database URL in the code.</li>
  <li>Upload the sketch to the microcontroller.</li>
</ul>

<h3>3. Cloud Setup</h3>
<ul>
  <li>Create a Firebase Realtime Database project.</li>
  <li>Set read/write rules appropriately for testing.</li>
  <li>Ensure database nodes match the structure expected by the firmware and web application.</li>
</ul>

<h3>4. Web Dashboard Setup</h3>
<ul>
  <li>Open the <strong>web</strong> folder.</li>
  <li>Ensure Firebase database URL is correctly set in <strong>app.js</strong> and <strong>analytics.js</strong>.</li>
  <li>Open <strong>index.html</strong> in a web browser.</li>
</ul>

<hr>

<h2>Data Visualization</h2>

<p>
The web dashboard displays real-time sensor readings, including distance, flow rate, and system
status. An analytics page provides historical charts and a table of recent readings, allowing
users to observe trends and potential early warning signs even when no immediate blockage is detected.
Users can switch between these two interfaces via a navigation button.
</p>

<hr>

<h2>SDG 11 Contribution</h2>

<p>
This project supports <strong>SDG 11: Sustainable Cities and Communities</strong>, particularly
<strong>Target 11.5</strong>, by contributing to disaster risk reduction through early detection
of drainage blockages. By enabling proactive monitoring, the system helps reduce flood risks,
improve urban resilience, and support smarter infrastructure management.
</p>
