modded class DayZPlayerImplement {

    override void HandleView() {
        super.HandleView();

		HumanInputController hic = GetInputController();
		bool camChange = hic.CameraViewChanged();

		if (camChange && g_Game.pvez_Config.GENERAL.Force1stPersonInPVP) {
			PlayerBase p = PlayerBase.Cast(this);
			if (p && p.pvez_PlayerStatus) {
				if (p.pvez_PlayerStatus.GetIsInPVP()) {
					m_Camera3rdPerson = false;
				}
			}
		}
    }
}