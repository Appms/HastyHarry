#include "mge/behaviours/TestBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/config.hpp"

#include <sfml/window/event.hpp>
#include <iostream>

#include "mge/core/Timer.hpp"
#include <SFML/window.hpp>

TestBehaviour::TestBehaviour(float turnSpeed) : AbstractBehaviour(), _turnSpeed(turnSpeed)
{
}

TestBehaviour::~TestBehaviour()
{
}

void TestBehaviour::update(float pStep)
{
	_currMousePos = sf::Mouse::getPosition();
	_mouseDelta = glm::vec2(_currMousePos.x, _currMousePos.y) - glm::vec2(_prevMousePos.x, _prevMousePos.y);

	//Keep the mouse from leaving the center
	if (!Timer::IsPaused())
	{
		int screenHeight = sf::VideoMode::getDesktopMode().height;
		int screenWidth = sf::VideoMode::getDesktopMode().width;
		sf::Mouse::setPosition(sf::Vector2i(screenWidth / 2, screenHeight / 2));
	}

	//Update previous mouse position
	_prevMousePos = sf::Mouse::getPosition();

	_owner->rotate(_mouseDelta.x / _turnSpeed, glm::vec3(0, 1, 0));
	_owner->rotate(_mouseDelta.y / _turnSpeed, glm::vec3(1, 0, 0));
}

