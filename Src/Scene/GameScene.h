#pragma once
#include "SceneBase.h"

#include <vector>
#include <memory>
#include"../Object/Actor/System/CollisionSystem/CollisionSystem.h"


class ActorBase;

class GameScene :
    public SceneBase
{
public:
	GameScene(void);
	~GameScene(void)override;

	void Load(void)override;


	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;
private:
	std::vector<std::shared_ptr<ActorBase>> actors_;
	// 衝突判定システム
	CollisionSystem collisionSystem_;

private:
	int ShadowMapHandle_ = -1;
};

