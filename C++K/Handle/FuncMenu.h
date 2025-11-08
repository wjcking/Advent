#pragma once
#include "cocos2d.h"
#include "Fand.h"
#include "Constant.h"

class FuncMenu : public Menu
{
private:
	static short nextTag;
	Ref* callbackListener;
	CallFunc callback;
	void menuCallback(Ref* pSender);
	const char* table;
public:
	virtual bool init() override;
	create_func(funcmenu);
	void callbackFunc(Ref* target, SEL_CallFuncN callfun);
	void loadScript(const char* padTable);
};