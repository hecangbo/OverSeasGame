--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
cc.FileUtils:getInstance():addSearchPath("../res")
cc.FileUtils:getInstance():addSearchPath("../csb")
cc.FileUtils:getInstance():addSearchPath("../Animation")
--local path = cc.FileUtils:getInstance():getSearchPaths()

require("GF/SystemConfig.lua")
require("GF/CocostudioHelper.lua")
--require("GF/Utility.lua")
require("GF/SceneManager.lua")
require("GF/json.lua")
require("GF/Config.lua")
require("GF/TimerManager.lua")
require("GF/GameCenter.lua")
require("GF/Animation.lua")
require("GF/HttpClient.lua")
require("GF/HttpBindHelper.lua")
require("GF/LuaNotificationCenter.lua")
require("GF/Notifications.lua")
require("GF/UserManager.lua")
scenemanager = SceneManager:new()
addimagesync = my.ImageLoader:new()
notificationCenter = LuaNotificationCenter:new()
userManager = UserManager:new()

--当前环境重定义
local EnvDebug = Environment.DEBUG
local EnvRelease = Environment.Publish

--取得当前环境
local EnvCur = Environment.getEnv()

--整包更新地址
local tFullVersionUrl = {}
--http://192.168.1.105:8688/mahjong/1.txt
tFullVersionUrl[EnvDebug] = "http://192.168.1.105:8688/mahjong/APK/MahjongCD_HD_%d.apk"
tFullVersionUrl[EnvRelease] = ""

Mahjong =
{
    
    getFullSavePath = function()
        if Utility.IsWin() then
            return "f:/mnt/sdcard/qp.game.cache/"
        else
		    return "/mnt/sdcard/qp.game.cache/"
        end
	end;

    getFullVersionUrl = function(bigver)
        local strUrl = tFullVersionUrl[EnvCur]
        return string.format(strUrl, tonumber(bigver))
    end;
}
--endregion
