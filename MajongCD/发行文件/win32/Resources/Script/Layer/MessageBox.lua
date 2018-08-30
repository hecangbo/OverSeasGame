require('cocos/Cocos2d.lua')
require("cocos/GuiConstants.lua")

MessageBox = 
{
	TAG_CloseBtn=100,

}

function MessageBox:new(o)
	o = o or {} 
	setmetatable(o,self)
	self.__index = self
	return o
end

function MessageBox:registerBtnTouchEvent(btn)
	local function touchEvent(sender, eventType)
		local action = 
		{  
			[TouchEventType.began] = self.onPressBegan,
			[TouchEventType.moved] = self.onPressMove,
			[TouchEventType.ended] = self.onPressEnded,
			[TouchEventType.canceled] = self.onPressCanceled,
		}
		local func = action[eventType]
		if(func ~= nil) then
			return func(self, sender, eventType)
		else
			return false
		end
	end

	btn:addTouchEventListener(touchEvent)
end

function MessageBox:registerLayerEvent()
	local function eventHandler(eventType)  
		local action = 
		{  
			["enter"] = self.onEnter,
			["exit"] = self.onExit,
		}
		local func = action[eventType]
		if(func ~= nil) then
			func(self)
		end
	end  
	self._tg:registerScriptHandler(eventHandler)
end

function MessageBox:create(normalBtn, downBtn, btnCount)
	local box = MessageBox:new()
	box._tg = ccui.Widget:create()
	box._layer = ccui.Layout:create()
	--box._layer:setBackGroundColorType(BackGroundColorType.SOLID)
	box._layer:setBackGroundColor(cc.c3b(0, 0, 0))
	box._layer:setBackGroundColorOpacity(100)
	local lSize = cc.Director:getInstance():getVisibleSize()
	box._layer:setContentSize(lSize)
	box._tg:addChild(box._layer)

	box._bg = ccui.ImageView:create()
	box._bg:setScale9Enabled(true)
	--box._bg:setPosition(cc.p(640, 360))
    box._bg:setPosition(cc.p(480, 360))
	box._bg:loadTexture("message_box_background.png",ccui.TextureResType.plistType)
	box._bg:setScale9Enabled(true)
	box._bg:setContentSize(cc.size(480,320))
	box._tg:addChild(box._bg)

	box._layer:setTouchEnabled(true)
	box._layer:setSwallowTouches(true)

	for i = 1, btnCount do
		local button = ccui.Button:create()
		button:setTag(i)
		tolua.cast(button, "ccui.Widget"):setLocalZOrder(100)
		button:setTouchEnabled(true)
		button:loadTextures(normalBtn[i], downBtn[i], "",ccui.TextureResType.plistType)
		box._tg:addChild(button)
		box:registerBtnTouchEvent(button)
	end
	box._btnCount = btnCount

	box:registerLayerEvent()
	box:layout()
	return box
end

function MessageBox:pop(touchGroup)
	touchGroup:addChild(self._tg, 100)
	local lAction1 = cc.ScaleTo:create(0.08, 1.25)
	local lAction2 = cc.ScaleTo:create(0.1, 1)
	self._layer:runAction(cc.Sequence:create(lAction1, lAction2))
end

function MessageBox:onEnter()
	
end

function MessageBox:setLabel(label)
	self._contentLabel = label
	self._tg:addChild(self._contentLabel)
	
end

function MessageBox:setBackground(aImage)
	self._bg:loadTexture(aImage, ccui.TextureResType.localType)
end

function MessageBox:setSize(x, y)
	self._bg:setContentSize(cc.size(x, y));
	self:layout()
end

function MessageBox:setDelegate(func)
	self._delegate = func
end

function MessageBox:setContentLabelAlignment(alignment)
	self._alignment = alignment
end

function MessageBox:layout()

	if(self._contentLabel) then
		local lBoxSize = self._bg:getContentSize()
		local x=self._bg:getPositionX()
		local y=self._bg:getPositionY()
		local averageWidth = (lBoxSize.width - 40) / self._btnCount
		local nodeHeight=0
		for i = 1, self._btnCount do
			local x = self._bg:getPositionX() + (i - (self._btnCount + 1) * 0.5) * averageWidth
			local pNode =self._tg:getChildByTag(i)
			local height = self._bg:getPositionY()-self._bg:getContentSize().height * 0.5 + pNode:getContentSize().height * 0.5+35
			nodeHeight=pNode:getContentSize().height
			pNode:setPosition(cc.p(x, height))
		end
		local lSize = cc.size(lBoxSize.width - 60, lBoxSize.height)
		self._contentLabel:setTextHorizontalAlignment(0)
		self._contentLabel:setTextVerticalAlignment(0)
		self._contentLabel:ignoreContentAdaptWithSize(false)
		self._contentLabel:setTextAreaSize(cc.size(lSize.width, lSize.height/2))
		self._contentLabel:setPosition(cc.p(x,y+50))
	end

	if self._closeBtn~=nil then
		local posX=self._bg:getPositionX()
		local posY=self._bg:getPositionY()
		local size=self._bg:getContentSize()
		self._closeBtn:setPosition(cc.p(posX+size.width/2-10,posY+size.height/2-10))
	 end
end

function MessageBox:onPressEnded(sender)
	if(self._delegate ~= nil) then
		self._delegate(sender)
	end
	self._tg:removeFromParent()
end


function MessageBox:addCloseBtn()

	 local closeButton=ccui.Button:create()
	 closeButton:setTouchEnabled(true)
	 self:registerBtnTouchEvent(closeButton)
	 closeButton:loadTextures("close.png","close1.png","",ccui.TextureResType.plistType)
	 self._tg:addChild(closeButton)
	 self._closeBtn=closeButton
	 closeButton:setTag(MessageBox.TAG_CloseBtn)

end
