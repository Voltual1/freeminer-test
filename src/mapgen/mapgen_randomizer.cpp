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

#include "mapgen_randomizer.h"
#include "emerge.h"
#include "mapnode.h"
#include "nodedef.h"
#include "voxel.h"
#include "util/numeric.h"
#include <vector>

void MapgenRandomizerParams::readParams(const Settings *settings)
{
	MapgenV7Params::readParams(settings);
}

void MapgenRandomizerParams::writeParams(Settings *settings) const
{
	MapgenV7Params::writeParams(settings);
}

void MapgenRandomizerParams::setDefaultSettings(Settings *settings)
{
	MapgenV7Params::setDefaultSettings(settings);
}

MapgenRandomizer::MapgenRandomizer(MapgenRandomizerParams *params, EmergeParams *emerge)
	: MapgenV7(params, emerge)
{
}

void MapgenRandomizer::randomizeNodes()
{
	if (!vm || !ndef)
		return;

	// Collect valid candidate node contents for replacement
	std::vector<content_t> replace_candidates;
	const content_t max_id = ndef->getLargestId();
	for (content_t id = 0; id <= max_id; ++id) {
		const ContentFeatures &f = ndef->get(id);
		if (f.name.empty() || f.name == "ignore" || f.name == "air")
			continue;
		if (f.drawtype == NDT_NORMAL && !f.groups.count("not_in_creative_inventory")) {
			replace_candidates.push_back(id);
		}
	}

	if (replace_candidates.empty())
		return;

	const v3s32 &em = vm->m_area.getExtent();

	for (pos_t z = node_min.Z; z <= node_max.Z; z++) {
		for (pos_t y = node_min.Y; y <= node_max.Y; y++) {
			u32 vi = vm->m_area.index(node_min.X, y, z);
			for (pos_t x = node_min.X; x <= node_max.X; x++) {
				content_t c = vm->m_data[vi].getContent();
				if (c != CONTENT_AIR && c != CONTENT_IGNORE && c != CONTENT_UNKNOWN) {
					const ContentFeatures &f = ndef->get(c);
					if (f.drawtype != NDT_LIQUID && f.drawtype != NDT_FLOWINGLIQUID) {
						u32 rng = getBlockSeed2(v3pos_t(x, y, z), seed);
						content_t new_c = replace_candidates[rng % replace_candidates.size()];
						vm->m_data[vi].setContent(new_c);
					}
				}
				vi++;
			}
		}
	}
}

void MapgenRandomizer::makeChunk(BlockMakeData *data)
{
	MapgenV7::makeChunk(data);

	this->generating = true;
	randomizeNodes();
	this->generating = false;
}