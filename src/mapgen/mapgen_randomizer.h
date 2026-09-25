/*
This file is part of Freeminer.

Freeminer is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Freeminer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Freeminer. If not, see <http://www.gnu.org/licenses/>.
*/

// Inspired by Randomizer Mod: https://content.luanti.org/packages/NO11/randomizer/

#pragma once

#include "mapgen_v7.h"

struct MapgenRandomizerParams : public MapgenV7Params {
	MapgenRandomizerParams() = default;
	~MapgenRandomizerParams() = default;

	void readParams(const Settings *settings) override;
	void writeParams(Settings *settings) const override;
	void setDefaultSettings(Settings *settings) override;
};

class MapgenRandomizer : public MapgenV7 {
public:
	MapgenRandomizer(MapgenRandomizerParams *params, EmergeParams *emerge);
	~MapgenRandomizer() override = default;

	MapgenType getType() const override { return MAPGEN_RANDOMIZER; }

	void makeChunk(BlockMakeData *data) override;

private:
	void randomizeNodes();
};