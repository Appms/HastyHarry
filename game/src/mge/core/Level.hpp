#ifndef LEVEL_HPP
#define LEVEL_HPP
#include <string>
#include <vector>

class Mesh;
class World;
class AbstractMaterial;
class AbstractBehaviour;
class GameObject;

class Level
{
    public:
        Level();
        virtual ~Level();
        static bool Load(std::string pLevelName, World* pWorld);
		static void Unload();
        //TODO move this to a util class
        static std::vector<std::string> split( std::string str, char delimiter );
    protected:
    private:
		static std::vector<Mesh*> _loadedMeshes;
		static std::vector<std::string> _loadedMeshNames;
		static std::vector<AbstractMaterial*> _loadedMaterials;
		static std::vector<AbstractBehaviour*> _loadedBehaviours;
		static std::vector<GameObject*> _loadedGameObjects;
};

#endif // LEVEL_HPP
