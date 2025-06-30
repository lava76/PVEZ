class CfgPatches
{
	class PVEZ
	{
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Gear_Navigation"
		};
	};
};
class CfgMods
{
	class PVEZ
	{
		extra=0;
		type="mod";
		name="PVEZ";
		credits="Ermiq";
		author="Ermiq";
		inputs="PVEZ/data/inputs.xml";
		overview="$STR_pvez_overview";
		dependencies[]=
		{
			"GameLib",
			"Game",
			"World",
			"Mission"
		};
		class defs
		{
			class gameLibScriptModule
			{
				value="";
				files[]=
				{
					"PVEZ/Common"
				};
			};
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"PVEZ/Common",
					"PVEZ/3_Game",
					"PVEZ/Plugins/BasicMap/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"PVEZ/Common",
					"PVEZ/4_World",
					"PVEZ/Plugins/Expansion/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"PVEZ/Common",
					"PVEZ/5_Mission",
					"PVEZ/Plugins/Expansion/5_Mission",
					"PVEZ/Plugins/VPPAdminTools/5_Mission",
					"PVEZ/Plugins/Carim/5_Mission"
				};
			};
		};
	};
};
