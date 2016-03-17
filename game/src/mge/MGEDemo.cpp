#include <glm.hpp>
#include <iostream>
#include <string>
using namespace std;

#include "mge/core/Renderer.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/FPS.hpp"

#include "mge/core/Camera.hpp"

#include "mge/core/GameObject.hpp"
#include "mge/materials/AbstractMaterial.hpp"

#include "mge/materials/ColorMaterial.hpp"
#include "materials\PhongMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/LookAt.hpp"
#include "behaviours\TestBehaviour.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/MGEDemo.hpp"
#include "core\Light.hpp"


//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MGEDemo::MGEDemo():AbstractGame ()
{
}

void MGEDemo::initialize() {
    //setup the core part
    AbstractGame::initialize();

    //setup the custom part
	cout << "Initializing HUD" << endl;
	_hud = new DebugHud(_window);
	cout << "HUD initialized." << endl << endl;
}

//build the game _world
void MGEDemo::_initializeScene()
{
    _renderer->setClearColor(0,0,0);

    Mesh* testMesh = Mesh::load ("test.obj");
	AbstractMaterial* testMat = new PhongMaterial(Texture::load("test.png"),glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(0.1f), 2.0f);

    //SCENE SETUP

    GameObject* test = new GameObject ("Test", glm::vec3(0,0,0));
    test->setMesh(testMesh);
	test->setMaterial(testMat);
	test->setBehaviour(new TestBehaviour());
    _world->add(test);

    Camera* camera = new Camera ("camera", glm::vec3(0,3,3.5));
    camera->rotate(glm::radians(-40.0f), glm::vec3(1,0,0));
    _world->add(camera);
    _world->setMainCamera(camera);

	//Light* light = new Light("Light", glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(-2.0f, -1.0f, -3.0f), glm::vec3(1.0f, 1.0f, 1.0f), 2.0f, 60.0f, false);
	//_world->add(light);
	//_world->setLights(light);
}

void MGEDemo::_render() {
    AbstractGame::_render();
    _updateHud();

    //_world->renderDebugInfo();
}

void MGEDemo::_updateHud() {
    string debugInfo = "";
    debugInfo += string ("FPS:") + std::to_string(FPS::getFPS())+"\n";

    _hud->setDebugInfo(debugInfo);
    _hud->draw();
}


MGEDemo::~MGEDemo()
{
	delete _hud;
}
