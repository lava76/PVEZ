class PVEZ_Zone_Schedule : Managed {
	string Days = "1 2 3 4 5 6 7";
	int StartHour = 0;
	int EndHour = 24;
}

class PVEZ_Zone : Managed {
	int Type;
	string Name;
	float X;
	float Z;
	int Radius;
	bool ShowBorderOnMap;
	bool ShowNameOnMap;
	autoptr PVEZ_Zone_Schedule Activity_Schedule;
	
	void PVEZ_Zone(int type, float x, float z, string name, int radius, bool showBorder = true, bool showName = false) {
		Type = type;
		X = x;
		Z = z;
		Name = name;
		Radius = radius;
		ShowBorderOnMap = showBorder;
		ShowNameOnMap = showName;
		switch (type) {
			case PVEZ_ZONE_TYPE_STATIC:
				Activity_Schedule = new PVEZ_Zone_Schedule;
				break;
			case PVEZ_ZONE_TYPE_AIRDROP:
				Activity_Schedule = g_Game.pvez_Config.AIRDROP_ZONES.Activity_Schedule;
				break;
			case PVEZ_ZONE_TYPE_TERRITORYFLAG:
				Activity_Schedule = g_Game.pvez_Config.TERRITORYFLAG_ZONES.Activity_Schedule;
				break;
			case PVEZ_ZONE_TYPE_HELICRASH:
				Activity_Schedule = g_Game.pvez_Config.HELICRASH_ZONES.Activity_Schedule;
				break;
		}
	}

	bool GetIsActive() {
		int today = PVEZ_StaticFunctions.GetDayOfWeek(g_Game.pvez_Config.GENERAL.Week_Starts_On_Sunday);
		
		// Try to find today's index in the "Activity_Schedule.Days" string
		if (Activity_Schedule.Days.IndexOf(today.ToString()) >= 0) {
			// If found, compare current world time with the zone's activity hours
			int hour, minute, second;
			GetHourMinuteSecond(hour, minute, second);
			if (hour >= Activity_Schedule.StartHour && hour < Activity_Schedule.EndHour) {
				return true;
			}
		}
		return false;
	}

	vector GetVectorPos() {
		return Vector(X, 0, Z);
	}
}

class PVEZ_Zones : Managed {
	autoptr array<ref PVEZ_Zone> activeZones;
	autoptr array<ref PVEZ_Zone> staticZones;
	autoptr array<ref PVEZ_Zone> dynamicZones;

	void PVEZ_Zones() {
		activeZones = new array<ref PVEZ_Zone>;
		dynamicZones = new array<ref PVEZ_Zone>;
		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
			LoadFromJson();
	}

	void Init() {
		if (GetGame().IsServer() || !GetGame().IsMultiplayer()) {
			// Check if staticZones != NULL to prevent crashes when a dynamic zone is removed on the server shutdown.
			if (g_Game.pvez_Config && staticZones) {
				activeZones = new array<ref PVEZ_Zone>;
				if (g_Game.pvez_Config.GENERAL.Mode != PVEZ_MODE_PVE) {
					UpdateStaticZones();
					UpdateDynamicZones();
				}
				PushUpdateToClients();
			}
		}
	}

	void UpdateStaticZones() {
		// Add the zones that should be active at the moment based on their schedule
		foreach (auto zone: staticZones) {
			if (zone && zone.GetIsActive()) {
				activeZones.Insert(zone);
			}
		}
	}

