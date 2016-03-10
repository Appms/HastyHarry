#ifndef ARMBEHAVIOUR_H
#define ARMBEHAVIOUR_H

#include <glm.hpp>
#include "mge/behaviours/AbstractBehaviour.hpp"

class ArmBehaviour : public AbstractBehaviour
{
    public:
		ArmBehaviour(bool pCos);
        virtual ~ArmBehaviour();

        virtual void update( float step );

private:
	float _timer;
	bool _cos;
};

#endif // LookAt_H
