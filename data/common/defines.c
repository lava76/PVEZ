/*
	This file is listed in <config.cpp> to be included in all script modules (1_GameLib, 3_Game, 4_World, 5_Mission).
	To be more precise, the directory "PVEZ/Common/" is listed there, and the files in this directory get compiled in all modules.
	So the defines listed here should be detected by def check from any module.

#ifdef PVEZ
	//Do something specific if PVEZ is loaded
#endif

*/

// In DEBUG mode infected are treated as players.
//#define PVEZ_DEBUGMODE

// For other mods to use, if they want to know whether PVEZ is loaded on the server or not.
#define PVEZ