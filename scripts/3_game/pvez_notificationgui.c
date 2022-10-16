class PVEZ_NotificationGUI : Managed {
	Widget rootWidget;

	Widget iconWidget;
	ImageWidget iconImage;
	
	GridSpacerWidget notiWidget;
	RichTextWidget notiTextWidget;
	int notiTime;

	void PVEZ_NotificationGUI() {
		rootWidget = GetGame().GetWorkspace().CreateWidgets("PVEZ/GUI/Layouts/PvpIcon.layout");

		iconWidget = Widget.Cast(rootWidget.FindAnyWidget("IconWidget"));
		iconImage = ImageWidget.Cast(iconWidget.FindAnyWidget("Image"));
		iconImage.LoadImageFile(0, "PVEZ/GUI/Images/swords_new2.edds"); //image #1: PVP zone
		iconImage.LoadImageFile(1, "PVEZ/GUI/Images/skull_new2.edds"); //image #2: Lawbreaker
		iconImage.LoadImageFile(2, "PVEZ/GUI/Images/shield_new2.edds"); //image #3: Safe zone

		notiWidget = GridSpacerWidget.Cast(rootWidget.FindAnyWidget("NotificationWidget"));
		notiTextWidget = RichTextWidget.Cast(notiWidget.FindAnyWidget("NotificationTitle"));
		
		rootWidget.Show(true);
		iconWidget.Show(false);
		notiWidget.Show(false);
	}

	void UpdatePvpIcon(bool isInPVP, bool isLawbreaker, int zone) {
		if (isLawbreaker) {
			iconImage.SetImage(1);
			iconWidget.Show(true);
		}
		else if (zone >= 0) {
			if (isInPVP) {
				iconImage.SetImage(0);
				iconWidget.Show(true);
			}
			else {
				iconImage.SetImage(2);
				iconWidget.Show(true);
			}
		}
		else {
			iconWidget.Show(false);
		}
	}

	void ToggleVisibility(bool value) {
		rootWidget.Show(value);
	}

	void ShowNotification(string text, int duration, bool countdown) {
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.UpdateNotification);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.HideNotification);

		notiTextWidget.SetText(text);
		notiWidget.Show(true);
		
		notiTime = duration;

		if (countdown) {
			// Update immediately to set correct text (with seconds counter).
			UpdateNotification(text);
			// and register delayed repeatable update.
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.UpdateNotification, 1000, true, text);
		}
		else {
			// Register the delayed call of HideNotification(), it will be called duration*1000 miliseconds later.
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.HideNotification, duration * 1000, false);
		}
	}

	void UpdateNotification(string text) {
		notiTextWidget.SetText(text + "\n" + notiTime.ToString());
		notiWidget.Show(true);
		
		if (notiTime <= 0) {
			notiWidget.Show(false);
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(this.UpdateNotification);
		}
		notiTime -= 1;
	}

	void HideNotification() {
		notiWidget.Show(false);
	}
}