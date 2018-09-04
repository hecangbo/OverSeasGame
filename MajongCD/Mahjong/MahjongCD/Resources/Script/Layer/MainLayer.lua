--region *.lua
--Date
--此文件由[BabeLua]插件自动生成

MainLayer=
{
    --TAG_BUT_FASTBEGIN = 11,     --快速开始
    --TAG_BUT_HALLLAYER = 12,     --游戏大厅
    --TAG_BUT_CHANGEACCOUNT = 13  --更换帐号
    TAG_BG_PANEL = 2,
    TAG_IMAGE_TOP = 10,
    TAG_IMAGE_MIDDLE = 11,
    TAG_IMAGE_BOOTOM =12,
}

function MainLayer:new(o)
    o = o or {}
    setmetatable(o,self)
    self.__index = self
    return o
end

function MainLayer:create()
    return CocostudioHelper.createNode(self,"os_mainmenu.csb")
end

function MainLayer:onNodeLoaded()
    
end

function MainLayer:onEnter()
    logOut("网络测试")
    logOut("网络测试")
    

    self:registerBtnEvent()
end

function MainLayer:onEnterTransitionFinish()
    logOut("网络测试")
    self.bg_panel = self._tg:getChildByTag(self.TAG_BG_PANEL)
    self.img_top = self.bg_panel:getChildByTag(self.TAG_IMAGE_TOP)
    self.img_middle = self.bg_panel:getChildByTag(self.TAG_IMAGE_MIDDLE)
    self.img_bottom = self.bg_panel:getChildByTag(self.TAG_IMAGE_BOOTOM)

    --self.img_top:setVisible(false) --显示隐藏

    local skeletonNode = sp.SkeletonAnimation:create("Praise/Praise.json", "Praise/Praise.atlas", 0.6)
    skeletonNode:setPosition(cc.p(400,300))
    skeletonNode:setAnimation(0, "Praise", true)
    self.bg_panel:addChild(skeletonNode)
end

function MainLayer:onExit()
    --self:unRegisterBtnEvent()
end
function MainLayer:registerBtnEvent()
--    local tags =
--    {
--        self.TAG_BUT_FASTBEGIN,
--        self.TAG_BUT_HALLLAYER,
--        self.TAG_BUT_CHANGEACCOUNT,
--    }
--    CocostudioHelper.regBtnEvt(self, tags)
end

function MainLayer:unRegisterBtnEvent()
end

--function  MainLayer:onPressEnded(sender)
--    local action =
--    {
--        [self.TAG_BUT_FASTBEGIN]        = self.onBtnFastBeginGame,
--        [self.TAG_BUT_HALLLAYER]        = self.onBtnHall,
--        [self.TAG_BUT_CHANGEACCOUNT]    = self.onBtnChangeAccount,
--    }
--    Utility.callFunc(self, sender:getTag(), action)
--end


--function MainLayer:onBtnFastBeginGame()
--    logOut("onBtnFastBeginGame")
--    logOut("onBtnFastBeginGame")
--    require("cocos/Networkconstants.lua")
--    local xhr = cc.XMLHttpRequest:new()
--    xhr.responseType = cc.XMLHTTPREQUEST_RESPONSE_STRING
--    --xhr:open("GET", "http://httpbin.org/get")
--    xhr:open("POST","127.0.0.1:4321")
--    local function onReadyStateChanged()
--        if xhr.readyState == 4 and (xhr.status >= 200 and xhr.status < 207) then
--            print(xhr.response)
--        end
--            xhr:unregisterScriptHandler()
--    end
--    xhr:registerScriptHandler(onReadyStateChanged)
--    xhr:send()
--end


--function MainLayer:onBtnHall()
--    require("GF/WSProxy.lua")
--    logOut("............")
--end


function MainLayer:onTouchBegan(posX,posY)
    self.curTouchBeganX = posX
    self.imgBottomPosation = self.img_bottom:getPositionX()
    self.imgMiddlePosation = self.img_middle:getPositionX()
    self.imgTopPosation = self.img_top:getPositionX()
end

function MainLayer:onTouchMoved(posX,posY)
    self:calculatorPosation(posX,posY)
end

function MainLayer:onTouchEnded(posX,posY)
end


function MainLayer:calculatorPosation(posX,posY)
    local imgBottomSize = self.img_bottom:getContentSize()

    moveX = posX - self.curTouchBeganX
    local bottomDestX =  self.imgBottomPosation + moveX
    if bottomDestX> imgBottomSize.width /2 and moveX>0 then
        self.img_bottom:setPositionX(imgBottomSize.width /2)
    elseif bottomDestX<GLView.Size.width-imgBottomSize.width /2 and moveX<0 then
        self.img_bottom:setPositionX( GLView.Size.width-imgBottomSize.width /2 )
    else
        self.img_bottom:setPositionX( bottomDestX)
        self.img_middle:setPositionX( self.imgMiddlePosation + moveX/2)     --speed减半
        self.img_top:setPositionX( self.imgTopPosation + moveX/4)           --speed /4
    end
end
----endregion
