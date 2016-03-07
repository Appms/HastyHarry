#ifndef ABSTRACTBEHAVIOUR_H
#define ABSTRACTBEHAVIOUR_H

class GameObject;

/**
 * class Behaviour allows you to attach reusable behaviours to a GameObject.
 * Think of steering, rotating, billboarding, etc.
 * A Behaviour is set on a GameObject, which in turn passes in a reference to itself through the setOwner method.
 * This way we can enforce that a Behaviour can never have an owner different from the object it has been attached to.
 *
 * It is similar to MonoBehaviour in Unity.
 */
class AbstractBehaviour
{
	public:
		AbstractBehaviour();
		virtual ~AbstractBehaviour() = 0;

        //we would like to have this private and only accessible by GameObject, but this
        //doesnt work out for the CompositeBehaviour, we would have to declare both of them
        //as friends, tying this class to one of its subclasses, so design decision:
        //this is kept public but should not be used directly.
        virtual void setOwner (GameObject* pGameObject);

        //behaviour should be able to update itself every step and MUST be implemented
		virtual void update(float pStep) = 0;
		virtual void trigger(GameObject* pPlayer) { };

    protected:
	    //reference back its owner
		GameObject* _owner;

};

#endif // ABSTRACTBEHAVIOUR_H
