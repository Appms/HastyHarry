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
            glm::vec3 pColor = glm::vec3 ( 1.0f, 1.0f, 1.0f),
            float pIntensity = 1.0f,
            float pRange = 10.0f,
            bool pIsDirectional = false
        );

		virtual ~Light();

		static void ClearLights() { _lights.clear(); }

		static float GetAmbientCoefficient() { return _ambientCoefficient; };
		static float SetAmbientCoefficient(float value) {  _ambientCoefficient = value; };
		static int GetLightCount() { return _lights.size(); };
		static Light* GetLight(int index) { return _lights[index]; };

        glm::vec4 GetPosition() { return glm::vec4(getLocalPosition(), !_isDirectional); }
        glm::vec3 GetDirection() { return _direction; }
        glm::vec3 GetColor() { return _color; }
        float GetIntensity() { return _intensity; }
        float GetRange() {  return _range; }

		void SetDirection(glm::vec3 pDirection) { _direction = pDirection; }
		void IsDirectional(bool dir) { _isDirectional = dir; }
		void SetColor(glm::vec3 pColor) { _color = pColor; }
		void SetIntensity(float pIntensity) { _intensity = pIntensity; }
		void SetRange(float pRange) { _range = pRange; }

    private:
        static const int _maxLights;
        static float _ambientCoefficient;
        static std::vector<Light*> _lights;
        static void addLight(Light* pLight);

        bool _isDirectional;
        glm::vec3 _direction;
		glm::vec3 _color;
		float _intensity;
		float _range;
};

#endif // LIGHT_H
