--region *.lua
--Date
--此文件由[BabeLua]插件自动生成

MainLayer=
{
    TAG_BUT_FASTBEGIN = 11,     --快速开始
    TAG_BUT_HALLLAYER = 12,     --游戏大厅
    TAG_BUT_CHANGEACCOUNT = 13  --更换帐号
}

function MainLayer:new(o)
    o = o or {}
    setmetatable(o,self)
    self.__index = self
    return o
end

function MainLayer:create()
    return CocostudioHelper.createNode(self,"mainmenu.csb")
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
end

function MainLayer:onExit()
    self:unRegisterBtnEvent()
end
function MainLayer:registerBtnEvent()
    local tags =
    {
        self.TAG_BUT_FASTBEGIN,
        self.TAG_BUT_HALLLAYER,
        self.TAG_BUT_CHANGEACCOUNT,
    }
    CocostudioHelper.regBtnEvt(self, tags)
end

function MainLayer:unRegisterBtnEvent()
end

function  MainLayer:onPressEnded(sender)
    local action =
    {
        [self.TAG_BUT_FASTBEGIN]        = self.onBtnFastBeginGame,
        [self.TAG_BUT_HALLLAYER]        = self.onBtnHall,
        [self.TAG_BUT_CHANGEACCOUNT]    = self.onBtnChangeAccount,
    }
    Utility.callFunc(self, sender:getTag(), action)
end


function MainLayer:onBtnFastBeginGame()
    logOut("onBtnFastBeginGame")
    logOut("onBtnFastBeginGame")
    require("cocos/Networkconstants.lua")
    local xhr = cc.XMLHttpRequest:new()
    xhr.responseType = cc.XMLHTTPREQUEST_RESPONSE_STRING
    --xhr:open("GET", "http://httpbin.org/get")
    xhr:open("POST","127.0.0.1:4321")
    local function onReadyStateChanged()
        if xhr.readyState == 4 and (xhr.status >= 200 and xhr.status < 207) then
            print(xhr.response)
        end
            xhr:unregisterScriptHandler()
    end
    xhr:registerScriptHandler(onReadyStateChanged)
    xhr:send()
end


function MainLayer:onBtnHall()
    require("GF/WSProxy.lua")
    logOut("............")
end
--endregion
