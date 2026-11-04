//
// Created by PinkySmile on 29/03/2026.
//

#include <nlohmann/json.hpp>
#include "D2KUI.hpp"
#include "D2KCameo.hpp"
#include "UI/UI.hpp"
#include "Resources/Assert.hpp"
#include "Data/Faction.hpp"
#include "Data/Mod.hpp"
#include "Data/Player.hpp"
#include "Resources/Game.hpp"
#include "Data/Animation.hpp"
#include "Data/GameManager.hpp"

static std::string __upgradeOrder[] = {
	"cony.upgrade",
	"barr.upgrade",
	"ahtec.upgrade",
	"lfac.upgrade",
	"hfac.upgrade"
};
static std::string __buildOrder[] = {
	"2con",
	"3con",
	"wtra",
	"barr",
	"wall",
	"refi",
	"gunt",
	"outp",
	"roct",
	"ahtec",
	"htec",
	"lfac",
	"silo",
	"hfac",
	"star",
	"repr",
	"ixrc",
	"apala",
	"hpala",
	"opala"
};
static std::string __unitsOrder[] = {
	"rifl",
	"trop",
	"engi",
	"thump",
	"trik",
	"ride",
	"quad",
	"harv",
	"tank",
	"mcv",
	"misl",
	"devi",
	"sieg",
	"soni",
	"deva",
	"carr",
	"orth",
	"frem",
	"sabo",
	"dhmi",
	"gren",
	"srid",
	"sard",
};

static_assert(std::size(__upgradeOrder) == 5);
static_assert(std::size(__buildOrder)   == 20);
static_assert(std::size(__unitsOrder)   == 23);

void D2KUI::_loadEvaSounds(const std::filesystem::path &base, const std::string &folder)
{
	this->_evaSounds[EVA_BUILDING]         .load(base / "announcers" / game->locale / folder / "build.ogg", false);
	this->_evaSounds[EVA_BUILDING_DONE]    .load(base / "announcers" / game->locale / folder / "building_done.ogg", false);
	this->_evaSounds[EVA_BUILDING_LOST]    .load(base / "announcers" / game->locale / folder / "building_lost.ogg", false);
	this->_evaSounds[EVA_CANCEL]           .load(base / "announcers" / game->locale / folder / "cancel.ogg", false);
	this->_evaSounds[EVA_CANNOT_DEPLOY]    .load(base / "announcers" / game->locale / folder / "cannot_deploy.ogg", false);
	this->_evaSounds[EVA_CANNOT_BUILD]     .load(base / "announcers" / game->locale / folder / "cannot_place.ogg", false);
	this->_evaSounds[EVA_GUARDING]         .load(base / "announcers" / game->locale / folder / "guarding.ogg", false);
	this->_evaSounds[EVA_HOLD_QUEUE]       .load(base / "announcers" / game->locale / folder / "hold.ogg", false);
	this->_evaSounds[EVA_LOST]             .load(base / "announcers" / game->locale / folder / "lose.ogg", false);
	this->_evaSounds[EVA_WON]              .load(base / "announcers" / game->locale / folder / "win.ogg", false);
	this->_evaSounds[EVA_LOW_POWER]        .load(base / "announcers" / game->locale / folder / "low_power.ogg", false);
	this->_evaSounds[EVA_UNLOCKED]         .load(base / "announcers" / game->locale / folder / "new_unlock.ogg", false);
	this->_evaSounds[EVA_NO_CREDITS]       .load(base / "announcers" / game->locale / folder / "no_money.ogg", false);
	this->_evaSounds[EVA_PRIMARY_SELECTED] .load(base / "announcers" / game->locale / folder / "primary.ogg", false);
	this->_evaSounds[EVA_RETREAT]          .load(base / "announcers" / game->locale / folder / "retreat.ogg", false);
	this->_evaSounds[EVA_SELL]             .load(base / "announcers" / game->locale / folder / "sell.ogg", false);
	this->_evaSounds[EVA_TRAINING]         .load(base / "announcers" / game->locale / folder / "train.ogg", false);
	this->_evaSounds[EVA_UNIT_READY]       .load(base / "announcers" / game->locale / folder / "unit_ready.ogg", false);
	this->_evaSounds[EVA_UPGRADING]        .load(base / "announcers" / game->locale / folder / "upgrading.ogg", false);
	this->_evaSounds[EVA_REQUIRE_STORAGE]  .load(base / "announcers" / game->locale / folder / "silo_needed.ogg", false);
	this->_evaSounds[EVA_UNIT_LOST]        .load(base / "announcers" / game->locale / folder / "unit_lost.ogg", false);
	this->_evaSounds[EVA_HARV_UNDER_ATTACK].load(base / "announcers" / game->locale / folder / "harvester_attacked.ogg", false);
	this->_evaSounds[EVA_BUILDING_ATTACKED].load(base / "announcers" / game->locale / folder / "building_attacked.ogg", false);
	this->_evaSounds[EVA_BUILDING_CAPTURED].load(base / "announcers" / game->locale / folder / "captured.ogg", false);
	this->_evaSounds[EVA_HAZARD_SPAWNED]   .load(base / "announcers" / game->locale / folder / "worm_sign.ogg", false);
	this->_evaSounds[EVA_HAZARD_ATTACKED]  .load(base / "announcers" / game->locale / folder / "worm_attack.ogg", false);
	this->_evaSounds[EVA_ORDER_PLACED]     .load(base / "announcers" / game->locale / folder / "order_placed.ogg", false);
	this->_evaSounds[EVA_ORDER_IN_1]       .load(base / "announcers" / game->locale / folder / "1min_left.ogg", false);
	this->_evaSounds[EVA_ORDER_IN_2]       .load(base / "announcers" / game->locale / folder / "2min_left.ogg", false);
	this->_evaSounds[EVA_ORDER_IN_3]       .load(base / "announcers" / game->locale / folder / "3min_left.ogg", false);
	this->_evaSounds[EVA_ORDER_IN_4]       .load(base / "announcers" / game->locale / folder / "4min_left.ogg", false);
	this->_evaSounds[EVA_ORDER_IN_5]       .load(base / "announcers" / game->locale / folder / "5min_left.ogg", false);
	this->_evaSounds[EVA_NUKE_PREP]        .load(base / "announcers" / game->locale / folder / "dh_prep.ogg", false);
	this->_evaSounds[EVA_NUKE_LAUNCH]      .load(base / "announcers" / game->locale / folder / "dh_launch.ogg", false);
	this->_evaSounds[EVA_NUKE_READY]       .load(base / "announcers" / game->locale / folder / "dh_ready.ogg", false);
	this->_evaSounds[EVA_CASH_DOWN]        .load(base / "sounds/CASHTIK1.ogg", false);
	this->_evaSounds[EVA_CASH_UP]          .load(base / "sounds/CASHTIK1.ogg", false);
	this->_evaSounds[EVA_POWER_OFF]        .load(base / "sounds/POWRDN1.ogg", false);
	this->_evaSounds[EVA_POWER_ON]         .load(base / "sounds/POWRUP1.ogg", false);
	this->_evaSounds[EVA_RADAR_UP]         .load(base / "sounds/RADRON1.ogg", false);
}

