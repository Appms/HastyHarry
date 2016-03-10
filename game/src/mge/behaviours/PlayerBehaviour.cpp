#include "mge/behaviours/PlayerBehaviour.hpp"
#include "mge/behaviours/SoundBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Camera.hpp"
#include <sfml/window/event.hpp>
#include <iostream>
#include "..\..\include\tokamak.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <SFML/window.hpp>
#include "mge/PlayerControllerCB.hpp"
#include "mge/core/World.hpp"
#include "mge/core/GameObject.hpp"
#include "SFML/Audio.hpp"
#include "mge/util/Utility.hpp"

#include "mge/config.hpp"
#include "mge/core/Mesh.hpp"

#include "mge/core/SoundEngine.hpp"
#include "mge/core/Level.hpp"
#include "mge/core/Timer.hpp"
#include "mge/materials/ColorMaterial.hpp"

#define WALLRUN_GRAVITY -0.1f
#define HARD_INERTIA  0.2f
#define AIR_CONTROLL  0.4f

PlayerBehaviour::PlayerBehaviour(Camera* pCamera, float pWalkForce, float pRotateSpeed, float pJumpForce) : AbstractBehaviour()
{
	_camera = pCamera;
	_walkForce = pWalkForce;
	_rotateSpeed = pRotateSpeed;
	_jumpForce = pJumpForce;

	_prevMousePos = sf::Mouse::getPosition();
}

void PlayerBehaviour::Initialize()
{
	//SoundEngine::PlayMusic("music");

	f32 mass = 65.0f;
	f32 diameter = 2.0f;

	//Request a rigidbody from Physicsworld
	if (_owner->getRigidBody() != NULL) _owner->GetWorld()->getPhysics()->FreeRigidBody(_owner->getRigidBody());
	neRigidBody* rigidBody = _owner->GetWorld()->getPhysics()->CreateRigidBody();

	//Add geometry to it and set it up
	neGeometry* geometry = rigidBody->AddGeometry();
	geometry->SetSphereDiameter(diameter);

	//Create material and apply it
	_owner->GetWorld()->getPhysics()->SetMaterial(0, 100000000.0f, 0.0f);
	geometry->SetMaterialIndex(0);

	//Add a sensor for groundcheck
	{
		neSensor* sensor;
		neV3 sensorPos, sensorDir;
		sensorPos.Set(0.0f, -1.0f, 0.0f);
		sensorDir.Set(0.0f, -0.1f, 0.0f);
		sensor = rigidBody->AddSensor();
		sensor->SetLineSensor(sensorPos, sensorDir);
		sensor->SetUserData('g');
	}

	//Add a sensor for raycast
	{
		neSensor* sensor;
		sensor = rigidBody->AddSensor();
		glm::vec3 vecDir = _camera->getLocalForwardVector();
		vecDir.z = -vecDir.z;
		vecDir = glm::normalize(vecDir) * 100.0f;
		sensor->SetLineSensor(Utility::glmToNe(_camera->getLocalPosition()), Utility::glmToNe(vecDir * 100));
		sensor->SetUserData('r');
	}

	//Add sensors for Wall check
	glm::vec3 sensorDir = glm::vec3(0, 0, 1.2);
	int sensorCount = 8;

	for (int i = 0; i < sensorCount; ++i)
	{
		neSensor* sensor;
		neV3 sensorPos;
		sensorPos.Set(0.0f, 0.0f, 0.0f);
		sensor = rigidBody->AddSensor();
		sensor->SetLineSensor(sensorPos, Utility::glmToNe(sensorDir));
		sensorDir = glm::rotateY(sensorDir, glm::radians(360.0f / sensorCount));
		sensor->SetUserData('w');
	}

	//Add a controller callback
	neRigidBodyController* controller;
	controller = rigidBody->AddController(&_playerControllerCB, 0);

	//Update dat Bounds
	rigidBody->UpdateBoundingInfo();

	//Set Physics Propertys
	rigidBody->SetInertiaTensor(neSphereInertiaTensor(diameter, mass));
	rigidBody->SetMass(mass);
	rigidBody->SetUserData((u32)this);

	//Parse pos and rot info to Tokamak
	//TODO Make more helper functions
	glm::quat rot = _owner->getRotation();;
	neQ nRot;
	nRot.Set(rot.x, rot.y, rot.z, rot.w);

	rigidBody->SetPos(Utility::glmToNe(_owner->getPosition()));
	rigidBody->SetRotation(nRot);

	//Set the created rigidbody as the players rigidbody
	_owner->setRigidBody(rigidBody);

	//Set Camera as child and set position
	_camera->setParent(_owner);
	_camera->setLocalPosition(glm::vec3(0, 0.8, 0));

	//Set audio listener volume
	sf::Listener::setGlobalVolume(50.0f);

	test = new GameObject("");
	Level::CurrentWorld->add(test);
	test->setMesh(Mesh::load(config::MGE_MODEL_PATH + "cube.obj"));
	test->setMaterial(new ColorMaterial(glm::vec3(1, 0, 1)));
}

