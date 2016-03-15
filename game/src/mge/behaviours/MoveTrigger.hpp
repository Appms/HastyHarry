#ifndef MOVETRIGGER_H
#define MOVETRIGGER_H

#include <glm.hpp>
#include "mge/behaviours/TriggerBehaviour.hpp"

class GameObject;

class MoveTrigger : public TriggerBehaviour
{
	public:
		MoveTrigger(float pRadius, glm::vec3 pOrigin, glm::vec3 pEnd, float pSpeed, bool pLoop, GameObject* pParent);
		MoveTrigger(glm::vec3 pStart, std::string params);
		virtual ~MoveTrigger();

	protected:
		void trigger();

		glm::vec3 _originPosition;
		glm::vec3 _endPosition;
		float _speed;
		bool _loop;
		bool _dead;

		GameObject* _parent;
};	

#endif // MOVETRIGGER_H
