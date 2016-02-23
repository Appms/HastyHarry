#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <string>
#include <iostream>
#include <glm.hpp>
#include "..\..\include\tokamak.h"

class AbstractCollider;
class AbstractBehaviour;
class AbstractMaterial;
class World;
class Mesh;

/**
 * A GameObject wraps all data required to display an (interactive / dynamic) object, but knows nothing about OpenGL or rendering.
 * GameObject exposes both local and world transform and shows one way of how you can cache the worldtransform.
 * A nice addition for you as a student would be for example to try and add an inversetransform and inverseworldtransform (also cached).
 *
 * You will need to alter this class to add colliders etc.
 */

/*class CollisionCallback : public neRigidBodyControllerCallback {
	public:
		CollisionCallback();
		virtual ~CollisionCallback();

		virtual void RigidBodyControllerCallback(neRigidBodyController * controller) {
			//controller code goes here.
		}
};*/

class GameObject
{
	public:

		enum PhysicsType
		{
			RIGIDBODY, ANIMATEDBODY
		};

		enum ColliderType {
			CUBE, SPHERE, CAPSULE
		};

		GameObject(std::string pName = NULL, glm::vec3 pPosition = glm::vec3( 0.0f, 0.0f, 0.0f ),  PhysicsType pPhysicsType = PhysicsType::ANIMATEDBODY, ColliderType pColliderType = ColliderType::SPHERE);
		virtual ~GameObject();

        void setName (std::string pName);
        std::string getName() const;

        //contains local rotation, scale, position
		void setTransform (const glm::mat4& pTransform);
        glm::mat4& getTransform();

        //access just the local position
		void setLocalPosition (glm::vec3 pPosition);
		glm::vec3 getLocalPosition();

        //get the objects world position by combining transforms
		glm::vec3 getWorldPosition();
		virtual glm::mat4& getWorldTransform();

        //change local position, rotation, scaling
		void translate(glm::vec3 pTranslation);
		void rotate(float pAngle, glm::vec3 pAxis);
		void scale(glm::vec3 pScale);

        //mesh and material should be shared as much as possible
		void setMesh(Mesh* pMesh);
		Mesh* getMesh() const;

		//you can just get the physics' bodies publicly, not set them... For now
		neRigidBody* getRigidBody() const;
		neAnimatedBody* getAnimatedBody() const;

		void setMaterial (AbstractMaterial* pMaterial);
		AbstractMaterial* getMaterial() const;

        //behaviour will probably be a unique instance per gameobject
		void setBehaviour(AbstractBehaviour* pBehaviour);
		AbstractBehaviour* getBehaviour() const;

		virtual void update(float pStep, const glm::mat4& pParentTransform);

        //child management

        //shortcut to set the parent of pChild to ourselves
		void add (GameObject* pChild);
		//shortcut to set the parent of pChild to NULL
		void remove (GameObject* pChild);

		virtual void setParent (GameObject* pGameObject);
		GameObject* getParent();

        int getChildCount();
        GameObject* getChildAt (int pIndex);

		std::string UniqueId;
		std::string UniqueParentId;

	protected:
		std::string _name;
		glm::mat4 _transform;
		glm::mat4 _worldTransform;

        GameObject* _parent;
		std::vector<GameObject*> _children;

        Mesh* _mesh;
		PhysicsType _physicsType;
		ColliderType _colliderType;
		neRigidBody* _rigidbody;
		neAnimatedBody* _animatedbody;
		AbstractBehaviour* _behaviour;
		AbstractMaterial* _material;
		World* _world;

        //update children list administration
        void _innerAdd (GameObject* pChild);
		void _innerRemove (GameObject* pChild);

		//update the rigidbodies and animatedbodies when you change the meshes
		void _updatePhysicsBody();
};

#endif // GAMEOBJECT_H
