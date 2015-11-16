//
//  BloodBar.hpp
//  BeatQuest
//
//  Created by chenbingfeng on 15/11/5.
//
//

#ifndef BloodBar_hpp
#define BloodBar_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "QuestProtocals.hpp"
#include "PixelNode.h"
#include "PixelTextNode.hpp"


//血条 及攻击力,pwr

class BloodBar:public BloodBarProtocal
{
public:

    void init(cocos2d::Layer* mainLayer, cocos2d::Camera* mainCamera);


    // 包括敌对双方的战斗信息，血条，攻击力min-max，Hero独有的pwr(？连击攻击力)，血条及血量，攻击力等数值变化的表现
    // 战时的数值信息：加血，少血，加战斗力，加连击力及其图形表现不在此。

    virtual void op_hero_configBlood(int bloodMax, int blood, bool direct = false);
    virtual void op_hero_configAttack(int attack_min, int attack_max, int pwr, bool direct = false);

    virtual void op_rival_configHead(int which); //配置敌人头像
    virtual void op_rival_configBlood(int bloodMax, int blood, int cnt = 1, bool direct = false); //cnt 1-5，血条数目 
    virtual void op_rival_reset();//恢复 rival 的信息到?状态

    virtual void op_rival_configAttack(int attack_min, int attack_max, bool direct = false);

    virtual void op_show();
    virtual void op_dismiss();


    virtual void op_configCombo(float radio); //设置 combo 进度
    virtual void op_toastComboFull(); //满了后闪亮一下，出来一个 combo 攻击
    virtual void op_toastComboReset(); //断连后红一下，表示事务了

protected:
    const float blood_change_time = 0.3f;
    const float blood_change_time_over_level = 0.3f;
    cocos2d::Layer* _mainLayer;
    cocos2d::Camera* _mainCamera;

    cocos2d::Sprite3D* _heroHubNode;
    PixelNode* _pxHeroOutline; //血条外框，包括红心
    PixelNode* _pxHeroBlood; //血条
    PixelNode* _pxHeroIcons; //攻击力图标,连击力图标
//    cocos2d::Label* _lbHeroAttack; //攻击力数值min-max
//    cocos2d::Label* _lbHeroPower; //连击力数值
//    cocos2d::Label* _lbHeroBlood; //血值(current/max)


    int _rivalHeadIndex = 0;
    cocos2d::Node* _rivalHubNode;
    PixelNode* _pxRivalOutline;
    PixelNode* _pxRivalHead;
    PixelNode* _pxRivalIcons;
    PixelNode* _pxRivalBloods[5]; //5中色彩
    int _oldRivalBloodIndex = 0;

    PixelTextNode* _pxtHeroBloodNum;
    PixelTextNode* _pxtRivalBloodNum;
    PixelTextNode* _pxtHeroAttackPwr;
    PixelTextNode* _pxtRivalAttack;


    PixelNode* _pxComboRect;
    PixelNode* _pxComboContent;
    float _comboRadio = 0;

};



#endif /* BloodBar_hpp */
