
LuaNotificationCenter =
{
	_center = {},
	_notify = Queue.new(),

    _syncMsg = {},
    _triggerMsg = {}
}

 function LuaNotificationCenter:new(o)
	o = o or {}
	setmetatable(o,self)
	self.__index = self
	return o
end


function LuaNotificationCenter:init()    
    local fOnTick = function()
        self:onTick()
    end
    self._scheduler = director:getScheduler():scheduleScriptFunc(fOnTick, 0.1, false)
end

function LuaNotificationCenter:onTick()    
    for k,msg in ipairs(self._syncMsg) do
        self:post(msg.name,msg.data)
    end

    self._syncMsg = {}
end

function LuaNotificationCenter:ReadyPost(name,data)
    self._triggerMsg[name] = data
end

function LuaNotificationCenter:Trigger(name)
    local data = self._triggerMsg[name]
    if data then
        self:SyncPost(name,data)
        self._triggerMsg[name] = nil
    end
end

function LuaNotificationCenter:SyncPost(name, data)
    local msg = {}
    msg.name = name
    msg.data = data
    table.insert(self._syncMsg, msg)    
end

	
function LuaNotificationCenter:register(obj, func, name)
	local lExist = false
	for _,v in pairs(self._center) do
		if v.name == name and v.obj == obj then
			lExist = true
			break
		end
	end

	if not lExist then
		local data = {}
		data.obj = obj
		data.func = func
		data.name = name
		table.insert(self._center, data)
	end
end

function LuaNotificationCenter:unregister(obj, name)
	local i = 1
	while i <= #self._center do
		if self._center[i].name == name and self._center[i].obj == obj then 
			table.remove(self._center, i)
			break
		else
			i = i + 1
		end
	end
end

function LuaNotificationCenter:unregisterAll(obj)
	if(obj ~= nil) then
		local i = 1
		while i <= #self._center do
			if self._center[i].obj == obj then 
				table.remove(self._center, i)
			else
				i = i + 1
			end
		end
	end
end

function LuaNotificationCenter:post(name, data)
	for i,v in pairs(self._center) do
		if(v.name == name) then
			v.func(v.obj, data)
            self._triggerMsg[name] = nil            
		end
	end
end

function LuaNotificationCenter:crossScenePost(name, data)
	Queue.pushFront(self._notify, {k = name, v = data})
end

function LuaNotificationCenter:sendCrossSceneNotify()
	local data = Queue.popFront(self._notify)

	while data ~= nil do
		self:post(data.k, data.v)
		data = Queue.popFront(self._notify)
	end
end