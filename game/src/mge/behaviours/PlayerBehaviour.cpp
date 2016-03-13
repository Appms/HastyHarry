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

//#define WALLRUN_GRAVITY -0.05f
//#define NORMAL_GRAVITY -0.35f

#define WALLRUN_GRAVITY -3.0f
#define NORMAL_GRAVITY -9.8f


#define HARD_INERTIA  0.75f
#define SOFT_INERTIA 0.9f


#define MOVE_FORCE 240.0f
#define RUN_MAXVELOCITY 15.0f
#define WALK_MAXVELOCITY 8.0f
#define AIR_CONTROLL  0.1f
#define AIR_MAXVELOCITY 18.5f


#define GROUND_JUMP_FORCE 7.5f


#define WALLJUMP_NORMALFORCE 25.0f
#define WALLJUMP_UPFORCE 11.0f
#define WALLJUMP_FORWARDFORCE 30.0f


#define ROTATE_SPEED 6.0f


#define SLOW_FEEDBACK_RADIUS 4.0f
#define SLOW_FEEDBACK_TIMER 11.0f


#define DEATH_HEIGHT -40.0f
#define RESPAWN_TIME 4.0f

bool PlayerBehaviour::IsMoving()
{
	return _grounded && _moving;
}

PlayerBehaviour::PlayerBehaviour(Camera* pCamera) : AbstractBehaviour()
{
	_camera = pCamera;

	_prevMousePos = sf::Mouse::getPosition();
	
}

