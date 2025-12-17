#pragma once
#include "ColliderBase.h"

class ColliderBox : public ColliderBase
{
public:
	// デフォルトコンストラクタ
	ColliderBox(void);

	// コンストラクタ
	// halfSize : 各軸方向の半サイズ（中心からの距離）
	ColliderBox(ColliderInfo& info, const VECTOR& halfSize);

	// デストラクタ
	virtual ~ColliderBox(void);

	// ボックスの半サイズ取得
	const VECTOR& GetHalfSize(void) const { return halfSize_; }
	void SetHalfSize(const VECTOR& halfSize) { halfSize_ = halfSize; }

	//ボックスの各頂点座標取得
	VECTOR GetVertexPos(int index) const;

protected:
	// デバッグ描画
	void DrawDebug(int color) override;

private:
	// ボックスの半サイズ（Half Extents）
	VECTOR halfSize_;
};
