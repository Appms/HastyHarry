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

LightingDemo::~LightingDemo()
{
	delete _hud;
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

	Level::Load("Goal.xml", _world);

	/*Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
	AbstractMaterial* textureMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "Normal2.png"));
	GameObject* plane = new GameObject("plane", glm::vec3(88, 10, 130), GameObject::PhysicsType::ANIMATEDBODY);
	_world->add(plane);
	plane->scale(glm::vec3(20, 1, 20));
	plane->setMesh(planeMeshDefault);
	plane->setMaterial(textureMaterial);
	//plane->setBehaviour(new LookAt());*/

	//Init Light
    Light* light = new Light("Light",glm::vec3(3.0f,3.0f,3.0f),glm::vec3(-2.0f,-1.0f,-3.0f),glm::vec3(1.0f,1.0f,1.0f),2.0f,60.0f,false);
    _world->add(light);
    _world->setLights(light);
}

void LightingDemo::_render() {
    AbstractGame::_render();
    _updateHud();

    _world->renderDebugInfo();
}

void LightingDemo::_updateHud() {
    string debugInfo = "";
    debugInfo += string ("FPS:") + std::to_string(FPS::getFPS())+"\n";

    _hud->setDebugInfo(debugInfo);
    _hud->draw();
}
