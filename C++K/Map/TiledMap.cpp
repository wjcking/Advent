#include "TiledMap.h"
#include "MapManager.h"
#include "../Tasnal/Tasmina.h"
#include "../Tasnal/Tasnal.h"
#include "../Utils/Constant.h"
#include "../Trikers/Triker.h"
#include "../Utils/Telisk.h"
#include "../Tasnal/RObject.h"
int TiledMap::nextTag = 1;
TiledMap::TiledMap() :secondMap(nullptr), inSecondMap(false), lastTasnalX(0.f), isStartOff(true)
{
	drawCollision = DrawNode::create();
	addChild(drawCollision);
}
TiledMap*TiledMap::create(const std::string& tmxFile)
{
	assert(!tmxFile.empty() && "Invalid filename for TiledMap");

	auto map = new TiledMap();

	map->setTag(nextTag++);
	if (map && map->initWithTMXFile(tmxFile))
	{
		map->autorelease();

		map->tileProperties[0].isSlope = false;
		map->tileProperties[0].isHarmful = false;
		//tileSize = map->getTileSize();
		const char BoolTrue[5] = "true";
		for (auto tp : map->_tileProperties)
		{
			auto& valueMap = tp.second.asValueMap();

			auto ent = valueMap.find(PIsSlope);
			map->tileProperties[tp.first].gid = tp.first;
			map->tileProperties[tp.first].isSlope = ent != valueMap.end() ? (valueMap.at(PIsSlope).asString() == BoolTrue ? true : false) : false;
			ent = valueMap.find(PSlopeDegree);
			map->tileProperties[tp.first].slopeDegree = ent != valueMap.end() ? valueMap.at(PSlopeDegree).asInt() : 0;
			ent = valueMap.find(PSlopeTowards);
			map->tileProperties[tp.first].slopeTowards = ent != valueMap.end() ? valueMap.at(PSlopeTowards).asInt() : SlopeTowards::faceNothing;
			ent = valueMap.find(PSlopeGravity);
			map->tileProperties[tp.first].slopeGravity = ent != valueMap.end() ? valueMap.at(PSlopeGravity).asFloat() : -3.5f;
			ent = valueMap.find(PTasnalOffsetY);
			map->tileProperties[tp.first].roleOffsetY = ent != valueMap.end() ? valueMap.at(PTasnalOffsetY).asFloat() : 0.f;
			ent = valueMap.find(PAllowThrough);
			map->tileProperties[tp.first].allowThrough = ent != valueMap.end() ? (valueMap.at(PAllowThrough).asString() == BoolTrue ? true : false) : false;
			ent = valueMap.find(PInitSlopeY);
			map->tileProperties[tp.first].initSlopeY = ent != valueMap.end() ? (valueMap.at(PInitSlopeY).asString() == BoolTrue ? true : false) : false;
			ent = valueMap.find(PIsOneWay);
			map->tileProperties[tp.first].isOneWay = ent != valueMap.end() ? (valueMap.at(PIsOneWay).asString() == BoolTrue ? true : false) : false;
			ent = valueMap.find(PIsHarmful);
			map->tileProperties[tp.first].isHarmful = ent != valueMap.end() ? (valueMap.at(PIsHarmful).asString() == BoolTrue ? true : false) : false;
			//	auto a = ent != valueMap.end() ? (valueMap.at(PIsHarmful).asString() == BoolTrue ? true : false) : false;
				//inset
			ent = valueMap.find(PInsetX);
			map->tileProperties[tp.first].inset.origin.x = ent != valueMap.end() ? valueMap.at(PInsetX).asFloat() : 0.0f;
			ent = valueMap.find(PInsetY);
			map->tileProperties[tp.first].inset.origin.y = ent != valueMap.end() ? valueMap.at(PInsetY).asFloat() : 0.0f;
			ent = valueMap.find(PInsetWidth);
			map->tileProperties[tp.first].inset.size.width = ent != valueMap.end() ? valueMap.at(PInsetWidth).asFloat() : 0.0f;
			ent = valueMap.find(PInsetHeight);
			map->tileProperties[tp.first].inset.size.height = ent != valueMap.end() ? valueMap.at(PInsetHeight).asFloat() : 0.0f;
		}

		return map;
	}
	CC_SAFE_DELETE(map);
	return nullptr;
}

