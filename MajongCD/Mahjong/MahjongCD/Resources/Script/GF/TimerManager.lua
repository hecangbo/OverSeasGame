
TimerEvent = 
{
    Updated = 1,
    Cancled = 2,
    Paused = 3,
    End    = 4,
}

TimerManager = 
{
    timers = {},

    findTimers = function(name)
        assert(type(name) == "string", "Timer's name must be a string type!")
        local timers = {}
        for k, v in ipairs(TimerManager.timers) do 
            if v._name == name then
                table.insert(timers, v._timer)
            end
        end
        return timers
    end;

    remove = function(name)
        assert(type(name) == "string", "Timer's name must be a string type!")
        local count = #TimerManager.timers
        for n = 1, count do 
            local timerInfo = TimerManager.timers[n]
            if timerInfo._timer ~= nil and not timerInfo._timer.bPaused then
                timerInfo._timer:cancle()
                timerInfo = nil 
            end
        end
    end;

    removeAll = function()
        for k, v in ipairs(TimerManager.timers) do 
            if v._timer ~= nil and not v._timer.bPaused then
                --log("remove timer :"..v._name)
                v._timer:cancle()
            end
        end
        TimerManager.timers = {}
    end;

    createTimer = function(t, f, name)
        if t == nil then return nil end
        
        local Timer 
        if type(t) == "function" then
            Timer = {target = nil, func = t}
        else
            Timer = {target = t, func = f}
        end

        Timer.name = ""
        Timer.bPaused = false
        Timer.bRunning = false
        Timer.eclipses =  0
        function Timer:start(t, b)
            t = t or 0
            if t < 0 then t = 0 end
            if b == nil then b = false end

            local updater = function(f)                
                self.eclipses = self.eclipses + f
                --log("update timer:"..Timer.name)
                if self.bPaused == false then
                    if self.func ~= nil and self.target ~= nil then
                        self.func(self.target, {frame = f, eclipses = self.eclipses, event = TimerEvent.Updated})
                    elseif self.func ~= nil and self.target == nil then
                        self.func(self, {frame = f, eclipses = self.eclipses, event = TimerEvent.Updated})
                    end
                end
            end

            --if not self.bPaused then
                local lScheduler = cc.Director:getInstance():getScheduler()
                self.bPaused = b
                self.scheduler = lScheduler:scheduleScriptFunc(updater, t, b)
                self.bRunning = true
            --end
        end

        function Timer:setCallback(call)
            self.func = call
        end

        function Timer:setEclipses(t)
            if not self.bPaused then
                t = t or 0
                if t <= 0 then t = 0 end
                self.eclipses = t
            end
        end

        function Timer:getEclipses()
            return self.eclipses
        end

        function Timer:cancle()
            if self.scheduler ~= nil then
                self.bPaused = true
                local lScheduler = cc.Director:getInstance():getScheduler()
                lScheduler:unscheduleScriptEntry(self.scheduler)
                --self.target = nil
                --self.func = nil
                self.eclipses = 0
                self.scheduler = nil
                self.bRunning = false
            end
        end

        function Timer:isRunning()
            return self.bRunning
        end

        function Timer:setCallback(target, func)
            if target then
                self.target = target
            end
            if func then
                self.func = func
            end
        end

        function Timer:setName(name)
            assert(type(name) == "string", "Timer's name must be a string type!")
            self.name = name
            for k, v in ipairs(TimerManager.timers) do 
                if v._timer == Timer then
                    v._name = name
                    break
                end
            end
        end

        function Timer:isValid()
            return not self._bPaused
        end

        table.insert(TimerManager.timers, {_name = "", _timer = Timer})
        return Timer
    end;
}
