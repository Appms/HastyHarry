#include "mge/behaviours/Turret.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/config.hpp"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/core/Level.hpp"
#include "mge/core/World.hpp"
#include "mge/behaviours/MovingBehaviour.hpp"

Turret::Turret(GameObject * pTarget, float pRadius): AbstractBehaviour(), _target(pTarget), _radius(pRadius) {}

Turret::Turret(std::string params) : AbstractBehaviour()
{
	_target = Level::CurrentPlayer;
	_radius = atof(params.c_str());
}

Turret::~Turret()
{
	_target = NULL;
}

void Turret::update( float step )
{
	if (glm::distance(_owner->getWorldPosition(), _target->getWorldPosition()) < _radius) {

 		glm::vec3 forward = glm::normalize(_target->getWorldPosition() - _owner->getWorldPosition());
		glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
		glm::vec3 up = glm::normalize(glm::cross(forward, right));

		_owner->setTransform(
			glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(_owner->getLocalPosition(), 1))
			);

		_timer += step;
		if (_timer >= 1.0f) {
			_timer -= 1.0f;

			Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "Syringe.obj");
			AbstractMaterial* textureMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "SyringUV.png"));
			
			GameObject* projectile = new GameObject("Projectile", _owner->getWorldPosition(), GameObject::PhysicsType::ANIMATEDBODY, GameObject::ColliderType::CUBE);
			Level::CurrentWorld->add(projectile);
			projectile->setMeshWithCollider(planeMeshDefault);
			projectile->setMaterial(textureMaterial);
			projectile->setTransform(
				glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(_owner->getWorldPosition(), 1))
				);
			projectile->setBehaviour(new MovingBehaviour(_owner->getWorldPosition(), _owner->getWorldPosition() + forward * _radius, 20.0f, false));
			((MovingBehaviour*)projectile->getBehaviour())->setDestroyFlag();
			//add sound trigger*/
		}
	}
}
