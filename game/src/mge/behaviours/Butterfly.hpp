#ifndef BUTTERFLY_H
#define BUTTERFLY_H

#include <glm.hpp>
#include <vector>

#include "mge/behaviours/AbstractBehaviour.hpp"

class MovingBehaviour;

/**
 * Describes the functionality of the Butterfly used as visual guidance in the levels.
 * The Butterfly moves between waypoints using a MovingBehaviour.
 * It jumps to the next waypoint when the player is close enough.
 */
class Butterfly : public AbstractBehaviour
{
    public:
		Butterfly(GameObject * pTarget, float pRadius, MovingBehaviour * pMovement, std::vector<glm::vec3> waypoints);
        virtual ~Butterfly();

        virtual void update( float step );

    private:
        GameObject * _target;
		MovingBehaviour * _movement;
		std::vector<glm::vec3> _waypoints;
		glm::vec3 _currentObjective;
		
		float _radius;
		bool _active;
};

#endif // BUTTERFLY_H
