#pragma once
#include "Tasnal.h"

class ProjectTile : public Tasnal
{
protected:
	bool bDestiny = false;
	Tasnal* owner;
	Tasnal* target;
	Vec2 offset = Vec2(30, 30);
	//�ӵ����� 0= ��ͨ�Զ� 1=���ݣ��೤ʱ�����ʧ����timeOfDisappear��
	ProjectType projectType = ProjectType::object;
	MovingDirection towards;
	Vec2 range = Vec2(ScreenSize.width, ScreenSize.height);
	bool canThough = false;
public:
	ProjectTile();
	~ProjectTile();
	//�����ǽ�ɫ����������ӵ��������ƶ�
	void setOwner(Tasnal&);
	inline bool& isDestiny() { return bDestiny; }
	inline Tasnal& getOwner() { return *owner; }
	inline Tasnal& getTarget() { return *target; }
	inline void setTarget(Tasnal& target) { this->target = &target; };

	//��lua�˵��ã�����ڲ����õĻ��������ӵ���ȫ����������tag�й�
	virtual bool isDead() override;
	virtual void setProject(const LuaIntf::LuaRef&);
	virtual inline ProjectType& getProjectType() { return projectType; }
	inline void setProjectType(const ProjectType& projectType) { this->projectType = projectType; }
	void update() override;
	inline bool& allowThrough() { return canThough; }
};

class SlugObject : public ProjectTile
{
private:
	//bool allowRelocate = false;
public:
	SlugObject();
	void setProject(const LuaRef&) override;
	bool isDead() override;
	void update() override;
};

class Footboard :public ProjectTile
{
private:
	DelayPhase delayDisappeared;
public:
	Footboard();
	bool isDead() override;
	void update() override;
	void setProject(const LuaRef&) override;
};

class Deadshot :public ProjectTile
{
public:
	Deadshot();

	void setProject(const LuaRef&) override;
	bool isDead() override;
	void update() override;
};

class Trace :public ProjectTile
{
private:
	Vec2 target = Vec2::ZERO;
public:
	inline Trace() { projectType = ProjectType::trace; };
	inline void setAim(const Vec2& val) { target = val; };
	inline void setProject(const LuaRef& ref)override { ProjectTile::setProject(ref); };
	inline bool isDead() override { return ProjectTile::isDead(); }
	inline void update() override { bDestiny = locate(target); }
};