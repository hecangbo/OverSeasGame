--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
require("Layer/UpdateLayer.lua")
Update=
{
    _currentVersion=
    {
        GameHotFixVersion = 4,
        GameBigVersion = 2
    }
}

function Update:new(o)
    o = o or {}
    setmetatable(o,self)
    self.__index = self
    self._updater = my.UpdateService:new()
    self:checkVersionFile()
    self:loadVersion()
    return o
end

function Update:setLayer(layer)
	self._layer = layer
end

function Update:checkVersionFile()
	local fl =io.open(Config.getUpdateInfo().VerFile, "r")
	if(fl == nil) then
		self:writeVersion()
	else
		fl:close()
	end
end


function Update:loadVersion()
    local rfile = io.open(Config.getUpdateInfo().VerFile, "r")
    if rfile~= nil then
        local str = rfile:read("*a")
        self._currentVersion = json.decode(str)

        --删除小版本更新文件
        local delete = function()
            local writableDir = cc.FileUtils:getInstance():getWritablePath()
            local bOk = os.rmdir(writableDir)
            if bOk then
                logOut("delete dir:"..writableDir.." success!")
                self:writeVersion()
            end
        end
--[[
        if(luajava ~= nil) then
            local version = self._currentVersion.versionCode or 0
            local javaApp = luajava.bindClass("com.qpgame.gameframe.GameFrame")
            local infos = javaApp:getAppInfo()
            if infos then
                if tonumber(infos.versionCode) > version then
                    self._currentVersion.versionCode = tonumber(infos.versionCode)
                    delete()
                end
            end

            --local lBigVerion = javaApp:getGameBigVersion()
            self._currentVersion.GameBigVersion = tonumber(infos.versionCode)
            self:writeVersion()
        end

        if Utility.IIsIOS then
            require( "GF/GameFrame.lua" )
            local version = self._currentVersion.versionCode or 0
            local args = {class = "PlatformHelper", func = "getAppBuildCode"}
            local ok, ret = luaoc.callStaticMethod("LuaocDispatcher","dispatchCall",args)
            if ok and ret then
                logOut("ret.version:"..ret.version)
                logOut("version:"..version)
                self._currentVersion.GameBigVersion = tonumber(ret.version)
                if tonumber(ret.version) > tonumber(version) then
                    self._currentVersion.versionCode = tonumber(ret.version)
                    delete()
                end
            end
        end
--]]
        rfile:close()
    end
end


function Update:writeVersion()
	local wfile =io.open(Config.getUpdateInfo().VerFile, "w")
	if(wfile ~= nil) then
		wfile:write(json.encode(self._currentVersion))
		wfile:close()
	end
end


--安装包的小版本号
function Update:getCurHotVersion()
    if self._currentVersion and self._currentVersion.GameHotFixVersion then
        return self._currentVersion.GameHotFixVersion
    else
        return 0
    end
end

--安装包的大版本号
function Update:getCurBigVersion()
    if self._currentVersion and self._currentVersion.GameBigVersion then
        return self._currentVersion.GameBigVersion
    else
        return 0
    end
end


function Update:StartGame()
    self._updater:Stop()
    require("Main.lua")
end

function Update:Start()
    local UpdateInfo =json.encode(Config.getUpdateInfo())    
    logt(UpdateInfo)
    self._updater:Start(UpdateInfo, function(event, result, value, flag, bigver,size) self:onUpdate(event, result, value, flag, bigver,size) end)
    self._updater:checkVersion()
    self._updatedCount = 0
    self._checkScheduler = director:getScheduler():scheduleScriptFunc(function() self:onCheckVersion(false, 0, 0, 0, 0) end, 12, false)
end


function Update:onUpdate(event,result, value,flag, bigver,size)
    logOut("function Update_onUpdate")
    local action = 
    {
        ["CheckVersion"] = Update.onCheckVersion,       --检查
        ["CheckDownload"] = Update.onCheckDownload,
        ["DownloadFile"] = Update.onDownloadFile,
        ["DownloadCheckList"] = Update.onDownloadCheckList,
        ["DownloadFullVersionresult"] = Update.onDownloadFullVersion,       --整包更新
        ["Downloading"] = Update.onDownloadprogress,    --下载进度
    }

    local func = action[event]
    if func~= nil then
        func(self,result,value,flag, bigver,size)
    end
end


-----begin--
function Update:onCheckVersion(result, value, flag, bigver, size)
    director:getScheduler():unscheduleScriptEntry(self._checkScheduler)
    local lStartGame = true
    ---test
    --result =true
   -- bigver = 3
   -- value = 5
    if result then
        if bigver > self:getCurBigVersion() then
            logOut("大版本更新")
            lStartGame = false
            self._newBigVer = bigver
            self._layer:dectedNewVersionFull(size)
        elseif value ~= self:getCurHotVersion() then
            logOut("小版本更新")
            lStartGame = false
            self._newHotVer = value
            --self:checkDownload()
            self._layer:DownloadHotFix()
        end
    end
    if lStartGame then
        self:StartGame()
    end
