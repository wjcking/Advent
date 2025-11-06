


void Tasmina::appendTasnal(LuaIntf::LuaRef ref)
{
	CCASSERT(ref.isTable(), "Tasmina:appendTasnal error");
	auto roleList = LUAH->getGlobal(Luat_Tasnal);
	registerLuaTasnal(ref);
}


void setTeshnal(LuaIntf& ref, Tasnal* tasnal)
{
	//Luaf_FrameIndexes
		tasnal->registerFrameIndexes(ref.get(Luaf_FrameIndexes));
	//�������Ժ��ٲſ��Ը���prefix
	//Luaf_FramePrefix))
		tasnal->framePrefix = ref.get(Luaf_FramePrefix).toValue<std::string>();
	//Luaf_AC))
		tasnal->allowCollision(ref[Luaf_AC].value<CollisionDirection>());
	//Luaf_DC))
		tasnal->disallowCollision(ref[Luaf_DC].value<CollisionDirection>());

	//Luaf_AllowCollision))
		tasnal->allowCollision(ref[Luaf_AllowCollision].value<CollisionDirection>());
	//Luaf_DisallowCollision))
		tasnal->disallowCollision(ref[Luaf_DisallowCollision].value<CollisionDirection>());

	tasnal->setAllowFollow(ref.get(Luaf_AllowFollow, false));
	//Luaf_AllowPush))
		tasnal->EndPush(ref[Luaf_AllowPush].value<CollisionDirection>());
	//��ͼΪˮƽ�ӽ����������������������ȫ�������ⲻ��Ҫ����
	if (tasnal->getMap()->getViewType() == MapView::horizontal)
	{
		auto defGravity = (tasnal->getType() == TasnalType::robject) ? false : true;
		tasnal->isGravityOn = ref.has(Luaf_AllowGravity) ? ref.get(Luaf_AllowGravity, defGravity) : defGravity;
		tasnal->originGravity = tasnal->isGravityOn;
	}
	else
	{
		tasnal->isGravityOn = false;
		tasnal->originGravity = false;
	}
	//Luaf_Health))
		tasnal->hp = ref.get(Luaf_Health, 0);
	//Luaf_Gravity))
	{
		tasnal->gravity = ref[Luaf_Gravity].value<Vec2>();
		tasnal->defaultGravity = ref[Luaf_Gravity].value<Vec2>();
	}
	//Luaf_MoveStep))
	{
		tasnal->moveStep = ref[Luaf_MoveStep].value<Vec2>();
		tasnal->defaultMoveStep = tasnal->moveStep;
	}
	//Luaf_JumpForce))
	{
		tasnal->defaultJumpForce = ref[Luaf_JumpForce].value<Vec2>();
		tasnal->jumpForce = ref[Luaf_JumpForce].value<Vec2>();
		tasnal->defaultJumpCutOff = tasnal->jumpForce;
		tasnal->jumpCutoff = tasnal->jumpForce;
	}
	//Luaf_InsetTile))
		tasnal->insetTile = ref[Luaf_InsetTile].value<Vec2>();
	//ע��inset���ȼ�
	//Luaf_InsetObjectX))
		tasnal->setInsetObjectX(ref[Luaf_InsetObjectX].value<float>());
	//Luaf_InsetObjectY))
		tasnal->setInsetObjectY(ref[Luaf_InsetObjectY].value<float>());
	//Luaf_InsetObject))
		tasnal->insetObject = ref[Luaf_InsetObject].value<Vec2>();
	//Luaf_OnCheckTile))
		tasnal->onCheckTile = ref.get(Luaf_OnCheckTile).toValue<bool>();
	//Luaf_InsetTileX))
		tasnal->setInsetTileX(ref[Luaf_InsetTileX].value<float>());
	//Luaf_InsetTileY))
		tasnal->setInsetTileY(ref[Luaf_InsetTileY].value<float>());
	//Luaf_InsetTile))
		tasnal->insetTile = ref[Luaf_InsetTile].value<Vec2>();

	//Luaf_IsSolid))
		tasnal->setIsSolid(ref[Luaf_IsSolid].value<bool>());
	//Luaf_Visible))
		tasnal->setVisible(ref[Luaf_Visible].value<bool>());
	//Luaf_AllowFlip))
		tasnal->allowFlip = ref[Luaf_AllowFlip].value<bool>();
	//������ע���¼�
	//Luaf_AllowTouch))
		tasnal->setAllowTouch(ref[Luaf_AllowTouch].value<bool>());

	//Luaf_AllowDragX))
		tasnal->allowDragX = ref[Luaf_AllowDragX].value<bool>();
	//Luaf_AllowDragY))
		tasnal->allowDragY = ref[Luaf_AllowDragY].value<bool>();
	//��ק
	//Luaf_DragStart))
		tasnal->dragStart = ref[Luaf_DragStart].value<Vec2>();
	//Luaf_DragEnd))
		tasnal->dragEnd = ref[Luaf_DragEnd].value<Vec2>();
	//�����Ǹ���ͼ
	//Luaf_MapTag))
		tasnal->mapTag = ref[Luaf_MapTag].value<bool>();
	//Luaf_ZOrder))
		tasnal->setLocalZOrder(ref[Luaf_ZOrder].value<int>());
	//Luaf_IsHarmful))
		tasnal->setHarmful(ref[Luaf_IsHarmful].value<bool>());
	//��ʼ������
	//Luaf_Weapon))
		tasnal->getWeapon()->registerWeapon(ref[Luaf_Weapon]);
	//Luaf_FrameSpeed))
		tasnal->setFrameSpeed(ref.get(Luaf_FrameSpeed,0.12f));
	//��ʼ������
	//Luaf_Animate))
		tasnal->setAnimation(ref[Luaf_Animate]);
	//��ɫ����
	tasnal->facedDirection = ref.get(Luaf_Face, MovingDirection::toRight);
	if (tasnal->facedDirection == MovingDirection::toLeft)
		tasnal->setFlippedX(true);
}

