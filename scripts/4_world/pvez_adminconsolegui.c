class PVEZ_AdminConsoleGUI extends UIScriptedMenu {

	protected autoptr PVEZ_Zone selectedZone;
	protected int dynamicZonesTypeSelected;
	protected autoptr PVEZ_Lawbreaker selectedLawbreaker;
	protected autoptr array<Man> playersOnServer;

	protected bool isTypingText;

	// header buttons
	protected ButtonWidget btnGeneral;
	protected ButtonWidget btnZones;
	protected ButtonWidget btnLawbreakers;
	protected ButtonWidget btnBounties;

	protected Widget descriptionFrame;
	protected RichTextWidget descriptionTitle;

	/// SETTINGS page
	protected Widget SettingsRootPanel;
	// general settings
	protected XComboBoxWidget modeValue;
	protected Widget modePanel;
	protected EditBoxWidget freqValue;
	protected Widget freqPanel;
	protected CheckBoxWidget showNotifsValue;
	protected Widget showNotifsPanel;
	protected CheckBoxWidget showUIValue;
	protected Widget showUIPanel;
	protected CheckBoxWidget showNameInNotifsValue;
	protected Widget showNameInNotifsPanel;
	protected EditBoxWidget exitDelayValue;
	protected Widget exitDelayPanel;
	protected CheckBoxWidget force1stPersonValue;
	protected Widget force1stPersonPanel;
	protected EditBoxWidget customMsgEnterValue;
	protected Widget customMsgEnterPanel;
	protected EditBoxWidget customMsgExitValue;
	protected Widget customMsgExitPanel;
	protected EditBoxWidget customMsgExitCountdownValue;
	protected Widget customMsgExitCountdownPanel;
	// damage settings
	protected CheckBoxWidget healTargetValue;
	protected Widget healTargetPanel;
	protected CheckBoxWidget healCargoValue;
	protected Widget healCargoPanel;
	protected CheckBoxWidget allowDmgBeyondZoneValue;
	protected Widget allowDmgBeyondZonePanel;
	protected CheckBoxWidget reflectDmgWeaponsValue;
	protected CheckBoxWidget reflectDmgExplosivesValue;
	protected CheckBoxWidget reflectDmgVehiclesValue;
	protected CheckBoxWidget reflectDmgFistsValue;
	protected Widget reflectDmgPanel;
	// lawbreakers settings
	protected CheckBoxWidget lbKillTypeWeaponsValue;
	protected CheckBoxWidget lbKillTypeExplosivesValue;
	protected CheckBoxWidget lbKillTypeVehiclesValue;
	protected CheckBoxWidget lbKillTypeFistsValue;
	protected Widget lbKillTypesPanel;
	protected CheckBoxWidget lbServerMessageValue;
	protected Widget lbServerMessagePanel;
	protected CheckBoxWidget lbAttackAnywhereValue;
	protected Widget lbAttackAnywherePanel;
	protected CheckBoxWidget lbPardonOnAnyDeathValue;
	protected Widget lbPardonOnAnyDeathPanel;
	protected CheckBoxWidget lbAutoclearValue;
	protected Widget lbAutoclearPanel;
	protected EditBoxWidget lbAutoclearPeriodValue;
	protected XComboBoxWidget lbAutoclearPeriodMode;
	protected Widget lbAutoclearPeriodPanel;
	protected EditBoxWidget customMsgLbServerWideValue;
	protected Widget customMsgLbServerWidePanel;
	protected EditBoxWidget customMsgLbPersonalValue;
	protected Widget customMsgLbPersonalPanel;

	// Bounty system
	protected Widget BountySettingsRootPanel;
	protected CheckBoxWidget bountyCheckboxValue;
	protected Widget bountyCheckboxPanel;
	protected EditBoxWidget bountySearchValue;
	protected TextListboxWidget bountyAllItemsList;
	protected ButtonWidget bountyAddButton;
	protected TextListboxWidget bountySelectedItemsList;
	protected ButtonWidget bountyRemoveButton;
	protected ButtonWidget bountyApplyButton;

	// map settings
	protected Widget mapPlayerMarkerPanel;
	protected CheckBoxWidget mapPlayerMarkerValue;
	protected Widget mapPlayerMarkerCustomTextPanel;
	protected EditBoxWidget mapPlayerMarkerCustomTextValue;
	protected Widget mapZoneBorderColorPanel;
	protected SliderWidget mapZoneBorderColorValueR;
	protected SliderWidget mapZoneBorderColorValueG;
	protected SliderWidget mapZoneBorderColorValueB;
	protected ImageWidget mapZoneBorderColorImage;
	protected CheckBoxWidget mapShowLbMarkersValue;
	protected Widget mapShowLbMarkersPanel;
	protected CheckBoxWidget mapShowLbNameValue;
	protected Widget mapShowLbNamePanel;
	protected EditBoxWidget mapLbCustomTextValue;
	protected Widget mapLbCustomTextPanel;
	protected EditBoxWidget mapLbMarkersUpdateFreqValue;
	protected Widget mapLbMarkersUpdateFreqPanel;
	protected CheckBoxWidget mapLbMarkersUseApproximateValue;
	protected Widget mapLbMarkersUseApproximatePanel;
	protected EditBoxWidget mapLbMarkersApproximateOffsetValue;
	protected Widget mapLbMarkersApproximateOffsetPanel;
	protected CheckBoxWidget mapLbCanSeeTheirOwnMarkerValue;
	protected Widget mapLbCanSeeTheirOwnMarkerPanel;
	// apply button
	protected ButtonWidget btnSettingsApply;

	/// ZONES page
	protected Widget ZonesRootPanel;
	protected TextListboxWidget zonesList;
	protected ButtonWidget btnCreateNewZone;
	// selected zone side panel
	protected Widget ZoneSettingsPanel;
	protected EditBoxWidget zoneNameValue;
	protected EditBoxWidget zoneCoordXValue;
	protected EditBoxWidget zoneCoordZValue;
	protected ButtonWidget btnZoneSetCurrentPosition;
	protected EditBoxWidget zoneRadiusValue;
	protected CheckBoxWidget zoneShowBorderValue;
	protected CheckBoxWidget zoneShowNameValue;
	protected EditBoxWidget zoneDaysValue;
	protected CheckBoxWidget zoneDaysModeValue;
	protected EditBoxWidget zoneHourStartValue;
	protected EditBoxWidget zoneHourEndValue;
	protected TextWidget serverTimeValue;
	// selected zone actions
	protected ButtonWidget btnZoneApply;
	protected ButtonWidget btnZoneDelete;

	// dynamic zones setup panel
	protected ButtonWidget btnAirdropZonesSetup;
	protected ButtonWidget btnFlagZonesSetup;
	protected ButtonWidget btnHeliCrashZonesSetup;

	protected Widget DynamicZoneSettingsPanel;
	protected TextWidget dynamicZoneTitle;
	protected EditBoxWidget dynamicZoneNameValue;
	protected EditBoxWidget dynamicZoneCoordXValue;
	protected EditBoxWidget dynamicZoneCoordZValue;
	protected EditBoxWidget dynamicZoneRadiusValue;
	protected CheckBoxWidget dynamicZoneShowBorderValue;
	protected CheckBoxWidget dynamicZoneShowNameValue;
	protected EditBoxWidget dynamicZoneDaysValue;
	protected CheckBoxWidget dynamicZoneDaysModeValue;
	protected EditBoxWidget dynamicZoneHourStartValue;
	protected EditBoxWidget dynamicZoneHourEndValue;
	protected Widget dynamicZoneFlagRaisedPanel;
	protected CheckBoxWidget dynamicZoneFlagRaisedValue;
	protected ButtonWidget btnDynamicZoneApply;
	protected ButtonWidget btnDynamicZoneDelete;

	/// LAWBREAKERS page
	protected Widget LawbreakersRootPanel;
	protected TextListboxWidget lbRosterList;
	protected TextListboxWidget lbPlayersList;
	// selected LB side panel
	protected Widget lbDataPanel;
	protected RichTextWidget lbUIDValue;
	protected ButtonWidget btnUIDCopy;
	protected RichTextWidget lbNamesValue;
	protected TextWidget lbCountValue;
	protected TextWidget lbLatestMurderTimeValue;
	protected CheckBoxWidget lbStatusValue;
	protected PlayerPreviewWidget lbPreview;
	protected ButtonWidget btnLbApply;
	protected ButtonWidget btnLbDelete;
	protected ImageWidget btnLbDeleteBackground;

	bool IsTypingText() {
		return isTypingText;
	}

	override Widget Init() {
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("PVEZ\\data\\gui\\layouts\\AdminConsole.layout");

		/// SETTINGS, ZONES, LAWBREAKERS header buttons
		btnGeneral = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnGeneral"));
		btnZones = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnZones"));
		btnLawbreakers = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnLawbreakers"));
		btnBounties = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnBounties"));

		descriptionFrame = Widget.Cast(layoutRoot.FindAnyWidget("descriptionFrame"));
		descriptionTitle = RichTextWidget.Cast(layoutRoot.FindAnyWidget("descriptionTitle"));

		/// SETTINGS page
		SettingsRootPanel = Widget.Cast(layoutRoot.FindAnyWidget("SettingsRootPanel"));
		SettingsRootPanel.Show(true);
		// general
		modeValue = XComboBoxWidget.Cast(layoutRoot.FindAnyWidget("modeValue"));
		modePanel = Widget.Cast(layoutRoot.FindAnyWidget("modePanel"));
		freqValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("freqValue"));
		freqPanel = Widget.Cast(layoutRoot.FindAnyWidget("freqPanel"));
		showNotifsValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("showNotifsValue"));
		showNotifsPanel = Widget.Cast(layoutRoot.FindAnyWidget("showNotifsPanel"));
		showUIValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("showUIValue"));
		showUIPanel = Widget.Cast(layoutRoot.FindAnyWidget("showUIPanel"));
		showNameInNotifsValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("showNameInNotifsValue"));
		showNameInNotifsPanel = Widget.Cast(layoutRoot.FindAnyWidget("showNameInNotifsPanel"));
		exitDelayValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("exitDelayValue"));
		exitDelayPanel = Widget.Cast(layoutRoot.FindAnyWidget("exitDelayPanel"));
		force1stPersonValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("force1stPersonValue"));
		force1stPersonPanel = Widget.Cast(layoutRoot.FindAnyWidget("force1stPersonPanel"));
		customMsgEnterValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("customMsgEnterValue"));
		customMsgEnterPanel = Widget.Cast(layoutRoot.FindAnyWidget("customMsgEnterPanel"));
		customMsgExitValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("customMsgExitValue"));
		customMsgExitPanel = Widget.Cast(layoutRoot.FindAnyWidget("customMsgExitPanel"));
		customMsgExitCountdownValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("customMsgExitCountdownValue"));
		customMsgExitCountdownPanel = Widget.Cast(layoutRoot.FindAnyWidget("customMsgExitCountdownPanel"));
		// damage
		healTargetValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("healTargetValue"));
		healTargetPanel = Widget.Cast(layoutRoot.FindAnyWidget("healTargetPanel"));
		healCargoValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("healCargoValue"));
		healCargoPanel = Widget.Cast(layoutRoot.FindAnyWidget("healCargoPanel"));
		allowDmgBeyondZoneValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("allowDmgBeyondZoneValue"));
		allowDmgBeyondZonePanel = Widget.Cast(layoutRoot.FindAnyWidget("allowDmgBeyondZonePanel"));
		reflectDmgWeaponsValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("reflectDmgWeaponsValue"));
		reflectDmgExplosivesValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("reflectDmgExplosivesValue"));
		reflectDmgVehiclesValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("reflectDmgVehiclesValue"));
		reflectDmgFistsValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("reflectDmgFistsValue"));
		reflectDmgPanel = Widget.Cast(layoutRoot.FindAnyWidget("reflectDmgPanel"));
		// lawbreakers
		lbKillTypeWeaponsValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("lbKillTypeWeaponsValue"));
		lbKillTypeExplosivesValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("lbKillTypeExplosivesValue"));
		lbKillTypeVehiclesValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("lbKillTypeVehiclesValue"));
		lbKillTypeFistsValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("lbKillTypeFistsValue"));
		lbKillTypesPanel = Widget.Cast(layoutRoot.FindAnyWidget("lbKillTypesPanel"));
		lbServerMessageValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("lbServerMessageValue"));
		lbServerMessagePanel = Widget.Cast(layoutRoot.FindAnyWidget("lbServerMessagePanel"));
		lbAttackAnywhereValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("lbAttackAnywhereValue"));
		lbAttackAnywherePanel = Widget.Cast(layoutRoot.FindAnyWidget("lbAttackAnywherePanel"));
		lbPardonOnAnyDeathValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("lbPardonOnAnyDeathValue"));
		lbPardonOnAnyDeathPanel = Widget.Cast(layoutRoot.FindAnyWidget("lbPardonOnAnyDeathPanel"));
		lbAutoclearValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("lbAutoclearValue"));
		lbAutoclearPanel = Widget.Cast(layoutRoot.FindAnyWidget("lbAutoclearPanel"));
		lbAutoclearPeriodValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("lbAutoclearPeriodValue"));
		lbAutoclearPeriodMode = XComboBoxWidget.Cast(layoutRoot.FindAnyWidget("lbAutoclearPeriodMode"));
		lbAutoclearPeriodPanel = Widget.Cast(layoutRoot.FindAnyWidget("lbAutoclearPeriodPanel"));
		customMsgLbServerWideValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("customMsgLbServerWideValue"));
		customMsgLbServerWidePanel = Widget.Cast(layoutRoot.FindAnyWidget("customMsgLbServerWidePanel"));
		customMsgLbPersonalValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("customMsgLbPersonalValue"));
		customMsgLbPersonalPanel = Widget.Cast(layoutRoot.FindAnyWidget("customMsgLbPersonalPanel"));
		
		// map
		mapPlayerMarkerPanel = Widget.Cast(layoutRoot.FindAnyWidget("mapPlayerMarkerPanel"));
		mapPlayerMarkerValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("mapPlayerMarkerValue"));
		mapPlayerMarkerCustomTextPanel = Widget.Cast(layoutRoot.FindAnyWidget("mapPlayerMarkerCustomTextPanel"));
		mapPlayerMarkerCustomTextValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("mapPlayerMarkerCustomTextValue"));
		mapZoneBorderColorPanel = Widget.Cast(layoutRoot.FindAnyWidget("mapZoneBorderColorPanel"));
		mapZoneBorderColorValueR = SliderWidget.Cast(layoutRoot.FindAnyWidget("mapZoneBorderColorValueR"));
		mapZoneBorderColorValueG = SliderWidget.Cast(layoutRoot.FindAnyWidget("mapZoneBorderColorValueG"));
		mapZoneBorderColorValueB = SliderWidget.Cast(layoutRoot.FindAnyWidget("mapZoneBorderColorValueB"));
		mapZoneBorderColorImage = ImageWidget.Cast(layoutRoot.FindAnyWidget("mapZoneBorderColorImage"));
		mapShowLbMarkersValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("mapShowLbMarkersValue"));
		mapShowLbMarkersPanel = Widget.Cast(layoutRoot.FindAnyWidget("mapShowLbMarkersPanel"));
		mapShowLbNameValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("mapShowLbNameValue"));
		mapShowLbNamePanel = Widget.Cast(layoutRoot.FindAnyWidget("mapShowLbNamePanel"));
		mapLbCustomTextValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("mapLbCustomTextValue"));
		mapLbCustomTextPanel = Widget.Cast(layoutRoot.FindAnyWidget("mapLbCustomTextPanel"));
		mapLbMarkersUpdateFreqValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("mapLbMarkersUpdateFreqValue"));
		mapLbMarkersUpdateFreqPanel = Widget.Cast(layoutRoot.FindAnyWidget("mapLbMarkersUpdateFreqPanel"));
		mapLbMarkersUseApproximateValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("mapLbMarkersUseApproximateValue"));
		mapLbMarkersUseApproximatePanel = Widget.Cast(layoutRoot.FindAnyWidget("mapLbMarkersUseApproximatePanel"));
		mapLbMarkersApproximateOffsetValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("mapLbMarkersApproximateOffsetValue"));
		mapLbMarkersApproximateOffsetPanel = Widget.Cast(layoutRoot.FindAnyWidget("mapLbMarkersApproximateOffsetPanel"));
		mapLbCanSeeTheirOwnMarkerValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("mapLbCanSeeTheirOwnMarkerValue"));
		mapLbCanSeeTheirOwnMarkerPanel = Widget.Cast(layoutRoot.FindAnyWidget("mapLbCanSeeTheirOwnMarkerPanel"));
		// settings Apply button
		btnSettingsApply = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnSettingsApply"));

		/// ZONES page
		ZonesRootPanel = Widget.Cast(layoutRoot.FindAnyWidget("ZonesRootPanel"));
		ZonesRootPanel.Show(false);
		// list of zones
		zonesList = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("zonesList"));
		btnCreateNewZone = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnCreateNewZone"));
		// selected zone side panel
		ZoneSettingsPanel = Widget.Cast(layoutRoot.FindAnyWidget("ZoneSettingsPanel"));
		ZoneSettingsPanel.Show(false);
		zoneNameValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("zoneNameValue"));
		zoneCoordXValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("zoneCoordXValue"));
		zoneCoordZValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("zoneCoordZValue"));
		btnZoneSetCurrentPosition = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnZoneSetCurrentPosition"));
		zoneRadiusValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("zoneRadiusValue"));
		zoneShowBorderValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("zoneShowBorderValue"));
		zoneShowNameValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("zoneShowNameValue"));
		zoneDaysValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("zoneDaysValue"));
		zoneDaysModeValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("zoneDaysModeValue"));
		zoneHourStartValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("zoneHourStartValue"));
		zoneHourEndValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("zoneHourEndValue"));
		serverTimeValue = TextWidget.Cast(layoutRoot.FindAnyWidget("serverTimeValue"));
		// selected zone actions
		btnZoneApply = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnZoneApply"));
		btnZoneDelete = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnZoneDelete"));

		// dynamic zones setup panel
		btnAirdropZonesSetup = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnAirdropZonesSetup"));
		btnFlagZonesSetup = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnFlagZonesSetup"));
		btnHeliCrashZonesSetup = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnHeliCrashZonesSetup"));
		
		DynamicZoneSettingsPanel = Widget.Cast(layoutRoot.FindAnyWidget("DynamicZoneSettingsPanel"));
		DynamicZoneSettingsPanel.Show(false);
		dynamicZoneTitle = TextWidget.Cast(layoutRoot.FindAnyWidget("dynamicZoneTitle"));
		dynamicZoneNameValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("dynamicZoneNameValue"));
		dynamicZoneCoordXValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("dynamicZoneCoordXValue"));
		dynamicZoneCoordZValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("dynamicZoneCoordZValue"));
		dynamicZoneRadiusValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("dynamicZoneRadiusValue"));
		dynamicZoneShowBorderValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("dynamicZoneShowBorderValue"));
		dynamicZoneShowNameValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("dynamicZoneShowNameValue"));
		dynamicZoneDaysValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("dynamicZoneDaysValue"));
		dynamicZoneDaysModeValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("dynamicZoneDaysModeValue"));
		dynamicZoneHourStartValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("dynamicZoneHourStartValue"));
		dynamicZoneHourEndValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("dynamicZoneHourEndValue"));
		dynamicZoneFlagRaisedPanel = Widget.Cast(layoutRoot.FindAnyWidget("dynamicZoneFlagRaisedPanel"));
		dynamicZoneFlagRaisedValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("dynamicZoneFlagRaisedValue"));
		// selected zone actions
		btnDynamicZoneApply = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnDynamicZoneApply"));

		/// LAWBREAKERS page
		LawbreakersRootPanel = Widget.Cast(layoutRoot.FindAnyWidget("LawbreakersRootPanel"));
		LawbreakersRootPanel.Show(false);
		// list of LBs
		lbRosterList = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("lbRosterList"));
		lbPlayersList = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("lbPlayersList"));
		// selected LB side panel
		lbDataPanel = Widget.Cast(layoutRoot.FindAnyWidget("lbDataPanel"));
		lbDataPanel.Show(false);
		btnUIDCopy = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnUIDCopy"));
		lbUIDValue = RichTextWidget.Cast(layoutRoot.FindAnyWidget("lbUIDValue"));
		lbNamesValue = RichTextWidget.Cast(layoutRoot.FindAnyWidget("lbNamesValue"));
		lbCountValue = TextWidget.Cast(layoutRoot.FindAnyWidget("lbCountValue"));
		lbLatestMurderTimeValue = TextWidget.Cast(layoutRoot.FindAnyWidget("lbLatestMurderTimeValue"));
		lbStatusValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("lbStatusValue"));
		lbPreview = PlayerPreviewWidget.Cast(layoutRoot.FindAnyWidget("lbPreview"));
		// selected LB actions
		btnLbApply = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnLbApply"));
		btnLbDelete = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btnLbDelete"));
		btnLbDeleteBackground = ImageWidget.Cast(layoutRoot.FindAnyWidget("btnLbDeleteBackground"));

		// BOUNTIES page
		BountySettingsRootPanel = Widget.Cast(layoutRoot.FindAnyWidget("BountySettingsRootPanel"));
		BountySettingsRootPanel.Show(false);
		bountyCheckboxValue = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("bountyCheckboxValue"));
		bountyCheckboxPanel = Widget.Cast(layoutRoot.FindAnyWidget("bountyCheckboxPanel"));
		bountySearchValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("bountySearchValue"));
		bountyAllItemsList = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("bountyAllItemsList"));
		bountyAddButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("bountyAddButton"));
		bountySelectedItemsList = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("bountySelectedItemsList"));
		bountyRemoveButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("bountyRemoveButton"));
		bountyApplyButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("bountyApplyButton"));

		layoutRoot.Show(false);
		return layoutRoot;
	}

	override void OnShow() {
		super.OnShow();
		GetGame().GetInput().ChangeGameFocus( 1 );

		modeValue.SetCurrentItem(g_Game.pvez_Config.GENERAL.Mode);
		freqValue.SetText(g_Game.pvez_Config.GENERAL.Update_Frequency.ToString());
		showNotifsValue.SetChecked(g_Game.pvez_Config.GENERAL.Show_Notifications);
		showUIValue.SetChecked(g_Game.pvez_Config.GENERAL.Show_Notifications && g_Game.pvez_Config.GENERAL.Use_UI_Notifications);
		showNameInNotifsValue.SetChecked(g_Game.pvez_Config.GENERAL.Add_Zone_Name_To_Message);
		exitDelayValue.SetText(g_Game.pvez_Config.GENERAL.Exit_Zone_Countdown.ToString());
		force1stPersonValue.SetChecked(g_Game.pvez_Config.GENERAL.Force1stPersonInPVP);
		customMsgEnterValue.SetText(g_Game.pvez_Config.GENERAL.Custom_Enter_Zone_Message);
		customMsgExitValue.SetText(g_Game.pvez_Config.GENERAL.Custom_Exit_Zone_Message);
		customMsgExitCountdownValue.SetText(g_Game.pvez_Config.GENERAL.Custom_Exit_Zone_Countdown_Message);

		healTargetValue.SetChecked(g_Game.pvez_Config.DAMAGE.Restore_Target_Health);
		healCargoValue.SetChecked(g_Game.pvez_Config.DAMAGE.Protect_Clothing_And_Cargo);
		allowDmgBeyondZoneValue.SetChecked(g_Game.pvez_Config.DAMAGE.Allow_Damage_Between_PVP_and_PVE);
		reflectDmgWeaponsValue.SetChecked(g_Game.pvez_Config.DAMAGE.Damage_Types_Sent_Back_To_The_Attacker.Weapon_Damage);
		reflectDmgExplosivesValue.SetChecked(g_Game.pvez_Config.DAMAGE.Damage_Types_Sent_Back_To_The_Attacker.Explosive_Damage);
		reflectDmgVehiclesValue.SetChecked(g_Game.pvez_Config.DAMAGE.Damage_Types_Sent_Back_To_The_Attacker.Vehicle_Damage);
		reflectDmgFistsValue.SetChecked(g_Game.pvez_Config.DAMAGE.Damage_Types_Sent_Back_To_The_Attacker.Fist_Fight_Damage);

		lbKillTypeWeaponsValue.SetChecked(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Weaponary_Kill);
		lbKillTypeExplosivesValue.SetChecked(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Explosive_Kill);
		lbKillTypeVehiclesValue.SetChecked(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Vehicle_Kill);
		lbKillTypeFistsValue.SetChecked(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Fist_Fight_Kill);
		lbServerMessageValue.SetChecked(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Server_Wide_Message_About_Lawbreaker);
		lbAttackAnywhereValue.SetChecked(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Allow_Lawbreakers_To_Attack_Anywhere);
		lbPardonOnAnyDeathValue.SetChecked(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Pardon_On_Death_From_Any_Source);
		lbAutoclearValue.SetChecked(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Auto_Clear_Lawbreakers_Data);
		lbAutoclearPeriodValue.SetText(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Autoclear_Period_Amount.ToString());
		lbAutoclearPeriodMode.SetCurrentItem(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Autoclear_Period_Mode);
		customMsgLbServerWideValue.SetText(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Custom_Crime_Notification_Message_Format);
		customMsgLbPersonalValue.SetText(g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Custom_Message_for_the_Lawbreaker);

		mapPlayerMarkerValue.SetChecked(g_Game.pvez_Config.MAP.Show_Player_Marker);
		mapPlayerMarkerCustomTextValue.SetText(g_Game.pvez_Config.MAP.Custom_Player_Marker_Text);
		mapZoneBorderColorValueR.SetCurrent(g_Game.pvez_Config.MAP.Zones_Border_Color.R);
		mapZoneBorderColorValueG.SetCurrent(g_Game.pvez_Config.MAP.Zones_Border_Color.G);
		mapZoneBorderColorValueB.SetCurrent(g_Game.pvez_Config.MAP.Zones_Border_Color.B);
		mapZoneBorderColorImage.SetColor(ARGB(255, g_Game.pvez_Config.MAP.Zones_Border_Color.R, g_Game.pvez_Config.MAP.Zones_Border_Color.G, g_Game.pvez_Config.MAP.Zones_Border_Color.B));
		mapShowLbMarkersValue.SetChecked(g_Game.pvez_Config.MAP.Lawbreakers_Markers.Show_Markers_On_Map);
		mapShowLbNameValue.SetChecked(g_Game.pvez_Config.MAP.Lawbreakers_Markers.Show_Name);
		mapLbCustomTextValue.SetText(g_Game.pvez_Config.MAP.Lawbreakers_Markers.Custom_Lawbreaker_Label);
		mapLbMarkersUpdateFreqValue.SetText(g_Game.pvez_Config.MAP.Lawbreakers_Markers.Update_Frequency.ToString());
		mapLbMarkersUseApproximateValue.SetChecked(g_Game.pvez_Config.MAP.Lawbreakers_Markers.Approximate_Location);
		mapLbMarkersApproximateOffsetValue.SetText(g_Game.pvez_Config.MAP.Lawbreakers_Markers.Approximate_Location_Max_Offset.ToString());
		mapLbCanSeeTheirOwnMarkerValue.SetChecked(g_Game.pvez_Config.MAP.Lawbreakers_Markers.Lawbreakers_Can_See_Their_Own_Markers);
	}

	override void OnHide() {
		super.OnHide();
		GetGame().GetInput().ResetGameFocus();
		// Free some memory, we don't need these anymore
		playersOnServer = NULL;
		selectedZone = NULL;
		selectedLawbreaker = NULL;
	}

	override bool OnChange( Widget w, int x, int y, bool finished ) {
		if (w == showNotifsValue) {
			if (!showNotifsValue.IsChecked())
				showUIValue.SetChecked(false);
			return true;
		}
		if (w == showUIValue) {
			if (showUIValue.IsChecked())
				showNotifsValue.SetChecked(true);
			return true;
		}
		if (w == bountySearchValue) {
			UpdateBountiesAllItemsList();
			return true;
		}
		if (w == mapZoneBorderColorValueR || w == mapZoneBorderColorValueG || w == mapZoneBorderColorValueB) {
			mapZoneBorderColorImage.SetColor(ARGB(255, mapZoneBorderColorValueR.GetCurrent(), mapZoneBorderColorValueG.GetCurrent(), mapZoneBorderColorValueB.GetCurrent()));
			return true;
		}
		return false;
	}

	override bool OnClick( Widget w, int x, int y, int button ) {
		if (w == btnUIDCopy) {
			GetGame().CopyToClipboard(selectedLawbreaker.Id);
			return true;			
		}
		if (w == btnGeneral) {
			SettingsRootPanel.Show(true);
			ZonesRootPanel.Show(false);
			LawbreakersRootPanel.Show(false);
			lbDataPanel.Show(false);
			BountySettingsRootPanel.Show(false);
			return true;
		}
		if (w == btnZones) {
			SettingsRootPanel.Show(false);
			ZonesRootPanel.Show(true);
			ZoneSettingsPanel.Show(false);
			DynamicZoneSettingsPanel.Show(false);
			LawbreakersRootPanel.Show(false);
			lbDataPanel.Show(false);
			BountySettingsRootPanel.Show(false);
			if (GetGame().IsMultiplayer())
				GetGame().RPCSingleParam(GetGame().GetPlayer(), PVEZ_RPC.ADMIN_ZONES_DATA_REQUEST, NULL, true);
			else
				UpdateZonesList();
			return true;
		}
		if (w == btnLawbreakers) {
			SettingsRootPanel.Show(false);
			ZonesRootPanel.Show(false);
			LawbreakersRootPanel.Show(true);
			lbDataPanel.Show(false);
			BountySettingsRootPanel.Show(false);
			if (GetGame().IsMultiplayer())
				GetGame().RPCSingleParam(GetGame().GetPlayer(), PVEZ_RPC.ADMIN_LAWBREAKERS_DATA_REQUEST, NULL, true);
			else
				UpdateLawbreakersList(NULL);
			return true;
		}
		if (w == btnBounties) {
			SettingsRootPanel.Show(false);
			ZonesRootPanel.Show(false);
			LawbreakersRootPanel.Show(false);
			lbDataPanel.Show(false);
			BountySettingsRootPanel.Show(true);
			UpdateBountiesAllItemsList();
			if (GetGame().IsMultiplayer())
				GetGame().RPCSingleParam(GetGame().GetPlayer(), PVEZ_RPC.ADMIN_BOUNTIES_DATA_REQUEST, NULL, true);
			else
				UpdateBountiesPage(g_Game.pvez_Bounties);
			return true;
		}
		if (w == btnSettingsApply) {
			g_Game.pvez_Config.GENERAL.Mode = modeValue.GetCurrentItem();
			
			g_Game.pvez_Config.GENERAL.Update_Frequency = freqValue.GetText().ToInt();
			g_Game.pvez_Config.GENERAL.Show_Notifications = showNotifsValue.IsChecked();
			g_Game.pvez_Config.GENERAL.Use_UI_Notifications = showUIValue.IsChecked();
			g_Game.pvez_Config.GENERAL.Add_Zone_Name_To_Message = showNameInNotifsValue.IsChecked();
			g_Game.pvez_Config.GENERAL.Exit_Zone_Countdown = exitDelayValue.GetText().ToInt();
			g_Game.pvez_Config.GENERAL.Force1stPersonInPVP = force1stPersonValue.IsChecked();
			g_Game.pvez_Config.GENERAL.Custom_Enter_Zone_Message = customMsgEnterValue.GetText();
			g_Game.pvez_Config.GENERAL.Custom_Exit_Zone_Message = customMsgExitValue.GetText();
			g_Game.pvez_Config.GENERAL.Custom_Exit_Zone_Countdown_Message = customMsgExitCountdownValue.GetText();
			
			g_Game.pvez_Config.DAMAGE.Restore_Target_Health = healTargetValue.IsChecked();
			g_Game.pvez_Config.DAMAGE.Protect_Clothing_And_Cargo = healCargoValue.IsChecked();
			g_Game.pvez_Config.DAMAGE.Allow_Damage_Between_PVP_and_PVE = allowDmgBeyondZoneValue.IsChecked();
			g_Game.pvez_Config.DAMAGE.Damage_Types_Sent_Back_To_The_Attacker.Weapon_Damage = reflectDmgWeaponsValue.IsChecked();
			g_Game.pvez_Config.DAMAGE.Damage_Types_Sent_Back_To_The_Attacker.Explosive_Damage = reflectDmgExplosivesValue.IsChecked();
			g_Game.pvez_Config.DAMAGE.Damage_Types_Sent_Back_To_The_Attacker.Vehicle_Damage = reflectDmgVehiclesValue.IsChecked();
			g_Game.pvez_Config.DAMAGE.Damage_Types_Sent_Back_To_The_Attacker.Fist_Fight_Damage = reflectDmgFistsValue.IsChecked();

			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Weaponary_Kill = lbKillTypeWeaponsValue.IsChecked();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Explosive_Kill = lbKillTypeExplosivesValue.IsChecked();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Vehicle_Kill = lbKillTypeVehiclesValue.IsChecked();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Declare_a_Lawbreaker_When_Killed_a_Player_in_PVE_Area.On_Fist_Fight_Kill = lbKillTypeFistsValue.IsChecked();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Server_Wide_Message_About_Lawbreaker = lbServerMessageValue.IsChecked();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Allow_Lawbreakers_To_Attack_Anywhere = lbAttackAnywhereValue.IsChecked();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Pardon_On_Death_From_Any_Source = lbPardonOnAnyDeathValue.IsChecked();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Auto_Clear_Lawbreakers_Data = lbAutoclearValue.IsChecked();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Autoclear_Period_Amount = lbAutoclearPeriodValue.GetText().ToInt();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Autoclear_Period_Mode = lbAutoclearPeriodMode.GetCurrentItem();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Custom_Crime_Notification_Message_Format = customMsgLbServerWideValue.GetText();
			g_Game.pvez_Config.LAWBREAKERS_SYSTEM.Custom_Message_for_the_Lawbreaker = customMsgLbPersonalValue.GetText();

			g_Game.pvez_Config.MAP.Show_Player_Marker = mapPlayerMarkerValue.IsChecked();
			g_Game.pvez_Config.MAP.Custom_Player_Marker_Text =mapPlayerMarkerCustomTextValue.GetText();
			g_Game.pvez_Config.MAP.Zones_Border_Color.R = mapZoneBorderColorValueR.GetCurrent();
			g_Game.pvez_Config.MAP.Zones_Border_Color.G = mapZoneBorderColorValueG.GetCurrent();
			g_Game.pvez_Config.MAP.Zones_Border_Color.B = mapZoneBorderColorValueB.GetCurrent();
			g_Game.pvez_Config.MAP.Lawbreakers_Markers.Show_Markers_On_Map = mapShowLbMarkersValue.IsChecked();
			g_Game.pvez_Config.MAP.Lawbreakers_Markers.Show_Name = mapShowLbNameValue.IsChecked();
			g_Game.pvez_Config.MAP.Lawbreakers_Markers.Custom_Lawbreaker_Label = mapLbCustomTextValue.GetText();
			g_Game.pvez_Config.MAP.Lawbreakers_Markers.Update_Frequency = mapLbMarkersUpdateFreqValue.GetText().ToInt();
			g_Game.pvez_Config.MAP.Lawbreakers_Markers.Approximate_Location = mapLbMarkersUseApproximateValue.IsChecked();
			g_Game.pvez_Config.MAP.Lawbreakers_Markers.Approximate_Location_Max_Offset = mapLbMarkersApproximateOffsetValue.GetText().ToInt();
			g_Game.pvez_Config.MAP.Lawbreakers_Markers.Lawbreakers_Can_See_Their_Own_Markers = mapLbCanSeeTheirOwnMarkerValue.IsChecked();
			ApplySettings(PVEZ_RPC.UPDATE_CONFIG);
			return true;
		}
		if (w == zonesList) {
			int selectedZoneRow = zonesList.GetSelectedRow();
			if (selectedZoneRow == -1) return true;

			Param1<PVEZ_Zone> selectedZoneParam = new Param1<PVEZ_Zone>(NULL);
			zonesList.GetItemData(selectedZoneRow, 0, selectedZoneParam);
			selectedZone = selectedZoneParam.param1;

			if (selectedZone)
				OnZoneSelected(selectedZone);
			else
				ZoneSettingsPanel.Show(false);
			return true;
		}
		if (w == btnCreateNewZone) {
			selectedZone = new PVEZ_Zone(PVEZ_ZONE_TYPE_STATIC, 0, 0, "New zone", 100, false, false);
			OnZoneSelected(selectedZone);
			return true;
		}
		if (w == btnZoneSetCurrentPosition) {
			vector curPos = GetGame().GetPlayer().GetPosition();
			zoneCoordXValue.SetText(curPos[0].ToString());
			zoneCoordZValue.SetText(curPos[2].ToString());
			return true;
		}
		if (w == btnZoneApply) {
			int zoneindex = g_Game.pvez_Zones.staticZones.Find(selectedZone);
			if (zoneindex >= 0) {
				selectedZone = g_Game.pvez_Zones.staticZones[zoneindex];
			}
			else {
				g_Game.pvez_Zones.staticZones.Insert(selectedZone);
			}
			selectedZone.Name = zoneNameValue.GetText();
			selectedZone.X = zoneCoordXValue.GetText().ToInt();
			selectedZone.Z = zoneCoordZValue.GetText().ToInt();
			selectedZone.Radius = zoneRadiusValue.GetText().ToInt();
			selectedZone.ShowBorderOnMap = zoneShowBorderValue.IsChecked();
			selectedZone.ShowNameOnMap = zoneShowNameValue.IsChecked();
			selectedZone.Activity_Schedule.Days = zoneDaysValue.GetText();
			selectedZone.Activity_Schedule.StartHour = zoneHourStartValue.GetText().ToInt();
			selectedZone.Activity_Schedule.EndHour = zoneHourEndValue.GetText().ToInt();
			ApplySettings(PVEZ_RPC.UPDATE_ZONES);
			// If the week mode has been toggled, then push new config update to server
			if (zoneDaysModeValue.IsChecked() != g_Game.pvez_Config.GENERAL.Week_Starts_On_Sunday) {
				g_Game.pvez_Config.GENERAL.Week_Starts_On_Sunday = zoneDaysModeValue.IsChecked();
				ApplySettings(PVEZ_RPC.UPDATE_CONFIG);
			}
			UpdateZonesList();
			return true;
		}
		if (w == btnZoneDelete) {
			g_Game.pvez_Zones.staticZones.RemoveItem(selectedZone);
			ApplySettings(PVEZ_RPC.UPDATE_ZONES);
			UpdateZonesList();
			return true;
		}
		if (w == btnAirdropZonesSetup) {
			UpdateDynamicZonesSettingsPanel(PVEZ_ZONE_TYPE_AIRDROP);
		}
		if (w == btnFlagZonesSetup) {
			UpdateDynamicZonesSettingsPanel(PVEZ_ZONE_TYPE_TERRITORYFLAG);
		}
		if (w == btnHeliCrashZonesSetup) {
			UpdateDynamicZonesSettingsPanel(PVEZ_ZONE_TYPE_HELICRASH);
		}
		if (w == btnDynamicZoneApply) {
			if (dynamicZonesTypeSelected == PVEZ_ZONE_TYPE_AIRDROP) {
				g_Game.pvez_Config.AIRDROP_ZONES.Name = dynamicZoneNameValue.GetText();
				g_Game.pvez_Config.AIRDROP_ZONES.Radius = dynamicZoneRadiusValue.GetText().ToInt();
				g_Game.pvez_Config.AIRDROP_ZONES.ShowNameOnMap = dynamicZoneShowNameValue.IsChecked();
				g_Game.pvez_Config.AIRDROP_ZONES.ShowBorderOnMap = dynamicZoneShowBorderValue.IsChecked();
				g_Game.pvez_Config.AIRDROP_ZONES.Activity_Schedule.Days = dynamicZoneDaysValue.GetText();
				g_Game.pvez_Config.AIRDROP_ZONES.Activity_Schedule.StartHour = dynamicZoneHourStartValue.GetText().ToInt();
				g_Game.pvez_Config.AIRDROP_ZONES.Activity_Schedule.EndHour = dynamicZoneHourEndValue.GetText().ToInt();
				g_Game.pvez_Config.GENERAL.Week_Starts_On_Sunday = dynamicZoneDaysModeValue.IsChecked();
			}
			else if (dynamicZonesTypeSelected == PVEZ_ZONE_TYPE_TERRITORYFLAG) {
				g_Game.pvez_Config.TERRITORYFLAG_ZONES.Name = dynamicZoneNameValue.GetText();
				g_Game.pvez_Config.TERRITORYFLAG_ZONES.Radius = dynamicZoneRadiusValue.GetText().ToInt();
				g_Game.pvez_Config.TERRITORYFLAG_ZONES.ShowNameOnMap = dynamicZoneShowNameValue.IsChecked();
				g_Game.pvez_Config.TERRITORYFLAG_ZONES.ShowBorderOnMap = dynamicZoneShowBorderValue.IsChecked();
				g_Game.pvez_Config.TERRITORYFLAG_ZONES.Activity_Schedule.Days = dynamicZoneDaysValue.GetText();
				g_Game.pvez_Config.TERRITORYFLAG_ZONES.Activity_Schedule.StartHour = dynamicZoneHourStartValue.GetText().ToInt();
				g_Game.pvez_Config.TERRITORYFLAG_ZONES.Activity_Schedule.EndHour = dynamicZoneHourEndValue.GetText().ToInt();
				g_Game.pvez_Config.TERRITORYFLAG_ZONES.OnlyWhenFlagIsRaised = dynamicZoneFlagRaisedValue.IsChecked();
				g_Game.pvez_Config.GENERAL.Week_Starts_On_Sunday = dynamicZoneDaysModeValue.IsChecked();
			}
			else if (dynamicZonesTypeSelected == PVEZ_ZONE_TYPE_HELICRASH) {
				g_Game.pvez_Config.HELICRASH_ZONES.Name = dynamicZoneNameValue.GetText();
				g_Game.pvez_Config.HELICRASH_ZONES.Radius = dynamicZoneRadiusValue.GetText().ToInt();
				g_Game.pvez_Config.HELICRASH_ZONES.ShowNameOnMap = dynamicZoneShowNameValue.IsChecked();
				g_Game.pvez_Config.HELICRASH_ZONES.ShowBorderOnMap = dynamicZoneShowBorderValue.IsChecked();
				g_Game.pvez_Config.HELICRASH_ZONES.Activity_Schedule.Days = dynamicZoneDaysValue.GetText();
				g_Game.pvez_Config.HELICRASH_ZONES.Activity_Schedule.StartHour = dynamicZoneHourStartValue.GetText().ToInt();
				g_Game.pvez_Config.HELICRASH_ZONES.Activity_Schedule.EndHour = dynamicZoneHourEndValue.GetText().ToInt();
				g_Game.pvez_Config.GENERAL.Week_Starts_On_Sunday = dynamicZoneDaysModeValue.IsChecked();
			}
			DynamicZoneSettingsPanel.Show(false);
			ApplySettings(PVEZ_RPC.UPDATE_CONFIG);
		}
		if (w == lbRosterList || w == lbPlayersList) {
			int selectedLBRow = TextListboxWidget.Cast(w).GetSelectedRow();
			if (selectedLBRow == -1) return true;

			Param2<string, string> selectedLBParams = new Param2<string, string>("", "");
			TextListboxWidget.Cast(w).GetItemData(selectedLBRow, 0, selectedLBParams);
			string id = selectedLBParams.param1;
			string name = selectedLBParams.param2;

			selectedLawbreaker = g_Game.pvez_LawbreakersRoster.GetById(id);
			if (!selectedLawbreaker)
				selectedLawbreaker = new PVEZ_Lawbreaker(id, name, 0, false, PVEZ_Date.Now());
			OnLawbreakerSelected(selectedLawbreaker);
			return true;
		}
		if (w == btnLbApply) {
			int lbindex = g_Game.pvez_LawbreakersRoster.lbDataBase.Find(selectedLawbreaker);
			if (lbindex >= 0) {
				selectedLawbreaker.Is_Currently_Outlaw = lbStatusValue.IsChecked();
				PlayerBase.Cast(GetGame().GetPlayer()).MessageStatus("PVEZ: Changing the lawbreaker status. The player need to wait a minute.");
			}
			else if (lbindex < 0 && lbStatusValue.IsChecked()) {
				// If the LB in not in the roster yet and admin wants to flag him as LB
				selectedLawbreaker.Is_Currently_Outlaw = lbStatusValue.IsChecked();
				selectedLawbreaker.Murder_Count = 1;
				g_Game.pvez_LawbreakersRoster.lbDataBase.Insert(selectedLawbreaker);
				PlayerBase.Cast(GetGame().GetPlayer()).MessageStatus("PVEZ: Changing the lawbreaker status. The player need to wait a minute.");
			}
			ApplySettings(PVEZ_RPC.ADMIN_UPDATE_LAWBREAKERS);
			lbDataPanel.Show(false);
			return true;
		}
		if (w == btnLbDelete) {
			PlayerBase.Cast(GetGame().GetPlayer()).MessageStatus("PVEZ: Deleting the lawbreaker data. The player need to wait a minute.");
			g_Game.pvez_LawbreakersRoster.lbDataBase.RemoveItem(selectedLawbreaker);
			ApplySettings(PVEZ_RPC.ADMIN_UPDATE_LAWBREAKERS);
			lbDataPanel.Show(false);
			return true;
		}
		if (w == bountyAddButton) {
			int itemRow = bountyAllItemsList.GetSelectedRow();
			if (itemRow == -1) return true;
			Param2<string, string> itemParam = new Param2<string, string>("", "");
			bountyAllItemsList.GetItemData(itemRow, 0, itemParam);
			AddToBountySelectedItemsList(itemParam.param1, itemParam.param2);
			return true;
		}
		if (w == bountyRemoveButton) {
			RemoveFromBountySelectedItemsList(bountySelectedItemsList.GetSelectedRow());
			return true;
		}
		if (w == bountyApplyButton) {
			array<ref PVEZ_BountyItemData> newBountiesList = new array<ref PVEZ_BountyItemData>;
			for (int i = 0; i < bountySelectedItemsList.GetNumItems(); i++) {
				Param3<string, string, int> selectedItemParam = new Param3<string, string, int>("", "", -1);
				bountySelectedItemsList.GetItemData(i, 0, selectedItemParam);
				string className = selectedItemParam.param1;
				string displayName = selectedItemParam.param2;
				int amount = selectedItemParam.param3;
				PVEZ_BountyItemData itemData = new PVEZ_BountyItemData(className, displayName, amount);
				newBountiesList.Insert(itemData);
			}
			Param2<bool, array<ref PVEZ_BountyItemData>> newBountiesSettings = new Param2<bool, array<ref PVEZ_BountyItemData>>(bountyCheckboxValue.IsChecked(), newBountiesList);
			if (GetGame().IsMultiplayer())
				GetGame().RPCSingleParam(GetGame().GetPlayer(), PVEZ_RPC.UPDATE_BOUNTIES, newBountiesSettings, true);
			else {
				g_Game.pvez_Bounties.Enabled = bountyCheckboxValue.IsChecked();
				g_Game.pvez_Bounties.Items = newBountiesList;
				g_Game.pvez_Bounties.SaveToJson();
			}
			return true;
		}
		return false;
	}

	void ApplySettings(int type) {
		switch (type) {
			case PVEZ_RPC.UPDATE_CONFIG:
				if (GetGame().IsMultiplayer())
					GetGame().RPCSingleParam(GetGame().GetPlayer(), PVEZ_RPC.UPDATE_CONFIG, new Param1<PVEZ_Config>(g_Game.pvez_Config), true);
				else {
					g_Game.pvez_Config.SaveToJson();
					g_Game.pvez_Zones.Init();
				}
				break;
			case PVEZ_RPC.UPDATE_ZONES:
				if (GetGame().IsMultiplayer())
					GetGame().RPCSingleParam(GetGame().GetPlayer(), PVEZ_RPC.UPDATE_ZONES, new Param1<array<ref PVEZ_Zone>>(g_Game.pvez_Zones.staticZones), true);
				else {
					g_Game.pvez_Zones.SaveToJson();
					g_Game.pvez_Zones.Init();
				}
				break;
			case PVEZ_RPC.ADMIN_UPDATE_LAWBREAKERS:
				if (GetGame().IsMultiplayer())
					GetGame().RPCSingleParam(GetGame().GetPlayer(), PVEZ_RPC.ADMIN_UPDATE_LAWBREAKERS, new Param1<array<ref PVEZ_Lawbreaker>>(g_Game.pvez_LawbreakersRoster.lbDataBase), true);
				else {
					g_Game.pvez_LawbreakersRoster.SaveToJson();
					g_Game.pvez_LawbreakersRoster = new PVEZ_LawbreakersRoster(g_Game.pvez_Config);
				}
				break;
		}
	}

	void UpdateZonesList() {
		zonesList.ClearItems();
		for (int i = 0; i < g_Game.pvez_Zones.staticZones.Count(); i++) {
			PVEZ_Zone zone = g_Game.pvez_Zones.staticZones.Get(i);
			Param1<PVEZ_Zone> data = new Param1<PVEZ_Zone>(zone);
			zonesList.AddItem((i + 1).ToString() + ". " + g_Game.pvez_Zones.staticZones[i].Name, data, 0, i);
		}
		if (g_Game.pvez_Zones.staticZones.Find(selectedZone) < 0)
			ZoneSettingsPanel.Show(false);
	}

	void OnZoneSelected(PVEZ_Zone selectedZone) {
		DynamicZoneSettingsPanel.Show(false);
		ZoneSettingsPanel.Show(true);
		
		zoneNameValue.SetText(selectedZone.Name);
		zoneCoordXValue.SetText(selectedZone.X.ToString());
		zoneCoordZValue.SetText(selectedZone.Z.ToString());
		zoneRadiusValue.SetText(selectedZone.Radius.ToString());
		zoneShowBorderValue.SetChecked(selectedZone.ShowBorderOnMap);
		zoneShowNameValue.SetChecked(selectedZone.ShowNameOnMap);
		zoneDaysValue.SetText(selectedZone.Activity_Schedule.Days);
		zoneDaysModeValue.SetChecked(g_Game.pvez_Config.GENERAL.Week_Starts_On_Sunday);
		zoneHourStartValue.SetText(selectedZone.Activity_Schedule.StartHour.ToString());
		zoneHourEndValue.SetText(selectedZone.Activity_Schedule.EndHour.ToString());
		// Display server time
		int day, hour, min, sec;
		GetHourMinuteSecond(hour, min, sec);
		day = PVEZ_StaticFunctions.GetDayOfWeek(g_Game.pvez_Config.GENERAL.Week_Starts_On_Sunday);
		serverTimeValue.SetText("[Day]" + day.ToString() + ", [Time]" + hour.ToString() + ":" + min.ToString());
	}

	void UpdateLawbreakersList(array<Man> players) {
		if (!players && !GetGame().IsMultiplayer()) {
			playersOnServer = new array<Man>;
			playersOnServer.Insert(GetGame().GetPlayer());
		}
		else {
			playersOnServer = new array<Man>;
			playersOnServer.Copy(players);
		}
		
		lbRosterList.ClearItems();
		lbPlayersList.ClearItems();

		string id;
		string name;
		PVEZ_Lawbreaker lb;
		Param2<string, string> data;
		
		// Lawbreakers in the roster
		for (int i = 0; i < g_Game.pvez_LawbreakersRoster.lbDataBase.Count(); i++) {
			lb = g_Game.pvez_LawbreakersRoster.lbDataBase.Get(i);
			id = lb.Id;
			name = lb.Recent_Character_Names[0];
			data = new Param2<string, string>(id, name);
			lbRosterList.AddItem((i + 1).ToString() + ". " + name + " (" + id + ")", data, 0, i);
		}

		// Other players who are not in the roster
		for (int j = 0; j < playersOnServer.Count(); j++) {
			id = PVEZ_StaticFunctions.GetEntityId(playersOnServer[j]);
			lb = g_Game.pvez_LawbreakersRoster.GetById(id);
			if (!lb) {
				name = PVEZ_StaticFunctions.GetEntityName(playersOnServer[j]);
				if (id == "" || name == "") {
					name = "Failed to get player name...";
				}
				data = new Param2<string, string>(id, name);
				lbPlayersList.AddItem((j + 1).ToString() + ". " + name + " (" + id + ")", data, 0, j);
			}
		}
		//if (g_Game.pvez_LawbreakersRoster.lbDataBase.Find(selectedLawbreaker) < 0)
		lbDataPanel.Show(false);
	}

	void OnLawbreakerSelected(PVEZ_Lawbreaker selectedLawbreaker) {
		if (!lbDataPanel.IsVisible())
			lbDataPanel.Show(true);
		// Fake lawbreaker (a player who's not in the LB roster yet) has murder count 0.
		// Disable "Delete" button if the selected player is a fake LB.
		btnLbDeleteBackground.Show(selectedLawbreaker.Murder_Count > 0);
		lbUIDValue.SetText(selectedLawbreaker.Id);
		string names = "";
		for (int i = 0; i < selectedLawbreaker.Recent_Character_Names.Count(); i++) {
			names = names + selectedLawbreaker.Recent_Character_Names[i] + ", ";
		}
		lbNamesValue.SetText(names);
		lbCountValue.SetText(selectedLawbreaker.Murder_Count.ToString());
		lbLatestMurderTimeValue.SetText(PVEZ_StaticFunctions.GetFormattedDateTimeFromPVEZ_Date(selectedLawbreaker.Latest_Murder_Time));
		lbStatusValue.SetChecked(selectedLawbreaker.Is_Currently_Outlaw);

		if (playersOnServer && playersOnServer.Count() > 0) {
			foreach (Man p : playersOnServer) {
				if (PVEZ_StaticFunctions.GetEntityId(p) == selectedLawbreaker.Id) {
					lbPreview.SetPlayer(DayZPlayer.Cast(p));
					lbPreview.SetModelPosition( "0 0 0.605" );
					lbPreview.SetSize( 0.5, 0.5 );
					break;
				}
			}
		}
	}

	void UpdateBountiesPage(PVEZ_Bounties data) {
		bountyCheckboxValue.SetChecked(data.Enabled);
		bountySelectedItemsList.ClearItems();
		for (int i = 0; i < data.Items.Count(); i++) {
			string className = data.Items[i].className;
			string displayName = data.Items[i].displayName;
			int amount = data.Items[i].amount;
			bountySelectedItemsList.AddItem(displayName + " x" + amount, new Param3<string, string, int>(className, displayName, amount), 0, i);
		}
	}

	void UpdateBountiesAllItemsList() {
		bountyAllItemsList.ClearItems();

		TStringArray configs = new TStringArray;
		configs.Insert( CFG_VEHICLESPATH );
		configs.Insert( CFG_WEAPONSPATH );
		configs.Insert( CFG_MAGAZINESPATH );

		string strSearch = bountySearchValue.GetText();
		strSearch.ToLower();

		for ( int nConfig = 0; nConfig < configs.Count(); ++nConfig ) {
			string strConfigPath = configs.Get( nConfig );
			int nClasses = g_Game.ConfigGetChildrenCount( strConfigPath );

			for ( int nClass = 0; nClass < nClasses; ++nClass ) {
				string strName;
				g_Game.ConfigGetChildName( strConfigPath, nClass, strName );
				string displayName;
				g_Game.ConfigGetText( strConfigPath + " " + strName + " displayName", displayName );
				
				int scope = g_Game.ConfigGetInt( strConfigPath + " " + strName + " scope" );
				if ( scope == 0 ) continue;

				string strNameLower = strName;
				string displayNameLower = displayName;
				strNameLower.ToLower();
				displayNameLower.ToLower();

				if (strSearch != "" && !(strNameLower.Contains(strSearch) || displayNameLower.Contains(strSearch))) continue;

				bountyAllItemsList.AddItem(strName + " [" + displayName + "]", new Param3<string, string, int>(strName, displayName, 1), 0, nClasses);
			}
		}
	}

	void AddToBountySelectedItemsList(string className, string displayName) {
		Param3<string, string, int> p = new Param3<string, string, int>("", "", 1);
		bool alreadyInList;

		for (int i = 0; i < bountySelectedItemsList.GetNumItems(); i++) {
			alreadyInList = false;
			bountySelectedItemsList.GetItemData(i, 0, p);
			if (p.param1 == className) {
				alreadyInList = true;
				p.param3++;
				bountySelectedItemsList.SetItem(i, p.param2 + " x" + p.param3, p, 0);
				break;
			}
		}
		if (!alreadyInList) {
			p = new Param3<string, string, int>(className, displayName, 1);
			bountySelectedItemsList.AddItem(displayName + " x1", p, 0, bountySelectedItemsList.GetNumItems() + 1);
		}
	}

	void RemoveFromBountySelectedItemsList(int row) {
		if (row < 0)
			return;

		Param3<string, string, int> p = new Param3<string, string, int>("", "", -1);
		bountySelectedItemsList.GetItemData(row, 0, p);
		p.param3--;
		if (p.param3 > 0)
			bountySelectedItemsList.SetItem(row, p.param2 + " x" + p.param3, p, 0);
		else {
			bountySelectedItemsList.RemoveRow(row);
			if (bountySelectedItemsList.GetNumItems() > 0)
				bountySelectedItemsList.SelectRow(0);
			else
				bountySelectedItemsList.ClearItems();
		}
	}

	void UpdateDynamicZonesSettingsPanel(int type) {
		ZoneSettingsPanel.Show(false);
		DynamicZoneSettingsPanel.Show(true);
			
		dynamicZonesTypeSelected = type;
		
		if (type == PVEZ_ZONE_TYPE_AIRDROP) {
			dynamicZoneTitle.SetText("Airdrop zones settings:");
			dynamicZoneNameValue.SetText(g_Game.pvez_Config.AIRDROP_ZONES.Name);
			dynamicZoneRadiusValue.SetText(g_Game.pvez_Config.AIRDROP_ZONES.Radius.ToString());
			dynamicZoneShowNameValue.SetChecked(g_Game.pvez_Config.AIRDROP_ZONES.ShowNameOnMap);
			dynamicZoneShowBorderValue.SetChecked(g_Game.pvez_Config.AIRDROP_ZONES.ShowBorderOnMap);
			dynamicZoneDaysValue.SetText(g_Game.pvez_Config.AIRDROP_ZONES.Activity_Schedule.Days);
			dynamicZoneHourStartValue.SetText(g_Game.pvez_Config.AIRDROP_ZONES.Activity_Schedule.StartHour.ToString());
			dynamicZoneHourEndValue.SetText(g_Game.pvez_Config.AIRDROP_ZONES.Activity_Schedule.EndHour.ToString());
			dynamicZoneFlagRaisedPanel.Show(false);
			dynamicZoneDaysModeValue.SetChecked(g_Game.pvez_Config.GENERAL.Week_Starts_On_Sunday);
		}
		else if (type == PVEZ_ZONE_TYPE_TERRITORYFLAG) {
			dynamicZoneTitle.SetText("Territory flag zones settings:");
			dynamicZoneNameValue.SetText(g_Game.pvez_Config.TERRITORYFLAG_ZONES.Name);
			dynamicZoneRadiusValue.SetText(g_Game.pvez_Config.TERRITORYFLAG_ZONES.Radius.ToString());
			dynamicZoneShowNameValue.SetChecked(g_Game.pvez_Config.TERRITORYFLAG_ZONES.ShowNameOnMap);
			dynamicZoneShowBorderValue.SetChecked(g_Game.pvez_Config.TERRITORYFLAG_ZONES.ShowBorderOnMap);
			dynamicZoneDaysValue.SetText(g_Game.pvez_Config.TERRITORYFLAG_ZONES.Activity_Schedule.Days);
			dynamicZoneHourStartValue.SetText(g_Game.pvez_Config.TERRITORYFLAG_ZONES.Activity_Schedule.StartHour.ToString());
			dynamicZoneHourEndValue.SetText(g_Game.pvez_Config.TERRITORYFLAG_ZONES.Activity_Schedule.EndHour.ToString());
			dynamicZoneFlagRaisedPanel.Show(true);
			dynamicZoneFlagRaisedValue.SetChecked(g_Game.pvez_Config.TERRITORYFLAG_ZONES.OnlyWhenFlagIsRaised);
			dynamicZoneDaysModeValue.SetChecked(g_Game.pvez_Config.GENERAL.Week_Starts_On_Sunday);
		}
		else if (type == PVEZ_ZONE_TYPE_HELICRASH) {
			dynamicZoneTitle.SetText("Heli crash zones settings:");
			dynamicZoneNameValue.SetText(g_Game.pvez_Config.HELICRASH_ZONES.Name);
			dynamicZoneRadiusValue.SetText(g_Game.pvez_Config.HELICRASH_ZONES.Radius.ToString());
			dynamicZoneShowNameValue.SetChecked(g_Game.pvez_Config.HELICRASH_ZONES.ShowNameOnMap);
			dynamicZoneShowBorderValue.SetChecked(g_Game.pvez_Config.HELICRASH_ZONES.ShowBorderOnMap);
			dynamicZoneDaysValue.SetText(g_Game.pvez_Config.HELICRASH_ZONES.Activity_Schedule.Days);
			dynamicZoneHourStartValue.SetText(g_Game.pvez_Config.HELICRASH_ZONES.Activity_Schedule.StartHour.ToString());
			dynamicZoneHourEndValue.SetText(g_Game.pvez_Config.HELICRASH_ZONES.Activity_Schedule.EndHour.ToString());
			dynamicZoneFlagRaisedPanel.Show(false);
			dynamicZoneDaysModeValue.SetChecked(g_Game.pvez_Config.GENERAL.Week_Starts_On_Sunday);
		}
	}

	override bool OnMouseEnter(Widget w, int x, int y) {
		if (w.GetTypeName() == "EditBoxWidget") {
			isTypingText = true;
		}
		
		switch (w.GetParent()) {
			case modePanel:
				descriptionTitle.SetText("#pvez_aui_desc_mode");
				descriptionFrame.Show(true);
				return true;
			case freqPanel:
				descriptionTitle.SetText("#pvez_aui_desc_updatefreq");
				descriptionFrame.Show(true);
				return true;
			case showNotifsPanel:
				descriptionTitle.SetText("#pvez_aui_desc_shownotifs");
				descriptionFrame.Show(true);
				return true;
			case showUIPanel:
				descriptionTitle.SetText("#pvez_aui_desc_useuinotifs");
				descriptionFrame.Show(true);
				return true;
			case showNameInNotifsPanel:
				descriptionTitle.SetText("#pvez_aui_desc_zonenameinnotifs");
				descriptionFrame.Show(true);
				return true;
			case exitDelayPanel:
				descriptionTitle.SetText("#pvez_aui_desc_exitdelay");
				descriptionFrame.Show(true);
				return true;
			case force1stPersonPanel:
				descriptionTitle.SetText("#pvez_aui_desc_force1stperson");
				descriptionFrame.Show(true);
				return true;
			case customMsgEnterPanel:
				descriptionTitle.SetText("#pvez_aui_desc_custommsgenter");
				descriptionFrame.Show(true);
				return true;
			case customMsgExitPanel:
				descriptionTitle.SetText("#pvez_aui_desc_custommsgexit");
				descriptionFrame.Show(true);
				return true;
			case customMsgExitCountdownPanel:
				descriptionTitle.SetText("#pvez_aui_desc_custommsgcntdwn");
				descriptionFrame.Show(true);
				return true;
			case healTargetPanel:
				descriptionTitle.SetText("#pvez_aui_desc_healtarget");
				descriptionFrame.Show(true);
				return true;
			case healCargoPanel:
				descriptionTitle.SetText("#pvez_aui_desc_healcargo");
				descriptionFrame.Show(true);
				return true;
			case allowDmgBeyondZonePanel:
				descriptionTitle.SetText("#pvez_aui_desc_allowdmgbeyond");
				descriptionFrame.Show(true);
				return true;
			case reflectDmgPanel:
				descriptionTitle.SetText("#pvez_aui_desc_reflecttypes");
				descriptionFrame.Show(true);
				return true;
			case lbKillTypesPanel:
				descriptionTitle.SetText("#pvez_aui_desc_lbcases");
				descriptionFrame.Show(true);
				return true;
			case lbServerMessagePanel:
				descriptionTitle.SetText("#pvez_aui_desc_lbserverwide");
				descriptionFrame.Show(true);
				return true;
			case lbAutoclearPanel:
				descriptionTitle.SetText("#pvez_aui_desc_lbautoclear");
				descriptionFrame.Show(true);
				return true;
			case lbAutoclearPeriodPanel:
				descriptionTitle.SetText("#pvez_aui_desc_lbautoclearperiod");
				descriptionFrame.Show(true);
				return true;
			case lbAttackAnywherePanel:
				descriptionTitle.SetText("#pvez_aui_desc_lbattackanywhere");
				descriptionFrame.Show(true);
				return true;
			case lbPardonOnAnyDeathPanel:
				descriptionTitle.SetText("#pvez_aui_desc_lbpardonanydeath");
				descriptionFrame.Show(true);
				return true;
			case customMsgLbServerWidePanel:
				string msg = "#pvez_aui_desc_lbswcustom";
				descriptionTitle.SetText(msg);
				descriptionFrame.Show(true);
				return true;
			case customMsgLbPersonalPanel:
				descriptionTitle.SetText("#pvez_aui_desc_lbpersonalmsg");
				descriptionFrame.Show(true);
				return true;
			case mapPlayerMarkerPanel:
				descriptionTitle.SetText("#pvez_aui_desc_mapplayermarker");
				descriptionFrame.Show(true);
				return true;
			case mapPlayerMarkerCustomTextPanel:
				descriptionTitle.SetText("#pvez_aui_desc_mapplayermarkercustom");
				descriptionFrame.Show(true);
				return true;
			case mapZoneBorderColorPanel:
				descriptionTitle.SetText("#pvez_aui_desc_mapzonecolor");
				descriptionFrame.Show(true);
				return true;
			case mapShowLbMarkersPanel:
				descriptionTitle.SetText("#pvez_aui_desc_lbshowmarkers");
				descriptionFrame.Show(true);
				return true;
			case mapShowLbNamePanel:
				descriptionTitle.SetText("#pvez_aui_desc_lbshowname");
				descriptionFrame.Show(true);
				return true;
			case mapLbCustomTextPanel:
				descriptionTitle.SetText("#pvez_aui_desc_lbcustomtext");
				descriptionFrame.Show(true);
				return true;
			case mapLbMarkersUpdateFreqPanel:
				descriptionTitle.SetText("#pvez_aui_desc_lbmarkerupdatefreq");
				descriptionFrame.Show(true);
				return true;
			case mapLbMarkersUseApproximatePanel:
				descriptionTitle.SetText("#pvez_aui_desc_lbapproximate");
				descriptionFrame.Show(true);
				return true;
			case mapLbMarkersApproximateOffsetPanel:
				descriptionTitle.SetText("#pvez_aui_desc_lbapproximate_offset");
				descriptionFrame.Show(true);
				return true;
			case mapLbCanSeeTheirOwnMarkerPanel:
				descriptionTitle.SetText("#pvez_aui_desc_lbcanseethemselves");
				descriptionFrame.Show(true);
				return true;
		}
		switch (w) {
			case btnAirdropZonesSetup:
				descriptionTitle.SetText("#pvez_aui_desc_airdropzonesbtn");
				descriptionFrame.Show(true);
				return true;
			case btnFlagZonesSetup:
				descriptionTitle.SetText("#pvez_aui_desc_flagzonesbtn");
				descriptionFrame.Show(true);
				return true;
			case btnHeliCrashZonesSetup:
				descriptionTitle.SetText("#pvez_aui_desc_helicrashzonesbtn");
				descriptionFrame.Show(true);
				return true;
		}
		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y) {
		if (w.GetTypeName() == "EditBoxWidget") {
			isTypingText = false;
		}
		descriptionFrame.Show(false);
		return true;
	}
}