end
function Update:checkDownload()
    self._updater:checkDownload(self._newHotVer)
end

function Update:download()
    self._updater:download(self._updatedCount)
    --self._progress = TimerManager.createTimer(self, self.updateProgress)
    --self._progress:start(0)
end

function Update:downloadfullverison()
      logOut("整包更新")
--    if Utility.getTargetPlatform() == TargetPlatform.IOS then
--		local lURL = BuYv.getAppDownloadUrl(Info.AppleId)
--		local args = {class = "PlatformHelper", func = "openUrl", arg1 = lURL}
--		local ok , ret = luaoc.callStaticMethod("LuaocDispatcher","dispatchCall", args)
--		if not ok then
--			log("open url fail on iOS platform!")
--		end
--	else
		local lUrl = Mahjong.getFullVersionUrl(self._newBigVer)
		local pathSave = Mahjong.getFullSavePath()
        logOut("lUrl==="..lUrl)

		self._updater:checkDownloadFull(lUrl,pathSave)
        logOut("下载完成")
        logOut("下载完成")

       -- self._progress = TimerManager.createTimer(self, self.updateProgress)
       -- self._progress:start(0)
--	end
end

function Update:updateProgress()
        logOut("........Update:updateProgress")
        logOut("........Update:updateProgress")
--    if self._updater then
--        local progress = self._updater:getProgress()
--        if progress and self._layer then
--            if progress.total > 0 then
--                self._layer:setDownloadFullProgress(progress.addUp, progress.total, progress.total)
--                if progress.addUp >= progress.total then
--                    if progress.flag == 1 then
--                        self:onDownloadFullVersion(true, true)
--                    else
--                        self:smallVersionDownloadDone()
--                    end
--                end
--            end
--        end
--    end
end

function Update:onDownloadCheckList(result)
    if result then
        if self._newHotVer then
            self._currentVersion.GameHotFixVersion = self._newHotVer
        end
        self:writeVersion()
        self:StartGame()
    else
        self._layer:DownloadCheckListFailed()
    end

end


function Update:onDownloadFullVersion(result, value)
    if not result then return end
    if result then
        local lpath = Mahjong.getFullSavePath()
        lpath = lpath .. "mahjongCD.apk"
        logOut("当前的路径为:" .. lpath)
        --android        
        if (PLATFORM.PLATFORM_OS_ANDROID == tonumber(Utility.getTargetPlatform())) then
            local luaj = require "cocos.cocos2d.luaj"
            local className = "org/cocos2dx/lua/AppActivity"
            sigs = "(Ljava/lang/String;)Z"

            args = { lpath }
            if luaj ~= nil then
                local ok,ret = luaj.callStaticMethod(className,"InstallAPK",args,sigs)                
                if ok then
                    logOut("begin install apk")
                    cc.Director:getInstance():endToLua()
                end
            end
        end

        if PLATFORM.PLATFORM_OS_WINDOWS == Utility.getTargetPlatform() then
            if self._newBigVer then
               self._currentVersion.GameBigVersion = self._newBigVer
            end
            self:writeVersion()
        end
        self:sendExitNotify()
    else
        logOut("大版本更新失败")
        self._layer:downloadFailed()
       -- self:downloadfullverison()
    end
end


function Update:sendExitNotify()
--    if Utility.getTargetPlatform() == TargetPlatform.ANDROID and luajava then
--		local lGameFrame = luajava.bindClass("com.qpgame.gameframe.GameFrame")
--		if(lGameFrame ~= nil) then
--			lGameFrame:OnExitGameNotify()
--		end
--	elseif Utility.getTargetPlatform() == TargetPlatform.IOS then
--		local lURL = BuYv.getAppDownloadUrl(Info.AppleId)
--		local args = {class = "PlatformHelper", func = "exitApp", arg1 = lURL}
--		local ok , ret = luaoc.callStaticMethod("LuaocDispatcher","dispatchCall", args)
--	end
    logOut("game is over")
	cc.Director:getInstance():endToLua()
    
end

function Update:onDownloadprogress(result,lFileSize, lDown, lTotal)
    if result then
        logOut("lFileSize =="..lFileSize..";lDown=="..lDown.. ";lTotal=="..lTotal)
        self._layer:setDownloadProgress(lFileSize, lDown, lTotal)
    end

end


---end---
GameCenter.setGame("mahjong", GameCenter)
GameCenter.invokeGame("mahjong")


local layer = UpdateLayer:create()
local scene = cc.Scene:create()
scene:addChild(layer._tg)
director:runWithScene(scene)

local update = Update:new()
update:setLayer(layer)
layer:setUpdater(update)

--update:Start()      --先检测版本
update:StartGame()  --不进行版本检测


--endregion
