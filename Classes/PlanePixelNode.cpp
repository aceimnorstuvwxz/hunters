// (C) 2015 Turnro.com

#include "PlanePixelNode.h"
#include "SOCommon.h"
USING_NS_CC;

bool PlanePixelNode::init()
{
    assert(Node::init());

    if (_programState == nullptr) {
        prepareVertexData();
        prepareShaders();
    }

    return true;
}



void PlanePixelNode::prepareVertexData()
{

    _count = 0;
}

void PlanePixelNode::prepareShaders()
{
    auto fileUtils = FileUtils::getInstance();
    auto vertSource = fileUtils->getStringFromFile("3d/plane_pixel.vsh");
    auto fragSource = fileUtils->getStringFromFile("3d/plane_pixel.fsh");

    auto glprogram = GLProgram::createWithByteArrays(vertSource.c_str(), fragSource.c_str());
    glprogram->bindAttribLocation("a_positioin", 0);
    glprogram->bindAttribLocation("a_color", 1);
    glprogram->bindAttribLocation("a_normal2", 2);

    glprogram->link();
    glprogram->updateUniforms();

    _programState = GLProgramState::getOrCreateWithGLProgram(glprogram);
    _programState->retain();
}



void PlanePixelNode::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    _command.init(_globalZOrder, transform, flags);
    _command.func = CC_CALLBACK_0(PlanePixelNode::onDraw, this, transform, flags);
    renderer->addCommand(&_command);
}

void PlanePixelNode::onDraw(const cocos2d::Mat4 &transform, uint32_t flags)
{

    auto glProgram = _programState->getGLProgram();
    glProgram->use();


    auto loc = glProgram->getUniformLocation("u_move_line");
    glProgram->setUniformLocationWith1f(loc, _moveLine);
    loc = glProgram->getUniformLocation("u_move_width");
    glProgram->setUniformLocationWith1f(loc, _moveWidth);

    loc = glProgram->getUniformLocation("u_x_diff");
    glProgram->setUniformLocationWith1f(loc, _baseXDiff_current + _varyXDiff* (getOpacity()/255.f-0.5f));

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
//    glEnable (GL_BLEND);
//    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
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


void PlanePixelNode::configBatch(const std::vector<PlanePixelBatchTuple>& data, bool cutBack)
{
    _count = 0;
    int totalSize = 0;
    for (auto& p : data) {
        totalSize += p.pixels->size();
    }

    auto _vertexData = (PlanePixelVertexPormat*)malloc(sizeof(PlanePixelVertexPormat)*36*totalSize);

    // 8 point
    const float half_step = 0.5f;

    cocos2d::Vec3 cornors[8] = {{1,1,1}, {-1,1,1}, {-1,-1,1}, {1,-1,1}, {1,1,-1}, {-1,1,-1}, {-1,-1,-1}, {1,-1,-1}};

    auto genface = [this, &cornors, half_step, &_vertexData](int a, int b, int c, int d, const Color3B& color, const cocos2d::Vec3& relativePos, const Vec3& normal2, float scale){
        _vertexData[_count].position = cornors[a] * half_step * scale + relativePos;
        _vertexData[_count].normal = normal2;
        _vertexData[_count++].color = {color.r/255.f, color.g/255.f, color.b/255.f};

        _vertexData[_count].position = cornors[b] * half_step * scale+ relativePos;
        _vertexData[_count].normal = normal2;
        _vertexData[_count++].color = {color.r/255.f, color.g/255.f, color.b/255.f};

        _vertexData[_count].position = cornors[c]  * half_step * scale + relativePos;
        _vertexData[_count].normal = normal2;
        _vertexData[_count++].color = {color.r/255.f, color.g/255.f, color.b/255.f};

        _vertexData[_count].position = cornors[a] * half_step * scale + relativePos;
        _vertexData[_count].normal = normal2;
        _vertexData[_count++].color = {color.r/255.f, color.g/255.f, color.b/255.f};

        _vertexData[_count].position = cornors[c] * half_step * scale + relativePos;
        _vertexData[_count].normal = normal2;
        _vertexData[_count++].color = {color.r/255.f, color.g/255.f, color.b/255.f};

        _vertexData[_count].position = cornors[d] * half_step * scale + relativePos;
        _vertexData[_count].normal = normal2;
        _vertexData[_count++].color = {color.r/255.f, color.g/255.f, color.b/255.f};
    };

    for (auto& p : data) {
        for (auto& pix : *p.pixels) {
            cocos2d::Vec3 relativePos = p.position + Vec3{static_cast<float>(pix.pos.x), static_cast<float>(pix.pos.y), 0.f} * p.scale;


            genface(0,1,2,3,color4b2color3b(pix.color), relativePos, {0,0,1}, p.scale); //正
            if (!cutBack) genface(5,4,7,6,color4b2color3b(pix.color), relativePos, {0,0,-1}, p.scale); //反
            if (!pix.R) genface(4,0,3,7,color4b2color3b(pix.color), relativePos, {1,0,0}, p.scale);
            if (!pix.L) genface(1,5,6,2,color4b2color3b(pix.color), relativePos, {-1,0,0}, p.scale);
            if (!pix.T) genface(4,5,1,0,color4b2color3b(pix.color), relativePos, {0,1,0}, p.scale);
            if (!pix.B) genface(6,7,3,2,color4b2color3b(pix.color), relativePos, {0,-1,0}, p.scale);
        }
    }

    glGenVertexArrays(1, &_vao);
    cocos2d::GL::bindVAO(_vao);
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PlanePixelVertexPormat)*_count, _vertexData, GL_STREAM_DRAW);
    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PlanePixelVertexPormat), (GLvoid *)offsetof(PlanePixelVertexPormat, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PlanePixelVertexPormat), (GLvoid *)offsetof(PlanePixelVertexPormat, color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(PlanePixelVertexPormat), (GLvoid *)offsetof(PlanePixelVertexPormat, normal));

    cocos2d::GL::bindVAO(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    CCLOG("pixel batch new VAO=%d VBO=%d", _vao, _vbo);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    free(_vertexData);

}