void PlayerBehaviour::Initialize()
{
	_lastPosition = _owner->getPosition();
	SpawnPos = _owner->getPosition();

	f32 mass = 65.0f;
	f32 diameter = 2.0f;

	//Request a rigidbody from Physicsworld
	if (_owner->getRigidBody() != NULL) _owner->GetWorld()->getPhysics()->FreeRigidBody(_owner->getRigidBody());
	neRigidBody* rigidBody = _owner->GetWorld()->getPhysics()->CreateRigidBody();

	rigidBody->GravityEnable(false);

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

	//Add a sensor for Ceilcheck
	{
		neSensor* sensor;
		neV3 sensorPos, sensorDir;
		sensorPos.Set(0.0f, 1.0f, 0.0f);
		sensorDir.Set(0.0f, 0.01f, 0.0f);
		sensor = rigidBody->AddSensor();
		sensor->SetLineSensor(sensorPos, sensorDir);
		sensor->SetUserData('c');
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
	test->scale(glm::vec3(0.1f, 0.1f, 0.1f));
}

PlayerBehaviour::~PlayerBehaviour()
{
	_enemies.clear();
	delete test;
}

void PlayerBehaviour::PlayerController(neRigidBodyController* pController, float pStep)
{
	bool _landed  = !_grounded;

	_moving = false;
	_grounded = false;
	_walled = false;

	_mouseDelta = _mouseDelta * pStep * ROTATE_SPEED;

	_angleY += _mouseDelta.x / 90.0f;

	neM3 rotationMatrixY;
	rotationMatrixY[0].Set(cos(_angleY), 0.0f, sin(_angleY));
	rotationMatrixY[1].Set(0.0f, 1.0f, 0.0f);
	rotationMatrixY[2].Set(-sin(_angleY), 0.0f, cos(_angleY));

	pController->GetRigidBody()->SetRotation(rotationMatrixY);

	/*
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
	*/

	//TODO Fix the rotation limit
	_camera->rotate(glm::radians(-_mouseDelta.y), glm::vec3(1, 0, 0));

	//Sensor reading
	pController->GetRigidBody()->BeginIterateSensor();
	neSensor* sensor;

	std::vector<glm::vec3> wallNormals;
	while (sensor = pController->GetRigidBody()->GetNextSensor())
	{
		if (sensor->GetUserData() == 'g' && sensor->GetDetectDepth() > 0.0f)
		{
			if (_landed)
			{
				SoundEngine::PlayAudio("land", _owner);
			}
			
			_grounded = true;
		}
		else if (sensor->GetUserData() == 'c' && sensor->GetDetectDepth() > 0.0f)
		{
 			_physicsVelocity.y = 0.0f;
		}
		else if (sensor->GetUserData() == 'w' && sensor->GetDetectDepth() > 0.0f)
		{
			//TODO Make sure the object is actually a wall
			_walled = true;
			wallNormals.push_back(Utility::neToGlm(sensor->GetDetectNormal()));
		}
		else if (sensor->GetUserData() == 'r')
		{
			test->setLocalPosition(Utility::neToGlm(sensor->GetDetectContactPoint()));
			
			glm::vec3 vecDir = -_camera->getLocalForwardVector();
			vecDir = glm::normalize(vecDir) * 100.0f;
			
			sensor->SetLineSensor(Utility::glmToNe(_camera->getLocalPosition()), Utility::glmToNe(vecDir));

			if (!Timer::IsPaused() && !_holdingShoot && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && sensor->GetDetectDepth() > 0.0f) {
				GameObject* object = (GameObject*)sensor->GetDetectRigidBody()->GetUserData();
				delete (object);
			}

			_holdingShoot = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
		}
	}


	//Group the wallnormals
	std::vector<glm::vec3> groupedWallNormals;

	for (std::vector<glm::vec3>::iterator it = wallNormals.begin(); it != wallNormals.end(); ++it)
	{
		bool notGroupedYet = false;

		if (groupedWallNormals.size() == 0)
		{
			notGroupedYet = true;
		}
		else
		{
			for (std::vector<glm::vec3>::iterator it2 = groupedWallNormals.begin(); it2 != groupedWallNormals.end(); ++it2)
			{
				if (!(*it == *it2)) {
					notGroupedYet = true;
				}
			}
		}

		if (notGroupedYet) {
			groupedWallNormals.push_back(*it);
		}
	}


	//Get the best candidate for wallrunning
	glm::vec3 potentialNormal;

	if (_walled)
	{
		float potentialDot = 1.0f;

		for (std::vector<glm::vec3>::iterator it = groupedWallNormals.begin(); it != groupedWallNormals.end(); ++it)
		{
			float dot = glm::dot((*it), _owner->getForwardVector());
			if (abs(dot) < potentialDot)
			{
				potentialNormal = (*it);
			}
		}
	}


	//Reset WallJumped Flag
	if (_grounded || _walled && _falling) {
		_wallJumped = false;
	}

	//Apply Jump force
	_jumpVelocity = glm::vec3(0, 0, 0);

	if (!_holdingJump && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (_grounded) {
			_jumpVelocity.y = GROUND_JUMP_FORCE;
			SoundEngine::PlayVoice("jump");
		}
		else if (_walled && !_wallJumped) {
			//TODO Revise Walljump
			_jumpVelocity.y = WALLJUMP_UPFORCE;
			_moveVelocity += -_camera->getForwardVector() * WALLJUMP_FORWARDFORCE + potentialNormal * WALLJUMP_NORMALFORCE;
			_wallJumped = true;
		}
	}

	_holdingJump = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);


	//Get Inputs
	_inputVector = glm::vec3(0, 0, 0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		_inputVector -= glm::vec3(_owner->getForwardVector().x, 0.0f, _owner->getForwardVector().z);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		_inputVector += glm::vec3(_owner->getForwardVector().x, 0.0f, _owner->getForwardVector().z);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		_inputVector += glm::vec3(_owner->getRightVector().x, 0.0f, _owner->getRightVector().z);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		_inputVector -= glm::vec3(_owner->getRightVector().x, 0.0f, _owner->getRightVector().z);
	}


	//See if walking
	_walking = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);


	//Apply proper acceleration
	if (glm::length2(_inputVector) > 0.0f) {
		_moveVelocity +=  glm::normalize(_inputVector) * (_grounded ? MOVE_FORCE : MOVE_FORCE * AIR_CONTROLL) * pStep;
		_moving = true;
	} else if(_grounded){
		_moveVelocity *= HARD_INERTIA;
	}

	//TODO Do not apply Walk_MAxvelocity when in air but Air_MAXVELCOITY
	//Clamp the moveVelocity
	if (glm::length2(_moveVelocity) > (_grounded ? ((_walking && !_walled) ? WALK_MAXVELOCITY * WALK_MAXVELOCITY : RUN_MAXVELOCITY * RUN_MAXVELOCITY) : AIR_MAXVELOCITY * AIR_MAXVELOCITY))
	{
		_moveVelocity = glm::normalize(_moveVelocity) * ((_grounded && !_walled) ? (_walking ? WALK_MAXVELOCITY : RUN_MAXVELOCITY) : AIR_MAXVELOCITY);
	}


	//TODO Implement that the player cant wallrun backwards

	//Remove Wallnormals from move Velocity
	for (std::vector<glm::vec3>::iterator it = groupedWallNormals.begin(); it != groupedWallNormals.end(); ++it)
	{
		float dot = glm::dot((*it), _moveVelocity);

		//TODO Sticking to the wall might be buggy
		if (_grounded && dot < 0.0f || _wallJumped && dot < 0.0f || !_grounded && !_wallJumped)
		{
			_moveVelocity -= (*it) * dot;
		}
	}

	//See if falling
	_falling = _physicsVelocity.y < 0.0f;


	//Apply Gravity
	_physicsVelocity += _jumpVelocity;

	//TODO Apply deltatime to gravity otherwise slowmotion and pause wont work properly
	if (_walled && !_grounded && _falling)
	{
		_physicsVelocity += glm::vec3(0, WALLRUN_GRAVITY, 0) * pStep;
	}
	else if(!_grounded)
	{
		_physicsVelocity += glm::vec3(0, NORMAL_GRAVITY, 0) * pStep;
	}
	else if (_grounded && _falling)
	{
		_physicsVelocity = glm::vec3(0, 0, 0);
	}


	if (!Timer::IsPaused())
	{
		//std::cout << "P: " << glm::round(_physicsVelocity) << " J: " << glm::round(_jumpVelocity) << " M: " << glm::round(_moveVelocity) << " R: " << glm::round(_moveVelocity + _jumpVelocity + _physicsVelocity) << std::endl;
		//std::cout << "G: " << _grounded << " W: " << _walled << " F: " << _falling << " J: " << _wallJumped << std::endl;
	}

	//Apply the velocity
	pController->GetRigidBody()->SetVelocity(Utility::glmToNe(_moveVelocity + _physicsVelocity));

	//Apply too slow feedback
	if (glm::length2(_lastPosition - _owner->getPosition()) > SLOW_FEEDBACK_RADIUS * SLOW_FEEDBACK_RADIUS) {
		_lastPosition = _owner->getPosition();
		_slowFeedbackTimer = 0.0f;
	} else {
		_slowFeedbackTimer += pStep;

		if (_slowFeedbackTimer >= SLOW_FEEDBACK_TIMER) {
			_slowFeedbackTimer = 0.0f;
			SoundEngine::PlayVoice("slow_feedback");
		}
	}

	//Apply Falling feedback
	if (_owner->getPosition().y < DEATH_HEIGHT) {
		_respawnTimer += pStep;

		if (!_dead)
		{
			_dead = true;
			SoundEngine::PlayVoice("fall");
		} else if(_respawnTimer >= RESPAWN_TIME) {
			SoundEngine::PlayVoice("fall_feedback");
			_physicsVelocity = glm::vec3(0,0,0);
			_moveVelocity = glm::vec3(0,0,0);
			pController->GetRigidBody()->SetVelocity(Utility::glmToNe(glm::vec3(0,0,0)));
			pController->GetRigidBody()->SetPos(Utility::glmToNe(SpawnPos));
			_respawnTimer = 0.0f;
			_dead = false;
		}
	}
}

