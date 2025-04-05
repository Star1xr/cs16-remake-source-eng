#include "cbase.h"
#include "tier1/convar.h"
#include "bot.cpp"

// ConVar'lar
ConVar sm_bot_use_shield_enable("sm_bot_use_shield_enable", "1", FCVAR_PLUGIN, "Enable the plugin? (1: enabled; 0: disabled)");
ConVar sm_bot_use_shield_enable_tbot("sm_bot_use_shield_enable_tbot", "0", FCVAR_PLUGIN, "Allow terrorists to use shields?");
ConVar sm_bot_use_shield_enable_ctbot("sm_bot_use_shield_enable_ctbot", "1", FCVAR_PLUGIN, "Allow CT to use shields?");
ConVar sm_player_use_shield_enable("sm_player_use_shield_enable", "1", FCVAR_PLUGIN, "Allow players to use shields?");
ConVar sm_player_use_shield_weapon("sm_player_use_shield_weapon", "weapon_shield", FCVAR_PLUGIN, "Weapon used as shield by players");
void CShieldPlugin::DistributeShieldsRandomly()
{
    if (!CheckGameRules())  // Oyun kurallarını kontrol et
        return;

    int numPlayers = gpGlobals->maxClients;
    int numDistributed = 0;
    for (int i = 1; i <= numPlayers; ++i)
    {
        CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);
        if (!pPlayer || pPlayer->IsFakeClient())
            continue;  // Botları atla

        if (random() % 2 == 0)  // Rastgele oyuncu seçimi (0 veya 1)
        {
            pPlayer->EquipShield();  // Kalkanı ekle
            ++numDistributed;
        }
    }

    Msg("Distributed %d shields to players.\n", numDistributed);
}
bool CShieldPlugin::CheckGameRules()
{
    if (!sm_player_use_shield_enable.GetBool() && !sm_bot_use_shield_enable.GetBool())
    {
        Msg("Shields are disabled by the game rules.\n");
        return false;  // Kalkanlar devre dışı
    }
    return true;  // Kalkanlar aktif
}
CON_COMMAND(sm_distribute_shields, "Distribute shields randomly to players")
{
    CShieldPlugin* plugin = dynamic_cast<CShieldPlugin*>(plugin);  // Plugin'e erişim
    if (plugin)
    {
        plugin->DistributeShieldsRandomly();  // Komut çalıştırıldığında kalkanları dağıt
    }
}
void CShieldPlugin::OnRoundStart()
{
    DistributeShieldsRandomly();  // Oyun başladığında rastgele kalkan dağıt
}
void CShieldPlugin::ApplyShields()
{
    if (!sm_bot_use_shield_enable.GetBool())
        return;

    const char* shieldWeapon = sm_bot_use_shield_weapon.GetString();
    if (Q_strlen(shieldWeapon) == 0)
        return;

    // Botlar için kalkan ekleme
    for (int i = 1; i <= gpGlobals->maxClients; ++i)
    {
        CBasePlayer* pPlayer = UTIL_PlayerByIndex(i);
        if (!pPlayer || pPlayer->IsFakeClient())
            continue;

        int team = pPlayer->GetTeamNumber();

        // Teröristler için kalkan kullanımı
        if (team == TEAM_TERRORIST && sm_bot_use_shield_enable_tbot.GetBool())
        {
            CBot* pBot = dynamic_cast<CBot*>(pPlayer);
            if (pBot && pBot->ShouldEquipShield())
            {
                pBot->EquipShield();
            }
        }
        // CT'ler için kalkan kullanımı
        else if (team == TEAM_CT && sm_bot_use_shield_enable_ctbot.GetBool())
        {
            CBot* pBot = dynamic_cast<CBot*>(pPlayer);
            if (pBot && pBot->ShouldEquipShield())
            {
                pBot->EquipShield();
            }
        }
    }
}
