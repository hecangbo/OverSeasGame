require("cocos/NetworkConstants.lua")
HttpClient = 
{
    sendRequest = function(url, httpType, callback)
        local request = LuaHttpRequest:create()
        request:setUrl(url)
        request:setRequestType(httpType)
        request:setResponseScriptCallback(callback)
        CCHttpClient:getInstance():send(request)
        request:release()
    end;

	sendRequestWithUserData = function(url, httpType, callback, aUserData)
		local request = LuaHttpRequest:create()
		request:setUrl(url)
		request:setRequestType(httpType)
		request:setResponseScriptCallback(callback)
		tolua.cast(request, 'CCHttpRequest'):setUserData(aUserData)
		CCHttpClient:getInstance():send(request)
		request:release()
	end;

	sendRequestWithPostData = function(url, httpType, callback, postData)
		local request = LuaHttpRequest:create()
		request:setUrl(url)
		request:setRequestType(httpType)
		request:setResponseScriptCallback(callback)
		tolua.cast(request, 'CCHttpRequest'):setRequestData(postData, string.len(postData))
		CCHttpClient:getInstance():send(request)
		request:release()
	end;


    sendXMLRequest = function(url, httpType, callback)
        local xhr = cc.XMLHttpRequest:new()
        xhr.responseType = cc.XMLHTTPREQUEST_RESPONSE_JSON        
        --url = "http://thirdwx.qlogo.cn/mmopen/vi_32/zAVNw1mDwndtmh8BMvaNy1atooKriaMYbpic84ZGlOdFNKVkfuusUkPEMcMdPUgLwibwjsicDAgNtDMop2rX0oh3iag/132"
        xhr:open(httpType, url)
        local function onReadyStateChanged()
            if xhr.readyState == 4 and (xhr.status >= 200 and xhr.status < 207) then
                callback(xhr.response)
            else
                print("error:xhr.readyState is:", xhr.readyState, "xhr.status is: ",xhr.status)
            end
            xhr:unregisterScriptHandler()
        end
        xhr:registerScriptHandler(onReadyStateChanged)
        xhr:send()
    end;

}