	void UpdateDynamicZones() {
		for (int i = 0; i < dynamicZones.Count(); i++) {
			if (dynamicZones[i].Type == PVEZ_ZONE_TYPE_AIRDROP) {
				dynamicZones[i].Name = g_Game.pvez_Config.AIRDROP_ZONES.Name;
				dynamicZones[i].Radius = g_Game.pvez_Config.AIRDROP_ZONES.Radius;
				dynamicZones[i].ShowNameOnMap = g_Game.pvez_Config.AIRDROP_ZONES.ShowNameOnMap;
				dynamicZones[i].ShowBorderOnMap = g_Game.pvez_Config.AIRDROP_ZONES.ShowBorderOnMap;
				dynamicZones[i].Activity_Schedule = g_Game.pvez_Config.AIRDROP_ZONES.Activity_Schedule;
			}
			else if (dynamicZones[i].Type == PVEZ_ZONE_TYPE_TERRITORYFLAG) {
				dynamicZones[i].Name = g_Game.pvez_Config.TERRITORYFLAG_ZONES.Name;
				dynamicZones[i].Radius = g_Game.pvez_Config.TERRITORYFLAG_ZONES.Radius;
				dynamicZones[i].ShowNameOnMap = g_Game.pvez_Config.TERRITORYFLAG_ZONES.ShowNameOnMap;
				dynamicZones[i].ShowBorderOnMap = g_Game.pvez_Config.TERRITORYFLAG_ZONES.ShowBorderOnMap;
				dynamicZones[i].Activity_Schedule = g_Game.pvez_Config.TERRITORYFLAG_ZONES.Activity_Schedule;
			}
			else if (dynamicZones[i].Type == PVEZ_ZONE_TYPE_HELICRASH) {
				dynamicZones[i].Name = g_Game.pvez_Config.HELICRASH_ZONES.Name;
				dynamicZones[i].Radius = g_Game.pvez_Config.HELICRASH_ZONES.Radius;
				dynamicZones[i].ShowNameOnMap = g_Game.pvez_Config.HELICRASH_ZONES.ShowNameOnMap;
				dynamicZones[i].ShowBorderOnMap = g_Game.pvez_Config.HELICRASH_ZONES.ShowBorderOnMap;
				dynamicZones[i].Activity_Schedule = g_Game.pvez_Config.HELICRASH_ZONES.Activity_Schedule;
			}
			if (dynamicZones[i].GetIsActive()) {
				activeZones.Insert(dynamicZones[i]);
			}
		}
	}

	void PushUpdateToClients() {
		Param1<array<ref PVEZ_Zone>> zonesData = new Param1<array<ref PVEZ_Zone>>(activeZones);
		g_Game.PVEZ_RPCForAllClients(PVEZ_RPC.UPDATE_ZONES, zonesData);
	}

	// Check if the given position is within some zone area
	int GetZoneIndex(vector playerPos) {
		if (activeZones.Count() == 0)
			return -1;
		
		// Set playerPos Y to 0, so we could make an altitude independent check.
		playerPos[1] = 0;

		PVEZ_Zone zone;
		vector zonePos;
		int result = -1;
		
		for (int i = 0; i < activeZones.Count(); i++) {
			zone = activeZones[i];
			zonePos = zone.GetVectorPos();
			// So, both vectors have the same 0 altitude, now check the horizontal distance between them
			if (vector.DistanceSq(playerPos, zonePos) <= zone.Radius * zone.Radius) {
				if (result == -1)
					result = i;
				else if (zone.Radius > activeZones[result].Radius)
					result = i;
			}
		}
		zone = NULL;
		return result;
	}

	PVEZ_Zone GetByIndex(int index) {
		if (activeZones.Count() > 0 && index >= 0)
			return activeZones.Get(index);
		else
			return NULL;
	}

	PVEZ_Zone AddZone(int type, float coordX, float coordZ, float radius, string name, bool showBorder = true, bool showName = false) {
		if (GetGame().IsClient() && GetGame().IsMultiplayer())
			return NULL;
		
		PVEZ_Zone zone = new PVEZ_Zone(type, coordX, coordZ, name, radius, showBorder, showName);
		if (type == PVEZ_ZONE_TYPE_STATIC)
			staticZones.Insert(zone);
		else
			dynamicZones.Insert(zone);
		Init();
		return zone;
	}

	void RemoveZone(PVEZ_Zone zone) {
		if (GetGame().IsClient() && GetGame().IsMultiplayer())
			return;
		
		if (!zone)
			return;
		
		if (zone.Type == PVEZ_ZONE_TYPE_STATIC)
			staticZones.RemoveItem(zone);
		else
			dynamicZones.RemoveItem(zone);
		Init();
	}

	void LoadFromJson() {
		if (FileExist(PVEZ_ZONES_JSON)) {
			// Read Json file and fullfill this instance with the data stored in the Json
			JsonFileLoader<array<ref PVEZ_Zone>>.JsonLoadFile(PVEZ_ZONES_JSON, staticZones);
		}

		// If Json file has syntax errors then read process fails and <Zones> stays NULL.
		// The same happens if the file doesn't exist.
		if (staticZones == NULL) {
			// Load defaults and rewrite/create the Json file with default values
			Print(PVEZ_ERROR_PREFIX + "'Zones.json' is corrupted or doesn't exist, a new file is going to be created with default zones.");
			LoadDefaultZones();
		}
	}

	void SaveToJson() {
		JsonFileLoader<array<ref PVEZ_Zone>>.JsonSaveFile(PVEZ_ZONES_JSON, staticZones);
	}

