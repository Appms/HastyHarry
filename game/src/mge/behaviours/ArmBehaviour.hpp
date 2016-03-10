#ifndef ARMBEHAVIOUR_H
#define ARMBEHAVIOUR_H

#include <glm.hpp>
#include "mge/behaviours/AbstractBehaviour.hpp"

class ArmBehaviour : public AbstractBehaviour
{
    public:
		ArmBehaviour();
        virtual ~ArmBehaviour();

        virtual void update( float step );

private:
	float _timer;
};

#endif // LookAt_H
