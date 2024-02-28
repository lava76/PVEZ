modded class Wreck_Mi8_Crashed {

	protected autoptr PVEZ_Zone pvez_Zone;

	void Wreck_Mi8_Crashed() {
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