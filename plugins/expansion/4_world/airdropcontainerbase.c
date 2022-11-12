#ifdef EXPANSIONMODMISSIONS
modded class ExpansionAirdropContainerBase {

	protected autoptr PVEZ_Zone pvez_Zone;
	protected vector pvez_lastPosition;

	//////////////////////////////////////////////////////////////////////////////////////////////
	///--------------------------------Expansion overrides:------------------------------------///
	void ExpansionAirdropContainerBase() {
		// Create a zone 2 seconds after the container spawn (when it's ready to return correct position value)
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CreatePVEZZone, 2000, false);

		// Start the repeatable position check to adjust the zone position on possible wind drift
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CheckContainerPosition, 5000, true);
	}

	void ~ExpansionAirdropContainerBase() {
		// Remove zone if PVEZ zones manager is still alive
		if (g_Game && g_Game.pvez_Zones) {
			g_Game.pvez_Zones.RemoveZone(pvez_Zone);
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
///-------------------------------------PVEZ methods:--------------------------------------------//
	void CreatePVEZZone() {
		pvez_lastPosition = GetPosition();
		pvez_lastPosition[1] = 0; // we need to store X and Z only.
		pvez_Zone = g_Game.pvez_Zones.AddZone(
			PVEZ_ZONE_TYPE_AIRDROP, // 1
			pvez_lastPosition[0],
			pvez_lastPosition[2],
			g_Game.pvez_Config.AIRDROP_ZONES.Radius,
			g_Game.pvez_Config.AIRDROP_ZONES.Name,
			g_Game.pvez_Config.AIRDROP_ZONES.ShowBorderOnMap,
			g_Game.pvez_Config.AIRDROP_ZONES.ShowNameOnMap);
	}

	void CheckContainerPosition() {
		// Remove the updater if the container is landed and the loot has been spawned.
		if (m_HasLanded || !pvez_Zone) {
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(CheckContainerPosition);
			return;
		}

		vector currentPosition = GetPosition();
		currentPosition[1] = 0; // we need X and Z only.
		if (vector.Distance(pvez_lastPosition, currentPosition) > 10) {
			pvez_lastPosition = currentPosition;
			pvez_Zone.X = currentPosition[0];
			pvez_Zone.Z = currentPosition[2];
		}
	}
}
#endif
