#include "MoveSystem.h"
#include"../../../Object/Actor/ActorBase.h"
#include"../../../Object/Actor/Component/MoveComponent/MoveComponent.h"
#include"../../../Object/Actor/Component/RigidBodyComponent/RigidBody.h"

#include"../../../Manager/SceneManager.h"
#include"../../../Object/Actor/Camera/Camera.h"



void MoveSystem::Update(const std::vector<std::shared_ptr<ActorBase>>& objects)
{
    for (auto obj : objects)
    {
		if (obj->HasComponent<MoveComponent>()) {

			auto& move = obj->GetComponent<MoveComponent>();

			//ˆÚ“®•ûŒü
			VECTOR dir = move.GetDirection();
			//³‹K‰»
			float len = VSize(dir);
			if (len != 0.0f)
			{
				dir = VScale(dir, 1.0f / len);
			}
			// Rigidbody‚ª‚ ‚ê‚Î
			if (obj->HasComponent<RigidBody>()) {
				auto& rb = obj->GetComponent<RigidBody>();
				// —Í‚ð‰Á‚¦‚é
				rb.AddForce(VScale(dir, move.GetSpeed()));
			}

		}
    }
}
