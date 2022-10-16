//////////////////////////////////////////////////////////////////////////
//---------------------- Config management class -----------------------//
class PVEZ_Config : Managed {
	
	//--------------------PVEZ_Config.JSON sections:--------------------//
	autoptr PVEZ_GeneralSettings GENERAL;
	autoptr PVEZ_DamageSettings DAMAGE;
	autoptr PVEZ_LawbreakersSettings LAWBREAKERS_SYSTEM;
	autoptr PVEZ_MapSettings MAP;
	autoptr PVEZ_AirdropZonesSettings AIRDROP_ZONES;
	autoptr PVEZ_TerritoryFlagZonesSettings TERRITORYFLAG_ZONES;
	autoptr PVEZ_HeliCrashZonesSettings HELICRASH_ZONES;
	int version = 113;
	//__________________________________________________________________//

	void PVEZ_Config() {
		
		if (!FileExist(PVEZ_CONFIG_FOLDER) && FileExist(PVEZ_CONFIG_FOLDER_OLD))
			RenameOldConfigFiles();

		if (GetGame().IsServer() || !GetGame().IsMultiplayer()) {
			Init();
			LoadFromJson();
		}
	}

	void Init() {
		string profilePath;
		if (!GetCLIParam("profiles", profilePath)) {
			Print("PVEZ :: The server is started with no -profile=<PATH>. Will use config files in default DayZ path: C:/Users/YOUR_USER_NAME/AppData/Local/DayZ/");
			PVEZ_CONFIG_FOLDER = "$saves:\\PVEZ";
		}
		else {
			Print("PVEZ :: Will use config files in " + profilePath);
		}

		// Create mod folder in the server profile folder (if it doesn't exist yet).
		MakeDirectory(PVEZ_CONFIG_FOLDER);
		// Create admins.txt if doesn't exist.
		if (!FileExist(PVEZ_ADMINS_TXT)) {
			FileHandle file = OpenFile(PVEZ_ADMINS_TXT, FileMode.WRITE);
			if (file != 0) {
				FPrintln(file, "# Write admins UIDs (44 digits) below. Each one at a new line.");
				CloseFile(file);
			}
		}

		GENERAL = new PVEZ_GeneralSettings;
		DAMAGE = new PVEZ_DamageSettings;
		LAWBREAKERS_SYSTEM = new PVEZ_LawbreakersSettings;
		MAP = new PVEZ_MapSettings;
		AIRDROP_ZONES = new PVEZ_AirdropZonesSettings;
		TERRITORYFLAG_ZONES = new PVEZ_TerritoryFlagZonesSettings;
		HELICRASH_ZONES = new PVEZ_HeliCrashZonesSettings;
	}

	void LoadFromJson() {
		if (FileExist(PVEZ_CONFIG_JSON)) {
			// Read Json file and fullfill this instance with the data stored in the Json
			JsonFileLoader<PVEZ_Config>.JsonLoadFile(PVEZ_CONFIG_JSON, this);
		}

		if (version < 1) {
			version = 1;
			
			AIRDROP_ZONES.Name = "Airdrop";
			AIRDROP_ZONES.Radius = 150;
			AIRDROP_ZONES.ShowNameOnMap = true;
			AIRDROP_ZONES.ShowBorderOnMap = true;
			AIRDROP_ZONES.Activity_Schedule = new PVEZ_Zone_Schedule;

			TERRITORYFLAG_ZONES.Name = "Base territory";
			TERRITORYFLAG_ZONES.Radius = 150;
			TERRITORYFLAG_ZONES.ShowNameOnMap = false;
			TERRITORYFLAG_ZONES.ShowBorderOnMap = false;
			TERRITORYFLAG_ZONES.OnlyWhenFlagIsRaised = true;
			TERRITORYFLAG_ZONES.Activity_Schedule = new PVEZ_Zone_Schedule;
			// Disable territory flag zones by default:
			TERRITORYFLAG_ZONES.Activity_Schedule.Days = "";
			TERRITORYFLAG_ZONES.Activity_Schedule.StartHour = 0;
			TERRITORYFLAG_ZONES.Activity_Schedule.EndHour = 0;
		}

		if (version < 3) {
			version = 113;
			
			HELICRASH_ZONES.Name = "Helicopter crash site";
			HELICRASH_ZONES.Radius = 150;
			HELICRASH_ZONES.ShowNameOnMap = false;
			HELICRASH_ZONES.ShowBorderOnMap = false;
			HELICRASH_ZONES.Activity_Schedule = new PVEZ_Zone_Schedule;
		}

		// Update the Json to add new properties to the config file.
		// The existing Json contains old properties only, while this instance could be initialized with new properties,
		// and then during the file read process mutual properties get values from the file, but the file doesn't know about new properties added through the code here.
		// In the save file process, new properties will be added to the file along with mutual ones.
		SaveToJson();
	}

	void SaveToJson() {
		JsonFileLoader<PVEZ_Config>.JsonSaveFile(PVEZ_CONFIG_JSON, this);
	}

	void RenameOldConfigFiles() {
		
		MakeDirectory(PVEZ_CONFIG_FOLDER);

		TStringArray list = new TStringArray;
		string	file_name;
		int 	file_attr;
		int		flags;
		
		string path_find_pattern = PVEZ_CONFIG_FOLDER_OLD +"\\*";
		FindFileHandle file_handler = FindFile(path_find_pattern, file_name, file_attr, flags);
		
		bool found = true;
		while ( found ) {
			list.Insert(file_name);
			found = FindNextFile(file_handler, file_name, file_attr);
		}

		string adminsOldName = PVEZ_ADMINS_TXT_OLD;
		adminsOldName.Replace(PVEZ_CONFIG_FOLDER_OLD + "\\", "");
		string configOldName = PVEZ_CONFIG_JSON_OLD;
		configOldName.Replace(PVEZ_CONFIG_FOLDER_OLD + "\\", "");
		string lbsOldName = PVEZ_LAWBREAKERS_ROSTER_JSON_OLD;
		lbsOldName.Replace(PVEZ_CONFIG_FOLDER_OLD + "\\", "");
		string zonesOldName = PVEZ_ZONES_JSON_OLD;
		zonesOldName.Replace(PVEZ_CONFIG_FOLDER_OLD + "\\", "");

		for (int i = 0; i < list.Count(); i++) {	
			switch (list[i]) {
				case adminsOldName:
					CopyFile(PVEZ_CONFIG_FOLDER_OLD + "\\" + list[i], PVEZ_ADMINS_TXT);
					break;
				case configOldName:
					CopyFile(PVEZ_CONFIG_FOLDER_OLD + "\\" + list[i], PVEZ_CONFIG_JSON);
					break;
				case lbsOldName:
					CopyFile(PVEZ_CONFIG_FOLDER_OLD + "\\" + list[i], PVEZ_LAWBREAKERS_ROSTER_JSON);
					break;
				case zonesOldName:
					CopyFile(PVEZ_CONFIG_FOLDER_OLD + "\\" + list[i], PVEZ_ZONES_JSON);
					break;
			}
			DeleteFile(PVEZ_CONFIG_FOLDER_OLD + "\\" + list[i]);
		}
		DeleteFile(PVEZ_CONFIG_FOLDER_OLD);
	}
}