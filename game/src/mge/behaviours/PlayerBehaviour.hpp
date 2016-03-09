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
	PlayerBehaviour(Camera* pCamera, float pWalkForce, float pRotateSpeed, float pJumpForce);
	virtual ~PlayerBehaviour();
	void Initialize();
	virtual void update(float step);
	void PlayerController(neRigidBodyController* pController, float pStep);
	void AddEnemy(GameObject* pEnemy) { _enemies.push_back(pEnemy); };
private:
	float _walkForce;
	float _angleY;
	float _rotateSpeed;
	float _jumpForce;

	std::vector<GameObject*> _enemies;

	PlayerControllerCB _playerControllerCB;

	//These are for button states
	//TODO Write Input Manager
	bool _holdingJump;
	bool _holdingShoot;
	bool _holdingSlowmotion;
	bool _holdingPause;
	GameObject *test;

	Camera* _camera;
	sf::Vector2i _currMousePos;
	sf::Vector2i _prevMousePos;
	glm::vec2 _mouseDelta;
};

#endif // PLAYERBEHAVIOUR_H
