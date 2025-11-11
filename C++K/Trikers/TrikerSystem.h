#ifndef TRIGGERSYSTEM_H
#define TRIGGERSYSTEM_H 

#include "cocos2d.h"
class Tasnal;
class Triker;
class TrikerRegion;

class TrikerSystem
{
private:
	vector<Triker*> trigerVector; 
	void updateTrikers();
public:
	static unsigned short trikerNext;
	static unsigned short sensorNext ;
	~TrikerSystem();

	short checkTriker(const short& tag, Tasnal& role);
	void scanTrikers(std::function<void(Triker&)>);
	//���Ե����tag��Ϊrole��sensor
	void scanSensor(Tasnal& entity, std::function<void(Triker&)>);
	void clear(); 
	void addTriker(Triker*);
	void addSensor(Triker*);
	void render(DrawNode* drawNode);
	inline int  getCount() const { return  trigerVector.size(); };
};


#endif