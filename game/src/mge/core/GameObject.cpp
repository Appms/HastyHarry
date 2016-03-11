#include <cassert>
#include <iostream>
using namespace std;

#include <glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "..\..\include\tokamak.h"
#include "mge/behaviours/PlayerBehaviour.hpp"
#include "mge/core/Level.hpp"
#include "mge/util/Utility.hpp"

GameObject::GameObject(std::string pName, glm::vec3 pPosition, GameObject::PhysicsType pPhysicsType, GameObject::ColliderType pColliderType )
:	_name( pName ), _transform( glm::translate( pPosition ) ),  _parent(NULL), _children(),
    _mesh( NULL ), _physicsType(pPhysicsType), _colliderType(pColliderType), _rigidbody(NULL), _animatedbody(NULL),_behaviours( NULL ), _material(NULL), _world(NULL)
{
}

GameObject::~GameObject()
{
	
    //detach all children
    cout << "GC running on:" << _name << endl;

    while (_children.size() > 0) {
        GameObject* child = _children[0];
        remove (child);
        delete child;
    }

	_mesh = NULL;

	if (_world != NULL) {
		if (_rigidbody != NULL) {
			_world->getPhysics()->FreeRigidBody(_rigidbody);
			_rigidbody = NULL;
		}
		if (_animatedbody != NULL) {
			_world->getPhysics()->FreeAnimatedBody(_animatedbody);
			_animatedbody = NULL;
		}
	}

	for each (AbstractBehaviour* behaviour in _behaviours)
	{
		delete behaviour;
	}
	_behaviours.clear();

	_material = NULL;

	if (_parent != NULL) _parent->remove(this);
	_world = NULL;
}

void GameObject::setName (std::string pName)
{
    _name = pName;
}

std::string GameObject::getName() const
{
	return _name;
}

void GameObject::setTransform (const glm::mat4& pTransform)
{
    _transform = pTransform;
}

glm::mat4& GameObject::getTransform()
{
    return _transform;
}

void GameObject::setLocalPosition (glm::vec3 pPosition)
{
    _transform[3] = glm::vec4 (pPosition,1);
}

glm::vec3 GameObject::getLocalPosition()
{
	return glm::vec3(_transform[3]);
}

void GameObject::setMaterial(AbstractMaterial* pMaterial)
{
    _material = pMaterial;
}

AbstractMaterial * GameObject::getMaterial() const
{
    return _material;
}

void GameObject::setMesh(Mesh* pMesh)
{
	_mesh = pMesh;
}

void GameObject::setMeshWithCollider(Mesh* pMesh)
{
	_mesh = pMesh;
	_updatePhysicsBody();
}

