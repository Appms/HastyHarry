#include <glm.hpp>
#include "mge/behaviours/ArmBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/window.hpp>
#include <iostream>
#include <cmath>
#include "mge/behaviours/PlayerBehaviour.hpp"
using namespace std;

ArmBehaviour::ArmBehaviour(bool pCos): AbstractBehaviour()
{
	_cos = pCos;
}

ArmBehaviour::~ArmBehaviour()
{
}

void ArmBehaviour::update(float step)
{
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

	//std::cout << "Pos: " << _owner->getLocalPosition() << " Rot: " << _owner->getRotation() << std::endl;

	if (((PlayerBehaviour*)_owner->getParent()->getBehaviour())->IsMoving())
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
			_owner->rotate(-sin(_timer * 7.5f) * 0.1f, glm::vec3(1, 0, 0));
		}
		else {
			_owner->rotate(sin(_timer * 7.5f) * 0.1f, glm::vec3(1, 0, 0));
		}
	}
}