PlayerBehaviour::~PlayerBehaviour()
{
	_enemies.clear();
	delete test;
}

void PlayerBehaviour::PlayerController(neRigidBodyController* pController, float pStep)
{
	_mouseDelta = _mouseDelta * pStep * _rotateSpeed;
	//HACK X and Z rotation locking (may be deprecated)
	_angleY += _mouseDelta.x / 90.0f;

	neM3 rotationMatrixY;
	rotationMatrixY[0].Set(cos(_angleY), 0.0f, sin(_angleY));
	rotationMatrixY[1].Set(0.0f, 1.0f, 0.0f);
	rotationMatrixY[2].Set(-sin(_angleY), 0.0f, cos(_angleY));

	pController->GetRigidBody()->SetRotation(rotationMatrixY);

	glm::vec3 camFor = _camera->getForwardVector() * -1.f;
	glm::vec3 bodyFor = _owner->getForwardVector();

	bool camGreater = camFor.y > bodyFor.y ? true : false;
	
	if (camGreater) {
		if (glm::abs(glm::dot(bodyFor, camFor)) > 0.1) {
			_camera->rotate(glm::radians(-_mouseDelta.y), glm::vec3(1, 0, 0));
		}
		else if (-_mouseDelta.y < 0.0f) _camera->rotate(glm::radians(-_mouseDelta.y), glm::vec3(1, 0, 0));
	}
	else {
		if (glm::abs(glm::dot(bodyFor, camFor)) > 0.1) {
			_camera->rotate(glm::radians(-_mouseDelta.y), glm::vec3(1, 0, 0));
		}
		else if (-_mouseDelta.y > 0.0f) _camera->rotate(glm::radians(-_mouseDelta.y), glm::vec3(1, 0, 0));
	}
	

	//TODO Fix the rotation limit
	//_camera->rotate(glm::radians(-_mouseDelta.y), glm::vec3(1, 0, 0));

	//Sensor reading
	pController->GetRigidBody()->BeginIterateSensor();
	neSensor* sensor;

	bool grounded = false;
	//_owner->setParent(Level::CurrentWorld);
	bool onWall = false;
	std::vector<glm::vec3> wallNormals;

	while (sensor = pController->GetRigidBody()->GetNextSensor())
	{
		if (sensor->GetUserData() == 'g' && sensor->GetDetectDepth() > 0.0f)
		{
			grounded = true;
			//_owner->setParent((GameObject*)sensor->GetDetectAnimatedBody()->GetUserData());
		}
		else if (sensor->GetUserData() == 'w' && sensor->GetDetectDepth() > 0.0f)
		{
			//TODO Make sure the object is actually a wall
			onWall = true;
			wallNormals.push_back(Utility::neToGlm(sensor->GetDetectNormal()));
		}
		else if (sensor->GetUserData() == 'r')
		{
			glm::vec3 pointDir = _camera->getWorldPosition() + _camera->getForwardVector() * -10.0f;
			//test->setLocalPosition(pointDir);
			test->setLocalPosition(Utility::neToGlm(sensor->GetDetectContactPoint()));
			
			//if(sensor->GetDetectAnimatedBody() != NULL)
			//std::cout << ((GameObject*)(sensor->GetDetectAnimatedBody()->GetUserData()))->getName() << std::endl;

			glm::vec3 vecDir = _camera->getLocalForwardVector();
			vecDir = glm::normalize(vecDir);// *-100.0f;

			std::cout << vecDir << std::endl;
			sensor->SetLineSensor(Utility::glmToNe(_camera->getLocalPosition()), Utility::glmToNe(vecDir) * -100.f);

			if (!Timer::IsPaused() && !_holdingShoot && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && sensor->GetDetectDepth() > 0.0f) {
				GameObject* object = (GameObject*)sensor->GetDetectAnimatedBody()->GetUserData();
				delete (object);
			}

			_holdingShoot = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
		}
	}

	if (onWall)
	{
		glm::vec3 potentialNormal;
		float potentialDot = 1.0f;

		for (std::vector<glm::vec3>::iterator it = wallNormals.begin(); it != wallNormals.end(); ++it)
		{
			float dot = glm::dot((*it), _owner->getForwardVector());
			if (abs(dot) < potentialDot)
			{
				potentialNormal = (*it);
			}
		}

		neV3 newVel; 
		newVel.Set(pController->GetRigidBody()->GetVelocity());
		newVel[1] = WALLRUN_GRAVITY;
		pController->GetRigidBody()->SetVelocity(newVel);

		//TODO WTF
		//glm::vec3 moveVec = glm::vec3(1, 1, 1) - glm::vec3(abs(), abs(), abs());
	}

	glm::vec3 _speedVector = glm::vec3(0, 0, 0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		_speedVector -= glm::vec3(_owner->getForwardVector().x, 0.0f, _owner->getForwardVector().z);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		_speedVector += glm::vec3(_owner->getForwardVector().x, 0.0f, _owner->getForwardVector().z);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		_speedVector += glm::vec3(_owner->getRightVector().x, 0.0f, _owner->getRightVector().z);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		_speedVector -= glm::vec3(_owner->getRightVector().x, 0.0f, _owner->getRightVector().z);
	}

	bool applyVelocity = false;
	
	//Normalize and apply appropriate speed
	if (glm::length(_speedVector) > 0.0f)
	{
		//TODO Add Air controll paramater
		_speedVector = glm::normalize(_speedVector) * _walkForce * pStep;
		applyVelocity = true;
	}

	//Add current Y velocity unchanged
	_speedVector.y = pController->GetRigidBody()->GetVelocity()[1];

	//Jump
	if (!_holdingJump && grounded && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		_speedVector.y = _jumpForce;
		applyVelocity = true;
	}

	_holdingJump = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

	if (applyVelocity) {
		pController->GetRigidBody()->SetVelocity(Utility::glmToNe(_speedVector));
	}

	//Slowmotion
	if (!_holdingSlowmotion && sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
		Timer::Slowmotion = !Timer::Slowmotion;
	}

	_holdingSlowmotion = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);

	if (grounded) {
		pController->GetRigidBody()->SetLinearDamping(HARD_INERTIA);
	} else {
		pController->GetRigidBody()->SetLinearDamping(0.0f);
	}
}

