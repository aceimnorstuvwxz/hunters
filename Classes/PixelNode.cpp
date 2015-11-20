// (C) 2015 Turnro.com

#include "PixelNode.h"
#include "PixelDataCache.hpp"

USING_NS_CC;

bool PixelNode::init()
{
    assert(Node::init());

    if (_programState == nullptr) {
        prepareVertexData();
        prepareShaders();
    }

    return true;
}



void PixelNode::prepareVertexData()
{

    _count = 0;
    /*
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        glGenVertexArrays(1, &_vao);
        GL::bindVAO(_vao);
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(SimplePixelVertexPormat)*_count, _vertexData, GL_STREAM_DRAW);
        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PixelVertexPormat), (GLvoid *)offsetof(PixelVertexPormat, position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PixelVertexPormat), (GLvoid *)offsetof(PixelVertexPormat, color));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(PixelVertexPormat), (GLvoid *)offsetof(PixelVertexPormat, normal));

        GL::bindVAO(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }
    else
    {
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(SimplePixelVertexPormat)*_count, _vertexData, GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    CHECK_GL_ERROR_DEBUG();*/
}

void PixelNode::prepareShaders()
{
    auto fileUtils = FileUtils::getInstance();
    auto vertSource = fileUtils->getStringFromFile("3d/pixel.vsh");
    auto fragSource = fileUtils->getStringFromFile("3d/pixel.fsh");

    auto glprogram = GLProgram::createWithByteArrays(vertSource.c_str(), fragSource.c_str());
    glprogram->bindAttribLocation("a_positioin", 0);
    glprogram->bindAttribLocation("a_color", 1);
    glprogram->bindAttribLocation("a_normal2", 2);
    glprogram->bindAttribLocation("a_posdiff", 3);
    glprogram->bindAttribLocation("a_seed", 4);

    glprogram->link();
    glprogram->updateUniforms();

    _programState = GLProgramState::getOrCreateWithGLProgram(glprogram);
    _programState->retain();
}



void PixelNode::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    _command.init(_globalZOrder, transform, flags);
    _command.func = CC_CALLBACK_0(PixelNode::onDraw, this, transform, flags);
    renderer->addCommand(&_command);
}

void PixelNode::onDraw(const cocos2d::Mat4 &transform, uint32_t flags)
{

    auto glProgram = _programState->getGLProgram();
    glProgram->use();


    auto loc = glProgram->getUniformLocation("u_cbf_opacity");
    glProgram->setUniformLocationWith1f(loc, _opacityAsAlpha ? getOpacity()/255.f : 1.f);
    loc = glProgram->getUniformLocation("u_posexpand");
    glProgram->setUniformLocationWith1f(loc, _opacityAsPosExpand ? _posexpand*(1.f - getOpacity()/255.f) : 0.f);
    loc = glProgram->getUniformLocation("u_mixcolor");
    glProgram->setUniformLocationWith4fv(loc, &(_mixColor.x), 1);

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
    // shadow cover打开depth test同时在fsh中对a为0的进行discard，以保证重合交叠处不会交叠而加深。
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
    if (_stencil) {

        glEnable(GL_STENCIL_TEST);
        // Draw floor
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0xFF); // Write to stencil buffer
        glDepthMask(GL_FALSE); // Don't write to depth buffer
        glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
    }
    if (_stenciled) {

        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
        glStencilMask(0x00); // Don't write anything to stencil buffer
        glDepthMask(GL_TRUE); // Write to depth buffer
    }

    glDrawArrays(GL_TRIANGLES, 0, _count);


    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(0);
    }
    glDisable(GL_STENCIL_TEST);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(false);
    glEnable (GL_BLEND);

    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,_count);
    CHECK_GL_ERROR_DEBUG();
}

/*
void PixelNode::config(const std::vector<PixelUnit>& data)
{
    _count = 0;

    auto _vertexData = (PixelVertexPormat*)malloc(sizeof(PixelVertexPormat)*36*data.size());

    // 8 point
    const float half_step = 0.5f;

    cocos2d::Vec3 cornors[8] = {{1,1,1}, {-1,1,1}, {-1,-1,1}, {1,-1,1}, {1,1,-1}, {-1,1,-1}, {-1,-1,-1}, {1,-1,-1}};

    auto genface = [this, &cornors, half_step, &_vertexData](int a, int b, int c, int d, const Color3B& color, const cocos2d::Vec3& relativePos, const Vec3& normal2){
        _vertexData[_count].position = cornors[a] * half_step + relativePos;
        _vertexData[_count].normal = normal2;
        _vertexData[_count++].color = {color.r/255.f, color.g/255.f, color.b/255.f};

        _vertexData[_count].position = cornors[b] * half_step + relativePos;
        _vertexData[_count].normal = normal2;
        _vertexData[_count++].color = {color.r/255.f, color.g/255.f, color.b/255.f};

        _vertexData[_count].position = cornors[c]  * half_step + relativePos;
        _vertexData[_count].normal = normal2;
        _vertexData[_count++].color = {color.r/255.f, color.g/255.f, color.b/255.f};

        _vertexData[_count].position = cornors[a] * half_step + relativePos;
        _vertexData[_count].normal = normal2;
        _vertexData[_count++].color = {color.r/255.f, color.g/255.f, color.b/255.f};

        _vertexData[_count].position = cornors[c] * half_step + relativePos;
        _vertexData[_count].normal = normal2;
        _vertexData[_count++].color = {color.r/255.f, color.g/255.f, color.b/255.f};

        _vertexData[_count].position = cornors[d] * half_step + relativePos;
        _vertexData[_count].normal = normal2;
        _vertexData[_count++].color = {color.r/255.f, color.g/255.f, color.b/255.f};
    };

    for (auto pix : data) {
        cocos2d::Vec3 relativePos = {static_cast<float>(pix.pos.x), static_cast<float>(pix.pos.y), 0.f};
        genface(0,1,2,3,pix.color, relativePos, {0,0,1});
        genface(4,0,3,7,pix.color, relativePos, {1,0,0});
        genface(5,4,7,6,pix.color, relativePos, {0,0,-1});
        genface(1,5,6,2,pix.color, relativePos, {-1,0,0});
        genface(4,5,1,0,pix.color, relativePos, {0,1,0});
        genface(6,7,3,2,pix.color, relativePos, {0,-1,0});
    }

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PixelVertexPormat)*_count, _vertexData, GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    free(_vertexData);
}*/


