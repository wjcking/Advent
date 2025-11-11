
void TiledMap::setCameraY(const Vec2 & rolePos)
{
	float absoluteY = abs(getPositionY());
	float trikerTopY = absoluteY + limitedSize.y*0.8f; //��Ļ�߶ȵ�5��֮2
	float trikerBottomY = absoluteY + limitedSize.y*0.18f; //��Ļ�߶ȵ�10��֮2
	float limitedHeight = getTotalSize().height - ScreenSize.height;
	auto md = getTasnalDirection(rolePos);

	switch (md)
	{
	case MovingDirection::toTop:
	case MovingDirection::toTopLeft:
	case MovingDirection::toTopRight:
		if (rolePos.y >= trikerTopY && absoluteY <= getTotalSize().height)
			setPositionY(getPositionY() - (rolePos.y - trikerTopY));
		if (absoluteY >= limitedHeight)
			setPositionY(-limitedHeight);
		break;
	case MovingDirection::toBottom:
	case MovingDirection::toBottomLeft:
	case MovingDirection::toBottomRight:
		if (rolePos.y <= trikerBottomY && rolePos.y > getTileSize().height * 2)
			setPositionY(getPositionY() + (trikerBottomY - rolePos.y));
		//else if (getPositionY() >  getTileSize().height)
		//{
		//	setPositionY(getPositionY() + 4.f);
		//	//Y����Ϊ0�������©������ͼƬ ���� ��
		//	if (getPositionY()>0.f)
		//		setPositionY(0.f);
		//}
		break;
	case MovingDirection::stayStill:
	case MovingDirection::toLeft:
	case MovingDirection::toRight:
		//����ǵ�ͼ��ʼλ�ã����������Ƹ߶� 1/4�� ���Զ��ƶ�����ʼλ��
		if (absoluteY < limitedSize.y / 3.5f && getPositionY() < 0)
			setPositionY(getPositionY() + 4.f);
		//Y����Ϊ0�������©������ͼƬ ���� ��
		if (getPositionY() > 0.f)
			setPositionY(0.f);
		break;
	default:
		break;
	}
}
void TiledMap::setCameraSlide(const Vec2& rolePos, const CameraView& slideStyle)
{
	if (rolePos.x <= 1 || rolePos.x >= getTotalSize().width || rolePos.y <= 1 || rolePos.y >= getTotalSize().height || lastTasnalPosition == Vec2::ZERO)
	{
		lastTasnalPosition = rolePos;
		return;
	}
	if (Vec2::ZERO == per10msDistance)
		per10msDistance = Vec2(ScreenSize.width *0.014, ScreenSize.height *0.014);
	//�ƶ���Ļ��return û���ƶ������ִ������Ĳ����Է����� �ƶ�λ�ü������
	switch (moveDirection)
	{
	case MovingDirection::toLeft:

		if (per10msSteps.x <= limitedSize.x && slideStyle != CameraView::vertical)
		{
			per10msSteps.x += per10msDistance.x;
			setPositionX(getPositionX() + per10msDistance.x);
			return;
		}
		break;
	case MovingDirection::toRight:
		if (per10msSteps.x <= limitedSize.x && slideStyle != CameraView::vertical)
		{
			per10msSteps.x += per10msDistance.x;
			setPositionX(getPositionX() - per10msDistance.x);
			return;
		}
		break;

	case MovingDirection::toTop:

		if (per10msSteps.y <= limitedSize.y && slideStyle != CameraView::horizontal)
		{
			per10msSteps.y += per10msDistance.y;
			setPositionY(getPositionY() - per10msDistance.y);
			return;
		}
		break;
	case MovingDirection::toBottom:

		if (per10msSteps.y <= limitedSize.y && slideStyle != CameraView::horizontal)
		{
			per10msSteps.y += per10msDistance.y;
			setPositionY(getPositionY() + per10msDistance.y);
			return;
		}
		break;

	default:
		break;
	}

	if (rolePos.x > lastTasnalPosition.x)
	{
		float xPos = rolePos.x + getPositionX();
		if (xPos > limitedSize.x)
		{
			//setPositionX(getPositionX() - ScreenSize.width); //��ɫֱ�ӵ�λ
			moveDirection = MovingDirection::toRight;
			per10msSteps.x = 0.f;
		}
	}
	else if (rolePos.x < lastTasnalPosition.x)
	{
		float absMapPosX = abs(getPositionX());
		if (absMapPosX > rolePos.x)
		{
			//setPositionX(getPositionX() + ScreenSize.width);//��ɫֱ�ӵ�λ
			moveDirection = MovingDirection::toLeft;
			per10msSteps.x = 0.f;
		}
	}

	//�����ͼ����Ļ��
	if (getTotalSize().height > ScreenSize.height)
	{
		if (rolePos.y > lastTasnalPosition.y)
		{
			float yPos = rolePos.y + getPositionY();
			if (yPos > limitedSize.y)
			{
				//setPositionX(getPositionY() - limitedHeight);
				moveDirection = MovingDirection::toTop;
				per10msSteps.y = 0.f;
			}
		}
		else if (rolePos.y < lastTasnalPosition.y)
		{
			float absMapPosY = abs(getPositionY());

			if (absMapPosY > rolePos.y)
			{
				//setPositionX(getPositionY() +limitedHeight);
				moveDirection = MovingDirection::toBottom;
				per10msSteps.y = 0.f;
			}
		}
	}

	lastTasnalPosition = rolePos;
}

