#include <glm.hpp>

#include "mge/core/Timer.hpp"
#include "mge/materials/AnimatedTextureMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/config.hpp"

ShaderProgram* AnimatedTextureMaterial::_shader = NULL;

AnimatedTextureMaterial::AnimatedTextureMaterial(std::vector<Texture*> pTextures):_diffuseTextures(pTextures) {
	_currentTexture = _diffuseTextures[0];
    _lazyInitializeShader();
}

AnimatedTextureMaterial::~AnimatedTextureMaterial() {
	for each (Texture* text in _diffuseTextures)
	{
		delete text;
	}
	_diffuseTextures.clear();
}

void AnimatedTextureMaterial::_lazyInitializeShader() {
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH+"texture.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH+"texture.fs");
        _shader->finalize();
    }
}

void AnimatedTextureMaterial::render(World* pWorld, GameObject* pGameObject, Camera* pCamera) {
    if (!_currentTexture) return;

    _shader->use();

	_timer += Timer::deltaTime();
	if (_timer > 0.1f) {
		_timer -= 0.1f;
		_textureIterator = (_textureIterator + 1) % _diffuseTextures.size();
		_currentTexture = _diffuseTextures[_textureIterator];
	}

    //setup texture slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _currentTexture->getId());
    glUniform1i (_shader->getUniformLocation("textureDiffuse"), 0);

    //pass in all MVP matrices separately
    glUniformMatrix4fv ( _shader->getUniformLocation("projectionMatrix"),   1, GL_FALSE, glm::value_ptr(pCamera->getProjection()));
    glUniformMatrix4fv ( _shader->getUniformLocation("viewMatrix"),         1, GL_FALSE, glm::value_ptr(glm::inverse(pCamera->getWorldTransform())));
    glUniformMatrix4fv ( _shader->getUniformLocation("modelMatrix"),        1, GL_FALSE, glm::value_ptr(pGameObject->getWorldTransform() ) );

    //now inform mesh of where to stream its data
    pGameObject->getMesh()->streamToOpenGL(
        _shader->getAttribLocation("vertex"),
        _shader->getAttribLocation("normal"),
        _shader->getAttribLocation("uv")
    );
}
