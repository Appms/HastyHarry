#include <iostream>
#include <cmath>
#include <glm.hpp>
#include <SFML/window.hpp>

#include "mge/behaviours/ArmBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/behaviours/PlayerBehaviour.hpp"
#include "mge/core/Level.hpp"

ArmBehaviour::ArmBehaviour(bool pCos): AbstractBehaviour(), _cos(pCos){
	
}

void ArmBehaviour::Init()
{
	_initMat = _owner->getTransform();
}

ArmBehaviour::~ArmBehaviour(){}

void ArmBehaviour::update(float step)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0)) {
		testSwitch = !testSwitch;
	}

	if (!testSwitch && !_cos || testSwitch && _cos)
	{
		//Positioning of the arms at runtime.
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			glm::vec3 pos = _owner->getLocalPosition();
			pos.z -= step;
			_owner->setLocalPosition(pos);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			glm::vec3 pos = _owner->getLocalPosition();
			pos.z += step;
			_owner->setLocalPosition(pos);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			glm::vec3 pos = _owner->getLocalPosition();
			pos.x -= step;
			_owner->setLocalPosition(pos);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			glm::vec3 pos = _owner->getLocalPosition();
			pos.x += step;
			_owner->setLocalPosition(pos);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
			glm::vec3 pos = _owner->getLocalPosition();
			pos.y += step;
			_owner->setLocalPosition(pos);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1)) {
			glm::vec3 pos = _owner->getLocalPosition();
			pos.y -= step;
			_owner->setLocalPosition(pos);
		}

		//Rotation of the arms at runtime.
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8)) {
			_owner->rotate(step, glm::vec3(1, 0, 0));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5)) {
			_owner->rotate(-step, glm::vec3(1, 0, 0));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)) {
			_owner->rotate(step, glm::vec3(0, 0, 1));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6)) {
			_owner->rotate(-step, glm::vec3(0, 0, 1));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7)) {
			_owner->rotate(step, glm::vec3(0, 1, 0));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad9)) {
			_owner->rotate(-step, glm::vec3(0, 1, 0));
		}
	}

	//HACK Level loading

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
		Level::Load("Level_01.xml", Level::CurrentWorld);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
		Level::Load("Level_02.xml", Level::CurrentWorld);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
		Level::Load("Level_03.xml", Level::CurrentWorld);
	}

	//Output position and rotation.
	//std::cout << "Pos: " << _owner->getLocalPosition() << " Rot: " << _owner->getRotation() << std::endl;

	//Move and rotate the arms while running.
	if (((PlayerBehaviour*)(_owner->getParent()->getBehaviour()))->IsMoving())
	{
		_timer += step;
		glm::vec3 pos = _owner->getLocalPosition();
		
		if (_cos) {
			pos.z += -sin(_timer * 7.5f) * 0.05f;
		} else {
			pos.z += sin(_timer * 7.5f) * 0.05f;
		}
		
		_owner->setLocalPosition(pos);

		if (_cos) {
			_owner->rotate((-sin(_timer * 7.5f)) * 0.1f, glm::vec3(1, 0, 0));
		}
		else {
			_owner->rotate((sin(_timer * 7.5f)) * 0.1f, glm::vec3(1, 0, 0));
		}
	}
	else
	{
		//_owner->setTransform(_initMat);
	}
}
