#include "mge/behaviours/PlayerBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Camera.hpp"
#include <sfml/window/event.hpp>
#include <iostream>
#include "..\..\include\tokamak.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <SFML/window.hpp>
#include "mge/PlayerController.hpp"

PlayerBehaviour::PlayerBehaviour(Camera* pCamera, float pWalkForce, float pMaxVelocity, float pRotateSpeed, float pJumpForce) : AbstractBehaviour(), _camera(pCamera), _walkForce(pWalkForce), _maxVelocity(pMaxVelocity), _rotateSpeed(pRotateSpeed), _jumpForce(pJumpForce)
{
	//_controllerCallback = new PlayerController();

	_prevMousePos = sf::Mouse::getPosition();
	//_camera->setLocalPosition(glm::vec3(0, 0, 0));
	//_owner->getRigidBody()->AddController(&_controllerCallback, 0);
}

PlayerBehaviour::~PlayerBehaviour()
{
}

void PlayerBehaviour::update(float pStep)
{
	_speedVector = glm::vec3(0, 0, 0);
	bool pressedKey = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		_speedVector.z = -1.0;
		pressedKey = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		_speedVector.z = 1.0;
		pressedKey = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		_speedVector.x = 1.0;
		pressedKey = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		_speedVector.x = -1.0;
		pressedKey = true;
	}

	if (pressedKey) {
		_speedVector = glm::normalize(_speedVector) * _walkForce * pStep;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		_speedVector.y = _jumpForce * pStep;
		pressedKey = true;
	}

	//if(pressedKey)
	if(pressedKey)
	{
		neV3 curVel = _owner->getRigidBody()->GetVelocity();
		curVel[2] = 0.0f;

		if (curVel.Length() < _maxVelocity)
		{
			_owner->getRigidBody()->ApplyImpulse(toTokamak(_speedVector));
		}
	}

	//std::cout << _owner->getRigidBody()
	
	_owner->getRigidBody()->SetTorque(toTokamak(glm::vec3(0.0, 0.0, 0.0)));
	_owner->getRigidBody()->SetAngularMomentum(toTokamak(glm::vec3(0.0, 0.0, 0.0)));
	_owner->getRigidBody()->SetRotation(neQ(0.0f, 0.0f, 0.0f, 1.0f));


	sf::Vector2i curMousePos = sf::Mouse::getPosition();
	glm::vec2 mouseVector = glm::vec2(curMousePos.x, curMousePos.y) - glm::vec2(_prevMousePos.x, _prevMousePos.y);

	int screenHeight = sf::VideoMode::getDesktopMode().height;
	int screenWidth = sf::VideoMode::getDesktopMode().width;
	sf::Mouse::setPosition(sf::Vector2i(screenWidth / 2, screenHeight / 2));
	_prevMousePos = sf::Mouse::getPosition();

	mouseVector *= _rotateSpeed;
}

neV3 PlayerBehaviour::toTokamak(glm::vec3 v)
{
	neV3 vec;
	vec[0] = v[0];
	vec[1] = v[1];
	vec[2] = v[2];
	return vec;
};

glm::vec3 PlayerBehaviour::toGLM(neV3 v)
{
	return glm::vec3(v[0], v[1], v[2]);
};