void Tasmina::registerTasnal(LuaIntf::LuaRef ref)
{
	assert(ref.has(Luaf_Type) && "�����ý�ɫ����");
	auto type = ref[Luaf_Type].value<TasnalType>();
	unsigned short mapTag = 1;
	if (ref.has(Luaf_MapTag))
		mapTag = ref[Luaf_MapTag].value<unsigned short>();
	Tasnal* role = nullptr;

	//1)����objectͼƬ�ļ�
	if (TasnalType::player == type)
	{
		//1)����objectͼƬ�ļ�
		if (ref.has(Luaf_File))
		{
			auto file = ref[Luaf_File].value<std::string>();
			assert(FileUtils::getInstance()->isFileExist(file) && "�ļ�������");
			role = Tasnal::createWithFileName<Player>(file);
		}

		else if (ref.has(Luaf_Frame))
		{
			auto frame = ref[Luaf_Frame].value<std::string>();
			role = Tasnal::createWithFrameName<Player>(frame);
		}
		else
		{
			auto tile = ref.has(Luaf_Tile) ? ref[Luaf_Tile].value<Vec2>() : Vec2(0, 1);
			auto tileFrame = MAP_WITHTAG(mapTag)->getFrameWithTile(tile);
			role = Tasnal::createWithFrame<Player>(tileFrame);
		}

		role->setLocalZOrder(Z_ROLE);
	}
	else if (TasnalType::npc == type)
	{
		//1)����objectͼƬ�ļ�
		if (ref.has(Luaf_File))
		{
			auto file = ref[Luaf_File].value<std::string>();
			assert(FileUtils::getInstance()->isFileExist(file) && "�ļ�������");
			role = Tasnal::createWithFileName<Npc>(file);
		}
		else if (ref.has(Luaf_Frame))
		{
			auto frame = ref[Luaf_Frame].value<std::string>();
			role = Tasnal::createWithFrameName<Npc>(frame);
		}
		else
		{
			auto tile = ref.has(Luaf_Tile) ? ref[Luaf_Tile].value<Vec2>() : Vec2(0, 1);
			auto tileFrame = MAP_WITHTAG(mapTag)->getFrameWithTile(tile);
			role = Tasnal::createWithFrame<Npc>(tileFrame);
		}

		role->setLocalZOrder(Z_Enemy);
	}
	else if (TasnalType::robject == type)
	{
		//1)����objectͼƬ�ļ�
		if (ref.has(Luaf_File))
		{
			auto file = ref[Luaf_File].value<std::string>();
			assert(FileUtils::getInstance()->isFileExist(file) && "�ļ�������");
			role = Tasnal::createWithFileName<RObject>(file);
		}
		else if (ref.has(Luaf_Frame))
		{
			auto frame = ref[Luaf_Frame].value<std::string>();
			role = Tasnal::createWithFrameName<RObject>(frame);
		}
		else
		{
			auto tile = ref.has(Luaf_Tile) ? ref[Luaf_Tile].value<Vec2>() : Vec2(0, 1);
			auto tileFrame = MAP_WITHTAG(mapTag)->getFrameWithTile(tile);
			role = Tasnal::createWithFrame<RObject>(tileFrame);
		}
		auto robject = dynamic_cast<RObject*>(role);
		//��ʼ������
		if (ref.has(Luaf_Spring))
			robject->registerSpring(ref[Luaf_Spring]);
		if (ref.has(Luaf_Knocks))
			robject->registerKnocks(ref.get(Luaf_Knocks));
		if (ref.has(Luaf_Text))
			robject->registerText(ref[Luaf_Text]);
		if (ref.has(Luaf_Char))
			robject->registerChar(ref[Luaf_Char]);
		if (ref.has(Luaf_Switch))
			robject->registerSwitch(ref[Luaf_Switch]);
		role->setLocalZOrder(Z_RObject);
	}
	else if (TasnalType::projectTile == type)
	{
		//1)����objectͼƬ�ļ�
		if (ref.has(Luaf_File))
		{
			auto file = ref[Luaf_File].value<std::string>();
			assert(FileUtils::getInstance()->isFileExist(file) && "�ļ�������");
			role = Tasnal::createWithFileName<ProjectTile>(file);
		}
		else if (ref.has(Luaf_Frame))
		{
			auto frame = ref[Luaf_Frame].value<std::string>();
			role = Tasnal::createWithFrameName<ProjectTile>(frame);
		}
		else
		{
			auto tile = ref.has(Luaf_Tile) ? ref[Luaf_Tile].value<Vec2>() : Vec2(0, 1);
			auto tileFrame = MAP_WITHTAG(mapTag)->getFrameWithTile(tile);
			role = Tasnal::createWithFrame<ProjectTile>(tileFrame);
		}
		role->setLocalZOrder(Z_ProjectTile);
	}
	role->mapTag = mapTag;
	role->getMap()->addChild(role);
	ROLE_MANAGER->registerTasnal(role);

	//���������������checkpoint����λ
	if (ref.has(Luaf_Pos))
		role->originMapPosition = ref[Luaf_Pos].value<Vec2>();
	//������Ƿ����� ,ע��offset
	if (TasnalType::projectTile != type)
		role->spawn(role->originMapPosition, ref.get(Luaf_Offset, Vec2::ZERO));

	//1.���м��ص�ʱ������ 2.����������
	setTeshnal(ref, role);
	//��ɫ��ԭʼ��ʼ��λ�ã�Ҳ�����ڴ�����
	//������tile ��ȡ�ص�������Ӱ����ʼλ��
	//setAnimation �� ����õ���ʼλ��û�����Զ���ʼ��
	role->originPosition = role->getPosition();
	//[OBB]��ʼ������С��ʼ���Ժ��ܸı�,������ײ��׼ȷ
	role->originBound = role->getCollisionBound(role->insetObject);

	//��thisָ��
	ref.set(Luaf_Tag, role->getTag());
	ref.set(Luaf_CppRef, role);

	//����Ѿ������վ�����վ��ǰ��ȫ������
	if (Checkpoint::isChecked() && ref.has(Luaf_CheckpointID))
	{
		auto checkpointID = ref[Luaf_CheckpointID].value<short>();

		if (Checkpoint::id >= checkpointID)
			ref.set(Luaf_IsDisposed, true);
	}

	LUAH->flush();
}