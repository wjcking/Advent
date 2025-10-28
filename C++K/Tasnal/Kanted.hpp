comment(Tik.Encounter);

enum BorderLimited
{
	notLimited = 0,
	limiteHorizon = 1,
	limiteVertical = 2,
	limiteBoth = 3
};

enum class BodyStatus
{
	healthy,
	hurting,
	dead,
	fall
};
enum class TasnalType :unsigned short
{
	player = 1,
	npc,
	robject,
	projectTile
};
enum class Anistyle
{
	toast = 1,
	fade,
	rollOut,
	explosion,
	hovering,
	rebound,
	rotate,
	jump
};
enum SlopeDegree :unsigned short
{
	degree45 = 45,
	degree22_5 = 22
};