#pragma once
#include<DxLib.h>
class MoveComponent
{
	public:
	MoveComponent(float speed);
	~MoveComponent();

	//‘¬“xŽæ“¾
	float GetSpeed() { return speed; }
	//‘¬“xÝ’è
	void SetSpeed(float speed) { this->speed = speed; }
	//ˆÚ“®•ûŒüŽæ“¾
	VECTOR GetDirection() { return direction; }
	//ˆÚ“®•ûŒüÝ’è
	void SetDirection(VECTOR direction) { this->direction = direction; }

private:
	float speed;
	VECTOR direction;
};

