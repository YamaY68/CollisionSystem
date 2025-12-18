#include "MoveSystem.h"
#include"../../../Object/Actor/ActorBase.h"
#include"../../../Object/Actor/Component/MoveComponent/MoveComponent.h"


void MoveSystem::Update(const std::vector<std::shared_ptr<ActorBase>>& objects)
{
    for (auto obj : objects)
    {
		// MoveComponentを持っていなければスキップ
        if (!obj->HasComponent<MoveComponent>())continue;
		auto& move = obj->GetComponent<MoveComponent>();

        VECTOR dir = move.GetDirection();

        float len = VSize(dir);
		// 正規化
        if (len != 0.0f)
        {
            dir = VScale(dir, 1.0f / len);
            move.SetDirection(dir);
		}
        //ブレイクポイント用
		//移動ベクトルの長さが0なら移動しない
		if (len == 0.0f)continue;

		// 移動処理
        //デバッグしやすいようにスピードを取得
		float speed = move.GetSpeed();

        obj->GetTransform().pos = VAdd(obj->GetTransform().pos,
            VScale(dir, speed));
    }
}
