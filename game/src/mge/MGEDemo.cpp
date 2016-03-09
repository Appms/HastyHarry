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
#include "mge/materials/TextureMaterial.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/LookAt.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/MGEDemo.hpp"


//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MGEDemo::MGEDemo():AbstractGame ()
{
}

MGEDemo::~MGEDemo()
{
	delete _hud;
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

    //SCENE SETUP

    GameObject* plane = new GameObject ("plane", glm::vec3(0,0,0));
    plane->scale(glm::vec3(5,5,5));
    plane->setMesh(planeMeshDefault);
    plane->setMaterial(textureMaterial);
    _world->add(plane);

    GameObject* teapot = new GameObject ("teapot", glm::vec3(-3,1,0));
    teapot->setMesh (teapotMeshS);
    teapot->setMaterial(textureMaterial2);
    teapot->setBehaviour (new KeysBehaviour());
    _world->add(teapot);

    GameObject* monkey = new GameObject ("monkey", glm::vec3(3,1,0));
    monkey->setMesh (suzannaMeshF);
    monkey->setMaterial(colorMaterial);
    monkey->setBehaviour (new RotatingBehaviour());
    _world->add(monkey);

    Camera* camera = new Camera ("camera", glm::vec3(0,6,7));
    camera->rotate(glm::radians(-40.0f), glm::vec3(1,0,0));
    camera->setBehaviour(new LookAt (teapot));
    _world->add(camera);
    _world->setMainCamera(camera);
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
