#ifndef FRAMECOUNTER_H
#define FRAMECOUNTER_H
#include <unordered_map>

class Clock
{
	static float tik;
	static int iTick;

	//ÿ�μ�0.005
	static float&  getTickFloat();
	static int&  getTickInteger();
	static float&  addDeltaCount();
};

//�ӳٶ���������
struct DelayPhase
{
	float delaySecond;
	float timestramp;

	DelayPhase(const float& d = 1.f)
	{
		delaySecond = d;
		timestramp = 0.f;
	};

	//�Ƿ�ʱ��
	bool isTimeUp()
	{
		if (timestramp == 0.f)
			timestramp = Clock::getTickFloat() + delaySecond;

		return Clock::getTickFloat() > timestramp;
	}

	inline
	void reset() { timestramp = 0.f; }
};

class DelayCollection
{
private:
	static unordered_map<short,DelayPhase> delay;
public:
	static void  clear();
	static void  reset(const short&);
	static bool isTimeUp(const short&,const float&);
};


#endif