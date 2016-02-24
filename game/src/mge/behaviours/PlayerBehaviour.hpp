#ifndef PLAYERBEHAVIOUR_H
#define PLAYERBEHAVIOUR_H

#include <glm.hpp>
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "..\..\include\tokamak.h"
#include <SFML/window.hpp>
#include "mge/PlayerController.hpp"

class Camera;

class PlayerBehaviour : public AbstractBehaviour
{
public:
	PlayerBehaviour(Camera* pCamera, float pWalkForce, float pMaxVelocity, float pRotateSpeed, float pJumpForce);
	virtual ~PlayerBehaviour();
	virtual void update(float step);
	neV3 toTokamak(glm::vec3 v);
	glm::vec3 toGLM(neV3 v);
private:
	float _walkForce;
	float _rotateSpeed;
	float _jumpForce;
	float _maxVelocity;

	PlayerController _controllerCallback;

	Camera* _camera;
	glm::vec3 _speedVector;
	sf::Vector2i _prevMousePos;
};

#endif // PLAYERBEHAVIOUR_H
