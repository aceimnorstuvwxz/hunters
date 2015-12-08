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
    };
    static ACSoundManage* s() { return &_instance; }

    void load();
    void play(int SN_NAME);

protected:

    static ACSoundManage _instance;

    std::unordered_map<int, std::string> _effectsMap;
    std::unordered_map<int, std::string> _musicMap;

};
#endif /* ACSoundManage_hpp */
