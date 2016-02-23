#include "mge/behaviours/PlayerBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Camera.hpp"
#include <sfml/window/event.hpp>
#include <iostream>

PlayerBehaviour::PlayerBehaviour(Camera* pCamera, float pWalkSpeed, float pRotateSpeed, float pJumpForce) : AbstractBehaviour(), _camera(pCamera), _walkSpeed(pWalkSpeed), _rotateSpeed(pRotateSpeed), _jumpForce(pJumpForce)
{
	_camera->setParent(_owner);
	_camera->setLocalPosition(glm::vec3(0, 0, 0));
}

PlayerBehaviour::~PlayerBehaviour()
{
}

void PlayerBehaviour::update(float pStep)
{
	/*
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		moveSpeed = _moveSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		moveSpeed = -_moveSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		turnSpeed = -_turnSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		turnSpeed = +_turnSpeed;
	}
	*/
}