	void LoadDefaultZones() {
		staticZones = new array<ref PVEZ_Zone>;

		string worldName = GetGame().GetWorldName();
		worldName.ToLower();

		// Chernarus zones:
		if (worldName == "chernarusplus") {
			Print("PVEZ :: The server is hosting Chernarus map. Creating default zones for Chernarus.");
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 7230,	3038,	"Chernogorsk military base", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 10447,	5969,	"Staroye", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 10227,	4016,	"Topolka Dam", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 10461,	9142,	"Gorka-Dubrovka crossing", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 11963,	12490,	"Krasnostav Airstrip", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 13615,	13753,	"Svetloyarsk-Turovo crossing", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 10947,	13393,	"Ostry Peak", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 9561,	11808,	"Krasnostav-Gvozdno crossing (Noviy Lug)", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 8525,	13983,	"Arsenievo camp (to the North of Severograd)", 250, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 7862,	14688,	"Troitskoye military base", 700, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 7082,	14531,	"Kamensk mining site", 100, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 7088,	12715,	"military camp between Severograd and Ratnoye", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 4948,	12991,	"Zaprudnoye", 200, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 3634,	14929,	"Grozovoy military", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 1576,	14000,	"Tisy military base", 600, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 4042,	11751,	"The tower near North-West Airfield", 400, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 4491,	10358,	"North-West Airfield", 1100, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 4532,	8295,	"VMC military base", 250, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 331,	9367,	"Tri Kresta Gas station", 200, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 1154,	7242,	"over the hill to the West of Myshkino", 200, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 2696,	6755,	"on the road between Pustoshka and Sosnovka", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 2146,	3452,	"Pavlovo military base", 350, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 758,	1876,	"on the coast of Tihiy Zaliv", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 5066,	2393,	"Balota Airstrip", 350, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 2693,	1309,	"Prison Island", 200, true));
		}
		// Livonia zones:
		else if (worldName == "enoch") {
			Print("PVEZ :: The server is hosting Livonia map. Creating default zones for Livonia.");
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 2157,	10967,	"Military Area near Castle", 450, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 3917,	10212,	"Gliniska Airstrip", 600, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 8109,	10889,	"Military Area near Krsnik", 600, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 7901,	9857,	"Krsnik Military Area", 400, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 9836,	8555,	"Borek", 300, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 11344,	10048,	"Hunting Area near Sitnik", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 6434,	8173,	"Nidek Military Area", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 5520,	8773,	"Kona Prison", 200, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 4422,	6697,	"Muratyn Military Area", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 4573,	6397,	"Muratyn Destroyed Area", 400, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 7454,	6164,	"Radunin Quaratine Area", 600, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 9026,	6624,	"Lembork Military Area", 200, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 9903,	3799,	"Military Area near Wrzeszcz", 400, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 8915,	2041,	"Rodzanica Military Area", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 4939,	2146,	"Swarog Military Area", 800, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 7478,	2718,	"Military Area to SE of Nadbor", 600, true));
		}
		// Namalsk zones:
		else if (worldName == "namalsk") {
			Print("PVEZ :: The server is hosting Namalsk map. Creating default zones for Namalsk.");
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 4831,	10841,	"Military BK-M06", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 4406,	10739,	"Military BK-T07", 100, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 5566,	9974,	"Sebjan reservour pass 1", 400, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 6185,	10034,	"Sebjan reservour pass 2", 400, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 6801,	9958,	"Sebjan reservour pass 3", 400, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 3941,	10003,	"Military BK-T08", 200, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 5167,	8962,	"Refugee camp", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 4074,	9234,	"Military BK-T12", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 4982,	6548,	"Athena-2", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 4161,	6646,	"Military BK-L01", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 3405,	6734,	"Military BK-L02", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 5992,	6632,	"Military BK-L16", 150, true));
			staticZones.Insert(new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 1230,	11836,	"Bering outpost", 200, true));
		}
		else {
			Print("PVEZ :: There's no default zones preset for the map hosted on the server (" + worldName + "). You'll need to create your own zones.");
		}
		SaveToJson();
	}
}

class PVEZ_Raycast : Managed {
	static float GetGroundY(float posX, float posZ) {
		vector begPos = Vector(posX, 1000, posZ);
		vector endPos = Vector(posX, -100, posZ);
		vector contactPos;
		vector contactDir;
		int contactComponent;
		if (DayZPhysics.RaycastRV(begPos, endPos, contactPos, contactDir, contactComponent, NULL, NULL, NULL, false, true))
			return contactPos[1];
		else
			return 0;		
	}
}