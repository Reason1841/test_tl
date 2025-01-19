
-- For this question I decided to create a module. When the window is created in-game,
-- a recursive function is called to keep updating the button position every 100 milliseconds,
-- is this case the button moves 15 pixels to the right until it's clicked or it reaches the border
-- of the window. if the button is clicked, then it's position is set to a new random one
-- inside the window.

jumpWindow = {}
modWindow = nil
modWindowButton = nil

function updateButtonPosition()

	if not modWindowButton then
		return false
	end
	
    if(modWindowButton:getMarginRight() >= 270) then
        modWindowButton:setMarginRight(20)
    else
        modWindowButton:setMarginRight(modWindowButton:getMarginRight() + 15)
    end
    scheduleEvent(updateButtonPosition, 150)
end

function jumpWindow.init()
	connect(g_game, {onGameStart = jumpWindow.run})
end

function jumpWindow.terminate()
	disconnect(g_game, {onGameStart = jumpWindow.run})
end

function jumpWindow.run()
	modWindow = g_ui.displayUI('jumpWindow.otui')
	modWindowButton = modWindow:getChildById("jumpWindowButton")
	updateButtonPosition()
end

function jumpWindow.onButtonClicked()
	modWindowButton:setMarginBottom(math.random(0, 270))
end

function jumpWindow.destroy()
	modWindowButton = nil
	modWindow:destroy()
end