#include "GameBuilder.h"

#ifndef GAME_BUILDER_FACTORY_INCLUDED
#define GAME_BUILDER_FACTORY_INCLUDED

class GameBuilderFactory
{
public:
	static GameBuilder* CreateGameBuilder() {
		return new GameBuilder("YELLOW", "BLACK", 5);
	}
};

#endif
