#include "Telisk.h"
#include "../Tasnal/Tasnal.h"
#include "../Tasnal/TasnalFrame.h"
#include "../Tasnal/Player.h"
#include "../Tasnal/ProjectTile.h"
#include "../Tasnal/RObjectPhase.h" 
#include "../Tasnal/TeliskSystem.h"
#include "../../Common/StateMachine/State.h"
#include "../../Tik/Encounter.h"
#include "../Map/TiledMap.h"
#include "../Map/CheckPoint.h"
#include "../Tasnal/Tasmina.h"
#include "../Tasnal/RObject.h"
#include "../Tasnal/Npc.h"
#include "../Hezk/StageHezk.h"
#include "../Hezk/MainHezk.h"
#include "../Trikers/Triker.h"
#include "../Ads/Ads.h"

#include "ActionExtend.h"
#include "SimpleAudioEngine.h"
#include "PopupLayer.h"
#include "Fand.h"
#include "ResourceHelper.h"
#include "JoyStick.h"

using namespace cocos2d;

static const string Luah::ScriptFolder = "script/";
unsigned short Luah::CurrentStage = 1;

void Luah::loadPackages()
{
	//����
	auto path = FileUtils::getInstance()->getWritablePath() + ScriptFolder;

	//�ж��ļ����Ƿ����
	if (!FileUtils::getInstance()->isDirectoryExist(path))
		FileUtils::getInstance()->createDirectory(path);

	//LoadFile(ScriptFolder + "robjectstatus.lua");

	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		return;
	//����������ļ��У���ִ��lua require
	setRequirePath(path.c_str());
	CCASSERT(FileUtils::getInstance()->isFileExist(ScriptFolder + "packages.lua"), "non exsits packages");
	auto luaText = __String::createWithContentsOfFile(ScriptFolder + "packages.lua")->getCString();

	FileUtils::getInstance()->writeStringToFile(luaText, path + "packages.lua");
	LoadFile(path + "packages.lua");
	auto packages = getGlobal("Packages");
	CCASSERT(packages.isTable(), "������Packages");

	for (auto iter = packages.begin(); iter != packages.end(); ++iter)
	{
		CCASSERT(iter.value().has(Luaf_File), "file������");
		auto file = iter.value()[Luaf_File].value<std::string>();

		luaText = String::createWithContentsOfFile(ScriptFolder + file)->getCString();
		FileUtils::getInstance()->writeStringToFile(luaText, path + file);
		//	LoadFile(path + file);
	}
}
string Luah::LoadStage()
{
	//�ֻ��豸�洢Ŀ¼
	auto stage = CurrentStage;
	auto stageFile = StringUtils::format("stage%d.lua", stage);
	//���û��stage�ļ�ֱ���˳�
	if (!FileUtils::getInstance()->isFileExist(ScriptFolder + stageFile))
		return "";
	auto language = UserDefault::getInstance()->getIntegerForKey(User_Language, Lang_Chinwan);

	const auto globalFile = "global.lua";
	auto sceneFile = StringUtils::format("scene%d.lua", language);
	auto popupFile = StringUtils::format("popup%d.lua", language);
	auto dialogFile = StringUtils::format("dialog%d-%d.lua", stage, language);
	auto stateFile = StringUtils::format("state%d.lua", stage);
	////�����winLoadwsƽ̨
	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	{
		setRequirePath("script\\");
		LoadFile(ScriptFolder + globalFile);
		LoadFile(ScriptFolder + sceneFile);
		LoadFile(ScriptFolder + dialogFile);
		LoadFile(ScriptFolder + popupFile);
		LoadFile(ScriptFolder + stageFile);
		LoadFile(ScriptFolder + stateFile);

		return ScriptFolder + stageFile;
	}

	auto path = FileUtils::getInstance()->getWritablePath() + ScriptFolder;
	//���ѡ������Թۿ���ǰ���Ժ͹ؿ�һ�£����˳�
	if (language == currentLanguage && stage == currentStage)
	{
		LoadFile(path + globalFile);
		LoadFile(path + sceneFile);
		LoadFile(path + dialogFile);
		LoadFile(path + popupFile);
		LoadFile(path + stageFile);
		LoadFile(path + stateFile);

		return path + stageFile;
	}

	//params ȫ�ֱ���ҲҪ����
	auto luaText = String::createWithContentsOfFile(ScriptFolder + globalFile)->getCString();
	auto writtenResult = FileUtils::getInstance()->writeStringToFile(luaText, path + globalFile);
	LoadFile(path + globalFile);

	CCASSERT(FileUtils::getInstance()->isFileExist(ScriptFolder + dialogFile), "dialogFile lua non exists");
	CCASSERT(FileUtils::getInstance()->isFileExist(ScriptFolder + popupFile), "popupFile lua non exists");
	CCASSERT(FileUtils::getInstance()->isFileExist(ScriptFolder + stateFile), "stateFile lua non exists");
	CCASSERT(FileUtils::getInstance()->isFileExist(ScriptFolder + dialogFile), "dialogFile lua non exists");
	//�Ի� language
	luaText = String::createWithContentsOfFile(ScriptFolder + dialogFile)->getCString();
	FileUtils::getInstance()->writeStringToFile(luaText, path + dialogFile);
	LoadFile(path + dialogFile);
	//������ 
	luaText = String::createWithContentsOfFile(ScriptFolder + popupFile)->getCString();
	FileUtils::getInstance()->writeStringToFile(luaText, path + popupFile);
	LoadFile(path + popupFile);
	//�ؿ�
	luaText = String::createWithContentsOfFile(ScriptFolder + stageFile)->getCString();
	FileUtils::getInstance()->writeStringToFile(luaText, path + stageFile);
	LoadFile(path + stageFile);
	//ÿһ�ص�״̬��
	luaText = String::createWithContentsOfFile(ScriptFolder + stateFile)->getCString();
	FileUtils::getInstance()->writeStringToFile(luaText, path + stateFile);
	LoadFile(path + stateFile);
	//��¼��һ�ε�ѡ��
	currentStage = stage;
	currentLanguage = language;
	return path + stageFile;

}