void D2KUI::_setupCursorMapping()
{
	this->_cursorMapping[CURSOR_NORMAL]                 = "normal";
	this->_cursorMapping[CURSOR_START_SELECT]           = "normal";
	this->_cursorMapping[CURSOR_MOVE]                   = "move";
	this->_cursorMapping[CURSOR_MOVE_RESTRICT]          = "move_inf";
	this->_cursorMapping[CURSOR_NO_MOVE]                = "no_move";
	this->_cursorMapping[CURSOR_SELECT]                 = "select";
	this->_cursorMapping[CURSOR_HARVEST]                = "attack";
	this->_cursorMapping[CURSOR_ATTACK]                 = "attack";
	this->_cursorMapping[CURSOR_ATTACK_MOVE]            = ""; // TODO:
	this->_cursorMapping[CURSOR_ENTER]                  = "enter";
	this->_cursorMapping[CURSOR_DUMP]                   = "enter";
	this->_cursorMapping[CURSOR_SCROLL_ALL_DIRS]        = ""; // TODO:
	this->_cursorMapping[CURSOR_SCROLL_RIGHT]           = "scroll_r";
	this->_cursorMapping[CURSOR_SCROLL_LEFT]            = "scroll_l";
	this->_cursorMapping[CURSOR_SCROLL_TOP]             = "scroll_t";
	this->_cursorMapping[CURSOR_SCROLL_TOP_RIGHT]       = "scroll_tr";
	this->_cursorMapping[CURSOR_SCROLL_TOP_LEFT]        = "scroll_tl";
	this->_cursorMapping[CURSOR_SCROLL_BOTTOM]          = "scroll_b";
	this->_cursorMapping[CURSOR_SCROLL_BOTTOM_RIGHT]    = "scroll_br";
	this->_cursorMapping[CURSOR_SCROLL_BOTTOM_LEFT]     = "scroll_bl";
	this->_cursorMapping[CURSOR_NO_SCROLL_TOP]          = "no_scroll_t";
	this->_cursorMapping[CURSOR_NO_SCROLL_TOP_RIGHT]    = "no_scroll_tr";
	this->_cursorMapping[CURSOR_NO_SCROLL_TOP_LEFT]     = "no_scroll_tl";
	this->_cursorMapping[CURSOR_NO_SCROLL_BOTTOM]       = "no_scroll_b";
	this->_cursorMapping[CURSOR_NO_SCROLL_BOTTOM_RIGHT] = "no_scroll_br";
	this->_cursorMapping[CURSOR_NO_SCROLL_BOTTOM_LEFT]  = "no_scroll_bl";
	this->_cursorMapping[CURSOR_NO_SCROLL_RIGHT]        = "no_scroll_r";
	this->_cursorMapping[CURSOR_NO_SCROLL_LEFT]         = "no_scroll_l";
	this->_cursorMapping[CURSOR_DEPLOY]                 = "deploy";
	this->_cursorMapping[CURSOR_NO_DEPLOY]              = "no_deploy";
	this->_cursorMapping[CURSOR_PRIMARY]                = "deploy";
	this->_cursorMapping[CURSOR_NO_REPAIR]              = "no_repair";
	this->_cursorMapping[CURSOR_REPAIR]                 = "repair";
	this->_cursorMapping[CURSOR_NO_SELL]                = "no_sell";
	this->_cursorMapping[CURSOR_SELL]                   = "sell";
	this->_cursorMapping[CURSOR_NUKE]                   = "atomb";
	this->_cursorMapping[CURSOR_AIR_DROP]               = "air_strike";
	this->_cursorMapping[CURSOR_EXPLODE]                = "c4";
}

