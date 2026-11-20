//
// Created by PinkySmile on 19/11/2026.
//

#include "SpiceBloom.hpp"
#include "Data/StateContext.hpp"
#include "Data/Effect.hpp"
#include "Data/GameManager.hpp"
#include "Data/Player.hpp"
#include "Data/Weapon.hpp"
#include "Resources/Game.hpp"

SpiceBloom::SpiceBloom(
	const Faction &base,
	const std::string &id,
	const std::unordered_map<std::string, std::vector<Animation>> &animList,
	const std::unordered_map<std::string, MultiSound> &sounds,
	const std::unordered_map<std::string, Effect> &effects,
	const std::unordered_map<std::string, Weapon> &weapons,
	const nlohmann::json &json
) :
	GameObject(base, id, animList, sounds, effects, weapons, json)
{
	assert_exp(json.contains("bloom_time") && json["bloom_time"].is_number_unsigned());
	assert_exp(json.contains("bloom_effect") && json["bloom_effect"].is_string());
	assert_exp(json.contains("explode") && json["explode"].is_object());
	assert_exp(json["explode"].contains("projectile") && json["explode"]["projectile"].is_string());
	assert_exp(json["explode"].contains("instant") && json["explode"]["instant"].is_string());
	assert_exp(json["explode"].contains("sound") && json["explode"]["sound"].is_string());
	assert_exp(json["explode"].contains("count") && json["explode"]["count"].is_array());
	for (size_t i = 0; i < json["explode"]["count"].size(); i++)
		TRY {
			assert_exp(json["explode"]["count"][i].is_number_unsigned());
		} CATCH_AND_RETHROW("In [\"explode\"][\"count\"][" + std::to_string(i) + "]")

	this->_solid = false;
	this->_selectable = false;
	this->_autoAttack = false;
	this->_triggersAutoAttack = false;
	this->_ignoreTerrain = true;

	auto &mod = base.getBase();
	std::string effect = RESOURCE(mod.getName(), json["bloom_effect"]);
	std::string projectile = RESOURCE(mod.getName(), json["explode"]["projectile"]);
	std::string instant = RESOURCE(mod.getName(), json["explode"]["instant"]);
	std::string sound = RESOURCE(mod.getName(), json["explode"]["sound"]);

	assert_msg(effects.contains(effect), effect);
	assert_msg(weapons.contains(projectile), projectile);
	assert_msg(weapons.contains(instant), instant);
	assert_msg(sounds.contains(sound), sound);
	this->_bloomTime = json["bloom_time"];
	this->_explodeProjectiles = json["explode"]["count"].get<std::vector<unsigned>>();
	this->_bloomEffect = &effects.at(effect);
	this->_explodeProjectile = &weapons.at(projectile);
	this->_explodeInstant = &weapons.at(instant);
	this->_explodeSound = &sounds.at(sound);
	assert_exp(this->_anims[0].size() == this->_explodeProjectiles.size());
}

GameObject *SpiceBloom::copy() const
{
	return new SpiceBloom(*this);
}

void SpiceBloom::update()
{
	unsigned index = this->_gridPos.x + this->_gridPos.y;
	auto &lmap = this->manager->getLoadedMap();
	uint32_t attrs = lmap.grid.grid[index];
	unsigned res = GRID_RESOURCE_TYPE(attrs);
 
	if (res == 0)
		this->_bloomTimer++;
	if (this->_bloomTimer / this->_bloomTime >= this->_anims.front().size()) {
		this->_bloomTimer = this->_bloomTime * this->_anims.front().size() - 1;
		this->_hp = 0;
	} else if ((attrs & GRIDMASK_IS_FREE) == 0)
		this->_hp = 0;
	if (this->_hp == 0) {
		this->_hp = 1;
		if (this->_bloomTimer >= this->_bloomTime) {
			this->_explodeSound->play();
			this->manager->weaponUsedAt(
				this->_explodeInstant->getDamage(),
				this->_explodeInstant,
				this->_position + this->_size / 2,
				this->owner->state.index
			);
			for (size_t i = this->_explodeProjectiles[this->_bloomTimer / this->_bloomTime]; i; i--) {
				float angle = random_distrib(game->gameRandom, 0, 360) * M_PI / 180;
				unsigned distance = random_distrib(game->gameRandom, 0, this->_explodeInstant->getRange());
				Vector2f vec = {
					distance * std::cos(angle),
					distance * std::sin(angle)
				};

				this->manager->spawnProjectile(
					this,
					this->_explodeProjectile,
					this->_position + this->_size / 2,
					nullptr,
					this->_position + this->_size / 2 + vec
				);
			}
			this->_bloomTimer = 0;
		}
	} else if (this->_bloomTimer % this->_bloomTime == 0 && this->_bloomTimer)
		this->manager->spawnEffect(*this->_bloomEffect, this->_position + this->_size / 2, this->owner);

	auto &anim = this->_anims.front()[this->_bloomTimer / this->_bloomTime];

	anim() && anim.update();
}

void SpiceBloom::render(int layer)
{
	auto &anim = this->_anims.front()[this->_bloomTimer / this->_bloomTime];

	if (anim() && anim.getLayer() == layer)
		this->_displayAnim(anim.getAnim());
}

void SpiceBloom::saveState(StateSaveContext &ctx) const
{
	auto &out = ctx.out;

	GameObject::saveState(ctx);
	out.u32(this->_bloomTimer);
}

void SpiceBloom::loadState(StateLoadContext &ctx)
{
	auto &in = ctx.in;

	GameObject::loadState(ctx);
	this->_bloomTimer = in.u32();
}

void SpiceBloom::damage(unsigned, DamageType)
{
	this->_hp = 0;
}
