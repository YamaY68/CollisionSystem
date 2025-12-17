#include<DxLib.h>
#include "GameScene.h"

#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include".../../../Object/Actor/Camera/Camera.h"


#include"../Object/Actor/ActorBase.h"
GameScene::GameScene(void):
	SceneBase()
{
}

GameScene::~GameScene(void)
{
}

template<typename T>
std::vector<std::shared_ptr<T>> ObjSearch(const std::vector<std::shared_ptr<ActorBase>>& objects)
{
	static_assert(std::is_base_of_v<ActorBase, T>);

	std::vector<std::shared_ptr<T>> out;
	out.reserve(objects.size());

	for (const auto& obj : objects)
	{
		if (!obj) continue;

		if (auto casted = std::dynamic_pointer_cast<T>(obj))
		{
			out.push_back(casted); 
		}
	}
	return out;
}
void GameScene::Load(void)  
{  


}
void GameScene::Init(void)
{
}

void GameScene::Update(void)
{

	for (auto& actor : actors_)
	{
		actor->Update();
	}
}

void GameScene::Draw(void)
{
	DrawString(0, 0, "game", 0xffffff);
	for (auto& actor : actors_)
	{
		actor->Draw();
	}
}

void GameScene::Release(void)
{
	for (auto& actor : actors_)
	{
		actor->Release();
	}
	actors_.clear();
}
