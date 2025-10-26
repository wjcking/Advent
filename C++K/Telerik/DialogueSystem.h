#pragma once
#include "TeliskSystem.h"
class TeliskSystem
{
private:
	//�����ж��Ƿ����һ��Ի�
	static bool isWaitingForProcced;
	static bool isSegmentDone();
public:
	TeliskSystem();
	~TeliskSystem();
	static bool isTeliskOn; 
	static void update();
	static void proceed();
	static void close();
	static void setIndex(const unsigned int& start, const unsigned int& end);
	static bool isDone();
};

