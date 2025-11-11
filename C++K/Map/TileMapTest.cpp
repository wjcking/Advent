TMXOrthoTest2::TMXOrthoTest2()
{
    auto map = TMXTiledMap::create("TileMaps/orthogonal-test1.tmx");
    addChild(map, 0, kTagTileMap);

    Size CC_UNUSED s = map->getContentSize();
    CCLOG("ContentSize: %f, %f", s.width,s.height);

    auto& children = map->getChildren();
    SpriteBatchNode* child = nullptr;

    for(const auto &obj : children) {
        child = static_cast<SpriteBatchNode*>(obj);
        child->getTexture()->setAntiAliasTexParameters();
    }
    map->runAction( ScaleBy::create(2, 0.5f) ) ;
}

bool TiledMap::exchangeMap(Tasnal* role)
{
    ////����ڵ�һ�ŵ�ͼ
    ////if (!inSecondMap)
    ////{
    //	//������ ����Ѿ�����ڶ��ŵ�ͼ
    //	if (role->getPositionX() >= getTotalSize().width)
    //	{
    //		auto px = role->getPositionX();
    //		//���������һ�ź͵ڶ��ŵ�ͼ�Ľ��紦
    //		inSecondMap = true;
    //		auto diff = role->getPositionX() - getTotalSize().width;
    //		secondMap->addChild(role);
    //		role->setPositionX(-role->getMoveStep().x / 2);
    //		//�мǼ�¼�ڶ��ŵ�ͼ����һ��X����
    //		lastTasnalX = 0;
    //		secondMap->lastTasnalX = role->getPositionX();
    //		removeChild(role, false);
    //		return true;
    //	}
    ////}
    ////else
    ////{
    //	//������ ����Ѿ������һ�ŵ�ͼ
    //	if (role->getPositionX() >= secondMap->getTotalSize().width)
    //	{
    //		inSecondMap = false;
    //		auto diff = role->getPositionX() - secondMap->getTotalSize().width;
    //		addChild(role);
    //		//�мǼ�¼��һ�ŵ�ͼ����һ��X����
    //		lastTasnalX = role->getPositionX();
    //		secondMap->lastTasnalX = 0;
    //		role->setPositionX(-role->getMoveStep().x / 2);
    //		secondMap->removeChild(role, false);
    //		return true;
    //	}
    ////}
    return false;
}