#include <cassert>
#include <algorithm>
#include <iostream>
#include "mge/core/World.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Mesh.hpp"

using namespace std;

World::World():GameObject("root"), _mainCamera(0), _sceneLights(0), _physicsWorld(NULL)
{
	//ctor
	GameObject::_world = this;
	initPhysics();
}

void World::setMainCamera (Camera* pCamera) {
    if (pCamera != NULL) _mainCamera = pCamera;
}

Camera* World::getMainCamera () {
    return _mainCamera;
}

void World::setLights(Light* pLight) {
    if (pLight != NULL) _sceneLights = pLight;
}

Light* World::getLights(int index) {
    return _sceneLights->GetLight(index);
}

bool World::initPhysics() {
	neV3 gravity;		// A vector to store the direction and intensity of gravity
	neSimulatorSizeInfo sizeInfo;	// SizeInfo stores data about how many objects we are going to model

	// Create and initialise the simulator
	// Tell the simulator how many rigid bodies we have
	sizeInfo.rigidBodiesCount = 500;
	// Tell the simulator how many animated bodies we have
	sizeInfo.animatedBodiesCount = 500;
	//Tell the simulator how many sensors we have
	sizeInfo.sensorsCount = 100;
	// Tell the simulator how many bodies we have in total
	s32 totalBody = sizeInfo.rigidBodiesCount + sizeInfo.animatedBodiesCount;
	sizeInfo.geometriesCount = totalBody;
	// The overlapped pairs count defines how many bodies it is possible to be in collision
	// at a single time. The SDK states this should be calculated as:
	//   bodies * (bodies-1) / 2
	sizeInfo.overlappedPairsCount = totalBody * (totalBody - 1) / 2;
	// We're not using any of these so set them all to zero
	sizeInfo.rigidParticleCount = 0;
	sizeInfo.constraintsCount = 0;
	sizeInfo.terrainNodesStartCount = 0;

	// Set the gravity. Try changing this to see the effect on the objects
	gravity.Set(0.0f, -10.0f, 0.0f);

	// Ready to go, create the simulator object
	_physicsWorld = neSimulator::CreateSimulator(sizeInfo, NULL, &gravity);

	// Now we need to add some other elements to the physics engine, namely
	// the cubes and the floor.

	// All done
	return true;
}

neSimulator* World::getPhysics() {
	return _physicsWorld;
}

void World::killPhysics(void) {
	if (_physicsWorld)
	{
		// Destroy the simulator.
		// Note that this will release all related resources that we've allocated.
		neSimulator::DestroySimulator(_physicsWorld);
		_physicsWorld = NULL;
	}
}

void World::renderDebugInfo() {
    //walk through all meshes and debug their normals
    GameObject* gameObject;
    for (int i = 0; i < getChildCount(); i++) {
        gameObject = getChildAt(i);
        //oo wise it would be better to do gameObject->renderDebugInfo, but since this is only for debugging
        //it's sort of pasted on
        if (gameObject->getMaterial() && gameObject->getMesh()) {
            gameObject->getMesh()->renderDebugInfo(gameObject->getWorldTransform(), this);
        }
    }
}

