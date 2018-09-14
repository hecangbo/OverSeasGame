--region NewFile_1.lua
--Author : hechangbo
--Date   : 2018/9/12
--此文件由[BabeLua]插件自动生成
CCHttpRequest={
    kHttpGet = "GET",
    kHttpPost = "POST",
}
HttpBindHelper = {
    bindWeChatUserInfo = function(access_token, openid,callback)
        local url = string.format(Config.getWeChatUserInfoURL(), access_token, openid)
        logOut(url)
        HttpClient.sendXMLRequest(url, CCHttpRequest.kHttpGet, callback)
    end;


    requestHeadImag = function (sUrl, callBack)
        logOut("头像:")
        HttpClient.sendXMLRequest(sUrl, CCHttpRequest.kHttpGet, callBack) 
    end;
}


--endregion
