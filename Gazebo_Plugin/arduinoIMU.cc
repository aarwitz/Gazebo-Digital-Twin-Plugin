#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>
#include "socket.hpp"
#include <ignition/math.hh>

namespace gazebo
{
  class ArduinoIMU : public ModelPlugin
  {
    // Create sharedptr to socket that connects to arduino output
  	Socket::Ptr m_client = std::make_shared<Socket>("127.0.0.1",12345);
  
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
    {
		// Store the pointer to the model
		this->model = _parent;

    	// Listen to the update event. This event is broadcast every
    	// simulation iteration.
    	this->updateConnection = event::Events::ConnectWorldUpdateBegin(
          std::bind(&ArduinoIMU::OnUpdate, this));
      	// Connect to m_client
      	m_client->socket_connect();
     
    }

    // Called by the world update start event
    public: void OnUpdate()
    {
		// Receive a string from m_client
		std::string output = m_client->socket_receive();
		// Create RPY vector from string, each element split by commas
		std::stringstream ss(output);
		std::vector<std::string> v;
		while (ss.good()) {
			std::string substr;
			std::getline(ss, substr, ',');
			v.push_back(substr);
		}
		// Define roll, pitch, yaw w.r.t world frame from RPY vector
		double roll_w = std::stod(v[0]);
		double pitch_w = std::stod(v[1]);
		double yaw_w = std::stod(v[2]);
		// Print out values
		std::cout << "roll: " << roll_b << " pitch: " << pitch_b << " yaw: " << yaw_b << std::endl;
		// Set the RPY of the Gazebo model
		ignition::math::Pose3d pose(0.0, 0.0, 5.0, roll_b*.01745, pitch_b*.01745, yaw_b*.01745);
		this->model->SetWorldPose(pose);
    }

    // Pointer to the model
    private: physics::ModelPtr model;

    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(ArduinoIMU)
}
