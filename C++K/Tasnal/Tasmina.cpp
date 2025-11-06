#include "Tasnal.h"
#include "Tasmina.h"
#include "../Map/MapManager.h"
#include "../Map/TiledMap.h"
#include "../Map/CheckPoint.h"
#include "ProjectTile.h"
#include "Player.h"
#include "Npc.h"
#include "RObject.h"
#include "../Utils/Constant.h"
#include "../Triggers/Trigger.h"
#include "../Triggers/TriggerSystem.h"
Player* Tasmina::player = nullptr;

/*��̬���ʼ��*/

Tasmina::Tasmina()
{
	entityMap.reserve(Reserve_Tasnal);
}
Tasmina* Tasmina::getInstance()
{
	static Tasmina instance;
	return &instance;
}
Player& Tasmina::getPlayerRefer()
{
	if (nullptr == player)
		player = dynamic_cast<Player*>(ROLE_MANAGER->getTasnalByTag(ID_Player));

	return *player;
}

void Tasmina::process(function<void(Tasnal&)> roleAction)
{
	for (auto role : entityMap)
		roleAction(*role.second);
}

/*����id�ͷ����õ�����*/
Tasnal* Tasmina::getTasnalByTag(const int& id, bool allowAssert)const
{
	//find the entity
	auto ent = entityMap.find(id);

	if (allowAssert)
	{
		assert((ent != entityMap.end()) && "<Tasmina::GetEntityFromID>: û�д�ID");
		assert((ent->second != nullptr) && "role ��mapmanager ����Ϊ��");
		return  ent->second;
	}

	return ent != entityMap.end() ? ent->second : nullptr;
}

void Tasmina::removeTasnal(Tasnal* entity)
{
	entityMap.erase(entityMap.find(entity->getTag()));
}

void Tasmina::registerTasnal(Tasnal* newEntity)
{
	newEntity->generateTag();
	entityMap.insert(std::make_pair(newEntity->getTag(), newEntity));
}

void Tasmina::updateTasnal(const int & tag, const bool & isDisposed)
{ 
	auto role = ROLE_MANAGER->getTasnalByTag(tag,false);

	if (nullptr == role)	return;

	//һ��Ҫ��lua��ɾ���������ֻ��ᱨ��
	if (TasnalType::player == role->getType())
	{
		auto player = dynamic_cast<Player*>(role);
		player->update();
	}
	else if (TasnalType::npc == role->getType())
	{
		auto npc = dynamic_cast<Npc*>(role);
		npc->update();
	}
	else if (TasnalType::robject == role->getType())
	{
		auto object = dynamic_cast<RObject*>(role);
		object->update();
	}
	else if (TasnalType::projectTile == role->getType())
	{
		auto projectTile = dynamic_cast<ProjectTile*>(role);
		projectTile->update();

		if (isDisposed)
			role->getWeaponSystem().clear();
	}

	if (isDisposed)
	{
		if (nullptr != role->frameIndexes)
			delete[] role->frameIndexes;

		role->getMap()->removeChild(role);
		ROLE_MANAGER->removeTasnal(tag);
	}
}

void Tasmina::release()
{
	//�л�����������
	player = nullptr;

	for (auto w : entityMap)
	{
		w.second->stopAnimation();
		w.second->getWeaponSystem().clear();
		if (nullptr != w.second->frameIndexes)
			delete[] w.second->frameIndexes;
	}
	entityMap.clear();
}

void Tasmina::loadScript()
{
	auto entity = LUAH->getGlobal(Luat_Tasnal);

	assert(entity.isTable() && "������entity");
	assert(entity.len() > 0 && "����Ҫ��һ����ɫ");
	//���ñ�ʶ
	Tasnal::nextTag = 1;
	//���
	release();
	for (auto iter = entity.begin(); iter != entity.end(); ++iter)
	{
		registerLuaTasnal(iter.value());
	}
	//����ÿ��role�ű��ֿ���
	for (auto role : entityMap)
		role.second->loadScript();

}