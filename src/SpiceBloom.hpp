//
// Created by PinkySmile on 19/11/2026.
//

#ifndef MIXRTS_SPICEBLOOM_HPP
#define MIXRTS_SPICEBLOOM_HPP


#include "Data/Objects/GameObject.hpp"

class SpiceBloom : public GameObject {
private:
	unsigned _bloomTimer = 0;
	unsigned _bloomTime;
	const Effect *_bloomEffect;
	const MultiSound *_explodeSound;
	const Weapon *_explodeProjectile;
	const Weapon *_explodeInstant;
	std::vector<unsigned> _explodeProjectiles;

public:
	SpiceBloom(
		const Faction &base,
		const std::string &id,
		const std::unordered_map<std::string, std::vector<Animation>> &animList,
		const std::unordered_map<std::string, MultiSound> &sounds,
		const std::unordered_map<std::string, Effect> &effects,
		const std::unordered_map<std::string, Weapon> &weapons,
		const nlohmann::json &json
	);

	GameObject *copy() const override;
	void update() override;
	void render(int layer) override;
	void saveState(StateSaveContext &ctx) const override;
	void loadState(StateLoadContext &ctx) override;
	void damage(unsigned amount, DamageType type) override;
};


#endif //MIXRTS_SPICEBLOOM_HPP
