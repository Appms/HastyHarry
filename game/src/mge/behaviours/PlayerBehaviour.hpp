#ifndef PLAYERBEHAVIOUR_H
#define PLAYERBEHAVIOUR_H

#include <glm.hpp>
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "..\..\include\tokamak.h"
#include <SFML/window.hpp>
#include "mge/PlayerControllerCB.hpp"

class World;
class Camera;

class PlayerBehaviour : public AbstractBehaviour
{
public:
	PlayerBehaviour(Camera* pCamera, float pWalkForce, float pMaxVelocity, float pRotateSpeed, float pJumpForce);
	virtual ~PlayerBehaviour();
	void Initialize();
	virtual void update(float step);
	void PlayerController(neRigidBodyController* pController, float pStep);
private:
	float _walkForce;
	float _rotateSpeed;
	float _jumpForce;
	float _maxVelocity;

	PlayerControllerCB _playerControllerCB;

	Camera* _camera;
	glm::vec3 _speedVector;
	sf::Vector2i _currMousePos;
	sf::Vector2i _prevMousePos;
	glm::vec2 _mouseDelta;

	neV3 glmToNe(glm::vec3 v);
	glm::vec3 neToGlm(neV3 v);
};

#endif // PLAYERBEHAVIOUR_H
