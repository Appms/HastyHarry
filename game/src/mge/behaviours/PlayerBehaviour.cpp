#include "mge/behaviours/PlayerBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Camera.hpp"
#include <sfml/window/event.hpp>
#include <iostream>
#include "..\..\include\tokamak.h"
#include <glm/gtx/matrix_decompose.hpp>

PlayerBehaviour::PlayerBehaviour(Camera* pCamera, float pWalkSpeed, float pRotateSpeed, float pJumpForce) : AbstractBehaviour(), _camera(pCamera), _walkSpeed(pWalkSpeed), _rotateSpeed(pRotateSpeed), _jumpForce(pJumpForce)
{
	_camera->setParent(_owner);
	//_camera->setLocalPosition(glm::vec3(0, 0, 0));
}

PlayerBehaviour::~PlayerBehaviour()
{
}

void PlayerBehaviour::update(float pStep)
{
	glm::vec3 walkSpeed;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		walkSpeed.z = _walkSpeed * pStep;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		walkSpeed.z = -_walkSpeed * pStep;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		walkSpeed.x = _walkSpeed * pStep;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		walkSpeed.x = -_walkSpeed * pStep;
	}

	glm::vec3 yVel = toGLM(_owner->GetRigidBody()->GetVelocity());
	_owner->GetRigidBody()->SetVelocity(toTokamak(glm::vec3(walkSpeed.x, yVel.y, walkSpeed.z)));
	//_owner->GetRigidBody()->ApplyImpulse(toTokamak(glm::vec3(walkSpeed.x, 0.0f, walkSpeed.z)));
	//_owner->GetRigidBody()->SetAngularMomentum(toTokamak(glm::vec3(0, 0, 0)));

	neV3 pos;
	neQ rot;

	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(_owner->getTransform(), scale, rotation, translation, skew, perspective);

	pos[0] = translation[0];
	pos[1] = translation[1];
	pos[2] = translation[2];

	rot.X = rotation.x;
	rot.Y = rotation.y;
	rot.Z = rotation.z;
	rot.W = rotation.w;

	_owner->GetRigidBody()->SetRotation(rot);

	/*
	neV3 zero;

	zero = _owner->GetRigidBody()->GetAngularMomentum();
	zero[0] = 0;
	zero[2] = 0;
	_owner->GetRigidBody()->SetAngularMomentum(zero);
	*/

	//std::cout << toGLM(_owner->GetRigidBody()->GetAngularVelocity()) << std::endl;
}

neV3 PlayerBehaviour::toTokamak(glm::vec3 v)
{
	neV3 vec;
	vec[0] = v[0];
	vec[1] = v[1];
	vec[2] = v[2];
	return vec;
}

glm::vec3 PlayerBehaviour::toGLM(neV3 v)
{
	return glm::vec3(v[0], v[1], v[2]);
}