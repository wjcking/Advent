#include "Triker.h"
#include "../Utils/Constant.h"
#include "../Tasnal/Tasnal.h"
#include "../Utils/Utils.h"
Triker::Triker(const bool& bAct) :toBeRemoved(false),
bActive(bAct)
//graphNodeIndex(-1)
{
	relatedTags.reserve(Reserve_Tasnal);
}
Triker::Triker(const Size& size, const MovingDirection& md) :sensorRange(size),
sensorDirection(md),
toBeRemoved(false),
bActive(true)
{
	relatedTags.reserve(Reserve_Tasnal);
}



Triker::~Triker() {  }

/******* �߶�********/
TrikerLine::TrikerLine(const Vec2& start, const Vec2& end) :start(start), end(end)
{

}
TrikerLine::TrikerLine(const Size& range, const MovingDirection& md) : Triker(range, md) {};
TrikerLine::TrikerLine() {};
TrikerLine::~TrikerLine() {}

bool& TrikerLine::scanTouching(Tasnal& entityRact)
{
	isTouched = false;
	auto bound = entityRact.getCollisionBound();
	distance = Fand::getRange(Ract(start.x, end.y, end.x - start.x, 0.05f), bound);

	if (distance < RangeType::rangeThreatened)
		return isTouched;
	isTouched = Fand::isLineIntersectRact(start, end, entityRact.getCollisionBound());

	return isTouched;
}
void TrikerLine::render(DrawNode* draw)
{
	draw->drawLine(start, end, getColor());
};

void TrikerLine::updateRegion(Tasnal& role, const float& radius)
{
	switch (sensorDirection)
	{
	case toLeft:
		start = Vec2(role.getCollisionBound().origin.x - sensorRange.width, role.getCollisionBound().getMidY());
		end = Vec2(role.getCollisionBound().origin.x, role.getCollisionBound().getMidY());
		break;
	case toRight:
		start = Vec2(role.getCollisionBound().getMaxX(), role.getCollisionBound().getMidY());
		end = Vec2(role.getCollisionBound().getMaxX() + sensorRange.width, role.getCollisionBound().getMidY());
		break;
	case toTop:
		start = Vec2(role.getCollisionBound().getMinX(), role.getCollisionBound().getMaxY());
		end = Vec2(role.getCollisionBound().getMinX(), role.getCollisionBound().getMaxY() + sensorRange.height);
		break;
	case toBottom:
		start = Vec2(role.getCollisionBound().getMinX(), role.getCollisionBound().getMinY());
		end = Vec2(role.getCollisionBound().getMinX(), role.getCollisionBound().getMinY() - sensorRange.height);
		break;

	}
}
//Բ��
TrikerCircle::TrikerCircle(const Ract& rect, const float& radius) :radius(radius), circle(rect)
{
}

TrikerCircle::TrikerCircle(const Size& range, const MovingDirection& md) : Triker(range, md) {};
TrikerCircle::TrikerCircle() {};
TrikerCircle::~TrikerCircle() {};
bool& TrikerCircle::scanTouching(Tasnal& entityRact)
{
	//distance = Fand::getRange(entityRact, circle);
	//isTouched = entityRact.intersectsCircle(Vec2(circle.getMidX(), circle.getMidY()), radius == 0 ? circle.size.width / 2 : radius);

	return isTouched;
}

void TrikerCircle::render(DrawNode* draw)
{
	draw->drawCircle(Vec2(circle.getMidX(), circle.getMidY()), circle.size.width / 2, 360, 12, true, getColor());
}
void TrikerCircle::updateRegion(Tasnal& role, const float& radius)
{
	switch (sensorDirection)
	{
	case toTop:
		circle = Ract(Vec2(role.getCollisionBound().getMinX(), role.getCollisionBound().getMaxY()), sensorRange);
		break;
	case toBottom:
		circle = Ract(Vec2(role.getCollisionBound().getMinX(), role.getCollisionBound().getMinY() - sensorRange.height), sensorRange);
		break;
	case toLeft:
		circle = Ract(Vec2(role.getCollisionBound().origin.x - sensorRange.width, role.getCollisionBound().getMinY()), sensorRange);
		break;
	case toRight:
		circle = Ract(Vec2(role.getCollisionBound().getMaxX(), role.getCollisionBound().getMinY()), sensorRange);
		break;
	}
};
//����

TrikerRact::TrikerRact(const Ract& rect, const bool& isAct) :rect(rect)
{

}

TrikerRact::TrikerRact(const Size& range, const MovingDirection& md) : Triker(range, md) {};
TrikerRact::TrikerRact() {};
TrikerRact::~TrikerRact() {}

bool& TrikerRact::scanTouching(Tasnal& entityRact)
{
	auto bound = entityRact.getCollisionBound();
	distance = Fand::getRange(bound, rect);
	isTouched = false;
	//if (distance >= RangeType::rangeSensed)
	if (distance < RangeType::rangeThreatened)
		return isTouched;

	isTouched = bound.intersectsRact(rect);

	return isTouched;
}

void TrikerRact::render(DrawNode* draw)
{
	draw->drawRact(rect.origin, Vec2(rect.getMaxX(), rect.getMaxY()), getColor());
}

void TrikerRact::updateRegion(Tasnal& role, const float& radius)
{
	auto boundBox = role.getCollisionBound();

	switch (sensorDirection)
	{
	case toTop:
		rect = Ract(Vec2(boundBox.getMinX(), boundBox.getMaxY()), sensorRange);
		break;
	case toBottom:
		rect = Ract(Vec2(boundBox.getMinX(), boundBox.getMinY() - sensorRange.height), sensorRange);
		break;
	case toLeft:
		rect = Ract(Vec2(boundBox.origin.x - sensorRange.width, boundBox.getMinY()), sensorRange);
		break;
	case toRight:
		rect = Ract(Vec2(boundBox.getMaxX(), boundBox.getMinY()), sensorRange);
		break;
	}
}