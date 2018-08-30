--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
--require ("GF/LayoutHelper.lua")

TouchEventType =
{
	began = 0,
	moved = 1,
	ended = 2,
	canceled = 3,
}

UIType =
{
	Widget = 1,
	CCLayer = 2,
}


CCSTouchEventType =
{
	TOUCH_EVENT_BEGAN = 0,
	TOUCH_EVENT_MOVED = 1,
	TOUCH_EVENT_ENDED = 2,
	TOUCH_EVENT_CANCELED = 3
}

CocostudioHelper =
{
    registerKeyboardEvent = function(tg, releaseFunc, pressedFunc)
        local listener = cc.EventListenerKeyboard:creat()
        if(releaseFunc~= nil) then
            listener:registerScriptHandler(releaseFunc, cc.Handler.EVENT_KEYBOARD_RELEASED)
        end
        if(pressedFunc~=nil) then
            listener:registerScriptHandler(pressedFunc, cc.Handler.EVENT_KEYBOARD_PRESSED)
        end
        tg:getEventDispatcher():addEventListenerWithSceneGraphPriority(listener, tg)
    end;

    --createUI begin
    createUI = function(layer, file)
		layer._tg = ccui.Widget:create()
		if (layer._tg ~= nil and file ~= nil) then
			local widget = ccs.GUIReader:getInstance():widgetFromJsonFile(file)
			layer._tg:addWidget(widget)

			local function eventHandler(eventType)
				local action =
				{
					["enter"] = layer.onEnter,
					["exit"] = layer.onExit,
					["enterTransitionFinish"] = layer.onEnterTransitionFinish,
					["exitTransitionStart"] = layer.onExitTransitionStart,
					["cleanup"] = layer.onCleanup
				}
				local func = action[eventType]
				if (func ~= nil) then
					func(layer)
				end

				if (eventType == "enterTransitionFinish") and notificationCenter ~= nil then
					local function onKeyboardEvent(keycode, event)
						if (keycode == cc.KeyCode.KEY_BACKSPACE and layer.onBackClicked ~= nil) then
							layer:onBackClicked()
						end
					end

					CocostudioHelper.registerKeyboardEvent(tg, onKeyboardEvent)
					notificationCenter:sendCrossSceneNotify()
				end

				if (eventType == "exit") then
					layer._tg:unregisterScriptTouchHandler()
					layer._tg:unregisterScriptKeypadHandler()
					layer._tg:unregisterScriptAccelerateHandler()
					layer._tg:getEventDispatcher():removeEventListenersForTarget(layer._tg)
				end
			end
			layer._tg:registerScriptHandler(eventHandler)

			local function touchHandler(sender, eventType)
				local action =
				{
					[CCSTouchEventType.TOUCH_EVENT_BEGAN] = { layer.onTouchBegan, sender.getTouchBeganPosition },
					[CCSTouchEventType.TOUCH_EVENT_MOVED] = { layer.onTouchMoved, sender.getTouchMovePosition },
					[CCSTouchEventType.TOUCH_EVENT_ENDED] = { layer.onTouchEnded, sender.getTouchEndPosition },
					[CCSTouchEventType.TOUCH_EVENT_CANCELED] = { layer.onTouchCanceled, nil },
				}

				local funcs = action[eventType]
				if (funcs[1] ~= nil) then
					if (funcs[2] ~= nil) then
						local pos = funcs[2](sender)
						return funcs[1](layer, pos.x, pos.y)
					else
						return funcs[1](layer)
					end
				else
					return false
				end
			end
			widget:addTouchEventListener(touchHandler)

			local function KeypadHandler(eventType)
				local action =
				{
					[KeypadEventType.kTypeBackClicked] = layer.onBackClicked,
					[KeypadEventType.kTypeMenuClicked] = layer.onMenuClicked,
				}
				local func = action[eventType]
				if (func ~= nil) then
					func(layer)
				end
			end
			layer._tg:registerScriptKeypadHandler(KeypadHandler)
		end

	end;
    --createUI end

    --createLayer begin
    createLayer = function(layerType, file)
        local layer = layerType:new()
        CocostudioHelper.createUI(layerType, file)

        layer._layer = CCLayer:create()
        layer._layer:addChild(layer._tg)

        local function touchHandler(eventType,x,y)
            local action =
            {
                ["began"] = layer.onTouchBegan,
                ["moved"] = layer.onTouchMoved,
                ["ended"] = layer.onTouchEnded,
                ["cancelled"] = layer.onTouchCancelled,
            }
            local func = action[eventType]
            if(func ~= nil) then
                return func(Layer,x,y)
            else
                return false
            end
        end
        layer._layer:registerScriptTouchHandler(touchHandle, false, 0, true)
        
        if (layer.onNodeLoaded ~= nil) then
            layer:onNodeLoaded()
        end

        LayoutHelper:shareInstance():AdjustBackground(layer._tg)
        
        return layer
    end;
    --createLayer end



    getLayerTouchEventHanlder = function(target)
		function touchHandler(sender, eventType)
			local action =
			{
				[CCSTouchEventType.TOUCH_EVENT_BEGAN] = { target.onTouchBegan, sender.getTouchBeganPosition },
				[CCSTouchEventType.TOUCH_EVENT_MOVED] = { target.onTouchMoved, sender.getTouchMovePosition },
				[CCSTouchEventType.TOUCH_EVENT_ENDED] = { target.onTouchEnded, sender.getTouchEndPosition },
				[CCSTouchEventType.TOUCH_EVENT_CANCELED] = { target.onTouchCanceled, nil },
			}
			local funcs = action[eventType]
			if (funcs[1] ~= nil) then
				if (funcs[2] ~= nil) then
					local pos = funcs[2](sender)
					return funcs[1](target, pos.x, pos.y)
				else
					return funcs[1](target)
				end
			else
				return false
			end
		end
        return touchHandler
	end;

    
    --创建csb
    createNode = function(node, file)
        local lItem = node:new()
        lItem._tg = cc.CSLoader:createNode(file)
        lItem._root = lItem._tg:getChildByName("bg_panel")


        lItem.addChild = function(aWidget)
            lItem._root:addChild(aWidget)
        end

        if(lItem.onNodeLoaded ~= nil) then
            lItem:onNodeLoaded()
        end

        local function eventHandler(eventType)
            local action =
            {
                ["enter"] = lItem.onEnter,
                ["exit"] = lItem.onExit,
                ["enterTransitionFinish"] = lItem.onEnterTransitionFinish,
                ["exitTransitionStart"] = lItem.onExitTransitionStart,
                ["cleanup"] = lItem.onCleanup 
            }

            --        if eventType == "enterTransitionFinish" then
            --                local function onKeyboardEvent(keycode, event)
            --				    if (keycode == cc.KeyCode.KEY_BACK and lItem.onBackClicked ~= nil) then
            --					    lItem:onBackClicked()
            --				    end
            --			    end
            --                CocostudioHelper.registerKeyboardEvent(lItem._root, onKeyboardEvent)
            --            end

            local func = action[eventType]
                if(func~= nil) then
                    func(lItem)
                end
         end


        lItem._tg:registerScriptHandler(eventHandler)
    
        local touchHandler = CocostudioHelper.getLayerTouchEventHanlder(lItem)
        lItem._root:setTouchEnabled(true)
        lItem._root:addTouchEventListener(touchHandler)

        return lItem
    end;


     regBtnEvt = function(aLayer, aTags)
        local function touchEvent(sender, eventType)
            local action = 
            {
                [CCSTouchEventType.TOUCH_EVENT_BEGAN] = aLayer.onPressBegan,
                [CCSTouchEventType.TOUCH_EVENT_MOVED] = aLayer.onPressMove,
                [CCSTouchEventType.TOUCH_EVENT_ENDED] = aLayer.onPressEnded,
                [CCSTouchEventType.TOUCH_EVENT_CANCELED] = aLayer.onPressCanceled,
            }
            local func = action[eventType]
            if(func~=nil) then
                return func(aLayer,sender,eventType)
            else
                return false
            end
        end

        for i,v in ipairs(aTags) do
            local button = Utility.getWidget(aLayer,v)
            button = tolua.cast(button,"ccui.Widget")
            button:setTouchEnabled(true)
            button:addTouchEventListener(touchEvent)
        end
    end;
};
--endregion
