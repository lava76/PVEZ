class PVEZ_Notifications : Managed {

//////////////////////////////////////////////////////////////////////////
////                            ON SERVER                             ////
	static void PersonalNotification(DayZPlayer recipient, int type, int duration, bool isCountdown, string msg = "") {
		if (GetGame().IsMultiplayer()) {
			Param4<int, int, bool, string> data = new Param4<int, int, bool, string>(type, duration, isCountdown, msg);
			GetGame().RPCSingleParam(recipient, PVEZ_RPC.NOTIFICATION_PERSONAL, data, true, recipient.GetIdentity());
		}
		else
			NotificationFromType(type, duration, isCountdown, msg);
	}

	static void NotificationToAll(EntityAI killer, EntityAI victim, EntityAI weapon) {
		if (!g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Server_Wide_Message_About_Lawbreaker)
			return;
		
		string killerName = PVEZ_StaticFunctions.GetEntityName(killer);
		string victimName = PVEZ_StaticFunctions.GetEntityName(victim);
		string weaponDisplayName;
		if (weapon == killer)
			weaponDisplayName = "#pvez_bare_hands";
		else
			weaponDisplayName = weapon.GetDisplayName();
		string serverMessage = FormatMessage(PVEZ_NotificationType.NOTIF_LB_SERVERWIDE, killerName, victimName, weaponDisplayName);

		if (GetGame().GetWorld()) {
			array<Man> players = new array<Man>;
			GetGame().GetWorld().GetPlayerList(players);
			if (players.Count() > 0) {
				foreach (Man p : players) {
					if (!p.IsAlive() || p == killer)
						continue;

					if (g_Game.pvez_Config.GENERAL.Use_UI_Notifications)
						GetGame().RPCSingleParam(p, PVEZ_RPC.NOTIFICATION_SERVERWIDE, new Param1<string>(serverMessage), true, p.GetIdentity());
					else
						GetGame().RPCSingleParam(p, ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>(serverMessage), true, p.GetIdentity());
				}
			}
			delete players;
		}
	}

	static void IconUpdate(DayZPlayer player, bool isInPVP, bool isLawbreaker, int zone) {
		if (GetGame().IsMultiplayer())
			GetGame().RPCSingleParam(player, PVEZ_RPC.UPDATE_STATUS_ON_CLIENT, new Param3<bool, bool, int>(isInPVP, isLawbreaker, zone), true, player.GetIdentity());
		else
			GetGame().GetMission().GetHud().UpdatePVEZIcon(isInPVP, isLawbreaker, zone);
	}
////___________________________________________________________________////


///////////////////////////////////////////////////////////////////////////
////                            ON CLIENT                              ////
	static void NotificationFromType(int type, int duration, bool isCountdown, string msg) {
		switch (type) {
			case PVEZ_NotificationType.NOTIF_ZONE_ENTER:
			case PVEZ_NotificationType.NOTIF_ZONE_EXIT:
				NotificationZone(5, false, msg);
				break;
			case PVEZ_NotificationType.NOTIF_EXIT_COUNTDOWN:
				NotificationCountdown();
				break;
			case PVEZ_NotificationType.NOTIF_LB_PERSONAL:
				NotificationLawbreakerPersonal();
				break;
			case PVEZ_NotificationType.NOTIF_GENERIC:
			case PVEZ_NotificationType.NOTIF_LB_SERVERWIDE:
				NotificationFromString(msg);
				break;
		}
	}

	static void NotificationFromString(string msg) {
		GetGame().GetMission().GetHud().ShowPVEZNotification(msg, 10, false);
	}

	static void NotificationZone(int duration, bool isCountdown, string zoneIndex) {
		string msg = "";
		
		//On entering the zone
		if (zoneIndex != "") {
			PVEZ_Zone zone = g_Game.pvez_Zones.GetByIndex(zoneIndex.ToInt());
			msg = FormatMessage(PVEZ_NotificationType.NOTIF_ZONE_ENTER, zone.Name);
		}
		//On leaving the zone
		else {
			msg = FormatMessage(PVEZ_NotificationType.NOTIF_ZONE_EXIT);
		}
		
		if (g_Game.pvez_Config.GENERAL.Use_UI_Notifications && GetGame().GetMission().GetHud())
			GetGame().GetMission().GetHud().ShowPVEZNotification(msg, duration, isCountdown);
		else
			GetGame().ChatPlayer(msg);
	}

	static void NotificationCountdown() {
		if (g_Game.pvez_Config.GENERAL.Use_UI_Notifications) {
			string msg = FormatMessage(PVEZ_NotificationType.NOTIF_EXIT_COUNTDOWN);
			int countdownTime = g_Game.pvez_Config.GENERAL.Exit_Zone_Countdown;
			GetGame().GetMission().GetHud().ShowPVEZNotification(msg, countdownTime, true);
		}
	}

	static void NotificationLawbreakerPersonal() {
		string msg = FormatMessage(PVEZ_NotificationType.NOTIF_LB_PERSONAL);
		if (g_Game.pvez_Config.GENERAL.Use_UI_Notifications)
			GetGame().GetMission().GetHud().ShowPVEZNotification(msg, 10, false);
		else
			GetGame().ChatPlayer(msg);
	}

	static string FormatMessage(int type, string str1 = "", string str2 = "", string str3 = "") {
		string result = "";
		switch (type) {
			case PVEZ_NotificationType.NOTIF_ZONE_ENTER:
				if (g_Game.pvez_Config.GENERAL.Custom_Enter_Zone_Message != string.Empty)
					result = g_Game.pvez_Config.GENERAL.Custom_Enter_Zone_Message;
				else {
					if (g_Game.pvez_Config.GENERAL.Mode == PVEZ_MODE_PVE_ZONES)
						result = "#pvez_entr_msg_pve";
					else
						result = "#pvez_entr_msg_pvp";
				}
				if (g_Game.pvez_Config.GENERAL.Add_Zone_Name_To_Message && str1 != string.Empty)
					result = result + " " + str1;
				break;
			case PVEZ_NotificationType.NOTIF_ZONE_EXIT:
				if (g_Game.pvez_Config.GENERAL.Custom_Exit_Zone_Message != string.Empty)
					result = g_Game.pvez_Config.GENERAL.Custom_Exit_Zone_Message;
				else {
					if (g_Game.pvez_Config.GENERAL.Mode == PVEZ_MODE_PVE_ZONES)
						result = "#pvez_exit_msg_pve";
					else
						result = "#pvez_exit_msg_pvp";
				}
				break;
			case PVEZ_NotificationType.NOTIF_EXIT_COUNTDOWN:
				if (g_Game.pvez_Config.GENERAL.Custom_Exit_Zone_Countdown_Message != string.Empty)
					result = g_Game.pvez_Config.GENERAL.Custom_Exit_Zone_Countdown_Message;
				else {
					if (g_Game.pvez_Config.GENERAL.Mode == PVEZ_MODE_PVE_ZONES)
						result = "#pvez_cdwn_msg_pve";
					else
						result = "#pvez_cdwn_msg_pvp";
				}
				break;
			case PVEZ_NotificationType.NOTIF_LB_PERSONAL:
				if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Custom_Message_for_the_Lawbreaker != string.Empty)
					result = g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Custom_Message_for_the_Lawbreaker;
				else
					result = "#pvez_lb_personal";
				break;
			case PVEZ_NotificationType.NOTIF_LB_SERVERWIDE:
				if (g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Custom_Crime_Notification_Message_Format != string.Empty) {
					result = g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Custom_Crime_Notification_Message_Format;
					result.Replace("1", str1);
					result.Replace("2", str2);
					result.Replace("3", str3);
				}
				else
					result = str1 + " " + "#pvez_lb_serverwide1" + " " + str2 + "#pvez_lb_serverwide2" + " " + str3;
				break;
		}
		return result;
	}
////__________________________________________________________________////
}