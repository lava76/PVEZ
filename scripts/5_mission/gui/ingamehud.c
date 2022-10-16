modded class IngameHud {

	autoptr PVEZ_NotificationGUI pvez_NotificationGUI;

	override void Init(Widget hud_panel_widget) {
		super.Init(hud_panel_widget);

		pvez_NotificationGUI = new PVEZ_NotificationGUI;
	}

	override void ShowHud(bool show) {
		super.ShowHud(show);

		pvez_NotificationGUI.ToggleVisibility(show);
	}

	override void ShowHudUI(bool show) {
		super.ShowHudUI(show);

		pvez_NotificationGUI.ToggleVisibility(show);
	}

	override void UpdatePVEZIcon(bool isInPVP, bool isLawbreaker, int zone) {
		super.UpdatePVEZIcon(isInPVP, isLawbreaker, zone);

		pvez_NotificationGUI.UpdatePvpIcon(isInPVP, isLawbreaker, zone);
	}

	override void ShowPVEZNotification(string msg, int duration, bool isCountdown) {
		super.ShowPVEZNotification(msg, duration, isCountdown);

		pvez_NotificationGUI.ShowNotification(msg, duration, isCountdown);
	}
}