#include "Level.hpp"
#include <string>
#include "tinyxml\tinyxml.h"
#include "mesh.hpp"
#include "mge/config.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/World.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/PhongMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/PlayerBehaviour.hpp"
#include "mge/behaviours/SoundTrigger.hpp"
#include "mge/core/Camera.hpp"
#include <map>
#include "..\..\include\tokamak.h"
#include "mge/util/Utility.hpp"
#include "mge/core/Timer.hpp"
#include "mge/behaviours/ArmBehaviour.hpp"


std::vector<Mesh*> Level::_loadedMeshes;
std::vector<std::string> Level::_loadedMeshNames;
std::vector<AbstractMaterial*> Level::_loadedMaterials;
std::vector<GameObject*> Level::_loadedGameObjects;

World* Level::CurrentWorld;
GameObject* Level::CurrentPlayer;

Level::Level()
{
    //ctor
}

Level::~Level()
{
	Unload();
}

void Level::DeleteGameObject(GameObject* pGo)
{
	_loadedGameObjects.erase(std::remove(_loadedGameObjects.begin(), _loadedGameObjects.end(), pGo), _loadedGameObjects.end());
}

void Level::Unload()
{
	if (CurrentPlayer != NULL)
	{
		delete CurrentPlayer;
	}

	for each (Mesh* var in _loadedMeshes)
	{
		delete var;
	}

	_loadedMeshes.clear();


	_loadedMeshNames.clear();


	for each (AbstractMaterial* var in _loadedMaterials)
	{
		delete var;
	}

	_loadedMaterials.clear();

	for each (GameObject* var in _loadedGameObjects)
	{
		if(var->UniqueParentId == "")
			delete var;
	}

	_loadedGameObjects.clear();
}

std::vector<GameObject*>& Level::GetGameObjects()
{
	return _loadedGameObjects;
}

