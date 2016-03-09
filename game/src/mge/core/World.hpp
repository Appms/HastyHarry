#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "mge/core/GameObject.hpp"
#include "..\..\include\tokamak.h"

class Light;
class Camera;

class World : public GameObject
{
	public:
        World();
		~World();

		void setMainCamera (Camera* pCamera);
		Camera* getMainCamera();

		void setLights (Light* pLight);
		Light* getLights (int index);

		bool initPhysics();
		neSimulator* getPhysics();
		void killPhysics();

        void renderDebugInfo();

	private:
	    Camera* _mainCamera;
	    Light* _sceneLights;
		neSimulator* _physicsWorld;
		std::vector<neRigidBody*> _rigidbodyVector;
		std::vector<neAnimatedBody*> _animatedbodyVector;
};


#endif // WORLD_H
