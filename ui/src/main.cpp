//
// Created by PinkySmile on 07/10/2026.
//

#include "D2KUI.hpp"

#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C"
#endif

EXPORT UI *createUiFor(
	const Faction &faction,
	UIState *state,
	GameManager *manager,
	PlayerState *ownerState,
	MapState *map,
	const std::filesystem::path &base,
	const nlohmann::json &params
)
{
	return new D2KUI(faction, state, manager, ownerState, map, base, params);
}
