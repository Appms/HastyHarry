#ifndef SWITCHBEHAVIOUR_H
#define SWITCHBEHAVIOUR_H

#include "mge/behaviours/AbstractBehaviour.hpp"

class GameObject;

class SwitchBehaviour : public AbstractBehaviour
{
	public:
		SwitchBehaviour();
		virtual ~SwitchBehaviour();
		virtual void update(float step);

    protected:
		virtual void trigger() = 0;
		
		bool _triggered;
};	

#endif // SWITCHBEHAVIOUR_H
