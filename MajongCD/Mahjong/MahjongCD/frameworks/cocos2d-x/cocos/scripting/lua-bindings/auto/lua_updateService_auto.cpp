#include "scripting/lua-bindings/auto/lua_updateService_auto.hpp"
#include "UpdateService.h"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"

int lua_UpdateService_UpdateService_Start(lua_State* tolua_S)
{
    int argc = 0;
    UpdateService* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UpdateService",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UpdateService*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_UpdateService_UpdateService_Start'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        int arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "UpdateService:Start");

        /*ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "UpdateService:Start");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_UpdateService_UpdateService_Start'", nullptr);
            return 0;
        }*/
		arg1 = toluafix_ref_function(tolua_S, 3, 0);
		if (!arg1) return 0;
        cobj->Start(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "UpdateService:Start",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_UpdateService_UpdateService_Start'.",&tolua_err);
#endif

    return 0;
}
int lua_UpdateService_UpdateService_Stop(lua_State* tolua_S)
{
    int argc = 0;
    UpdateService* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UpdateService",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UpdateService*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_UpdateService_UpdateService_Stop'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_UpdateService_UpdateService_Stop'", nullptr);
            return 0;
        }
        cobj->Stop();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "UpdateService:Stop",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_UpdateService_UpdateService_Stop'.",&tolua_err);
#endif

    return 0;
}
int lua_UpdateService_UpdateService_checkVersion(lua_State* tolua_S)
{
    int argc = 0;
    UpdateService* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UpdateService",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UpdateService*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_UpdateService_UpdateService_checkVersion'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_UpdateService_UpdateService_checkVersion'", nullptr);
            return 0;
        }
        cobj->checkVersion();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "UpdateService:checkVersion",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_UpdateService_UpdateService_checkVersion'.",&tolua_err);
#endif

    return 0;
}
int lua_UpdateService_UpdateService_checkDownload(lua_State* tolua_S)
{
    int argc = 0;
    UpdateService* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UpdateService",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UpdateService*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_UpdateService_UpdateService_checkDownload'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "UpdateService:checkDownload");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_UpdateService_UpdateService_checkDownload'", nullptr);
            return 0;
        }
        cobj->checkDownload(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "UpdateService:checkDownload",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_UpdateService_UpdateService_checkDownload'.",&tolua_err);
#endif

    return 0;
}
int lua_UpdateService_UpdateService_checkDownloadFull(lua_State* tolua_S)
{
    int argc = 0;
    UpdateService* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UpdateService",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UpdateService*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_UpdateService_UpdateService_checkDownloadFull'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        std::string arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "UpdateService:checkDownloadFull");

        ok &= luaval_to_std_string(tolua_S, 3,&arg1, "UpdateService:checkDownloadFull");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_UpdateService_UpdateService_checkDownloadFull'", nullptr);
            return 0;
        }
        cobj->checkDownloadFull(arg0, arg1);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "UpdateService:checkDownloadFull",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_UpdateService_UpdateService_checkDownloadFull'.",&tolua_err);
#endif

    return 0;
}
int lua_UpdateService_UpdateService_constructor(lua_State* tolua_S)
{
    int argc = 0;
    UpdateService* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_UpdateService_UpdateService_constructor'", nullptr);
            return 0;
        }
        cobj = new UpdateService();
        //cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"UpdateService");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "UpdateService:UpdateService",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_UpdateService_UpdateService_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_UpdateService_UpdateService_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (UpdateService)");
    return 0;
}

int lua_register_UpdateService_UpdateService(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"UpdateService");
    tolua_cclass(tolua_S,"UpdateService","UpdateService","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"UpdateService");
        tolua_function(tolua_S,"new",lua_UpdateService_UpdateService_constructor);
        tolua_function(tolua_S,"Start",lua_UpdateService_UpdateService_Start);
        tolua_function(tolua_S,"Stop",lua_UpdateService_UpdateService_Stop);
        tolua_function(tolua_S,"checkVersion",lua_UpdateService_UpdateService_checkVersion);
        tolua_function(tolua_S,"checkDownload",lua_UpdateService_UpdateService_checkDownload);
        tolua_function(tolua_S,"checkDownloadFull",lua_UpdateService_UpdateService_checkDownloadFull);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(UpdateService).name();
    g_luaType[typeName] = "UpdateService";
    g_typeCast["UpdateService"] = "UpdateService";
    return 1;
}
TOLUA_API int register_all_UpdateService(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"my",0);
	tolua_beginmodule(tolua_S,"my");

	lua_register_UpdateService_UpdateService(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