void GameObject::_updatePhysicsBody() {

	neGeometry* geometry;
	neV3 size;
	float sphereSize;
	float capsuleHeight;
	neV3 pos;
	neQ rot;
	glm::vec3 worldPos = this->getWorldPosition();
	f32 mass = 100.0f;

	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(_transform, scale, rotation, translation, skew, perspective);

	pos[0] = translation[0];
	pos[1] = translation[1];
	pos[2] = translation[2];

	rot.X = rotation.x;
	rot.Y = rotation.y;
	rot.Z = rotation.z;
	rot.W = rotation.w;

	switch (_physicsType)
	{
	case GameObject::RIGIDBODY:
		if (_rigidbody != NULL) _world->getPhysics()->FreeRigidBody(_rigidbody);

		_rigidbody = _world->getPhysics()->CreateRigidBody();
		geometry = _rigidbody->AddGeometry();
		
		switch (_colliderType)
		{
		case GameObject::CUBE:
			size.Set(scale[0] , scale[1] , scale[2] );
			geometry->SetBoxSize(size[0], size[1], size[2]);
			_rigidbody->UpdateBoundingInfo();

			_rigidbody->SetInertiaTensor(neBoxInertiaTensor(size[0], size[1], size[2], mass));
			_rigidbody->SetMass(mass);
			break;
		case GameObject::SPHERE:
			sphereSize = max(max(scale.x, scale.y), scale.z);

			geometry->SetSphereDiameter(sphereSize);
			_rigidbody->UpdateBoundingInfo();
			
			_rigidbody->SetInertiaTensor(neSphereInertiaTensor(sphereSize, mass));
			_rigidbody->SetMass(mass);
			break;
		case GameObject::CAPSULE:
			sphereSize = max(scale.x, scale.z);
			capsuleHeight = scale.y;

			geometry->SetCylinder(sphereSize, capsuleHeight);
			_rigidbody->UpdateBoundingInfo();

			_rigidbody->SetInertiaTensor(neCylinderInertiaTensor(sphereSize, capsuleHeight, mass));
			_rigidbody->SetMass(mass);
			break;
		}
		
		_rigidbody->SetPos(pos);
		_rigidbody->SetRotation(rot);

		//CollisionCallback callbackInstance;
		//_rigidbody->AddController(&callbackInstance, 0);

		break;
	case GameObject::ANIMATEDBODY:
		if (_animatedbody != NULL) _world->getPhysics()->FreeAnimatedBody(_animatedbody);

		_animatedbody = _world->getPhysics()->CreateAnimatedBody();
		geometry = _animatedbody->AddGeometry();

		switch (_colliderType)
		{
		case GameObject::CUBE:
			size.Set(scale[0] , scale[1] , scale[2] );
			geometry->SetBoxSize(size[0], size[1], size[2]);
			_animatedbody->UpdateBoundingInfo();
			break;
		case GameObject::SPHERE:
			sphereSize = max(max(scale.x, scale.y), scale.z);

			geometry->SetSphereDiameter(sphereSize);
			_animatedbody->UpdateBoundingInfo();
			break;
		case GameObject::CAPSULE:
			sphereSize = max(scale.x, scale.z);
			capsuleHeight = scale.y;

			geometry->SetCylinder(sphereSize, capsuleHeight);
			_animatedbody->UpdateBoundingInfo();
			break;
		}

		_animatedbody->SetPos(pos);
		_animatedbody->SetRotation(rot);
		break;
	}
}

Mesh * GameObject::getMesh() const
{
    return _mesh;
}

neRigidBody * GameObject::getRigidBody() const {
	return _rigidbody;
}

neAnimatedBody * GameObject::getAnimatedBody() const {
	return _animatedbody;
}

void GameObject::setBehaviour(AbstractBehaviour* pBehaviour)
{
	_behaviours.push_back(pBehaviour);
	_behaviours.back()->setOwner(this);
}

AbstractBehaviour * GameObject::getBehaviour() const
{
    return _behaviours.back();
}

AbstractBehaviour * GameObject::getBehaviour(int index) const
{
	return _behaviours[index];
}

void GameObject::setParent (GameObject* pParent) {
    //remove from previous parent
    if (_parent != NULL) {
        _parent->_innerRemove(this);
        _parent = NULL;
        _world = NULL;
    }

    //set new parent
    if (pParent != NULL) {
        _parent = pParent;
        _parent->_innerAdd(this);
        //pass on world to child
        _world = _parent->_world;
    }
}

GameObject* GameObject::getParent() {
    return _parent;
}

void GameObject::_innerAdd(GameObject* pChild)
{
    //set new parent
    pChild->_parent = this;
	_children.push_back(pChild);
}

void GameObject::_innerRemove (GameObject* pChild) {
    for (auto i = _children.begin(); i != _children.end(); ++i) {
        if (*i == pChild) {
            (*i)->_parent = NULL;
            _children.erase(i);
            return;
        }
    }
}

void GameObject::add (GameObject* pChild) {
    pChild->setParent(this);
}

void GameObject::remove (GameObject* pChild) {
    pChild->setParent(NULL);
}

////////////

glm::vec3 GameObject::getWorldPosition()
{
	return glm::vec3(getWorldTransform()[3]);
}

glm::mat4& GameObject::getWorldTransform()
{
    return _worldTransform;
}

////////////

void GameObject::translate(glm::vec3 pTranslation)
{
	setTransform(glm::translate(_transform, pTranslation));
}