void PlayerBehaviour::update(float pStep)
{
	//Calculate the mouse Vector
	_currMousePos = sf::Mouse::getPosition();
	_mouseDelta = glm::vec2(_currMousePos.x, _currMousePos.y) - glm::vec2(_prevMousePos.x, _prevMousePos.y);


	//Keep the mouse from leaving the center
	if (!Timer::IsPaused())
	{
		int screenHeight = sf::VideoMode::getDesktopMode().height;
		int screenWidth = sf::VideoMode::getDesktopMode().width;
		sf::Mouse::setPosition(sf::Vector2i(screenWidth / 2, screenHeight / 2));
	}


	//Update previous mouse position
	_prevMousePos = sf::Mouse::getPosition();


	//Updating the audio listener pos and dir
	sf::Listener::setPosition(_owner->getWorldPosition().x, _owner->getWorldPosition().y, _owner->getWorldPosition().z);
	sf::Listener::setDirection(_owner->getForwardVector().x, _owner->getForwardVector().y, -_owner->getForwardVector().z);
	sf::Listener::setUpVector(_camera->getUpVector().x, _camera->getUpVector().y, _camera->getUpVector().z);


	//Pause
	if (!_holdingPause && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) {
		Timer::TogglePause();
	}
	_holdingPause = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P);


	//Slowmotion
	if (!_holdingSlowmotion && sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
		Timer::Slowmotion = !Timer::Slowmotion;
	}
	_holdingSlowmotion = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
	
}