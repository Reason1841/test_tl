-- For this question, all I had to do was iterate through the query result and print
-- the guild names and also free the query result after everything was done.

function printSmallGuildNames(memberCount)
    local selectGuildQuery = "SELECT `name` FROM `guilds` WHERE `max_members` < %d;"
    local resultId = db.storeQuery(string.format(selectGuildQuery, memberCount))
    if resultId ~= false then
        repeat
            local guildName = result.getString(resultId, "name")
            print(guildName)
        until not result.next(resultId)
    end
    result.free(resultId);
end
