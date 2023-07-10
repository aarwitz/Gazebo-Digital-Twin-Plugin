Hardware: Arduino, Adafruit BNO055 9-Axis IMU

<img src="https://github.com/aarwitz/Gazebo-Digital-Twin-Plugin/blob/main/Quad.gif" width="400" height="710.33" />

***Add instructions up here for building client/server

```
git clone https://github.com/aarwitz/Gazebo-Digital-Twin-Plugin.git
```
In Arduino, run the code contained in SerialCom/src/ArduinoCode.txt

Build the SerialCom code to start streaming pose coordinates from Arduino to TCP/IP:
```
cd SerialCom
mkdir build
cd build
cmake -S ../src -B .
make -j${nproc}
```
Before running the executable, run command to give user permission to arduino port
```
sudo chmod a+rw /dev/ttyACM1
```

Then run the executable:
```
./ArduinoSerial
```

Now build Gazebo Plugin to take in poses from TCP/IP
```
cd Gazebo_Plugin
mkdir build
cd build
cmake -S ../ -B .
make -j${nproc}
```

Now we can run gzserver and gzclient
In one terminal source your plugin build: ```export GAZEBO_PLUGIN_PATH=${GAZEBO_PLUGIN_PATH}:~/Gazebo-Digital-Twin-Plugin/Gazebo_Plugin/build```

Then run gzserver with pause -u option: ```gzserver -u Quad.world```

In a second terminal, source your plugin build as done above and then run: ```gzclient```
