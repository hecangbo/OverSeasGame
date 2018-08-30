Queue = {}
function Queue.new()
    return {first = 0, last = -1}
end

function Queue.pushFront(queue, value)
    local first = queue.first - 1;
    queue.first = first
    queue[first] = value
end

function Queue.pushBack(queue, value)
    local last = queue.last + 1
    queue.last = last
    queue[last] = value
end

function Queue.popFront(queue)
    local first = queue.first
    if first > queue.last then
		return nil
	end
    local value = queue[first]
    queue[first] = nil
    queue.first = first + 1
    return value
end

function Queue.popLast(queue)
    local last = queue.last
    if queue.first > last then
		return nil
	end
    local value = queue[last]
    queue[last] = nil
    queue.last = last - 1     
	return value
end

function Queue.front(queue)
	return queue[queue.first]
end

function Queue.isEmpty(queue)
	return queue.first > queue.last;
end

function Queue.count(queue)
    return queue.last - queue.first + 1
end