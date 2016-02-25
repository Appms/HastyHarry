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
#include "materials\TextureMaterial.hpp"
#include "materials\PhongMaterial.hpp"

#include "mge/behaviours/PlayerBehaviour.hpp"
#include "behaviours\LookAt.hpp"
#include "behaviours\SoundBehaviour.hpp"

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

	//Level::Load("Level_04.xml", _world);

	Mesh* cubeMesh = Mesh::load(config::MGE_MODEL_PATH + "cube.obj");
	Mesh* monkeyMesh = Mesh::load(config::MGE_MODEL_PATH + "suzanna_smooth.obj");

	AbstractMaterial* textureMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Normal.png"));
	AbstractMaterial* phongMaterial = new PhongMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.1f, 0.1f, 0.1f), 2.0f);

	GameObject* cube = new GameObject("ground", glm::vec3(0,-2,0), GameObject::PhysicsType::ANIMATEDBODY);
	_world->add(cube);
	cube->scale(glm::vec3(50,1,50));
	cube->setMesh(cubeMesh);
	cube->setMaterial(textureMaterial);

	//Init Light
    Light* light = new Light("Light",glm::vec3(3.0f,3.0f,3.0f),glm::vec3(-2.0f,-1.0f,-3.0f),glm::vec3(1.0f,1.0f,1.0f),2.0f,60.0f,false);
    _world->add(light);
    _world->setLights(light);

	//Init Camera
    Camera* camera = new Camera ("camera", glm::vec3(0,0,0));
    _world->setMainCamera(camera);
	_world->add(camera);
	
	//Init Player
	GameObject* player = new GameObject("player", glm::vec3(0, 3, -2), GameObject::RIGIDBODY, GameObject::CAPSULE);
	player->setParent(_world);
	player->setBehaviour(new PlayerBehaviour(camera, 500.0f, 40.0f, 0.1f, 5.0f));
	((PlayerBehaviour *)player->getBehaviour())->Initialize();

	GameObject* monkey = new GameObject("monkey", glm::vec3(0, 0, 0), GameObject::PhysicsType::ANIMATEDBODY);
	_world->add(monkey);
	monkey->setMesh(monkeyMesh);
	monkey->setMaterial(phongMaterial);
	monkey->setBehaviour(new SoundBehaviour("jump.wav", glm::vec3(0,0,0)));
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
