#include "CollisionSystem.h"
#include"../../../../Object/Actor/ActorBase.h"
#include"../../../../Object/Actor/Component/RigidBodyComponent/RigidBody.h"

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

void CollisionSystem::AddCollider(const std::map<int, std::shared_ptr<ColliderBase>>& colliders)  
{  
   // コライダー登録  
   for (auto& c : colliders)  
   {  
       if (c.second->GetColliderInfo().isDynamic)  
       {  
           // 動的コライダー登録  
           AddDynamicCollider(c.second);  
		   auto actor = c.second->GetFollowActor();
		   BindDynamicEntityId(dynamicColliders_.size()-1, actor->GetEntityId());
       }  
       else  
       {  
           // 静的コライダー登録  
           AddStaticCollider(c.second);  
		   auto actor = c.second->GetFollowActor();
		   BindStaticEntityId(staticColliders_.size() - 1, actor->GetEntityId());
       }  
   }  
}



void CollisionSystem::BindDynamicEntityId(std::size_t dynamicIndex, std::uint32_t entityId)
{
	// 動的エンティティID紐付け
	// インデックスが範囲外なら拡張
	if (dynamicIndex >= dynamicIndex2EntityId_.size())
	{
		dynamicIndex2EntityId_.resize(dynamicIndex + 1, 0);
	}
	// エンティティID設定
	dynamicIndex2EntityId_[dynamicIndex] = entityId;
}

void CollisionSystem::BindStaticEntityId(std::size_t staticIndex, std::uint32_t entityId)
{
	// 静的エンティティID紐付け
	// インデックスが範囲外なら拡張
	if (staticIndex >= staticIndex2EntityId_.size())
	{
		staticIndex2EntityId_.resize(staticIndex + 1, 0);
	}
	// エンティティID設定
	staticIndex2EntityId_[staticIndex] = entityId;
}

void CollisionSystem::DiffPairs(pair& currentPairs, pair& prevPairs, pair& beginPairs, pair& endPairs)
{
	//正規化関数
	auto norm = [](auto& v)
		{
			//ソート
			std::sort(v.begin(), v.end());
			//削除
			v.erase(
				//重複している後ろを前に詰める
				std::unique(v.begin(), v.end()),
				v.end());
		};

	//正規化
	norm(currentPairs);
	norm(prevPairs);
	//出力をクリア
	beginPairs.clear();
	endPairs.clear();
	//差分抽出
	std::size_t a = 0, b = 0;
	
	while (a < currentPairs.size() && b < prevPairs.size())
	{
		//今フレームのペアが前フレームのペアより大きい場合
		if (currentPairs[a] < prevPairs[b]) 
			//新規ペア
		{ beginPairs.push_back(currentPairs[a]); ++a; }
		//前フレームのペアが前フレームのペアより大きい場合
		else if (prevPairs[b] < currentPairs[a])
			//消失ペア
		{ endPairs.push_back(prevPairs[b]); ++b; }
		//同じ場合
		//変化なし
		else
		{
			++a; ++b;
		}
	}
	//片方だけ残ってる分を処理
	//今フレーム分
	while (a < currentPairs.size())beginPairs.push_back(currentPairs[a++]);
	//前フレーム分
	while (b < prevPairs.size())endPairs.push_back(prevPairs[b++]);
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
    for (int loop = 0; loop < 1; ++loop)
    {
        for (size_t i = 0; i < activeDynamicColliders_.size(); i++)
        {
            //コライダーを取得
            auto&  ColliderA= activeDynamicColliders_[i];
			// 動的 × 動的
            for (size_t j =0; j < activeDynamicColliders_.size(); j++)
            {
				if (ColliderA == activeDynamicColliders_[j])continue;
				ColliderA->GetColliderInfo().layer;
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
	a->GetFollowActor()->GetComponent<RigidBody>().Setgrounded(false);
	b->GetFollowActor()->GetComponent<RigidBody>().Setgrounded(false);

	//今フレームで衝突しているペアを集める
	std::vector < std::pair < std::size_t, std::size_t>>currentPairs;

	// 衝突ペアタイプ取得
	CollisionPairType pairType = CollisionLogic::GetCollisionPairType(
		a->GetColliderInfo().shape,
		b->GetColliderInfo().shape);
	
	// 衝突処理実行
	CollisionResult result = CollisionLogic::DispatchCollision(
		pairType,
		a,
		b);
	// 衝突していない場合
    if (!result.isHit)return;
	// めり込み許容値以下なら処理しない
	if (result.penetration < PENETRATION_ALLOWANCE)return;

	//押し出し処理
	if (result.pushA > 0.0f)
	{
		//デバッグしやすいように変数に格納
		VECTOR& posA = a->GetFollow()->prevPos;
		//VECTOR normal = VScale(result.normal, -1.0f);
		VECTOR push = VScale(result.normal, result.pushA);
		posA = VAdd(posA, push);
		//押し出しが重力と逆方向なら重力を０にする
		float dot = VDot(result.normal, VGet(0, 1, 0));
		
		if (dot > 0.5f)
		{
			a->GetFollowActor()->GetComponent<RigidBody>().Setgrounded(true);
		}	
	}
	if (result.pushB > 0.0f)
	{
		//デバッグしやすいように変数に格納
		VECTOR& posB = b->GetFollow()->prevPos;
		VECTOR push = VScale(
			VScale(result.normal,-1), result.pushB);
		posB = VAdd(posB,push);

		//押し出しが重力と逆方向なら重力を０にする
		float dot = VDot(result.normal, VGet(0, 1, 0));
		if (dot < -0.5f)
		{
			b->GetFollowActor()->GetComponent < RigidBody>().Setgrounded(true);
		}
	}
	//衝突ペア登録
	currentPairs.emplace_back(
		a->GetFollowActor()->GetEntityId(),
		b->GetFollowActor()->GetEntityId()
	);
	//差分摘出用新規、消失ペアリスト
	std::vector< std::pair<std::size_t, std::size_t>>begins, ends;
	// 差分抽出
	DiffPairs(currentPairs, prevPairs_, begins, ends);
	//コールバック関数が設定されていれば呼び出し
	if (onBegin_) {
		// 新規ペアコールバック
		for (auto [i, j] : begins) {
			auto idA = (i < dynamicIndex2EntityId_.size()
				? dynamicIndex2EntityId_[i]: 0);
			auto idB = (j < dynamicIndex2EntityId_.size()
				? dynamicIndex2EntityId_[j] : 0);
			if (idA && idB)
				onBegin_(idA, idB);
		}
	}
	if (onEnd_) {
		// 消失ペアコールバック
		for (auto [i, j] : ends) {
			auto idA = (i < dynamicIndex2EntityId_.size()
				? dynamicIndex2EntityId_[i] : 0);
			auto idB = (j < dynamicIndex2EntityId_.size()
				? dynamicIndex2EntityId_[j] : 0);
			if (idA && idB)
				onEnd_(idA, idB);
		}
	}
	prevPairs_.swap(currentPairs);
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
