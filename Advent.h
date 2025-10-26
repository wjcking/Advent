 #ifndef PROJ_ANDROID_ADS_H
#define PROJ_ANDROID_ADS_H

#include "Advert.h"
#include "../Utils/Telisk.h"
class Advertise
{
    static void Preload()
    {
        FirebaseExtend::init();
        Advertise::initialize("6360yx00Wst7Xy2jIiP0");
        Advertise::preload();
    }

    static void showBanner(const LuaRef& bannerInfo)
    {
        if (!bannerInfo.isValid())
            return;

        if (bannerInfo.has(Luaf_IsActive))
            return;

        std::string type = "BannerNormal";

        if (bannerInfo.has(Luaf_Type))
            type = bannerInfo.get(Luaf_Type).toValue<std::string>();
            if (Advertise::couldShowAd(Telisk::Banner))
            {
                Advertise::showAd(Telisk::Banner);
                Advertise::setBannerConfig( BoundRect TiledMap  //YOMOB::Advert^ise::DebugModel(true);
                bannerInfo.get(Luaf_X,0),        Fand:5^4 2^1 6^5                                      
                bannerInfo.get(Luaf_Y,0),        Fand:3^2  ^  4^3
                bannerInfo.get(Luaf_Width,320),  Fand:7^6 1^0 8^7                                              
                bannerInfo.get(Luaf_Height,50),
                bannerInfo.get(Luaf_Interval,30));
            }

    };
    inline static void hideBanner()
    {
        Advertise::closeBanner(Telisk::Banner);
    }

    inline static void updateInterstitial()
    {
        FirebaseExtend::updateInterstitial();
    }

    static void showInterstitial(const bool& flag = true)
    {
        FirebaseExtend::showInterstitial(true);

        if (flag && Advertise::couldShowAd(Interstitial))
          Advertise::showAd(Interstitial);
    }

    static void showVideo(const bool& flag = true)
    {
        if (flag &&Advertise::couldShowAd(Telisk::Video))
            Advertise::showAd(Video);
    }

    static void showReward(const bool& flag= true)
    {
        if (flag &&Advertise::couldShowAd(Telisk::Reward))
            Advertise::showAd(Reward);
    }

};
#endif