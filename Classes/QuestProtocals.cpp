// (C) 2015 arisecbf


#include "QuestProtocals.hpp"
#include "QuestDef.hpp"

float heroPositionType2floatYposition(HeroPositionType positionType)
{
    const float start_pos = QuestDef::HUNTING_X_HEROPOS_BEGIN;
    const float pos_step = QuestDef::HUNTING_X_HEROPOS_STEP;
    const float out_pos_n = 5;
    float r;
    switch (positionType) {
        case HeroPositionType::HPT_3:
            r = start_pos;
            break;
        case HeroPositionType::HPT_2:
            r = start_pos + pos_step;
            break;
        case HeroPositionType::HPT_1:
            r = start_pos + 2*pos_step;
            break;
        case HeroPositionType::HPT_0:
            r = start_pos + 3*pos_step;
            break;
        case HeroPositionType::HPT_OUT:
            r = start_pos - out_pos_n*pos_step;
            break;

        default:
            assert(false);
            break;
    }
    return r;
    
}

HuntingArrowType huntingHeroType2huntingArrowType(HeroType heroType, int grade)
{
    assert(grade >= 0 && grade <= 2);
    switch (heroType) {
        case HeroType::HT_META:
            return grade == 0 ? HuntingArrowType::META_0 :
            grade == 1 ? HuntingArrowType::META_1 : HuntingArrowType::META_2;
            break;

        case HeroType::HT_SLOW_DOWN:
            return grade == 0 ? HuntingArrowType::SLOW_0 :
            grade == 1 ? HuntingArrowType::SLOW_1 : HuntingArrowType::SLOW_2;
            break;

        case HeroType::HT_MULTI_ATTACK:
            return grade == 0 ? HuntingArrowType::MULTI_0 :
            grade == 1 ? HuntingArrowType::MULTI_1 : HuntingArrowType::MULTI_2;
            break;

        case HeroType::HT_HIGH_ATTACK:
            return grade == 0 ? HuntingArrowType::HIGH_0 :
            grade == 1 ? HuntingArrowType::HIGH_1 : HuntingArrowType::HIGH_2;
            break;

        case HeroType::HT_BOMB_ATTACK:
            return grade == 0 ? HuntingArrowType::BOMB_0 :
            grade == 1 ? HuntingArrowType::BOMB_1 : HuntingArrowType::BOMB_2;
            break;

        default:
            assert(false);
            break;
    }
}