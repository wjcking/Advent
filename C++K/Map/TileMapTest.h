/****************************************************************************
 http://www.cocos2d-x.org
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED  WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE. Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 ****************************************************************************/

#include "../BaseTest.h"

DEFINE_TEST_SUITE(TileMapTests);

class TileDemo : public TestCase
{
public:
    TileDemo();
    virtual ~TileDemo();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onExit() override;

    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
};

class TileMapTest : public TileDemo
{
public:
    CREATE_FUNC(TileMapTest);
    TileMapTest();
    virtual std::string title() const override;
};

class TileMapEditTest : public TileDemo
{
public:
    CREATE_FUNC(TileMapEditTest);
    TileMapEditTest ();
    virtual std::string title() const override;

    void updateMap(float dt);
};

class TMXOrthoTest : public TileDemo
{
public:
    CREATE_FUNC(TMXOrthoTest);
    TMXOrthoTest();
    virtual std::string title() const override;

    virtual void onEnter() override;
    virtual void onExit() override;
};

class TMXStaggeredTest : public TileDemo
{
public:
    CREATE_FUNC(TMXStaggeredTest);
    TMXStaggeredTest();
    virtual std::string title() const override;
    
    virtual void onEnter() override;
    virtual void onExit() override;
};

class TMXOrthoTest2 : public TileDemo
{
public:
    CREATE_FUNC(TMXOrthoTest2);
    TMXOrthoTest2();
    virtual std::string title() const override;
};

class TMXOrthoTest3 : public TileDemo
{
public:
    CREATE_FUNC(TMXOrthoTest3);
    TMXOrthoTest3();
    virtual std::string title() const override;
};

class TMXOrthoTest4 : public TileDemo
{
public:
    CREATE_FUNC(TMXOrthoTest4);
    TMXOrthoTest4();
    void removeSprite(float dt);
    virtual std::string title() const override;
};

class TMXReadWriteTest : public TileDemo
{
    unsigned int _gid;
    unsigned int _gid2;
public:
    CREATE_FUNC(TMXReadWriteTest);
    TMXReadWriteTest();
    virtual std::string title() const override;

    void removeSprite(Node* sender);
    void updateCol(float dt);
    void repaintWithGID(float dt);
    void removeTiles(float dt);
};

class TMXIsoTest : public TileDemo
{
public:
    CREATE_FUNC(TMXIsoTest);
    TMXIsoTest();
    virtual std::string title() const override;
};

class TMXIsoTest1 : public TileDemo
{
public:
    CREATE_FUNC(TMXIsoTest1);
    TMXIsoTest1();
    virtual std::string title() const override;
};

class TMXIsoTest2 : public TileDemo
{
public:
    CREATE_FUNC(TMXIsoTest2);
    TMXIsoTest2();
    virtual std::string title() const override;
};

class TMXOrthoZorder : public TileDemo
{
    cocos2d::Sprite*    _tamara;
public:
    CREATE_FUNC(TMXOrthoZorder);
    TMXOrthoZorder();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    virtual ~TMXOrthoZorder();
    void repositionSprite(float dt);
};
