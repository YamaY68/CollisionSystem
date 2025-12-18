#pragma once
#include<DxLib.h>
#include<vector>
#include<cstdint>
#include<memory>
#include<map>
#include"../../../Object/Actor/Collider/ColliderBase.h"
#include"CollisionLogic.h"

class CollisionSystem
{
public:
	//コンストラクタ
	CollisionSystem(void);
	//デストラクタ
	~CollisionSystem(void);

	//コライダの登録
	void AddCollider(const std::map<int, std::shared_ptr<ColliderBase>>& collider);



	//登録コライダー削除
	void Clear(void);

	//衝突判定処理
	void Check(void);

private:
	//動的コライダ登録
	void AddDynamicCollider(const std::shared_ptr<ColliderBase>& collider);
	//静的コライダ登録
	void AddStaticCollider(const std::shared_ptr<ColliderBase>& collider);

	// 衝突判定処理
	void CollisionCheck(const std::shared_ptr<ColliderBase>& a,
		const std::shared_ptr<ColliderBase>& b);

	//アクティブなコライダーを取得
	void GetActiveColliders(void);

private:
	//衝突回数の上限
	static constexpr unsigned char MAX_COLLISION_COUNT = 3;
	//めり込み許容値
	static constexpr float PENETRATION_ALLOWANCE = 0.05f;
	//動的コライダーの最大数
	static constexpr size_t MAX_DYNAMIC_COLLIDER = 64;
	//静的コライダーの最大数
	static constexpr size_t MAX_STATIC_COLLIDER = 128;


	//動的コライダのリスト
	std::vector<std::weak_ptr<ColliderBase>> dynamicColliders_;
	//静的コライダのリスト
	std::vector<std::weak_ptr<ColliderBase>> staticColliders_;

	//アクティブなコライダーのリスト
	//動的
	std::vector<std::shared_ptr<ColliderBase>> activeDynamicColliders_;
	//静的
	std::vector<std::shared_ptr<ColliderBase>> activeStaticColliders_;
};

