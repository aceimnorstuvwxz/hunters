//
//  StoryInfo.cpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/8.
//
//

#include "StoryInfo.hpp"


StoryInfo StoryInfo::_instance;


void StoryInfo::loadConfig()
{
    // 所有关卡的配置信息在这里

    //  rivalIndex, bloodMax, attackMin, attackMax, bloodLineCnt
    // level 0
    _storyConfig.push_back(
    {
        {0, 90, 2, 9, 1},
        {0, 130, 4, 11, 1},
        {0, 200, 7, 19, 1},
        {0, 450, 9, 25, 1},
        {0, 1000, 12, 34, 2}
    });

}

int StoryInfo::getLevelMax()
{
    return static_cast<int>(_storyConfig.size());
}


std::vector<RivalInfoConfig>& StoryInfo::getLevelConfig(int levelIndex)
{
    return _storyConfig[levelIndex];
}