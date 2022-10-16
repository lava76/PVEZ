class PVEZ_MapMarkersDrawer : Managed {

	private static autoptr array<ref PVEZ_LawbreakerMarker> lbmarkers;
	private static autoptr array<ref PVEZ_Zone> zones;
	private static autoptr array<vector> zoneCirclePoints = new array<vector>;
	static float mapScale;
	static float screenWidth;
	static float screenHeight;

	// For updating refresh calls:
	static vector screenCenterCoord;	// the world coordinate at current center of the map
	static float screenDiagDistOnMap;	// the distance (meters) from center coordinate to the coordinate in 0,0 corner of the screen

	static void LoadPVEZMarkers(MapWidget mapWidget, bool isUpdating = false) {
		if (!mapWidget)
			return;
		
		// Drawing zones
		zones = g_Game.pvez_Zones.activeZones;
		if (zones && zones.Count() > 0) {
			if (isUpdating) {
				mapWidget.GetScreenSize(screenWidth, screenHeight);
				mapScale = vector.Distance(mapWidget.ScreenToMap("0 0 0"), mapWidget.ScreenToMap(Vector(0, screenHeight, 0)));
				mapScale /= 150;
				screenCenterCoord = mapWidget.ScreenToMap(Vector(screenWidth * 0.5, screenHeight * 0.5, 0));
				screenDiagDistOnMap = vector.Distance(screenCenterCoord, mapWidget.ScreenToMap("0 0 0"));
				DrawZonesRefresh(mapWidget);
			}
			else {
				mapScale = 10;
				DrawZones(mapWidget);
			}
		}

		// Draw lawbreakers markers
		lbmarkers = g_Game.pvez_LawbreakersMarkers.markers;
		if (lbmarkers && lbmarkers.Count() > 0) {
			DrawLawbreakers(mapWidget);
		}
	}

	static void DrawZones(MapWidget mapWidget) {
		vector screenPos;
		zoneCirclePoints = new array<vector>;
		
		// Get color from config
		auto color = ARGB(
			255,											// transparancy
			g_Game.pvez_Config.MAP.Zones_Border_Color.R,	// red
			g_Game.pvez_Config.MAP.Zones_Border_Color.G,	// green
			g_Game.pvez_Config.MAP.Zones_Border_Color.B);	// blue	

		for (int i = 0; i < zones.Count(); i++) {
			if (zones[i].ShowBorderOnMap) {
				// Draw the border circle
				zoneCirclePoints.Clear();
				GetCirclePoints(zones[i], zoneCirclePoints);
				for (int j = 0; j < zoneCirclePoints.Count(); j++) {
					mapWidget.AddUserMark(zoneCirclePoints[j], "", color, "\\DZ\\gear\\navigation\\data\\map_bush_ca.paa");
				}
			}
			// Name
			if (zones[i].ShowNameOnMap) {
				mapWidget.AddUserMark(zones[i].GetVectorPos(), zones[i].Name, color, "");
			}
		}
	}

	static void DrawZonesRefresh(MapWidget mapWidget) {
		vector screenPos;
		zoneCirclePoints = new array<vector>;
		
		// Get color from config
		auto color = ARGB(
			255,											// transparancy
			g_Game.pvez_Config.MAP.Zones_Border_Color.R,	// red
			g_Game.pvez_Config.MAP.Zones_Border_Color.G,	// green
			g_Game.pvez_Config.MAP.Zones_Border_Color.B);	// blue	

		for (int i = 0; i < zones.Count(); i++) {	
			if (zones[i].ShowBorderOnMap) {
				// Draw the border circle
				// Draw border points only if it's not an updating call (vanilla static map with no refresh)
				// or (if updating) then check whether the border is currently on screen or not.
				if (IsBorderVisible(zones[i])) {
					zoneCirclePoints.Clear();
					GetCirclePoints(zones[i], zoneCirclePoints);
					for (int j = 0; j < zoneCirclePoints.Count(); j++) {
						screenPos = mapWidget.MapToScreen(zoneCirclePoints[j]);
						// Don't put this particular border point marker on map if it's out of screen space
						if (screenPos[0] < 0 || screenPos[0] > screenWidth || screenPos[1] < 0 || screenPos[1] > screenHeight)
							continue;
						mapWidget.AddUserMark(zoneCirclePoints[j], "", color, "\\DZ\\gear\\navigation\\data\\map_bush_ca.paa");
					}
				}
			}
			// Name
			if (zones[i].ShowNameOnMap) {
				// Check if the zone center is actually visible on screen
				screenPos = mapWidget.MapToScreen(zones[i].GetVectorPos());
				if (screenPos[0] > 0 && screenPos[0] < screenWidth && screenPos[1] > 0 && screenPos[1] < screenHeight)
					mapWidget.AddUserMark(zones[i].GetVectorPos(), zones[i].Name, color, "");
			}
		}
	}

	static void DrawLawbreakers(MapWidget mapWidget) {
		string label = "";
		if (g_Game.pvez_Config.MAP.Lawbreakers_Markers.Custom_Lawbreaker_Label != string.Empty)
			label = g_Game.pvez_Config.MAP.Lawbreakers_Markers.Custom_Lawbreaker_Label;
		else
			label = Widget.TranslateString("#pvez_lb_map_label");

		for (int i = 0; i < lbmarkers.Count(); i++) {
			mapWidget.AddUserMark(lbmarkers[i].Position, label + " " + lbmarkers[i].Name, ARGB(255, 255, 90, 0), "\\PVEZ\\GUI\\Textures\\skull_ca.paa");
		}
	}

	// Check if any part of the zone borderline is currently visible on screen.
	static bool IsBorderVisible(PVEZ_Zone zone) {
		bool isVisible = false;
		float distFromCenterToZone = vector.Distance(screenCenterCoord, zone.GetVectorPos());
		// The border might be between zone center and screen center or beyond screen center but still within screen space,
		// so we compare Math.Abs(zoneCenter-zoneRadius) distance and screen diagonale distance
		if (Math.AbsFloat(distFromCenterToZone - zone.Radius) <= screenDiagDistOnMap) {
			isVisible = true;
		}
		return isVisible;
	}

	static void GetCirclePoints(PVEZ_Zone zone, out array<vector> result) {
		// How many points we gotta draw. Depends on the circle radius. The bigger the circle, the more points.
		float L = 2 * Math.PI * zone.Radius; // circle lenght (real in-game size in meters)
		// how many points to draw around the zone
		L /= mapScale;
		float slice = 2 * Math.PI / L;
		for (int i = 0; i < L; i++) {
			float angle = slice * i;
			float newX = zone.GetVectorPos()[0] + zone.Radius * Math.Cos(angle);
			float newY = zone.GetVectorPos()[2] + zone.Radius * Math.Sin(angle);
			vector point = Vector(newX, 0, newY);
			if (!IntersectsWithOtherCircles(point, zone))
				result.Insert(point);
		}
	}

	static bool IntersectsWithOtherCircles(vector point, PVEZ_Zone zone) {
		PVEZ_Zone zoneToCheck;
		for (int i = 0; i < zones.Count(); i++) {
			zoneToCheck = zones[i];
			// Ignore the zones with disabled borders and if it's the same zone, also if the zones have the same center and radius
			if (!zoneToCheck.ShowBorderOnMap || zoneToCheck == zone || (zoneToCheck.GetVectorPos() == zone.GetVectorPos() && zoneToCheck.Radius == zone.Radius))
				continue;
			
			if (vector.DistanceSq(zoneToCheck.GetVectorPos(), point) <= zoneToCheck.Radius * zoneToCheck.Radius)
				return true;
		}
		return false;
	}
}