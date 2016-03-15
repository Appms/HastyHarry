#ifndef ROTATETRIGGER_H
#define ROTATETRIGGER_H

#include <glm.hpp>
#include "mge/behaviours/TriggerBehaviour.hpp"

class GameObject;

class RotateTrigger : public TriggerBehaviour
{
	public:
		RotateTrigger(float pRadius, glm::vec3 pAxis, float pAngles, GameObject* pParent);
		RotateTrigger(std::string params);
		virtual ~RotateTrigger();

	protected:
		void trigger();

		glm::vec3 _axis;
		float _anglePerSec;

		GameObject* _parent;
};	

#endif // ROTATETRIGGER_H
