TMXOrthoTest2::TMXOrthoTest2()
{
    auto map = TMXTiledMap::create("TileMaps/orthogonal-test1.tmx");
    addChild(map, 0, kTagTileMap);

    Size CC_UNUSED s = map->getContentSize();
    CCLOG("ContentSize: %f, %f", s.width,s.height);

    auto& children = map->getChildren();
    SpriteBatchNode* child = nullptr;

    for(const auto &obj : children) {
        child = static_cast<SpriteBatchNode*>(obj);
        child->getTexture()->setAntiAliasTexParameters();
    }
    map->runAction( ScaleBy::create(2, 0.5f) ) ;
}
