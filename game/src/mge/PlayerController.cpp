#include "mge/PlayerController.hpp"

void PlayerController::RigidBodyControllerCallback(neRigidBodyController * controller, float timeStep)
{
	glm::vec3 _speedVector = glm::vec3(0, 0, 0);
	bool pressedKey = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		_speedVector.z = -20.0;
		pressedKey = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		_speedVector.z = 20.0;
		pressedKey = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		_speedVector.x = 20.0;
		pressedKey = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		_speedVector.x = -20.0;
		pressedKey = true;
	}
	controller->SetControllerForce(toTokamak(glm::vec3(_speedVector.x, 0.0f, _speedVector.z)));
}