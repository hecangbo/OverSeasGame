--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
WebProxy=
{
}

function WebProxy:new(o)
    o = o or {}
    setmetatable(o,self)
    self.__index = self
    return o
end

function WebProxy:setUrl(sUrl)
    self._url = sUrl
end
function WebProxy:getUrl()
    return self._url
end


function wsSendTextOpen(strData)
    if strData~= nil then
        print(strData)
        print(strData)
    end
end

function wsSendTextMessage(strData)
    -- receiveTextTimes= receiveTextTimes + 1
    local strInfo= "response text msg: "..strData
    print(strInfo)
    print(strInfo)
end

function wsSendTextClose(strData)
    print("_wsiSendText websocket instance closed.")
    sendTextStatus = nil
    wsSendText = nil
end

function wsSendTextError(strData)
    print("sendText Error was fired")
end



function WebProxy:init()
    
    if nil ~= wsSendText then
            wsSendText:registerScriptHandler(wsSendTextOpen,0)
            wsSendText:registerScriptHandler(wsSendTextMessage,1)
            wsSendText:registerScriptHandler(wsSendTextClose,2)
            wsSendText:registerScriptHandler(wsSendTextError,3)
    end
    if 1 == wsSendText:getReadyState() then
        wsSendText:sendString("Hello WebSocket中文, I'm a text message.")
    end
end

function WebProxy:onSend(url)
    wsSendText = cc.WebSocket:create(url)
end

local webproxy = WebProxy:new()
webproxy:setUrl("ws://echo.websocket.org")
wsSendText   = cc.WebSocket:create(webproxy:getUrl())
webproxy:init()

--webproxy:setUrl("ws://192.168.32.1")
webproxy:setUrl("ws:127.0.0.1")
wsSendText   = cc.WebSocket:create(webproxy:getUrl())
webproxy:init()
--endregion
