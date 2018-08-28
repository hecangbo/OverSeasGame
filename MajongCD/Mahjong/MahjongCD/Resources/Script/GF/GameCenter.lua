GameCenter =
{
--    layoutConfig =
--    {
--        noExactFit = false,
--        orientation = Orientation.Landscape
--    },
    names =
    {
        niuniu  = "niuniu",
        chexuan = "chexuan",
        mahjong = "mahjong",
        buyv = "buyv",
        unknown = "unknown"
    },
    games = {},
    curGameName = "unknown",


    setGame = function(name, game)
        assert(type(name) == "string", "GameCenter.setGame: name must be a type of string!")
        assert(type(game) == "table",  "GameCenter.setGame: game must be a type of table!")
        GameCenter.names[name] = name
        GameCenter.games[name] = game
    end;

    invokeGame = function(name)
        assert(type(name) == "string", "GameCenter.invokeGame: name must be a type of string!")
        for k, v in pairs(GameCenter.games) do
            if k == name then
                GameCenter.curGameName = name
                return
            end
        end
    end;

    getGame = function(name)
        assert(type(name) == "string", "GameCenter.getGame: name must be a type of string!")
        for k, v in pairs() do
            if k == name then
                return GameCenter.games[name]
            end
        end
        return nil
    end;

    getCurrentGame = function()
        return GameCenter.games[GameCenter.curGameName]
    end;

    getCurrentGameName = function()
        return GameCenter.curGameName
    end;

	getGameLayoutConfig = function()
        return GameCenter.layoutConfig
    end;

    setGameLayoutConfig = function(config)
        GameCenter.layoutConfig = config
    end;
}
