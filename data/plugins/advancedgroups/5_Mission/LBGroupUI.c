modded class LBGroupUI {

	override void AddCustomMarkers() {
		super.AddCustomMarkers();
		if (!mapMarkerManager)
			return;
		array<ref PVEZ_Zone> zones = g_Game.pvez_Zones.activeZones;
		int color = ARGB(255,
			g_Game.pvez_Config.MAP.Zones_Border_Color.R,	// red
			g_Game.pvez_Config.MAP.Zones_Border_Color.G,	// green
			g_Game.pvez_Config.MAP.Zones_Border_Color.B);	// blue	
		foreach (PVEZ_Zone zone : zones) {
			if (zone.ShowBorderOnMap) {
				mapMarkerManager.AddCircleNonScaling(zone.GetVectorPos(), zone.Radius, color, 5488);
			}
		}
	}
	
	override void AddCustomMarkersOnMapOpen() {
		super.AddCustomMarkersOnMapOpen();
		if (!mapWidget)
			return;
		array<ref PVEZ_LawbreakerMarker> lbmarkers = g_Game.pvez_LawbreakersMarkers.markers;
		array<ref PVEZ_Zone> zones = g_Game.pvez_Zones.activeZones;
		int color = ARGB(255,
			g_Game.pvez_Config.MAP.Zones_Border_Color.R,	// red
			g_Game.pvez_Config.MAP.Zones_Border_Color.G,	// green
			g_Game.pvez_Config.MAP.Zones_Border_Color.B);	// blue	
		foreach (PVEZ_Zone zone : zones) {
			vector pos = Vector(zone.X, 0, zone.Z);
			if (zone.ShowNameOnMap) {
				mapWidget.AddUserMark(zone.GetVectorPos(), zone.Name, color, "");
			}
		}
		PVEZ_MapMarkersDrawer.DrawLawbreakers(mapWidget);
	}
}