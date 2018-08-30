--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
module("Config", package.seeall)

--function getExternStorePath()
--    local path = "/mnt/com.hcb.game/"
--    print("当前平台为:"..path)
--    return path
--end
require("GF/Environment.lua")
local EnvDebug = Environment.DEBUG
local EnvPublish = Environment.Publish
local UpdateInfo = {}

--当前平台
local EnvCur= Environment.getEnv()

UpdateInfo[EnvDebug] = 
{
    Version ="http://192.168.1.105:8688/mahjong/android/version.txt:8688",
    --DefaultSize = 50,
    WaitTime = 8,
    VerFile = cc.FileUtils:getInstance():getWritablePath().."Version",
    --GamePrefix = "http://192.168.3.2/downloads/",
    GamePrefix = "http://192.168.1.105:8688/",
    GameSuffix = "/hall"
}

UpdateInfo[EnvPublish] =
{
	Version = "g1.cy9527.com:8688",
	DefaultSize = 10 * 1024,
	WaitTime = 8,
	VerFile = cc.FileUtils:getInstance():getWritablePath().."Version",
    GamePrefix = "http://d.qpgame.com/download/update/",
    GameSuffix = ""
}


function getUpdateInfo()
    local gameName = GameCenter.getCurrentGameName()
    local info = UpdateInfo[EnvCur]
    info.Game = info.GamePrefix .. gameName .. info.GameSuffix
    if Utility.IsIOS() then
       info.Game = info.Game .. "/ios/"
    elseif Utility.IsAndroid() then
       info.Game = info.Game .. "/android/"
    elseif Utility.IsWin() then
       info.Game = info.Game .. "/android/" 
    end
    return info
end

--endregion
