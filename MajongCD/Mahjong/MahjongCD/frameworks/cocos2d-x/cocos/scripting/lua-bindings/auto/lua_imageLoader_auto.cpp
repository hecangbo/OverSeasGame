#include "scripting/lua-bindings/auto/lua_imageLoader_auto.hpp"
#include "ImageLoader.h"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"

int lua_ImageLoader_ImageLoader_Load(lua_State* tolua_S)
{
    int argc = 0;
    ImageLoader* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ImageLoader",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ImageLoader*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ImageLoader_ImageLoader_Load'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ImageLoader_ImageLoader_Load'", nullptr);
            return 0;
        }
        cobj->Load();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ImageLoader:Load",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ImageLoader_ImageLoader_Load'.",&tolua_err);
#endif

    return 0;
}
int lua_ImageLoader_ImageLoader_AddImageAsync(lua_State* tolua_S)
{
    int argc = 0;
    ImageLoader* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ImageLoader",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ImageLoader*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ImageLoader_ImageLoader_AddImageAsync'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::vector<std::string> arg0;

        ok &= luaval_to_std_vector_string(tolua_S, 2, &arg0, "ImageLoader:AddImageAsync");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ImageLoader_ImageLoader_AddImageAsync'", nullptr);
            return 0;
        }
        cobj->AddImageAsync(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ImageLoader:AddImageAsync",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ImageLoader_ImageLoader_AddImageAsync'.",&tolua_err);
#endif

    return 0;
}
int lua_ImageLoader_ImageLoader_LoadOneDone(lua_State* tolua_S)
{
    int argc = 0;
    ImageLoader* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ImageLoader",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ImageLoader*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ImageLoader_ImageLoader_LoadOneDone'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Texture2D* arg0;

        ok &= luaval_to_object<cocos2d::Texture2D>(tolua_S, 2, "cc.Texture2D",&arg0, "ImageLoader:LoadOneDone");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ImageLoader_ImageLoader_LoadOneDone'", nullptr);
            return 0;
        }
        cobj->LoadOneDone(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ImageLoader:LoadOneDone",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ImageLoader_ImageLoader_LoadOneDone'.",&tolua_err);
#endif

    return 0;
}
int lua_ImageLoader_ImageLoader_constructor(lua_State* tolua_S)
{
    int argc = 0;
    ImageLoader* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_ImageLoader_ImageLoader_constructor'", nullptr);
            return 0;
        }
        cobj = new ImageLoader();
        tolua_pushusertype(tolua_S,(void*)cobj,"ImageLoader");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "ImageLoader:ImageLoader",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_ImageLoader_ImageLoader_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_ImageLoader_ImageLoader_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (ImageLoader)");
    return 0;
}

int lua_register_ImageLoader_ImageLoader(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"ImageLoader");
    tolua_cclass(tolua_S,"ImageLoader","ImageLoader","",nullptr);

    tolua_beginmodule(tolua_S,"ImageLoader");
        tolua_function(tolua_S,"new",lua_ImageLoader_ImageLoader_constructor);
        tolua_function(tolua_S,"Load",lua_ImageLoader_ImageLoader_Load);
        tolua_function(tolua_S,"AddImageAsync",lua_ImageLoader_ImageLoader_AddImageAsync);
        tolua_function(tolua_S,"LoadOneDone",lua_ImageLoader_ImageLoader_LoadOneDone);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ImageLoader).name();
    g_luaType[typeName] = "ImageLoader";
    g_typeCast["ImageLoader"] = "ImageLoader";
    return 1;
}
TOLUA_API int register_all_ImageLoader(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"my",0);
	tolua_beginmodule(tolua_S,"my");

	lua_register_ImageLoader_ImageLoader(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

