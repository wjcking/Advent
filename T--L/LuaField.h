#pragma once
#include "cocos2d.h"
#include "Constant.h"
#include "Fand.h"
#include "../../Common/lua/include/lua.hpp" 
#include "../../Common/Lua-Intf/LuaIntf.h" 

using namespace LuaIntf;
using namespace cocos2d;

struct Luaf
{
	Luaf() {};
	Luaf(LuaRef);
	float delay = 0.f;
	string name = "";
	//�еļ���չ���еĲ��� .png ������
	string file = "";
	Vec2 position = Vec2::ZERO;
	Vec2 offset = Vec2::ZERO;
	float duration;
	int zorder = 0;
};
//���������Ϣ
struct FontInfo
{
	FontInfo() {};
	explicit FontInfo(LuaRef);
	float size = 16.f;
	bool isBold = false;
	cocos2d::Color3B color = cocos2d::Color3B::BLACK;
};

struct CurtainInfo : Luaf
{
	CurtainInfo() {};
	CurtainInfo(LuaRef);
	CurtainType type = CurtainType::none;
	//��Ļ ����߻��ϱ�
	string image = "background/curtain.png";
	//��Ļ ����߻��ϱ�
	string image1 = "background/curtainleft.png";
	//�ұ߻��±�
	string image2 = "background/curtainright.png";

	FontInfo font;
};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINZK) 
#pragma comment(lib, "lua5.3.4.lib")
#endif