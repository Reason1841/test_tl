// Note: Due to the default drawing order in the standard otclient, when facing west or north, 
// clones are drawn before the tiles to the east and south, respectively. This causes the clones 
// to be obscured and not appear as intended. Also, I tried my best to get the shader to work
// on the outfits in this version of OTC but I ultimately couldn't get it to function properly.

Creature::Creature() : Thing()
{
    m_id = 0;
    m_healthPercent = 100;
    m_speed = 200;
    m_direction = Otc::South;
    m_walkAnimationPhase = 0;
    m_walkedPixels = 0;
    m_walkTurnDirection = Otc::InvalidDirection;
    m_skull = Otc::SkullNone;
    m_shield = Otc::ShieldNone;
    m_emblem = Otc::EmblemNone;
    m_type = Proto::CreatureTypeUnknown;
    m_icon = Otc::NpcIconNone;
    m_lastStepDirection = Otc::InvalidDirection;
    m_nameCache.setFont(g_fonts.getFont("verdana-11px-rounded"));
    m_nameCache.setAlign(Fw::AlignTopCenter);
    m_footStep = 0;
    m_speedFormula.fill(-1);
    m_outfitColor = Color::white;
    m_hasAfterImage = false;
    m_numberOfAfterImages = 0;
}


void Creature::internalDrawOutfit(Point dest, float scaleFactor, bool animateWalk, bool animateIdle, Otc::Direction direction, LightView* lightView)
{
    g_painter->setColor(m_outfitColor);

    // outfit is a real creature
    if (m_outfit.getCategory() == ThingCategoryCreature) {

        if (m_hasAfterImage) {
            animateWalk = false;
        }
        int animationPhase = animateWalk ? m_walkAnimationPhase : 0;

        if (isAnimateAlways() && animateIdle) {
            int ticksPerFrame = 1000 / getAnimationPhases();
            animationPhase = (g_clock.millis() % (ticksPerFrame * getAnimationPhases())) / ticksPerFrame;
        }

        // xPattern => creature direction
        int xPattern;
        if (direction == Otc::NorthEast || direction == Otc::SouthEast)
            xPattern = Otc::East;
        else if (direction == Otc::NorthWest || direction == Otc::SouthWest)
            xPattern = Otc::West;
        else
            xPattern = direction;

        int zPattern = 0;

        if (m_hasAfterImage) {

            auto datType = rawGetThingType();
            Color color = g_painter->getColor();
            Otc::Direction dir = getDirection();

            for (uint8_t i = 0; i < m_numberOfAfterImages; i++) {
                g_painter->setColor(color);
                switch (dir) {
                case 0: // North
                    datType->draw(Point(dest.x, dest.y + (i * 25)), scaleFactor, 0, xPattern, 0, zPattern, animationPhase, lightView);
                    break;
                case 1: // East
                    datType->draw(Point(dest.x - (i * 25), dest.y), scaleFactor, 0, xPattern, 0, zPattern, animationPhase, lightView);
                    break;
                case 2: // South
                    datType->draw(Point(dest.x, dest.y - (i * 25)), scaleFactor, 0, xPattern, 0, zPattern, animationPhase, lightView);
                    break;
                case 3: // West
                    datType->draw(Point(dest.x + (i * 25), dest.y), scaleFactor, 0, xPattern, 0, zPattern, animationPhase, lightView);
                    break;
                default:
                    break;
                }
                color.setAlpha(255 - (i * 30));
            }
        }

        if (m_outfit.getMount() != 0) {
            auto datType = g_things.rawGetThingType(m_outfit.getMount(), ThingCategoryCreature);
            dest -= datType->getDisplacement() * scaleFactor;
            datType->draw(dest, scaleFactor, 0, xPattern, 0, 0, animationPhase, lightView);
            dest += getDisplacement() * scaleFactor;
            zPattern = std::min<int>(1, getNumPatternZ() - 1);
        }

        PointF jumpOffset = m_jumpOffset * scaleFactor;
        dest -= Point(stdext::round(jumpOffset.x), stdext::round(jumpOffset.y));

        // yPattern => creature addon
        for (int yPattern = 0; yPattern < getNumPatternY(); yPattern++) {

            // continue if we dont have this addon
            if (yPattern > 0 && !(m_outfit.getAddons() & (1 << (yPattern - 1))))
                continue;

            auto datType = rawGetThingType();
            datType->draw(dest, scaleFactor, 0, xPattern, yPattern, zPattern, animationPhase, yPattern == 0 ? lightView : nullptr);

            if (getLayers() > 1) {
                Color oldColor = g_painter->getColor();
                Painter::CompositionMode oldComposition = g_painter->getCompositionMode();
                g_painter->setCompositionMode(Painter::CompositionMode_Multiply);
                g_painter->setColor(m_outfit.getHeadColor());
                datType->draw(dest, scaleFactor, SpriteMaskYellow, xPattern, yPattern, zPattern, animationPhase);
                g_painter->setColor(m_outfit.getBodyColor());
                datType->draw(dest, scaleFactor, SpriteMaskRed, xPattern, yPattern, zPattern, animationPhase);
                g_painter->setColor(m_outfit.getLegsColor());
                datType->draw(dest, scaleFactor, SpriteMaskGreen, xPattern, yPattern, zPattern, animationPhase);
                g_painter->setColor(m_outfit.getFeetColor());
                datType->draw(dest, scaleFactor, SpriteMaskBlue, xPattern, yPattern, zPattern, animationPhase);
                g_painter->setColor(oldColor);
                g_painter->setCompositionMode(oldComposition);
            }
        }
        // outfit is a creature imitating an item or the invisible effect
    }
    else {
        ThingType* type = g_things.rawGetThingType(m_outfit.getAuxId(), m_outfit.getCategory());

        int animationPhase = 0;
        int animationPhases = type->getAnimationPhases();
        int animateTicks = Otc::ITEM_TICKS_PER_FRAME;

        // when creature is an effect we cant render the first and last animation phase,
        // instead we should loop in the phases between
        if (m_outfit.getCategory() == ThingCategoryEffect) {
            animationPhases = std::max<int>(1, animationPhases - 2);
            animateTicks = Otc::INVISIBLE_TICKS_PER_FRAME;
        }

        if (animationPhases > 1) {
            if (animateIdle)
                animationPhase = (g_clock.millis() % (animateTicks * animationPhases)) / animateTicks;
            else
                animationPhase = animationPhases - 1;
        }

        if (m_outfit.getCategory() == ThingCategoryEffect)
            animationPhase = std::min<int>(animationPhase + 1, animationPhases);

        type->draw(dest - (getDisplacement() * scaleFactor), scaleFactor, 0, 0, 0, 0, animationPhase, lightView);
    }

    g_painter->resetColor();
}