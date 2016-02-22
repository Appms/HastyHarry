#ifndef LITTEXTUREMATERIAL_H
#define LITTEXTUREMATERIAL_H

#include "mge/core/Texture.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include <glm.hpp>

class LitTextureMaterial : public AbstractMaterial
{
    public:
        LitTextureMaterial(Texture* pDiffuseTexture, float pShininess = 1, glm::vec3 pSpecularColor = glm::vec3(1.0f,1.0f,1.0f) );
        virtual ~LitTextureMaterial();
        virtual void render(World* pWorld, GameObject* pGameObject, Camera* pCamera);

        //in rgb values
        void setDiffuseTexture (Texture* pDiffuseTexture);

    private:
        //all the static properties are shared between instances of LitTextureMaterial
        //note that they are all PRIVATE, we do not expose this static info to the outside world
        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

        //in this example we cache all identifiers for uniforms & attributes
        static GLint _uMVPMatrix;
        static GLint _uMMatrix;
        static GLint _uDiffuseTexture;
        static GLint _uCameraPosition;
        static GLint _uShininess;
        static GLint _uSpecularColor;

        static GLint _aVertex ;
        static GLint _aNormal;
        static GLint _aUV ;

        //this one is unique per instance of color material
        Texture* _diffuseTexture;
        float _shininess;
        glm::vec3 _specularColor;
};

#endif // LITTEXTUREMATERIAL_H
