#include "InputSystem.h"
#include"../../../Object/Actor/Component/PlayerInputComponent/PlayerInputComponent.h"
#include"../../../Object/Actor/Component/MoveComponent/MoveComponent.h"
#include"../../../Object/Actor/ActorBase.h"

void InputSystem::Update(const std::vector<std::shared_ptr<ActorBase>>& objects)
{
	
	for (auto obj : objects)
	{
		// PlayerInputComponentを持っていないオブジェクトはスキップ
		if (!obj->HasComponent<PlayerInputComponent>()) continue;
		auto& input = obj->GetComponent<PlayerInputComponent>();
		auto& move = obj->GetComponent<MoveComponent>();
		
		VECTOR moveDir = VGet(0, 0, 0);

		// 入力に応じて移動方向を設定
		if (CheckHitKey(input.keyFront)) moveDir.z += 1.0f;
		if (CheckHitKey(input.keyBack)) moveDir.z -= 1.0f;
		if (CheckHitKey(input.keyRight)) moveDir.x += 1.0f;
		if (CheckHitKey(input.keyLeft)) moveDir.x -= 1.0f;
		if (CheckHitKey(input.keyUp)) moveDir.y += 1.0f;
		if (CheckHitKey(input.keyDown)) moveDir.y -= 1.0f;

		move.SetDirection(moveDir);
	}
}
