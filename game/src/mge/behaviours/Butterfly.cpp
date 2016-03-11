#include <glm.hpp>
#include "mge/behaviours/Butterfly.hpp"
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

Butterfly::Butterfly(GameObject * pTarget, float pRadius, glm::vec3 pStart, MovingBehaviour * pMovement, std::vector<glm::vec3> pWaypoints): AbstractBehaviour()
{
	_target = pTarget;
	_radius = pRadius; 
	_movement = pMovement; 
	_waypoints = pWaypoints;
	_currentObjective = pStart;
}

Butterfly::~Butterfly()
{
	_target = NULL;
}

/**
 * Looks at the given target
 */
void Butterfly::update( float step )
{

	glm::vec3 forward = glm::normalize(_owner->getLocalPosition() - _target->getLocalPosition());
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
	glm::vec3 up = glm::normalize(glm::cross(forward, right));

	_owner->setTransform(
		glm::mat4(glm::vec4(right, 0), glm::vec4(forward, 0), glm::vec4(up, 0), glm::vec4(_owner->getLocalPosition(), 1))
	);

	if (_active) {
		if (glm::distance(_owner->getWorldPosition(), _target->getWorldPosition()) < _radius) {
			_movement->updateValues(_owner->getWorldPosition(), _waypoints.back(), 1.0f, false);
			_currentObjective = _waypoints.back();
			_waypoints.pop_back();

			_active = false;
		}
	}
	else if (glm::distance(_owner->getWorldPosition(), _currentObjective) < _radius && _waypoints.size() > 0) _active = true;

	
}
