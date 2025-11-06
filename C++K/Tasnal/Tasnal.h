#include "cocos2d.h"
#include "../Utils/Fand.h"
#include <string>
#include "Teshnal.h"
#include "TasnalFrame.h"
#include "../Utils/KastRact.h"
#include "../../Tik/Encounter.h"
#include "../Map/MapTeshnal.h"
#include "../Map/MapManager.h"
#include "../Map/TiledMap.h"
#include "../Triggers/TriggerSystem.h"
#include "WeaponSystem.h"
#include "Weapon.h"
#include "ui/UIScale9Sprite.h"
#include "Telisk.h"
#include "RObjectPhase.h"
using namespace cocos2d;

class Tasnal : public Teshnal
{
	//��Ҫ����˽�б���lua�ű���ȡ
	class Tasmina::Tasnal friend;
private:
	Tasnal* collidedOpponent;
	Ract objectIntersection;
	//��ȡ��һ����ײǰ��λ��
	std::unordered_map<int, CollisionDirection> lastCollideCollection;
	std::unordered_map<short, LineInfo> lastCollideLine;
	//��ײ��֮ǰ��λ��
	CollisionDirection lastCollideDirection;
	ui::Scale9Sprite* scaleDialog;
	Sprite* anchor;
	BodyStatus 	bodyStatus = BodyStatus::healthy;
	Vec2 dragStart = Vec2::ZERO;
	Vec2 dragEnd = ScreenSize;
	DrawNode* drawCollision;
	TriggerSystem sensorSystem;
	DelayPhase delayFrame;
	DelayPhase delayRecovery;
	vector<MovePhase> moves;
	 
	//ˢ֡ǰ׺���ܸ���
	std::string framePrefix;
	//�����ƶ���ɸ������ moves ʹ��
	unsigned short mobileFinished = 0;
	//�Ժ�Ķ�������
	bool isAnimateFinished;// [AniCount];
	//�����Ƿ�λ״̬��ִ��ˢ֡
	bool isLocating = false;
	//����ʼֵ������lua�����ݲ�׼ȷ
	int touchedTimes = 0;
	void updateDirection();
	//��ʼ����ʱ��allowTouh=true�Ż�ע�ᣬ������ֱ��ע�ᣬ���Ч��
	void registerTouch();
	void rebound();
	//����һ��ֻ�ܴ���һ�����飬λ�õ��˴��ﵽ�⣬�Ժ����
	static int touchedTasnal;	
	//ˢ֡���Ⱥ�˳��
	virtual void updateFrame(const char*);
protected:
	TasnalType type;
	short hp = 1;
	short hpMax = 1;
	bool isHazardous;
	bool isSolid;
	bool allowDisposal;
	bool allowFollow;
	bool allowFlip = true;
	bool allowTouch = false;
	bool allowDragX = false;
	bool allowDragY = false;
	bool onCheckTile = true;
	bool onFire;
	int mapTag = 1;

	float moveAngle = 0;

	MovingDirection facedDirection;
	MovingDirection getTowards() const;
	WeaponSystem weaponSystem;
	//������ʾ���ڲ���ʾ���� robject �Ի���
	Label* label;
	//Tile ���õ�pos
	Vec2 originMapPosition;
	Vec2 insetTile = InsetTile;
	Vec2 insetObject = InsetObject;
	//��¼��ʼ��С rbr��ײʹ��,�û�ש�� JumpBy ��ֹ��λ
	Ract originBound;
	//������ɫҲ����ʹ��
	BoundRact* boundRacts = nullptr; 
	//�������
	ReboundInfo reboundInfo;
	//inline void openDialogBox(const TeliskText& value) { openDialogBox(value.t) }
	//Ψһ����
	bool checkTileSlopes(const unsigned short&, BoundRact[BandRact]);
	void setDesirePosition(const BoundPosition&, const Ract&);
	void resizeContent();
	FrameIndexes* frameIndexes = nullptr;

public:
	static int nextTag;
	Fenk FenkLeft;
	Fenk FenkRight;
	Fenk FenkTop;
	Fenk FenkBottom;
	Tasnal();
	virtual ~Tasnal();
	virtual void update();
	//�������Ϊtrue��ʱ��ִ��
	virtual void  updatePosition();
	virtual void loadScript();
	void registerFrameIndexes(const LuaRef&);
	inline virtual  bool isDead() { return (getBodyStatus() == BodyStatus::dead || getBodyStatus() == BodyStatus::fall); }
	MovingDirection scanSensor(Tasnal* opponent);
	void spawn(const Vec2&, const Vec2& = Vec2::ZERO);

