#include "ProjectTile.h"
#include "RoleSystem.h"

#include "../Map/MapManager.h"
#include "../Map/TiledMap.h"


ProjectTile::ProjectTile() : Role()
{
	isGravityOn = false;
	//�Զ�Ԥ�����ò���ײ��Ƭ
	onCheckTile = false;
	type = RoleType::projectTile;
	//isSolid = projectType == ProjectType::footboard;
	setLocalZOrder(Z_ROLE);
	//��ʱ 
	//��ҪԤ������isDispose���������������Ļ�Ĭ��Ϊtrue
	//LUAH->setDisposal(getTag(), false);
}
ProjectTile::~ProjectTile()
{
}

void ProjectTile::setOwner(Role& owner)
{
	this->owner = &owner;
	towards = owner.getFacedDirection();
	switch (towards)
	{
	case MovingDirection::toLeft:
		originPosition = Vec2(owner.getPositionX() - offset.x, owner.getPositionY());
		break;
	case MovingDirection::toRight:
		originPosition = Vec2(owner.getPositionX() + offset.x, owner.getPositionY());
		break;
	case MovingDirection::toTop:
		setFlippedY(false);
		originPosition = Vec2(owner.getPositionX(), owner.getPositionY() + offset.y);
		break;
	case MovingDirection::toBottom:
		setFlippedY(false);
		originPosition = Vec2(owner.getPositionX(), owner.getPositionY() - offset.y);
		break;
	}
	setPosition(originPosition);

}

bool ProjectTile::isDead()
{
	Vec2 distance = getPosition() - originPosition;
	if (abs(distance.x) > ScreenSize.width || abs(distance.y) > ScreenSize.height)
		return true;

	return false;
}
void ProjectTile::update()
{
	//�������λ��
	//����footboard���棬�������
	Role::update();
}
void ProjectTile::setProject(const LuaRef&  ref)
{
	//auto projectLua ref.get(Luaf_Project)
	if (ref.has(Luaf_Offset))
		offset = ref.get(Luaf_Offset).toValue<Vec2>();
	if (ref.has(Luaf_AllowThrough))
		canThough = ref.get(Luaf_AllowThrough, false);
	//1.���м��ص�ʱ������ 2.����������
	RoleSystem::setTeshnal(ref, this);
}
/*
* ��������
*/
SlugObject::SlugObject() : ProjectTile()
{
}

void SlugObject::setProject(const LuaRef & ref)
{
	 ProjectTile::setProject(ref);
}

bool SlugObject::isDead()
{
	Vec2 distance = getPosition() - originPosition;
	if (abs(distance.x) > range.x || abs(distance.y) > range.y)
		return true;

	return false;
}
void SlugObject::update()
{ 
	switch (towards)
	{
	case MovingDirection::toLeft:
		setBackward();
		break;
	case MovingDirection::toRight:
		setForward();
		break;
	case MovingDirection::toTop:
		setUpward();
		break;
	case MovingDirection::toBottom:
		setDownward();
		break;
	}
	ProjectTile::update();
	//�ƶ���
	//gotoDesirePosition();
}
/*
* ̤��
*/
Footboard::Footboard() : ProjectTile(), delayDisappeared(3.f)
{
	isSolid = true;
	projectType = ProjectType::footboard;
}

void Footboard::setProject(const LuaRef&  ref)
{
	ProjectTile::setProject(ref);
	if (ref.has(Luaf_Delay))
		delayDisappeared = ref.get(Luaf_Delay).toValue<float>();
}
bool Footboard::isDead()
{
	if (delayDisappeared.isTimeUp())
		return true;

	return false;
}
void Footboard::update()
{
	//������ϣ����ǲ��ܲȵ�����
	ProjectTile::update();
}
/*
* ��������
*/
Deadshot::Deadshot() : ProjectTile()
{
	projectType = ProjectType::deadshot;
}

bool Deadshot::isDead()
{
	if (nullptr == target)
		return true;

	return ProjectTile::isDead();
}
void Deadshot::setProject(const LuaRef&  ref)
{
	ProjectTile::setProject(ref);
	if (ref.has(Luaf_Target))
	{
		auto tag = ref.get(Luaf_Target).toValue<int>();
		target = ROLE_MANAGER->getRoleByTag(tag, false);
	}
}

void Deadshot::update()
{
	ProjectTile::update();

	if (nullptr == target)
		return;

	bDestiny = locate(target->getPosition());
}

 
