#include<DxLib.h>
#include "GameScene.h"

#include"../Manager/SceneManager.h"
#include"../Manager/ResourceManager.h"
#include".../../../Object/Actor/Camera/Camera.h"

#include"../Manager/System/InputSystem/InputSystem.h"
#include"../Manager/System/MoveSystem/MoveSystem.h"

#include"../Object/Actor/ActorBase.h"

#include"../Object/Actor/Component/MoveComponent/MoveComponent.h"
#include"../Object/Actor/Component/PlayerInputComponent/PlayerInputComponent.h"

#include"../Object/Actor/Shape/ShapeBase.h"
#include"../Object/Actor/Shape/Sphere.h"
#include"../Object/Actor/Shape/Box.h"

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
	// オブジェクト生成  
	std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>();  
	sphere->GetTransform().pos = VGet(-300.0f, 0.0f, 0.0f);  
	sphere->AddComponent(std::make_shared<MoveComponent>(5));
	sphere->AddComponent(std::make_shared<PlayerInputComponent>(
		KEY_INPUT_W, KEY_INPUT_S,
		KEY_INPUT_A, KEY_INPUT_D,
		KEY_INPUT_Q, KEY_INPUT_E
	));
	actors_.push_back(sphere);  

	for (int i = 0; i < 3; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			for (int j = 0; j <3; j++)
			{


			sphere = std::make_shared<Sphere>(10, 0xff00aa);
			actors_.push_back(sphere);
			sphere->GetTransform().pos = VGet(
				i * 10, k * 10, j * 10
			);
			}
		}
	}


	std::shared_ptr<Box>box = std::make_shared<Box>();
	box->GetTransform().pos = VGet(300.0f, 0.0f, 0.0f);
	actors_.push_back(box);
	box->AddComponent(std::make_shared<MoveComponent>(5));
	box->AddComponent(std::make_shared<PlayerInputComponent>(
		KEY_INPUT_I, KEY_INPUT_K,
		KEY_INPUT_J, KEY_INPUT_L,
		KEY_INPUT_U, KEY_INPUT_O
	));

	// カメラ生成  
	auto camera = std::make_shared<Camera>();  
	actors_.push_back(camera);  
}
void GameScene::Init(void)
{
	for (auto& actor : actors_)
	{
		actor->Init();
		collisionSystem_.AddCollider(actor->GetOwnColliders());
	}
}

void GameScene::Update(void)
{

	for (auto& actor : actors_)
	{
		actor->Update();
	}

	inputSystem_.Update(actors_);
	moveSystem_.Update(actors_);


	collisionSystem_.Check();
}

void GameScene::Draw(void)
{
	DrawString(0, 0, "game", 0xffffff);
	for (auto& actor : actors_)
	{
		actor->Draw();
		for (const auto& [shape, collider] : actor->GetOwnColliders())
		{
			collider->Draw();
		}
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
