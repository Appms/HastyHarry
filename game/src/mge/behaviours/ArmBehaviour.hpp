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
		void Init();
        virtual void update( float step );

private:
	float _timer;
	bool _cos;
	glm::mat4x4 _initMat;
	bool testSwitch;
};

#endif // ARMBEHAVIOUR_H
