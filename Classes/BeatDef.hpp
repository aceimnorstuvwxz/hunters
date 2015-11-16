// (C) 2015 Arisecbf

#ifndef BeatDef_hpp
#define BeatDef_hpp

#include <stdio.h>


enum class TestBeatType{
    BLOOD, //血
    SWORD, //剑
    SHIELD,//盾
    COMBO, //连击
};

inline std::string testbeattype2string(TestBeatType t)
{
    std::string  r;
    switch (t) {
        case TestBeatType::BLOOD:
            r = "BLOOD";
            break;

        case TestBeatType::SWORD:
            r = "SWORD";
            break;

        case TestBeatType::SHIELD:
            r = "SHIELD";
            break;

        case TestBeatType::COMBO:
            r = "COMBO";
            break;

        default:
            assert(false);
            break;
    }
    return r;
}

#endif /* BeatDef_hpp */
