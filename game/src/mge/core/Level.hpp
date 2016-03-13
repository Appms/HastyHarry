#ifndef LEVEL_HPP
#define LEVEL_HPP
#include <string>
#include <vector>

class Mesh;
class World;
class AbstractMaterial;
class AbstractBehaviour;
class GameObject;

/*
* Takes care of reading an XML with information about the level generated in Unity.
* It also takes care of unloading the previous loaded level, allowing easy level transitions.
*/
class Level
{
    public:
        Level();
        virtual ~Level();
        static bool Load(std::string pLevelName, World* pWorld);
		static void Unload();

		static World* CurrentWorld;
		static GameObject* CurrentPlayer;
		static std::vector<GameObject*>& GetGameObjects();
		static void DeleteGameObject(GameObject* pGo);

    protected:
    private:
		static std::vector<Mesh*> _loadedMeshes;
		static std::vector<std::string> _loadedMeshNames;
		static std::vector<AbstractMaterial*> _loadedMaterials;
		static std::vector<std::string> _loadedMaterialsNames;
		static std::vector<GameObject*> _loadedGameObjects;
};

#endif // LEVEL_HPP
