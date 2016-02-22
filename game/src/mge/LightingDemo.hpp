#ifndef LIGHTINGDEMO_H
#define LIGHTINGDEMO_H

#include <mge/core/AbstractGame.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

class Swapper;
class GameObject;
class DebugHud;

class LightingDemo: public AbstractGame
{
    //PUBLIC FUNCTIONS

	public:
		LightingDemo();
		virtual ~LightingDemo();

        virtual void initialize();

	protected:
        virtual void _initializeScene();

	    //override render to render the hud as well.
	    virtual void _render();

	private:
		DebugHud* _hud;                   //hud display

        void _updateHud();
};

#endif // LIGHTINGDEMO_H
