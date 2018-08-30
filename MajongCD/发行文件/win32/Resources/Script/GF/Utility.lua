--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
--公用函数

local targetPlatform = cc.Application:getInstance():getTargetPlatform()
PLATFORM=
{
    PLATFORM_OS_WINDOWS = 0,
    PLATFORM_OS_LINUX   = 1,
    PLATFORM_OS_MAC     = 2,
    PLATFORM_OS_ANDROID = 3,
    PLATFORM_OS_IPHONE  = 4,
    PLATFORM_OS_IPAD    = 5,
    PLATFORM_OS_BLACKBERRY = 6,
    PLATFORM_OS_NACL    = 7,
    PLATFORM_OS_EMSCRIPTEN = 8,
    PLATFORM_OS_TIZEN   = 9,
    PLATFORM_OS_WINRT   = 10,
    PLATFORM_OS_WP8     = 11
}


function __G__TRACKBACK__(msg)
    logOut("调用lua失败")
end

function logOut(str)
    if luajava ==nil then
        print("[Lua].......hcb:"..str)
    else
        local logcat = luajava.bindClass("android.util.Log")
        logcat:d("[Lua].......hcb:",str)
    end
end

function log(t)
    if(type(t) == "string" or type(t)== "number") then
        print("\n\t"..t)
    elseif(type(t) == "table") then
        print("\n\t"..tostring(t))
    else
        print("\n\t".. tostring(t))
    end
end

function logt(t)
    if type(t) == "table" then
        logOut(tostring(t))
    else
        logOut(t)
    end
end

Utility=
{
    --要据tag、name获取资源
    getWidget= function(aLayer, aTag)
        return ccui.Helper:seekWidgetByTag(aLayer._root,aTag)
    end;

    getWidgetByName = function(aLayer, name)
        return ccui.Helper:seekWidgetByName(aLayer._root, name)
    end;

    --Label写入内容
    setLabelText = function(aLayer, tag, text, color)
        local obj = Utility.getWidget(aLayer, tag)
        if obj~= nil then
            local l = tolua.cast(obj, "ccui.Text")
            l:setString(text)
            if color ~= nil then
                l:setColor(color)
            end
        end
    end;

    setLabelTextByName = function(aLayer, name, text, color)
        local obj = Utility.getWidgetByName(aLayer, name)
        if obj~= nil then
            local l = tolua.cast(obj, "ccui.Text")
            l:setString(text)
            if color ~= nil then
                l:setColor(color)
            end
        end
    end;

    --当前平台
    getTargetPlatform= function()
        return tonumber(targetPlatform)
    end;

    IsIOS = function()
        return tonumber(targetPlatform) == PLATFORM.PLATFORM_OS_IPHONE
    end;

    IsAndroid = function()
        return tonumber(targetPlatform) == PLATFORM.PLATFORM_OS_ANDROID
    end;
    IsWin = function()
        return tonumber(targetPlatform) == PLATFORM.PLATFORM_OS_WINDOWS
    end;

    callFunc = function (obj, tag, funcs)
        local func = funcs[tag]
        if(func ~= nil ) then
            func(obj, tag)
        end
    end

}
--endregion
