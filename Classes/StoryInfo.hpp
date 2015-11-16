//
//  StoryInfo.hpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/8.
//
//

#ifndef StoryInfo_hpp
#define StoryInfo_hpp

#include <stdio.h>



// 定义关卡信息
// 各关卡的 怪-》怪-》怪—》怪-》》》》》-》BOSS的内容及配置信息
// 场景的配置信息
// 只是数据没有任何运算

struct RivalInfoConfig
{
    int rivalId; //用来获取rival的图形
    int bloodMax;
    int attackMin;
    int attackMax;
    int bloodLineCnt;
};

class StoryInfo
{
public:
    static StoryInfo* s(){ return &_instance; }
    void loadConfig(); //加载数据
    std::vector<RivalInfoConfig>& getLevelConfig(int levelIndex); //获取某个关卡的rival配置
    int getLevelMax(); //获取总的关卡数目

protected:
    static StoryInfo _instance;

    std::vector<std::vector<RivalInfoConfig>> _storyConfig;
};

#endif /* StoryInfo_hpp */
