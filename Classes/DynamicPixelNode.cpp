//
//  DynamicPixelNode.cpp
//  hunters
//
//  Created by chenbingfeng on 15/11/23.
//
//

#include "DynamicPixelNode.hpp"
#include "PixelDataCache.hpp"

USING_NS_CC;


cocos2d::GLProgramState* DynamicPixelNode::_programState = nullptr;

bool DynamicPixelNode::init()
{
    assert(Node::init());

    prepareVertexData();
    if (_programState == nullptr) {
        prepareShaders();
    }

    for (int i = 0; i < BONE_NUM_MAX; i++) {
        auto n = Node::create();
        this->addChild(n);
        _bones[i] = n;
    }

    _specialBonePreNode= Node::create();
    _specialBonePreNode->retain();

    return true;
}


void DynamicPixelNode::prepareVertexData()
{
    // 主要任务就是产生 VAO/VBO
    _count = 0;

     glGenVertexArrays(1, &_vao);
     GL::bindVAO(_vao);
     glGenBuffers(1, &_vbo);
     glBindBuffer(GL_ARRAY_BUFFER, _vbo);

     glBufferData(GL_ARRAY_BUFFER, sizeof(DynamicPixelVertexPormat) * _bufferStoreSize, nullptr, GL_STREAM_DRAW); //预支空间

     // position
     glEnableVertexAttribArray(0);
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DynamicPixelVertexPormat), (GLvoid *)offsetof(DynamicPixelVertexPormat, position));
     glEnableVertexAttribArray(1);
     glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(DynamicPixelVertexPormat), (GLvoid *)offsetof(DynamicPixelVertexPormat, color));
     glEnableVertexAttribArray(2);
     glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(DynamicPixelVertexPormat), (GLvoid *)offsetof(DynamicPixelVertexPormat, normal));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(DynamicPixelVertexPormat), (GLvoid *)offsetof(DynamicPixelVertexPormat, boneIndex));


     GL::bindVAO(0);
     glBindBuffer(GL_ARRAY_BUFFER, 0);

     CHECK_GL_ERROR_DEBUG();
}

void DynamicPixelNode::prepareShaders()
{
    auto fileUtils = FileUtils::getInstance();
    auto vertSource = fileUtils->getStringFromFile("3d/dynamic_pixel.vsh");
    auto fragSource = fileUtils->getStringFromFile("3d/dynamic_pixel.fsh");

    auto glprogram = GLProgram::createWithByteArrays(vertSource.c_str(), fragSource.c_str());
    glprogram->bindAttribLocation("a_positioin", 0);
    glprogram->bindAttribLocation("a_color", 1);
    glprogram->bindAttribLocation("a_cbf_normal", 2);
    glprogram->bindAttribLocation("a_bone_index", 3);

    glprogram->link();
    glprogram->updateUniforms();

    _programState = GLProgramState::getOrCreateWithGLProgram(glprogram);
    _programState->retain();
}



void DynamicPixelNode::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    _command.init(_globalZOrder, transform, flags);
    _command.func = CC_CALLBACK_0(DynamicPixelNode::onDraw, this, transform, flags);
    renderer->addCommand(&_command);
}

void DynamicPixelNode::onDraw(const cocos2d::Mat4 &transform, uint32_t flags)
{

    auto glProgram = _programState->getGLProgram();
    glProgram->use();

//
//    auto loc = glProgram->getUniformLocation("u_cbf_opacity");
//    glProgram->setUniformLocationWith1f(loc, _opacityAsAlpha ? getOpacity()/255.f : 1.f);
//    loc = glProgram->getUniformLocation("u_posexpand");
//    glProgram->setUniformLocationWith1f(loc, _opacityAsPosExpand ? _posexpand*(1.f - getOpacity()/255.f) : 0.f);
    auto loc = glProgram->getUniformLocation("u_mixcolor");
    glProgram->setUniformLocationWith4fv(loc, &(_mixColor.x), 1);
//
//    loc = glProgram->getUniformLocation("u_y_cut");
//    glProgram->setUniformLocationWith1f(loc, _yCut);

    // TODO bone matrix uniform array

    for (int i = 0; i < BONE_NUM_MAX; i++) {
        _boneMatrixs[i] = _bones[i]->getNodeToParentTransform();
    }
    if (_specialBoneIndex != -1) {
        _boneMatrixs[_specialBoneIndex] = _boneMatrixs[_specialBoneIndex]*_specialBonePreNode->getNodeToParentTransform();
    }
    loc = glProgram->getUniformLocation("u_bone_matrixs");
    glProgram->setUniformLocationWithMatrix4fv(loc, reinterpret_cast<float*>(_boneMatrixs), BONE_NUM_MAX);

    glProgram->setUniformsForBuiltins(transform);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(_vao);
    }
    else
    {
        // TODO
    }

    if (_blend) {
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glDisable(GL_BLEND);
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
    CHECK_GL_ERROR_DEBUG();

    glDrawArrays(GL_TRIANGLES, 0, _count);

    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(false);
    glEnable (GL_BLEND);

    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,_count);
    CHECK_GL_ERROR_DEBUG();
}