void TiledMap::createRepeat(const std::string & tmxFile, const bool&)
{
	secondMap = TiledMap::create(tmxFile); //���ڶ��ŵ�ͼ��λ�����õ��ڶ��ŵ�ͼ�ĺ���
	secondMap->setPositionX(getPositionX() + getContentSize().width - mapOffsetX);
}


TiledMap::TiledMap(const string& tmxFile)
{

}

TiledMap::~TiledMap()
{
	trikerSystem.clear();
}

TileTeshnalInfo&  TiledMap::getTeshnal(const int& gid)
{
	auto propertyValue = tileProperties.find(gid);

	if (propertyValue != tileProperties.end())
		return propertyValue->second;

	return  tileProperties.at(0);
}
//һ��Ҫ������Tasnal��Ϻ����ִ�д˷���������tag�Ḳ��ԭ�е�roleԪ��
void registerKnocks(const LuaRef&);
void processTileRange(const Vec2&, const Vec2&, function<void(const Vec2&, const short&)>);
inline void TiledMap::removeObjectRange(const Vec2& s, const Vec2& e)
{
	processTileRange(s, e, [&](const Vec2& rangeTile, const short& i) {
		getWalls("object").removeTileAt(rangeTile);
	});
}
inline void TiledMap::removeLayerRange(const std::string& name, const Vec2& s, const Vec2& e)
{
	processTileRange(s, e, [&](const Vec2& rangeTile, const short& i) {
		getWalls(name).removeTileAt(rangeTile);
	});
}
BoundRact TiledMap::getCenterTile(const Vec2& rolePosition)
{
	auto layer = getLayer(LayerWalls);
	assert(layer != nullptr && "ͼ�㲻��Ϊ��");

	if (rolePosition.y <= 0 || rolePosition.y < getTileSize().height || rolePosition.x > getTotalSize().width || rolePosition.y > getTotalSize().height)
		return BoundRact(0, Vec2::ZERO, Ract::ZERO);
	//  ���ش˴���tile����
	Vec2 tilePosition = this->getTileCoordinateByPosition(rolePosition);
	//���Ĳ�λ�Լ�

	auto gid = getGidAt(tilePosition);
	auto tileRact = getRactByTileCoordinate(tilePosition, gid);
	auto tileTeshnal = getTeshnal(gid);

	auto boundRact = BoundRact(gid, tilePosition, tileRact);
	boundRact.setTileTeshnal(tileTeshnal);

	return boundRact;

}

Vec2 TiledMap::getTileCoordinateByPosition(Vec2 position)
{
	float x = floor(position.x / getTileSize().width); //λ��xֵ/��ͼһ��tile�Ŀ��ȼ��ɵõ�x����
	float levelHeightInPixels = getMapSize().height * getTileSize().height; //��ͼ��ʵ�ʸ߶�
	float y = floor((levelHeightInPixels - position.y) / getTileSize().height);  //��ͼ��ԭ�������Ͻǣ���cocos2d-x�ǲ�ͬ��(2dxԭ�������½�)
	return Vec2(x, y);
}

Ract TiledMap::getRactByTileCoordinate(Vec2 tileCoords, const int& gid)
{
	float levelHeightInPixels = getMapSize().height * getTileSize().height; //��ͼ��ʵ�ʸ߶�
	//�ѵ�ͼ����tileCoordsת��Ϊʵ����Ϸ�е�����
	auto origin = Vec2(tileCoords.x *  getTileSize().width, levelHeightInPixels - ((tileCoords.y + 1)* getTileSize().height));
	if (0 == gid)
		return Ract(origin.x, origin.y, getTileSize().width, getTileSize().height);

	auto inset = tileProperties[gid].inset;

	return Ract(origin.x - inset.origin.x, origin.y - inset.origin.y, getTileSize().width - inset.size.width, getTileSize().height - inset.size.height);
}
TMXLayer& TiledMap::getWalls(const std::string& layerName) const
{
	auto layer = getLayer(layerName);
	assert(layer != nullptr && "û�иò�");
	return *layer;
}

