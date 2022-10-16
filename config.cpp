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
		credits="GumbyMN";
		author="Ermiq";
		inputs="PVEZ/data/modded_Inputs.xml";
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
					"PVEZ/data/Common"
				};
			};
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"PVEZ/data/Common",
					"PVEZ/scripts/3_Game",
					"PVEZ/data/Plugins/BasicMap/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"PVEZ/data/Common",
					"PVEZ/scripts/4_World",
					"PVEZ/data/Plugins/Expansion/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"PVEZ/data/Common",
					"PVEZ/scripts/5_Mission",
					"PVEZ/data/Plugins/Expansion/5_Mission"
				};
			};
		};
	};
};
