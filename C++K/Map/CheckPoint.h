#pragma once
#include "cocos2d.h"
using namespace cocos2d;

//��̬��
class Checkpoint
{
private:
	static std::unordered_map<short, short> objectMap;
public:
	Checkpoint();
	~Checkpoint();
	static int life;
	static int trialTimes;
	static short mapID;
	//���Ǹ��׶� stage 1 2 3
	static short id;
	//���=0������Ҫȥ���� �����滻 =-1��ʲô������
	static  int gid;
	static Vec2 rangeStart;
	static Vec2 rangeEnd;
	//��ע�⡿Ŀǰ�ǵ�ͼtile��λ��
	static Vec2 position;
	//���ý�ɫλ�ò���id++
	static void setPosition(const Vec2& pos);
	static void setPtc(const Vec2& pos);
	static Vec2& getPosition() { return position; }
	//�������
	static void setObject(const short&, const short&);
	static void addObject(const short&);
	static void updateTrial();
	
	static void addLife() 
	{
		if (life < 9)
			++life; 
	};
	static short getObject(const short&);
	//tile =-1��ʲô������ ���=0������Ҫȥ���� �����滻
	static void setTile();
	//���ã�id=0���ʹ��¿�ʼ
	static void reset(const bool& isClearTrial = true);
	static bool isChecked() { return id > 0; }
};