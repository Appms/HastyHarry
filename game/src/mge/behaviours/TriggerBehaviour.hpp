#ifndef TRIGGERBEHAVIOUR_H
#define TRIGGERBEHAVIOUR_H

#include "mge/behaviours/AbstractBehaviour.hpp"

class GameObject;

class TriggerBehaviour : public AbstractBehaviour
{
	public:
		TriggerBehaviour();
		virtual ~TriggerBehaviour();
		virtual void update( float step );

	protected:
		virtual void trigger() = 0;
		float _radius;
		GameObject* _activator;

    private:
		bool _triggered;
};	

#endif // TRIGGERBEHAVIOUR_H