void Luah::LoadHezk()
{
	//�ֻ��豸�洢Ŀ¼
	auto language = UserDefault::getInstance()->getIntegerForKey(User_Language, Lang_Chinwan);
	//�����ǰ����һ����ʲô������
	auto sceneFile = StringUtils::format("scene%d.lua", language);
	//�����winLoadwƽ̨
	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	{
		setRequirePath("script\\");
		LoadFile(ScriptFolder + "global.lua");
		LoadFile(ScriptFolder + sceneFile);
		return;
	}

	auto path = FileUtils::getInstance()->getWritablePath() + ScriptFolder;


	//��������ļ���
	//FileUtils::getInstance()->addSearchPath(path);
	//params ȫ�ֱ���ҲҪ����
	auto luaText = String::createWithContentsOfFile(ScriptFolder + "global.lua")->getCString();
	auto writtenResult = FileUtils::getInstance()->writeStringToFile(luaText, path + "global.lua");
	LoadFile(path + "global.lua");
	//scene
	luaText = String::createWithContentsOfFile(ScriptFolder + sceneFile)->getCString();
	writtenResult = FileUtils::getInstance()->writeStringToFile(luaText, path + sceneFile);
	LoadFile(path + sceneFile);

}
void Luah::registerClasses()
{
	using namespace LuaIntf;

	//������̨
	LuaBinding(l).beginClass<Layer>("Layer")
		.addFunction("getName",&Layer::getName)
		.endClass()
		.beginExtendClass<MainHezk, Layer>("MainHezk")
		.addFunction("promptAchieve", &MainHezk::promptAchieve)
		.endClass()
		.beginExtendClass<StageHezk, Layer>("StageHezk")
		.addFunction("gotoNext", &StageHezk::gotoNext)
		.addFunction("getBeginDelta", &StageHezk::getBeginDelta)
		.addFunction("getEndDelta", &StageHezk::getEndDelta)
		.addFunction("isSlided", &StageHezk::isSlided)
		.addFunction("isTypeDone", &StageHezk::isTypeDone )
		.addFunction("prompt", &StageHezk::prompt)
		.addFunction("promptAchieve", &StageHezk::promptAchieve)
		.addFunction("output", &StageHezk::output)
		.addFunction("loadCurtain", &StageHezk::loadCurtain)
		.addFunction("dropCurtain", &StageHezk::dropCurtain)
		.addFunction("shutCurtain", &StageHezk::shutCurtain)
		.addFunction("setToolbar", &StageHezk::setToolbar)
		.endClass()
		.beginExtendClass<PopupLayer, Layer>("PopupLayer")	//�Ի���
		.addFunction("getChild", &PopupLayer::getChild)
		.addFunction("isSlided", &PopupLayer::isSlided)
		.addFunction("allowSmack", &PopupLayer::allowSmack)
		.addFunction("setSmack", &PopupLayer::setSmack)
		.addFunction("setString", &PopupLayer::setString)
		.addFunction("setButtonName", &PopupLayer::setButtonName)
		.addFunction("setButtonVisible", &PopupLayer::setButtonVisible)
		.endClass();

	//��ɫ
	LuaBinding(l).beginClass<Fenk>("Fenk")
		.addVariableRef("KnockMax", &Fenk::KnockMax)
		.addVariableRef("pushSteps", &Fenk::pushSteps)
		.addVariableRef("knockedTimes", &Fenk::knockedTimes)
		.addVariableRef("isHarmful", &Fenk::isHarmful)
		.addVariableRef("isShowThorn", &Fenk::isShowThorn)
		.addVariableRef("EndPush", &Fenk::EndPush)
		.addVariableRef("allowCollision", &Fenk::allowCollision)
		.addVariableRef("isCollided", &Fenk::isCollided)
		.addFunction("getAllowKnock", &Fenk::getAllowKnock)
		.endClass();


	LuaBinding(l).beginClass<Node>("Node")
		.addFunction("getTag", &Node::getTag)
		.addFunction("setZOrder", &Node::setLocalZ)
		.addFunction("getPositionX", &Node::getPositionX)
		.addFunction("getPositionY", &Node::getPositionY)
		.addFunction("getBoundingBox",&Node::getBoundingBox)
		
		.addFunction("getContentSize", &Node::getContentSize)
		.endClass()
		.beginExtendClass<Sprite, Node>("Sprite")
		
		.addTeshnal("isFlippedX", &Sprite::isFlippedX, &Sprite::setFlippedX)
		.addTeshnal("isFlippedY", &Sprite::isFlippedY, &Sprite::setFlippedY)
		.addTeshnal("isVisible", &Sprite::isVisible, &Sprite::setVisible)
		.endClass()
		.beginExtendClass<Teshnal, Sprite>("Teshnal")
		.addFunction("getDistance", &Teshnal::getDistance)
		.addFunction("isMoving", &Teshnal::isMoving)
		.addFunction("getDirection", &Teshnal::getDirection)
		.addFunction("getMovingX", &Teshnal::getMovingX)
		.addFunction("getMovingY", &Teshnal::getMovingY)
		.addFunction("setDirection", &Teshnal::setDirection)
		.addFunction("setForward", &Teshnal::setForward)
		.addFunction("setBackward", &Teshnal::setBackward)
		.addFunction("setUpward", &Teshnal::setUpward)
		.addFunction("setDownward", &Teshnal::setDownward)
		.addFunction("refreshOrigin", &Teshnal::refreshOrigin)
		.addFunction("respawn", &Teshnal::respawn)
		.addFunction("stop", &Teshnal::stop)
		.addFunction("resetVelocity", &Teshnal::resetVelocity)
		.addFunction("resetGravity", &Teshnal::resetGravity)
		.addFunction("setGravity", &Teshnal::setGravity)
		.addFunction("setGravityOn", &Teshnal::setGravityOn)
		.addFunction("setGravityOff", &Teshnal::setGravityOff)
		.addFunction("resetMoveSteps", &Teshnal::resetMoveSteps)
		.addFunction("setJumpForce", &Teshnal::setJumpForce)
		.addFunction("setJumpForceOnWall", &Teshnal::setJumpForceOnWall)
		.addTeshnal("originPosition", &Tasnal::getOrigin, &Tasnal::setOrigin)
		.addTeshnal("moveStep", &Teshnal::getMoveStep, &Teshnal::setMoveStep)
		.addTeshnal("onObject", &Teshnal::getOnObject, &Teshnal::setonObject)
		.addTeshnal("onJump", &Teshnal::getOnJump, &Teshnal::setOnJump)
		.addTeshnal("allowGravity", &Teshnal::getGravityStatus, &Teshnal::setGravityStatus)
		.addFunction("getJumpStatus", &Teshnal::getJumpStatus)
		.addFunction("inAir", &Teshnal::inAir)
		.addFunction("displace", &Teshnal::displace)
		.endClass()
		.beginExtendClass<Tasnal, Teshnal>("Tasnal")
		.addVariableRef("FenkLeft", &Tasnal::FenkLeft)
		.addVariableRef("FenkRight", &Tasnal::FenkRight)
		.addVariableRef("FenkTop", &Tasnal::FenkTop)
		.addVariableRef("FenkBottom", &Tasnal::FenkBottom)
		.addTeshnal("face", &Tasnal::getFacedDirection, &Tasnal::setFaceDirection)
		.addFunction("getBoundSelf", &Tasnal::getBoundSelf)
		.addFunction("getType", &Tasnal::getType)
		.addFunction("getInsetObject", &Tasnal::getInsetObject)
		.addFunction("getRange", &Tasnal::getRange)
		.addFunction("getPosition", &Tasnal::getPositionByLua)
		.addFunction("getCollisionBound", &Tasnal::getCollisionBound)
		.addFunction("getCollisionDirection", &Tasnal::getCollisionDirection)
		.addFunction("getWeaponSystem", &Tasnal::getWeaponSystem)
		.addFunction("getWeapon", &Tasnal::getWeapon)
		.addFunction("getCollidedDirection", &Tasnal::getCollidedDirection)
		.addFunction("getBodyStatus", &Tasnal::getBodyStatus)
		.addFunction("getTouchTimes", &Tasnal::getTouchTimes)
		.addFunction("gotPushed", &Tasnal::gotPushed)
		.addFunction("recover", &Tasnal::recover)
		.addFunction("spawn", &Tasnal::spawn)
		.addFunction("setFrame", &Tasnal::setFrame)
		.addFunction("setFrameIndex", &Tasnal::setFrameIndex)
		.addFunction("setOnFire", &Tasnal::setOnFire)
		.addFunction("setAllowFollow", &Tasnal::setAllowFollow)

		.addFunction("setAnimation", &Tasnal::setAnimation)
		.addFunction("setFrameSpeed", &Tasnal::setFrameSpeed)
		.addFunction("setAllowTouch", &Tasnal::setAllowTouch)
		.addFunction("setAllowDrag", &Tasnal::setAllowDrag)
		.addFunction("setAllowDragX", &Tasnal::setAllowDragX)
		.addFunction("setAllowDragY", &Tasnal::setAllowDragY)
		.addFunction("setCheckTile", &Tasnal::setCheckTile)
		.addFunction("setTexture", &Tasnal::setTexture)

		.addFunction("clearFrame", &Tasnal::clearFrame)
		.addFunction("stopAnimation", &Tasnal::stopAnimation)
		.addFunction("locate", &Tasnal::locate)
		.addFunction("locateTile", &Tasnal::locateTile)
		.addFunction("control", &Tasnal::control)
		.addFunction("scanSensor", &Tasnal::scanSensor)
		.addFunction("allowFollow", &Tasnal::getAllowFollow)
		.addFunction("gotoDesirePosition", &Tasnal::gotoDesirePosition)
		.addFunction("updatePosition", &Tasnal::updatePosition)

		.addFunction("checkHints", &Tasnal::checkHints)
		.addFunction("showHints", &Tasnal::showHints)
		.addFunction("closeDialog", &Tasnal::closeDialog)
		.addFunction("checkLine", &Tasnal::checkLine)
		.addFunction("checkSpike", &Tasnal::checkSpike)
		.addFunction("checkObjectCollision", &Tasnal::checkObjectCollision)
		.addFunction("checkObjectFenk", &Tasnal::checkObjectFenk)
		.addFunction("checkFollowing", &Tasnal::checkFollowing)
		.addFunction("checkTileCollision", &Tasnal::checkTileCollision)
		.addFunction("checkBorder", &Tasnal::checkBorder)
		.addFunction("limiteScreenBorder", &Tasnal::limiteScreenBorder)
		.addFunction("gotHit", &Tasnal::gotHit)
		.addFunction("gotHurt", &Tasnal::gotHurt)
		.addFunction("gotoHell", &Tasnal::gotoHell)
		.addFunction("allowCollision", &Tasnal::allowCollision)
		.addFunction("disallowCollision", &Tasnal::disallowCollision)
		.addFunction("hasPushes", &Tasnal::hasPushes)
		.addFunction("EndPush", &Tasnal::EndPush)
		.addFunction("disEndPush", &Tasnal::disEndPush)
		.addTeshnal("isSolid", &Tasnal::getIsSolid, &Tasnal::setIsSolid)

		.addTeshnal("isHarmful", &Tasnal::isHarmful, &Tasnal::setHarmful)
		.addFunction("isAlive", &Tasnal::isAlive)
		.addFunction("isTouched", &Tasnal::isTouched)
		.addFunction("isDead", &Tasnal::isDead)
		.addTeshnal("allowFollow", &Tasnal::getAllowFollow, &Tasnal::setAllowFollow)
		.addTeshnal("hp", &Tasnal::getHP, &Tasnal::setHP)
		.addTeshnal("hpMax", &Tasnal::getHPMax, &Tasnal::setHPMax)
		.endClass()
		.beginExtendClass<Player, Tasnal>("Player")
		.addFunction("isDead", &Player::isDead)
		.addFunction("gotoHell", &Player::gotoHell)
		.addFunction("carryObject", &Player::carryObject)
		.addFunction("isCarrying", &Player::isCarrying)
		.addFunction("setCarriedOffset", &Player::setCarriedOffset)
		.addFunction("dropObject", &Player::dropObject)
		.addFunction("isFreezed", &Player::isFreezed)
		.addFunction("freeze", &Player::freeze)
		.addFunction("unfreeze", &Player::unfreeze)
		.addFunction("setAnimation", &Player::setAnimation)
		.endClass()
		.beginExtendClass<RObject, Tasnal>("RObject")
		.addFunction("registerChar", &RObject::registerChar)
		.addFunction("registerSpring", &RObject::registerSpring)
		.addFunction("registerRebound", &RObject::registerRebound)
		.addFunction("registerMoves", &RObject::registerMoves)
		.addFunction("registerKnocks", &RObject::registerKnocks)
		.addFunction("registerText", &RObject::registerText)
		.addFunction("registerSwitch", &RObject::registerSwitch)
		.addFunction("setAnimation", &RObject::setAnimation)
		.addFunction("hasKnocks", &RObject::hasKnocks)
		.addFunction("checkRactAround", &RObject::checkRactAround)
		.addFunction("switchFrame", &RObject::switchFrame)
		.addFunction("gotKnocked", &RObject::gotKnocked)
		.addFunction("checkKnocks", &RObject::checkKnocks)
		.addFunction("moveObject", &RObject::moveObject)
		.addFunction("pollChar", &RObject::pollChar)
		.addFunction("getChar", &RObject::getChar)
		.addFunction("getSwitch", &RObject::getSwitch)
		.addFunction("blinkGroup", &RObject::blinkGroup)
		.addFunction("blinkSelf", &RObject::blinkSelf)
		.addFunction("bounce", &RObject::bounce)
		.endClass()
		.beginExtendClass<Npc, Tasnal>("Npc")
		.addFunction("setAnimation", &Npc::setAnimation)
		.addFunction("setTowards", &Npc::setTowards)
		.addFunction("patrollingX", &Npc::patrollingX)
		.addFunction("patrollingY", &Npc::patrollingY)
		.addFunction("wanderingX", &Npc::wanderingX)
		//������
		.endClass().beginExtendClass<ProjectTile, Tasnal>("ProjectTile")
		.addFunction("allowThrough", &ProjectTile::allowThrough)
		.addFunction("getProjectType", &ProjectTile::getProjectType)
		.addFunction("getOwner", &ProjectTile::getOwner)
		.addFunction("getTarget", &ProjectTile::getTarget)
		.addFunction("isDestiny", &ProjectTile::isDestiny)
		.addFunction("isDead", &ProjectTile::isDead)
		.endClass()
		.beginExtendClass<SlugObject, ProjectTile>("SlugObject")
		.addFunction("isDead", &SlugObject::isDead)
		.endClass()
		.beginExtendClass<Footboard, ProjectTile>("Footboard")
		.addFunction("isDead", &Footboard::isDead)
		.endClass()
		.beginExtendClass<Deadshot, ProjectTile>("Deadshot")
		.addFunction("isDead", &Deadshot::isDead)
		.endClass()
		.beginExtendClass<Trace, ProjectTile>("Trace")
		.addFunction("isDead", &Trace::isDead)
		.endClass();
	//������
	LuaBinding(l).beginClass<Triker>("Triker")
		.addFunction("isTouching", &Triker::isTouching)
		.addFunction("scan", &Triker::scanTouching)
		.addFunction("setActivation", &Triker::setActivation)
		.endClass()
		.beginExtendClass<TrikerLine, Triker>("TrikerLine")
		.addFunction("scan", &Triker::scanTouching)
		.endClass()
		.beginExtendClass<TrikerCircle, Triker>("TrikerCircle")
		.addFunction("scan", &Triker::scanTouching)
		.endClass()
		.beginExtendClass<TrikerRact, Triker>("TrikerRact")
		.addFunction("scan", &Triker::scanTouching)

		.endClass();
	LuaBinding(l).beginClass<WeaponSystem>("WeaponSystem")
		.addFunction("shootAt", &WeaponSystem::shootAt)
		.addFunction("getCurrent", &WeaponSystem::getCurrentWeapon)
		.endClass();
	LuaBinding(l).beginClass<Weapon>("Weapon")
		.addFunction("unload", &Weapon::unload)
		.addFunction("setFireOn", &Weapon::setFireOn)
		.addFunction("setFireOff", &Weapon::setFireOff)
		.addFunction("isFiring", &Weapon::isFiring)
		.addFunction("registerWeapon", &Weapon::registerWeapon)
		.addFunction("reload", &Weapon::reload)
		.addFunction("getRef", &Weapon::getRef)
		.addFunction("getSlugLeft", &Weapon::getSlugLeft)
		.addFunction("shootAt", &Weapon::shootAt)
		//.endClass()
		//.beginExtendClass<Pistol,Weapon>("Pistol")
		.endClass();
	//��Դ��
	LuaBinding(l).beginClass<std::string>("String")
		.endClass();
	LuaBinding(l).beginClass<Luah>("Luah")
		.addStaticFunction("output", &Luah::output)
		.endClass();
	LuaBinding(l).beginClass<Clock>("Clock")
		.addStaticFunction("getCounter", &Clock::getTickInteger)
		.addStaticFunction("getTick", &Clock::getTickFloat)
		.endClass();
	//�ӳ�
	LuaBinding(l).beginClass<DelayCollection>("Delay")
		.addStaticFunction("isTimeUp", &DelayCollection::isTimeUp)
		.addStaticFunction("reset", &DelayCollection::reset)
		.endClass();
	//��Դ��������������¼
	LuaBinding(l).beginClass<Resh>("Resh")
		.addStaticVariable("Kingsure", &Resh::Kingsure)
		.addStaticFunction("openURL", &Resh::openURL)
		.addStaticFunction("playSound", &Resh::playSound)
		.addStaticFunction("playMusic", &Resh::playMusic)
		.addStaticFunction("resetAchieve", &Resh::resetAchieve)
		.addStaticFunction("setMusicVolume", &Resh::setMusicVolume)
		.addStaticFunction("getLanguage", &Resh::getLanguage)
		.addStaticFunction("getNumber", &Resh::getNumber)
		.addStaticFunction("addNumber", &Resh::addNumber)
		.addStaticFunction("setNumber", &Resh::setNumber)
		.addStaticFunction("getAchieveCount", &Resh::getAchieveCount)
		.addStaticFunction("getPlatform", &Resh::getPlatform)
		.addStaticFunction("reset", &Resh::reset)
			.addStaticFunction("getVisibleSize", &Resh::getVisibleSize)
		.endClass();
	LuaBinding(l).beginClass<Ract>("Ract")
		.addConstructor(LUA_ARGS(float, float, float, float))
		.addVariable("origin", &Ract::origin)
		.addFunction("getMinX", &Ract::getMinX)
		.addFunction("getMidX", &Ract::getMidX)
		.addFunction("getMaxX", &Ract::getMaxX)
		.addFunction("getMinY", &Ract::getMinY)
		.addFunction("getMidY", &Ract::getMidY)
		.addFunction("getMaxY", &Ract::getMaxY)
		//.endClass()
		//.beginExtendClass<BoundRact, Ract>("BoundRact")
		//.addVariableRef("gid", &BoundRact::gid)
		//.addVariableRef("tilePosition", &BoundRact::tilePosition)
		//.addFunction("getTeshnal", &BoundRact::getTeshnal)
		.endClass();
	LuaBinding(l).beginClass<BoundRact>("BoundRact")
		.addVariable("gid", &BoundRact::gid)
		.addVariable("tilePosition", &BoundRact::tilePosition)
		.addFunction("getTeshnal", &BoundRact::getTeshnal)
		.endClass();
	LuaBinding(l).beginClass<Clock>("Clock")
		.addStaticTeshnal("tick", &Clock::getTickFloat)
		.endClass();

	LuaBinding(l).beginClass<Vec2>("Vec2")
		.addVariableRef("x", &Vec2::x)
		.addVariableRef("y", &Vec2::y)
		.addStaticVariable("ZERO", &Vec2::ZERO)
		.addConstructor(LUA_ARGS(float, float))
		.endClass();
	//��ͼ
	LuaBinding(l).beginClass<TiledMap>("TiledMap")
		.addFunction("moveCameraX", &TiledMap::moveCameraX)
		.addFunction("setCameraY", &TiledMap::setCameraY)
		.addFunction("setCameraCenter", &TiledMap::setCameraCenter)
		.addFunction("setCameraSlide", &TiledMap::setCameraSlide)
		.addFunction("setCameraFrame", &TiledMap::setCameraFrame)
		.addFunction("setCameraRepeat", &TiledMap::setCameraRepeat)
		.addFunction("collapse", &TiledMap::collapse)
		.addFunction("exchangeMap", &TiledMap::exchangeMap)
		.addFunction("getWalls", &TiledMap::getWalls)
		.addFunction("setLayerGid", &TiledMap::setLayerGid)
		.addFunction("setObjectGid", &TiledMap::setObjectGid)
		.addFunction("setGid", &TiledMap::setGid)
		.addFunction("setGidRange", &TiledMap::setGidRange)
		//	.addFunction("reorderChild", &TiledMap::reorderChild)
		.addFunction("removeTile", &TiledMap::removeTile)
		.addFunction("removeTileRange", &TiledMap::removeTileRange)
		.addFunction("removeObjectRange", &TiledMap::removeObjectRange)
		.addFunction("removeLayerRange", &TiledMap::removeLayerRange)
		.addFunction("registerKnocks", &TiledMap::registerKnocks)
		.addFunction("getPtc", &TiledMap::getPositionByTileCoordinate)
		.addFunction("setPtc", &TiledMap::setPtc)
		.addFunction("getPositionY", &TiledMap::getPositionY)
		.addFunction("getPositionX", &TiledMap::getPositionX)
		.addFunction("getRange", &TiledMap::getRange)
		.endClass();
	//�Ի�ϵͳ
	LuaBinding(l).beginClass<TeliskSystem>("TeliskSystem")
		.addStaticFunction("proceed", &TeliskSystem::proceed)
		.addStaticFunction("update", &TeliskSystem::update)
		.addStaticFunction("close", &TeliskSystem::close)
		.addStaticFunction("setIndex", &TeliskSystem::setIndex)
		.addStaticFunction("isDone", &TeliskSystem::isDone)
		.endClass();
	//��ɫ������
	LuaBinding(l).beginClass<Tasmina>("Tasmina")
		.addStaticFunction("appendTasnal", &Tasmina::appendTasnal)
		.addStaticFunction("updateTasnal", &Tasmina::updateTasnal)
		.endClass();
	//��̬����
	LuaBinding(l).beginModule("Constant")
		.addVariable("User_Trial", &User_Trial)
		.addVariable("User_LastStage", &User_LastStage)


		.endModule();
	//��̬����
	LuaBinding(l).beginClass<Checkpoint>("Checkpoint")
		.addStaticFunction("setPosition", &Checkpoint::setPosition)
		.addStaticFunction("setPtc", &Checkpoint::setPtc)
		.addStaticFunction("setObject", &Checkpoint::setObject)
		.addStaticFunction("getObject", &Checkpoint::getObject)
		.addStaticFunction("addObject", &Checkpoint::addObject)
		.addStaticFunction("setTile", &Checkpoint::setTile)
		.addStaticFunction("addLife", &Checkpoint::addLife)
		.addStaticVariableRef("id", &Checkpoint::id)
		.addStaticVariableRef("position", &Checkpoint::position)
		.addStaticVariableRef("gid", &Checkpoint::gid)
		.addStaticVariableRef("mapID", &Checkpoint::mapID)
		.addStaticVariableRef("rangeStart", &Checkpoint::rangeStart)
		.addStaticVariableRef("rangeEnd", &Checkpoint::rangeEnd)
		.addStaticVariableRef("life", &Checkpoint::life)
		.addStaticVariableRef("trial", &Checkpoint::trialTimes)
		.addStaticFunction("updateTrial", &Checkpoint::updateTrial)
		.endClass();
	//��̬��������
	LuaBinding(l).beginClass<ActionExtend>("ActionExtend")
		.addStaticFunction("corrupt", &ActionExtend::corrupt)
		.addStaticFunction("toast", &ActionExtend::toast)
		.endClass();
	//��̬��������
	LuaBinding(l).beginClass<Ads>("Ads")
			.addStaticFunction("popup", &Ads::showInterstitial)
			.addStaticFunction("updateBanner", &Ads::updateBanner)
			.addStaticFunction("updateInterstitial", &Ads::updateInterstitial)
			.addStaticFunction("showBanner", &Ads::showBanner)
			.addStaticFunction("showReward", &Ads::showReward)
			.addStaticFunction("showVideo", &Ads::showVideo)
			.endClass();
	LuaBinding(l).beginClass<Size>("Size")
		.addConstructor(LUA_ARGS(float, float))
			.addVariable("width", &Size::width)
			.addVariable("height", &Size::height)
		.endClass();
	LuaBinding(l).beginClass<Joystick>("Joystick")
		.addFunction("getDirection", &Joystick::getDirection)
		.addFunction("getAngle", &Joystick::getAngle)
		.addFunction("getTouchedPad", &Joystick::getTouchedPad)
		.addFunction("getTouches", &Joystick::getTouches)
		.addFunction("setString", &Joystick::setString)
		.addFunction("setVisible", &Joystick::setVisible)
		.addFunction("setAllVisible", &Joystick::setAllVisible)
		.addFunction("hideRocker", &Joystick::hideRocker)
		.addFunction("showRocker", &Joystick::showRocker)
		.addFunction("hideJoystick", &Joystick::hideRocker)
		.addFunction("showJoystick", &Joystick::showRocker)
		.endClass();

	//�ṹ�� ֡ �û�ש�� �ƶ� ��˸ ��ǩ
	LuaBinding(l).beginClass<BlockTesh>("BlockTesh")
		.addStaticFunction("registerBlinks", &BlockTesh::registerBlinks)
		.endClass();
	LuaBinding(l).beginClass<KnockPhase>("KnockPhase")
		.addVariable("direction", &KnockPhase::colliderDirection)
		.addVariable("knockAction", &KnockPhase::knockAction)
		.addVariable("times", &KnockPhase::times)
		.addVariable("isColliding", &KnockPhase::isColliding)
		.endClass();
	//LuaBinding(l).beginClass<FrameIndexes>("Frame")
	//	.addConstructor(LUA_ARGS(unsigned short, unsigned short, unsigned short))
	//	.endClass();
	LuaBinding(l).beginClass<FrameSwitch>("FrameSwitch")

		.addConstructor(LUA_ARGS(void))
		//.addVariable("frameIndex", &FrameIndexes::index)
		//armeabi gcc �����ø���FrameIndexesע��lua����������
		.addFunction("getIndex", &FrameSwitch::getIndex)
		//.endClass()
		//.beginExtendClass<FrameSwitch, FrameIndexes>("FrameSwitch")
		.addVariable("direction", &FrameSwitch::direction)
		.addVariable("touchedTimes", &FrameSwitch::touchedTimes)
		.addVariable("framePrefix", &FrameSwitch::framePrefix)
		.addVariable("isTouched", &FrameSwitch::isTouched)
		.endClass();
	
	LuaBinding(l).beginClass<SpringInfo>("SpringInfo") 
		.addVariable("isBounced", &SpringInfo::isBounced)
		.addVariable("force", &SpringInfo::force)
		.addVariable("times", &SpringInfo::times)
		.endClass();
	LuaBinding(l).beginClass<TileTeshnalInfo>("TileInfo")
		.addVariable("isOneWay", &TileTeshnalInfo::isOneWay)
		.addVariable("isHarmful", &TileTeshnalInfo::isHarmful)
		.addVariable("allowThrough", &TileTeshnalInfo::allowThrough)
		.addVariable("gid", &TileTeshnalInfo::gid)
		.addVariable("inset", &TileTeshnalInfo::inset)
		.endClass();

void Luah::preloadResources(const char* resTable)
{
	using namespace CocosDenshion;
	//��ԴԤ�ȼ���
	auto resources = getGlobal(resTable);

	CCASSERT(resources.isTable(), StringUtils::format("[Luah:preloadResources]������%s", resTable).c_str());
	for (auto iter = resources.begin(); iter != resources.end(); ++iter)
	{
		if (iter.value().has(Luaf_Sound))
			SimpleAudioEngine::getInstance()->preloadEffect(iter.value()[Luaf_Sound].value<const char*>());
		//ios �����ж������Ƿ񲥷�
		else if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() && iter.value().has(Luaf_Music))
			SimpleAudioEngine::getInstance()->preloadBackgroundMusic(iter.value()[Luaf_Music].value<const char*>());
		else if (iter.value().has(Luaf_Image))
			Director::getInstance()->getTextureCache()->addImage(iter.value()[Luaf_Image].value<std::string>());
		//	else if (iter.value().has(Luaf_Frame))
		 //
			//	assert(iter.value().has(Luaf_Ract) && "������rect");
				//	Resh::preloadFrame(iter.value()[Luaf_Frame].value<std::string>(), iter.value()[Luaf_Ract].value<Ract>());

		else if (iter.value().has(Luaf_Plist))
		{
			auto plist = iter.value()[Luaf_Plist].value<std::string>();
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist + ".plist", plist + ".png");
		}
	}
}
void Luah::registerTasnal(const char * className, const char * funcName, Tasnal * role)
{
	LuaRef ref = createTable();
	ref.set(Luaf_CppRef, role);
	flush();
	callback(className, funcName, ref);
}

