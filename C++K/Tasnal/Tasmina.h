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

#define ROLE_MANAGER Tasmina::getInstance()
#define ROLE_PLAYER Tasmina::getPlayerRefer()  

class Tasmina
{
	private:
	unordered_map<unsigned int, Tasnal*> entityMap;

	Tasmina();
	Tasmina(const Tasmina&);
	Tasmina& operator=(const Tasmina&);
	static Player* player;

	public:
	static Tasmina* getInstance();
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
};
#endif