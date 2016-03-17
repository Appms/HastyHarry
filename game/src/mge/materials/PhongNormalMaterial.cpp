#include <glm.hpp>
#include <string>

#include "mge/materials/PhongNormalMaterial.hpp"
#include "mge/config.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/util/Utility.hpp"

ShaderProgram* PhongNormalMaterial::_shader = NULL;

GLint PhongNormalMaterial::_aVertex = 0;
GLint PhongNormalMaterial::_aNormal = 0;
GLint PhongNormalMaterial::_aTextureCoords0 = 0;
GLint PhongNormalMaterial::_aTangent = 0;

GLint PhongNormalMaterial::_uMVMatrix = 0;
GLint PhongNormalMaterial::_uProjectionMatrix = 0;
GLint PhongNormalMaterial::_uNormalMatrix = 0;

GLint PhongNormalMaterial::_uDiffuseTexture = 0;
GLint PhongNormalMaterial::_uNormalTexture = 0;
GLint PhongNormalMaterial::_uKa = 0;
GLint PhongNormalMaterial::_uKd = 0;
GLint PhongNormalMaterial::_uKs = 0;
GLint PhongNormalMaterial::_uShininess = 0;

GLint PhongNormalMaterial::_uLightPosition = 0;
GLint PhongNormalMaterial::_uLa = 0;
GLint PhongNormalMaterial::_uLd = 0;
GLint PhongNormalMaterial::_uLs = 0;

PhongNormalMaterial::PhongNormalMaterial(Texture* pDiffuseTexture, Texture* pNormalTexture, glm::vec3 pKa, glm::vec3 pKd, glm::vec3 pKs, float pShininess): _diffuseTexture (pDiffuseTexture), _normalTexture (pNormalTexture), _Ka(pKa), _Kd(pKd), _Ks(pKs), _shininess(pShininess)
{
    //every time we create an instance of colormaterial we check if the corresponding shader has already been loaded
    _lazyInitializeShader();
}

PhongNormalMaterial::PhongNormalMaterial(std::string params)
{

	std::vector<std::string> col = Utility::Split(params, ',');

	//TODO save every loaded texture and request existing
	//TODO Check image type

	_diffuseTexture = Texture::load(config::MGE_TEXTURE_PATH + col[0] + ".png");
	_normalTexture = Texture::load(config::MGE_TEXTURE_PATH + col[0] + "_NORMAL.png");
	_Ka = glm::vec3(0.2f, 0.2f, 0.2f);
	_Kd = glm::vec3(atof(col[1].c_str()), atof(col[2].c_str()), atof(col[3].c_str()));
	_Ks = glm::vec3(0.1f, 0.1f, 0.1f);
	_shininess = atof(col[4].c_str());

	_lazyInitializeShader();
}

void PhongNormalMaterial::_lazyInitializeShader() {
    //this shader contains everything the material can do (it can render something in 3d using a single color)
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH+"PhongNormal.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH+"PhongNormal.fs");
        _shader->finalize();

        //cache all the uniform and attribute indexes
        _aVertex =          _shader->getAttribLocation("VertexPosition");
        _aNormal =          _shader->getAttribLocation("VertexNormal");
        _aTextureCoords0 =  _shader->getAttribLocation("TextureCoords");
        _aTangent =         _shader->getAttribLocation("VertexTangent");

        _uMVMatrix =        _shader->getUniformLocation("ModelViewMatrix");
        _uProjectionMatrix =_shader->getUniformLocation("ProjectionMatrix");
        _uNormalMatrix =    _shader->getUniformLocation("NormalMatrix");

        _uLightPosition =   _shader->getUniformLocation("Light.Position");
        _uLa =              _shader->getUniformLocation("Light.La");
        _uLd =              _shader->getUniformLocation("Light.Ld");
        _uLs =              _shader->getUniformLocation("Light.Ls");

        _uDiffuseTexture =  _shader->getUniformLocation("Material.DiffuseTexture");
        _uNormalTexture =   _shader->getUniformLocation("Material.NormalTexture");
        _uKa =              _shader->getUniformLocation("Material.Ka");
        _uKd =              _shader->getUniformLocation("Material.Kd");
        _uKs =              _shader->getUniformLocation("Material.Ks");
        _uShininess =       _shader->getUniformLocation("Material.Shininess");
    }
}

PhongNormalMaterial::~PhongNormalMaterial()
{
	delete _diffuseTexture;
	delete _normalTexture;
}

void PhongNormalMaterial::setDiffuseTexture(Texture* pDiffuseTexture) {
    _diffuseTexture = pDiffuseTexture;
}

void PhongNormalMaterial::setNormalTexture(Texture* pNormalTexture) {
    _normalTexture = pNormalTexture;
}

void PhongNormalMaterial::render(World* pWorld, GameObject* pGameObject, Camera* pCamera) {
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

	Light* light = Light::GetLight(0);
    glUniform4fv(_uLightPosition, 1, glm::value_ptr(light->GetPosition()));
    glUniform3fv(_uLa, 1, glm::value_ptr(glm::vec3(1.0f,1.0f,1.0f)));
    glUniform3fv(_uLd, 1, glm::value_ptr(glm::vec3(1.0f,1.0f,1.0f)));
    glUniform3fv(_uLs, 1, glm::value_ptr(glm::vec3(1.0f,1.0f,1.0f)));

    //MATERIAL

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
    glUniform1i (_uDiffuseTexture, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _normalTexture->getId());
    glUniform1i (_uNormalTexture, 1);

    glUniform3fv(_uKa, 1, glm::value_ptr(_Ka));
    glUniform3fv(_uKd, 1, glm::value_ptr(_Kd));
    glUniform3fv(_uKs, 1, glm::value_ptr(_Ks));
    glUniform1f(_uShininess, _shininess);

    //ATTRIBUTES

    pGameObject->getMesh()->streamToOpenGL(_aVertex, _aNormal, _aTextureCoords0, _aTangent);
    //pGameObject->getMesh()->renderDebugInfo(pGameObject->getWorldTransform(), pWorld);
}