void D2KUI::_getPanelObjects(const std::filesystem::path &path)
{
	this->_panel->loadWidgetsFromFile(path.native());
	this->_sell            = this->_panel->get<tgui::Button>("Sell");
	this->_guard           = this->_panel->get<tgui::Button>("Guard");
	this->_repair          = this->_panel->get<tgui::Button>("Repair");
	this->_retreat         = this->_panel->get<tgui::Button>("Retreat");
	this->_world           = this->_panel->get<tgui::Button>("Map");
	this->_up1             = this->_panel->get<tgui::Button>("Up1");
	this->_up2             = this->_panel->get<tgui::Button>("Up2");
	this->_down1           = this->_panel->get<tgui::Button>("Down1");
	this->_down2           = this->_panel->get<tgui::Button>("Down2");
	this->_options         = this->_panel->get<tgui::Button>("Options");
	this->_main1           = this->_panel->get<tgui::Button>("Main1");
	this->_main2           = this->_panel->get<tgui::Button>("Main2");
	this->_buy             = this->_panel->get<tgui::Button>("Buy");
	this->_upgrade         = this->_panel->get<tgui::Button>("Upgrade");
	this->_starport        = this->_panel->get<tgui::Button>("Starport");
	this->_blazon          = this->_panel->get<tgui::Picture>("Blazon");
	this->_wonMsg          = this->_panel->get<tgui::Picture>("WonMsg");
	this->_lostMsg         = this->_panel->get<tgui::Picture>("LostMsg");
	this->_power           = this->_panel->get<tgui::Panel>("Power");
	this->_settings        = this->_panel->get<tgui::Panel>("Settings");
	this->_greenPower      = this->_power->get<tgui::Picture>("GreenPower");
	this->_yellowPower     = this->_power->get<tgui::Picture>("YellowPower");
	this->_redPower        = this->_power->get<tgui::Picture>("RedPower");
	this->_buildQueue      = this->_panel->get<tgui::ScrollablePanel>("BuildingQueue");
	this->_unitsQueue      = this->_panel->get<tgui::ScrollablePanel>("UnitsQueue");
	this->_upgradeQueue    = this->_panel->get<tgui::ScrollablePanel>("UpgradeQueue");
	this->_starportQueue   = this->_panel->get<tgui::ScrollablePanel>("StarportQueue");
	this->_creditDigits[0] = this->_panel->get<tgui::Picture>("Credit0");
	this->_creditDigits[1] = this->_panel->get<tgui::Picture>("Credit1");
	this->_creditDigits[2] = this->_panel->get<tgui::Picture>("Credit2");
	this->_creditDigits[3] = this->_panel->get<tgui::Picture>("Credit3");
	this->_creditDigits[4] = this->_panel->get<tgui::Picture>("Credit4");
	this->_creditDigits[5] = this->_panel->get<tgui::Picture>("Credit5");
	this->_creditDigits[6] = this->_panel->get<tgui::Picture>("Credit6");
	this->_creditDigits[7] = this->_panel->get<tgui::Picture>("Credit7");
	this->_resumeGame      = this->_settings->get<tgui::Button>("Resume");
}

void D2KUI::_initPanel(const nlohmann::json &json)
{
	this->_blazon->getRenderer()->setTexture({(this->_base / json["blazon"].get<std::string>()).native()});
	this->_settings->setVisible(false);
	this->_main1->setVisible(false);
	this->_main2->setVisible(false);
	this->_upgrade->setVisible(false);
	this->_starport->setVisible(false);
	this->_upgradeQueue->setVisible(false);
	this->_starportQueue->setVisible(false);
	this->_buy->setVisible(false);
	this->_wonMsg->setVisible(false);
	this->_lostMsg->setVisible(false);
	this->_buildQueue->getVerticalScrollbar()->setScrollAmount(47);
	this->_unitsQueue->getVerticalScrollbar()->setScrollAmount(47);
}

void D2KUI::_placeUiEvents()
{
	this->_repair->onMousePress([this] {
		this->_soundButton.play();
		this->_repairing = !this->_repairing;
		this->_selling = false;
	});
	this->_sell->onMousePress([this] {
		this->_soundButton.play();
		this->_selling = !this->_selling;
		this->_repairing = false;
	});
	this->_up1->onMousePress([this] {
		auto amount = this->_buildQueue->getContentOffset();

		this->_buildQueue->scrolled(1, {0, 0}, false);
		if (amount == this->_buildQueue->getContentOffset())
			this->_soundMoveBlocked.play();
		else
			this->_soundMove.play();
	});
	this->_up2->onMousePress([this] {
		auto amount = this->_unitsQueue->getContentOffset();

		this->_unitsQueue->scrolled(1, {0, 0}, false);
		if (amount == this->_unitsQueue->getContentOffset())
			this->_soundMoveBlocked.play();
		else
			this->_soundMove.play();
	});
	this->_down1->onMousePress([this] {
		auto amount = this->_buildQueue->getContentOffset();

		this->_buildQueue->scrolled(-1, {0, 0}, false);
		if (amount == this->_buildQueue->getContentOffset())
			this->_soundMoveBlocked.play();
		else
			this->_soundMove.play();
	});
	this->_down2->onMousePress([this] {
		auto amount = this->_unitsQueue->getContentOffset();

		this->_unitsQueue->scrolled(-1, {0, 0}, false);
		if (amount == this->_unitsQueue->getContentOffset())
			this->_soundMoveBlocked.play();
		else
			this->_soundMove.play();
	});
	this->_buy->onMousePress([this] {
		this->_eventQueue.emplace_back(StartBuildEvent{"base:order", this->_modName + ":starport"});
		this->_main2->onMousePress.emit(&*this->_main2, tgui::Vector2f{});
	});
	this->_upgrade->onMousePress(&D2KUI::_openUpgradeMenu, this);
	this->_starport->onMousePress(&D2KUI::_openStarportMenu, this);
	this->_main1->onMousePress(&D2KUI::_closeUpgradeMenu, this);
	this->_main2->onMousePress(&D2KUI::_closeStarportMenu, this);
	this->_options->onMousePress([this]{
		this->_soundButton.play();
	});
	this->_options->onClick([]{
		game->paused = true;
	});
	this->_resumeGame->onMousePress([this]{
		this->_soundButton.play();
	});
	this->_resumeGame->onClick([]{
		game->paused = false;
	});
}

