//
//  ProgressInfo.hpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/8.
//
//

#ifndef ProgressInfo_hpp
#define ProgressInfo_hpp

#include <stdio.h>


class ProgressInfo
{
public:
    static ProgressInfo* s() {return & _instance;}

    void load(); //从持久化中读出已有的进度数据
    void save(); //保存当前的进度数据到持久化中
    void reset(); //新游戏时需要先重置进度及hero的数据

    bool hasProgress() { return _hasProgress; } //获取是否有历史记录

    int getLevelIndex() { return _levelIndex; }
    int getInitBlood() { return _levelInitBlood; }
    int getInitBloodMax() { return _levelInitBloodMax; }
    int getInitAttackMin() { return _levelInitAttackMin; }
    int getInitAttackMax() { return _levelInitAttackMax; }
    int getInitPower() { return _levelInitPower; }

    void saveLevelResult(int blood, int bloodMax, int attackMin, int attackMax, int power); //在关卡BOSS打败之后调用，来保持进度，levelIndex会自动加1，会立即持久化

protected:

    bool _hasProgress; //是否有进度

    
    int _levelIndex; //所在关卡
    // 关卡初始时Hero的数据
    int _levelInitBlood;
    int _levelInitBloodMax;
    int _levelInitAttackMin;
    int _levelInitAttackMax;
    int _levelInitPower;

    static ProgressInfo _instance;
};

#endif /* ProgressInfo_hpp */