void Luah::processTable(const char * tableName, std::function<void(LuaIntf::LuaRef)> each, const bool& needAssert)
{
	auto luaTable = getGlobal(tableName);
	if (needAssert)
	{
		if (!luaTable.isTable())
		{//ע������еĻ����׳�
			CCLOG("������processTable%s", tableName);
			return;
		}
		
	}
	else
	{
		if (!luaTable.isTable())
			return;
	}

	//auto len = luaTable.len();
	for (auto iter = luaTable.begin(); iter != luaTable.end(); ++iter)
	{
		//�������ÿһ����table ��
	//	auto isValid = iter.value().isValid();
		if (iter.value().isTable())
			each(iter.value());
	}
}

void Luah::processTableFont(const char * tableName, std::function<void(LuaIntf::LuaRef, const FontInfo&)> each, const bool & needAssert)
{
	auto luaTable = getGlobal(tableName);
	if (needAssert)
	{
		if (!luaTable.isTable())
		{//ע������еĻ����׳�
			CCLOG("������processTable%s", tableName);
			return;
		}
	}
	else
	{
		if (!luaTable.isTable())
			return;
	}

	for (auto iter = luaTable.begin(); iter != luaTable.end(); ++iter)
	{
		FontInfo font(iter.value());
		each(iter.value(), font);
	}
}