void TiledMap::setGidRange(const int & gid, const Vec2& rangeStart, const Vec2& rangeEnd)
{
	processTileRange(rangeStart, rangeEnd, [&](const Vec2& rangeTile, const short& i) {
		getWalls().setTileGID(gid, rangeTile);
	});
}

void TiledMap::setLayerGid(const std::string & layerName, const int & gid, const Vec2 & rangeStart, const Vec2 &rangeEnd)
{
	processTileRange(rangeStart, rangeEnd, [&](const Vec2& rangeTile, const short& i) {
		getWalls(layerName).setTileGID(gid, rangeTile);
	});
}

//�߽��жϷ���ios�˳�����
uint32_t TiledMap::getGidAt(Vec2  tilePos, const std::string& layerName)
{
	//	CCASSERT(pos.x < _layerSize.width && pos.y < _layerSize.height && pos.x >= 0 && pos.y >= 0, "TMXLayer: invalid position");
	if (tilePos.x > getWalls(layerName).getLayerSize().width - 1)
		tilePos.x = getWalls(layerName).getLayerSize().width - 1;
	if (tilePos.y > getWalls(layerName).getLayerSize().height - 1)
		tilePos.y = getWalls(layerName).getLayerSize().height - 1;

	if (tilePos.x <= 0)
		tilePos.x = 0;

	if (tilePos.y <= 0)
		tilePos.y = 0;

	return getWalls(layerName).getTileGIDAt(tilePos);
}

void TiledMap::removeTileRange(const Vec2 &rangeStart, const Vec2 &rangeEnd)
{
	processTileRange(rangeStart, rangeEnd, [&](const Vec2& rangeTile, const short& i) {
		getWalls().removeTileAt(rangeTile);
	});
}

MovingDirection& TiledMap::getTasnalDirection(const Vec2 & rolePos)
{
	//�ŵ�����ǰ����������ƶ���λ�� ���������� 
	if (rolePos.x != lastTasnalPosition.x && rolePos.y == lastTasnalPosition.y)
		moveDirection = rolePos.x > lastTasnalPosition.x ? MovingDirection::toRight : MovingDirection::toLeft;

	if (rolePos.x == lastTasnalPosition.x && rolePos.y != lastTasnalPosition.y)
		moveDirection = rolePos.y < lastTasnalPosition.y ? MovingDirection::toBottom : MovingDirection::toTop;

	if (rolePos.x > lastTasnalPosition.x && rolePos.y > lastTasnalPosition.y)
		moveDirection = MovingDirection::toTopRight;

	if (rolePos.x < lastTasnalPosition.x && rolePos.y < lastTasnalPosition.y)
		moveDirection = MovingDirection::toBottomLeft;

	if (rolePos.x > lastTasnalPosition.x && rolePos.y < lastTasnalPosition.y)
		moveDirection = MovingDirection::toBottomRight;

	if (rolePos.x < lastTasnalPosition.x && rolePos.y > lastTasnalPosition.y)
		moveDirection = MovingDirection::toTopLeft;

	if (lastTasnalPosition == Vec2::ZERO || (rolePos.x == lastTasnalPosition.x && rolePos.y == lastTasnalPosition.y))
		moveDirection = MovingDirection::stayStill;

	lastTasnalPosition = rolePos;
	return moveDirection;
}

bool TiledMap::moveCameraX(const float & step, const float & distance)
{
	if (Vec2::ZERO == originPosition)
		originPosition = getPosition();

	if (abs(abs(getPositionX()) - abs(originPosition.x)) >= distance)
		return true;
	//��ͷ��
	if (abs(getPositionX()) >= (getTotalSize().width - ScreenSize.width))
		return true;
	setPositionX(getPositionX() - step);
	return false;
}

/*ƽ̨�ƶ�����*/
Vec2 TiledMap::getPositionByTileCoordinate(const Point &tileCoord)   const
{
	float factor = CC_CONTENT_SCALE_FACTOR();
	int x = (tileCoord.x * getTileSize().width) + getTileSize().width / 2;
	int y = (getMapSize().height * getTileSize().height) - (tileCoord.y * getTileSize().height) - getTileSize().height / 2;
	return Point(x / factor, y / factor);
}

