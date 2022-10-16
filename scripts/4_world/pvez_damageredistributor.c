class PVEZ_DamageRedistributor : Managed {

	EntityAI entity;
	protected EntityAI DirectDmgInitiator;
	protected EntityAI BleedingDmgInitiator;
	bool lastHitWasAllowed;

	void PVEZ_DamageRedistributor (EntityAI e) {
		entity = e;
	}

	bool LastHitWasAllowed() {
		return lastHitWasAllowed;
	}
	
	EntityAI GetDirectDmgInitiator() {
		return DirectDmgInitiator;
	}

	EntityAI GetKillerEntity() {
		if (BleedingDmgInitiator)
			return BleedingDmgInitiator;
		else
			return DirectDmgInitiator;
	}

	void OnAllBleedingSourcesRemoved() {
		BleedingDmgInitiator = NULL;
	}

	bool IsHitByAnotherPlayer() {
#ifdef PVEZ_DEBUGMODE
		// If the source is an infected (in debug mode)
		if (DirectDmgInitiator && DirectDmgInitiator.IsInherited(DayZInfected)) {
			return true;
		}
		if (DirectDmgInitiator && DirectDmgInitiator.IsPlayer() && DirectDmgInitiator == entity) {
			return true;
		}
#endif
		// A fireplace is also considered a weapon, so we'll proceed only when the root entity is an actual player.
		if (DirectDmgInitiator && DirectDmgInitiator.IsPlayer() && DirectDmgInitiator != entity) {
			return true;
		}
		else {
			DirectDmgInitiator = NULL;
			return false;
		}
	}

	bool IsKilledByAnotherPlayer() {
#ifdef PVEZ_DEBUGMODE
		// If the source is an infected (in debug mode)
		if (DirectDmgInitiator && DirectDmgInitiator.IsInherited(DayZInfected)) {
			return true;
		}
		else if (BleedingDmgInitiator && BleedingDmgInitiator.IsInherited(DayZInfected)) {
			return true;
		}
#endif
		// A fireplace is also considered a weapon, so we'll proceed only when the root entity is an actual player.
		if (DirectDmgInitiator && DirectDmgInitiator.IsPlayer() && DirectDmgInitiator != entity) {
			return true;
		}
		else if (BleedingDmgInitiator && BleedingDmgInitiator.IsPlayer() && BleedingDmgInitiator != entity) {
			return true;
		}
		else {
			DirectDmgInitiator = NULL;
			BleedingDmgInitiator = NULL;
			return false;
		}
	}

	void RegisterHit(EntityAI victim, EntityAI source, out int wpnType, bool gotBleeding) {
		DirectDmgInitiator = GetDamageInitiator(source, wpnType);
		if (gotBleeding) {
			if (BleedingDmgInitiator == NULL || DirectDmgInitiator != entity) {
				BleedingDmgInitiator = DirectDmgInitiator;
			}
		}

		lastHitWasAllowed = true;
		
		if (IsHitByAnotherPlayer()) {
			PlayerBase player;
#ifdef PVEZ_DEBUGMODE
			if (ZombieBase.Cast(victim))
				lastHitWasAllowed = ZombieBase.Cast(victim).PVEZ_IsPvpAttackAllowed();
			else
#endif
			if (Class.CastTo(player, victim) && player.pvez_PlayerStatus)
				lastHitWasAllowed = player.pvez_PlayerStatus.PVEZ_IsPvpAttackAllowed(DirectDmgInitiator);
		}
	}

	void RegisterDeath(EntityAI victim, EntityAI source, out int wpnType) {
		DirectDmgInitiator = GetDamageInitiator(source, wpnType);

		if (IsKilledByAnotherPlayer()) {
			bool killedInLaw;
			PlayerBase victimP = PlayerBase.Cast(victim);
			if (victimP) {
				killedInLaw = victimP.pvez_PlayerStatus.PVEZ_IsPvpAttackAllowed(GetKillerEntity());
				PVEZ_KillManager.OnPlayerKilled(victimP, GetKillerEntity(), source, wpnType, killedInLaw);
			}
#ifdef PVEZ_DEBUGMODE
			else {
				ZombieBase victimZ = ZombieBase.Cast(victim);
				if (victimZ) {
					killedInLaw = victimZ.PVEZ_IsPvpAttackAllowed();
					PVEZ_KillManager.OnZombieKilled(victimZ, GetKillerEntity(), source, wpnType, killedInLaw);
				}
			}
#endif
		}
	}

	/*
	Look into the damage source roots and return the actual initiator entity.
	@param source - the source of the damage from <EEOnDamageCalculated()> (might be a weapon, fists, an animal claws, a car...)
	@param wpnType - returns one of the constants depending on the damage source weapon type. */
	private EntityAI GetDamageInitiator(EntityAI source, out int wpnType) {
		// First of all, check if it's an infected/animal,
		// because it's the most probable option on PVE server and we not gonna iterate through other <if> statements here.
		if (source.IsInherited(DayZInfected) || source.IsInherited(DayZAnimal)) {
			wpnType = PVEZ_DAMAGE_SOURCE_TYPE_FISTS;
			return source;
		}
		// Since grenades and other weapons are all inherited from <Weapon>, we'll check if it's a grenade first.
		// <Grenade_Base.c> is modded to have <(PlayerBase)Thrower> property.
		// The property is set in modded <ActionUnpin> class on grenade unpin action execution.
		else if (source.IsInherited(Grenade_Base)) {
			Grenade_Base grenade = Grenade_Base.Cast(source);
			wpnType = PVEZ_DAMAGE_SOURCE_TYPE_EXPLOSIVE;
			return grenade.Thrower;
		}
		// a landmine. The one who stepped on it will be treated as a damage dealer himself.
		else if (source.IsInherited(TrapBase)) {
			wpnType = PVEZ_DAMAGE_SOURCE_TYPE_EXPLOSIVE;
			return entity;
		}
		// and then check if it's some other weapon.
		else if (source.IsWeapon() || source.IsMeleeWeapon()) {
			EntityAI weapon = EntityAI.Cast(source);
			wpnType = PVEZ_DAMAGE_SOURCE_TYPE_WEAPON;
			return weapon.GetHierarchyRootPlayer();
		}
		else if (source.IsInherited(CarScript)) {
			CarScript car = CarScript.Cast(source);
			Human driver = Human.Cast(car.CrewMember(0));
			wpnType = PVEZ_DAMAGE_SOURCE_TYPE_VEHICLE;
			return driver;
		}
		else if (source.IsInherited(SurvivorBase)) { // player uses fists
			wpnType = PVEZ_DAMAGE_SOURCE_TYPE_FISTS;
			return source;
		}
		else {
			return NULL;
		}
	}

	void HealDamageReceived(TotalDamageResult damageResult, string dmgZone, bool gotBleeding) {
		// Player might be dead already (e.g., one shot - one kill), in this case healing will break the respawn process -
		// player is dead but gets health restored and there's no menu to respawn,
		// or the game will just reset health to 0 after healing.
		// Because when player's health drops to 0, seems like the game puts the player to a deadmen list somewhere.
		// I can't find any way to restore the player after that.
		// So, let's check if player is alive before restoring any health:
		if (entity.IsAlive()) {
			if (gotBleeding) {
				// Get bleeding sources and remove one of them
				PlayerBase player = PlayerBase.Cast(entity);
				if (player)
					player.GetBleedingManagerServer().RemoveMostSignificantBleedingSource();
			}
			
			entity.AddHealth(dmgZone, "", damageResult.GetDamage(dmgZone, "")); //restore health of the damaged body part
			entity.AddHealth("", "", damageResult.GetDamage("", "")); //restore global health
			entity.AddHealth("", "Shock", damageResult.GetDamage("", "Shock")); //restore from shock
		}
	}
	
	void ProcessDamageReflection(int wpnType, float healthDmgAmount) {
		if (DirectDmgInitiator && DirectDmgInitiator.IsAlive()) {
			if (ShouldReflectDamage(wpnType)) {
				float dmg = healthDmgAmount;
				DirectDmgInitiator.AddHealth(-dmg);
			}
		}
	}

	private bool ShouldReflectDamage(int weaponType) {
		switch (weaponType) {
			case PVEZ_DAMAGE_SOURCE_TYPE_EXPLOSIVE:
				return g_Game.pvez_Config.DAMAGE.Damage_Types_Sent_Back_To_The_Attacker.Explosive_Damage;
			case PVEZ_DAMAGE_SOURCE_TYPE_WEAPON:
				return g_Game.pvez_Config.DAMAGE.Damage_Types_Sent_Back_To_The_Attacker.Weapon_Damage;
			case PVEZ_DAMAGE_SOURCE_TYPE_VEHICLE:
				return g_Game.pvez_Config.DAMAGE.Damage_Types_Sent_Back_To_The_Attacker.Vehicle_Damage;
			case PVEZ_DAMAGE_SOURCE_TYPE_FISTS:
				return g_Game.pvez_Config.DAMAGE.Damage_Types_Sent_Back_To_The_Attacker.Fist_Fight_Damage;
		}
		return false;
	}
}