void PixelNode::configBatch(const std::vector<PixelBatchTuple>& data)
{
    _count = 0;
    int totalSize = 0;
    for (auto& p : data) {
        totalSize += p.pixels->size();
    }

    auto _vertexData = (PixelVertexPormat*)malloc(sizeof(PixelVertexPormat)*36*totalSize);

    // 8 point
    const float half_step = 0.5f;

    cocos2d::Vec3 cornors[8] = {{1,1,1}, {-1,1,1}, {-1,-1,1}, {1,-1,1}, {1,1,-1}, {-1,1,-1}, {-1,-1,-1}, {1,-1,-1}};

    auto genface = [this, &cornors, half_step, &_vertexData](int a, int b, int c, int d, const Color4B& color, const cocos2d::Vec3& relativePos, const Vec3& normal2, float scale, float seed){
        _vertexData[_count].position = cornors[a] * half_step * scale + relativePos;
        _vertexData[_count].normal = normal2;
        _vertexData[_count].seed = seed;
        _vertexData[_count++].color = {color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f};

        _vertexData[_count].position = cornors[b] * half_step * scale+ relativePos;
        _vertexData[_count].normal = normal2;
        _vertexData[_count].seed = seed;
        _vertexData[_count++].color = {color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f};

        _vertexData[_count].position = cornors[c]  * half_step * scale + relativePos;
        _vertexData[_count].normal = normal2;
        _vertexData[_count].seed = seed;
        _vertexData[_count++].color = {color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f};

        _vertexData[_count].position = cornors[a] * half_step * scale + relativePos;
        _vertexData[_count].normal = normal2;
        _vertexData[_count].seed = seed;
        _vertexData[_count++].color = {color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f};

        _vertexData[_count].position = cornors[c] * half_step * scale + relativePos;
        _vertexData[_count].normal = normal2;
        _vertexData[_count].seed = seed;
        _vertexData[_count++].color = {color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f};

        _vertexData[_count].position = cornors[d] * half_step * scale + relativePos;
        _vertexData[_count].normal = normal2;
        _vertexData[_count].seed = seed;
        _vertexData[_count++].color = {color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f};
    };

    for (auto& p : data) {
        for (auto& pix : *p.pixels) {
            cocos2d::Vec3 relativePos = p.position + Vec3{static_cast<float>(pix.pos.x), static_cast<float>(pix.pos.y), 0.f} * p.scale;
            float seed = rand_0_1();
            genface(0,1,2,3,pix.color, relativePos, {0,0,1}, p.scale, seed);
            genface(4,0,3,7,pix.color, relativePos, {1,0,0}, p.scale, seed);
            genface(5,4,7,6,pix.color, relativePos, {0,0,-1}, p.scale, seed);
            genface(1,5,6,2,pix.color, relativePos, {-1,0,0}, p.scale, seed);
            genface(4,5,1,0,pix.color, relativePos, {0,1,0}, p.scale, seed);
            genface(6,7,3,2,pix.color, relativePos, {0,-1,0}, p.scale, seed);
        }
    }
    if (!_batched) {
        glGenVertexArrays(1, &_vao);
        cocos2d::GL::bindVAO(_vao);
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PixelVertexPormat), (GLvoid *)offsetof(PixelVertexPormat, position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(PixelVertexPormat), (GLvoid *)offsetof(PixelVertexPormat, color));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(PixelVertexPormat), (GLvoid *)offsetof(PixelVertexPormat, normal));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(PixelVertexPormat), (GLvoid *)offsetof(PixelVertexPormat, seed));


        cocos2d::GL::bindVAO(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        CCLOG("pixel batch new VAO=%d VBO=%d", _vao, _vbo);
    }


    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PixelVertexPormat)*_count, _vertexData, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    free(_vertexData);

    _batched = true;
}

void PixelNode::configSopx(const std::string& file)
{
    auto ab = PixelDataCache::s()->get(file);
    _vao = ab.vao;
    _vbo = ab.vbo;
    _count = ab.count;
}

PixelNode::~PixelNode(){
    if (_batched) {
        glDeleteBuffers(1, &_vbo);
        _vbo = 0;

        glDeleteVertexArrays(1, &_vao);
        cocos2d::GL::bindVAO(0);
        _vao = 0;
    }
}

void PixelNode::configMixColor(const cocos2d::Vec4& mixColor) //设置叠色，将用alpha与原色混合。默认alpha为0，显示原色。

{
    _mixColor = mixColor;
}

void PixelNode::configMixColorAni(const cocos2d::Vec4& mixColor, float fadeInOutTime, int repeat) //混色动画，用来实现，rival受攻击
{
    _mixColor = {mixColor.x, mixColor.y, mixColor.z, 0};
    _aniMixColorTarget = mixColor;
    _aniMixColorTime = repeat;
    _aniMixColorAlphaStep = mixColor.w*2.f/fadeInOutTime;
    scheduleUpdate();
}

void PixelNode::update(float dt)
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
