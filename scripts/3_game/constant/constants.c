const string PVEZ_ERROR_PREFIX = "PVEZ ERROR :: ";

string PVEZ_CONFIG_FOLDER = "$profile:\\PVEZ";
const string PVEZ_ADMINS_TXT = PVEZ_CONFIG_FOLDER + "\\Admins.txt";
const string PVEZ_CONFIG_JSON = PVEZ_CONFIG_FOLDER + "\\Config.json";
const string PVEZ_LAWBREAKERS_ROSTER_JSON = PVEZ_CONFIG_FOLDER + "\\Lawbreakers.json";
const string PVEZ_BOUNTIES_JSON = PVEZ_CONFIG_FOLDER + "\\Bounties.json";
const string PVEZ_ZONES_JSON = PVEZ_CONFIG_FOLDER + "\\Zones.json";
const string PVEZ_ZONES_CHERNARUS_JSON = PVEZ_CONFIG_FOLDER + "\\Zones_Chernarus.json";
const string PVEZ_ZONES_LIVONIA_JSON = PVEZ_CONFIG_FOLDER + "\\Zones_Livonia.json";
const string PVEZ_ZONES_NAMALSK_JSON = PVEZ_CONFIG_FOLDER + "\\Zones_Namalsk.json";

const string PVEZ_CONFIG_FOLDER_OLD = "$profile:\\PVEZ_Config";
const string PVEZ_CONFIG_JSON_OLD = PVEZ_CONFIG_FOLDER_OLD + "\\PVEZ_Config.json";
const string PVEZ_LAWBREAKERS_ROSTER_JSON_OLD = PVEZ_CONFIG_FOLDER_OLD + "\\PVEZ_Lawbreakers.json";
const string PVEZ_BOUNTIES_JSON_OLD = PVEZ_CONFIG_FOLDER_OLD + "\\PVEZ_Bounties.json";
const string PVEZ_ZONES_JSON_OLD = PVEZ_CONFIG_FOLDER_OLD + "\\PVEZ_Zones.json";
const string PVEZ_ADMINS_TXT_OLD = PVEZ_CONFIG_FOLDER_OLD + "\\Admins.txt";

const int PVEZ_DAMAGE_SOURCE_TYPE_EXPLOSIVE = 0;
const int PVEZ_DAMAGE_SOURCE_TYPE_WEAPON = 1;
const int PVEZ_DAMAGE_SOURCE_TYPE_VEHICLE = 2;
const int PVEZ_DAMAGE_SOURCE_TYPE_FISTS = 3;
const int PVEZ_DAMAGE_SOURCE_TYPE_SELF = 4;
const int PVEZ_DAMAGE_SOURCE_TYPE_CLAWS = 5;

const string PVEZ_DEFAULT_ENTER_ZONE_MESSAGE = "You have entered PVP zone";
const string PVEZ_DEFAULT_EXIT_ZONE_MESSAGE = "Left PVP zone";
const int PVEZ_DEFAULT_EXIT_ZONE_COUNTDOWN = 10;
const string PVEZ_DEFAULT_EXIT_ZONE_COUNTDOWN_MESSAGE = "You are going to leave PVP zone in";

const int PVEZ_MODE_PVP_ZONES = 0;
const int PVEZ_MODE_PVE_ZONES = 1;
const int PVEZ_MODE_PVE = 2;
const int PVEZ_MODE_PVP = 3;

const int PVEZ_ZONE_TYPE_STATIC = 0;
const int PVEZ_ZONE_TYPE_AIRDROP = 1;
const int PVEZ_ZONE_TYPE_TERRITORYFLAG = 2;
const int PVEZ_ZONE_TYPE_HELICRASH = 3;