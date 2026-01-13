#pragma once
#include<DxLib.h>
#include<vector>
#include<cstdint>
#include<memory>
#include<map>
#include<functional>
#include"../../../../Object/Actor/Collider/ColliderBase.h"
#include"CollisionLogic.h"

using ContactCallback = std::function<void(std::uint32_t, std::uint32_t)>;
//フレーム間で重なっているペアを保持する
using pair = std::vector<std::pair<std::size_t, std::size_t>>;

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

	//コールバック設定
	void SetContactCallbacks(ContactCallback onBegin, ContactCallback onEnd)
	{
		onBegin_ = onBegin;
		onEnd_ = onEnd;
	}


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

	//エンティティID設定
	void BindDynamicEntityId(std::size_t dynamicIndex, std::uint32_t entityId);
	void BindStaticEntityId(std::size_t staticIndex, std::uint32_t entityId);

	/// @brief 新しく当たったもの、離れたものを抽出
	/// @param currentPairs	今フレームの接触ペア
	/// @param prevPairs 前フレームの接触ペア
	/// @param beginPairs 新しく発生したペア
	/// @param endPairs 消えたペア
	void DiffPairs(pair& currentPairs, pair& prevPairs, pair& beginPairs, pair& endPairs);
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

	//動的コライダーのエンティティID対応表
	std::vector<std::int32_t>dynamicIndex2EntityId_;
	//静的コライダーのエンティティID対応表
	std::vector<std::int32_t>staticIndex2EntityId_;

	//前フレームの重なりペア
	pair prevPairs_;

	//衝突開始コールバック
	ContactCallback onBegin_, onEnd_;
};

