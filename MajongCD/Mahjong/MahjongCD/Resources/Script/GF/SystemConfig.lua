--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
--资源路径
local director = cc.Director:getInstance()
local glView   = director:getOpenGLView()
if nil == glView then
    glView = cc.GLViewImpl:createWithRect("四川Mahjong", cc.rect(0,0,960,640))
    director:setOpenGLView(glView)
end
cc.Director:getInstance():getOpenGLView():setDesignResolutionSize(960, 640, 4)
--turn on display FPS
director:setDisplayStats(true)

--set FPS. the default value is 1.0/60 if you don't call this
director:setAnimationInterval(1.0 / 60)


--endregion
