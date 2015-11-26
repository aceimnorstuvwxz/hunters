//
//  MoneyManager.hpp
//  hunters
//
//  Created by chenbingfeng on 15/11/26.
//
//

#ifndef MoneyManager_hpp
#define MoneyManager_hpp

#include "QuestProtocals.hpp"
//金币管理
class MoneyManager
{
public:
    static MoneyManager* s() { return &_instance; }
    void configProtocals(TopIconsProtocal* topIconsProtocal) {
        reset();
        _topIconsProtocal = topIconsProtocal;
        _topIconsProtocal->op_configGold(_gold);
    }
    void reset(){ //重设初始化，可能有初始化的金币数量
        _gold = 999;
    }
    void add(int gold){
        _gold += gold;
        _topIconsProtocal->op_configGold(_gold);
    }
    int get(){
        return _gold;
    }
    void cost(int gold){
        _gold -= gold; assert(_gold >= 0);
        _topIconsProtocal->op_configGold(_gold);
    }

protected:
    int _gold = 0;
    static MoneyManager _instance;
    TopIconsProtocal* _topIconsProtocal;
};

#endif /* MoneyManager_hpp */