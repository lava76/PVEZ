#ifdef EXPANSIONMODNAVIGATION
modded class ExpansionMapMenu {

	override Widget Init() {
		super.Init();

		// m_MapWidget is the Expansion's base class field, protected MapWidget m_MapWidget.
		PVEZ_MapMarkersDrawer.LoadPVEZMarkers(m_MapWidget);
		return layoutRoot;
	}

	override void UpdateMarkers() {
		super.UpdateMarkers();

		//PVEZ_MapMarkersDrawer.LoadPVEZMarkers(m_MapWidget);
	}
}
#endif
