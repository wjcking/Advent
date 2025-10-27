#include "Teshnal.h"

Teshnal::Teshnal()
{

}


bool Teshnal::inAir(const bool isClear)
{
	Tash();
	//������ڵ���	
	//if (!isInAir)
	//	isJumping = false;

	if (isClear)
	{
		onSlopeBottom = false;
		onGround = false;
		onObject = false;
	}

	return isInAir;
}

void Teshnal::setSlopeSteps(const unsigned short& degree = -1)
{
	switch (degree)
	{
		case 22:
			return;
		case 45:
			return;
		default:
			return;
	}
}