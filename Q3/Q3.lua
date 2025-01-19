-- For this question I assumed that the intention was given a playerId and a memberName,
-- Remove the player with the name 'memberName' from the playerId's party.
-- To achieve this, I added a few checks to make sure the player is found and is
-- in a party to, only then, try and remove the memberName from the party.

function doRemovePartyMemberByName(playerId, memberName)

	local player = Player(playerId)
	if not player then
		return false
	end

	local party = player:getParty()
	if not party then
		return false
	end

	for _, value in pairs(party:getMembers()) do
		if value == Player(memberName) then
			party:removeMember(value)
			return true
		end
	end
	return false
end