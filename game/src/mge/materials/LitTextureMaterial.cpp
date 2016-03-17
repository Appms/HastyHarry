#include <glm.hpp>
#include <string>

#include "mge/materials/LitTextureMaterial.hpp"
#include "mge/config.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"

ShaderProgram* LitTextureMaterial::_shader = NULL;

GLint LitTextureMaterial::_uMVPMatrix = 0;
GLint LitTextureMaterial::_uMMatrix = 0;
GLint LitTextureMaterial::_uDiffuseTexture = 0;
GLint LitTextureMaterial::_uCameraPosition = 0;
GLint LitTextureMaterial::_uShininess = 0;
GLint LitTextureMaterial::_uSpecularColor = 0;

GLint LitTextureMaterial::_aVertex = 0;
GLint LitTextureMaterial::_aNormal = 0;
GLint LitTextureMaterial::_aUV = 0;

LitTextureMaterial::LitTextureMaterial(Texture* pDiffuseTexture, float pShininess, glm::vec3 pSpecularColor): _diffuseTexture (pDiffuseTexture), _shininess(pShininess), _specularColor(pSpecularColor)
{
    //every time we create an instance of colormaterial we check if the corresponding shader has already been loaded
    _lazyInitializeShader();
}

void LitTextureMaterial::_lazyInitializeShader() {
    //this shader contains everything the material can do (it can render something in 3d using a single color)
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH+"litTexture.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH+"litTexture.fs");
        _shader->finalize();



        //cache all the uniform and attribute indexes
        _aVertex =          _shader->getAttribLocation("vertex");
        _aNormal =          _shader->getAttribLocation("normal");
        _aUV =              _shader->getAttribLocation("uv");

        _uMVPMatrix=        _shader->getUniformLocation("MVP");


        _uMMatrix=          _shader->getUniformLocation("modelMatrix");
        _uCameraPosition =  _shader->getUniformLocation("cameraPosition");

        _uShininess =       _shader->getUniformLocation("materialShininess");
        _uSpecularColor =   _shader->getUniformLocation("materialSpecularColor");
        _uDiffuseTexture =  _shader->getUniformLocation("diffuseTexture");
    }
}

LitTextureMaterial::~LitTextureMaterial()
{
	delete _diffuseTexture;
}

void LitTextureMaterial::setDiffuseTexture(Texture* pDiffuseTexture) {
    _diffuseTexture = pDiffuseTexture;
}

void LitTextureMaterial::render(World* pWorld, GameObject* pGameObject, Camera* pCamera) {
    _shader->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
    glUniform1i (_uDiffuseTexture, 0);

    //pass in a precalculate mvp matrix (see texture material for the opposite)
    glm::mat4 mMatrix = pGameObject->getWorldTransform();
    glm::mat4 mvpMatrix = pCamera->getProjection() * glm::inverse(pCamera->getWorldTransform()) * pGameObject->getWorldTransform();

    glUniformMatrix4fv ( _uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
    glUniformMatrix4fv ( _uMMatrix, 1, GL_FALSE, glm::value_ptr(mMatrix));

    //set the material attributes
    glUniform3fv (_uSpecularColor, 1, glm::value_ptr(_specularColor));
    glUniform1f (_uShininess, _shininess);

    //set the camera position
    //TODO this might cause trouble when set as child
    glm::vec3 camPos = pCamera->getLocalPosition();
    glUniform3f (_uCameraPosition, camPos.x, camPos.y, camPos.z);

    glUniform1f(_shader->getUniformLocation("ambientCoefficient"), Light::GetAmbientCoefficient());
    glUniform1i(_shader->getUniformLocation("numLights"), Light::GetLightCount());

    for(int i = 0; i < Light::GetLightCount(); i++)
    {
        Light* light = Light::GetLight(i);

        glUniform4fv(_shader->getUniformLocation("lights["+ std::to_string(i) +"].position"), 1, glm::value_ptr(light->GetPosition()));
        glUniform3fv(_shader->getUniformLocation("lights["+ std::to_string(i) +"].intensities"), 1, glm::value_ptr(light->GetColor()));
        glUniform3fv(_shader->getUniformLocation("lights["+ std::to_string(i) +"].direction"), 1, glm::value_ptr(light->GetDirection()));

        glUniform1f(_shader->getUniformLocation("lights["+ std::to_string(i) +"].attenuation"), light->GetIntensity());
        glUniform1f(_shader->getUniformLocation("lights["+ std::to_string(i) +"].coneAngle"), light->GetRange());
    }


    //now inform mesh of where to stream its data
    pGameObject->getMesh()->streamToOpenGL(_aVertex, _aNormal, _aUV);
}
