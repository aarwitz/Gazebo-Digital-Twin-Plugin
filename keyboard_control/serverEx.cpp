#include <iostream>
#include "socket.hpp"
// server
#include <conio.h>

#define KEY_UP 119
#define KEY_DOWN 115
#define KEY_LEFT 97
#define KEY_RIGHT 100



float velocity = 5.0;


float keyboard_to_linear_vel(int key) {
	std::cout << "\n\n";
	std::cout << key << std::endl;
	std::cout << "\n\n";
	switch(key) {
	case KEY_UP:
		std::cout << "Up" << std::endl;
		return 5.0;
	case KEY_DOWN:
		std::cout << "Down" << std::endl;
		return -5.0;
	// Takes in a key (up,down,right,left) and outputs a linear velocity in m/s
	}
	return 0;
}



void thread_1()
{
	std::cout << "Starting Thread 1\n";
	
	// Connect to client on port 12345
	Socket::Ptr server = std::make_shared<Socket>("127.0.0.1",12345);
	server->socket_connect();

	// Send a string to client
	while(true) {
		sleep(.1);
		std::string send_str = std::to_string(velocity);
		server->socket_send(send_str);
	}
	
	// Receive a string from client
//	std::string output = server->socket_receive();
	//std::cout << "Client>" << output << std::endl;
	
}


void thread_input() {
	char key;
	int key_value;
	while(true) {
		key = getch();
		key_value = key;
		velocity = keyboard_to_linear_vel(key_value);
	}
}



int main() {
	std::thread t1(thread_1);
	std::thread t2(thread_input);
	
	t1.join();
	t2.join();
	return 0;
}
