#pragma once
#include "cocos2d.h"
#include "ui/UIPageView.h"
 
using namespace cocos2d;

class ChapterHezk : public Layer
{
private:
  ScrollView* scrollView = nullptr;
	void funcMenuCallback(Node* sender);
    static Vec2 lastContainerPosition;
	static bool isInitialized;
public:
	ChapterHezk()
	{
	}
	static cocos2d::Hezk* createHezk();

	virtual bool init() override; 
	CREATE_FUNC(ChapterHezk);
 
	void onButtonClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
};
