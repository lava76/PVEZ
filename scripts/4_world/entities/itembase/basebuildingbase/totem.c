modded class TerritoryFlag {
	protected autoptr PVEZ_Zone pvez_Zone;
	protected bool m_RefresherActivePrevState;

	void TerritoryFlag() {
		if (GetGame().IsServer() && !g_Game.pvez_Config.TERRITORYFLAG_ZONES.OnlyWhenFlagIsRaised)
			// Create the zone at every flag pole instance
			CreatePVEZZone();
	}
	
	void ~TerritoryFlag() {
		// Remove zone if PVEZ zones manager is still alive
		if (g_Game && g_Game.pvez_Zones) {
			g_Game.pvez_Zones.RemoveZone(pvez_Zone);
		}
	}

	override void SetRefresherActive(bool state) {
		m_RefresherActivePrevState = m_RefresherActive;
		
		super.SetRefresherActive(state);

		if (g_Game.pvez_Config.TERRITORYFLAG_ZONES.OnlyWhenFlagIsRaised) {
			// Create or delete the zone every time the flag state is changed.
			if (m_RefresherActivePrevState != m_RefresherActive) {
				m_RefresherActivePrevState = m_RefresherActive;
				if (m_RefresherActive)
					CreatePVEZZone();
				else
					g_Game.pvez_Zones.RemoveZone(pvez_Zone);
			}
		}
	}

	void CreatePVEZZone() {
		vector position = GetPosition();
		pvez_Zone = g_Game.pvez_Zones.AddZone(
			PVEZ_ZONE_TYPE_TERRITORYFLAG,
			position[0],
			position[2],
			g_Game.pvez_Config.TERRITORYFLAG_ZONES.Radius,
			g_Game.pvez_Config.TERRITORYFLAG_ZONES.Name,
			g_Game.pvez_Config.TERRITORYFLAG_ZONES.ShowBorderOnMap,
			g_Game.pvez_Config.TERRITORYFLAG_ZONES.ShowNameOnMap);
	}
}