	void openDialogBox(const HintText&);
	Ract getCollisionBound(const Vec2& = Vec2::ZERO, const bool& isOriginBound = false);
	//��Ƭ�ͱ�������ײ
	void checkSelfCollision();
	//��Ƭ��ײ
	void checkTileCollision();
	//������ײ�󣬰Ѷ��ָ���collidedOpponent�Ա���ײϸ�ڵ���
	bool checkObjectCollision(Tasnal&, const bool& isOriginBound = false);
	//����Զ�����tag ����Tasnalmanager���й���
	//��ִ�д˺����򲻱�lua�е�Tasnal������,�����Ľ�ɫ������ܵ���ʹ��
	inline void generateTag()
	{
		setTag(nextTag);
		id = nextTag;
		nextTag++;
	}
	//�̶���
	inline void checkSpike()
	{
		//����ˢ��
		auto boundRact = getMap()->getCenterTile(getPosition());
		if (boundRact.getTeshnal().isHarmful)
		 	gotoHell("sound/sting.wav");
	}
	void checkObjectFenk();
	//Ŀǰ�����߶�ƽ��������
	void checkLine(const short& tag,const Vec2& start, const Vec2& end);
	void checkFollowing();
	void checkBorder(const Vec2&, const Vec2&);
	void checkHints(const bool& = true);
	void showHints(const LuaRef&);
	//���Զ�λ�� 
	bool locate(const Vec2& target, const bool& allowStop = true);
	void registerRebound(const LuaRef&);
	inline bool locateTile(const Vec2& tile, const bool& allowStop = true)
	{
		return locate(MAP_STAGE->getPositionByTileCoordinate(tile), allowStop);
	}
	void closeDialog();
	//��ɫ�����ͼ�߽�����
	void limiteScreenBorder(const BorderLimited& = BorderLimited::limiteHorizon);
	//������Ҫ��ȥ��HP
	BodyStatus& gotHurt(const short& = 0, const char* = "");
	//ֱ����������
	virtual void gotoHell(const char* = "");
	void registerMoves(const LuaRef&);
	virtual void setAnimation(const LuaRef&);
	//�˺�����ײ���
	inline bool gotHit(Tasnal& opponent) { return getCollisionBound(insetObject).intersectsRact(opponent.getCollisionBound(insetObject)); };
	BodyStatus& getBodyStatus();

	RangeType getRange(Tasnal*) ;
 
	//ֱ�ӵ���boundRact�����tile position����,����ˢ��һ��
	inline BoundRact getBoundSelf()
	{
		return getMap()->getCenterTile(getPosition());
	}
	/*
	*����
	*/
	inline bool& getIsSolid() { return  isSolid; };
	//ע��range�������̫���Ļ�����Щ�ط�������ײ
	RactAround getRactAround(const bool& = true);
	inline TasnalType& getType() { return this->type; };
	inline bool& isDisposed() { return this->allowDisposal; };
	inline bool isAlive() { return getBodyStatus() == BodyStatus::healthy; }
	inline bool& getOnFire() { return this->onFire; };
	inline bool& getAllowFollow() { return this->allowFollow; };
	inline int& getTouchTimes() { return this->touchedTimes; };

	inline void recover() { bodyStatus = BodyStatus::healthy; }
	inline short& getHP() { return this->hp; };
	inline short& getHPMax() { return this->hpMax; };
	inline Vec2& getInsetObject() { return this->insetObject; }
	inline Vec2& getInsetTile() { return this->insetTile; }
	inline 	TriggerSystem& getSensor() { return sensorSystem; };
	inline CollisionDirection& getCollidedDirection() { return this->lastCollideDirection; };