void D2KUI::_loadExtraAssets()
{
	this->_creditRawDigits[0] = tgui::Texture{(this->_base / "ui/numbers/credit_0.png").native()};
	this->_creditRawDigits[1] = tgui::Texture{(this->_base / "ui/numbers/credit_1.png").native()};
	this->_creditRawDigits[2] = tgui::Texture{(this->_base / "ui/numbers/credit_2.png").native()};
	this->_creditRawDigits[3] = tgui::Texture{(this->_base / "ui/numbers/credit_3.png").native()};
	this->_creditRawDigits[4] = tgui::Texture{(this->_base / "ui/numbers/credit_4.png").native()};
	this->_creditRawDigits[5] = tgui::Texture{(this->_base / "ui/numbers/credit_5.png").native()};
	this->_creditRawDigits[6] = tgui::Texture{(this->_base / "ui/numbers/credit_6.png").native()};
	this->_creditRawDigits[7] = tgui::Texture{(this->_base / "ui/numbers/credit_7.png").native()};
	this->_creditRawDigits[8] = tgui::Texture{(this->_base / "ui/numbers/credit_8.png").native()};
	this->_creditRawDigits[9] = tgui::Texture{(this->_base / "ui/numbers/credit_9.png").native()};
	this->_creditRawDigits[10]= tgui::Texture{(this->_base / "ui/numbers/credit_empty.png").native()};
	for (size_t i = 0; i < this->_selectCorners.size(); i++)
		assert_exp(this->_selectCorners[i].loadFromFile(this->_base / ("cursor/select_" + std::to_string(i) + ".png")));

	this->_soundMove.load(this->_base / "sounds/SIDEBAR1.ogg", false);
	this->_soundButton.load(this->_base / "sounds/BUTTON1.ogg", false);
	this->_soundMoveBlocked.load(this->_base / "sounds/ENDLIST1.ogg", false);

	assert_exp(this->_primary.loadFromFile(this->_base / "ui/common/primary.png"));
}

void D2KUI::_loadCameos()
{
	auto start = this->_modName;

	start.push_back(':');
	this->_upgradeOrder.reserve(std::size(__upgradeOrder));
	for (auto &b : __upgradeOrder)
		this->_upgradeOrder.push_back(start + b);
	this->_buildOrder.reserve(std::size(__buildOrder));
	for (auto &b : __buildOrder)
		this->_buildOrder.push_back(start + b);
	this->_unitsOrder.reserve(std::size(__unitsOrder));
	for (auto &u : __unitsOrder)
		this->_unitsOrder.push_back(start + u);

	for (const auto &entry : std::filesystem::directory_iterator(this->_base / "cameos")) {
		const auto &entryPath = entry.path();
		auto name = entryPath.filename().stem().string().substr(2);

		if (name.size() >= this->_prefix.size() && name[this->_prefix.size()] == '_') {
			if (!name.starts_with(this->_prefix))
				continue;
			name = name.substr(this->_prefix.size() + 1);
		}
		this->_cameos[start + name] = tgui::Texture{entryPath.native(), {}, {}, true};
	}
}

