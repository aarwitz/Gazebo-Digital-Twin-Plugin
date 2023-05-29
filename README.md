
git clone https://github.com/aarwitz/Gazebo-Digital-Twin-Plugin.git
In Arduino, run the code contained in SerialCom/src/ArduinoCode.txt

First, build the SerialCom code to start streaming from Arduino serial:
cd SerialCom
mkdir build
cd build
cmake -S ../src -B .
make -j${nproc}
Before running the executable, run command to give user permission to arduino port
sudo chmod a+rw /dev/ttyACM1
Then run the executable:
./ArduinoSerial

Open a new terminal to start gazebo
cd Gazebo_Plugin
mkdir build
cd build
cmake -S ../ -B .
make -j${nproc}

Now we can run gzserver and gzclient
In one terminal source your plugin build: export GAZEBO_PLUGIN_PATH=${GAZEBO_PLUGIN_PATH}:~/Gazebo-Digital-Twin-Plugin/Gazebo_Plugin/build
Then run gzserver with pause -u option: gzserver -u Quad.world
In a second terminal, source your plugin build as done above and then run: gzclient