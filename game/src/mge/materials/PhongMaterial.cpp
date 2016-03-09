#include <glm.hpp>
#include <string>

#include "mge/materials/PhongMaterial.hpp"
#include "mge/config.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Level.hpp"
#include "mge/util/Utility.hpp"

ShaderProgram* PhongMaterial::_shader = NULL;

GLint PhongMaterial::_aVertex = 0;
GLint PhongMaterial::_aNormal = 0;
GLint PhongMaterial::_aTextureCoords0 = 0;

GLint PhongMaterial::_uMVMatrix = 0;
GLint PhongMaterial::_uProjectionMatrix = 0;
GLint PhongMaterial::_uNormalMatrix = 0;

GLint PhongMaterial::_uDiffuseTexture = 0;
GLint PhongMaterial::_uKa = 0;
GLint PhongMaterial::_uKd = 0;
GLint PhongMaterial::_uKs = 0;
GLint PhongMaterial::_uShininess = 0;

GLint PhongMaterial::_uLightPosition = 0;
GLint PhongMaterial::_uLa = 0;
GLint PhongMaterial::_uLd = 0;
GLint PhongMaterial::_uLs = 0;

PhongMaterial::PhongMaterial(Texture* pDiffuseTexture, glm::vec3 pKa, glm::vec3 pKd, glm::vec3 pKs, float pShininess): _diffuseTexture (pDiffuseTexture), _Ka(pKa), _Kd(pKd), _Ks(pKs), _shininess(pShininess)
{
    //every time we create an instance of colormaterial we check if the corresponding shader has already been loaded
    _lazyInitializeShader();
}

PhongMaterial::PhongMaterial(std::string params)
{
	
	std::vector<std::string> col = Utility::Split(params, ',');

	//TODO save every loaded texture and request existing
	//TODO Check image type

	_diffuseTexture = Texture::load(config::MGE_TEXTURE_PATH+col[0]+".png");
	_Ka = glm::vec3(1, 1, 1) * 0.005f;
	_Kd = glm::vec3(atof(col[1].c_str()), atof(col[2].c_str()), atof(col[3].c_str()));
	_Ks = glm::vec3(1, 1, 1);
	_shininess = atof(col[4].c_str());

	_lazyInitializeShader();
}

void PhongMaterial::_lazyInitializeShader() {
    //this shader contains everything the material can do (it can render something in 3d using a single color)
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH+"Phong.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH+"Phong.fs");
        _shader->finalize();

        //cache all the uniform and attribute indexes
        _aVertex =          _shader->getAttribLocation("VertexPosition");
        _aNormal =          _shader->getAttribLocation("VertexNormal");
        _aTextureCoords0 =  _shader->getAttribLocation("TextureCoords0");

        _uMVMatrix =        _shader->getUniformLocation("modelViewMatrix");
        _uProjectionMatrix =_shader->getUniformLocation("projectionMatrix");
        _uNormalMatrix =    _shader->getUniformLocation("normalMatrix");

        _uLightPosition =   _shader->getUniformLocation("Light.Position");
        _uLa =              _shader->getUniformLocation("Light.La");
        _uLd =              _shader->getUniformLocation("Light.Ld");
        _uLs =              _shader->getUniformLocation("Light.Ls");

        _uDiffuseTexture =  _shader->getUniformLocation("Material.DiffuseTexture");
        _uKa =              _shader->getUniformLocation("Material.Ka");
        _uKd =              _shader->getUniformLocation("Material.Kd");
        _uKs =              _shader->getUniformLocation("Material.Ks");
        _uShininess =       _shader->getUniformLocation("Material.Shininess");
    }
}

PhongMaterial::~PhongMaterial()
{
	delete _diffuseTexture;
}

void PhongMaterial::setDiffuseTexture(Texture* pDiffuseTexture) {
    _diffuseTexture = pDiffuseTexture;
}

void PhongMaterial::render(World* pWorld, GameObject* pGameObject, Camera* pCamera) {
    _shader->use();

    //MATRIXES
    glm::mat4 MVMatrix = glm::inverse(pCamera->getWorldTransform()) * pGameObject->getWorldTransform();
    glm::mat4 projectionMatrix = pCamera->getProjection();
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(MVMatrix)));

    glUniformMatrix4fv( _uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv( _uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix3fv( _uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    //LIGHT

    //ADD METHODS TO LIGHTS TO ADD DIFFERENT LA,LD,LS

    Light* light = pWorld->getLights(0);
    glUniform4fv(_uLightPosition, 1, glm::value_ptr(light->GetPosition()));
    glUniform3fv(_uLa, 1, glm::value_ptr(glm::vec3(1.0f,1.0f,1.0f)));
    glUniform3fv(_uLd, 1, glm::value_ptr(glm::vec3(1.0f,1.0f,1.0f)));
    glUniform3fv(_uLs, 1, glm::value_ptr(glm::vec3(1.0f,1.0f,1.0f)));

    //MATERIAL

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
    glUniform1i (_uDiffuseTexture, 0);

    glUniform3fv(_uKa, 1, glm::value_ptr(_Ka));
    glUniform3fv(_uKd, 1, glm::value_ptr(_Kd));
    glUniform3fv(_uKs, 1, glm::value_ptr(_Ks));
    glUniform1f(_uShininess, _shininess);

    //ATTRIBUTES

    pGameObject->getMesh()->streamToOpenGL(_aVertex, _aNormal, _aTextureCoords0);
    //pGameObject->getMesh()->renderDebugInfo(pGameObject->getWorldTransform(), pWorld);
}
