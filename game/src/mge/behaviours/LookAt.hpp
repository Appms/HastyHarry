#ifndef LookAt_H
#define LookAt_H

#include <glm.hpp>
#include "mge/behaviours/AbstractBehaviour.hpp"

/**
 * Keeps the owner looking at the given target.
 */
class LookAt : public AbstractBehaviour
{
    public:
        LookAt (GameObject * pTarget);
        virtual ~LookAt();

        virtual void update( float step );

    private:
        GameObject * _target;   //what are we looking at?
};

#endif // LookAt_H
