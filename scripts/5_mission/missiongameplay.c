modded class MissionGameplay {

	void MissionGameplay() {
		g_Game.PVEZ_Init();
	}

	void ~MissionGameplay() {
		g_Game.pvez_LawbreakersRoster.SaveToJson();
	}

	override void OnUpdate(float timeslice) {
		super.OnUpdate(timeslice);

		if (GetUApi().GetInputByName("UAPVEZOpenAdminMenu").LocalPress()) {
			UIScriptedMenu menu = GetUIManager().GetMenu();

			if (!PlayerBase.Cast(GetGame().GetPlayer()).IsPVEZAdmin()) {
				//PlayerBase.Cast(GetGame().GetPlayer()).MessageStatus("PVEZ: You're not an admin. Don't touch this.");
				return;
			}
			if (!menu && !PlayerBase.Cast(GetGame().GetPlayer()).GetPVEZAdminMenu().GetLayoutRoot().IsVisible()) {
				GetUIManager().ShowScriptedMenu(PlayerBase.Cast(GetGame().GetPlayer()).GetPVEZAdminMenu(), NULL);
				PlayerControlDisable(INPUT_EXCLUDE_ALL);
			} else if (PlayerBase.Cast(GetGame().GetPlayer()).GetPVEZAdminMenu().GetLayoutRoot().IsVisible()) {
				// If the cursor is on some edit box then the user is typing some text, don't close the GUI
				if (!PlayerBase.Cast(GetGame().GetPlayer()).GetPVEZAdminMenu().IsTypingText()) {
					GetUIManager().HideScriptedMenu(PlayerBase.Cast(GetGame().GetPlayer()).GetPVEZAdminMenu());
					PlayerControlEnable(false);
				}
			}
		}

		if (GetUApi().GetInputByName("UAUIBack").LocalPress() && GetGame().GetPlayer() && PlayerBase.Cast(GetGame().GetPlayer()).GetPVEZAdminMenu().GetLayoutRoot().IsVisible()) {
			GetUIManager().HideScriptedMenu(PlayerBase.Cast(GetGame().GetPlayer()).GetPVEZAdminMenu());
			PlayerControlEnable(false);
		}

		if (!GetGame().IsMultiplayer()) {
			PVEZ_OneMinuteTimer += timeslice;
			if (PVEZ_OneMinuteTimer >= 59) {
				PVEZ_OneMinuteTimer = 0;
			
				// Reinit zones every hour to enable/disable them based on their schedule:
				PVEZ_UpdateZonesActivity();
			
				// Check if lawbreakers have been reinitialized (when someone gets their flag off)
				if (g_Game.pvez_LawbreakersRoster.updated) {
					PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
					bool isLB = g_Game.pvez_LawbreakersRoster.Check(player);
					player.pvez_PlayerStatus.SetLawbreaker(isLB);
					g_Game.pvez_LawbreakersRoster.updated = false;
				}
			}

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

	void PVEZ_UpdatePlayersZoneStatus() {
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player && player.pvez_PlayerStatus) {
			int zone = g_Game.pvez_Zones.GetZoneIndex(player.GetPosition());
			player.pvez_PlayerStatus.Update(zone, false);
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