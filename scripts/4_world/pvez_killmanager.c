class PVEZ_KillManager : Managed {

	static void OnPlayerKilled(PlayerBase victim, EntityAI killer, EntityAI weapon, int weaponType, bool killedInLaw) {

		if (killer == victim) return;

		if (killedInLaw) {
			if (g_Game.pvez_Bounties.Enabled) {
				if (victim.pvez_PlayerStatus.GetIsLawbreaker() && !PlayerBase.Cast(killer).pvez_PlayerStatus.GetIsLawbreaker()) {
					string victimName = PVEZ_StaticFunctions.GetEntityName(victim);
					PlayerBase.Cast(killer).pvez_BountiesSpawner.SpawnRewards(g_Game.pvez_Bounties.Items, victimName);
				}
			}
			return;
		}

		// Both grenades and firearms return <IsWeapon()=true>, and we need grenades to be processed separately.
		// So, check if <weapon> is inherited from <Grenade_Base> first, and weapons will go later.
		switch (weaponType) {
			case PVEZ_DAMAGE_SOURCE_TYPE_EXPLOSIVE:
				if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Explosive_Kill)
					RegisterMurder(killer, victim, weapon);
				break;
			case PVEZ_DAMAGE_SOURCE_TYPE_WEAPON:
				if (killer && weapon) {
					if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Weaponary_Kill)
						RegisterMurder(killer, victim, weapon);
				}
				break;
			case PVEZ_DAMAGE_SOURCE_TYPE_VEHICLE:
				if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Vehicle_Kill)
					RegisterMurder(killer, victim, weapon);
				break;
			case PVEZ_DAMAGE_SOURCE_TYPE_FISTS:
				if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Fist_Fight_Kill)
					RegisterMurder(killer, victim, weapon);
				break;
#ifdef PVEZ_DEBUGMODE
			case PVEZ_DAMAGE_SOURCE_TYPE_CLAWS:
				RegisterMurder(killer, victim, weapon);
				break;
#endif
		}
	}

	static void RegisterMurder(EntityAI killerEntity, EntityAI victimEntity, EntityAI weapon) {
		PlayerBase killer = PlayerBase.Cast(killerEntity);
		if (killer) {
			PlayerBase victim = PlayerBase.Cast(victimEntity);
			if (victim)
				killer.pvez_PlayerStatus.SetLawbreaker(true);
		}
#ifdef PVEZ_DEBUGMODE
		ZombieBase killerZ = ZombieBase.Cast(killerEntity);
		if (killerZ)
			killerZ.SetLawbreaker(true);
		else {
			ZombieBase victimZ = ZombieBase.Cast(victimEntity);
			if (victimZ && killer)
				killer.pvez_PlayerStatus.SetLawbreaker(true);
		}
#endif
		// Show server wide notification
		PVEZ_Notifications.NotificationToAll(killerEntity, victimEntity, weapon);
		// and print in server logs (with addition of date & time)
		string killerName = PVEZ_StaticFunctions.GetEntityName(killerEntity);
		string victimName = PVEZ_StaticFunctions.GetEntityName(victimEntity);
		string crimeNotificationText = "Killer:" + killerName + " has killed player " + victimName + ". Weapon: " + weapon.GetDisplayName();
		string time = " (" + PVEZ_StaticFunctions.GetFormattedDateTime() + ").";
		Print("PVEZ :: " + crimeNotificationText + time);
	}

#ifdef PVEZ_DEBUGMODE
	static void OnZombieKilled(ZombieBase victim, EntityAI killer, EntityAI weapon, int weaponType, bool killedInLaw) {
		
		//if (killedInLaw)
		PlayerBase.Cast(killer).pvez_BountiesSpawner.SpawnRewards(g_Game.pvez_Bounties.Items, victim.GetDisplayName());

		switch (weaponType) {
			case PVEZ_DAMAGE_SOURCE_TYPE_EXPLOSIVE:
				if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Explosive_Kill)
					RegisterMurder(killer, victim, weapon);
				break;
			case PVEZ_DAMAGE_SOURCE_TYPE_WEAPON:
				if (killer && weapon) {
					if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Weaponary_Kill)
						RegisterMurder(killer, victim, weapon);
				}
				break;
			case PVEZ_DAMAGE_SOURCE_TYPE_VEHICLE:
				if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Vehicle_Kill)
					RegisterMurder(killer, victim, weapon);
				break;
			case PVEZ_DAMAGE_SOURCE_TYPE_FISTS:
				if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Fist_Fight_Kill)
					RegisterMurder(killer, victim, weapon);
				break;
			case PVEZ_DAMAGE_SOURCE_TYPE_CLAWS:
				break;
		}
	}
#endif
}