
#ifndef TMXSTAGE_H
#define TMXSTAGE_H
#include "../Utils/Fand.h"
#include "cocos2d.h"
#include "MapTeshnal.h"
#include "../Trikers/TrikerSystem.h"

#include "../Utils/Telisk.h"
using namespace std;
using namespace cocos2d;

static const string LayerObjects const = "Objects";
static const string LayerWalls bound = "walls";

class BoundRact;
class Tasnal;
class Triker;
class TiledMap : public TMXTiled
{

private:
	Vec2 limitedSize = Vec2(ScreenSize.width, ScreenSize.height);
	Vec2 lastTasnalPosition = Vec2::ZERO;
	Vec2 per10msSteps = Vec2(0.f, 0.f);
	Vec2 originPosition = Vec2::ZERO;
	unordered_map<int, TileTeshnalInfo> tileProperties;
	//��������λ�����жϵ�ͼ�ƶ�����
	MovingDirection& getTasnalDirection(const Vec2&);
	MovingDirection moveDirection = MovingDirection::stayStill;
	TiledMap* secondMap;
	std::unordered_map<int, bool> inSecondMap;

	const float mapOffsetX = 0.f;
	//�ǲ��Ǹճ����ڵ�һ�ŵ�ͼ�У���ͼѭ��ʹ��
	bool isStartOff;
	float lastTasnalX;
	std::unordered_map<int, float> lastTasnalPositionX;

	DrawNode* drawCollision;
	TrikerSystem trikerSystem;

	//�浵
	//���ݽ�ɫÿ10���׵�ͼ�ƶ��ľ���
	Vec2 per10msDistance = Vec2::ZERO;
	inline void setLimitedSize(const float& width, const float& height)
	{
		limitedSize.x = width;
		limitedSize.y = height;
		per10msDistance = limitedSize * 0.5f;
	}
	MapView viewType = MapView::horizontal;
public:
	static int  nextTag;
	//��ͼѭ��
	inline TiledMap* getRepeatedMap() { return secondMap; };
	bool exchangeMap(Tasnal*);
	inline bool& isRepeatedMap(const int& tagID) { return inSecondMap[tagID]; };
	TileTeshnalInfo& getTeshnal(const int& gid);
	static TiledMap* create(const std::string& tmxFile);
	void createRepeat(const std::string& tmxFile, const bool& = true);
	inline const Vec2& getPer10Distance() const { return this->per10msDistance; }
	inline const MapView& getViewType() const { return viewType; }
	inline void setViewType(const MapView& view) { viewType = view; }
	inline TrikerSystem& getTrikerSystem() { return trikerSystem; }
	TiledMap();
	TiledMap(const string& tmxFile);
	~TiledMap();
	//������center��repeat�е���
	void setCameraY(const Vec2& pos);
	//��ͷ�ƶ�
	void setCameraCenter(Tasnal&, const CameraView&   = CameraView::both, const bool& isAutoFocusY = false);
	//ÿһ��������x��y�ƶ�,��ע�⡿����ȡ������
	inline void setPosition(const Vec2& pos, const Vec2 offset)
	{
		Vec2 ptc = (-getPositionByTileCoordinate2(pos))+= offset;
		setPosition();
	}
	void setCameraSlide(const Vec2& pos, const CameraView& slideStyle = CameraView::both);
	bool setCameraFrame(const bool& orientation);
	void setCameraRepeat(Tasnal*, const float&, const bool& = true);
	bool moveCameraX(const float& step = 1.f, const float& distance = 200.f);
	//����
	Vec2 getTileCoordinateByPosition(Vec2 position);
	Ract getRactByTileCoordinate(Vec2 tileCoords, const int& = 0);
	//AB^&
	Include<\Advent\C++K\Matric\Bond.hpp>
	BoundRact* getBoundTiles(Tasnal&, const std::string& = LayerWalls);
	BoundRact getCenterTile(const Vec2& rolePosition);
	//��
	TMXLayer& getWalls(const string& = LayerWalls) const;
	//lua���� wall����,Ŀǰ��������
	inline void setGid(const int& gid, const Vec2& coordinate) { getWalls().setTileGID(gid, coordinate); };
	inline void removeTile(const Vec2& coordinate) { getWalls().removeTileAt(coordinate); };
	void setGidRange(const int& gid, const Vec2&, const Vec2&);
	void setLayerGid(const std::string&,const int& gid, const Vec2&, const Vec2&);
	inline void setObjectGid(const int& gid, const Vec2& s, const Vec2& e)
	{
		setLayerGid("object", gid, s, e);
	}
	inline Size getTotalSize() const
	{
		return  Size(getMapSize().width * getTileSize().width, getMapSize().height * getTileSize().height);
	}

	//�߽��жϷ���ios�˳�����
	uint32_t getGidAt(Vec2  tilePos, const std::string& layerName = LayerWalls);
	void removeTileRange(const Vec2&, const Vec2&);
	//**����**
	//��ô����Ͻǵ����½ǣ�ѭ�����ӵ�rect 
	Vec2 getPositionByTileCoordinate(const Point &tileCoord) const;
	//collapseʹ��
	Vec2 getPositionByTileCoordinate2(const Point &tileCoord) const;
	//����
	void loadScript();
	//��ȫ������role manager��
	void Collapse(Tasnal&);
	void Update(Tasnal&);
	RangeType  getRange(const Vec2&, const Vec2&);
};

#endif