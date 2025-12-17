#pragma once  
#include <DxLib.h>  
#include <cstdint>  
#include<initializer_list>

#include "ColliderShape.h"  
#include "ColliderLayer.h"  

#include "../ActorTag.h"  
#include "../../Common/Transform.h"

class Transform;  
class ColliderBase  
{  
public:  
	struct ColliderInfo
	{
		SHAPE shape;	//形状
		Transform* targetTransform;	//ターゲットのTransform
		TAG tag;	//当たり判定のタグ
		Layer layer;	//当たり判定を行うレイヤーのマスク
		uint32_t mask;	//当たり判定を受け付けるレイヤーのマスク
		VECTOR localPos;	//ローカル座標
		VECTOR localRot;	//ローカル回転
		bool isTrigger;	//トリガー判定かどうか
		bool isActive;	//有効かどうか
		float weight;	//重み(0　影響なし　１　完全影響)
	};
public:

	//デフォルトコンストラクタ
   ColliderBase(void);
   //コンストラクタ
   ColliderBase(ColliderInfo& info);


   //デストラクタ
   virtual ~ColliderBase(void);  

   // 初期化  
   virtual void Init(void);  

   // 更新  
   virtual void Update(void);  

   // 描画  
   virtual void Draw(void);  

   // 解放  
   virtual void Release(void);  

	//コライダ情報取得
	const ColliderInfo& GetColliderInfo() const { return colliderInfo_; }

	//親Transform取得
	Transform* GetFollow(void) const { return colliderInfo_.targetTransform; }

protected:  
	// ローカル座標から回転後のワールド座標を取得
   VECTOR GetRotPos(const VECTOR& localPos) const;  
   // デバッグ描画
   virtual void DrawDebug(int color) {};  

   // レイヤービット取得
   static constexpr uint32_t LAYER_BIT(Layer layer)  
   {  
       return 1 << static_cast<uint32_t>(layer);  
   }  

   // レイヤーマッチ判定
   static bool IsLayerMatch(Layer layerA, uint32_t maskA)  
   {  
       return (maskA & LAYER_BIT(layerA)) != 0;  
   }  
protected:  
	// コライダ情報
	ColliderInfo colliderInfo_;

private:  
	// マスク作成
   static uint32_t MakeMask(std::initializer_list<Layer> layers)  
   {  
       uint32_t mask = 0;  
       for (Layer layer : layers)  
           mask |= LAYER_BIT(layer);  
       return mask;  
   }  
private:
	// デバッグ表示の色
	static constexpr int COLOR_VALID = 0xff0000;
	static constexpr int COLOR_INVALID = 0xaaaaaa;
};
