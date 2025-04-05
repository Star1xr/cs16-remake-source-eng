#include "cbase.h"
#include "tier1/convar.h"

// ConVar'lar
ConVar sm_bot_use_shield_enable("sm_bot_use_shield_enable", "1", FCVAR_PLUGIN, "Enable the plugin? (1: enabled; 0: disabled)");
ConVar sm_bot_use_shield_enable_tbot("sm_bot_use_shield_enable_tbot", "0", FCVAR_PLUGIN, "Allow terrorists to use shields?");
ConVar sm_bot_use_shield_enable_ctbot("sm_bot_use_shield_enable_ctbot", "1", FCVAR_PLUGIN, "Allow CT to use shields?");
ConVar sm_player_use_shield_enable("sm_player_use_shield_enable", "1", FCVAR_PLUGIN, "Allow players to use shields?");
ConVar sm_player_use_shield_weapon("sm_player_use_shield_weapon", "weapon_shield", FCVAR_PLUGIN, "Weapon used as shield by players");
