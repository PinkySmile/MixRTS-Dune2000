//
// Created by PinkySmile on 11/10/2026.
//

#include "Data/GameManager.hpp"
#include "Data/Faction.hpp"
#include "Data/Mod.hpp"
#include "../../../src/Engine/Data/Objects/ObjectSpawner.hpp"
#include "Data/Player.hpp"
#include "Data/Objects/ObjectFactory.hpp"
#include "Resources/Game.hpp"

#define MOD_NAME "d2k:"

SL_DECLARE_VERSION;

SL_EXPORT bool update(CustomQueue &b, PlayerQueue &queue)
{
	for (auto &[name, entry] : queue.entries) {
		entry.paused = b.player->state.consumedPower > b.player->state.producedPower;
		if (name == MOD_NAME "dhmi") {
			if (entry.buildProgress == entry.buildTime)
				;
			else if (entry.buildProgress == 1)
				b.player->playEVA(UI::EVA_NUKE_PREP, 300, 1, 0, *entry.elementId);
			else if (entry.buildProgress == entry.buildTime - 1 && !entry.paused)
				b.player->playEVA(UI::EVA_NUKE_READY, 300, 1, 0, *entry.elementId);
		} else if (name == MOD_NAME "orth") {
			if (entry.buildProgress > 0) {
				for (auto obj : b.player->state.myObjects)
					if (obj->getId() == MOD_NAME "ahtec") {
						obj->producing(true);
						break;
					}
			}
		}
	}
	return true;
}

/*SL_EXPORT bool updateEntry(CustomQueue &b, PlayerQueueEntry *entry)
{
	return true;
}*/

SL_EXPORT bool tryEndBuild(CustomQueue &, PlayerQueueEntry *)
{
	return false;
}

SL_EXPORT bool handleStartBuildEvent(CustomQueue &b, PlayerQueueEntry *entry, const UI::StartBuildEvent &event)
{
	if (b.player->state.consumedPower > b.player->state.producedPower)
		return false;

	if (!entry) {
		auto itq = b.player->state.queues.find(event.queue);
		assert_exp(itq != b.player->state.queues.end());
		auto ite = itq->second.entries.find(event.id);
		assert_exp(ite != itq->second.entries.end());
		entry = &ite->second;
	}
	if (entry->buildProgress != entry->buildTime)
		return false;

	if (event.id == MOD_NAME "frem") {
		if (!b.player->parent.spawnObject(b.player->state.index, {-1, -1}, MOD_NAME "apalace", *entry->elementId))
			return false;
		b.player->parent.spawnObject(b.player->state.index, {-1, -1}, MOD_NAME "apalace", *entry->elementId);
	} else if (event.id == MOD_NAME "sabo") {
		if (!b.player->parent.spawnObject(b.player->state.index, {-1, -1}, MOD_NAME "opalace", *entry->elementId))
			return false;
	}
	entry->buildProgress = 0;
	b.player->checkUnlocked();
	return false;
}

SL_EXPORT bool handlePlaceBuildEvent(CustomQueue &b, PlayerQueueEntry *entry, const UI::PlaceBuildEvent &event)
{
	if (b.player->state.consumedPower > b.player->state.producedPower)
		return false;

	if (!entry) {
		auto itq = b.player->state.queues.find(event.queue);
		assert_exp(itq != b.player->state.queues.end());
		auto ite = itq->second.entries.find(event.id);
		assert_exp(ite != itq->second.entries.end());
		entry = &ite->second;
	}
	if (entry->buildProgress != entry->buildTime)
		return false;

	if (event.id == MOD_NAME "dhmi") {
		Vector2f pos = event.position;
		GameObject *dh = b.player->parent.spawnObject(b.player->state.index, {-1, -1}, MOD_NAME "hpalace", event.id);

		if (!dh)
			return false;
		pos.x += random_distrib(game->gameRandom, -64, 64);
		pos.y += random_distrib(game->gameRandom, -64, 64);
		dh->attackPos(pos);
		for (auto &p : b.player->parent.getPlayers())
			p.playEVA(UI::EVA_NUKE_LAUNCH, 900, 4, 0, event.id);
		entry->buildProgress = 0;
	} else if (event.id == MOD_NAME "orth") {
		Vector2i pos;
		Vector2u target = event.position;
		const LoadedMap &map = b.player->parent.getLoadedMap();
		Vector2u size = {
			map.grid.gridSize.x * map.grid.tileSize.x,
			map.grid.gridSize.y * map.grid.tileSize.y
		};

		switch (random_distrib(game->gameRandom, 0, 3)) {
		case 0:
			pos.x = -64;
			pos.y = random_distrib(game->gameRandom, -64, size.y + 64);
			break;
		case 1:
			pos.x = size.x + 64;
			pos.y = random_distrib(game->gameRandom, -64, size.y + 64);
			break;
		case 2:
			pos.x = random_distrib(game->gameRandom, -64, size.x + 64);
			pos.y = -64;
			break;
		default:
			pos.x = random_distrib(game->gameRandom, -64, size.x + 64);
			pos.y = size.y + 64;
			break;
		}
		std::vector<Order> orders;

		orders.emplace_back(AttackPosOrder{Vector2f(target), 4, false, false});
		orders.emplace_back(KeepMovingOrder{50});
		orders.emplace_back(AttackPosOrder{Vector2f(target), 4, false, false});
		orders.emplace_back(KeepMovingOrder{50});
		orders.emplace_back(AttackPosOrder{Vector2f(target), 4, false, false});
		orders.emplace_back(KeepMovingAndDieOrder{});
		b.player->parent.spawnCustomObject(b.player->state.index, pos, new ObjectSpawner(MOD_NAME "orth", 3, 25, std::move(orders)));
		entry->buildProgress = 0;
	}
	return false;
}

/*SL_EXPORT bool handleCancelBuildEvent(CustomQueue &, PlayerQueueEntry *, const UI::CancelBuildEvent &)
{
	return false;
}*/

SL_EXPORT void registerFactory(ObjectFactory &factory)
{
	factory.push<GameObject>("bloom");
	factory.push<GameObject>("worm");
}