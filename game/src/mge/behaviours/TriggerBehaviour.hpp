#ifndef TRIGGERBEHAVIOUR_H
#define TRIGGERBEHAVIOUR_H

#include <glm.hpp>
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "SFML/Audio.hpp"

class GameObject;

class TriggerBehaviour : public AbstractBehaviour
{
	public:
		TriggerBehaviour(GameObject* pActivator, float pRadius);
		virtual ~TriggerBehaviour();
		virtual void update( float step );

	protected:
		virtual void trigger() = 0;

    private:
		bool _triggered;
		float _radius;
		GameObject* _activator;
};	

#endif // TRIGGERBEHAVIOUR_H
