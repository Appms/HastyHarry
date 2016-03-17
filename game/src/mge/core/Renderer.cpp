#include "mge/core/Renderer.hpp"
#include <cassert>
#include <GL/glew.h>

#include "mge/core/GameObject.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Camera.hpp"
#include "mge/materials/AbstractMaterial.hpp"

#include <iostream>
using namespace std;

Renderer::Renderer()
{
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE ); // default GL_BACK
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation (GL_FUNC_ADD);
	glClearColor((float)0x2d/0xff, (float)0x6b/0xff, (float)0xce/0xff, 1.0f );
}

Renderer::~Renderer()
{
}

void Renderer::setClearColor(int pR, int pG, int pB) {
    glClearColor((float)pR/0xff, (float)pG/0xff, (float)pB/0xff, 1.0f );
}

void Renderer::render (World* pWorld) {
    render (pWorld, pWorld, pWorld->getMainCamera(), true);
}

void Renderer::render (World* pWorld, GameObject * pGameObject, Camera * pCamera, bool pRecursive)
{
    AbstractMaterial* material = pGameObject->getMaterial();

    //our material (shader + settings) determines how we actually look
    if (pGameObject->getMesh() && material != NULL) {
        material->render(pWorld, pGameObject, pCamera);
    }

    if (!pRecursive) return;

    int childCount = pGameObject->getChildCount();
    if (childCount < 1) return;

    //note that with a loop like this, deleting children during rendering is not a good idea :)
    for (int i = 0; i < childCount; i++) {
        render (pWorld, pGameObject->getChildAt(i), pCamera, pRecursive);
    }
}

