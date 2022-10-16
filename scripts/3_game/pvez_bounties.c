class PVEZ_BountyItemData : Managed {
	string className;
	string displayName;
	int amount;

	void PVEZ_BountyItemData(string cName, string dName, int a) {
		className = cName;
		displayName = dName;
		amount = a;
	}
}

class PVEZ_Bounties : Managed {

	bool Enabled;
	autoptr array<ref PVEZ_BountyItemData> Items;

	void PVEZ_Bounties() {
		if (GetGame().IsServer() || !GetGame().IsMultiplayer()) {
			LoadFromJson();
			SaveToJson();
		}
	}

	void LoadFromJson() {
		if (FileExist(PVEZ_BOUNTIES_JSON)) {
			JsonFileLoader<PVEZ_Bounties>.JsonLoadFile(PVEZ_BOUNTIES_JSON, this);
		}
	}

	void SaveToJson() {
		JsonFileLoader<PVEZ_Bounties>.JsonSaveFile(PVEZ_BOUNTIES_JSON, this);
	}
}