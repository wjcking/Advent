#ifndef TRIGGERSYSTEM_H
#define TRIGGERSYSTEM_H 

#include "cocos2d.h"
class Tasnal;
class Trigger;
class TriggerRegion;

class TriggerSystem
{
private:
	std::vector<Trigger*> trigerVector; 
	void updateTriggers();
public:
	static unsigned short triggerNext;
	static unsigned short sensorNext ;
	~TriggerSystem();

	short checkTrigger(const short& tag, Tasnal& role);
	void scanTriggers(std::function<void(Trigger&)>);
	//���Ե����tag��Ϊrole��sensor
	void scanSensor(Tasnal& entity, std::function<void(Trigger&)>);
	void clear(); 
	void addTrigger(Trigger*);
	void addSensor(Trigger*);
	void render(DrawNode* drawNode);
	inline int  getCount() const { return  trigerVector.size(); };
};


#endif