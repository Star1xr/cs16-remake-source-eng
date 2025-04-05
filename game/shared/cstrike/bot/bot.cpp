//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

// Author: Michael S. Booth (mike@turtlerockstudios.com), Leon Hartwig, 2003

#include "cbase.h"
#include "basegrenade_shared.h"

#include "bot.h"
#include "bot_util.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

/// @todo Remove this nasty hack - CreateFakeClient() calls CBot::Spawn, which needs the profile and team
const BotProfile *g_botInitProfile = NULL;
int g_botInitTeam = 0;

//
// NOTE: Because CBot had to be templatized, the code was moved into bot.h
//


//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------

ActiveGrenade::ActiveGrenade( CBaseGrenade *grenadeEntity )
{
	m_entity = grenadeEntity;
	m_detonationPosition = grenadeEntity->GetAbsOrigin();
	m_dieTimestamp = 0.0f;
	m_radius = HEGrenadeRadius;

	m_isSmoke = FStrEq( grenadeEntity->GetClassname(), "smokegrenade_projectile" );
	if ( m_isSmoke )
	{
		m_radius = SmokeGrenadeRadius;
	}

	m_isFlashbang = FStrEq( grenadeEntity->GetClassname(), "flashbang_projectile" );
	if ( m_isFlashbang )
	{
		m_radius = FlashbangGrenadeRadius;
	}
}

//--------------------------------------------------------------------------------------------------------------
/**
 * Called when the grenade in the world goes away
 */
void ActiveGrenade::OnEntityGone( void )
{
	if (m_isSmoke)
	{
		// smoke lingers after grenade is gone
		const float smokeLingerTime = 4.0f;
		m_dieTimestamp = gpGlobals->curtime + smokeLingerTime;
	}

	m_entity = NULL;
}

//--------------------------------------------------------------------------------------------------------------
void ActiveGrenade::Update( void )
{
	if (m_entity != NULL)
	{
		m_detonationPosition = m_entity->GetAbsOrigin();
	}
}

//--------------------------------------------------------------------------------------------------------------
/**
 * Return true if this grenade is valid
 */
bool ActiveGrenade::IsValid( void ) const
{
	if ( m_isSmoke )
	{
		if ( m_entity == NULL && gpGlobals->curtime > m_dieTimestamp )
		{
			return false;
		}
	}
	else
	{
		if ( m_entity == NULL )
		{
			return false;
		}
	}

	return true;
}

//--------------------------------------------------------------------------------------------------------------
const Vector &ActiveGrenade::GetPosition( void ) const
{
	// smoke grenades can vanish before the smoke itself does - refer to the detonation position
	if (m_entity == NULL)
		return GetDetonationPosition();

	return m_entity->GetAbsOrigin();
}
// Botun kalkan alıp almayacağını kontrol eder
bool CBot::ShouldEquipShield()
{
    // Kalkan kullanımını botların takımına göre kontrol et
    if (GetTeam() == TEAM_CT && sm_bot_use_shield_enable_ctbot.GetBool())
    {
        return true;  // CT botları kalkan alabilir
    }
    else if (GetTeam() == TEAM_TERRORIST && sm_bot_use_shield_enable_tbot.GetBool())
    {
        return false;  // TERRORIST botları kalkan kullanmaz
    }

    return false;  // Diğer durumlarda kalkan kullanılmaz
}

// Botlara kalkan silahı ekler
void CBot::EquipShield()
{
    if (!sm_bot_use_shield_enable.GetBool())
        return;

    const char* shieldWeapon = sm_bot_use_shield_weapon.GetString();
    if (Q_strlen(shieldWeapon) == 0)
        return;

    // Botun envanterine kalkanı ekler
    CBaseEntity* pShield = CreateEntityByName(shieldWeapon);
    DispatchSpawn(pShield);
    Weapon_Equip((CBaseCombatWeapon*)pShield);  // Kalkanı botun eline verir
}
