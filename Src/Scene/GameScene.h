#pragma once
#include "SceneBase.h"

#include <vector>
#include <memory>



class ActorBase;

class GameScene :
    public SceneBase
{
public:
	GameScene(void);
	~GameScene(void)override;

	void Load(void)override;


	// ‰Šú‰»
	void Init(void) override;

	// XV
	void Update(void) override;

	// •`‰æ
	void Draw(void) override;

	// ‰ğ•ú
	void Release(void) override;
private:
	std::vector<std::shared_ptr<ActorBase>> actors_;


private:
	int ShadowMapHandle_ = -1;
};

