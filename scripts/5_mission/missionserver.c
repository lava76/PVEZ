modded class MissionBase {
	float PVEZ_ZonesUpdateTimer;
	float PVEZ_LBMarkersUpdateTimer;
	float PVEZ_OneMinuteTimer;
	bool PVEZNeedUpdate;
}

modded class MissionServer {

	/// This doesn't work on servers with long run times (4-5+ hours)
	/*
	override void OnInit() {
		super.OnInit();

		// Register map markers update function
		if (g_Game.pvez_Config.MAP.Lawbreakers_Markers.Update_Frequency > 0)
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.PVEZ_UpdateLBMarkers, g_Game.pvez_Config.MAP.Lawbreakers_Markers.Update_Frequency * 1000, true);
		// Register PVP zones update function
		if (g_Game.pvez_Config.GENERAL.Update_Frequency > 0)
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.PVEZ_UpdatePlayersZoneStatus, g_Game.pvez_Config.GENERAL.Update_Frequency * 1000, true);
	}
	void ~MissionServer() {
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.PVEZ_UpdateLBMarkers);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.PVEZ_UpdatePlayersZoneStatus);
	}
	*/

	void MissionServer() {
		g_Game.PVEZ_Init();

#ifdef PVEZ_DEBUGMODE
	Print("PVEZ :: Running in DEBUG mode.");
#endif
#ifdef EXPANSIONMODMISSIONS
	Print("PVEZ :: Expansion Missions defined.");
#endif
#ifdef EXPANSIONMODNAVIGATION
	Print("PVEZ :: Expansion Navigation defined.");
#endif
	}

	void ~MissionServer() {
		g_Game.pvez_LawbreakersRoster.SaveToJson();
	}

	override void OnUpdate(float timeslice) {
		super.OnUpdate(timeslice);

		if (GetGame().IsMultiplayer()) {
			PVEZ_OneMinuteTimer += timeslice;
			if (PVEZ_OneMinuteTimer >= 59) {
				PVEZ_OneMinuteTimer = 0;
			
				// Reinit zones every hour to enable/disable them based on their schedule:
				PVEZ_UpdateZonesActivity();

				g_Game.pvez_LawbreakersRoster.RemoveOutdated(g_Game.pvez_Config);
				// Check if lawbreakers have been reinitialized (when someone gets their flag off)
				if (g_Game.pvez_LawbreakersRoster.updated && m_Players.Count() > 0) {
					foreach (Man p : m_Players) {
						bool isLB = g_Game.pvez_LawbreakersRoster.Check(p);
						PlayerBase.Cast(p).pvez_PlayerStatus.SetLawbreaker(isLB);
					}
					g_Game.pvez_LawbreakersRoster.updated = false;
				}
			}

			if (m_Players.Count() > 0) {
				PVEZ_ZonesUpdateTimer += timeslice;
				PVEZ_LBMarkersUpdateTimer += timeslice;

				if (g_Game.pvez_Config.GENERAL.Update_Frequency > 0 && PVEZ_ZonesUpdateTimer >= g_Game.pvez_Config.GENERAL.Update_Frequency) {
					PVEZ_UpdatePlayersZoneStatus();
					PVEZ_ZonesUpdateTimer = 0;
				}

				if (g_Game.pvez_Config.MAP.Lawbreakers_Markers.Update_Frequency > 0 && PVEZ_LBMarkersUpdateTimer >= g_Game.pvez_Config.MAP.Lawbreakers_Markers.Update_Frequency) {
					PVEZ_UpdateLBMarkers();
					PVEZ_LBMarkersUpdateTimer = 0;
				}
			}
		}
	}

	override void PlayerDisconnected(PlayerBase player, PlayerIdentity identity, string uid) {
		// Dont't rely on <player>. It might not be NULL but <player.GetIdentity()> still might return NULL, FFS!
		// Removing marker by ID is the only way.
		g_Game.pvez_LawbreakersMarkers.RemoveMarker(uid);
		
		super.PlayerDisconnected(player, identity, uid);
	}

	void PVEZ_UpdatePlayersZoneStatus() {
		if (m_Players.Count() == 0)
			return;
		
		PlayerBase player;
		for (int i = 0; i < m_Players.Count(); i++) {
			player = PlayerBase.Cast(m_Players.Get(i));

			if (player && player.pvez_PlayerStatus) {
				int zone = g_Game.pvez_Zones.GetZoneIndex(player.GetPosition());
				player.pvez_PlayerStatus.Update(zone, false);
			}
		}
	}

	void PVEZ_UpdateLBMarkers() {
		g_Game.pvez_LawbreakersMarkers.UpdateMarkersPositions();
	}

	void PVEZ_UpdateZonesActivity() {
		int hour, min, sec;
		GetHourMinuteSecond(hour, min, sec);
		// Updating zones within first 2 minutes of every hour.
		if (PVEZNeedUpdate && min < 2) {
			g_Game.pvez_Zones.Init();
			PVEZNeedUpdate = false;
		}
		if (min > 2 && !PVEZNeedUpdate)
			PVEZNeedUpdate = true;
	}
}