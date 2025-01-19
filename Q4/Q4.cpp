// For this method I assumed that it's intention was to, given a player name, it would add an item to
// the player's inbox. To fix the memory leak I just had to guarantee that in every possible outcome of the function
// the player object was properly destroyed.

void Game::addItemToPlayer(const std::string& recipient, uint16_t itemId)
{
    bool newlyCreatedPlayer = false;
    Player* player = g_game.getPlayerByName(recipient);

    if (!player) {
        player = new Player(nullptr);
        newlyCreatedPlayer = true;

        if (!IOLoginData::loadPlayerByName(player, recipient)) {
            delete player;
            return;
        }
    }

    Item* item = Item::CreateItem(itemId);
    if (!item) {
        if (newlyCreatedPlayer) {
            delete player;
        }
        return;
    }

    g_game.internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT);

    if (player->isOffline()) {
        IOLoginData::savePlayer(player);

        if (newlyCreatedPlayer) {
            delete player;
        }
    }

}