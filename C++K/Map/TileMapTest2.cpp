
TMXBug987New::TMXBug987New()
{
    auto map = cocos2d::FastTMXTiledMap::create("TileMaps/orthogonal-test6.tmx");
    addChild(map, 0, kTagTileMap);

    Size CC_UNUSED s1 = map->getContentSize();
    CCLOG("ContentSize: %f, %f", s1.width,s1.height);

    map->setAnchorPoint(Vec2(0.0f, 0.0f));
    auto layer = map->getLayer("Tile Layer 1");
    layer->setTileGID(3, Vec2(2,2));
}
