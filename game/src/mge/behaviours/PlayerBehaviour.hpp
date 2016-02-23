#ifndef PLAYERBEHAVIOUR_H
#define PLAYERBEHAVIOUR_H

#include "mge/behaviours/AbstractBehaviour.hpp"

class Camera;
/**
* KeysBehaviour allows you to move an object using the keyboard in its own local space.
* Left right turns, forward back moves.
*/
class PlayerBehaviour : public AbstractBehaviour
{
public:
	PlayerBehaviour(Camera* pCamera, float pWalkSpeed, float pRotateSpeed, float pJumpForce);
	virtual ~PlayerBehaviour();
	virtual void update(float step);

private:
	float _walkSpeed;
	float _rotateSpeed;
	float _jumpForce;
	Camera* _camera;
};

#endif // PLAYERBEHAVIOUR_H
