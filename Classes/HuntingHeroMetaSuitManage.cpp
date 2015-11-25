//
//  HuntingHeroMetaSuitManage.cpp
//  hunters
//
//  Created by chenbingfeng on 15/11/25.
//
//

#include "HuntingHeroMetaSuitManage.hpp"
HuntingHeroMetaSuitManage HuntingHeroMetaSuitManage::_instance;


int HuntingHeroMetaSuitManage::borrow()
{
    int r = _suits.front();
    _suits.pop_front();
    return r;
}
void HuntingHeroMetaSuitManage::giveback(int suit)
{
    _suits.push_back(suit);
}

void HuntingHeroMetaSuitManage::reset()
{
    _suits = {0,1,2,3};
}