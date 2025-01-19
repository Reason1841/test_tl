-- For this question what I did was add a new argument to the function 'releaseStorage'
-- so that it can be used for other storages. Also, instead of passing the player object
-- as an argument, I instead decided to pass the player's id. This combined with the removal of
-- the addEvent should prevents potential issues that could arise if the player goes offline between
-- the function call and its execution.

local function releaseStorage(pid, storageId)
    local player = Player(pid)
    if player then
        player:setStorageValue(storageId, -1)
        return true
    end
    return false
end

function onLogout(player)
    if player:getStorageValue(1000) == 1 then
		releaseStorage(player:getId(), 1000)
    end
	return true
end