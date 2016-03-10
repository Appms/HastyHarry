#ifndef MOVINGBEHAVIOUR_H
#define MOVINGBEHAVIOUR_H

#include <glm.hpp>
#include "mge/behaviours/AbstractBehaviour.hpp"
#include <string>
/**
 * Simply rotates the object around its origin
 */
class MovingBehaviour : public AbstractBehaviour
{
public:
	MovingBehaviour(glm::vec3 pOrigin, glm::vec3 pEnd, float pSpeed, bool pLoop = false);
	MovingBehaviour(std::string params);
	virtual ~MovingBehaviour();

	virtual void update(float step);
private:
	glm::vec3 _originPosition;
	glm::vec3 _endPosition;
	float _speed;
	bool _loop;
	bool _dead;
};

#endif // ROTATINGBEHAVIOUR_H
