#include "PhysicsSystem.h"
#include"../../../Object/Actor/ActorBase.h"
#include"../../../Object/Actor/Component/RigidBodyComponent/RigidBody.h"


void PhysicsSystem::Update(const std::vector<std::shared_ptr<ActorBase>>& objects)
{
    const float dt = 1.0f / 60.0f;
    const VECTOR GRAVITY = VGet(0.0f, -98, 0.0f);

    for (auto obj : objects)
    {
        if (!obj->HasComponent<RigidBody>()) continue;

        auto& rb = obj->GetComponent<RigidBody>();

        // Static / Kinematic を弾くならここ
        if (rb.GetInvMass() == 0.0f) continue;

        // ===== 重力を Force として加える =====
        if (rb.IsUseGravity())
        {
            if (!rb.IsGrounded()) {
                // F = m * g
                rb.AddForce(VScale(GRAVITY, rb.GetMass()));
            }
            else rb.SetVelocity({ rb.GetVelocity().x,0,rb.GetVelocity().z });
        }

        // ===== Force → Acceleration =====
        VECTOR acceleration = VScale(rb.GetForce(), rb.GetInvMass());

        // ===== Velocity 更新 =====
        VECTOR newVel = VAdd(rb.GetVelocity(), acceleration); //VScale(acceleration, dt));
        rb.SetVelocity(newVel);


        // ===== 減速（ダンピング） =====
        newVel = VScale(newVel, 0.7);

        rb.SetVelocity(newVel);



		//意図移動考慮した位置更新
		// ===== 位置更新 =====
        obj->GetTransform().prevPos = VAdd(
			obj->GetTransform().pos,
			VScale(rb.GetVelocity(), dt)
		);

        // ===== Force クリア =====
        rb.ClearForce();
    }
}

void PhysicsSystem::Resolve(const std::vector<std::shared_ptr<ActorBase>>& objects)
{
	for (auto obj : objects)
	{
		if (!obj->HasComponent<RigidBody>()) continue;
		auto& rb = obj->GetComponent<RigidBody>();
		// Static / Kinematic を弾くならここ
		if (rb.GetInvMass() == 0.0f) continue;
		// 最終位置決定
		obj->GetTransform().pos = obj->GetTransform().prevPos;
	}
}

