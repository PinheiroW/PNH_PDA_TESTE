class CfgMods
{
	class PNH_Tablet
	{
		name="PNH_Tablet";
		credits="Pinheiro";
		author="Pinheiro";
		authorID="";
		version="1.0";
		extra=0;
		type="mod";
		inputs="PNH_Tablet/PDA/data/Inputs.xml";
		dependencies[]=
		{
			"Game",
			"World",
			"Mission"
		};
		class defs
		{
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"PNH_Tablet/Scripts/3_Game",
					"PNH_Tablet/Scripts/3_Game/VPP"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"PNH_Tablet/Scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"PNH_Tablet/Scripts/5_Mission",
					"PNH_Tablet/Scripts/5_Mission/VPP",
					"PNH_Tablet/Scripts/5_Mission/PDA"
				};
			};
		};
	};
};

class CfgPatches
{
	class PNH_Tablet
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Gear_Containers",
			"DZ_Gear_Camping",
			"DZ_Data",
			"DZ_Scripts",
			"JM_CF_Scripts"
		};
	};
};