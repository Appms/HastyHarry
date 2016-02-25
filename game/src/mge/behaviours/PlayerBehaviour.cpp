#include "mge/behaviours/PlayerBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Camera.hpp"
#include <sfml/window/event.hpp>
#include <iostream>
#include "..\..\include\tokamak.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <SFML/window.hpp>
#include "mge/PlayerControllerCB.hpp"
#include "mge/core/World.hpp"
#include "mge/core/GameObject.hpp"

PlayerBehaviour::PlayerBehaviour(Camera* pCamera, float pWalkForce, float pMaxVelocity, float pRotateSpeed, float pJumpForce) : AbstractBehaviour()
{
	_camera = pCamera;
	_walkForce = pWalkForce;
	_maxVelocity = pMaxVelocity;
	_rotateSpeed = pRotateSpeed;
	_jumpForce = pJumpForce;

	_prevMousePos = sf::Mouse::getPosition();
}

void PlayerBehaviour::Initialize()
{
	f32 mass = 1.0f;
	f32 height = 1.0f;
	f32 radius = 0.5f;

	neV3 sensorPos, sensorDir;
	sensorPos.Set(0.0f, -1.0f, 0.0f);
	sensorDir.Set(0.0f, 1.0f, 0.0f);

	//Request a rigidbody from Physicsworld
	if (_owner->getRigidBody() != NULL) _owner->GetWorld()->getPhysics()->FreeRigidBody(_owner->getRigidBody());
	neRigidBody* rigidBody = _owner->GetWorld()->getPhysics()->CreateRigidBody();

	//Add geometry to it and set it up
	neGeometry* geometry = rigidBody->AddGeometry();
	geometry->SetCylinder(radius, height);

	//Create material and apply it
	_owner->GetWorld()->getPhysics()->SetMaterial(0, 0.0f, 0.0f);
	geometry->SetMaterialIndex(0);

	//Add a sensor for groundcheck
	neSensor* sensor;
	sensor = rigidBody->AddSensor();
	sensor->SetLineSensor(sensorPos, sensorDir);

	//Add a controller callback
	neRigidBodyController* controller;
	controller = rigidBody->AddController(&_playerControllerCB, 0);

	//Update dat Bounds
	rigidBody->UpdateBoundingInfo();

	//Set Physics Propertys
	rigidBody->SetInertiaTensor(neCylinderInertiaTensor(radius, height, mass));
	rigidBody->SetMass(mass);
	rigidBody->SetUserData((u32)this);

	//Parse pos and rot info to Tokamak
	glm::vec3 pos;
	glm::quat rot;
	pos = _owner->getPosition();
	rot = _owner->getRotation();
	neV3 nPos;
	neQ nRot;
	nPos.Set(pos.x, pos.y, pos.z);
	nRot.Set(rot.x, rot.y, rot.z, rot.w);

	rigidBody->SetPos(nPos);
	rigidBody->SetRotation(nRot);

	//Set the created rigidbody as the players rigidbody
	_owner->setRigidBody(rigidBody);

	//Set Camera as child and set position
	_camera->setParent(_owner);
	_camera->setLocalPosition(glm::vec3(0, 0, 0));
}

PlayerBehaviour::~PlayerBehaviour()
{
}

void PlayerBehaviour::PlayerController(neRigidBodyController* pController, float pStep)
{
	//TODO Add body rotation over y axis (without physics?)
	pController->GetRigidBody()->SetAngularMomentum(glmToNe(glm::vec3(0.0f, glm::radians(-_mouseDelta.x), 0.0f)));
	//pController->GetRigidBody()->SetAngularMomentum(glmToNe(glm::vec3(0.0f, glm::radians(-0.5f), 0.0f)));

	//TODO Add camera rotation over xz plane
	_camera->rotate(glm::radians(-_mouseDelta.y), glm::vec3(1, 0, 0));


	pController->GetRigidBody()->BeginIterateSensor();
	neSensor* sensor;
	bool grounded = false;

	while (sensor = pController->GetRigidBody()->GetNextSensor())
	{
		//std::cout << sensor->GetDetectDepth() << std::endl;

		if (sensor->GetDetectDepth() > 1.0f)
		{
			grounded = true;
		}
	}



	_speedVector = glm::vec3(0, 0, 0);

	//Get Inputs
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		_speedVector += glm::vec3(_owner->getForwardVector().x, 0.0f, -_owner->getForwardVector().z);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		_speedVector -= glm::vec3(_owner->getForwardVector().x, 0.0f, -_owner->getForwardVector().z);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		_speedVector += glm::vec3(_owner->getRightVector().x, 0.0f, -_owner->getRightVector().z);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		_speedVector -= glm::vec3(_owner->getRightVector().x, 0.0f, -_owner->getRightVector().z);
	}

	//Normalize and apply appropriate speed
	if (glm::length(_speedVector) > 0.0f)
	{
		_speedVector = glm::normalize(_speedVector) * _walkForce * pStep;
	}

	//Add current Y velocity unchanged
	_speedVector.y = pController->GetRigidBody()->GetVelocity()[1];

	if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		_speedVector.y = _jumpForce;
	}

	//Set dem velocitys
	//TODO Get the forward and right vector to apply translation in the right direction
	pController->GetRigidBody()->SetVelocity(glmToNe(_speedVector));

	/*---------------NOTES-----------------

	- Maybe update rigidbody when rotate
	and translate function of a GameObject
	are called (check animatedbody??)

	- Add handy functions to GameObject 
	would be neat (get scale, rot, pos)

	- Add a function to set the rotation
	of a GameObject

	-------------------------------------*/

	//TODO Freeze rotation on x and z axis
}

void PlayerBehaviour::update(float pStep)
{
	//Calculate the mouse Vector
	_currMousePos = sf::Mouse::getPosition();
	_mouseDelta = glm::vec2(_currMousePos.x, _currMousePos.y) - glm::vec2(_prevMousePos.x, _prevMousePos.y);
	_mouseDelta *= _rotateSpeed;

	//Keep the mouse from leaving the center
	int screenHeight = sf::VideoMode::getDesktopMode().height;
	int screenWidth = sf::VideoMode::getDesktopMode().width;
	sf::Mouse::setPosition(sf::Vector2i(screenWidth / 2, screenHeight / 2));

	//Update previous mouse position
	_prevMousePos = sf::Mouse::getPosition();
}

neV3 PlayerBehaviour::glmToNe(glm::vec3 v)
{
	neV3 result;
	result.Set(v.x, v.y, v.z);
	return result;
}

glm::vec3 PlayerBehaviour::neToGlm(neV3 v)
{
	return glm::vec3(v[0], v[1], v[2]);
}
