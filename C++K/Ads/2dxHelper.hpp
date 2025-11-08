//MOB^ON 2016/12/15
//Custom Event
#define INIT_SUCCESS "InitSuccess"
#define INIT_FAILED  "InitFailed"

#define PRELOAD_SUCCESS "PreloadSuccess"
#define PRELOAD_FAILED  "PreloadFailed"
#define CPAD_LOADED     "CPADLoaded"
#define VIDEOAD_LOADED  "VideoADLoaded"

#define AD_SHOW_SUCCESS "ShowSuccess"
#define AD_SHOW_FAILED  "ShowFailed"
#define AD_COMPLETE     "ADComplete"
#define AD_CLICK        "ADClick"
#define AD_CLOSE        "ADClose"

#define REWARD_SUCCESS "ADAwardSuccess"
#define REWARD_FAILED  "ADAwardFailed"

#define BANNER_LOADED  "BannerLoaded"
#define BANNER_FAILED  "BannerFailed"
#define BANNER_CLICK   "BannerClick"
#define BANNER_CLOSE   "BannerClose"

#define GUSER_NON_PAYING_USER      "NON_PAYING_USER"
#define NGUSER_SMALL_PAYMENT_USER  "SMALL_PAYMENT_USER"
#define NGUSER_MEDIUM_PAYMENT_USER "MEDIUM_PAYMENT_USER"
#define NGUSER_LARGE_PAYMENT_USER  "LARGE_PAYMENT_USER"

#define BANNER_TYPE_NORMAL "BannerNormal"
#define BANNER_TYPE_MEDIUM "BannerMediumRactangle"
#define BANNER_TYPE_LARGE  "BannerLarge"

#include "cocos2d.h"

#if ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    static const char* Yomob_Interstitial = "0vu9o4U0TMre0GSdymm";
    static const char* Yomob_Video = "3cQscE25xYL9RaFhTuP";
    static const char* Yomob_Banner = "vJ9SzC5MwlQ6vB9gv9S";
    static const char* Yomob_Reward = "jbs7IK7r8cQyoTy4KnQ";
#else
static const char* Yomob_Interstitial = "0vu9o4U0TMre0GSdymm";
    static const char* Yomob_Video = "3cQscE25xYL9RaFhTuP";
    static const char* Yomob_Banner = "vJ9SzC5MwlQ6vB9gv9S";
    static const char* Yomob_Reward = "jbs7IK7r8cQyoTy4KnQ";
#endif

namespace yomob {
    typedef enum {
        NonPayingUser,
        SmallPaymentUser,
        MediumPaymentUser,
        LargePaymentUser
    } PayingUser;
    class SDKDelegate {
    public:
        virtual void onInitSuccess(const std::string ret){};
        virtual void onInitFailed(const std::string ret){};
    };

    class PreloadDelegate {
    public:
        virtual void onPreloadSuccess(const std::string ret){};
        virtual void onPreloadFailed(const std::string ret){};
        virtual void onCPADLoaded(const std::string ret){};
        virtual void onVideoADLoaded(const std::string ret){};
    };

    class ADDelegate {
    public:
        virtual void onShowSuccess(const std::string ret){};
        virtual void onShowFailed(const std::string ret){};
        virtual void onADComplete(const std::string ret){};
        virtual void onADClick(const std::string ret){};
        virtual void onADClose(const std::string ret){};
    };

    class RewardDelegate {
    public:
        virtual void onADAwardSuccess(const std::string ret){};
        virtual void onADAwardFailed(const std::string ret){};
    };

    class BannerDelegate {
    public:
        virtual void onBannerLoaded(const std::string scene, const std::string ret){};
        virtual void onBannerFailed(const std::string scene, const std::string ret const std::string error){};
        virtual void onBannerClick(const std::string scene, const std::string ret){};
        virtual void onBannerClose(const std::string scene, const std::string ret){};
    }
    class Helper {
    public:
        static void setDebugModel(bool debug);
        
        static void setSDKConfig(const std::string key, const std::string val);
        static std::string getSDKConfig(const std::string key);
        
        static void initialize(void);
        static void initialize(const std::string appid);
        static void initialize(const std::string appid, const std::string channelid);
        
        static int isWIFI();

        static void preload();

        static int getIntParameterFromAdHezk(const std::string scene, const std::string key, int def = 0);
        static float getFloatParameterFromAdHezk(const std::string scene, const std::string key, float def = 0);
        static std::string getStringParameterFromAdHezk(const std::string scene, const std::string key, const std::string def = "");
        static void setBannerConfig(const std::string scene, const std::string type, float x, float y, float width, float height, int interval);
        
        static bool couldShowAd(const std::string scene);
        static void showAd(const std::string scene);
        static void showTestView(const std::string scene);
        static void closeBanner(const std::string scene);

        static void reportAdRejected(const std::string scene);
        static void showAdHezk(const std::string scene);

        static void sendCounter(const std::string name, const std::string metaData);

        static void tagPayingUser(yomob::TGSDKCocosedxPayingUser user, const std::string currency, float currentAmount, float totalAmount);

        static std::string getUserGDPRConsentStatus();
        static void setUserGDPRConsentStatus(const std::string status);

        static std::string getIsAgeRestrictedUser();
        static void setIsAgeRestrictedUser(const std::string status);

        static void setSDKDelegate(SDKDelegate *delegate);
        static void setPreloadDelegate(PreloadDelegate *delegate);
        static void setADDelegate(ADDelegate *delegate);
        static void setRewardDelegate(RewardDelegate *delegate);
        static void setBannerDelegate(BannerDelegate *delegate);
        static void handleEvent(const std::string event, const std::string result);
        
        static void bindScript(void);
    };
};

void Yomob::SetRequirePath(const char* path)
{
    luaL_openlibs(l);
    lua_getglobal(l, "package");
    lua_getfield(l, -1, "path"); // get field "path" from table at top of stack (-1)
    std::string cur_path = lua_tostring(l, -1); // grab path string from top of stack
    cur_path.append(";"); //your path here
    cur_path.append(path);
    cur_path.append("?.lua");//�����������lua�ű����������
    lua_pop(l, 1); // get rid of the string on the stack we just pushed on line 5
    lua_pushstring(l, cur_path.c_str()); // push the new one
    lua_setfield(l, -2, "path"); // set the field "path" in table at -2 with value at top of stack
    lua_pop(l, 1); // get rid of package table from top of stack
}