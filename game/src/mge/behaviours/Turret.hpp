#ifndef Turret_H
#define Turret_H

#include <glm.hpp>
#include "mge/behaviours/AbstractBehaviour.hpp"

/**
 * Keeps the owner looking at the given target.
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

#endif // LookAt_H
