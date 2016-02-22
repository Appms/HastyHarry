#include <cassert>
#include <iostream>
using namespace std;

#include <glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/behaviours/AbstractBehaviour.hpp"

GameObject::GameObject(std::string pName, glm::vec3 pPosition, GameObject::PhysicsType pPhysicsType )
:	_name( pName ), _transform( glm::translate( pPosition ) ),  _parent(NULL), _children(),
    _mesh( NULL ), _physicsType(pPhysicsType), _rigidbody(NULL), _animatedbody(NULL),_behaviour( NULL ), _material(NULL), _world(NULL)
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

    //do not forget to delete behaviour, material, mesh, collider manually if required!
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
	_updatePhysicsBody();
}

void GameObject::_updatePhysicsBody() {

	neGeometry* geometry;
	neV3 boxSize;
	neV3 pos;
	neQ rot;
	glm::vec3 worldPos = this->getWorldPosition();
	f32 mass = 1.0f;

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
		_rigidbody = _world->getPhysics()->CreateRigidBody();
		geometry = _rigidbody->AddGeometry();
		boxSize.Set(scale[0] * 2.0f, scale[1] * 2.0f, scale[2] * 2.0f);
		geometry->SetBoxSize(boxSize[0], boxSize[1], boxSize[2]);
		_rigidbody->UpdateBoundingInfo();

		_rigidbody->SetInertiaTensor(neBoxInertiaTensor(boxSize[0], boxSize[1], boxSize[2], mass));
		_rigidbody->SetMass(mass);
		
		_rigidbody->SetPos(pos);
		_rigidbody->SetRotation(rot);
		break;
	case GameObject::ANIMATEDBODY:
		_animatedbody = _world->getPhysics()->CreateAnimatedBody();
		geometry = _animatedbody->AddGeometry();
		boxSize.Set(scale[0] * 2.0f, scale[1] * 2.0f, scale[2] * 2.0f);
		geometry->SetBoxSize(boxSize[0], boxSize[1], boxSize[2]);
		_animatedbody->UpdateBoundingInfo();

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
	_behaviour = pBehaviour;
	_behaviour->setOwner(this);
}

AbstractBehaviour * GameObject::getBehaviour() const
{
    return _behaviour;
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
	if (_rigidbody != NULL ) {//|| _animatedbody != NULL) {
		neT3 t;
		switch (_physicsType)
		{
		case GameObject::RIGIDBODY:
			t = _rigidbody->GetTransform();
			break;
		case GameObject::ANIMATEDBODY:
			t = _animatedbody->GetTransform();
			break;
		}

		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(_transform, scale, rotation, translation, skew, perspective);

		_transform = glm::mat4(scale.x * (float)t.rot[0][0], scale.x * (float)t.rot[0][1], scale.x * (float)t.rot[0][2], 0.0f,
								scale.y * (float)t.rot[1][0], scale.y * (float)t.rot[1][1], scale.y * (float)t.rot[1][2], 0.0f,
								scale.z * (float)t.rot[2][0], scale.z * (float)t.rot[2][1], scale.z * (float)t.rot[2][2], 0.0f,
								(float)t.pos[0], (float)t.pos[1], (float)t.pos[2], 1.0f);
	}

    _worldTransform = pParentTransform * _transform;

    //make sure behaviour is updated after worldtransform is set
	if (_behaviour) {
		_behaviour->update(pStep);
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
