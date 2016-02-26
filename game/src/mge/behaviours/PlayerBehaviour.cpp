#include "mge/behaviours/PlayerBehaviour.hpp"
#include "mge/behaviours/SoundBehaviour.hpp"
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
#include "SFML/Audio.hpp"
#include "mge/config.hpp"

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

	//Set audio listener volume
	sf::Listener::setGlobalVolume(50.0f);

	_resetPos = _owner->getPosition();

	_counter = 0;
	_prevPos = _owner->getWorldPosition();
}

PlayerBehaviour::~PlayerBehaviour()
{
}

void PlayerBehaviour::PlayerController(neRigidBodyController* pController, float pStep)
{

	//Capsule rotation (X-Z Locked)
	_angleY += _mouseDelta.x / 90.0f;

	neM3 rotationMatrixY;
	rotationMatrixY[0].Set(cos(_angleY), 0.0f, sin(_angleY));
	rotationMatrixY[1].Set(0.0f, 1.0f, 0.0f);
	rotationMatrixY[2].Set(-sin(_angleY), 0.0f, cos(_angleY));

	pController->GetRigidBody()->SetRotation(rotationMatrixY);

	glm::vec3 camFor = _camera->getForwardVector();
	glm::vec3 bodyFor = _owner->getForwardVector();

	bool camGreater = camFor.y > bodyFor.y ? true : false;

	if (camGreater) {
		if (glm::abs(glm::dot(bodyFor, camFor)) > 0.1) {
			_camera->rotate(glm::radians(-_mouseDelta.y), glm::vec3(1, 0, 0));
		} else if (-_mouseDelta.y < 0.0f) _camera->rotate(glm::radians(-_mouseDelta.y), glm::vec3(1, 0, 0));
	}
	else {
		if (glm::abs(glm::dot(bodyFor, camFor)) > 0.1) {
			_camera->rotate(glm::radians(-_mouseDelta.y), glm::vec3(1, 0, 0));
		} else if (-_mouseDelta.y > 0.0f) _camera->rotate(glm::radians(-_mouseDelta.y), glm::vec3(1, 0, 0));
	} 

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

	if (!_holdingJump && grounded && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		GameObject* sound = new GameObject("Sound");
		_owner->add(sound);
		sound->setBehaviour(new SoundBehaviour("jumpSound", _owner->getWorldPosition(), true, true));

		_speedVector.y = _jumpForce;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		_holdingJump = true;
	}
	else
	{
		_holdingJump = false;
	}

	glm::vec3 momentum = glm::vec3(0.0f, pController->GetRigidBody()->GetAngularMomentum().Y(), 0.0f);

	//Set dem velocitys
	//TODO Get the forward and right vector to apply translation in the right direction
	pController->GetRigidBody()->SetVelocity(glmToNe(_speedVector));
	
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

	sf::Listener::setPosition(_owner->getWorldPosition().x, _owner->getWorldPosition().y, _owner->getWorldPosition().z);
	sf::Listener::setDirection(_owner->getForwardVector().x, _owner->getForwardVector().y, -_owner->getForwardVector().z);

	if (_owner->getWorldPosition().y <= -1.0f && !_dead)
	{
		_dead = true;
		GameObject* sound = new GameObject("Sound");
		_owner->add(sound);
		sound->setBehaviour(new SoundBehaviour("fallingSound", _owner->getWorldPosition(), true, true));
		_timer = 0.0f;
	}

	if (!_dead)
	{
		if (glm::length(_prevPos - _owner->getWorldPosition()) <= 4.0f)
		{
			_timer += pStep;
		}
		else
		{
			_prevPos = _owner->getWorldPosition();
			_timer = 0.0f;
		}

		std::string sounds[2] = {
			"Come on slowpoke",
			"Take your time"
		};

		if (_timer > 6.0f)
		{
			GameObject* sound = new GameObject("Sound");
			_owner->add(sound);
			sound->setBehaviour(new SoundBehaviour(sounds[_counter], _owner->getWorldPosition(), true, true));
			_timer = 0.0f;
			_counter++;
			if (_counter > 1) _counter = 0;
		}
	}
	else
	{
		_timer += pStep;

		std::string sounds[2] = {
			"did you seriously just miss that",
			"please youve made this mistake before"
		};

		if (_timer > 3.0f)
		{
			std::cout << _resetPos << std::endl;
			_owner->getRigidBody()->SetVelocity(glmToNe(glm::vec3(0,0,0)));;
			_owner->getRigidBody()->SetPos(glmToNe(_resetPos));;

			GameObject* sound = new GameObject("Sound");
			_owner->add(sound);
			sound->setBehaviour(new SoundBehaviour(sounds[_counter], _owner->getWorldPosition(), true, true));

			_timer = 0.0f;
			_counter++;
			if (_counter > 1) _counter = 0;

			_dead = false;
		}
	}
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
