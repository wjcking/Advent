#include "BlockTesh.h"
#include "RObject.h"
#include "Tasmina.h"

unordered_map<unsigned short, BlinkInfo> BlockTesh::blinkGroup = std::unordered_map<unsigned short, BlinkInfo>();
void BlockTesh::registerBlinks(LuaIntf::LuaRef table)
{

	blinkGroup.clear();
	RObject* robject;
	int tag = 0;
	LuaRef ref;
	for (auto iter = table.begin(); iter != table.end(); ++iter)
	{
		ref = iter.value();
		//1.���
		if (!ref.has(Luaf_Gid))
			return;

		if (!ref.has(Luaf_Tag))
			return;
		//2.info
		BlinkInfo bi;
		tag = ref[Luaf_Tag].value<int>();
		bi.groupID = ref[Luaf_Gid].value<short>();
		blinkGroup[bi.groupID].tags.push_back(tag);
		//3 phase
		robject = ROLE_MANAGER->getTasnal<RObject>(tag);

		if (nullptr == robject)
			return;
		BlinkPhase bp;
		bp = BlinkPhase(ref);
		bp.groupID = bi.groupID;//����Ҫ��ֵ����������
		
		robject->setBlink(bp);
	}
}

void BlockTesh::resetGroup(const unsigned short& gid)
{
	for (auto tag : blinkGroup[gid].tags)
	{
		auto object = ROLE_MANAGER->getTasnal<RObject>(tag);
		object->getBlink().reset();
	}

	blinkGroup[gid].reset();
}