void PlayerBehaviour::update(float pStep)
{
	//Calculate the mouse Vector
	_currMousePos = sf::Mouse::getPosition();
	_mouseDelta = glm::vec2(_currMousePos.x, _currMousePos.y) - glm::vec2(_prevMousePos.x, _prevMousePos.y);

	//Keep the mouse from leaving the center
	int screenHeight = sf::VideoMode::getDesktopMode().height;
	int screenWidth = sf::VideoMode::getDesktopMode().width;

	if (!Timer::IsPaused())
	{
		sf::Mouse::setPosition(sf::Vector2i(screenWidth / 2, screenHeight / 2));
	}

	//Update previous mouse position
	_prevMousePos = sf::Mouse::getPosition();

	//Updating the audio listener pos and dir
	sf::Listener::setPosition(_owner->getWorldPosition().x, _owner->getWorldPosition().y, _owner->getWorldPosition().z);
	sf::Listener::setDirection(_owner->getForwardVector().x, _owner->getForwardVector().y, -_owner->getForwardVector().z);
	sf::Listener::setUpVector(_camera->getUpVector().x, _camera->getUpVector().y, _camera->getUpVector().z);

	//Pause
	if (!_holdingPause && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
	{
		Timer::TogglePause();
	}
	
	_holdingPause = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P);
}