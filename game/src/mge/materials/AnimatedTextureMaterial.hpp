#ifndef ANIMATEDTEXTUREMATERIAL_H
#define ANIMATEDTEXTUREMATERIAL_H

#include "mge/core/Texture.hpp"
#include "mge/materials/AbstractMaterial.hpp"

/**
 * Simple single texture material, this is a sample which doesn't cache anything upfront and
 * passes in separate matrices for the MVP calculation
 */
class AnimatedTextureMaterial : public AbstractMaterial
{
    public:
		AnimatedTextureMaterial (std::vector<Texture*> pTextures);
        virtual ~AnimatedTextureMaterial ();
        virtual void render(World* pWorld, GameObject* pGameObject, Camera* pCamera);

    protected:
    private:
        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

        std::vector<Texture*> _diffuseTextures;
		Texture* _currentTexture;
		int _textureIterator = 0;
		float _timer;
};

#endif // TEXTUREMATERIAL_H