Vec2 TiledMap::getPositionByTileCoordinate2(const Point & tileCoord) const
{
	float factor = CC_CONTENT_SCALE_FACTOR();
	int x = (tileCoord.x * getTileSize().width);
	int y = (getMapSize().height * getTileSize().height) - (tileCoord.y * getTileSize().height) - getTileSize().height;
	return Vec2(x / factor, y / factor);
}

void TiledMap::loadScript()
{
	Triker* triker;
	TrikerSystem::trikerNext = 1;
	unsigned short type = 0;
	LUAH->processTable(Luat_Trikers, [&](LuaIntf::LuaRef ref) {

		type = ref.get(Luaf_Type, 0);

		if (type == 0)
			return;

		switch (type)
		{
		case 1:
			CCASSERT(ref.has(Luaf_Ract), "������rect");
			triker = new TrikerRact(ref.get(Luaf_Ract, Ract(200, 200, 33, 44)));
			break;
		case 2:
			CCASSERT(ref.has(Luaf_RangeStart), "������RangeStart");
			CCASSERT(ref.has(Luaf_RangeStart), "������RangeStart");
			triker = new TrikerLine(ref.get(Luaf_RangeStart, Vec2(100, 100)), ref.get(Luaf_RangeEnd, Vec2(100, 200)));
			break;
		case 3:
			CCASSERT(ref.has(Luaf_Ract), "������rect");
			triker = new TrikerCircle(ref.get(Luaf_Ract, Ract(200, 200, 33, 44)));
			break;
		}

		if (ref.has(Luaf_IsActive))
			triker->setActivation(ref.get(Luaf_IsActive, true));

		if (ref.has(Luaf_Name))
			triker->setName(ref.get(Luaf_Name).toValue<std::string>());
		else
			triker->setName("triker");
		trikerSystem.addTriker(triker);
		//��thisָ��
		auto tag = triker->getTag();
		ref.set(Luaf_Tag, triker->getTag());
		ref.set(Luaf_CppRef, triker);
		LUAH->flush();
	}, false);
}

void TiledMap::collapse(LuaRef ref)
{
	//���lua����һ�ε�collapse���ڣ�������ú����ʹ�÷�������rangeԽ��,�����������ֵ��������
	auto rangeStart = ref.get(Luaf_RangeStart, Vec2::ZERO);
	auto rangeEnd = ref.get(Luaf_RangeEnd, Vec2::ZERO);
	//auto step = ref.get(Luaf_MoveStep, Vec2::ZERO);
	//auto direction = ref.get(Luaf_Direction, MovingDirection::toBottom);
	//auto range = ref.get(Luaf_Range, Vec2(ScreenSize.width, ScreenSize.height));

	RObject* collisionTile = nullptr;

	Action Color process = BoundMap::TileRange(&rangeStart, &rangeEnd)
	{
		auto tile = getWalls().getTileAt(rangeTile);
		if (nullptr == tile)
		{
			log("[Collapse]tile Collapse incorrect settings or no such tile coordinates");
			return;
		}

		//bool isOutOfRange = rangeTile.x >= getMapSize().width || rangeTile.y >= getMapSize().height;
		//auto mapSize = getMapSize();
		bool isInRange = rangeTile.x < getMapSize().width && rangeTile.y < getMapSize().height;
			
		log("[TiledMap::collapse]out of range");
		if (!isInRange)	return;
		auto tileFrame = tile->getSpriteFrame();
			
		log("[TiledMap::collapse]mapTag tile frame");
		if (nullptr == tileFrame)	return;

		collisionTile = Tasnal::createWithFrame<RObject>(tileFrame);;
		//collisionTile->setName(StringUtils::format("collapse%d", collisionTile->getTag()));
		collisionTile->setPosition(getPositionByTileCoordinate(rangeTile));
		collisionTile->setOrigin(collisionTile->getPosition());
		//�Ƴ���ǰtiledmap�е�sprite
		getWalls().removeTileAt(rangeTile);
		//���ݵ�ͼid����
		ROLE_MANAGER->registerTasnal(collisionTile);
		addChild(collisionTile);

		ref.set(Luaf_CppRef, collisionTile);
		//ע�ᵽ�ű�
		//[&](const Vec2& rangeTile, const short& i)
	});

}

