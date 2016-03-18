#ifndef PLAYERBEHAVIOUR_H
#define PLAYERBEHAVIOUR_H

#include <glm.hpp>
#include <tokamak.h>
#include <SFML/window.hpp>

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/PlayerControllerCB.hpp"

class World;
class Camera;

/*
* Class that takes care of all the player physics, movement and shooting.
* It also takes care of the camera controls.
*/
class PlayerBehaviour : public AbstractBehaviour
{
	public:
		PlayerBehaviour(Camera* pCamera);
		virtual ~PlayerBehaviour();
		void Initialize();
		
		void PlayerController(neRigidBodyController* pController, float pStep);
		void AddEnemy(GameObject* pEnemy) { _enemies.push_back(pEnemy); };
		bool IsMoving() { return _grounded && _moving; };

		virtual void update(float step);
		glm::vec3 SpawnPos;

	private:
		float _angleY;

		bool _grounded;
		bool _walled;
		bool _falling;
		bool _wallJumped;
		bool _walking;
		bool _moving;
		bool _gravityReset;

		glm::vec3 _inputVector;
		glm::vec3 _moveVelocity;
		glm::vec3 _jumpVelocity;
		glm::vec3 _physicsVelocity;

		glm::vec3 _lastPosition;
		float _slowFeedbackTimer;

		bool _dead;
		float _respawnTimer;

		std::vector<GameObject*> _enemies;

		PlayerControllerCB _playerControllerCB;

		//These are for button states
		//TODO Write Input Manager
		bool _holdingJump;
		bool _holdingShoot;
		bool _holdingSlowmotion;
		bool _holdingPause;

		GameObject *_raycastCube;

		Camera* _camera;
		sf::Vector2i _currMousePos;
		sf::Vector2i _prevMousePos;
		glm::vec2 _mouseDelta;
};

#endif // PLAYERBEHAVIOUR_H
