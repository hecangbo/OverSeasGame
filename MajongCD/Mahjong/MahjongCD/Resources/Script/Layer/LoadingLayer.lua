--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
require("Layer/MainLayer.lua")
require("GF/WeChat.lua")
FileInfo = 
{
   "login.plist",
   "res_mj.plist",
   "res00.plist",
   "res01.plist",
   "res02.plist",
   "roomBG.plist",  
   "res_mj.plist",
   "os_mainmenu.plist"
}


LoadingLayer = {
    TAG_Flag = 13,
}

function LoadingLayer:new(o)
	o = o or {}
	setmetatable(o,self)
	self.__index = self
	return o
end

function LoadingLayer:create()
    return CocostudioHelper.createNode(LoadingLayer,"loading.csb")
end

function LoadingLayer:onNodeLoaded()
    
end

function LoadingLayer:onEnter()
    self.bg_panel = self._tg:getChildByTag(2)
    self._progressbar = self.bg_panel:getChildByTag(12)
    self._moveflag = self.bg_panel:getChildByTag(13)

    self._curAddCount = 0
end

function LoadingLayer:onEnterTransitionFinish()
    local listener = cc.EventListenerCustom:create("OneImageLoaded", function() self:onOneImageLoaded() end)--异步加载
    self._tg:getEventDispatcher():addEventListenerWithFixedPriority(listener, 1)
    --
    listener = cc.EventListenerCustom:create("ImageLoadDone", function() self:onImageLoadDone() end)        --加载完成
    self._tg:getEventDispatcher():addEventListenerWithFixedPriority(listener,1)

    addimagesync:AddImageAsync(FileInfo)
    
    notificationCenter:register(self, self.onWeChatUserInfo, Notifications.bindWeChatUserInfo)
end

function LoadingLayer:onExit()
    self._tg:getEventDispatcher():removeEventListenersForTarget(self._tg)
end


--加载资源
function LoadingLayer:onOneImageLoaded()
    self._curAddCount = self._curAddCount +1
    if self._curAddCount<= #FileInfo then
        cc.SpriteFrameCache:getInstance():addSpriteFrames(FileInfo[self._curAddCount])
        local lPercent = self._curAddCount / #FileInfo *100
        self._progressbar:setPercent(lPercent)
        local posX = self._moveflag:getPositionX() + self._progressbar:getContentSize().width/ #FileInfo
        self._moveflag:setPositionX(posX)
    end
end

--资源加载完成
function LoadingLayer:onImageLoadDone()
    if (PLATFORM.PLATFORM_OS_ANDROID == tonumber(Utility.getTargetPlatform())) then
        WeChat.WeChatLogin()
  else     
        --直接进入，不使用微信登录
        scenemanager:run(MainLayer)
    end    
end

function LoadingLayer:onWeChatUserInfo(msg)
    local str = json.decode(msg)
    logOut(str.accessToken..";"..str.unionID)
    local function userinfo(msg2)
        local str = json.decode(msg2)       
        userManager:setWeChatUserInfo(msg2)        
        scenemanager:run(MainLayer)
    end
    HttpBindHelper.bindWeChatUserInfo(str.accessToken, str.unionID, userinfo)
end

--endregion
