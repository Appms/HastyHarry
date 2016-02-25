#include "mge/PlayerControllerCB.hpp"
#include "mge/behaviours/PlayerBehaviour.hpp"

void PlayerControllerCB::RigidBodyControllerCallback(neRigidBodyController * controller, float timeStep)
{
	neRigidBody* rigidBody = controller->GetRigidBody();
	PlayerBehaviour* behaviour = (PlayerBehaviour *)(rigidBody->GetUserData());
	behaviour->PlayerController(controller, timeStep);
}