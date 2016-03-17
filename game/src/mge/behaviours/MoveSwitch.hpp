#ifndef MOVESWITCH_H
#define MOVESWITCH_H

#include <glm.hpp>
#include "mge/behaviours/SwitchBehaviour.hpp"

class GameObject;

class MoveSwitch : public SwitchBehaviour
{
	public:
		MoveSwitch(glm::vec3 pOrigin, glm::vec3 pEnd, float pSpeed, bool pLoop, GameObject* pParent);
		MoveSwitch(std::string params);
		virtual ~MoveSwitch();

		void trigger();

	protected:
		glm::vec3 _originPosition;
		glm::vec3 _endPosition;
		float _speed;
		bool _loop;
		bool _dead;

		GameObject* _parent;
};	

#endif // MOVESWITCH_H
