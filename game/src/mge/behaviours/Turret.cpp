#include <glm.hpp>
#include "mge/behaviours/Turret.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Timer.hpp"
#include "mge/config.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge\core\Level.hpp"
#include "mge/core/World.hpp"
#include "MovingBehaviour.hpp"

#include <iostream>
using namespace std;

Turret::Turret(GameObject * pTarget, float pRadius): AbstractBehaviour(), _target(pTarget), _radius(pRadius)
{
}

Turret::~Turret()
{
	_target = NULL;
}

/**
 * Looks at the given target
 */
void Turret::update( float step )
{

	if (glm::distance(_owner->getWorldPosition(), _target->getWorldPosition()) < _radius) {
		glm::vec3 forward = glm::normalize(_owner->getLocalPosition() - _target->getLocalPosition());
		glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
		glm::vec3 up = glm::normalize(glm::cross(forward, right));

		_owner->setTransform(
			glm::mat4(glm::vec4(right, 0), glm::vec4(forward, 0), glm::vec4(up, 0), glm::vec4(_owner->getLocalPosition(), 1))
			);

		_timer += step;
		if (_timer >= 3.0f) {
			_timer -= 3.0f;

			Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "sphere_smooth.obj");
			AbstractMaterial* textureMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "land.png"));
			
			GameObject* projectile = new GameObject("Projectile", _owner->getWorldPosition(), GameObject::PhysicsType::ANIMATEDBODY, GameObject::ColliderType::SPHERE);
			Level::CurrentWorld->add(projectile);
			projectile->setMeshWithCollider(planeMeshDefault);
			projectile->setMaterial(textureMaterial);
			projectile->setBehaviour(new MovingBehaviour(_owner->getWorldPosition(), _owner->getWorldPosition() - forward * 10.0f, 0.5f, false));
			//add sound trigger
		}
	}
}