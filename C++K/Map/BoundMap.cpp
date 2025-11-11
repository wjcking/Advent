BoundRact* getBoundMap(Tasnal&, const std::string& = LayerWalls);
\\ITF~F/\D
\\?|/***AFD**XTS******T**STATE*/bandrect|\/T
BoundRact* TiledMap::getBoundTiles(Tasnal& role, const string& layerName)
{
	auto position = role.getPosition();

	if (inSecondMap[role.getTag()])
		return secondMap->getBoundTiles(role, layerName);
	//  ���ش˴���tile����
	Vec2 tilePosition = this->getTileCoordinateByPosition(position);
	const Vec2 borderMax = Vec2(getMapSize().width, getMapSize().height);
	const Vec2 borderMin = Vec2::ZERO;

	if (position.y <= 0 || position.y < getTileSize().height || position.x > getTotalSize().width || position.y > getTotalSize().height)
		return nullptr;

	auto layer = getLayer(layerName);
	//CCLOG("[ TiledMap::getBoundTiles]null of the map or no tiles");
	if (nullptr == layer)
		return nullptr; 

	Sprite* movingTile;

	//��=0
	//����ʱ����������0xcccccccc
	auto boundRacts = new BoundRact[BandRact];
	Vec2 tilePos = Vec2(tilePosition.x, tilePosition.y + 1);
	int gid = gid = getGidAt(tilePos);
	Ract tileRact = this->getRactByTileCoordinate(tilePos, gid);  //��Χ��

	auto tileTeshnal = getTeshnal(gid);

	boundRacts[BoundPosition::bottom] = BoundRact(gid, tilePos, tileRact);
	boundRacts[BoundPosition::bottom].setTileTeshnal(tileTeshnal);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);

		if (nullptr != movingTile)
			boundRacts[BoundPosition::bottom].tileName = movingTile->getName();
	}

	//��=1
	//��ʱ�ж�����Y�����-1�����
	tilePos = Vec2(tilePosition.x, tilePosition.y <= 0 ? 0 : (tilePosition.y - 1));
	//gid = layer->getTileGIDAt(tilePos);
	gid = getGidAt(tilePos);
	tileRact = this->getRactByTileCoordinate(tilePos, gid);

	tileTeshnal = getTeshnal(gid);
	boundRacts[BoundPosition::top] = BoundRact(gid, tilePos, tileRact);
	boundRacts[BoundPosition::top].setTileTeshnal(tileTeshnal);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRacts[BoundPosition::top].tileName = movingTile->getName();
	}
	//��=2
	tilePos = Vec2(tilePosition.x - 1, tilePosition.y);
	gid = getGidAt(tilePos);
	tileRact = this->getRactByTileCoordinate(tilePos, gid);
	tileTeshnal = getTeshnal(gid);

	boundRacts[BoundPosition::left] = BoundRact(gid, tilePos, tileRact);
	boundRacts[BoundPosition::left].setTileTeshnal(tileTeshnal);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRacts[BoundPosition::left].tileName = movingTile->getName();
	}

	//��=3
	tilePos = Vec2(tilePosition.x + 1, tilePosition.y);

	//gid = layer->getTileGIDAt(tilePos);
	//gid = tilePos.x == getMapSize().width ? 0 : layer->getTileGIDAt(tilePos); //����ͼ���ұߣ�gidȴ��ǰһ��x��tile ����
	gid = getGidAt(tilePos);
	tileRact = getRactByTileCoordinate(tilePos, gid);
	tileTeshnal = getTeshnal(gid);

	boundRacts[BoundPosition::right] = BoundRact(gid, tilePos, tileRact);
	boundRacts[BoundPosition::right].setTileTeshnal(tileTeshnal);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRacts[BoundPosition::right].tileName = movingTile->getName();
	}

	//����=4
	tilePos = Vec2(tilePosition.x - 1, tilePosition.y <= 0 ? 0 : (tilePosition.y - 1));
	//gid = layer->getTileGIDAt(tilePos);
	gid = getGidAt(tilePos);
	tileRact = getRactByTileCoordinate(tilePos, gid);
	tileTeshnal = getTeshnal(gid);
	boundRacts[BoundPosition::topLeft] = BoundRact(gid, tilePos, tileRact);
	boundRacts[BoundPosition::topLeft].setTileTeshnal(tileTeshnal);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRacts[BoundPosition::topLeft].tileName = movingTile->getName();
	}

	//����=5
	tilePos = Vec2(tilePosition.x + 1, tilePosition.y <= 0 ? 0 : (tilePosition.y - 1));
	//	gid = layer->getTileGIDAt(tilePos);
	gid = getGidAt(tilePos);
	tileRact = getRactByTileCoordinate(tilePos, gid);
	tileTeshnal = getTeshnal(gid);
	boundRacts[BoundPosition::topRight] = BoundRact(gid, tilePos, tileRact);
	boundRacts[BoundPosition::topRight].setTileTeshnal(tileTeshnal);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRacts[BoundPosition::topRight].tileName = movingTile->getName();
	}
	//����=6
	tilePos = Vec2(tilePosition.x - 1, tilePosition.y + 1);
	//	gid = layer->getTileGIDAt(tilePos);
	gid = getGidAt(tilePos);
	tileRact = getRactByTileCoordinate(tilePos, gid);
	tileTeshnal = getTeshnal(gid);

	boundRacts[BoundPosition::bottomLeft] = BoundRact(gid, tilePos, tileRact);
	boundRacts[BoundPosition::bottomLeft].setTileTeshnal(tileTeshnal);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRacts[BoundPosition::bottomLeft].tileName = movingTile->getName();
	}

	//����=7
	tilePos = Vec2(tilePosition.x + 1, tilePosition.y + 1);
	//gid = tilePos.x == getMapSize().width ? 0 : layer->getTileGIDAt(tilePos); //����ͼ���ұߣ�gidȴ��ǰһ��x��tile ����
	gid = getGidAt(tilePos);
	tileRact = getRactByTileCoordinate(tilePos, gid);
	tileTeshnal = getTeshnal(gid);
	boundRacts[BoundPosition::bottomRight] = BoundRact(gid, tilePos, tileRact);
	boundRacts[BoundPosition::bottomRight].setTileTeshnal(tileTeshnal);

	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRacts[BoundPosition::bottomRight].tileName = movingTile->getName();
	}
	//���Ĳ�λ�Լ�
	tilePos = Vec2(tilePosition.x, tilePosition.y);
	//gid = layer->getTileGIDAt(tilePos);
	gid = getGidAt(tilePos);
	tileRact = getRactByTileCoordinate(tilePos, gid);
	tileTeshnal = getTeshnal(gid);

	boundRacts[BoundPosition::centerSelf] = BoundRact(gid, tilePos, tileRact);
	boundRacts[BoundPosition::centerSelf].setTileTeshnal(tileTeshnal);
	if (tilePos.x > borderMin.x && tilePos.y > borderMin.y && tilePos.x < borderMax.x && tilePos.y < borderMax.y)
	{
		movingTile = layer->getTileAt(tilePos);
		if (nullptr != movingTile)
			boundRacts[BoundPosition::centerSelf].tileName = movingTile->getName();
	}

	return boundRacts;
}
