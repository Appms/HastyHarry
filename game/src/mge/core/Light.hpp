#ifndef LIGHT_H
#define LIGHT_H

#include <string>
#include <vector>
#include "mge/core/GameObject.hpp"

class Light : public GameObject
{
	public:
		Light(
            std::string pName = "Light",
            glm::vec3 pPosition = glm::vec3( 0.0f, 0.0f, 0.0f ),
            glm::vec3 pDirection = glm::vec3( 0.0f, 0.0f, 0.0f ),
            glm::vec3 pIntensities = glm::vec3 ( 1.0f, 1.0f, 1.0f),
            float pAttenuation = 2.0f,
            float pConeAngle = 60.0f,
            bool pIsDirectional = false
        );

		virtual ~Light();

		static float GetAmbientCoefficient() { return _ambientCoefficient; };
		static float SetAmbientCoefficient(float value) {  _ambientCoefficient = value; };
		static int GetLightCount() { return _lights.size(); };
		static Light* GetLight(int index) { return _lights[index]; };

        glm::vec4 GetPosition() { return glm::vec4(getLocalPosition(), !_isDirectional); }
        glm::vec3 GetDirection() { return _direction; }
        glm::vec3 GetInensities() { return _intensities; }
        float GetAttenuation() { return _attenuation; }
        float GetConeAngle() {  return _coneAngle; }
    private:
        static const int _maxLights;
        static float _ambientCoefficient;
        static std::vector<Light*> _lights;
        static void addLight(Light* pLight);

        bool _isDirectional;
        glm::vec3 _direction;
        glm::vec3 _intensities;
        float _attenuation;
        float _coneAngle;
};

#endif // LIGHT_H
