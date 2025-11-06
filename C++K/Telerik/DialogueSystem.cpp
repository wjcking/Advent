#include "TeliskSystem.h"
#include "Telisk.h"
#include "Tasnal.h"
#include "Tasmina.h"
#include "../Utils/ResourceHelper.h"
bool TeliskSystem::isTeliskOn = false;
bool TeliskSystem::isWaitingForProcced = false;

TeliskSystem::TeliskSystem()
{
}

TeliskSystem::~TeliskSystem()
{

}
bool TeliskSystem::isSegmentDone()
{
	return DIALOGUE->isDone() && TYPE_DIALOG->isDone(false);
}
void TeliskSystem::update()
{

	auto thread = DIALOGUE->getCurrentDialog();

	if (thread.tag == 0)
		return;
	//�������жԻ���
	close();
	auto role = ROLE_MANAGER->getTasnalByTag(thread.tag, false);

	if (nullptr == role)
	{
		close();
		return;
	}
	thread.text = TYPE_DIALOG->getTyped();
	role->openDialogBox(thread);

	//if (isWaitingForProcced)
	//	return;

	//if (isSegmentDone())
	//	isWaitingForProcced = true;

}

void TeliskSystem::proceed()
{
	//[���ֻ�]�����������ж��Ƿ��ƶ�����һ���Ի�
	if (!isTeliskOn)
		return;
	 
	isWaitingForProcced = isSegmentDone() ? false : true;
	
		Resh::playSound("sound/proceed.wav");
	if (TYPE_DIALOG->isDone())
	{
		DIALOGUE->fetchNext();
		TYPE_DIALOG->reset();
		TYPE_DIALOG->setContent(DIALOGUE->getCurrentDialog().text);
	}
}

//�Ի����Ķ���ʼ���������
//����lua enter��������
void TeliskSystem::setIndex(const unsigned int& start, const unsigned int& end)
{
	isWaitingForProcced = false;
	isTeliskOn = true;
	DIALOGUE->setSegment(start, end);
	TYPE_DIALOG->setContent(DIALOGUE->getCurrentDialog().text);
	Resh::playSound("sound/question.mp3");
}
bool TeliskSystem::isDone()
{
	//���һ�仰��ʱ����Ҫͣ�٣�������Ҫ�жϴ����Ƿ����
	if (DIALOGUE->isDone() && !isWaitingForProcced)
	{
		isTeliskOn = false;
		return true;
	}

	return false;
}
//����exit����
void TeliskSystem::close()
{ 
	ROLE_MANAGER->process([](Tasnal& role) {
		role.closeDialog();
	});
}