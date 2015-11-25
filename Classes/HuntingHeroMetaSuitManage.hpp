//
//  HuntingHeroMetaSuitManage.hpp
//  hunters
//
//  Created by chenbingfeng on 15/11/25.
//
//

#ifndef HuntingHeroMetaSuitManage_hpp
#define HuntingHeroMetaSuitManage_hpp

#include <stdio.h>


//第三方的管理基础的4中人物套装0,1,2,3
class HuntingHeroMetaSuitManage
{
public:
    static HuntingHeroMetaSuitManage* s(){return &_instance;}
    int borrow();
    void giveback(int suit);
    void reset();


protected:
    std::deque<int> _suits = {0,1,2,3};

    static HuntingHeroMetaSuitManage _instance;
};

#endif /* HuntingHeroMetaSuitManage_hpp */
