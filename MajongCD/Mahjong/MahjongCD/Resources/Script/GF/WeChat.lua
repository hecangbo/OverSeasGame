--region NewFile_1.lua
--Author : hechangbo
--Date   : 2018/9/11
--此文件由[BabeLua]插件自动生成

WeChat={

    --获取授权信息
    WeChatLogin= function()
        local appid = "wx71181ef0abf7bea9"
        local appsecret ="3d70a4f0915643e16f7262f3c4068821"
        local accessTokenURL ="https://api.weixin.qq.com/sns/oauth2/access_token?appid=%s&secret=%s&code=%s&grant_type=authorization_code"
        LuaJavaBinding.WeChatLoginInfo(appid, appsecret, accessTokenURL)
    end;

    --验证授权是否有效
    --省略不做


    --获取微信用户信息
    getWeChatUserInfo = function(info)  --info format is json
        --local str = json.decode(info)
        
    end;

    --下载微信头像
    LoadWeChatHeadImag = function(sUrl)
        local Callback = function(info)
           path = cc.FileUtils:getInstance():getWritablePath()
           path =path .."headImg.jpg";
           logOut(path)
           file = io.open(path,"wb")
           file:write(info)
           file:close()
        end
        HttpBindHelper.requestHeadImag(sUrl, Callback)
    end;

    setWeChatHeadImag = function(tg, tag, isWidget)
        if(isWidget == nil) then
			isWidget = false
		end

        local lHeadImag = nil
		if(isWidget) then
			lHeadImag = tg:getChildByTag(tag)
		else
			lHeadImag = tg:getChildByTag(tag)
		end
                
        local lName = cc.FileUtils:getInstance():getWritablePath() .. "headImg.jpg"

        if(Utility.isFileExists(lName)) then
            lHeadImag:loadTexture(lName)        
        end

        return true
    end;
}

--endregion
