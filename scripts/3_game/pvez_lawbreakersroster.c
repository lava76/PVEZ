class PVEZ_Lawbreaker : Managed {
	string Id;
	autoptr array<string> Recent_Character_Names;
	int Murder_Count;
	autoptr PVEZ_Date Latest_Murder_Time;
	bool Is_Currently_Outlaw;

	void PVEZ_Lawbreaker(string id, string name, int crimesCount, bool outlaw = true, PVEZ_Date lastMurderTime = NULL) {
		Id = id;
		Recent_Character_Names = new array<string>;
		Recent_Character_Names.Insert(name);
		Murder_Count = crimesCount;
		Latest_Murder_Time = lastMurderTime;
		Is_Currently_Outlaw = outlaw;
	}
}

class PVEZ_LawbreakersRoster : Managed {

	autoptr array<ref PVEZ_Lawbreaker> lbDataBase;
	autoptr map<string, EntityAI> lbEntities;

	// This flag is monitored by <MissionServer> to update lawbreaker statuses on players when this roster is getting re-initialized.
	bool updated;

	void PVEZ_LawbreakersRoster(PVEZ_Config c) {
		lbDataBase = new array<ref PVEZ_Lawbreaker>;
		lbEntities = new map<string, EntityAI>;
		if (GetGame().IsServer() || !GetGame().IsMultiplayer()) {
			LoadFromJson();
			RemoveOutdated(c);
			SaveToJson();
			updated = true;
		}
	}

	void SaveToJson() {
		JsonFileLoader<array<ref PVEZ_Lawbreaker>>.JsonSaveFile(PVEZ_LAWBREAKERS_ROSTER_JSON, lbDataBase);
	}
	
	void LoadFromJson() {
		// Read Json file and fullfill the <roster> array with the data stored in the Json
		if (FileExist(PVEZ_LAWBREAKERS_ROSTER_JSON)) {
			JsonFileLoader<array<ref PVEZ_Lawbreaker>>.JsonLoadFile(PVEZ_LAWBREAKERS_ROSTER_JSON, lbDataBase);
		}
	}

	void RemoveOutdated(PVEZ_Config config) {
		if (!config.LAWBREAKERS_SYSTEM.Auto_Clear_Lawbreakers_Data || lbDataBase.Count() == 0)
			return;
		
		// Call it again after 60 secs to update LBs on server.
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.RemoveOutdated, 60000, false, g_Game.pvez_Config);
		
		PVEZ_Date lastMurderTime;
		for (int i = 0; i < lbDataBase.Count(); i++) {
			lastMurderTime = lbDataBase[i].Latest_Murder_Time;
			
			if (!lastMurderTime) {
				RemoveEntity(lbDataBase[i]);
				lbDataBase.Remove(i);
				continue;
			}	
			// Check the player's last murder date. If it was long ago, forgive the player.
			int count;
			switch (config.LAWBREAKERS_SYSTEM.Autoclear_Period_Mode) {
				case 0: // Mode 0 counts minutes
					count = PVEZ_StaticFunctions.CountMinutesBetweenDates(lastMurderTime, PVEZ_Date.Now());
					break;
				case 1: // Mode 1 counts hours
					count = PVEZ_StaticFunctions.CountHoursBetweenDates(lastMurderTime, PVEZ_Date.Now());
					break;
				case 2: // Mode 2 counts days
					count = PVEZ_StaticFunctions.CountDaysBetweenDates(lastMurderTime, PVEZ_Date.Now());
					break;
			}
			if (count >= config.LAWBREAKERS_SYSTEM.Autoclear_Period_Amount) {
				Print("PVEZ :: Removing outdated lawbreaker data (ID: " + lbDataBase[i].Id + ", Name: " + lbDataBase[i].Recent_Character_Names[0] + ", Latest murder time: " + PVEZ_StaticFunctions.GetFormattedDateTimeFromPVEZ_Date(lastMurderTime));
				RemoveEntity(lbDataBase[i]);
				lbDataBase.Remove(i);
			}
		}
		updated = true;
	}

	PVEZ_Lawbreaker GetById(string idToFind) {
		for (int i = 0; i < lbDataBase.Count(); i++) {
			string id = lbDataBase[i].Id;
			if (idToFind == id) {
				return lbDataBase[i];
			}
		}
		return NULL;
	}

	EntityAI GetEntity(string idToFind) {
		for (int i = 0; i < lbEntities.Count(); i++) {
			string id = lbEntities.GetKey(i);
			if (idToFind == id) {
				return lbEntities.Get(id);
			}
		}
		return NULL;
	}

	void RemoveEntity(PVEZ_Lawbreaker lb) {
		for (int i = 0; i < lbEntities.Count(); i++) {
			string id = lbEntities.GetKey(i);
			if (lb.Id == id) {
				lbEntities.Remove(id);
			}
		}
	}

	// This can be called by other scripts on lawful kills
	void Update(EntityAI lbEntity, bool isLawbreaker, PVEZ_Date time = NULL) {
		// Initialize the roster first
		if (!lbDataBase)
			LoadFromJson();
		
		string murdererId = PVEZ_StaticFunctions.GetEntityId(lbEntity);
		string murdererName = PVEZ_StaticFunctions.GetEntityName(lbEntity);

		// Looking for the murderer in the roster
		PVEZ_Lawbreaker lb;
		bool lawbreakerFound;
		for (int i = 0; i < lbDataBase.Count(); i++) {
			lb = lbDataBase[i];
			if (lb.Id == murdererId) {
				lawbreakerFound = true;

				// Checking if the murderer's current character name is already stored in roster
				bool nameAlreadyKnown;
				for (int j = 0; j < lb.Recent_Character_Names.Count(); j++) {
					if (lb.Recent_Character_Names[j] == murdererName) {
						nameAlreadyKnown = true;
						break;
					}
				}
				// if the name is not in the roster yet
				if (!nameAlreadyKnown) {
					// Putting new name in the murderer's data (only 3 the most recent names are stored)
					if (lb.Recent_Character_Names.Count() < 3)
						lb.Recent_Character_Names.Insert(murdererName);
					else {
						lb.Recent_Character_Names.Remove(0);
						lb.Recent_Character_Names.Insert(murdererName);
					}
				}
				if (time) {
					lb.Murder_Count++;
					lb.Latest_Murder_Time = time;
				}
				lb.Is_Currently_Outlaw = isLawbreaker;
				if (lbEntities.Get(lb.Id) == NULL)
					lbEntities.Insert(murdererId, lbEntity);
				break;
			}
		}
		if (!lawbreakerFound && isLawbreaker) {
			// Adding new lawbreaker data to the roster
			lbDataBase.Insert(new PVEZ_Lawbreaker(murdererId, murdererName, 1, true, time));
			lbEntities.Insert(murdererId, lbEntity);
		}
	}

	bool Check(EntityAI entity) {
		// Initialize the roster first
		if (!lbDataBase)
			LoadFromJson();

		if (lbDataBase.Count() == 0)
			return false;
		
		string id = PVEZ_StaticFunctions.GetEntityId(entity);

		foreach (PVEZ_Lawbreaker lb : lbDataBase) {
			if (lb.Id == id) {
				if (lbEntities.Get(id) == NULL)
					lbEntities.Insert(id, entity);
				return lb.Is_Currently_Outlaw;
			}
		}
		return false;
	}
}