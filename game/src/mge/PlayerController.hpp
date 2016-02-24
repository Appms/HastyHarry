#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <glm.hpp>
#include "..\..\include\tokamak.h"
#include <SFML/window.hpp>

class PlayerController : public neRigidBodyControllerCallback
{
public:
	virtual void RigidBodyControllerCallback(neRigidBodyController * controller, float timeStep);

	neV3 toTokamak(glm::vec3 v)
	{
		neV3 vec;
		vec[0] = v[0];
		vec[1] = v[1];
		vec[2] = v[2];
		return vec;
	};

	glm::vec3 toGLM(neV3 v)
	{
		return glm::vec3(v[0], v[1], v[2]);
	};
};

#endif