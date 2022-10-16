modded class PlayerBase {

	protected bool isPVEZAdmin;
	bool IsPVEZAdmin() { return isPVEZAdmin; }

	// GUI widgets for client instance.
	protected autoptr PVEZ_NotificationGUI pvez_NotificationGUI;
	protected autoptr PVEZ_AdminConsoleGUI pvez_AdminConsoleGUI;

	autoptr PVEZ_PlayerStatus pvez_PlayerStatus;
	autoptr PVEZ_BountiesSpawner pvez_BountiesSpawner;

	// Used to decide whether the damage should be reflected back depending on config settings.
	protected int pvez_weaponType;

	autoptr PVEZ_DamageRedistributor pvez_DamageRedistributor;

	override void OnPlayerLoaded() {
		super.OnPlayerLoaded();

		// Check if identity != NULL. This code runs twice, in main menu and then in-game.
		// At first run player's identity is NULL and IsMultiplayer() is false.
		if (GetGame().IsMultiplayer()) {
			if (GetGame().IsServer() && GetIdentity()) {
				g_Game.PVEZ_SendConfigToClient(this);
				g_Game.PVEZ_SendActiveZonesToClient(this);
				g_Game.PVEZ_GetAdminStatus(this);
				pvez_PlayerStatus = new PVEZ_PlayerStatus(this);
				pvez_BountiesSpawner = new PVEZ_BountiesSpawner(this);
				pvez_DamageRedistributor = new PVEZ_DamageRedistributor(this);
			}
			else if (GetGame().IsClient())
				pvez_PlayerStatus = new PVEZ_PlayerStatus(this);
		}
		else {
			// Hud will be initialized in-game, in main menu this should be skipped to prevent client side error on PVEZ icon update.
			if (m_Hud && IsControlledPlayer()) {
				isPVEZAdmin = true;
				pvez_PlayerStatus = new PVEZ_PlayerStatus(this);
				pvez_BountiesSpawner = new PVEZ_BountiesSpawner(this);
				pvez_DamageRedistributor = new PVEZ_DamageRedistributor(this);
			}
		}
	}

	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef) {
		if (!super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef))
			return false;

		if (!IsAlive() || g_Game.pvez_Config.GENERAL.Mode == PVEZ_MODE_PVP)
			return true;
		
		if (GetGame().IsMultiplayer() && !GetGame().IsServer())
			return true;

		// Workaround for AI bots, they don't have PVEZ_DamageRedistributor
		if (!pvez_DamageRedistributor)
			return true;

		bool isDamageAllowed = true;

		pvez_DamageRedistributor.RegisterHit(this, source, pvez_weaponType, false);
		if (!pvez_DamageRedistributor.LastHitWasAllowed() && damageResult) {
			if (g_Game.pvez_Config.DAMAGE.Restore_Target_Health)
				isDamageAllowed = false;
			pvez_DamageRedistributor.ProcessDamageReflection(pvez_weaponType, damageResult.GetDamage("", ""));
		}

		return isDamageAllowed;
	}

	override void OnBleedingSourceRemoved() {
		super.OnBleedingSourceRemoved();

		// Seems like sometimes this could be called very early when the DamageRedistributor is not ready yet,
		// so have check if it's ready
		if (IsAlive() && m_BleedingSourceCount == 0 && pvez_DamageRedistributor)
			pvez_DamageRedistributor.OnAllBleedingSourcesRemoved();
	}

	override void EEKilled(Object killer) {
		super.EEKilled(killer);

		if (GetGame().IsServer() || !GetGame().IsMultiplayer()) {
			// AI bots don't have identity and thus no PVEZ_PlayerStatus or PVEZ_DamageRedistributor
			if (!pvez_PlayerStatus) {
				if (GetIdentity())
					Print(PVEZ_ERROR_PREFIX + "No PlayerStatus on death!");
				return;
			}

			if (!pvez_DamageRedistributor)
				return;

			pvez_DamageRedistributor.RegisterDeath(this, EntityAI.Cast(killer), pvez_weaponType);
			if (PVEZ_ShouldBePardonedOnDeath())
				pvez_PlayerStatus.SetLawbreaker(false);
		}
	}

	bool PVEZ_ShouldBePardonedOnDeath() {
		if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Pardon_On_Death_From_Any_Source) {
			return true;
		}
		else if (pvez_DamageRedistributor.IsKilledByAnotherPlayer()) {
			PlayerBase killerPlayer = PlayerBase.Cast(pvez_DamageRedistributor.GetKillerEntity());
			if (killerPlayer && !killerPlayer.pvez_PlayerStatus.GetIsLawbreaker()) {
				return true;
			}
		}
		return false;
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx) {
		super.OnRPC(sender, rpc_type, ctx);

		// All UI functions can only be executed on client.
		if (GetGame().IsClient()) {
			switch (rpc_type) {
				case PVEZ_RPC.UPDATE_CONFIG:
					Param1<PVEZ_Config> data1 = new Param1<PVEZ_Config>(NULL);
					if (!ctx.Read(data1)) {
						MessageStatus("PVEZ: Failed to get config from server.");
						break;
					}
					g_Game.pvez_Config = data1.param1;
					break;
				case PVEZ_RPC.UPDATE_ZONES:
					Param1<array<ref PVEZ_Zone>> data2 = new Param1<array<ref PVEZ_Zone>>(NULL);
					if (!ctx.Read(data2)) break;
					g_Game.pvez_Zones.activeZones = data2.param1;
					break;
				case PVEZ_RPC.ADMIN_ZONES_DATA_REQUEST:
					if (isPVEZAdmin && GetPVEZAdminMenu().GetLayoutRoot().IsVisible()) {
						Param1<array<ref PVEZ_Zone>> dataAZ = new Param1<array<ref PVEZ_Zone>>(NULL);
						if (!ctx.Read(dataAZ)) {
							MessageStatus("PVEZ: Failed to get new zones settings from server.");
							break;
						}
						g_Game.pvez_Zones.staticZones = dataAZ.param1;
						GetPVEZAdminMenu().UpdateZonesList();
					}
					break;
				case PVEZ_RPC.ADMIN_LAWBREAKERS_DATA_REQUEST:
					if (isPVEZAdmin && GetPVEZAdminMenu().GetLayoutRoot().IsVisible()) {
						Param2<array<ref PVEZ_Lawbreaker>, array<Man>> data3 = new Param2<array<ref PVEZ_Lawbreaker>, array<Man>>(NULL, NULL);
						if (!ctx.Read(data3)) {
							MessageStatus("PVEZ: Failed to get lawbreakers data from server.");
							break;
						}
						g_Game.pvez_LawbreakersRoster.lbDataBase = data3.param1;
						GetPVEZAdminMenu().UpdateLawbreakersList(data3.param2);
					}
					break;
				case PVEZ_RPC.ADMIN_BOUNTIES_DATA_REQUEST:
					if (isPVEZAdmin && GetPVEZAdminMenu().GetLayoutRoot().IsVisible()) {
						Param1<PVEZ_Bounties> data4 = new Param1<PVEZ_Bounties>(NULL);
						if (!ctx.Read(data4)) {
							MessageStatus("PVEZ: Failed to get bounties data from server.");
							break;
						}
						GetPVEZAdminMenu().UpdateBountiesPage(data4.param1);
					}
					break;
				case PVEZ_RPC.UPDATE_STATUS_ON_CLIENT:
					// Here the params sent should contain 2 booleans: IsInPVP & IsLawbreaker, and the zone data (could be NULL if left a zone).
					Param3<bool, bool, int> data5 = new Param3<bool, bool, int>(false, false, -1);
					if (!ctx.Read(data5)) break;
					// Update client-side IsInPVP and process forced 1st person view when required
					if (pvez_PlayerStatus) {
						pvez_PlayerStatus.IsInPVP = data5.param1;
						if (pvez_PlayerStatus.IsInPVP && g_Game.pvez_Config.GENERAL.Force1stPersonInPVP) {
							DayZPlayerImplement dzp = this;
							if (dzp)
								dzp.m_Camera3rdPerson = false;
						}
					}
					if (m_Hud)
						m_Hud.UpdatePVEZIcon(data5.param1, data5.param2, data5.param3);
					break;
				case PVEZ_RPC.NOTIFICATION_PERSONAL:
					Param4<int, int, bool, string> data6 = new Param4<int, int, bool, string>(-1, 0, false, "");
					if (!ctx.Read(data6)) break;
					PVEZ_Notifications.NotificationFromType(data6.param1, data6.param2, data6.param3, data6.param4);
					break;
				case PVEZ_RPC.NOTIFICATION_SERVERWIDE:
					Param1<string> data7 = new Param1<string>("");
					if (!ctx.Read(data7)) break;
					PVEZ_Notifications.NotificationFromString(data7.param1);
					break;
				case PVEZ_RPC.ADMIN_ACCESS_REQUEST:
					Param1<bool> data8 = new Param1<bool>(false);
					if (!ctx.Read(data8)) break;
					isPVEZAdmin = data8.param1;
					if (isPVEZAdmin)
						MessageStatus("PVEZ: You've got PVEZ Admin status.");
					break;
			}
		}
	}

	override void OnScheduledTick(float deltaTime) {
		super.OnScheduledTick(deltaTime);

		if (pvez_PlayerStatus)
			pvez_PlayerStatus.OnScheduledTick(deltaTime);
	}

	bool PVEZ_ShouldProtectClothing() {
		if (g_Game.pvez_Config)
			return g_Game.pvez_Config.DAMAGE.Protect_Clothing_And_Cargo;
		else return false;
	}

	PVEZ_AdminConsoleGUI GetPVEZAdminMenu() {
		if (!pvez_AdminConsoleGUI) {
			pvez_AdminConsoleGUI = new PVEZ_AdminConsoleGUI;
			pvez_AdminConsoleGUI.Init();
		}
		return pvez_AdminConsoleGUI;
	}
}