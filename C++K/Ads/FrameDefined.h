#define
CREATED_END \
{ \
    sprite->autorelease(); \
    return sprite; \
} \
else \
{ \
    delete sprite; \
    sprite = nullptr; \
    return nullptr; \
} 

#define  LUAH Luah::getInstance() 
#define  LUASTATE Luah::getInstance()->getState()
#define
LUA_ENUM(l, val) \
lua_pushliteral(l, #val); \
lua_pushnumber(l, val); \
lua_settable(l, -3)

#define
CREATE_WITH_FILE(__TYPE__,NAME) \
    auto *sprite = new(std::nothrow) __TYPE__(); \
    if (sprite && sprite->initWithFile(NAME)) \
CREATED_END

#define
CREATE_WITH_FRAMENAME(__TYPE__,NAME) \
    auto *sprite = new(std::nothrow) __TYPE__(); \
    if (sprite && sprite->initWithSpriteFrameName(NAME)) \
CREATED_END

#define
CREATE_WITH_FRAME(__TYPE__,FRAME) \
    auto *sprite = new(std::nothrow) __TYPE__(); \
    if (sprite && sprite->initWithSpriteFrame(FRAME)) \
CREATED_END

template<class T>
static T* createWithFrame(SpriteFrame* spriteFrame)
{
    CREATE_WITH_FRAME(T, spriteFrame);
    return nullptr;
}
template<class T>
static T* createWithFrameName(const std::string& fileName)
{
    CREATE_WITH_FRAMENAME(T, fileName);
    return nullptr;
}
template<class T>
static T* createWithFileName(const std::string& fileName)
{
    CREATE_WITH_FILE(T, fileName);
    return nullptr;
}