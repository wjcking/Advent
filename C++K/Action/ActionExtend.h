#ifndef __ACTIONEXTEND_H__
#define __ACTIONEXTEND_H__

#include "cocos2d.h"
#include "Telisk.h"
USING_NS_CC;

static const unsigned short   DRWhite = 0;
static const unsigned short   DRGray = 1;
//static const unsigned short   DRWhite = 0;
static const unsigned short   DROrange = 3;
static const unsigned short   DRRed = 4;
class ActionExtend
{
private:
	SpriteFrame* frame;
public:
	ActionExtend() :frame(nullptr)
	{

	}
	~ActionExtend()
	{
	}

	/*����һ�������߶���
	������
	t ʱ��
	startPoint		��ʼ��
	endPoint		������
	height			�߶ȣ�Ӱ�������ߵĸ߶ȣ�
	angle			�Ƕȣ������������������Ƶ���y��ļнǣ�ֱ��Ӱ�쾫����׳��Ƕȣ�
	*/
	static EaseInOut* CastArch(const float& t, const Vec2& startPoint, const Vec2& endPoint, 
														const float& height = 0, const float& angle = 60);
	static void corrupt(const Vec2&, const unsigned short& = DRWhite);
	static void toast(const LuaIntf::LuaRef& ref);
	static void explode(const LuaIntf::LuaRef& ref);

};
#endif 
