#include "Level.hpp"
#include <string>
#include "tinyxml\tinyxml.h"
#include "mesh.hpp"
#include "mge/config.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/World.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/PhongMaterial.hpp"
#include <map>


std::vector<Mesh*> Level::_loadedMeshes;
std::vector<std::string> Level::_loadedMeshNames;
std::vector<AbstractMaterial*> Level::_loadedMaterials;

Level::Level()
{
    //ctor
}

Level::~Level()
{
    //dtor
}

bool Level::Load(std::string pLevelName, World* pWorld)
{


    std::cout << "Level Loader: Loading level \""+pLevelName+"\"..." << std::endl;
    _loadedMaterials.push_back(new PhongMaterial (Texture::load(config::MGE_TEXTURE_PATH+"bricks.jpg"), glm::vec3(0.2f,0.2f,0.2f), glm::vec3(0.8f,0.8f,0.8f), glm::vec3(0.1f,0.1f,0.1f), 2.0f));

    TiXmlDocument doc;
	string filePath = config::MGE_LEVEL_PATH + pLevelName;
    if(!doc.LoadFile(filePath.c_str()))
    {
        std::cout << "Level Loader: " << doc.ErrorDesc() << std::endl;
        return false;
    }

    TiXmlElement* root = doc.FirstChildElement();
    if(root == NULL)
    {
        std::cout << "Level Loader: Failed to load file: No root element." << std::endl;
        doc.Clear();
        return false;
    }

    for(TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
    {
        std::string elemName = elem->Value();

        for(TiXmlElement* obj = elem->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
        {
            if(0 == elemName.compare("gameobjects"))
            {
                GameObject* go = new GameObject ("");
                //TODO Load the Material from unity settings
                go->setMaterial(_loadedMaterials[0]);
                pWorld->add(go);

                for(TiXmlElement* part = obj->FirstChildElement(); part != NULL; part = part->NextSiblingElement())
                {
                    if(part && part->GetText())
                    {
                        if(0 == strcmp(part->Value(), "name"))
                        {
                            go->setName(part->GetText());
                        }
                        else if(0 == strcmp(part->Value(), "transform"))
                        {
                            std::string pos = part->GetText();
                            std::vector<std::string> mat = split(pos, ',');

                            glm::vec4 m1 = glm::vec4(atof(mat[0].c_str()), atof(mat[1].c_str()), atof(mat[2].c_str()), atof(mat[3].c_str()));
                            glm::vec4 m2 = glm::vec4(atof(mat[4].c_str()), atof(mat[5].c_str()), atof(mat[6].c_str()), atof(mat[7].c_str()));
                            glm::vec4 m3 = glm::vec4(atof(mat[8].c_str()), atof(mat[9].c_str()), atof(mat[10].c_str()), atof(mat[11].c_str()));
                            glm::vec4 m4 = glm::vec4(atof(mat[12].c_str()), atof(mat[13].c_str()), atof(mat[14].c_str()), atof(mat[15].c_str()));

                            glm::mat4 m = glm::mat4(m1, m2, m3 ,m4);
                            m = glm::transpose(m);
                            go->setTransform(m);
                        }
                        else if(0 == strcmp(part->Value(), "mesh"))
                        {
                            int counter = 0;
                            int indexPlace = 0;
                            bool found = false;
                            for(std::vector<std::string>::iterator it = _loadedMeshNames.begin(); it != _loadedMeshNames.end(); ++it)
                            {
                                if(0 == (*it).compare(part->GetText()))
                                {
                                    std::cout << "Level Loader: Mesh loaded from Buffer: " + *it + ".obj" << std::endl;
                                    found = true;
                                    indexPlace = counter;
                                }
                                counter++;
                            }

                            if(found)
                            {
                                go->setMesh(_loadedMeshes[indexPlace]);
                            }
                            else
                            {
                                std::cout << "Level Loader: Loading Mesh " << part->GetText() << ".obj" << std::endl << std::endl;
                                _loadedMeshNames.push_back(part->GetText());
                                _loadedMeshes.push_back(Mesh::load(config::MGE_MODEL_PATH+part->GetText()+"_flat.obj"));
                                go->setMesh(_loadedMeshes.back());
                                std::cout << std::endl;
                            }
                        }
                        else if(0 == strcmp(part->Value(), "behaviour"))
                        {

                        }
                        else if(0 == strcmp(part->Value(), "material"))
                        {

                        }
                        else if(0 == strcmp(part->Value(), "collidersize"))
                        {

                        }
                        else if(0 == strcmp(part->Value(), "collidercenter"))
                        {

                        }
                    }
                }
            }
            else if(0 == elemName.compare("lights"))
            {
                for(TiXmlElement* part = obj->FirstChildElement(); part != NULL; part = part->NextSiblingElement())
                {
                    //TODO Implement Light import
                }
            }
        }
    }

    std::cout << "Level Loader: \"" + pLevelName + "\" loaded successfully!" << std::endl;
	return true;
}

std::vector<std::string> Level::split( std::string str, char delimiter )
{
    std::vector<std::string> ret;

    size_t pos = str.find_first_of( delimiter );

    while ( !str.empty() )
    {
        std::string cur = str.substr( 0, pos );
        if ( !cur.empty() )
            ret.push_back( cur );

        if ( pos == std::string::npos )
            break;

        str = str.substr( pos + 1 );

        pos = str.find_first_of( delimiter );
    }

    return ret;
}