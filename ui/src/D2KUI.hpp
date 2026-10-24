//
// Created by PinkySmile on 29/03/2026.
//

#ifndef MIXRTS_D2KUI_HPP
#define MIXRTS_D2KUI_HPP


#include <map>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <TGUI/Widgets/ScrollablePanel.hpp>
#include <TGUI/Widgets/Picture.hpp>
#include <TGUI/Widgets/Button.hpp>

#include "D2KCameo.hpp"
#include "forward.hpp"
#include "UI/UI.hpp"
#include "Sprite.hpp"
#include "Sound.hpp"
#include "Data/Player.hpp"


class D2KUI : public UI {
public:
	D2KUI(
		const Faction &faction,
		UIState *state,
		GameManager *manager,
		PlayerState *ownerState,
		MapState *map,
		const std::filesystem::path &base,
		const nlohmann::json &params
	);
	~D2KUI() noexcept override;
	void update() override;
	void render() override;
	void updateRender() override;
	void playEVA(EVASound sound, unsigned int expireIn, unsigned int priority, unsigned int cooldown, const std::string &args) override;

protected:
	void _displaySelectedObjectUI(const GameObject &object) override;

private:
	bool _inMainMenu = true;
	unsigned _msgTimer = 0;
	std::string _prefix;
	std::string _modName;
	std::filesystem::path _base;

	tgui::Button::Ptr _resumeGame;
	tgui::Panel::Ptr _power;
	tgui::Button::Ptr _sell;
	tgui::Button::Ptr _guard;
	tgui::Button::Ptr _repair;
	tgui::Button::Ptr _retreat;
	tgui::Button::Ptr _world;
	tgui::Button::Ptr _up1;
	tgui::Button::Ptr _up2;
	tgui::Button::Ptr _down1;
	tgui::Button::Ptr _down2;
	tgui::Button::Ptr _options;
	tgui::Button::Ptr _main1;
	tgui::Button::Ptr _main2;
	tgui::Button::Ptr _buy;
	tgui::Button::Ptr _upgrade;
	tgui::Button::Ptr _starport;
	tgui::Picture::Ptr _wonMsg;
	tgui::Picture::Ptr _lostMsg;
	tgui::Picture::Ptr _blazon;
	tgui::Picture::Ptr _greenPower;
	tgui::Picture::Ptr _yellowPower;
	tgui::Picture::Ptr _redPower;
	std::vector<std::string> _buildOrder;
	std::vector<std::string> _unitsOrder;
	std::vector<std::string> _upgradeOrder;
	tgui::ScrollablePanel::Ptr _buildQueue;
	tgui::ScrollablePanel::Ptr _unitsQueue;
	tgui::ScrollablePanel::Ptr _upgradeQueue;
	tgui::ScrollablePanel::Ptr _starportQueue;

	std::unordered_map<std::string, tgui::Texture> _cameos;
	std::array<tgui::Picture::Ptr, 8> _creditDigits;
	std::array<char, 8> _creditOldDigits{11, 11, 11, 11, 11, 11, 11, 11};
	std::array<tgui::Texture, 11> _creditRawDigits;

	Sound _soundMoveBlocked;
	Sound _soundMove;
	sf::Texture _primary;

	PlayerQueue *_starportQ = nullptr;
	PlayerQueue *_buildingQ = nullptr;
	PlayerQueue *_upgradeQ  = nullptr;
	PlayerQueue *_infantryQ = nullptr;
	PlayerQueue *_aircraftQ = nullptr;
	PlayerQueue *_supportQ  = nullptr;
	PlayerQueue *_lightVehicleQ = nullptr;
	PlayerQueue *_heavyVehicleQ = nullptr;
	std::vector<PlayerQueueEntry *> _buildingQueueEntries;
	std::vector<PlayerQueueEntry *> _upgradeQueueEntries;
	std::vector<PlayerQueueEntry *> _unitsQueueEntries;

	std::vector<D2KCameo::Ptr> _buildQueueWidgets;
	std::vector<D2KCameo::Ptr> _upgradeQueueWidgets;
	std::vector<D2KCameo::Ptr> _unitsQueueWidgets;
	std::vector<D2KCameo::Ptr> _starportQueueWidgets;

	void _loadCameos();
	void _setupQueues();
	void _initPanel(const nlohmann::json &json);
	void _placeUiEvents();
	void _loadExtraAssets();
	void _setupCursorMapping();
	void _loadEvaSounds(const std::filesystem::path &base, const std::string &folder);
	void _getPanelObjects(const std::filesystem::path &path);
	void _createCameoWidgets();
	void _createCameoBuildWidgets();
	void _createCameoUnitsWidgets();
	void _createCameoUpgradeWidgets();
	void _createCameoStarportWidgets();

	bool _hasUpgrades();
	void _openUpgradeMenu();
	void _openStarportMenu();
	void _closeUpgradeMenu();
	void _closeStarportMenu();
	void _updateQueueCameos();
};



#endif //MIXRTS_D2KUI_HPP
