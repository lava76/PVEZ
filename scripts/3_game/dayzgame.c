modded class DayZGame {

	autoptr PVEZ_Config pvez_Config;
	autoptr PVEZ_Zones pvez_Zones;
	autoptr PVEZ_LawbreakersMarkers pvez_LawbreakersMarkers;
	autoptr PVEZ_LawbreakersRoster pvez_LawbreakersRoster;
	autoptr PVEZ_Bounties pvez_Bounties;

	void PVEZ_Init() {
		pvez_Config = new PVEZ_Config;
		pvez_Zones = new PVEZ_Zones;
		pvez_LawbreakersMarkers = new PVEZ_LawbreakersMarkers;
		pvez_LawbreakersRoster = new PVEZ_LawbreakersRoster(pvez_Config);
		
		if (IsServer() || !IsMultiplayer()) {
			pvez_Zones.Init();
			pvez_Bounties = new PVEZ_Bounties;
		}
	}

	void PVEZ_SendConfigToClient(DayZPlayer dzp = NULL) {
		if (IsServer()) {
			// Send PVEZ config to the client via RPC
			Param1<PVEZ_Config> configData = new Param1<PVEZ_Config>(pvez_Config);
			if (dzp)
				RPCSingleParam(dzp, PVEZ_RPC.UPDATE_CONFIG, configData, true, dzp.GetIdentity()); // to the given player
			else
				PVEZ_RPCForAllClients(PVEZ_RPC.UPDATE_CONFIG, configData); // to all players
		}
	}

	void PVEZ_SendActiveZonesToClient(DayZPlayer dzp = NULL) {
		if (IsServer()) {
			Param1<array<ref PVEZ_Zone>> zonesData = new Param1<array<ref PVEZ_Zone>>(pvez_Zones.activeZones);
			if (dzp)
				RPCSingleParam(dzp, PVEZ_RPC.UPDATE_ZONES, zonesData, true, dzp.GetIdentity());
			else
				PVEZ_RPCForAllClients(PVEZ_RPC.UPDATE_ZONES, zonesData);
		}
	}

	void PVEZ_GetAdminStatus(DayZPlayer dzp) {
		if (IsServer()) {
			bool isAdmin = false;
			// 44 signs hashed Steam ID
			string hashId = dzp.GetIdentity().GetId();
			// decimal SteamID 64 (account ID)
			string id = dzp.GetIdentity().GetPlainId();

			FileHandle file_handle = OpenFile(PVEZ_ADMINS_TXT, FileMode.READ);
			string line_content;
			while (FGets(file_handle, line_content) > 0) {
				if (line_content == hashId || line_content == id) {
					isAdmin = true;
					break;
				}
			}
			CloseFile(file_handle);
			
			Param1<bool> data = new Param1<bool>(isAdmin);
			RPCSingleParam(dzp, PVEZ_RPC.ADMIN_ACCESS_REQUEST, data, true, dzp.GetIdentity());
		}
	}

	void PVEZ_RPCForAllClients(int type, Param data) {
		if (GetGame().GetWorld()) {
			array<Man> players = new array<Man>;
			GetGame().GetWorld().GetPlayerList(players);
			if (players.Count() > 0) {
				foreach (Man p : players) {
					RPCSingleParam(p, type, data, true, p.GetIdentity());
				}
			}
		}
	}

	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {
		super.OnRPC(sender, target, rpc_type, ctx);

		if (IsClient()) {
			switch (rpc_type) {
				case PVEZ_RPC.UPDATE_LAWBREAKERS_MARKERS:
					// Assign markers array with the new data sent by server.
					Param1<array<ref PVEZ_LawbreakerMarker>> data2 = new Param1<array<ref PVEZ_LawbreakerMarker>>(NULL);
					if (!ctx.Read(data2)) break;
					pvez_LawbreakersMarkers.Sync(data2.param1);
					break;
			}
		}
		else {
			switch (rpc_type) {
				case PVEZ_RPC.UPDATE_CONFIG:
					Param1<PVEZ_Config> data4 = new Param1<PVEZ_Config>(NULL);
					if (!ctx.Read(data4)) {
						RPCSingleParam(DayZPlayer.Cast(target), ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>("PVEZ: ERROR. Failed to apply new settings."), true, DayZPlayer.Cast(target).GetIdentity());
						PVEZ_SendConfigToClient(DayZPlayer.Cast(target));
						break;
					}
					pvez_Config = data4.param1;
					pvez_Config.SaveToJson();
					pvez_Zones.Init(); // Re-init zones to reactivate them if the week mode setting has been changed
					PVEZ_SendConfigToClient();
					RPCSingleParam(DayZPlayer.Cast(target), ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>("PVEZ: Settings applied on server."), true, DayZPlayer.Cast(target).GetIdentity());
					break;
				case PVEZ_RPC.UPDATE_ZONES:
					Param1<array<ref PVEZ_Zone>> data5 = new Param1<array<ref PVEZ_Zone>>(NULL);
					if (!ctx.Read(data5)) {
						RPCSingleParam(DayZPlayer.Cast(target), ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>("PVEZ: ERROR. Failed to apply new zones data."), true, DayZPlayer.Cast(target).GetIdentity());
						PVEZ_SendActiveZonesToClient(DayZPlayer.Cast(target));
						break;
					}
					pvez_Zones.staticZones = data5.param1;
					pvez_Zones.SaveToJson();
					pvez_Zones.Init();
					RPCSingleParam(DayZPlayer.Cast(target), ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>("PVEZ: New zones applied on server."), true, DayZPlayer.Cast(target).GetIdentity());
					break;
				case PVEZ_RPC.UPDATE_BOUNTIES:
					Param2<bool, array<ref PVEZ_BountyItemData>> data6 = new Param2<bool, array<ref PVEZ_BountyItemData>>(false, NULL);
					if (!ctx.Read(data6)) {
						RPCSingleParam(DayZPlayer.Cast(target), ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>("PVEZ: ERROR. Failed to read new bounties data."), true, DayZPlayer.Cast(target).GetIdentity());
						break;
					}
					pvez_Bounties.Enabled = data6.param1;
					pvez_Bounties.Items = data6.param2;
					pvez_Bounties.SaveToJson();
					RPCSingleParam(DayZPlayer.Cast(target), ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>("PVEZ: Bounties settings applied on server."), true, DayZPlayer.Cast(target).GetIdentity());
					break;
				case PVEZ_RPC.ADMIN_ZONES_DATA_REQUEST:
					Param1<array<ref PVEZ_Zone>> data7 = new Param1<array<ref PVEZ_Zone>>(pvez_Zones.staticZones);
					RPCSingleParam(DayZPlayer.Cast(target), PVEZ_RPC.ADMIN_ZONES_DATA_REQUEST, data7, true, DayZPlayer.Cast(target).GetIdentity());
					break;
				case PVEZ_RPC.ADMIN_LAWBREAKERS_DATA_REQUEST:
					array<Man> players = new array<Man>;
					GetPlayers(players);
					Param2<array<ref PVEZ_Lawbreaker>, array<Man>> data8 = new Param2<array<ref PVEZ_Lawbreaker>, array<Man>>(pvez_LawbreakersRoster.lbDataBase, players);
					RPCSingleParam(DayZPlayer.Cast(target), PVEZ_RPC.ADMIN_LAWBREAKERS_DATA_REQUEST, data8, true, DayZPlayer.Cast(target).GetIdentity());
					break;
				case PVEZ_RPC.ADMIN_BOUNTIES_DATA_REQUEST:
					Param1<PVEZ_Bounties> data9 = new Param1<PVEZ_Bounties>(pvez_Bounties);
					RPCSingleParam(DayZPlayer.Cast(target), PVEZ_RPC.ADMIN_BOUNTIES_DATA_REQUEST, data9, true, DayZPlayer.Cast(target).GetIdentity());
					break;
				case PVEZ_RPC.ADMIN_UPDATE_LAWBREAKERS:
					Param1<array<ref PVEZ_Lawbreaker>> data10 = new Param1<array<ref PVEZ_Lawbreaker>>(NULL);
					if (!ctx.Read(data10)) break;
					pvez_LawbreakersRoster.lbDataBase = data10.param1;
					pvez_LawbreakersRoster.SaveToJson();
					pvez_LawbreakersRoster = new PVEZ_LawbreakersRoster(pvez_Config);
					// Send updated database back to admin client (the sender of this RPC call):
					array<Man> players2 = new array<Man>;
					GetPlayers(players2);
					Param2<array<ref PVEZ_Lawbreaker>, array<Man>> data11 = new Param2<array<ref PVEZ_Lawbreaker>, array<Man>>(pvez_LawbreakersRoster.lbDataBase, players2);
					RPCSingleParam(DayZPlayer.Cast(target), PVEZ_RPC.ADMIN_LAWBREAKERS_DATA_REQUEST, data11, true, DayZPlayer.Cast(target).GetIdentity());
					break;
			}
		}
	}
}