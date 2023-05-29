#include "../third-party/include/serial/serial.h"
#include <iostream>
#include "thread"
#include <fstream>
#include "socket.hpp"

std::string orientation{"000.00,000.00,000.00"};


size_t checkNewLine(std::string str) {
	size_t pos = std::string::npos; // Initialize position to a special value indicating not found

    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '\n') {
            pos = i; // Store the position of the newline character
            break;  // Exit the loop once the newline character is found
        }
    }

    if (pos != std::string::npos) {
  //      std::cout << "Newline found at position " << pos << std::endl;
    } else {
   //     std::cout << "Newline not found" << std::endl;
    }
	return pos;
}


void arduinoThread()
{
    serial::Serial my_serial("/dev/ttyACM1", 19200, serial::Timeout::simpleTimeout(3000));

    if (my_serial.isOpen())
    {
        std::cout << "Arduino port opened succesfully" << std::endl;
    }
    else
    {
        std::cout << "Arduino port failed to open" << std::endl;
    }
	
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    while (true) {
    	//std::this_thread::sleep_for(std::chrono::milliseconds(500));
        my_serial.flushInput();
        std::string str_test = " ";
        while (str_test[0]!='\n') {
        	for (size_t i = 0; i < str_test.length(); ++i) {
        		if (str_test[i] == '\n') {
        			break;
    			}
    			else {
    				str_test = my_serial.read();
    				//std::cout << str_test[0];
    			}
			}
        }
        std::string response = my_serial.read(20);
        orientation = response;
        size_t newline_pos = checkNewLine(orientation);
        if (newline_pos != std::string::npos) {
		    orientation.erase(newline_pos,1);// Remove the newline character at position pos
    	}
        std::cout << "Orientation: " << orientation << std::endl;
    }
}

void gazeboThread() {
	// Connect to client on port 12345
	Socket::Ptr server = std::make_shared<Socket>("127.0.0.1",12345);
	server->socket_connect();
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    	std::cout << "Arduino: " << orientation << std::endl;
        server->socket_send(orientation);
    }
}


int main() {
    std::thread t1(arduinoThread);
    std::thread t2(gazeboThread);
    t1.join();
    t2.join();
    return 0;
}
