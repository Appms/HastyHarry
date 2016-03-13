#ifndef COLLECTTRIGGER_H
#define COLLECTTRIGGER_H

#include "mge/behaviours/TriggerBehaviour.hpp"
/*
* Behaviour for collectables that play a sound when picked.
*/
class CollectTrigger : public TriggerBehaviour
{
	public:
		CollectTrigger();
		virtual ~CollectTrigger();

	protected:
		void trigger();
};	

#endif // COLLECTTRIGGER_H
