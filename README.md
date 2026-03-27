# Gazebo-Digital-Twin-Plugin

Gazebo plugin and serial/TCP bridge for a quadcopter digital twin.

This project mirrors orientation data from a physical IMU-driven setup into Gazebo in real time. The intended workflow is:

**Arduino + BNO055 IMU -> serial bridge -> TCP socket -> Gazebo ModelPlugin**

<img src="Quad.gif" width="400" alt="Gazebo digital twin demo" />

## What this repo includes
- `Gazebo_Plugin/` — Gazebo `ModelPlugin` that receives roll/pitch/yaw over a socket and updates the simulated quad pose
- `SerialCom/` — C++ bridge that reads Arduino serial output and forwards it over TCP/IP
- `keyboard_control/` — supporting keyboard-control experiments
- `Quad.world` / `quad2.stl` — world/model assets used for the demo

## Architecture
1. The Arduino streams orientation values over USB serial.
2. The serial bridge parses those values and forwards them on `127.0.0.1:12345`.
3. The Gazebo plugin receives the roll/pitch/yaw data.
4. The plugin updates the model pose in simulation, creating a hardware-in-the-loop digital twin visualization.

## Build and run
Clone the repo:

```bash
git clone https://github.com/aarwitz/Gazebo-Digital-Twin-Plugin.git
cd Gazebo-Digital-Twin-Plugin
```

### 1) Stream pose data from Arduino
Upload the Arduino-side code used by your hardware setup, then build the serial bridge:

```bash
cd SerialCom
mkdir -p build
cd build
cmake -S ../ -B .
make -j"$(nproc)"
```

Depending on your system, you may need serial-port permissions before running:

```bash
sudo chmod a+rw /dev/ttyACM1
```

Run the bridge:

```bash
./ArduinoSerial
```

### 2) Build the Gazebo plugin
In a separate terminal:

```bash
cd Gazebo_Plugin
mkdir -p build
cd build
cmake -S ../ -B .
make -j"$(nproc)"
```

Export the plugin path:

```bash
export GAZEBO_PLUGIN_PATH=${GAZEBO_PLUGIN_PATH}:~/Gazebo-Digital-Twin-Plugin/Gazebo_Plugin/build
```

### 3) Launch Gazebo
Start the server paused:

```bash
gzserver -u Quad.world
```

Then launch the client in another terminal:

```bash
gzclient
```

## Notes
- This repo is an experimental project repo, not a packaged Gazebo distribution.
- The plugin currently expects the data format emitted by the paired serial bridge.
- For the flight-controller side of this project, see [`aarwitz/Quadcopter-PID-Controller`](https://github.com/aarwitz/Quadcopter-PID-Controller).
