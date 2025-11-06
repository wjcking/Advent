#pragma once
#include "../Utils/Telisk.h"
#include "../Utils/Constant.h"
#include "../../Tik/Encounter.h"
using namespace std;
#define TYPE_DIALOG Typewriter::getInstance()
static Typewriter* Typewriter = new Typewriter;

class Typewriter
{
private:
	DelayPhase delayType;
	unsigned int  pos = 0;
	string content;
public:

	//�������л���ʱ�����ִ��ֻ��ٶȲ�һ�µ�bug��������һ�������ٶ�
	Typewriter() :delayType(0.065f) {};
	inline void setDelay(const float& delay) { delayType.delaySecond = delay; }
	inline void setContent(const std::string& val) 
	{
		//�������Բ�ͬ��ʼ����ӡ�����ٶ�
		delayType.delaySecond = Typewriter::Typewriter->getIntegerForKey(User_Language, Lang_Chinwan) == 1 ? 0.12f : 0.05f; 
		content = val;
	}
	inline void reset() { pos = 0; }
	bool isDone(const bool& isClear = true)
	{
		if (pos < content.length())
		{
			//ֱ��ȫ����ȡ���
			if (isClear)
				pos = content.length();
			return false;
		}

		return true;
	}
	string& getTyped()
	{
		if (pos >= content.length() || content.length() == 0)
		{
			return content;
		}

		if (delayType.isTimeUp())
		{
			delayType.reset();
			//��Ӣ���ж�
			pos += (content[pos] > -127 && content[pos] < 0) ? 3 : 1;
			return content.substr(0, pos);
		}

		return content.substr(0, pos);
	}
};

struct HintText
{
	short zorder = Z_ROLE;
	short index = 0;
	int tag = 0;
	float delay = 3.f;
	//bool hasChoices = false;
	float scaleWidth = 20.f;
	string text;
	FontInfo font;
	void initScaleWith(const float& = 0);
	inline std::string getName() const { return cocos2d::StringUtils::format("dialog%d", index); }
};
 
struct TeliskText : HintText
{
	std::string	choices[4];
	cocos2d::Menu* getChoices(); /*|CCPA|*/
	void choiceCallback(cocos2d::Ref* pSender);
	inline bool isAnswered() { return LUAH->getGlobal(Luat_Telisk)[index].value().has(Luaf_Key); }
};

#define DIALOGUE Telisk::getInstance()
class Telisk
{
	Telisk();
 	static Telisk* getInstance()
 	{
 		static Telisk instance;
 		return &instance;
 	}

	private:
	//RObject
	//unordered_map<int, HintText> robjectMap;
	unordered_map<int, vector<HintText>>  hintMap;
	unordered_map<int, unsigned int> hintIter;
	unordered_map<int, DelayPhase>   delayHint;

	dictionary<int, DelayPhase>  dictionaryHint;

	vector<TeliskText> dialogueList;
	TeliskText defaultTelisk;
	unsigned int dialogIndex = 0;
	unsigned int dialogEnd = 0;
	unsigned int previousIndex = 0;

	public:

	void loadScript();
	//������ӫĻ����ʹ��
	void loadCurtainText();
	//gcc++ ���ñ���������  error: invalid initialization of reference of type 'TeliskText&'
	//from expression of type 'const value_type {aka const TeliskText}'
	TeliskText& getCurrentDialog();
	HintText fetchHint(const int&);

	inline bool isDone() { return dialogIndex == dialogEnd; }	
	//���÷ֶζԻ���Ĭ�ϴ�ͷ��β
	inline void setSegment(const unsigned int& start, const unsigned int& end)
	{
		dialogIndex = start > end ? end : start;
		dialogEnd = end < start ? start : end;
	}

	void fetchNext()
	{
		previousIndex = dialogIndex;
		if (dialogIndex < dialogEnd)
			dialogIndex++;
		else
			dialogIndex = dialogEnd;
	}

	//inline TeliskText& getPreviousDialog() { return dialogueList[previousIndex]; }
	inline void resetHint(const int& tag)
	{
		hintIter[tag] = 0;
		delayHint[tag].reset();
	}

};