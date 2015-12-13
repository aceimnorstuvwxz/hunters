//
//  ACSoundManage.hpp
//  hunters
//
//  Created by chenbingfeng on 15/12/4.
//
//

#ifndef ACSoundManage_hpp
#define ACSoundManage_hpp

#include <stdio.h>


class ACSoundManage
{
public:
    enum SoundName{
        SN_ARROW_NORMAL_HIT,
        SN_ADD_HERO,
        SN_ARROW_SHOOT,
        SN_LASER_HIT,
        SN_GOLD_COST,
        SN_CROW_CRY,
        SN_HIT_SHIELD,
        SN_THUNDER_HIT,
        SN_NEW_WAVE,
        SN_NEW_HERO,
        SN_HERO_UPGRADE,
        SN_TORNADO,
//        SN_WIND_CHANGE,
        SN_KEY,
        SN_M_0,
        SN_M_1,
        SN_M_2,
        SN_M_3,
        SN_M_WELCO,
        SN_M_CREDITS,
    };
    static ACSoundManage* s() { return &_instance; }

    void load();
    void play(int SN_NAME, bool loop = false);
    void stop(int SN_NAME);

protected:

    static ACSoundManage _instance;

    std::unordered_map<int, std::string> _effectsMap;
    std::unordered_map<int, std::string> _musicMap;
    std::unordered_map<int, int> _soundIdMap;

};
#endif /* ACSoundManage_hpp */
