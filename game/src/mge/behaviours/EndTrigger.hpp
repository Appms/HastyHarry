#ifndef ENDTRIGGER_H
#define ENDTRIGGER_H

#include <glm.hpp>
#include "mge/behaviours/TriggerBehaviour.hpp"

class GameObject;

class EndTrigger : public TriggerBehaviour
{
	public:
		EndTrigger(float pRadius, std::string pNextLevel);
		EndTrigger(std::string params);
		virtual ~EndTrigger();

	protected:
		void trigger();
		std::string _nextLevel;
};	

#endif // ENDTRIGGER_H