	inline void setIsSolid(const bool& val) { this->isSolid = val; };
	//inline void setDisposal(const bool& val) { this->allowDisposal = val; };
	//��lua�˵���
	inline void setDisposal(const bool& val = true) { LUAH->setDisposal(getTag()); };
	inline void setOnFire(const bool& val) { this->onFire = val; };
	inline void setAllowFollow(const bool& val) { this->allowFollow = val; };
	inline void setAllowTouch(const bool& val)
	{
		this->allowTouch = val;
		registerTouch();
	};
	inline void setAllowDrag(const bool& val)
	{
		allowTouch = val;
		allowDragX = val;
		allowDragY = val;
		//registerTouch();
	}
	inline void setAllowDragX(const bool& val)
	{
		allowTouch = val;
		allowDragX = val;
		//registerTouch();
	};
	inline void setAllowDragY(const bool& val)
	{
		allowTouch = val;
		allowDragY = val;
		//registerTouch();
	};
	inline void setOriginMap(const Vec2& val) { originMapPosition = val; }
	inline void setInsetObject(const Vec2& val) { this->insetObject = val; };
	inline void setInsetObjectX(const float& val) { this->insetObject.x = val; };
	inline void setInsetObjectY(const float& val) { this->insetObject.y = val; };
	inline void setInsetTile(const Vec2& val) { this->insetTile = val; }
	inline void setInsetTileX(const float& val) { this->insetTile.x = val; }
	inline void setInsetTileY(const float& val) { this->insetTile.y = val; }
	inline void setCheckTile(const bool& val) { this->onCheckTile = val; };
	inline void setHP(const short& val) { this->hp = val; };
	inline void setHPMax(const short& val) { this->hpMax = val; };
	inline void setTexture(const std::string& frameFile) { Sprite::setTexture(frameFile); }
	//ע��png����Ҫˢ֡ updateFrame
	void setFrame(const char* frameName)
	{
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
		if (nullptr != frame)
			setSpriteFrame(frame);
	}
	inline void clearFrame() { framePrefix.clear(); }
	inline void stopAnimation()
	{ 
		stopAllActions();
		isAnimateFinished = true;
	}
	inline bool isTouched(const Vec2& touched) { return inRange(touched, getCollisionBound()); }
	inline void setFrameIndex(const FrameStyle& fs, const short& start, const short& end)
	{
		frameIndexes[static_cast<short>(fs)] = FrameIndexes(start, end);
	}
	inline void setFrameSpeed(const float& val) { this->delayFrame.delaySecond = val; };
	//property�������и�setDirection
	void control(const MovingDirection&, const float& = 0);
	unsigned short& moveObject(const bool& = false);
	inline  MovingDirection& getFacedDirection() { return facedDirection; }
	inline void setFaceDirection(const MovingDirection& val) { facedDirection = val; }
	inline	CollisionDirection getCollisionDirection(const Ract& rect) { return Fand::getCollsionDirection(getCollisionBound(insetObject), rect); }
	inline Tasnal* getCollider() { return collidedOpponent; }
	inline  void gotoDesirePosition() { setPosition(desiredPosition); }
	inline WeaponSystem& getWeaponSystem() { return weaponSystem; }
	inline Weapon* getWeapon() { return weaponSystem.getCurrentWeapon(); }
	inline TiledMap* getMap() { return MAP_WITHTAG(mapTag); }
	//lua-intf����ע�����غ���������ֻ���������һ�� 
	inline Vec2 getPositionByLua() const { return getPosition(); }
	void allowCollision(CollisionDirection cd = CollisionDirection::intersected)
	{
		if (cd == CollisionDirection::intersected)
		{
			FenkLeft.allowCollision = true;
			FenkRight.allowCollision = true;
			FenkTop.allowCollision = true;
			FenkBottom.allowCollision = true;
			return;
		}
		FenkLeft.allowCollision = CollisionDirection::atLeft == cd;
		FenkRight.allowCollision = CollisionDirection::atRight == cd;
		FenkTop.allowCollision = CollisionDirection::atTop == cd;
		FenkBottom.allowCollision = CollisionDirection::atBottom == cd;
	}
	void disallowCollision(CollisionDirection cd = CollisionDirection::intersected)
	{
		if (cd == CollisionDirection::intersected)
		{
			FenkLeft.allowCollision = false;
			FenkRight.allowCollision = false;
			FenkTop.allowCollision = false;
			FenkBottom.allowCollision = false;
			return;
		}
		FenkLeft.allowCollision = CollisionDirection::atLeft == cd ? false : true;
		FenkRight.allowCollision = CollisionDirection::atRight == cd ? false : true;
		FenkTop.allowCollision = CollisionDirection::atTop == cd ? false : true;
		FenkBottom.allowCollision = CollisionDirection::atBottom == cd ? false : true;
	}	
	void gotPushed(Tasnal&);
	inline bool hasPushes()
	{
		return FenkLeft.EndPush || FenkRight.EndPush || FenkTop.EndPush || FenkBottom.EndPush;
	}
	void EndPush(CollisionDirection cd = CollisionDirection::intersected)
	{
		if (cd == CollisionDirection::intersected)
		{
			FenkLeft.EndPush = true;
			FenkRight.EndPush = true;
			FenkTop.EndPush = true;
			FenkBottom.EndPush = true;
			return;
		}
		FenkLeft.EndPush = CollisionDirection::atLeft == cd;
		FenkRight.EndPush = CollisionDirection::atRight == cd;
		FenkTop.EndPush = CollisionDirection::atTop == cd;
		FenkBottom.EndPush = CollisionDirection::atBottom == cd;
	}
	inline void disEndPush(CollisionDirection cd = CollisionDirection::intersected)
	{
		if (cd == CollisionDirection::intersected)
		{
			FenkLeft.EndPush = false;
			FenkRight.EndPush = false;
			FenkTop.EndPush = false;
			FenkBottom.EndPush = false;
			return;
		}
		FenkLeft.EndPush = CollisionDirection::atLeft == cd ? false : true;
		FenkRight.EndPush = CollisionDirection::atRight == cd ? false : true;
		FenkTop.EndPush = CollisionDirection::atTop == cd ? false : true;
		FenkBottom.EndPush = CollisionDirection::atBottom == cd ? false : true;
	}
	inline bool isHarmful()
	{
		return FenkLeft.isHarmful && FenkRight.isHarmful && FenkTop.isHarmful && FenkBottom.isHarmful;
	}
	inline void setHarmful(const bool& value = true)
	{
		FenkLeft.isHarmful = value;
		FenkRight.isHarmful = value;
		FenkTop.isHarmful = value;
		FenkBottom.isHarmful = value;
	}
};

static const Vec2 InsetTile = Vec2(12.f, 0.0f);
static const Vec2 InsetObject = Vec2(3.5f, 0.f);
static const short AniCount = static_cast<short>(Anistyle::rotate) + 1;