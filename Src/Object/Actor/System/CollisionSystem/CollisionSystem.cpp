#include "CollisionSystem.h"

CollisionSystem::CollisionSystem(void)
{
	// 登録用コライダーリストの最大数設定
	dynamicColliders_.reserve(MAX_DYNAMIC_COLLIDER);
	staticColliders_.reserve(MAX_STATIC_COLLIDER);
	//アクティブ用コライダーリストの最大数設定
	activeDynamicColliders_.reserve(MAX_DYNAMIC_COLLIDER);
	activeStaticColliders_.reserve(MAX_STATIC_COLLIDER);
}

CollisionSystem::~CollisionSystem(void)
{
	// 登録コライダー削除
	Clear();
}

void CollisionSystem::AddDynamicCollider(const std::shared_ptr<ColliderBase>& collider)
{
	// 動的コライダー登録
	dynamicColliders_.push_back(collider);
}

void CollisionSystem::AddStaticCollider(const std::shared_ptr<ColliderBase>& collider)
{
	// 静的コライダー登録
	staticColliders_.push_back(collider);
}

void CollisionSystem::Clear(void)
{
	// 登録コライダー削除
	dynamicColliders_.clear();
	staticColliders_.clear();
	activeDynamicColliders_.clear();
	activeStaticColliders_.clear();
}

void CollisionSystem::Check()
{
	// アクティブなコライダーを取得
    GetActiveColliders();
	// 衝突判定処理
    for (int loop = 0; loop < MAX_COLLISION_COUNT; ++loop)
    {
        for (size_t i = 0; i < activeDynamicColliders_.size(); ++i)
        {
            //コライダーを取得
            auto&  ColliderA= activeDynamicColliders_[i];
			// 動的 × 動的
            for (size_t j = i + 1; j < activeDynamicColliders_.size(); ++j)
            {
                CollisionCheck(ColliderA, activeDynamicColliders_[j]);
            }
            // 動的 × 静的
            for (auto& ColliderS : activeStaticColliders_)
            {
                CollisionCheck(ColliderA, ColliderS);
            }
        }
    }
}

void CollisionSystem::CollisionCheck(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
	// 衝突ペアタイプ取得
	CollisionPairType pairType = CollisionLogic::GetCollisionPairType(
		a->GetColliderInfo().shape,
		b->GetColliderInfo().shape);
	
	// 衝突処理実行
	CollisionLogic::CollisionResult result = CollisionLogic::DispatchCollision(
		pairType,
		a,
		b);
	// 衝突していたら
    if (!result.isHit)return;
	// めり込み許容値以下なら処理しない
	if (result.penetration < PENETRATION_ALLOWANCE)return;
	// 衝突解決
	if (result.affectA)
	{
		a->GetFollow()->pos = VSub(a->GetFollow()->pos, VScale(result.normal, result.penetration));
	}
	if (result.affectB)
	{
		b->GetFollow()->pos = VAdd(b->GetFollow()->pos, VScale(VScale(result.normal,-1), result.penetration));
	}
}
void CollisionSystem::GetActiveColliders()
{
	// アクティブなコライダーリストをクリア
    activeDynamicColliders_.clear();
    activeStaticColliders_.clear();

	// アクティブな動的コライダーを取得
    for (auto& w : dynamicColliders_)
    {
        if (auto c = w.lock())
        {
            if (c->GetColliderInfo().isActive)
                activeDynamicColliders_.emplace_back(c);
        }
    }

	// アクティブな静的コライダーを取得
    for (auto& w : staticColliders_)
    {
        if (auto c = w.lock())
        {
            if (c->GetColliderInfo().isActive)
                activeStaticColliders_.emplace_back(c);
        }
    }
}
