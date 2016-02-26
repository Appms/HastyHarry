#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

#include "mge/core/Texture.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include <glm.hpp>
#include <string>

class PhongMaterial : public AbstractMaterial
{
    public:
        PhongMaterial(Texture* pDiffuseTexture, glm::vec3 pKa = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 pKd = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 pKs = glm::vec3(1.0f, 1.0f, 1.0f), float pShininess = 1);
		PhongMaterial(std::string params);
		virtual ~PhongMaterial();
        virtual void render(World* pWorld, GameObject* pGameObject, Camera* pCamera);

        //in rgb values
        void setDiffuseTexture (Texture* pDiffuseTexture);

    private:

        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

        //ATTRIBUTES
        static GLint _aVertex;
        static GLint _aNormal;
        static GLint _aTextureCoords0;

        //UNIFORMS
        static GLint _uMVMatrix;
        static GLint _uProjectionMatrix;
        static GLint _uNormalMatrix;

        static GLint _uLightPosition;
        static GLint _uLa;
        static GLint _uLd;
        static GLint _uLs;

        static GLint _uDiffuseTexture;
        static GLint _uKa;
        static GLint _uKd;
        static GLint _uKs;
        static GLint _uShininess;

        //NON-STATIC UNIFORMS
        Texture* _diffuseTexture;
        glm::vec3 _Ka;
        glm::vec3 _Kd;
        glm::vec3 _Ks;
        float _shininess;

};

#endif // PHONGMATERIAL_H