void D2KUI::_setupQueues()
{
	assert_msg(this->ownerState->queues.contains(this->_modName + ":hpalace"), this->_modName + ":hpalace");
	assert_msg(this->ownerState->queues.contains(this->_modName + ":upgrade"), this->_modName + ":upgrade");
	assert_msg(this->ownerState->queues.contains(this->_modName + ":starport"), this->_modName + ":starport");
	assert_msg(this->ownerState->queues.contains(this->_modName + ":building"), this->_modName + ":building");
	assert_msg(this->ownerState->queues.contains(this->_modName + ":infantry"), this->_modName + ":infantry");
	assert_msg(this->ownerState->queues.contains(this->_modName + ":aircraft"), this->_modName + ":aircraft");
	assert_msg(this->ownerState->queues.contains(this->_modName + ":ornithopter"), this->_modName + ":ornithopter");
	assert_msg(this->ownerState->queues.contains(this->_modName + ":support_power"), this->_modName + ":support_power");
	assert_msg(this->ownerState->queues.contains(this->_modName + ":light_vehicle"), this->_modName + ":light_vehicle");
	assert_msg(this->ownerState->queues.contains(this->_modName + ":heavy_vehicle"), this->_modName + ":heavy_vehicle");

	this->_upgradeQ = &this->ownerState->queues.at(this->_modName + ":upgrade");
	this->_supportQ = &this->ownerState->queues.at(this->_modName + ":support_power");
	this->_h_palaceQ = &this->ownerState->queues.at(this->_modName + ":hpalace");
	this->_ornithopterQ = &this->ownerState->queues.at(this->_modName + ":ornithopter");
	this->_starportQ = &this->ownerState->queues.at(this->_modName + ":starport");
	assert_exp(this->_starportQ->params->behavior == FactionQueue::TYPE_STARPORT);
	this->_buildingQ = &this->ownerState->queues.at(this->_modName + ":building");
	this->_infantryQ = &this->ownerState->queues.at(this->_modName + ":infantry");
	this->_aircraftQ = &this->ownerState->queues.at(this->_modName + ":aircraft");
	this->_lightVehicleQ = &this->ownerState->queues.at(this->_modName + ":light_vehicle");
	this->_heavyVehicleQ = &this->ownerState->queues.at(this->_modName + ":heavy_vehicle");

	this->_buildingQueueEntries.reserve(this->_buildOrder.size());
	for (const auto &b : this->_buildOrder) {
		auto realName = b;

		if (!this->_buildingQ->entries.contains(b))
			realName = this->_modName + ":" + this->_prefix + "_" + b.substr(b.find(':') + 1);
		if (this->_buildingQ->entries.contains(realName))
			this->_buildingQueueEntries.push_back(&this->_buildingQ->entries.at(realName));
		else
			this->_buildingQueueEntries.push_back(nullptr);
	}

	this->_upgradeQueueEntries.reserve(this->_upgradeOrder.size());
	for (auto &u : this->_upgradeOrder)
		if (this->_upgradeQ->entries.contains(u))
			this->_upgradeQueueEntries.push_back(&this->_upgradeQ->entries.at(u));
		else
			this->_upgradeQueueEntries.push_back(nullptr);

	this->_unitsQueueEntries.reserve(this->_unitsOrder.size());
	for (auto &u : this->_unitsOrder) {
		auto it = this->_infantryQ->entries.find(u);

		if (it == this->_infantryQ->entries.end())
			it = this->_aircraftQ->entries.find(u);
		if (it == this->_aircraftQ->entries.end())
			it = this->_lightVehicleQ->entries.find(u);
		if (it == this->_lightVehicleQ->entries.end())
			it = this->_heavyVehicleQ->entries.find(u);
		if (it == this->_heavyVehicleQ->entries.end())
			it = this->_supportQ->entries.find(u);
		if (it == this->_supportQ->entries.end())
			it = this->_h_palaceQ->entries.find(u);
		if (it == this->_h_palaceQ->entries.end())
			it = this->_ornithopterQ->entries.find(u);
		if (it != this->_ornithopterQ->entries.end())
			this->_unitsQueueEntries.push_back(&it->second);
		else
			this->_unitsQueueEntries.push_back(nullptr);
	}
}

void D2KUI::_createCameoBuildWidgets()
{
	std::string prefixedStart = this->_modName;
	std::string last;

	prefixedStart.push_back(':');
	prefixedStart += this->_prefix;
	prefixedStart.push_back('_');
	for (size_t i = 0; i < this->_buildOrder.size(); i++) {
		auto &b = this->_buildOrder[i];
		std::string realName;
		std::string name = b.substr(b.find(':') + 1);

		if (name == "barr" || name == "lfac" || name == "hfac")
			realName = prefixedStart + name;
		else
			realName = b;

		auto pic = D2KCameo::create(this->_base);
		auto r = pic->getRenderer();
		auto size = this->_cameos[b].getImageSize();

		pic->setSize("&.w", size.y + 1);
		pic->onMousePress([this, realName, i]{
			if (this->_buildingQueueEntries[i]->buildProgress == this->_buildingQueueEntries[i]->buildTime)
				this->selectBuildingForPlacement(realName);
			else
				this->_eventQueue.emplace_back(StartBuildEvent{
					realName, this->_modName + ":building"
				});
		});
		pic->onRightMousePress([this, realName]{
			this->_eventQueue.emplace_back(CancelBuildEvent{
				realName, this->_modName + ":building"
			});
			this->_placementBuilding = nullptr;
		});
		r->setTexture(this->_cameos[b]);
		if (!last.empty()) {
			std::string pos;

			pos.reserve(21 + 6);
			pos += "cameo_";
			pos += last;
			pos += ".y + cameo_";
			pos += last;
			pos += ".h";
			pic->setPosition({0, tgui::Layout{pos}});
		}
		last = b;
		this->_buildQueue->add(pic, "cameo_" + b);
		this->_buildQueueWidgets.push_back(pic);
	}
}

void D2KUI::_createCameoUnitsWidgets()
{
	std::string last;

	for (size_t i = 0; i < this->_unitsOrder.size(); i++) {
		auto &b = this->_unitsOrder[i];
		std::string name = b.substr(b.find(':') + 1);
		auto pic = D2KCameo::create(this->_base);
		auto r = pic->getRenderer();
		auto size = this->_cameos[b].getImageSize();

		pic->setSize("&.w", size.y + 1);
		pic->onMousePress([this, b, i]{
			auto *entry = this->_unitsQueueEntries[i];

			if (
				entry && !entry->paused &&
				entry->buildProgress == entry->buildTime &&
				entry->queue->params->unitSpawn == FactionQueue::SPAWN_PLACEMENT
			)
				this->selectBuildingForPlacement(b);
			else
				this->_eventQueue.emplace_back(StartBuildEvent{b, *this->_unitsQueueEntries[i]->queueId});
		});
		pic->onRightMousePress([this, b, i]{
			this->_eventQueue.emplace_back(CancelBuildEvent{b, *this->_unitsQueueEntries[i]->queueId});
			this->_placementBuilding = nullptr;
		});
		r->setTexture(this->_cameos[b]);
		if (!last.empty()) {
			std::string pos;

			pos.reserve(21 + 6);
			pos += "cameo_";
			pos += last;
			pos += ".y + cameo_";
			pos += last;
			pos += ".h";
			pic->setPosition({0, tgui::Layout{pos}});
		}
		last = b;
		this->_unitsQueue->add(pic, "cameo_" + b);
		this->_unitsQueueWidgets.push_back(pic);
	}
}

