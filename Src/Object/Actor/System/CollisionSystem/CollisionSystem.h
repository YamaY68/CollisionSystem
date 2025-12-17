#pragma once
#include<DxLib.h>
#include<vector>
#include<cstdint>
#include<memory>
#include"../../Collider/ColliderBase.h"

class CollisionSystem
{
public:
	//コンストラクタ
	CollisionSystem(void);
	//デストラクタ
	~CollisionSystem(void);

	//動的コライダ登録
	void AddDynamicCollider(const std::shared_ptr<ColliderBase>& collider);
	//静的コライダ登録
	void AddStaticCollider(const std::shared_ptr<ColliderBase>& collider);

	//衝突判定処理
	void Check(void);

private:
	// 衝突判定処理
	void CollisionCheck(const std::shared_ptr<ColliderBase>& a,
		const std::shared_ptr<ColliderBase>& b);


private:
	//動的コライダのリスト
	std::vector<std::weak_ptr<ColliderBase>> dynamicColliders_;
	//静的コライダのリスト
	std::vector<std::weak_ptr<ColliderBase>> staticColliders_;

};

