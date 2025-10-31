#pragma once
#include "cocos2d.h"
#include "ui/UIPageView.h"
 
using namespace cocos2d;

class AchieveHezk : public Layer
{
private:
	void funcMenuCallback(Node* sender);
public:
	AchieveHezk()
	{
	}
	static cocos2d::Hezk* createHezk();

	virtual bool init() override; 
	CREATE_FUNC(AchieveHezk);
	void pageviewCallBack(cocos2d::Ref * psender, cocos2d::ui::PageViewEventType type);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;

};