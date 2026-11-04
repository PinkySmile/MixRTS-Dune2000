//
// Created by PinkySmile on 11/10/2026.
//

#include "Data/GameManager.hpp"
#include "Data/Faction.hpp"
#include "Data/Mod.hpp"
#include "Data/Player.hpp"
#include "Resources/Game.hpp"

SL_DECLARE_VERSION;

SL_EXPORT bool update(CustomQueue &b, PlayerQueue &queue)
{
	std::string modName = b.player->getPrimaryFaction().getBase().getName();

	for (auto &[name, entry] : queue.entries) {
		entry.paused = b.player->state.consumedPower > b.player->state.producedPower;
		if (name == modName + ":dhmi") {
			if (entry.buildProgress == entry.buildTime)
				;
			else if (entry.buildProgress == 1)
				b.player->playEVA(UI::EVA_NUKE_PREP, 300, 1, 0, *entry.elementId);
			else if (entry.buildProgress == entry.buildTime - 1 && !entry.paused)
				b.player->playEVA(UI::EVA_NUKE_READY, 300, 1, 0, *entry.elementId);
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

	std::string modName = b.player->getPrimaryFaction().getBase().getName();

	if (event.id == modName + ":frem") {
		if (!b.player->parent.spawnObject(b.player->state.index, {-1, -1}, modName + ":apalace", *entry->elementId))
			return false;
		b.player->parent.spawnObject(b.player->state.index, {-1, -1}, modName + ":apalace", *entry->elementId);
	} else if (event.id == modName + ":sabo") {
		if (!b.player->parent.spawnObject(b.player->state.index, {-1, -1}, modName + ":opalace", *entry->elementId))
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

	std::string modName = b.player->getPrimaryFaction().getBase().getName();

	if (event.id == modName + ":dhmi") {
		Vector2f pos = event.position;
		GameObject *dh = b.player->parent.spawnObject(b.player->state.index, {-1, -1}, modName + ":hpalace", event.id);

		if (!dh)
			return false;
		pos.x += random_distrib(game->gameRandom, -64, 64);
		pos.y += random_distrib(game->gameRandom, -64, 64);
		dh->attackPos(pos);
		for (auto &p : b.player->parent.getPlayers())
			p.playEVA(UI::EVA_NUKE_LAUNCH, 900, 4, 0, event.id);
		entry->buildProgress = 0;
	}
	return false;
}

/*SL_EXPORT bool handleCancelBuildEvent(CustomQueue &, PlayerQueueEntry *, const UI::CancelBuildEvent &)
{
	return false;
}*/
