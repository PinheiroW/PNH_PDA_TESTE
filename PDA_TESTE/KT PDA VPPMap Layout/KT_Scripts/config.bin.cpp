class CfgMods
{
	class KT_SMU_Stalker
	{
		name="KT_SMU_Stalker";
		credits="KampfTeddy";
		author="KampfTeddy";
		authorID="";
		version="1.0";
		extra=0;
		type="mod";
		inputs="KT_SMU_Stalker/PDA/data/Inputs.xml";
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
					"KT_SMU_Stalker/KT_Scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"KT_SMU_Stalker/KT_Scripts/4_world"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"KT_SMU_Stalker/KT_Scripts/5_Mission"
				};
			};
		};
	};
};
class CfgPatches
{
	class KT_SMU_Stalker
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Gear_Containers",
			"DZ_Gear_Camping",
			"DZ_Data"
		};
	};
};
