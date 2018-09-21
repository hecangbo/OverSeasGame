require("GF/Utility.lua")
require("GF/LuaJavaBridge.lua")
require("Mahjong.lua")
print("game start")
require "cocos.init"

local function main()
    cc.SpriteFrameCache:getInstance():addSpriteFrames("loading.plist")
    cc.SpriteFrameCache:getInstance():addSpriteFrames("scene.plist")
    --sceneManager:run(LoadingLayer)
    require("Update.lua")
end




xpcall(main, __G__TRACKBACK__)