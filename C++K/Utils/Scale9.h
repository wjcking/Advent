#pragma once
#include "cocos2d.h"
#include "ui/UIScale9Sprite.h"
#include "Telisk.h"

USING_NS_CC;
using namespace ui;
class Role;
class DialogBox: Scale9Sprite
{
private:
	Label* label;
	static std::vector<TeliskText> dialogueList;
public:
	static void pushTelisk(const TeliskText&);
	bool display(); 
};