LuaIntf::LuaRef Luah::getTasnal(const int & tag, const std::string & name)
{
	auto role = getGlobal(Luat_Tasnal);
	CCASSERT(role.len() > 0, "role�����������1");

	if (0 == tag)
		return role;

	return "" == name ? role[tag].value() : role[tag].value()[name].value();
}

void Luah::callback(const char* className, const char* funcName)
{
	try
	{
		auto luaFunc = getGlobal( Luac_Callback);
		if (luaFunc.isFunction())
			luaFunc(className, funcName);

	}
	catch (LuaIntf::LuaException  e)
	{
		
#if  CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT
		log(StringUtils::format("[%s:%s]%s", className, funcName, e.what()).c_str());
#else
		
		log("callback error");
#endif
	}
}

void Luah::callback(const char* className, const char* funcName, const LuaRef&  params)
{
	try
	{
		auto luaFunc = getGlobal(Luac_Callback);
		if (luaFunc.isFunction())
			luaFunc(className, funcName, params);

	}
	catch (LuaIntf::LuaException  e)
	{

#if  CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT
		log(StringUtils::format("[%s:%s]%s", className, funcName, e.what()).c_str());
#else
		
		log("callback error");
#endif
	}
}

void Luah::callmenu(const char* className, const char* funcName, const LuaRef& params)
{
	try
	{
		auto luaFunc = getGlobal(Luac_CallbackMenu);
		if (luaFunc.isFunction())
			luaFunc(className, funcName, params);

	}
	catch (LuaIntf::LuaException  e)
	{

#if  CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT
		log(StringUtils::format("[%s:%s]%s", className, funcName, e.what()).c_str());
#else
		
		log("callmenu error");
#endif
	}
}
