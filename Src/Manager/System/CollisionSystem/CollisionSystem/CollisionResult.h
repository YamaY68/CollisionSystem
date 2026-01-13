#pragma once
#include<DxLib.h>
struct CollisionResult
{
	bool isHit = false;

	// 接触情報（共通）
	VECTOR normal;         // A → B 方向の法線

	float penetration;    // めり込み量

	float pushA = 0.0f;     // A に適用する押し出し量
	float pushB = 0.0f;     // B に適用する押し出し量
};