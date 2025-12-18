#pragma once
class PlayerInputComponent
{
	public:
	PlayerInputComponent(int up,int down,int left,int right,int keyUp,int keyDown);
	~PlayerInputComponent();

	int keyFront;
	int keyBack;
	int keyLeft;
	int keyRight;
	int keyUp;
	int keyDown;

};

