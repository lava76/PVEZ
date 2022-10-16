modded class MapMenu
{
	override Widget Init() {
		
		layoutRoot = super.Init();

		MapWidget mapWidget = MapWidget.Cast(layoutRoot.FindAnyWidget("Map"));
		
		PVEZ_MapMarkersDrawer.LoadPVEZMarkers(mapWidget);

		if (g_Game.pvez_Config.MAP.Show_Player_Marker == true) {
			string label = "";
			if (g_Game.pvez_Config.MAP.Custom_Player_Marker_Text != string.Empty)
				label = g_Game.pvez_Config.MAP.Custom_Player_Marker_Text;
			else
				label = Widget.TranslateString("#pvez_player_map_label");
			mapWidget.AddUserMark(GetGame().GetPlayer().GetPosition(), label, COLOR_BLUE, "\\DZ\\gear\\navigation\\data\\map_waypoint_ca.paa");
		}
		
		return layoutRoot;
	}
}