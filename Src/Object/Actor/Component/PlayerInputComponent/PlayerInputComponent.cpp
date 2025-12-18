#include "PlayerInputComponent.h"

PlayerInputComponent::PlayerInputComponent(int up, int down, int left, int right, int keyUp, int keyDown)
	:
	keyFront(up),
	keyBack(down),
	keyLeft(left),
	keyRight(right),
	keyUp(keyUp),
	keyDown(keyDown)
{
}

PlayerInputComponent::~PlayerInputComponent()
{
}
