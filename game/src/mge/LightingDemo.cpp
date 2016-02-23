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
#include "mge/materials/TextureMaterial.hpp"
#include "mge/materials/LitTextureMaterial.hpp"
#include "mge/materials/PhongMaterial.hpp"
#include "mge/materials/PhongNormalMaterial.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/LookAt.hpp"

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

    //MESHES

    //load a bunch of meshes we will be using throughout this demo
    //each mesh only has to be loaded once, but can be used multiple times:
    //F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
    Mesh* planeMeshDefault = Mesh::load (config::MGE_MODEL_PATH+"plane.obj");
    Mesh* cubeMeshF = Mesh::load (config::MGE_MODEL_PATH+"cube_flat.obj");
    Mesh* suzannaMeshF = Mesh::load (config::MGE_MODEL_PATH+"suzanna_flat.obj");
    Mesh* teapotMeshS = Mesh::load (config::MGE_MODEL_PATH+"teapot_smooth.obj");

    //MATERIALS

    AbstractMaterial* colorMaterial = new ColorMaterial (glm::vec3(0.2f,0,0.2f));
    AbstractMaterial* textureMaterial = new TextureMaterial (Texture::load (config::MGE_TEXTURE_PATH+"land.jpg"));
    AbstractMaterial* textureMaterial2 = new TextureMaterial (Texture::load (config::MGE_TEXTURE_PATH+"bricks.jpg"));
    AbstractMaterial* litTextureMaterial = new LitTextureMaterial (Texture::load (config::MGE_TEXTURE_PATH+"bricks.jpg"), 1.0f, glm::vec3(1.0f,1.0f,1.0f));
    AbstractMaterial* phongMaterial = new PhongMaterial (Texture::load(config::MGE_TEXTURE_PATH+"bricks.jpg"), glm::vec3(0.2f,0.2f,0.2f), glm::vec3(0.8f,0.8f,0.8f), glm::vec3(0.1f,0.1f,0.1f), 2.0f);
    AbstractMaterial* phongNormalMaterial = new PhongNormalMaterial (Texture::load(config::MGE_TEXTURE_PATH+"bricks.jpg"), Texture::load(config::MGE_TEXTURE_PATH+"Normal.png"), glm::vec3(0.2f,0.2f,0.2f), glm::vec3(0.8f,0.8f,0.8f), glm::vec3(0.1f,0.1f,0.1f), 2.0f);
	
	//SCENE SETUP

	//Level::Load("Level_01.xml", _world);

	for (int i = 0; i < 50; i++) {
		float rx = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float ry = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float rz = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		GameObject* cube = new GameObject("teapot", glm::vec3(-3.0f + rx * 6.0f, 3.0f + (float)(i % 3), -3.0f + rz * 6.0f), GameObject::PhysicsType::RIGIDBODY, GameObject::ColliderType::CUBE);
		_world->add(cube);
		cube->scale(glm::vec3(0.2 + rx * 0.2f, 0.2 + ry * 0.2f, 0.2 + rz * 0.2f));
		cube->setMesh(cubeMeshF);
		cube->setMaterial(phongNormalMaterial);
		//cube->setBehaviour(new KeysBehaviour());
	}

    GameObject* plane = new GameObject ("plane", glm::vec3(0,0,0), GameObject::PhysicsType::ANIMATEDBODY, GameObject::ColliderType::CUBE);
	_world->add(plane);
	plane->scale(glm::vec3(20,1,20));
    plane->setMesh(planeMeshDefault);
    plane->setMaterial(phongNormalMaterial);

    GameObject* teapot = new GameObject ("teapot", glm::vec3(0, 10, 0), GameObject::PhysicsType::RIGIDBODY, GameObject::ColliderType::CAPSULE);
	_world->add(teapot);
	teapot->scale(glm::vec3(0.2, 0.2, 0.2));
	teapot->setMesh (teapotMeshS);
    teapot->setMaterial(phongNormalMaterial);
    teapot->setBehaviour (new KeysBehaviour(5.0f, 45.0f, config::MGE_AUDIO_PATH + "jump.wav"));
	sf::Listener::setPosition(0, 10, 0);
	sf::Listener::setDirection(0,0,1);

    /*GameObject* monkey = new GameObject ("monkey", glm::vec3(0,8,0), GameObject::PhysicsType::RIGIDBODY);
	_world->add(monkey);
	monkey->scale(glm::vec3(3, 1, 1));
	monkey->setMesh (suzannaMeshF);
    monkey->setMaterial(phongNormalMaterial);
    //monkey->setBehaviour (new RotatingBehaviour());*/

    Light* light = new Light("Light",glm::vec3(3.0f,3.0f,3.0f),glm::vec3(-2.0f,-1.0f,-3.0f),glm::vec3(1.0f,1.0f,1.0f),2.0f,60.0f,false);
    _world->add(light);
    _world->setLights(light);

    //Camera* camera = new Camera ("camera", glm::vec3(-32,125,-53));
	Camera* camera = new Camera("camera", glm::vec3(0, 10, 10));
    camera->rotate(glm::radians(-40.0f), glm::vec3(1,0,0));
    camera->setBehaviour(new LookAt (teapot));
	//camera->setBehaviour(new KeysBehaviour());
    _world->add(camera);
    _world->setMainCamera(camera);
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
