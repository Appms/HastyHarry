#ifndef PLAYERCONTROLLERCB_H
#define PLAYERCONTROLLERCB_H

#include <glm.hpp>
#include "..\..\include\tokamak.h"
#include <SFML/window.hpp>

class PlayerControllerCB : public neRigidBodyControllerCallback
{
public:
	void RigidBodyControllerCallback(neRigidBodyController* controller, float timeStep);
};

#endif