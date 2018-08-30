require("GF/Queue.lua")
director = cc.Director:getInstance()
SceneManager =
{
	_scenes = Queue.new()
}

function SceneManager:new(o)
    o = o or {}
    setmetatable(o,self)
    self.__index = self
    return o
end


function SceneManager:run(layer, checkSession)
    local runingScene = director:getRunningScene()
    local scene = nil
    if runingScene == nil then
        scene, tg = self:createScene(layer,false)
        --LayoutHelper:shareInstance():layout(tg)
        director:runWithScene(scene)
    else
        scene, tg = self:createScene(layer,true)
        --LayoutHelper:shareInstance():layout(tg)
        director:replaceScene(scene)
    end
    Queue.pushFront(self._scenes, layer)
end

function SceneManager:createScene(layer, trans)
    local scene = cc.Scene:create()
    local tg
    if layer._type == UIType.CCLayer then
        tg = layer:create()._layer
    else
        tg = layer:create()._tg
    end
    scene:addChild(tg)
    if(trans) then
        return cc.TransitionFade:create(0.3,scene), tg
    else
        return scene, tg
    end
end

function SceneManager:close(obj)
    if obj~=nil then
        obj._tg:removeFromParent(true)
        --obj._tg:removeFromParentAndCleanup(true)
        Queue.popFront(self._scenes)
    end
    if Queue.isEmpty(self._scenes) == false then
        local scene, tg = self:createScene(Queue.front(self._scenes),true)
        cc.Director:getInstance():replaceScene(scene)
    end
end

sceneManager = SceneManager:new()