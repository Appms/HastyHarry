#ifndef TURRET_H
#define TURRET_H

#include <glm.hpp>
#include "mge/behaviours/AbstractBehaviour.hpp"

/**
 * Looks at the player when in range and shoots every few seconds.
 */
class Turret : public AbstractBehaviour
{
    public:
		Turret(GameObject * pTarget, float pRadius);
		Turret(std::string params);
        virtual ~Turret();

        virtual void update( float step );

    private:
        GameObject * _target;   //what are we looking at?
		float _radius;
		float _timer;
};

#endif // TURRET_H
