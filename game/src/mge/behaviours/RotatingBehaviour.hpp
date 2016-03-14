#ifndef ROTATINGBEHAVIOUR_H
#define ROTATINGBEHAVIOUR_H

#include <glm.hpp>

#include "mge/behaviours/AbstractBehaviour.hpp"
/**
 * Simply rotates the object around its origin
 */
class RotatingBehaviour : public AbstractBehaviour
{
public:
	RotatingBehaviour(glm::vec3 pAxis, float anglePerSec = 45.0);
	RotatingBehaviour(std::string params);
	virtual ~RotatingBehaviour();

	virtual void update(float step);
private:
	glm::vec3 _axis;
	float _anglePerSec;
};

#endif // ROTATINGBEHAVIOUR_H
