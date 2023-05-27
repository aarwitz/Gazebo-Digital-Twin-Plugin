#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>
#include "socket.hpp"
#include <ignition/math.hh>
bool simFinished = false;

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


    // Use while loop to get rpy out of the streamed string
	std::stringstream ss(output);
	std::vector<float> rpy;
	std::string s;
	std::cout << "Output: ";
    while (getline(ss, s, ' ')) {
   //   std::cout << s << "   ";
      rpy.push_back(std::stod(s));
    }
    for (int i=0;i<=3;i++) {
    	std::cout << i << ": ";
    	std::cout << rpy[i];
    	std::cout << " "; }
	std::cout << "\n";
     ignition::math::Pose3d pose(0.0, 0.0, 5.0, rpy[0], rpy[1], rpy[2]);
	 this->model->SetWorldPose(pose);
    
    	//std::cout << rpy[0] << " " << rpy[1] << " " << rpy[2];
     /* 
     ignition::math::Vector3d angularvel_vec(0,0,.2);
     ignition::math::Vector3d linearvel_vec(rpy[0],rpy[1],rpy[2]);
      
      this->model->SetAngularVel(angularvel_vec);
      this->model->SetLinearVel(linearvel_vec);
      */
    }

    // Pointer to the model
    private: physics::ModelPtr model;

    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(ArduinoIMU)
}
