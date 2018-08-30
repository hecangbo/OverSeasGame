--region *.lua
--Date
--此文件由[BabeLua]插件自动生成
require("Layer/MessageBox.lua")
UpdateTips =
{
	CheckingNewVersion = "正在检测新版本...",
	CheckDownloadFailed = "获取文件版本信息失败，是否重新获取？",
	ComfirmDownload = "检查到最新版本，需要更新的大小为%.2fM, 是否更新？",
	DownloadFileFailed = "下载更新文件失败，是否重试？",
	UpdateProgress = "更新进度：%.0f%%(%.1fK/%.1fK)",
	UpdateFullProgress = "更新进度: %.0f%%(%.1fK/%.1fK)",
    DownloadHotFix = "当前小版本更新，是否立即更新？",
}

UpdateType =
{
	CheckDownloadFailed = 1,
	ComfirmDownload = 2,
	DownloadFileFailed = 3,   --整包更新失败
	DownloadFullVerison = 4,  --整包更新
    DownloadHotFix = 5,  --小版更新
    DownloadCheckListFailed = 6, --小版本更新失败
}

UpdateLayer=
{
    _updater = nil,
    TAG_LABEL_PROMPTMESSAGE=18,
    TAG_SPRITE_LOADING = 17,
}

function UpdateLayer:new(o)
    o = o or {}
    setmetatable(o,self)
    self.__index = self
    return o
end

function UpdateLayer:create()
    return CocostudioHelper.createNode(UpdateLayer, "UpdateLayer.csb")
end

function UpdateLayer:onNodeLoaded()
    self:setTip(UpdateTips.CheckingNewVersion)

    local node = self._tg:getChildByTag(UpdateLayer.TAG_SPRITE_LOADING)
    node:runAction(cc.RepeatForever:create(cc.RotateBy:create(1, 1 / 2.0 * 360)))
end

function UpdateLayer:onEnter()

end

function UpdateLayer:onExit()

end


function UpdateLayer:setUpdater(updater)
	self._updater = updater
end

function UpdateLayer:setTip(args)
    local version = Utility.setLabelText(self, UpdateLayer.TAG_LABEL_PROMPTMESSAGE, args)
end


----
function UpdateLayer:checkDownloadFailed()
    self._status = UpdateType.CheckDownloadFailed
    self:showMessageBox(UpdateTips.CheckDownloadFailed)
end

function UpdateLayer:dectedNewVersion(size)
    self._status = UpdateType.ComfirmDownload
    local tips = string.format(UpdateTips.ComfirmDownload, size / (1000* 1000))
    self._updateSize = size
    self:showMessageBox(tips)
end

--小版本更新失败
function UpdateLayer:DownloadCheckListFailed()
    self._status = UpdateType.DownloadCheckListFailed
    self:showMessageBox(UpdateTips.DownloadFileFailed)
end

function UpdateLayer:downloadFailed()
    self._status = UpdateType.DownloadFileFailed
    self:showMessageBox(UpdateTips.DownloadFileFailed)
end

function UpdateLayer:dectedNewVersionFull(aSize)
    self._status = UpdateType.DownloadFullVerison
    local tips = string.format(UpdateTips.ComfirmDownload, aSize /1024)
    self._updateSize = aSize
    self:showMessageBox(tips)
end



function UpdateLayer:DownloadHotFix()
    self._status = UpdateType.DownloadHotFix
    self:showMessageBox(UpdateTips.DownloadHotFix)
end

--
function UpdateLayer:showMessageBox(tips)
    local lNormal = {"queding.png", "quxiao.png"}
    local lDown = {"queding1.png", "quxiao1.png"}

    local delegate = function(sender)
        self:onMessageBoxBtnClicked(sender)
        
        if Utility.IsIOS then
			if self._status == UpdateType.DownloadFullVerison then
				return false
			else
				return true
			end
		else
			return true
		end
        return true
    end

    local lpMessageBox
    if Utility.IsIOS then
		if self._status == UpdateType.DownloadFullVerison then
			lpMessageBox = MessageBox:create(lNormal, lDown, 1)
		else
			lpMessageBox = MessageBox:create(lNormal, lDown, 2)
		end
	else
		lpMessageBox = MessageBox:create(lNormal, lDown, 2)
	end

    lpMessageBox = MessageBox:create(lNormal, lDown, 2)
    local lpLabel = ccui.Text:create(tips, "Helvetica-Bold", 26)
    lpLabel:setColor(cc.c3b(255,255,255))
    --lpLabel:setPosition(cc.p(2,25))
    lpMessageBox:setLabel(lpLabel)
    lpMessageBox:setSize(480,260)
    lpMessageBox:setDelegate(delegate)
    lpMessageBox:pop(self._tg)
end

function UpdateLayer:onMessageBoxBtnClicked(sender)
    local action =
    {
        [1] = self.onBtnYes,
        [2] = self.onBtnNo,
    }
    self:callFunc(self, sender:getTag(), action)
end
    

function  UpdateLayer:callFunc(obj, tag, funcs)
    local func = funcs[tag]
    if func ~= nil then
        func(obj)
    end
end    

function UpdateLayer:onBtnYes()
    if self._updater ~= nil then
        local action=
        {
            [UpdateType.CheckDownloadFailed] = self._updater.checkDownload,
            [UpdateType.ComfirmDownload] = self._updater.download,
            [UpdateType.DownloadHotFix] = self._updater.checkDownload,
            [UpdateType.DownloadFileFailed] = self._updater.downloadfullverison,        --整包更新失败
            [UpdateType.DownloadFullVerison] = self._updater.downloadfullverison,       --整包更新
            [UpdateType.DownloadCheckListFailed] = self._updater.checkDownload,
        }
        self:callFunc(self._updater, self._status, action)
    end
end

function UpdateLayer:onBtnNo()
    if UpdateType.DownloadFileFailed ==self._status or 
        UpdateType.DownloadFullVerison ==self._status then
        self._updater:sendExitNotify()
    elseif UpdateType.DownloadFullVerison ~= self._status then
        self._updater:StartGame()
    else
        self._updater:sendExitNotify()
    end
end

--fileSize:下载速度， downSize：已经下载大小 ,totalSize:总共大小
function UpdateLayer:setDownloadProgress(fileSize, downSize, totalSize)     
    local progress = string.format(UpdateTips.UpdateProgress, 
             downSize * 100 /totalSize, downSize, totalSize)
    self:setTip(progress)
end            

function UpdateLayer:setDownloadFullProgress(downSize, fileSize, totalSize)
    if not totalSize or totalSize <=0 then return end
    local progress = string.format(UpdateTips.UpdateFullProgress,
            downSize * 100 / fileSize, downSize /1024, fileSize / 1024)
    self:setTip(progress)
end
--endregion
