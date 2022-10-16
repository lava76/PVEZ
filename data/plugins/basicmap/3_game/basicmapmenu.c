#ifdef BASICMAP
modded class BasicMapMenu {

	override Widget Init() {
		super.Init();

		PVEZ_MapMarkersDrawer.LoadPVEZMarkers(m_Map);
		return layoutRoot;
	}

	override void UpdateMarkers() {
		super.UpdateMarkers();

		if (m_PanelIsOpen && m_Map && BasicMap()) {
			PVEZ_MapMarkersDrawer.LoadPVEZMarkers(m_Map, true);
		}
	}
}
#endif