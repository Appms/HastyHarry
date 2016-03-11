#ifndef BUTTERFLY_H
#define BUTTERFLY_H

#include <glm.hpp>
#include <vector>
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "MovingBehaviour.hpp"

/**
 * Keeps the owner looking at the given target.
 */
class Butterfly : public AbstractBehaviour
{
    public:
		Butterfly(GameObject * pTarget, float pRadius, glm::vec3 pStart, MovingBehaviour * pMovement, std::vector<glm::vec3> waypoints);
        virtual ~Butterfly();

        virtual void update( float step );

    private:
        GameObject * _target;
		std::vector<glm::vec3> _waypoints;
		MovingBehaviour * _movement;
		float _radius;
		glm::vec3 _currentObjective;
		bool _active;
};

#endif // LookAt_H
