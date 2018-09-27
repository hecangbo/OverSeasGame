--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
require("Layer/TianFuLayer.lua")
MainLayer=
{
    TAG_BUT_FASTBEGIN = 11,     --快速开始
    TAG_BUT_HALLLAYER = 12,     --游戏大厅
    TAG_BUT_CHANGEACCOUNT = 13,  --更换帐号
    TAG_BUT_WECHATSHARD = 21,   --微信分享
    TAG_BUT_TIANFUN = 32,       --天府
    TAG_IMAGE_BOARD = 24,
    TAG_LABEL_USERNAME = 25,    --用户名
    TAG_IMAGE_HEAD = 9,          --头像
    TAG_BG_PANEL = 4,
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
    self:registerBtnEvent()
end

function MainLayer:onEnterTransitionFinish()
    logOut("网络测试")
    self.bg_panel = self._tg:getChildByTag(self.TAG_BG_PANEL)
    
    self.userHead = self.bg_panel:getChildByTag(self.TAG_IMAGE_HEAD)
    self.userName = self.bg_panel:getChildByTag(self.TAG_IMAGE_BOARD):getChildByTag(self.TAG_LABEL_USERNAME)
    
    
    local wechatinfo = userManager:getWeChatUserInfo()
    if wechatinfo ~=nil then
        if wechatinfo.nickname~=nil then
            local wechatInfo_NickName = wechatinfo.nickname    
            self.userName:setString(wechatInfo_NickName)
        end

        --微信头像
        if wechatinfo.headimgurl~= nil then
            WeChat.LoadWeChatHeadImag(wechatinfo.headimgurl)
            WeChat.setWeChatHeadImag(self.bg_panel, self.TAG_IMAGE_HEAD)
        end
    end
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
        self.TAG_BUT_WECHATSHARD,
        self.TAG_BUT_TIANFUN,
    }
    CocostudioHelper.regBtnEvt(self, tags)
end

function MainLayer:unRegisterBtnEvent()
end

function  MainLayer:onPressEnded(sender)
    local action =
    {
        [self.TAG_BUT_WECHATSHARD]      = self.onBtnWechatShard,
        [self.TAG_BUT_TIANFUN]          = self.onBtnTianFu,
        [self.TAG_BUT_FASTBEGIN]        = self.onBtnFastBeginGame,
        [self.TAG_BUT_HALLLAYER]        = self.onBtnHall,
        [self.TAG_BUT_CHANGEACCOUNT]    = self.onBtnChangeAccount,
    }
    Utility.callFunc(self, sender:getTag(), action)
end

function MainLayer:onBtnWechatShard()
    logOut("微信分享")
    local appid = "wx71181ef0abf7bea9"
    local context = "QPGame分享内容"
    LuaJavaBinding.WeChatShard(appid, context)
end

function MainLayer:onBtnTianFu()
    scenemanager:run(TianFuLayer)
end
function MainLayer:onBtnFastBeginGame()
    --扫描
    LuaJavaBinding.onZXingScan()
end


function MainLayer:onBtnHall()
    --require("GF/WSProxy.lua")
end

function MainLayer:onBtnChangeAccount()
    --支付宝支付
    local pid = "2088702104062798"
end

----endregion