void TiledMap::setCameraCenter(Tasnal& role, const CameraView& cameraView, const bool& isAutoFocusY)
{
	const float divded = 2.f;

	if (cameraView == CameraView::horizontal || cameraView == CameraView::both)
	{
		float x = std::max(role.getPositionX(), ScreenSize.width / divded);
		x = min(x, getTotalSize().width - ScreenSize.width / divded);
		float viewPosX = ScreenSize.width / divded - x;

		//���õ�ͼ�������ƶ�
		setPositionX(viewPosX);
	}

	//���������������������
	if (cameraView == CameraView::vertical || cameraView == CameraView::both)
	{
		//�����ͼû����ô�ߣ���ֻ�����������ƶ�
		if (getTotalSize().height < ScreenSize.height * 2)
			return;
		//y�������Ž�ɫ�ƶ�
		if (isAutoFocusY)
		{
			auto y = std::max(role.getPositionY(), ScreenSize.height / divded);
			y = min(y, getTotalSize().height - ScreenSize.height / divded);
			auto viewPosY = ScreenSize.height / divded - y;
			setPositionY(viewPosY);

		}
		//���򣬶������ں��
		else
			setCameraY(role.getPosition());
	}
}
bool TiledMap::setCameraFrame(const bool& orientation)
{
	if (Vec2::ZERO == per10msDistance)
		per10msDistance = Vec2(0.25f, 0.25f);

	if (orientation)
	{
		float absoluteX = abs(getPositionX());

		if (absoluteX + limitedSize.x >= getTotalSize().width)
			return true;

		setPositionX(getPositionX() - per10msDistance.x);
		return false;
	}

	float absoluteY = abs(getPositionY());

	if (absoluteY + limitedSize.y >= getTotalSize().height)
		return true;

	setPositionY(getPositionY() - per10msDistance.y);
	return false;
}
void TiledMap::setCameraRepeat(Tasnal* role, const float& mapStep, const bool& isFocusY)
{
	assert(nullptr != secondMap && "secondMap û�г�ʼ��");
	assert(getMapSize().height == getMapSize().height && "���ŵ�ͼ�߶Ȳ�һ��");

	if (isFocusY)
	{
		setCameraY(role->getPosition());
		secondMap->setCameraY(role->getPosition());
	}

	//1�������ж���Сǰ��movestep 2����ͼ���紦��ɫ�����ֵ
	auto halfStepX = mapStep / 2;

	if (abs(lastTasnalPositionX[role->getTag()] - role->getPositionX()) <= halfStepX)
		return;

	lastTasnalPositionX[role->getTag()] = role->getPositionX();

	auto absoluteRight = role->getDirection() == MovingDirection::toRight || role->getDirection() == MovingDirection::toBottomRight || role->getDirection() == MovingDirection::toTopRight;
	auto absoluteLeft = role->getDirection() == MovingDirection::toLeft || role->getDirection() == MovingDirection::toBottomLeft || role->getDirection() == MovingDirection::toTopLeft;

	log("Right:%d Left:%d Rep:%d,NotinMap:%d", absoluteRight, absoluteLeft, inSecondMap[role->getTag()], role->getParent() == nullptr);

	//float divded = ScreenSize.width / 2.f;
	if (inSecondMap.find(role->getTag()) == inSecondMap.end())
		inSecondMap[role->getTag()] = false;
	//����ڵ�һ�ŵ�ͼ

	if (!inSecondMap[role->getTag()])
	{
		auto a1 = getChildByTag(role->getTag());
		auto a2 = secondMap->getChildByTag(role->getTag());
		auto px = role->getPositionX();
		// ������
		if (absoluteRight)
		{
			//������ ����Ѿ�����ڶ��ŵ�ͼ
			if (role->getPositionX() >= getTotalSize().width)
			{
				//���������һ�ź͵ڶ��ŵ�ͼ�Ľ��紦
				inSecondMap[role->getTag()] = true;
				auto diff = role->getPositionX() - getTotalSize().width;

				secondMap->addChild(role);
				removeChild(role, false);
				role->setPositionX(-mapStep / 2);
				//�мǼ�¼�ڶ��ŵ�ͼ����һ��X����
				//lastTasnalX = 0;
				//secondMap->lastTasnalX = role->getPositionX();

				//auto px2 = role->getPositionX();
				//auto a1 = getChildByTag(role->getTag());
				//auto a2 = secondMap->getChildByTag(role->getTag());
				//auto px = role->getPositionX();
				return;
			}
			if (role->getTag() == ID_Player)
			{
				setPositionX(getPositionX() - mapStep);
				secondMap->setPositionX(secondMap->getPositionX() - mapStep);
			}

			isStartOff = false;

		}
		else if (absoluteLeft)
		{
			//������ ����Ѿ�����ڶ��ŵ�ͼ
			if (role->getPositionX() <= 0)
			{
				auto px = role->getPositionX();
				inSecondMap[role->getTag()] = true;
				secondMap->addChild(role);
				role->setPositionX(secondMap->getTotalSize().width + mapStep / 2);
				secondMap->lastTasnalX = role->getPositionX();//�мǼ�¼�ڶ��ŵ�ͼ����һ��X����
				removeChild(role, false);
				return;
			}

			if (role->getTag() == ID_Player)
			{
				setPositionX(getPositionX() + mapStep);
				secondMap->setPositionX(secondMap->getPositionX() + mapStep);
			}

		}

	}
	else//�ڶ��ŵ�ͼ
	{
		auto px = role->getPositionX();
		auto a1 = getChildByTag(role->getTag());
		auto a2 = secondMap->getChildByTag(role->getTag());

		// ������
		if (absoluteRight)
		{
			//������ ����Ѿ������һ�ŵ�ͼ
			if (role->getPositionX() >= secondMap->getTotalSize().width)
			{

				inSecondMap[role->getTag()] = false;
				auto diff = role->getPositionX() - secondMap->getTotalSize().width;
				addChild(role);
				role->setPositionX(-mapStep / 2);
				//�мǼ�¼��һ�ŵ�ͼ����һ��X����
				//lastTasnalX = role->getPositionX();
				//secondMap->lastTasnalX = 0;
				secondMap->removeChild(role, false);
				return;
			}
			if (role->getTag() == ID_Player)
			{
				secondMap->setPositionX(secondMap->getPositionX() - mapStep);
				setPositionX(getPositionX() - mapStep);
			}
		}
		else if (absoluteLeft)
		{
			//������ �����һ�ŵ�ͼ
			if (role->getPositionX() <= 0)
			{
				inSecondMap[role->getTag()] = false;
				role->setPositionX(getTotalSize().width + mapStep / 2);
				addChild(role);
				//�мǼ�¼��һ�ŵ�ͼ����һ��X����
				lastTasnalX = role->getPositionX();
				secondMap->removeChild(role, false);
				//return;
			}
			if (role->getTag() == ID_Player)
			{
				secondMap->setPositionX(secondMap->getPositionX() + mapStep);
				setPositionX(getPositionX() + mapStep);
			}
		}

		secondMap->lastTasnalX = role->getPositionX();
	}
	/********************************
		*��ͼ���������Ļ��С������λ�û�λ
		********************************/
	if (absoluteRight)
	{
		//����һ�ŵ�ͼ�����Ҷ˺ʹ��ڵ�������غ�ʱ
		if (getPositionX() + getContentSize().width <= 0)
		{
			//����һ�ŵ�ͼ���ӵ��ڶ��ŵ�ͼ����
			//��һ�ŵ�ͼ�ĺ����� = �ڶ��ŵ�ͼ�ĺ����� + �ڶ��ŵ�ͼ�Ŀ���
			setPositionX(secondMap->getPositionX() + secondMap->getContentSize().width - mapOffsetX);
		}

		//���ڶ��ŵ�ͼ�����Ҷ˺ʹ��ڵ�������غ�ʱ
		//���ڶ��ŵ�ͼ���ӵ���һ�ŵ�ͼ����
		if (secondMap->getPositionX() + secondMap->getContentSize().width <= 0)
		{
			//���ڶ��ŵ�ͼ���ӵ���һ�ŵ�ͼ����
			//�ڶ��ŵ�ͼ�ĺ����� = ��һ�ŵ�ͼ�ĺ����� + ��һ�ŵ�ͼ�Ŀ���
			secondMap->setPositionX(getPositionX() + getContentSize().width - mapOffsetX);
		}
	}
	else if (absoluteLeft)
	{
		if (!inSecondMap[role->getTag()])
		{
			if (secondMap->getPositionX() >= ScreenSize.width)
				secondMap->setPositionX(-secondMap->getTotalSize().width + getPositionX() + mapOffsetX);
		}
		else
		{
			if (getPositionX() >= ScreenSize.width)
				setPositionX(-getTotalSize().width + secondMap->getPositionX() + mapOffsetX);
		}
	}
}
