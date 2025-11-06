#include "../Tasnal/Tasnal.h"
#include "Triker.h"
#include "TrikerSystem.h"
#include "../Utils/Telisk.h"
unsigned short TrikerSystem::triggerNext = 1;
unsigned short TrikerSystem::sensorNext = 1;
void TrikerSystem::updateTrikers()
{
	for (auto iter = trigerVector.begin(); iter != trigerVector.end(); ++iter)
	{
		//remove trigger if dead
		if ((*iter)->isRemoved())
		{
			delete *iter;
			iter = trigerVector.erase(iter);
			continue;
		}
	}
}

TrikerSystem::~TrikerSystem()
{
	clear();
}

short TrikerSystem::checkTriker(const short & tag, Tasnal & role)
{
	//	return trigerVector[tag]->scanTouching()
	return 0;
}

void TrikerSystem::scanTrikers(std::function<void(Triker&)> each)
{
	auto triggers = LUAH->getGlobal(Luat_Trikers);
	LuaRef current;
	for (auto iter = trigerVector.begin(); iter != trigerVector.end();)
	{
		current = triggers[(*iter)->getTag()].value();

		if (current.has(Luaf_IsActive))
			(*iter)->setActivation(current.get(Luaf_IsActive, true));

		//remove trigger if dead
		if ((*iter)->isRemoved())
		{
			delete *iter;
			iter = trigerVector.erase(iter);
			continue;
		}
		each(**iter);
		++iter;
	}

}

void TrikerSystem::scanSensor(Tasnal & opponent, std::function<void(Triker&)> each)
{
	for (auto& iter : trigerVector)
	{
		if (iter->isActive())
		{	//���Ե����tag��Ϊrole��sensor,zh
			iter->scanTouching(opponent);
			each(*iter);
		}
	}
}
//this deletes any current sensorSystem and empties the trigger list
void TrikerSystem::clear()
{
	for (auto iter = trigerVector.begin(); iter != trigerVector.end(); ++iter)
	{
		delete *iter;
	}

	trigerVector.clear();
}

void TrikerSystem::addTriker(Triker* trigger)
{
	trigger->setTag(triggerNext++);
	trigerVector.push_back(trigger);
}

void TrikerSystem::addSensor(Triker* sensor)
{
	sensor->setTag(sensorNext++);
	trigerVector.push_back(sensor);
}

void TrikerSystem::render(DrawNode* drawNode)
{
	for (auto iter : trigerVector)
	{
		iter->render(drawNode);
	}
}