void D2KUI::_createCameoUpgradeWidgets()
{
	for (size_t i = 0; i < this->_upgradeOrder.size(); i++) {
		auto &b = this->_upgradeOrder[i];
		auto pos = b.find('.');
		auto pic = D2KCameo::create(this->_base);
		auto r = pic->getRenderer();
		auto &cameo = this->_cameos[pos == std::string::npos ? b : b.substr(0, pos)];

		pic->setPosition({i % 2 * 60, i / 2 * 47});
		pic->setSize(60, 48);
		pic->onMousePress([this, b] {
			this->_eventQueue.emplace_back(StartBuildEvent{b, this->_modName + ":upgrade"});
		});
		pic->onRightMousePress([this, b] {
			this->_eventQueue.emplace_back(CancelBuildEvent{b, this->_modName + ":upgrade"});
		});
		r->setTexture(cameo);
		this->_upgradeQueue->add(pic, "cameo" + b);
		this->_upgradeQueueWidgets.push_back(pic);
	}
}

void D2KUI::_createCameoStarportWidgets()
{
	this->_starportQueueWidgets.reserve(this->_starportQ->params->starportParams->units.size());
	for (size_t i = 0; i < this->_starportQ->params->starportParams->units.size(); i++) {
		auto &b = this->_starportQ->params->starportParams->units[i];
		auto pic = D2KCameo::create(this->_base);
		auto r = pic->getRenderer();
		auto &cameo = this->_cameos[b];

		pic->setPosition({i % 2 * 60, i / 2 * 47});
		pic->setSize(60, 48);
		pic->onMousePress([this, b]{
			this->_eventQueue.emplace_back(StartBuildEvent{b, this->_modName + ":starport"});
		});
		pic->onRightMousePress([this, b]{
			this->_eventQueue.emplace_back(CancelBuildEvent{b, this->_modName + ":starport"});
		});
		r->setTexture(cameo);
		this->_starportQueue->add(pic);
		this->_starportQueueWidgets.push_back(pic);
	}
}

void D2KUI::_createCameoWidgets()
{
	this->_createCameoBuildWidgets();
	this->_createCameoUnitsWidgets();
	this->_createCameoUpgradeWidgets();
	this->_createCameoStarportWidgets();
}

D2KUI::D2KUI(const Faction &faction, UIState *state, GameManager *manager, PlayerState *ownerState, MapState *map, const std::filesystem::path &base, const nlohmann::json &params) :
	UI(faction, state, manager, ownerState, map),
	_prefix(params["cameo_prefix"]),
	_modName(faction.getBase().getName()),
	_base(base)
{
	faction.getBase().loadCursors("cursors.json", this->_cursors);

	this->_getPanelObjects(base / "ui" / game->locale / "ui.gui");
	this->_initPanel(params);
	this->_placeUiEvents();
	this->_loadExtraAssets();
	this->_loadCameos();
	this->_setupQueues();
	this->_loadEvaSounds(base, params["eva_folder"]);
	this->_setupCursorMapping();
	this->_createCameoWidgets();
}

D2KUI::~D2KUI() noexcept = default;

void D2KUI::_openUpgradeMenu()
{
	this->_inMainMenu = false;
	this->_upgrade->setVisible(false);
	this->_starport->setVisible(false);
	this->_main1->setVisible(true);
	this->_buildQueue->setVisible(false);
	this->_unitsQueue->setVisible(false);
	this->_upgradeQueue->setVisible(true);
	this->_up1->setVisible(false);
	this->_up2->setVisible(false);
	this->_down1->setVisible(false);
	this->_down2->setVisible(false);
}

void D2KUI::_openStarportMenu()
{
	this->_inMainMenu = false;
	this->_upgrade->setVisible(false);
	this->_starport->setVisible(false);
	this->_main2->setVisible(true);
	this->_buildQueue->setVisible(false);
	this->_unitsQueue->setVisible(false);
	this->_starportQueue->setVisible(true);
	this->_up1->setVisible(false);
	this->_up2->setVisible(false);
	this->_down1->setVisible(false);
	this->_down2->setVisible(false);
	this->_buy->setVisible(true);
	this->_eventQueue.emplace_back(OpenQueueEvent{this->_modName + ":starport"});
}

void D2KUI::_closeUpgradeMenu()
{
	this->_inMainMenu = true;
	this->_upgrade->setVisible(this->_hasUpgrades());
	this->_starport->setVisible(this->ownerState->queues.at(this->_modName + ":starport").providers > 0);
	this->_main1->setVisible(false);
	this->_buildQueue->setVisible(true);
	this->_unitsQueue->setVisible(true);
	this->_upgradeQueue->setVisible(false);
	this->_up1->setVisible(true);
	this->_up2->setVisible(true);
	this->_down1->setVisible(true);
	this->_down2->setVisible(true);
}

