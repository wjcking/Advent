#include "LuaField.h"
using namespace cocos2d;
FontInfo::FontInfo(LuaRef ref)
{
	if (!ref.has(Luaf_Font))
		return;
	if (ref[Luaf_Font].value().has(1))
		size = ref[Luaf_Font].value()[1].value<float>();

	auto colorIndex = ref[Luaf_Font].value().has(2) ? ref[Luaf_Font].value()[2].value<int>() : 0;
	
	isBold = ref[Luaf_Font].value().has(3) ? ref[Luaf_Font].value()[3].value<bool>() : false;
};

Luaf::Luaf(LuaRef ref)
{
	if (ref.has(Luaf_Pos))
		position = ref[Luaf_Pos].value<Vec2>();
	//Luaf_Name))
		name = ref[Luaf_Name].value<std::string>();
	//Luaf_Delay))
		delay = ref[Luaf_Delay].value<float>();
	//Luaf_ZOrder))
		zorder = ref[Luaf_ZOrder].value<int>();
	//Luaf_Duration))
		duration = ref[Luaf_Duration].value<int>();
}

CurtainInfo::CurtainInfo(LuaRef ref) : Luaf(ref)
{
	//Luaf_Type))
		type = ref[Luaf_Type].value<CurtainType>();
	//Luaf_Image))
	
	//	auto imageCount = ref.get(Luaf_Image).len();
		if (type == CurtainType::updown || type == CurtainType::black)
			image = ref.get(Luaf_Image).toValue<std::string>();
		else  
		{ 
			image1 = ref.get(Luaf_Image).get(1).toValue<std::string>();
			image2 = ref.get(Luaf_Image).get(2).toValue<std::string>();
		}
	
	font = FontInfo(ref);
}

 
