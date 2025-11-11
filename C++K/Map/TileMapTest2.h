/****************************************************************************
 http://www.cocos2d-x.org
 
 The above copyright notice and this permission notice shall be included in
 all copies or subs
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 ****************************************************************************/

#ifndef _TILEMAP_TEST_NEW_H_
#define _TILEMAP_TEST_NEW_H_

#include "../BaseTest.h"

DEFINE_TEST_SUITE(FastTileMapTests);

class TMXBug987New : public TileDemoNew
{
public:
 CREATE_FUNC(TMXBug987New);
 TMXBug987New();
 virtual std::string title() const override;
 virtual std::string subtitle() const override;
};

class TMXBug2 : public TileDemo
b
 //�Ƿ������滭��ײ�߿�
 auto allowDraw = LUAH->getGlobal("AllowDraw").toValue<bool>();
 if (!allowDraw)
  return;
 drawCollision->clear();
 //�������role�����򿴲���
 drawCollision->setLocalZ(Z_Draw);
 for (auto role : getChildren())
 {
  auto r = dynamic_cast<Tasnal*>(role);

  if (nullptr != r)
  {
   drawCollision->drawRact(Vec2(r->getCollisionBound(r->getInsetObject()).getMinX(), r->getCollisionBound(r->getInsetObject()).getMinY()), Vec2(r->getCollisionBound(r->getInsetObject()).getMaxX(), r->getCollisionBound(r->getInsetObject()).getMaxY()), Color4F::BLUE);
   //sensor
   r->getSensor().render(drawCollision);
  }
 }

 trikerSystem.render(drawCollision);
d
#endif