void GameObject::scale(glm::vec3 pScale)
{
	setTransform(glm::scale(_transform, pScale));
}

void GameObject::rotate(float pAngle, glm::vec3 pAxis)
{
	setTransform(glm::rotate(_transform, pAngle, pAxis));
}

void GameObject::update(float pStep, const glm::mat4& pParentTransform)
{
		
	switch (_physicsType)
	{
		case GameObject::RIGIDBODY:
			if (_rigidbody != NULL) {
				neT3 t = _rigidbody->GetTransform();

				glm::vec3 scale = getScale();

				_transform = glm::mat4(scale.x * (float)t.rot[0][0], scale.x * (float)t.rot[0][1], scale.x * (float)t.rot[0][2], 0.0f,
					scale.y * (float)t.rot[1][0], scale.y * (float)t.rot[1][1], scale.y * (float)t.rot[1][2], 0.0f,
					scale.z * (float)t.rot[2][0], scale.z * (float)t.rot[2][1], scale.z * (float)t.rot[2][2], 0.0f,
					(float)t.pos[0], (float)t.pos[1], (float)t.pos[2], 1.0f);
			}
			break;
		case GameObject::ANIMATEDBODY:
			if (_animatedbody != NULL) {

				//this->setLocalPosition(this->getLocalPosition() - glm::vec3(1.0f, 0, 0));

				neV3 pos = _animatedbody->GetPos();
				pos.Set(_transform[3][0] + pParentTransform[3][0], _transform[3][1] + pParentTransform[3][1], _transform[3][2] + pParentTransform[3][2]);
				_animatedbody->SetPos(pos);
				
				//this->rotate(0.01f, glm::vec3(0, 1, 0));

				neM3 rot = _animatedbody->GetRotationM3();
				rot.SetColumns(Utility::glmToNe(glm::vec3(_transform[0])/getScale().x), Utility::glmToNe(glm::vec3(_transform[1])/ getScale().y), Utility::glmToNe(glm::vec3(_transform[2])/ getScale().z));
				_animatedbody->SetRotation(rot);
			}
			break;
	}

    _worldTransform = pParentTransform * _transform;
	
    //make sure behaviour is updated after worldtransform is set
	if (_behaviours.size() > 0) {
		for (int i = 0; i < _behaviours.size(); i++)
		{
			_behaviours[i]->update(pStep);
		}
		//_behaviour->update(pStep);
	}

    for (int i = _children.size()-1; i >= 0; --i ) {
        _children[i]->update(pStep, _worldTransform);
    }
}

int GameObject::getChildCount() {
    return _children.size();
}

GameObject* GameObject::getChildAt(int pIndex) {
    return _children[pIndex];
}

void GameObject::setRigidBody(neRigidBody* body) {
	_rigidbody = body;
}

void GameObject::setAnimatedBody(neAnimatedBody* body)
{
	_animatedbody = body;
}

glm::vec3 GameObject::getPosition() {
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(_transform, scale, rotation, translation, skew, perspective);

	return translation;
}

glm::quat GameObject::getRotation() {
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(_transform, scale, rotation, translation, skew, perspective);

	return rotation;
}

glm::vec3 GameObject::getScale() {
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(_transform, scale, rotation, translation, skew, perspective);

	return scale;
}

World* GameObject::GetWorld()
{
	return _world;
}

glm::vec3 GameObject::getRightVector()
{
	return glm::normalize(glm::vec3(_worldTransform[0]));
}

glm::vec3 GameObject::getUpVector()
{
	return glm::normalize(glm::vec3(_worldTransform[1]));
}

glm::vec3 GameObject::getForwardVector()
{
	return glm::normalize(glm::vec3(_worldTransform[2]));// glm::vec3(_worldTransform[0][2], _worldTransform[1][2], _worldTransform[2][2]));
}

glm::vec3 GameObject::getLocalForwardVector()
{
	return glm::normalize(glm::vec3(_transform[2]));// [2], _transform[1][2], _transform[2][2]));
}

