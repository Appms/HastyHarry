#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/config.hpp"

#include <sfml/window/event.hpp>
#include <iostream>

KeysBehaviour::KeysBehaviour(float moveSpeed, float turnSpeed, std::string pSoundName): AbstractBehaviour(), _moveSpeed(moveSpeed), _turnSpeed(turnSpeed)
{
	if (!_soundBuffer.loadFromFile(pSoundName)) {
		std::cout << "ERROR: Jumping sound failed to load" << std::endl;
	}
	else {
		_jumpingSound.setBuffer(_soundBuffer);
	}
}

KeysBehaviour::~KeysBehaviour()
{
}

void KeysBehaviour::update( float pStep )
{
	float moveSpeed = 0.0f; //default if no keys
	float turnSpeed = 0.0f;
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

	/*
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		if (!_jumping) {
			neRigidBody * rb = _owner->getRigidBody();

			neV3 vY = rb->GetVelocity();
			neV3 jumpVelocity = vY;
			jumpVelocity[1] += _moveSpeed;

			rb->SetVelocity(jumpVelocity);
			_jumpingSound.play();
		}
	}
	*/

	if (_owner->getRigidBody()->GetVelocity().Y() <= (f32)1.0f) _jumping = false;
	else _jumping = true;

	//neV3 v = _owner->getRigidBody()->GetVelocity();
	//printf("%f -- %f -- %f \n", (float)v.X(), (float)v.Y(), (float)v.Z());

	//translate the object in its own local space
	//_owner->translate( glm::vec3(0.0f, 0.0f, moveSpeed*pStep ) );

	//neV3 movSpeed = _owner->getRigidBody()->GetVelocity();
	//movSpeed[0] += moveSpeedX * 0.1f;
	//movSpeed[2] += moveSpeedZ * 0.1f;
	//_owner->getRigidBody()->SetVelocity(movSpeed);
	_owner->translate(glm::vec3(0.0f, 0.0f, moveSpeed*pStep));
	_owner->rotate(glm::radians(turnSpeed*pStep), glm::vec3(0.0f, 1.0f, 0.0f));



	//we can also translate directly, basically we take the z axis from the matrix
	//which is normalized and multiply it by moveSpeed*step, than we add it to the
	//translation component
	//glm::mat4 transform = _owner->getTransform();
	//transform[3] += transform[2] * moveSpeed*pStep;
	//_owner->setTransform(transform);

	//rotate the object in its own local space
	//_owner->rotate( glm::radians(turnSpeed*pStep), glm::vec3(0.0f, 1.0f, 0.0f ) );
}