PlanePixelNode::~PlanePixelNode(){
    glDeleteBuffers(1, &_vbo);
    _vbo = 0;

    glDeleteVertexArrays(1, &_vao);
    cocos2d::GL::bindVAO(0);
    _vao = 0;
}


// 为了树木摇晃而设计的，功能
void PlanePixelNode::configXDiff(float diffRadio) //在 x 方向上，偏移的距离=diffRadio*Y
{
    _baseXDiff_target = diffRadio;
    _varyXDiff = 0;
}


void PlanePixelNode::configXDiffAni(float baseDiffRadio, float varyDiffRadio, float interval) //对应的动画，先由当前偏转移动到baseDiff，然后在 base 的地方在一定范围内摇曳， speed 每秒偏多少速度
{
    _baseXDiff_target = baseDiffRadio;
    _varyXDiff = varyDiffRadio;
    this->stopAllActions();

    if (interval > 0) {
        this->runAction(RepeatForever::create(Sequence::create(FadeOut::create(interval*0.5f), FadeIn::create(interval*0.5f), NULL)));
    }
    scheduleUpdate();
}

void PlanePixelNode::update(float dt)
{
    const float speed = 0.05;
    if (_baseXDiff_current < _baseXDiff_target) {
        _baseXDiff_current += dt*speed;
        if (_baseXDiff_current > _baseXDiff_target) {
            _baseXDiff_current = _baseXDiff_target;
            unscheduleUpdate();
        }
    } else if (_baseXDiff_current > _baseXDiff_target) {
        _baseXDiff_current -= dt*speed;
        if (_baseXDiff_current < _baseXDiff_target) {
            _baseXDiff_current = _baseXDiff_target;
            unscheduleUpdate();
        }
    }
}
