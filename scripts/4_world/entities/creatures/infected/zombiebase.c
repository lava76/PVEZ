#ifdef PVEZ_DEBUGMODE

modded class ZombieBase {
	private bool pvez_IsInPVP = false;
	private bool pvez_IsLawbreaker = false;
	private EntityAI pvez_DmgInitiator; //root entity (player, infected, animal) of the previous damage source
	private int pvez_weaponType;	
	private bool pvez_death;
	private autoptr PVEZ_DamageRedistributor pvez_DamageRedistributor;

	override void Init() {
		super.Init();
		pvez_DamageRedistributor = new PVEZ_DamageRedistributor(this);
	}

	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef) {
		if (!super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef))
			return false;

		bool isDamageAllowed = true;

		if (GetGame().IsServer() || !GetGame().IsMultiplayer()) {
			if (!IsAlive() || g_Game.pvez_Config.GENERAL.Mode == PVEZ_MODE_PVP)
				return true;

			pvez_DamageRedistributor.RegisterHit(this, source, pvez_weaponType, false);
			if (!pvez_DamageRedistributor.LastHitWasAllowed() && damageResult) {
				if (g_Game.pvez_Config.DAMAGE.Restore_Target_Health) {
					isDamageAllowed = false;
				}
				pvez_DamageRedistributor.ProcessDamageReflection(pvez_weaponType, damageResult.GetDamage("", ""));
			}
		}

		return isDamageAllowed;
	}

	override bool EvaluateDeathAnimation(EntityAI pSource, string pComponent, string pAmmoType, out int pAnimType, out float pAnimHitDir) {
		bool result = super.EvaluateDeathAnimation(pSource, pComponent, pAmmoType, pAnimType, pAnimHitDir);

		if (GetGame().IsServer() || !GetGame().IsMultiplayer()) {
			if (!IsAlive() && !pvez_death) {
				EntityAI killerEntity = pSource;
				pvez_DamageRedistributor.RegisterDeath(this, killerEntity, pvez_weaponType);
				SetLawbreaker(false);
			}
		}
		pvez_death = true;
		return true;
	}

	bool GetIsInPVP() {
		if (GetGame().IsServer() || !GetGame().IsMultiplayer()) {
			int zoneIndex = g_Game.pvez_Zones.GetZoneIndex(GetPosition());
			switch (g_Game.pvez_Config.GENERAL.Mode) {
				case PVEZ_MODE_PVP:
					pvez_IsInPVP = true; //PVP always on
					break;
				case PVEZ_MODE_PVE:
					pvez_IsInPVP = false; //PVP always off
					//valueZone = (zone >= 0);
					//pvez_IsInPVP = valueZone; //PVP is on in zones (PVP still should work within Airdrop zones)
					break;
				case PVEZ_MODE_PVP_ZONES:
					pvez_IsInPVP = (zoneIndex >= 0);
					break;
				case PVEZ_MODE_PVE_ZONES:
					pvez_IsInPVP = (zoneIndex < 0);
					break;
			}
		}
		return pvez_IsInPVP;
	}

	void SetLawbreaker(bool value) {
		if (GetGame().IsServer() || !GetGame().IsMultiplayer()) {
			g_Game.pvez_LawbreakersRoster.Update(this, value, PVEZ_Date.Now());
			g_Game.pvez_LawbreakersMarkers.Update(this, value);
			pvez_IsLawbreaker = value;
		}
	}

	bool PVEZ_IsPvpAttackAllowed() {
		EntityAI source = pvez_DamageRedistributor.GetDirectDmgInitiator();
		// If this player is a lawbreaker:
		if (pvez_IsLawbreaker)
			return true;
		// if both fighters are in PVP zone:
		else if (GetIsInPVP() && PlayerBase.Cast(source).pvez_PlayerStatus.GetIsInPVP())
			return true;
		// if config allows PVP when at least one of them is in PVP zone:
		else if (g_Game.pvez_Config.DAMAGE.Allow_Damage_Between_PVP_and_PVE)
			return GetIsInPVP() || PlayerBase.Cast(source).pvez_PlayerStatus.GetIsInPVP();
		else
			return false;
	}
}
#endif