void D2KUI::_closeStarportMenu()
{
	this->_inMainMenu = true;
	this->_upgrade->setVisible(this->_hasUpgrades());
	this->_starport->setVisible(this->ownerState->queues.at(this->_modName + ":starport").providers > 0);
	this->_main2->setVisible(false);
	this->_buildQueue->setVisible(true);
	this->_unitsQueue->setVisible(true);
	this->_starportQueue->setVisible(false);
	this->_up1->setVisible(true);
	this->_up2->setVisible(true);
	this->_down1->setVisible(true);
	this->_down2->setVisible(true);
	this->_buy->setVisible(false);
	this->_eventQueue.emplace_back(CloseQueueEvent{this->_modName + ":starport"});
}

bool D2KUI::_hasUpgrades()
{
	auto it = this->ownerState->queues.find(this->_modName + ":upgrade");

	if (it == this->ownerState->queues.end())
		return false;
	return std::ranges::any_of(it->second.entries | std::ranges::views::values, [](const PlayerQueueEntry &q){
		return q.buildable && q.unlocked;
	});
}

void D2KUI::_updateQueueCameos()
{
	std::array orders = {
		std::tuple{&this->_buildOrder, &this->_buildingQueueEntries, &this->_buildQueueWidgets},
		std::tuple{&this->_unitsOrder, &this->_unitsQueueEntries, &this->_unitsQueueWidgets},
		std::tuple{&this->_upgradeOrder, &this->_upgradeQueueEntries, &this->_upgradeQueueWidgets}
	};

	for (auto &order : orders)
		for (size_t i = 0; i < std::get<0>(order)->size(); i++) {
			PlayerQueueEntry *entry = (*std::get<1>(order))[i];
			D2KCameo::Ptr w = (*std::get<2>(order))[i];

			if (!entry || !entry->unlocked) {
				if (w->isVisible()) {
					w->setSize(60, 0);
					w->setVisible(false);
				}
				continue;
			}
			if (!w->isVisible()) {
				w->setVisible(true);
				w->setSize(60, 48);
			}
			w->setOnHold(entry->paused);
			if (!entry->buildable) {
				w->setFill(0);
				w->setReady(false);
			} else if (entry->buildProgress == 0) {
				w->setFill(1);
				w->setReady(false);
			} else if (entry->buildProgress >= entry->buildTime) {
				w->setFill(1);
				w->setReady(true);
			} else {
				w->setFill(entry->buildProgress / static_cast<float>(entry->buildTime));
				w->setReady(false);
			}
		}
	for (size_t i = 0; i < this->_starportQ->params->starportParams->units.size(); i++) {
		const std::string &b = this->_starportQ->params->starportParams->units[i];
		D2KCameo::Ptr w = this->_starportQueueWidgets[i];

		if (this->_starportQ->busy) {
			w->setFill(0);
			w->setCount(0);
		} else if (this->_starportQ->stock.at(b) == 0 || this->_starportQ->nb >= this->_starportQ->params->starportParams->deliveryMax) {
			w->setFill(0);
			w->setCount(this->_starportQ->basket.at(b));
		} else {
			w->setFill(1);
			w->setCount(this->_starportQ->basket.at(b));
		}
	}
}

void D2KUI::update()
{
	UI::update();
	this->_minimapPos = this->_blazon->getPosition();
	this->_minimapSize = this->_blazon->getSize();
	if (this->ownerState->hasRadar) {
		this->_blazon->getRenderer()->setOpacity(0);
	} else {
		float opacity = this->_blazon->getRenderer()->getOpacity();

		opacity += 0.025;
		if (opacity >= 1)
			opacity = 1;
		this->_blazon->getRenderer()->setOpacity(opacity);
	}

	unsigned credits = std::min(99999999U, this->ownerState->getCombinedMoney("base:credits"));

	for (unsigned i = 0; i < 8; i++) {
		char m;

		if (credits != 0 || i == 0)
			m = credits % 10;
		else
			m = 10;
		if (this->_creditOldDigits[i] != m) {
			this->_creditDigits[i]->getRenderer()->setTexture(this->_creditRawDigits[m]);
			this->_creditOldDigits[i] = m;
		}
		credits /= 10;
	}
	this->_updateQueueCameos();

	float percent = 100 * (200.f + this->ownerState->producedPower) / (this->ownerState->consumedPower + 200.f);

	this->_power->setSize({10, percent});
	this->_greenPower->setVisible(percent >= 100);
	this->_yellowPower->setVisible(percent > 50 && percent < 100);
	this->_redPower->setVisible(percent <= 50);

	if (this->_inMainMenu) {
		this->_upgrade->setVisible(this->_hasUpgrades());
		this->_starport->setVisible(this->_starportQ->providers > 0);
	} else if (this->_starportQ->providers == 0 && this->_starportQueue->isVisible())
		this->_closeStarportMenu();
}

void D2KUI::updateRender()
{
	UI::updateRender();
	if (!this->won && !this->lost) {
		this->_wonMsg->setVisible(false);
		this->_lostMsg->setVisible(false);
		this->_msgTimer = 0;
	} else if (this->_msgTimer == 150) {
		this->_wonMsg->setVisible(false);
		this->_lostMsg->setVisible(false);
		if (this->won)
			this->_openEndGameDialog(this->_base / "ui" / game->locale / "endWon.gui");
		else
			this->_openEndGameDialog(this->_base / "ui" / game->locale / "endLost.gui");
		this->_msgTimer++;
	} else if (this->_msgTimer < 150) {
		this->_wonMsg->setVisible(this->won);
		this->_lostMsg->setVisible(!this->won);
		if (this->_msgTimer == 0)
			this->playEVA(this->won ? EVA_WON : EVA_LOST, 50000, 500, 0, "");
		this->_msgTimer++;
	}
}