DynamicPixelNode::~DynamicPixelNode()
{
    glDeleteBuffers(1, &_vbo);
    _vbo = 0;

    glDeleteVertexArrays(1, &_vao);
    cocos2d::GL::bindVAO(0);
    _vao = 0;
}



void DynamicPixelNode::configAddSopx(const std::string& file, int boneIndex, cocos2d::Vec3 relativePosition, bool cutInnerFace , bool cutBackFace, cocos2d::Vec3 baseRotation) //加入某个 sopx 内的 vertex，并且设它的 Bone 为某个值
{
    assert(boneIndex >=0 && boneIndex < BONE_NUM_MAX);
    auto rawPixels = PixelDataCache::s()->getRawPixels(file);

    auto vertexData = (DynamicPixelVertexPormat*)malloc(sizeof(DynamicPixelVertexPormat)*36*rawPixels->size());

    // 8 point
    const float half_step = 0.5f;

    cocos2d::Vec3 cornors[8] = {{1,1,1}, {-1,1,1}, {-1,-1,1}, {1,-1,1}, {1,1,-1}, {-1,1,-1}, {-1,-1,-1}, {1,-1,-1}};
    int localCount = 0;
    Node* node = Node::create();
    node->setRotation3D(baseRotation);
    auto mat = node->getNodeToParentTransform();

    auto genface = [this, &cornors, half_step, &vertexData, &localCount, &mat](int a, int b, int c, int d, const Color4B& color, const cocos2d::Vec3& cubeCenterPos, const cocos2d::Vec3& relativePos, const Vec3& normal, int boneIndex){
        vertexData[localCount].position = cornors[a] * half_step + cubeCenterPos;
        mat.transformVector(&vertexData[localCount].position);
        vertexData[localCount].position += relativePos;
        vertexData[localCount].normal = normal;
        vertexData[localCount].boneIndex = boneIndex;
        vertexData[localCount++].color = {color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f};

        vertexData[localCount].position = cornors[b] * half_step + cubeCenterPos;
        mat.transformVector(&vertexData[localCount].position);
        vertexData[localCount].position += relativePos;
        vertexData[localCount].normal = normal;
        vertexData[localCount].boneIndex = boneIndex;
        vertexData[localCount++].color = {color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f};

        vertexData[localCount].position = cornors[c] * half_step + cubeCenterPos;
        mat.transformVector(&vertexData[localCount].position);
        vertexData[localCount].position += relativePos;
        vertexData[localCount].normal = normal;
        vertexData[localCount].boneIndex = boneIndex;
        vertexData[localCount++].color = {color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f};

        vertexData[localCount].position = cornors[a] * half_step + cubeCenterPos;
        mat.transformVector(&vertexData[localCount].position);
        vertexData[localCount].position += relativePos;
        vertexData[localCount].normal = normal;
        vertexData[localCount].boneIndex = boneIndex;
        vertexData[localCount++].color = {color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f};

        vertexData[localCount].position = cornors[c] * half_step + cubeCenterPos;
        mat.transformVector(&vertexData[localCount].position);
        vertexData[localCount].position += relativePos;
        vertexData[localCount].normal = normal;
        vertexData[localCount].boneIndex = boneIndex;
        vertexData[localCount++].color = {color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f};

        vertexData[localCount].position = cornors[d] * half_step + cubeCenterPos;
        mat.transformVector(&vertexData[localCount].position);
        vertexData[localCount].position += relativePos;
        vertexData[localCount].normal = normal;
        vertexData[localCount].boneIndex = boneIndex;
        vertexData[localCount++].color = {color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f};
    };


    for (auto& pix : *rawPixels) {
        cocos2d::Vec3 cubeCenterPos =  Vec3{static_cast<float>(pix.pos.x), static_cast<float>(pix.pos.y), 0.f};
        genface(0,1,2,3,pix.color, cubeCenterPos, relativePosition, {0,0,1}, boneIndex);//forward
        if (!cutBackFace) genface(5,4,7,6,pix.color, cubeCenterPos, relativePosition, {0,0,-1}, boneIndex);//back

        if (!cutInnerFace && !pix.R) genface(4,0,3,7,pix.color, cubeCenterPos, relativePosition, {1,0,0}, boneIndex);
        if (!cutInnerFace && !pix.L) genface(1,5,6,2,pix.color, cubeCenterPos, relativePosition, {-1,0,0}, boneIndex);
        if (!cutInnerFace && !pix.T) genface(4,5,1,0,pix.color, cubeCenterPos, relativePosition, {0,1,0}, boneIndex);
        if (!cutInnerFace && !pix.B) genface(6,7,3,2,pix.color, cubeCenterPos, relativePosition, {0,-1,0}, boneIndex);
    }

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    CCLOG("buffer store capatity=%d, used=%d", _bufferStoreSize, _count+localCount);
    assert((_count+localCount) <= _bufferStoreSize);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(DynamicPixelVertexPormat)*_count, sizeof(DynamicPixelVertexPormat)*localCount, vertexData);
    _count += localCount;
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void DynamicPixelNode::configAction(int boneIndex, cocos2d::Vec3 position, cocos2d::Vec3 rotation, float scaleX, float scaleY, cocos2d::Action* action, bool set) //为某个骨骼配置动作，将取消该骨骼之前配置的动作
{
    assert(boneIndex >=0 && boneIndex < BONE_NUM_MAX);
    auto node = _bones[boneIndex];
    if (set) {
        node->setPosition3D(position);
        node->setRotation3D(rotation);
        node->setScale(scaleX, scaleY);
    }
    node->stopAllActions();
    node->runAction(action);
}


