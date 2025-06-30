#ifdef Carim
modded class CarimMenuMap {
    override void LoadMapMarkers() {
        super.LoadMapMarkers();
        PVEZ_MapMarkersDrawer.LoadPVEZMarkers(m_MapWidgetInstance);
    }
}
#endif