void D2KUI::render()
{
	UI::render();
	if (this->_dragging) {
		sf::VertexArray array{sf::PrimitiveType::LineStrip, 5};

		array[0].position.x = this->_startDrag->x;
		array[0].position.y = this->_startDrag->y;
		array[1].position.x = this->_startDrag->x;
		array[1].position.y = this->_placementPos.y;
		array[2].position.x = this->_placementPos.x;
		array[2].position.y = this->_placementPos.y;
		array[3].position.x = this->_placementPos.x;
		array[3].position.y = this->_startDrag->y;
		array[4].position.x = this->_startDrag->x;
		array[4].position.y = this->_startDrag->y;
		array[0].color = sf::Color{0xC5, 0xB6, 0xA4};
		array[1].color = sf::Color{0xC5, 0xB6, 0xA4};
		array[2].color = sf::Color{0xC5, 0xB6, 0xA4};
		array[3].color = sf::Color{0xC5, 0xB6, 0xA4};
		array[4].color = sf::Color{0xC5, 0xB6, 0xA4};
		game->screen->draw(array);
	}
}

void D2KUI::playEVA(EVASound sound, unsigned int expireIn, unsigned int priority, unsigned int cooldown, const std::string &args)
{
	EVASound ignore[] = {
		EVA_MY_BUILDING_CAPTURED,
		EVA_MY_UNIT_CAPTURED,
		EVA_UNIT_CAPTURED,
		EVA_UNIT_ATTACKED,
		EVA_ALLIED_MY_BUILDING_CAPTURED,
		EVA_ALLIED_BUILDING_CAPTURED,
		EVA_ALLIED_BUILDING_ATTACKED,
		EVA_ALLIED_BUILDING_LOST,
		EVA_ALLIED_MY_UNIT_CAPTURED,
		EVA_ALLIED_UNIT_CAPTURED,
		EVA_ALLIED_UNIT_ATTACKED,
		EVA_ALREADY_BUILDING
	};

	if (std::ranges::find(ignore, sound) != std::end(ignore))
		return;
	if (sound == EVA_BUILDING) {
		if (args == this->_modName + ":upgrade")
			sound = EVA_UPGRADING;
		else if (args == this->_modName + ":infantry")
			sound = EVA_TRAINING;
	} else if (sound == EVA_BUILDING_DONE) {
		if (args == this->_modName + ":upgrade")
			// No built sound for upgrade queue
			return;
		if (args != this->_modName + ":building")
			sound = EVA_UNIT_READY;
		if (args == this->_modName + ":sabo")
			return;
		else if (args == this->_modName + ":frem")
			return;
		else if (args == this->_modName + ":orth")
			return;
		else if (args == this->_modName + ":dhmi")
			return;
	}
	UI::playEVA(sound, expireIn, priority, cooldown, args);
}

void D2KUI::_displaySelectedObjectUI(const GameObject &object)
{
	auto select = object.getSelectBox();
	Vector2i size = select.size;
	Vector2i top = select.position;
	sf::Sprite sprite{this->_selectCorners[0]};
	sf::RectangleShape rect;
	bool isMyTeam = this->manager->getPlayers()[this->manager->getControlledPlayer()].state.team == object.owner->state.team;

	top += object.getPosition();
	rect.setPosition({top.x + 1.f, top.y + 1.f});
	rect.setSize({size.x - 2.f, 2});
	rect.setOutlineThickness(1);
	rect.setOutlineColor(sf::Color::Black);
	rect.setFillColor(sf::Color{0, 0, 0, 128});
	game->screen->draw(rect);

	rect.setSize({(size.x - 2.f) * object.animationState.health, 2});
	if (object.animationState.health >= 0.5)
		rect.setFillColor(sf::Color::Green);
	else if (object.animationState.health >= 0.3)
		rect.setFillColor(sf::Color::Yellow);
	else
		rect.setFillColor(sf::Color::Red);
	game->screen->draw(rect);
	this->_displaySelectBox(top, size);
	top.y += 4;

	auto percent = object.getStorageDisplay();

	if (isMyTeam && percent) {
		sf::Vector2f pos = {top.x + 2.f, top.y + size.y - 5.f};
		unsigned count = 7;

		if (size.x >= 64)
			count = 15;
		rect.setSize({2, 2});
		for (size_t i = 0; i < count; i++) {
			rect.setPosition(pos);
			if (*percent * count > i && percent != 0)
				rect.setFillColor(sf::Color{0, 255, 0, 255});
			else
				rect.setFillColor(sf::Color{0, 0, 0, 128});
			game->screen->draw(rect);
			pos.x += 3;
		}
	}
	if (isMyTeam && object.isPrimary()) {
		sf::Sprite s{this->_primary};

		s.setPosition(top + Vector2i{2, size.y - 5 - 5});
		game->screen->draw(s);
	}
	if (isMyTeam && object.getMaxEnergy() != 0) {
		sf::Vector2f pos = {top.x + 1.f, top.y + 2.f};
		unsigned max = object.getMaxEnergy();
		unsigned current = object.getEnergy();

		rect.setSize({2, 2});
		for (size_t i = 0; i < 5; i++) {
			rect.setPosition(pos);
			if (current * 5 / max > i)
				rect.setFillColor(sf::Color{0, 255, 0, 255});
			else
				rect.setFillColor(sf::Color{0, 0, 0, 128});
			game->screen->draw(rect);
			pos.y += 3;
		}
	}
}
