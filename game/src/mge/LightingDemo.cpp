#include <glm.hpp>
#include <iostream>
#include <string>
using namespace std;

#include "mge/core/Renderer.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/FPS.hpp"

#include "mge/core/Camera.hpp"
#include "mge/core/Light.hpp"

#include "mge/core/GameObject.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"

#include "mge/behaviours/PlayerBehaviour.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/LightingDemo.hpp"

#include "mge\core\Level.hpp"


//construct the game class into _window, _renderer and hud (other parts are initialized by build)
LightingDemo::LightingDemo():AbstractGame ()
{
}

void LightingDemo::initialize() {
    //setup the core part
    AbstractGame::initialize();

    //setup the custom part
	cout << "Initializing HUD" << endl;
	_hud = new DebugHud(_window);
	cout << "HUD initialized." << endl << endl;
}

//build the game _world
void LightingDemo::_initializeScene()
{
    _renderer->setClearColor(0,0,0);

	Level::Load("Level_04.xml", _world);

    Light* light = new Light("Light",glm::vec3(3.0f,3.0f,3.0f),glm::vec3(-2.0f,-1.0f,-3.0f),glm::vec3(1.0f,1.0f,1.0f),2.0f,60.0f,false);
    _world->add(light);
    _world->setLights(light);

    Camera* camera = new Camera ("camera", glm::vec3(0,0,0));
    _world->setMainCamera(camera);
	_world->add(camera);

	GameObject* player = new GameObject("player", glm::vec3(0, 2, -2), GameObject::RIGIDBODY, GameObject::CAPSULE);
	player->setParent(_world);
	player->SetPlayerPhysics();
	player->setMeshWithout(Mesh::load(config::MGE_MODEL_PATH + "cube.obj"));
	player->setMaterial(new ColorMaterial(glm::vec3(1,1,1)));
	player->setBehaviour(new PlayerBehaviour(camera, 2000, 40, 0.001, 10000));
	player->add(camera);

	//Mesh* m = Mesh::load(config::MGE_MODEL_PATH + "cube.obj");

	std::cout << _world->getChildAt(0)->getLocalPosition() << std::endl;

	/*
	GameObject* cube = new GameObject("cube", glm::vec3(0, 1, -3), GameObject::RIGIDBODY, GameObject::CUBE);
	_world->add(cube);
	cube->setMesh(m);
	cube->setMaterial(new ColorMaterial(glm::vec3(1, 1, 0)));

	GameObject* cube1 = new GameObject("cube", glm::vec3(1, 1, -3), GameObject::ANIMATEDBODY, GameObject::CUBE);
	_world->add(cube1);
	cube1->setMesh(m);
	cube->add(cube1);
	cube1->setMaterial(new ColorMaterial(glm::vec3(1, 1, 0)));
	*/

	//Player* player = new Player(_world, glm::vec3(0, 0, 0));
	//player->setBehaviour(new PlayerBehaviour(camera, 1, 1, 5));
}

void LightingDemo::_render() {
    AbstractGame::_render();
    _updateHud();

    //_world->renderDebugInfo();
}

void LightingDemo::_updateHud() {
    string debugInfo = "";
    debugInfo += string ("FPS:") + std::to_string(FPS::getFPS())+"\n";

    _hud->setDebugInfo(debugInfo);
    _hud->draw();
}


LightingDemo::~LightingDemo()
{
	//dtor
}
