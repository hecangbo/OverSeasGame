--region NewFile_1.lua
--Author : hechangbo
--Date   : 2018/9/11
--此文件由[BabeLua]插件自动生成

LuaJavaBinding={

--错误代码                            描述
--1                          不支持的参数类型或返回值类型
--2                          无效的签名
--3                          没有找到指定的方法
--4                          Java 方法执行时抛出了异常
--5                          Java 虚拟机出错
--6                          Java 虚拟机出错

    toJava= function(className, funName, args, sigs)
        if (PLATFORM.PLATFORM_OS_ANDROID == tonumber(Utility.getTargetPlatform())) then
            local luaj = require "cocos.cocos2d.luaj"
            local ok, ret = luaj.callStaticMethod(className, funName, args, sigs)
            if not ok then
                logOut("luaj error:", ret)
            elseif ok and ret ~=nil then
                logOut("the ret is succes:",ret)
            end
        end
    end;
   
   --微信授权登录
   WeChatLoginInfo = function(appid,appsecret, accessTokenURL)
        local className = "org/cocos2dx/lua/AppActivity"
        local funName = "WeChatInfo"
        local args = {appid, appsecret,accessTokenURL}
        local sigs = "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"
        LuaJavaBinding.toJava(className, funName, args, sigs)
   end;


    --微信分享
   WeChatShard = function(appid, context)
        local className = "org/cocos2dx/lua/AppActivity"
        local funName = "WeChatShard"
        local args = {appid, context}
        local sigs = "(Ljava/lang/String;Ljava/lang/String;)V"
        LuaJavaBinding.toJava(className, funName, args, sigs)
   end;

   onZXingScan = function()
        local className = "org/cocos2dx/lua/AppActivity"
        local funName = "ZXingScan"
        local args = {}
        local sigs = "()V"
        LuaJavaBinding.toJava(className, funName, args, sigs)
   end;
}


function getWeChatLoginInfo(WeChatInfo)
    notificationCenter:post(Notifications.bindWeChatUserInfo, WeChatInfo)
end

--endregion
