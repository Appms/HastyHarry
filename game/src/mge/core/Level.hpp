#ifndef LEVEL_HPP
#define LEVEL_HPP
#include <string>
#include <vector>

class Mesh;
class World;
class AbstractMaterial;

class Level
{
    public:
        Level();
        virtual ~Level();
        static bool Load(std::string pLevelName, World* pWorld);
        //TODO move this to a util class
        static std::vector<std::string> split( std::string str, char delimiter );
    protected:
    private:
        static std::vector<Mesh*> _loadedMeshes;
        //TODO I could put this into the mesh class
        static std::vector<std::string> _loadedMeshNames;

        static std::vector<AbstractMaterial*> _loadedMaterials;
};

#endif // LEVEL_HPP
