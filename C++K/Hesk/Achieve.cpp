#include "AchieveHezk.h"
#include "../Utils/ResourceHelper.h"
#include "../Utils/Funcpads.h"
#include "../Utils/Telisk.h"
#include "../Utils/Constant.h"
#include "ui/UIButton.h"
#include "StageHezk.h"
using namespace ui;

cocos2d::Hezk* AchieveHezk::createHezk()
{
	auto scene = Hezk::create();
	auto layer = AchieveHezk::create();
	scene->addChild(layer);
	return scene;
}

bool AchieveHezk::init()
{
	if (!Layer::init())
		return false;
	auto achieveTable = LUAH->getGlobal(Luat_Achievements);
	CCASSERT(achieveTable.isTable(), StringUtils::format("������%s", Luat_Achievements).c_str());

	auto bgSky = Sprite::create("background/achieve.png");
	addChild(bgSky, Z_BACKGROUND);
	bgSky->setPosition(bgSky->getContentSize().width / 2.f, bgSky->getContentSize().height / 2.f - 2);

	//LUAH->doStage();
	LUAH->doHezk();
	auto  funcMenu = FuncMenu::create();
	funcMenu->callbackFunc(this, callfuncN_selector(AchieveHezk::funcMenuCallback));
	funcMenu->loadScript("AchieveMenu");
	funcMenu->setPosition(Vec2::ZERO);
	addChild(funcMenu, Z_JoyStick);


	auto pageview = PageView::create();
	pageview->setLocalZ(-100);
	//pageview->setTouchEnabled(true);
	pageview->setContentSize(Size(ScreenSize.width, ScreenHeight * 0.7));
	
	pageview->setBackGroundColor(Color3B(0, 255, 0));

	//����4��page 
	float multi = 0.5f;
	const float dividedParam = 2.0f;
	float dividedY = dividedParam;
	const float offsetX = 20;
	unsigned short achieveIndex = 1;
	auto pageCount = achieveTable.len() / 6;
	auto achieveCount = achieveTable.len() < 6 ? achieveTable.len() : 6;
	pageCount = pageCount == 0 ? 1 : pageCount;
	LuaIntf::LuaRef ref;
 
	std::string achieveImageName;
	for (int i = 0; i < pageCount; i++)
	{
		Layout * layout = Layout::create();
		layout->setContentSize(pageview->getContentSize());
		for (int j = 1; j <= achieveCount; j++)
		{
			ref = achieveTable[achieveIndex].value();
			auto btnAchieve = Button::create("control/achieve/bg.png", "control/achieve/bg.png");
			achieveImageName = StringUtils::format("control/achieve/aimg%d.png", achieveIndex);
			//���������ͼƬ��������ʹ��Ĭ��
			if (!FileUtils::getInstance()->isFileExist(achieveImageName))
				achieveImageName = "control/achieve/aimg.png";
		 
			auto aimg = Sprite::create(achieveImageName);
			aimg->setPosition(aimg->getContentSize().width / 1.4, btnAchieve->getContentSize().height / 2);
			btnAchieve->addChild(aimg);
			auto text = Label::createWithSystemFont(ref[Luaf_Text].value<std::string>(), Resh::getFontName(), 14.f);
			text->setColor(Color3B::BLACK);
			text->setPosition(aimg->getContentSize().width * 2 + offsetX, btnAchieve->getContentSize().height / 2);


			btnAchieve->addChild(text);
			btnAchieve->setName(StringUtils::format("%s%d", User_Achieve, achieveIndex));
			btnAchieve->setPosition(Vec2(btnAchieve->getContentSize().width * multi + 40 * multi, pageview->getContentSize().height / dividedY));
			//���û�д������
			//���AchieveIndex ++
			if (!Resh::getAchieve(achieveIndex++))
			{
				btnAchieve->runAction(FadeTo::create(0.1f, 88));
				aimg->runAction(FadeTo::create(0.1f, 88));
			}
			layout->addChild(btnAchieve);
			if (j % 3 == 0)
			{
				dividedY = dividedParam * 2.5f;
				//����
				multi = 0.5f;

			}
			else
				multi++;
		}
		//����
		multi = 0.5f;
		dividedY = dividedParam;
		pageview->addPage(layout);
	}

	//pageview��
	pageview->setPosition(Vec2(80, ScreenHeight / 4));
	//pageview->addEventListener(CC_CALLBACK_2(this, &pagevieweventselector(AchieveHezk::pageviewCallBack));
	this->addChild(pageview, Z_JoyStick);

	return true;
}
void AchieveHezk::funcMenuCallback(Node * sender)
{
	//if (callback && callbackListener)
	//{
	//	(callbackListener->*callback)(menuItem);
	//}
}
 
void AchieveHezk::pageviewCallBack(cocos2d::Ref * psender, cocos2d::ui::PageViewEventType type)
{

}

bool AchieveHezk::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	return false;
}

void AchieveHezk::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
}
