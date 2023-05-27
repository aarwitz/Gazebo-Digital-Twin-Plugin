#include "../third-party/include/serial/serial.h"
#include <iostream>
#include "thread"
#include <fstream>
#include "socket.hpp"


float velocity = 2.0;

bool rcv_msg = false;

void arduinoThread()
{
    serial::Serial my_serial("/dev/ttyACM0", 19200, 			serial::Timeout::simpleTimeout(3000));

    if (my_serial.isOpen())
    {
        std::cout << "Port opened succesfully" << std::endl;
    }
    else
    {
        std::cout << "Port failed to open" << std::endl;
    }
   // my_serial.flushOutput();

    // Creating a file to write Arduino data into
    //std::ofstream MyFile("../ArduinoData.txt");
	

  //  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    int count = 100000000;
    while (count>0) {
        count--;
        my_serial.flushInput();
        std::string response = my_serial.read(6);
        std::cout << "Arduino: " << response << std::endl;
      //  std::cout << "Hellloooo?" << response;
       // std::cout<< "" << typeid(response).name();
      //  MyFile << response;
       // velocity = std::stof(response);
        // To convert into a float:
        // std::cout << std::stof(response) * 5 << std::endl;
        double f = 0.0;    
		std::stringstream ss;
		//std::string s = "213.1415";    
		ss << response;
		ss >> f;  //f now contains the converted string into a double  
		std::cout << "5*f = " << 5*f << std::endl; 
		velocity = f;
    }

    //MyFile.close();
}

void gazeboThread() {
    std::cout << "Starting Thread 1\n";
	
	// Connect to client on port 12345
	Socket::Ptr server = std::make_shared<Socket>("127.0.0.1",12345);
	server->socket_connect();

	// Send a string to client
	//std::string send_str = "Hello prease";
    while(true) {
        sleep(.5);
        std::string send_str = std::to_string(velocity);
        server->socket_send(send_str);
    }
	
	// Receive a string from client
	//std::string output = server->socket_receive();
	//std::cout << "Client>" << output << std::endl;

}



int main() {
    std::thread t1(arduinoThread);
    std::thread t2(gazeboThread);
    t1.join();
    t2.join();
    return 0;
}
