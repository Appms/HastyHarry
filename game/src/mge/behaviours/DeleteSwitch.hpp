#ifndef DELETESWITCH_H
#define DELETESWITCH_H

#include <glm.hpp>
#include "mge/behaviours/SwitchBehaviour.hpp"

class GameObject;

class DeleteSwitch : public SwitchBehaviour
{
	public:
		DeleteSwitch(GameObject* pParent);
		DeleteSwitch(std::string params);
		virtual ~DeleteSwitch();

		void trigger();

	protected:
		GameObject* _parent;
};	

#endif // DELETESWITCH_H
