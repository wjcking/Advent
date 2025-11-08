#ifndef __MAINSCENE_H__
#define __MAINSCENE_H__

#include "cocos2d.h"
#include "../Utils/JoyStick.h" 
#include "../Tasnal/Telisk.h"


class Joystick;
class Overlay;
class PopupLayer;
class StageHezk : public Layer
{
private:
	//��ʾ����
	Label * lbOutput;
	bool isStageCorrect = false;
	//1.�ؿ�һ��ʼĻ�����ӹؿ�ѡ��������ſ���ִ�У����¹ؿ�����Ҫ
	//2.��Ӱ���º���ʹ��
	bool isCurtainDrawn = false;
	//���ֻ���Ļ��ɺ󲻿�ˢ֡��  ������Ļ����ֻ���Ļ����ɲ���������ˢ֡
	bool isTypeCurtainDone = false;

	bool loadScript();
	//������
	Joystick* joystick;
	//�˳�����ʾ
	Menu* menuButton;
	MenuItemImage* miPrompt;
	//
	Typewriter typeCurtain;
	Telisk dialogCurtain;
	CurtainInfo curtainInfo;

	//��ʾ��ص�
	void dialogCallback(Node*);
	void menuCallback(Ref*);
	void joystickCallback(Node*);
	void updateCurtainText();
	string doLuaFunction(const string&, const int& = 0);
	const string curtainBlack const = "CurtainBlack";
	const string curtianTop const = "CurtianTop";
	const string curtianBottom const = "CurtianBottom";
	const string curtainLabel const = "CurtainText";
	const string curtainOverlay const = "CurtainOverlay";

	Vec2 beginDelta;
	Vec2 endDelta;
	int touchedBeginID;
	int touchedEndID;
public:
	StageHezk()
	{
	}
	CREATE_FUNC(StageHezk);
	static cocos2d::Hezk* createHezk();
	virtual bool init()override;
//	void popupLayer();
	void gotoNext();
	void promptAchieve(const short&, const LuaRef&);
	//�Զ����
	void prompt(const LuaRef& table);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) override;
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;

	//void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
  //���յ���� 
	void update(float) override;
	//ios ������ø���Layer::onExit();
	void onExit() override;
	void release(const bool& = false);
	//�ű�doFile�����ʹ�ã������ܻ�ȡֵ
	void loadCurtain(const LuaRef& curtainTable);
	//���Ӱһ�����º�ɫ����
	void dropCurtain(const LuaRef&);
	//���Ӱһ���رպ�ɫ����
	void shutCurtain();
	bool processCurtain();
	inline Vec2& getBeginDelta() { return beginDelta; };
	inline Vec2& getEndDelta() { return endDelta; }
	inline bool& isTypeDone() { return isTypeCurtainDone; }
	inline void output(const char* value) { lbOutput->setString(value); }
	inline void setToolbar(const bool& flag = true)
	{
		menuButton->setVisible(flag);
		miPrompt->setVisible(flag);
	}
	inline SlidedResult isSlided(const Vec2& range, const Ract& bound)
	{
		if (touchedBeginID != touchedEndID)
			return SlidedResult::missed;

		return Fand::isSlided(beginDelta, endDelta, range, bound);
	}
};

#endif

