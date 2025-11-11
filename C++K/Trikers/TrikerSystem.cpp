#include "../Tasnal/Tasnal.h"
#include "Triker.h"
#include "TrikerSystem.h"
#include "../Utils/Telisk.h"
unsigned short TrikerSystem::trikerNext = 1;
unsigned short TrikerSystem::sensorNext = 1;
void TrikerSystem::updateTrikers()
{
	for (auto iter = trigerVector.begin(); iter != trigerVector.end(); ++iter)
	{
		//remove triker if dead
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
	auto trikers = LUAH->getGlobal(Luat_Trikers);
	LuaRef current;
	for (auto iter = trigerVector.begin(); iter != trigerVector.end();)
	{
		current = trikers[(*iter)->getTag()].value();

		if (current.has(Luaf_IsActive))
			(*iter)->setActivation(current.get(Luaf_IsActive, true));

		//remove triker if dead
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
//this deletes any current sensorSystem and empties the triker list
void TrikerSystem::clear()
{
	for (auto iter = trigerVector.begin(); iter != trigerVector.end(); ++iter)
	{
		delete *iter;
	}

	trigerVector.clear();
}

void TrikerSystem::addTriker(Triker* triker)
{
	triker->setTag(trikerNext++);
	trigerVector.push_back(triker);
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