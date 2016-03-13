#ifndef ARMBEHAVIOUR_H
#define ARMBEHAVIOUR_H

#include <glm.hpp>

#include "mge/behaviours/AbstractBehaviour.hpp"

/**
* Class in charge of animating the player's arm through code.
*/
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

#endif // ARMBEHAVIOUR_H