void TiledMap::update(Tasnal& role)
{
	if (trikerSystem.getCount() == 0)
		return;
}

//һ��Ҫ��stage������Tasnal��Ϻ����ִ�д˷���������tag�Ḳ��ԭ�е�roleԪ���޷�ִ��
void TiledMap::registerKnocks(const LuaRef& ref)
{
	auto rangeStart = ref.get(Luaf_RangeStart, Vec2::ZERO);
	auto rangeEnd = ref.get(Luaf_RangeEnd, Vec2::ZERO);

	if (rangeStart == Vec2::ZERO)
		return;
	if (!ref.has(Luaf_Knocks))
	{
		log("[TiledMap::registerKnocks]please set value field of knocks actions");
		return;
	}
	RObject* knockTile = nullptr;
	processTileRange(rangeStart, rangeEnd, [&](const Vec2& rangeTile, const short& i) {
		auto tile = getWalls().getTileAt(rangeTile);
		if (nullptr == tile)
		{
			log("[Collapse]tile Collapse incorrect settings or no such tile coordinates");
			return;
		}

		//bool isOutOfRange = rangeTile.x >= getMapSize().width || rangeTile.y >= getMapSize().height;
		//auto mapSize = getMapSize();
		bool isInRange = rangeTile.x < getMapSize().width && rangeTile.y < getMapSize().height;

		if (!isInRange)
		{
			log("[TiledMap::registerKnocks]out of range");
			return;
		}
		auto tileFrame = tile->getSpriteFrame();

		if (nullptr == tileFrame)
		{
			log("[TiledMap::registerKnocks]mapTag tile frame null");
			return;
		}
		knockTile = Tasnal::createWithFrame<RObject>(tileFrame);;
		knockTile->setName(StringUtils::format("knock%d", knockTile->getTag()));
		knockTile->registerKnocks(ref.get(Luaf_Knocks));
		knockTile->setPosition(getPositionByTileCoordinate(rangeTile));
		//������ʵoriginmap ��λcorruptλ��
		knockTile->setOriginMap(rangeTile);

		//�Ƴ���ǰtiledmap�е�sprite
		getWalls().removeTileAt(rangeTile);
		//���ݵ�ͼid����
		addChild(knockTile);
		ROLE_MANAGER->registerTasnal(knockTile);
		//ע�ᵽ�ű�
		LUAH->registerTasnal("map", "knock", knockTile);
	});
}

SpriteFrame * TiledMap::getFrameWithTile(const Vec2& pos)
{
	auto tilePos = Vec2(getTileSize().width * pos.x, getTileSize().height * pos.y);
	auto tileFrame = SpriteFrame::createWithTexture(getWalls().getTexture(), Ract(tilePos, getTileSize()));

	return tileFrame;
}

//tiledmap editor ������ϵ���ѡ�񣬶����Ǵ��µ���
void TiledMap::processTileRange(const Vec2& rangeStart, const Vec2& rangeEnd, std::function<void(const Vec2& range, const short& iter)> each)
{
	CCASSERT(tileRangeCount > 0, "tiledmap editor ������ϵ���ѡ�񣬶����Ǵ��µ���");
	CCASSERT(rangeEnd.x >= rangeStart.x, "tiledmap editor ������ϵ���ѡ�񣬶����Ǵ��µ���");
	CCASSERT(rangeEnd.y >= rangeStart.y, "tiledmap editor ������ϵ���ѡ�񣬶����Ǵ��µ���");

	auto xTileCount = rangeEnd.x - rangeStart.x + 1;
	auto yTileCount = rangeEnd.y - rangeStart.y + 1;

	unsigned short i = 0;
	unsigned short x = 0;
	unsigned short y = 0;

	auto tileRangeCount = xTileCount * yTileCount;
	while (i < tileRangeCount)//��Ƭ��������
	{
		if (i > 0 && i % static_cast<short>(xTileCount) == 0) //ÿ�� y++,x����
		{
			y++;
			x = 0;
		}

		each(Vec2(rangeStart.x + x++, rangeStart.y + y), i);
		i++;
	}
}
