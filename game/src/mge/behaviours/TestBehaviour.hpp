#ifndef TESTBEHAVIOUR_H
#define TESTBEHAVIOUR_H

#include <glm.hpp>
#include "mge/behaviours/AbstractBehaviour.hpp"
#include <SFML/window.hpp>

class TestBehaviour : public AbstractBehaviour
{
	public:
		TestBehaviour(float turnSpeed = 45);
		virtual ~TestBehaviour();
		virtual void update( float step );

    private:
        float _turnSpeed;
		sf::Vector2i _currMousePos;
		sf::Vector2i _prevMousePos;
		glm::vec2 _mouseDelta;
};

#endif // TESTBEHAVIOUR_H
