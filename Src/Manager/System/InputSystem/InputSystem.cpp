#include "InputSystem.h"
#include"../../../Object/Actor/Component/PlayerInputComponent/PlayerInputComponent.h"
#include"../../../Object/Actor/Component/MoveComponent/MoveComponent.h"
#include"../../../Object/Actor/ActorBase.h"
#include"../../../Manager/SceneManager.h"
#include"../../../Object/Actor/Camera/Camera.h"
void InputSystem::Update(const std::vector<std::shared_ptr<ActorBase>>& objects)
{
	
	for (auto obj : objects)
	{
		// PlayerInputComponentを持っている場合カメラ基準で移動方向を設定
		if (obj->HasComponent<PlayerInputComponent>()) {

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

		const VECTOR cameraAngle = SceneManager::GetInstance().GetCamera()->GetAngles();
		MATRIX camYaw = MGetRotY(cameraAngle.y);
		//ローカル入力をカメラに合わせてワールドに変換
		moveDir = VTransform(moveDir, camYaw);

		move.SetDirection(moveDir);
		}
	}
}