void DynamicPixelNode::configMixColor(const cocos2d::Vec4& mixColor) //设置叠色，将用alpha与原色混合。默认alpha为0，显示原色。

{
    _mixColor = mixColor;
}

void DynamicPixelNode::configMixColorAni(const cocos2d::Vec4& mixColor, float fadeInOutTime, int repeat) //混色动画，用来实现，rival受攻击
{
    _mixColor = {mixColor.x, mixColor.y, mixColor.z, 0};
    _aniMixColorTarget = mixColor;
    _aniMixColorTime = repeat;
    _aniMixColorAlphaStep = mixColor.w*2.f/fadeInOutTime;
    scheduleUpdate();
}
void DynamicPixelNode::configClear() //清空 VBO
{
    _count = 0;
}

void DynamicPixelNode::update(float dt)
{
    if (_aniMixColorTime > 0 || _aniMixColorTime < 0) {
        _mixColor.w += _aniMixColorAlphaStep*dt;
        if (_aniMixColorAlphaStep > 0 && _mixColor.w > _aniMixColorTarget.w) {
            _aniMixColorAlphaStep = - _aniMixColorAlphaStep;
        } else if (_aniMixColorAlphaStep < 0 && _mixColor.w < 0) {
            _aniMixColorTime--;
            _mixColor.w = 0;
            _aniMixColorAlphaStep = - _aniMixColorAlphaStep;
        }
    }
}

void DynamicPixelNode::configSpecialDoubleBoneRotateFirst(int boneIndex, cocos2d::Vec3 rotateion) //弓的特殊要求!!
{
    _specialBonePreNode->removeFromParent();
    _bones[boneIndex]->addChild(_specialBonePreNode);
    _specialBonePreNode->setRotation3D(rotateion);
    _specialBoneIndex = boneIndex;
}
