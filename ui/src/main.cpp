//
// Created by PinkySmile on 07/10/2026.
//

#include "D2KUI.hpp"

SL_DECLARE_VERSION;

SL_EXPORT UI *createUiFor(
	const Faction &faction,
	UIState *state,
	GameManager *manager,
	Player *owner,
	MapState *map,
	const std::filesystem::path &base,
	const nlohmann::json &params
)
{
	return new D2KUI(faction, state, manager, owner, map, base, params);
}
