#include <iostream>

#include "mge/core/AbstractGame.hpp"
#include "mge/MGEDemo.hpp"
#include "mge/LightingDemo.hpp"
#include "mge/config.hpp"
//#include "lua.hpp"

/**
 * Main entry point for the Micro Engine.

 * Design goals:
 * - correct division of OpenGL into appropriate wrapper classes
 * - simple design
 * - each class should know as little as possible about other classes and non related parts of OpenGL
 * - the engine must allow different objects with different transforms, meshes, materials (shaders) etc
 * - consistent coding conventions
 * - reusable and extendable core set of classes which should require no modification
 *   (in other words it is possible to have a working "empty" example)
 *
 * All documentation is contained within the HEADER files, not the CPP files in possible.
 *
 */
int main()
{
    std::cout << "Starting Game" << std::endl;

    /*lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    if(luaL_dofile(L, "mge/scripts/helloworld.lua")){
        const char* err = lua_tostring(L, -1);
        printf(err);
    }

    lua_close(L);*/

    AbstractGame* game = new LightingDemo();
    game->initialize();
    game->run();

	delete game;

    return 0;
}



