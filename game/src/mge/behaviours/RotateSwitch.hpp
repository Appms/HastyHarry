#ifndef ROTATEWITCH_H
#define ROTATESWITCH_H

#include <glm.hpp>
#include "mge/behaviours/SwitchBehaviour.hpp"

class GameObject;

class RotateSwitch : public SwitchBehaviour
{
	public:
		RotateSwitch(glm::vec3 pAxis, float pAngles, GameObject* pParent);
		RotateSwitch(std::string params);
		virtual ~RotateSwitch();

		void trigger();

	protected:

		glm::vec3 _axis;
		float _anglePerSec;

		GameObject* _parent;
};	

#endif // ROTATESWITCH_H
