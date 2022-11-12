#ifdef VPPADMINTOOLS
modded class MenuTeleportManager extends AdminHudSubMenu
{
    override void UpdateMapEx()
    {
    	super.UpdateMapEx();
	
        if (g_Game.pvez_Config.MAP.Show_Zones_In_VPPA_Teleport_Manager == true)
        {
		    PVEZ_MapMarkersDrawer.LoadPVEZMarkers(m_Map);
        }
    }
}
#endif
