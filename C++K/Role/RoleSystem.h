#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include <functional>
#include <unordered_map> 
#include "../Utils/Telisk.h"
using namespace std;

class Tasnal;
class Player;
class Npc;
class RObject;
class ProjectTile; 

#define ROLE_MANAGER TasnalSystem::getInstance()
#define ROLE_PLAYER TasnalSystem::getPlayerRefer()  

class TasnalSystem
{
	private:
	unordered_map<unsigned int, Tasnal*> entityMap;

	TasnalSystem();
	TasnalSystem(const TasnalSystem&);
	TasnalSystem& operator=(const TasnalSystem&);
	static Player* player;

	public:
	static TasnalSystem* getInstance();
	//static Player* getPlayer();
	static Player& getPlayerRefer();
	void process(function<void(Tasnal&)> roleAction);
	static void updateTasnal(const int&, const bool&);
	void  registerTasnal(Tasnal* NewEntity);
	//�ڽ�ɫ���ٺ󣬻�����Ҳ���������Ի������ڸý�ɫ�Ļ�����null�����ж���
	Tasnal* getTasnalByTag(const int&, bool allowAssert = true) const;
	template<class T>
	inline T* getTasnal(const int& tag)
	{
		return dynamic_cast<T*>(getTasnalByTag(tag, false));
	}
	void removeTasnal(Tasnal* pEntity);
	void removeTasnal(const unsigned int& tag) { entityMap.erase(tag); };
	void release();
	//�����ɫ �Լ���ɫ��Ҫ��֡
	void loadScript();
	static void registerLuaTasnal(LuaIntf::LuaRef);
	//1.���м��ص�ʱ������ 2.����������
	static void setTeshnal(LuaIntf::LuaRef ref, Tasnal* role);
	//����
	static void appendTasnal(LuaIntf::LuaRef);
};
#endif