bool Level::Load(std::string pLevelName, World* pWorld)
{
	Timer::Pause();
	Unload();

	CurrentWorld = pWorld;

	//Mesh* monkeyMesh = Mesh::load(config::MGE_MODEL_PATH + "suzanna_smooth.obj");
	//PhongMaterial* phongMaterial = new PhongMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"));
	
	//Init Camera
	Camera* camera = new Camera("camera", glm::vec3(0, 0, 0));
	pWorld->setMainCamera(camera);
	pWorld->add(camera);

	//Init Player
	CurrentPlayer = new GameObject("player", glm::vec3(0, 0, 0), GameObject::RIGIDBODY, GameObject::CAPSULE);
	CurrentPlayer->setParent(pWorld);
	CurrentPlayer->setBehaviour(new PlayerBehaviour(camera));
	((PlayerBehaviour *)CurrentPlayer->getBehaviour())->Initialize();

	GameObject* LeftArm = new GameObject("LeftArm", glm::vec3(-0.8, 0.75, 0));
	LeftArm->setParent(CurrentPlayer);
	LeftArm->setBehaviour(new ArmBehaviour());
	LeftArm->setMesh(Mesh::load(config::MGE_MODEL_PATH+"LeftArm.obj"));
	LeftArm->setMaterial(new PhongMaterial(Texture::load(config::MGE_TEXTURE_PATH + "HandUV.png"), glm::vec3(0,0,0), glm::vec3(1,1,1), glm::vec3(0,0,0), 10.0f));

	/*
	GameObject* RightArm = new GameObject("RightArm", glm::vec3(1.2, -0.2, -0.5));
	RightArm->setParent(CurrentPlayer);
	RightArm->rotate(160.0f, glm::vec3(0, 1, 0));
	RightArm->setMesh(Mesh::load(config::MGE_MODEL_PATH + "RightArm.obj"));
	RightArm->setMaterial(new PhongMaterial(Texture::load(config::MGE_TEXTURE_PATH + "HandUV.png"), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 10.0f));
	*/

	string matName = "";
	string behName = "";

	float triggerRadius;

	glm::vec3 collSize, collCenter, worldPos;
	neQ worldRot;

	bool foundCollider = false;

    std::cout << "Level Loader: Loading level \""+pLevelName+"\"..." << std::endl;
    //_loadedMaterials.push_back(new PhongMaterial (Texture::load(config::MGE_TEXTURE_PATH+"bricks.jpg"), glm::vec3(0.2f,0.2f,0.2f), glm::vec3(0.8f,0.8f,0.8f), glm::vec3(0.1f,0.1f,0.1f), 2.0f));

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

		for (TiXmlElement* obj = elem->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
		{
			if (0 == elemName.compare("gameobjects"))
			{
				foundCollider = false;
				GameObject* go = new GameObject("", glm::vec3(0,0,0), GameObject::ANIMATEDBODY, GameObject::CUBE);
				_loadedGameObjects.push_back(go);
				pWorld->add(go);

				for (TiXmlElement* part = obj->FirstChildElement(); part != NULL; part = part->NextSiblingElement())
				{
					if (part && part->GetText())
					{
						if (0 == strcmp(part->Value(), "name"))
						{
							go->setName(part->GetText());

							if (0 == strcmp(part->GetText(), "Enemy"))
							{
								((PlayerBehaviour*)(CurrentPlayer->getBehaviour()))->AddEnemy(go);
							}
						}
						else if (0 == strcmp(part->Value(), "id"))
						{
							go->UniqueId = part->GetText();
						}
						else if (0 == strcmp(part->Value(), "parentid"))
						{
							go->UniqueParentId = part->GetText();
						}
						else if (0 == strcmp(part->Value(), "transform"))
						{
							std::string pos = part->GetText();
							std::vector<std::string> mat = Utility::Split(pos, ',');

							glm::vec4 m1 = glm::vec4(atof(mat[0].c_str()), atof(mat[1].c_str()), atof(mat[2].c_str()), atof(mat[3].c_str()));
							glm::vec4 m2 = glm::vec4(atof(mat[4].c_str()), atof(mat[5].c_str()), atof(mat[6].c_str()), atof(mat[7].c_str()));
							glm::vec4 m3 = glm::vec4(atof(mat[8].c_str()), atof(mat[9].c_str()), atof(mat[10].c_str()), atof(mat[11].c_str()));
							glm::vec4 m4 = glm::vec4(atof(mat[12].c_str()), atof(mat[13].c_str()), atof(mat[14].c_str()), atof(mat[15].c_str()));

							glm::mat4 m = glm::mat4(m1, m2, m3, m4);
							m = glm::transpose(m);
							go->setTransform(m);

							if (0 == strcmp(go->getName().c_str(), "PlayerSpawn"))
							{
								glm::vec3 asd = go->getLocalPosition();
								CurrentPlayer->getRigidBody()->SetPos(Utility::glmToNe(go->getLocalPosition()));
								//Curre->getRigidBody()->SetPos(glmToNe(_resetPos)); (go->getLocalPosition());
							}
						}
						else if (0 == strcmp(part->Value(), "mesh"))
						{
							int counter = 0;
							int indexPlace = 0;
							bool found = false;

							for (std::vector<std::string>::iterator it = _loadedMeshNames.begin(); it != _loadedMeshNames.end(); ++it)
							{
								if (0 == (*it).compare(part->GetText()))
								{
									//std::cout << "Level Loader: Mesh loaded from Buffer: " + *it + ".obj" << std::endl;
									found = true;
									indexPlace = counter;
								}
								counter++;
							}

							if (found)
							{
								go->setMesh(_loadedMeshes[indexPlace]);
							}
							else
							{
								//std::cout << "Level Loader: Loading Mesh " << part->GetText() << ".obj" << std::endl << std::endl;
								_loadedMeshNames.push_back(part->GetText());
								_loadedMeshes.push_back(Mesh::load(config::MGE_MODEL_PATH + part->GetText() + ".obj"));
								go->setMesh(_loadedMeshes.back());
								//std::cout << std::endl;
							}
						}
						else if (0 == strcmp(part->Value(), "behaviourname"))
						{
							behName = part->GetText();
						}
						else if (0 == strcmp(part->Value(), "behaviourparams"))
						{
							if (0 == behName.compare("RotatingBehaviour"))
							{
								go->setBehaviour(new RotatingBehaviour(part->GetText()));
							}
							else if (0 == behName.compare("SoundTrigger"))
							{
								go->setBehaviour(new SoundTrigger(part->GetText()));
							}
							else if (0 != behName.compare(""))
							{
								std::cout << "Level Loader: Behaviour \"" << behName << "\" not found!" << std::endl;
							}
						}
						else if (0 == strcmp(part->Value(), "materialname"))
						{
							matName = part->GetText();
						}
						else if (0 == strcmp(part->Value(), "materialparams"))
						{
							bool foundMat = false;
							//TODO Add all the materials
							if (0 == matName.compare("ColorMaterial"))
							{
								_loadedMaterials.push_back(new ColorMaterial(part->GetText()));
								foundMat = true;
							}
							if (0 == matName.compare("TextureMaterial"))
							{
								_loadedMaterials.push_back(new PhongMaterial(part->GetText()));
								foundMat = true;
							}
							else if (0 != matName.compare(""))
							{
								std::cout << "Level Loader: Material \"" << matName << "\" not found!" << std::endl;
								foundMat = false;
							}

							if (foundMat)
							{
								go->setMaterial(_loadedMaterials.back());
							}

						}
						else if (0 == strcmp(part->Value(), "collidersize"))
						{
							if (0 != strcmp(part->GetText(), ""))
							{
								foundCollider = true;
								std::string pos = part->GetText();
								std::vector<std::string> mat = Utility::Split(pos, ',');
								collSize = glm::vec3(atof(mat[0].c_str()), atof(mat[1].c_str()), atof(mat[2].c_str()));
							}
						}
						else if (0 == strcmp(part->Value(), "collidercenter"))
						{
							if (0 != strcmp(part->GetText(), ""))
							{
								foundCollider = true;
								std::string pos = part->GetText();
								std::vector<std::string> mat = Utility::Split(pos, ',');
								collCenter = glm::vec3(atof(mat[0].c_str()), atof(mat[1].c_str()), atof(mat[2].c_str()));
							}
						}
						else if (0 == strcmp(part->Value(), "mass"))
						{
							//TODO Add the mass to the level loading
						}
						else if (0 == strcmp(part->Value(), "iskinematic"))
						{
							//TODO Add distinction of Rigid and ANimated bodys when loading
						}
						else if (0 == strcmp(part->Value(), "worldposition"))
						{
							std::string pos = part->GetText();
							std::vector<std::string> mat = Utility::Split(pos, ',');
							worldPos = glm::vec3(atof(mat[0].c_str()), atof(mat[1].c_str()), atof(mat[2].c_str()));
						}
						else if (0 == strcmp(part->Value(), "worldrotation"))
						{
							std::string pos = part->GetText();
							std::vector<std::string> mat = Utility::Split(pos, ',');
							worldRot.Set(atof(mat[0].c_str()), atof(mat[1].c_str()), atof(mat[2].c_str()), atof(mat[3].c_str()));
						}
					}
				}

				if (foundCollider)
				{
					//TODO Export physics material propeertys
					neAnimatedBody* body = pWorld->getPhysics()->CreateAnimatedBody();
					neGeometry* geometry = body->AddGeometry();
					geometry->SetBoxSize(collSize.x * go->getScale().x, collSize.y * go->getScale().y, collSize.z * go->getScale().z);
					body->UpdateBoundingInfo();
					body->SetUserData((u32)go);
					
					neV3 nPos;
					nPos.Set(worldPos.x + collCenter.x, worldPos.y + collCenter.y, worldPos.z + collCenter.z);
					body->SetPos(nPos);
					body->SetRotation(worldRot);

					go->setAnimatedBody(body);

					//This code shows colliders
					/*
					GameObject* test = new GameObject("");
					pWorld->add(test);
					test->setTransform(go->getTransform());
					test->setLocalPosition(glm::vec3(worldPos.x + collCenter.x, worldPos.y + collCenter.y, worldPos.z + collCenter.z));
					test->setMesh(Mesh::load(config::MGE_MODEL_PATH + "cube.obj"));
					test->setMaterial(new ColorMaterial(glm::vec3(1,0,1)));
					test->scale(1.0f / test->getScale());
					test->scale(collSize * 1.1f);
					*/
				}
			}
			else if (0 == elemName.compare("lights"))
			{
				for (TiXmlElement* part = obj->FirstChildElement(); part != NULL; part = part->NextSiblingElement())
				{
					//TODO Implement Light import
				}
			}
		}
	}

	std::cout << "Level Loader: Setting up Hierarchy..." << std::endl;
	bool foundParent = false;
	for (std::vector<GameObject*>::iterator baseIt = _loadedGameObjects.begin(); baseIt != _loadedGameObjects.end(); ++baseIt)
	{
		foundParent = false;
		for (std::vector<GameObject*>::iterator lookIt = _loadedGameObjects.begin(); lookIt != _loadedGameObjects.end(); ++lookIt)
		{
			if (0 == ((*baseIt)->UniqueParentId).compare((*lookIt)->UniqueId))
			{
				(*lookIt)->add(*baseIt);
				//std::cout << "Level Loader: Adding \"" << (*baseIt)->getName() << "\" as child to \"" << (*lookIt)->getName() << "\"" << std::endl;
				foundParent = true;
			}
		}

		if (!foundParent)
		{
			pWorld->add(*baseIt);
			//std::cout << "Level Loader: Adding \"" << (*baseIt)->getName() << "\" to world" << std::endl;
		}
	}
	
	doc.Clear();

	Timer::UnPause();
	std::cout << "Level Loader: \"" + pLevelName + "\" loaded successfully!" << std::endl;
	return true;
}