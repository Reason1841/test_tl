


local config = {
	maxDashDistance = 5,
	numberOfAfterImages = 5,
	delay = 100
}

function pushCreature(cid, maxDistance)

	local creature = Creature(cid)
	if not creature then
		return true
	end

	if maxDistance <= 0 then
		creature:sendExtendedOpcode(101,"")
		return true
	end

	local creaturePos = creature:getPosition()
	local creatureDir = creature:getDirection()
	if creatureDir == 0 then --North
		creaturePos.y = creaturePos.y - 1
	elseif creatureDir == 1 then --East
		creaturePos.x = creaturePos.x + 1
	elseif creatureDir == 2 then --South
		creaturePos.y = creaturePos.y + 1
	elseif creatureDir == 3 then --West
		creaturePos.x = creaturePos.x - 1
	end

	local nextTile = Tile(creaturePos)

	if not nextTile or nextTile:hasFlag(TILESTATE_BLOCKSOLID) then
		creature:sendExtendedOpcode(101, "")
		return true
	end

	creature:teleportTo(creaturePos, true)
	addEvent(pushCreature, config.delay, creature:getId(), maxDistance - 1)

end

function onCastSpell(creature, variant)

	creature:sendExtendedOpcode(100, tostring(config.numberOfAfterImages))
	pushCreature(creature:getId(), config.maxDashDistance)
	return true
end
