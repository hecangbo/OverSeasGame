--region NewFile_1.lua
--Author : hechangbo
--Date   : 2018/9/12
--此文件由[BabeLua]插件自动生成

UserManager=
{
    --微信
    _weChat={
        --info={}         -- 个人信息
        info = "",
    },
}

function UserManager:new(o)
	o = o or {}
	setmetatable(o,self)
	self.__index = self
	return o
end


function UserManager:setWeChatUserInfo(msg)
    self._weChat.info = json.decode(msg)
end
function UserManager:getWeChatUserInfo()
    return self._weChat.info
end

--endregion
