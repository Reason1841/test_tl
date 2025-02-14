afterimage = {}

function drawAfterImages(protocol, opcode, buffer)

	local localPlayer = g_game.getLocalPlayer()
	if localPlayer then
		localPlayer:enableAfterimages(tonumber(buffer))
	end
end

function removeAfterImages(protocol, opcode, buffer)
	local localPlayer = g_game.getLocalPlayer()
	if localPlayer then
		localPlayer:removeAfterimages()
	end
end

function afterimage.init()
	ProtocolGame.registerExtendedOpcode(100, drawAfterImages)
	ProtocolGame.registerExtendedOpcode(101, removeAfterImages)
end

function afterimage.terminate()
	ProtocolGame.unregisterExtendedOpcode(100)
	ProtocolGame.unregisterExtendedOpcode(101)
end