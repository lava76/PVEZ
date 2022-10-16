/*
	This modded Mi8 wreck class declaration causes conflicts with the CFDZPlugin mod, so it is disabled.
	There is a task opened at https://feedback.bistudio.com/T156813 regarding this issue in February 2021. No solution from Bohemia so far.
*/

/*
class Wreck_Mi8 extends House {

	protected autoptr PVEZ_Zone pvez_Zone;

	void Wreck_Mi8() {
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CreatePVEZZone, 500, false, this);
	}
	
	override void EEDelete(EntityAI parent) {
		// Remove zone if PVEZ zones manager is still alive
		if (g_Game && g_Game.pvez_Zones) {
			g_Game.pvez_Zones.RemoveZone(pvez_Zone);
		}
	}

	void CreatePVEZZone() {
		vector position = GetPosition();
		pvez_Zone = g_Game.pvez_Zones.AddZone(
			PVEZ_ZONE_TYPE_HELICRASH, // 3
			position[0],
			position[2],
			g_Game.pvez_Config.HELICRASH_ZONES.Radius,
			g_Game.pvez_Config.HELICRASH_ZONES.Name,
			g_Game.pvez_Config.HELICRASH_ZONES.ShowBorderOnMap,
			g_Game.pvez_Config.HELICRASH_ZONES.ShowNameOnMap);
	}
}
*/