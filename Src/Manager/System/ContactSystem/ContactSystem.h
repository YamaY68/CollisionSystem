#pragma once
#include<cstdint>
#include<map>
#include<set>
#include<vector>

#include"../CollisionSystem/CollisionSystem/CollisionResult.h"

using EntityID = std::uint32_t;

class ContactSystem
{
public:
	//コリジョンシステムからの接触イベント
	void OnBeginContact(EntityID a, EntityID b,CollisionResult result);
	void OnEndContact(EntityID a, EntityID b, CollisionResult result);

	struct BeginEvent
	{
		EntityID a;
		EntityID b;
	};

	struct EndEvent
	{
		EntityID a;
		EntityID b;
	};

private:

	//このIDがどのIDたちと当たっているかを格納
	std::map<EntityID, std::set<EntityID>> touching_;

	//今フレームで始まった接触を記録
	std::vector<BeginEvent> beginEvents_;

	//今フレームで終わった接触を記録
	std::vector<EndEvent> endEvents_;
};

