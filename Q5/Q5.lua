-- For this spell, I simply tried to copy the video as closely as possible. I decided to have 12 areas instead of only 4 because
-- 

local spellVariables = {

	numberOfHits = 12,
	delay = 250,
}

local areas = { --
	[1] = {
		{0, 0, 0, 1, 0, 0 ,0},
		{0, 0, 0, 0, 1, 0 ,0},
		{0, 0, 0, 0, 0, 1, 0},
		{1, 0, 0, 2, 0, 0, 1},
		{0, 0, 0, 0, 0, 1, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0}
			
	},

	[2] = {
		{0, 0, 0, 0, 0, 0 ,0},
		{0, 0, 0, 0, 0, 0 ,0},
		{0, 1, 0, 1, 0, 0, 0},
		{0, 0, 0, 2, 0, 0, 0},
		{0, 1, 0, 0, 0, 0, 0},
		{0, 0, 1, 0, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 0}
	},

	[3] = {
		{0, 0, 0, 0, 0, 0 ,0},
		{0, 0, 1, 0, 1, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 1, 2, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0}
	},

	[4]  = {
		{0, 0, 0, 0, 0, 0 ,0},
		{0, 0, 0, 0, 0, 0 ,0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 2, 0, 0, 0},
		{0, 0, 0, 1, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 1, 0, 0, 0}
	}
}
local combats = {}

for i = 1, #areas do
	combats[i] = createCombatObject()
	combats[i]:setParameter(COMBAT_PARAM_TYPE, COMBAT_ICEDAMAGE)
	combats[i]:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_ICETORNADO)
	combats[i]:setFormula(COMBAT_FORMULA_LEVELMAGIC, 0, -200, 0, -500)
	combats[i]:setArea(createCombatArea(areas[i]))
end

	
function startSpell(cid, variant, area, delayBetweenHits)

	local creature = Creature(cid)
	if not creature or area > spellVariables.numberOfHits then
		return true
	end
	
	combats[(area % 4) + 1]:execute(creature, variant)
	addEvent(startSpell, delayBetweenHits, cid, variant, area + 1, delayBetweenHits)

end

function onCastSpell(creature, variant)
	startSpell(creature:getId(), variant, 1, spellVariables.delay)
	return true
end
