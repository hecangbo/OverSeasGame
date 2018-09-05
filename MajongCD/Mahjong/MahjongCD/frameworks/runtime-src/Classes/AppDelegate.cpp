#include "AppDelegate.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "audio/include/SimpleAudioEngine.h"
#include "cocos2d.h"
#include "scripting/lua-bindings/manual/lua_module_register.h"

#include "scripting/lua-bindings/auto/lua_imageLoader_auto.hpp"
#include "scripting/lua-bindings/auto/lua_updateService_auto.hpp"
using namespace CocosDenshion;

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();

#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
    // NOTE:Please don't remove this call if you want to debug with Cocos Code IDE
    RuntimeEngine::getInstance()->end();
#endif

}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // set default FPS
    Director::getInstance()->setAnimationInterval(1.0 / 60.0f);

    // register lua module
    auto engine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(engine);
    lua_State* L = engine->getLuaStack()->getLuaState();
    lua_module_register(L);
	register_all_ImageLoader(L);
	register_all_UpdateService(L);
    register_all_packages();

    LuaStack* stack = engine->getLuaStack();
    stack->setXXTEAKeyAndSign("2dxLua", strlen("2dxLua"), "XXTEA", strlen("XXTEA"));

    //register custom function
    //LuaStack* stack = engine->getLuaStack();
    //register_custom_function(stack->getLuaState());
	vector<string> searchPath;
	string lPath1 = CCFileUtils::sharedFileUtils()->getWritablePath();
	CCLOG("...............hcb:%s", lPath1.c_str());	///data/user/0/com.hcb.game/files/
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	searchPath.push_back(".");
 	searchPath.push_back("Resources");
	searchPath.push_back("Resources\\Script");
	searchPath.push_back("Resources\\res");
	searchPath.push_back("Resources\\csb");
	searchPath.push_back("Resources\\Animation");
	//searchPath.push_back("Script");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID or CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	string lPath = CCFileUtils::sharedFileUtils()->getWritablePath();
	searchPath.push_back("./");
	searchPath.push_back(lPath + "assets");
	searchPath.push_back(lPath + "assets/Script");
	searchPath.push_back(lPath + "assets/res");
	searchPath.push_back(lPath + "assets/csb");
	searchPath.push_back(lPath + "assets/Animation");
	searchPath.push_back("csb");
#endif
	searchPath.push_back("Script");
	searchPath.push_back("res");
	searchPath.push_back("csb");
	searchPath.push_back("Animation");
	for (vector<string>::iterator iter = searchPath.begin(); iter != searchPath.end(); ++iter)
	{
		CCLOG("...............hcb:==%s", (*iter).c_str());
	}
	FileUtils::getInstance()->setSearchPaths(searchPath);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("loading.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("scene.plist");
    if (engine->executeScriptFile("game.lua")) //src/main.lua
    {
        return false;
    }

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
