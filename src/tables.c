/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
*	ROM 2.4 is copyright 1993-1996 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Tartarus/doc/rom.license                  *
***************************************************************************/

/***************************************************************************
*       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
*	In using this code you agree to comply with the Tartarus license   *
*       found in the file /Tartarus/doc/tartarus.doc                       *
***************************************************************************/

#include "include.h"

/* for cabals */
const struct cabal_type cabal_table[MAX_CABAL] =
{
    /*  name,		who entry,	death-transfer room,   independent, open/closed */
    /* independent should be FALSE if is a real cabal */
    {   "",             "",              ROOM_VNUM_ALTAR,     TRUE , FALSE },
    {   "acadian",      "[ACADIAN] ",    ROOM_VNUM_ALTAR,     FALSE, FALSE },
    {   "protector",    "[PROTECTOR] ",  ROOM_VNUM_ALTAR,     FALSE, FALSE },
    {   "coven",        "[COVEN] ",      ROOM_VNUM_ALTAR,     FALSE, FALSE },
    {   "ravager",      "[RAVAGER] ",    ROOM_VNUM_ALTAR,     FALSE, FALSE },
    {   "vigilante",    "[VIGILANTE] ",  ROOM_VNUM_ALTAR,     FALSE, FALSE },
    {   "briar",        "[BRIAR] ",      ROOM_VNUM_ALTAR,     FALSE, FALSE },
};



const struct god_type god_table[MAX_GOD] =
{
	{	"",				"",						 },
	{	"Alisandra",	"Torture, Pain and Deceit" },
	{	"Ceial",		"Obediance and Humility" },
	{	"Aino",			"Love and Fire"			 },
	{	"Liriel",		"Earth and Nature"		 },
	{	"Gawinn",		"Pimpin?"				 },
	{	"Elocin",		"Mischief and Chaos"	 },
	{	"Kador",		"Scholarship and Magic"  },

};




/* for position */
const struct position_type position_table[] =
{
    {	"dead",			"dead"	},
    {	"mortally wounded",	"mort"	},
    {	"incapacitated",	"incap"	},
    {	"stunned",		"stun"	},
    {	"sleeping",		"sleep"	},
    {	"resting",		"rest"	},
    {   "sitting",		"sit"   },
    {	"fighting",		"fight"	},
    {	"standing",		"stand"	},
    {	NULL,			NULL	}
};

/* for sex */
const struct sex_type sex_table[] =
{
   {	"none"		},
   {	"male"		},
   {	"female"	},
   {	"either"	},
   {	NULL		}
};

/* for sizes */
const struct size_type size_table[] =
{
    {	"tiny"		},
    {	"small" 	},
    {	"medium"	},
    {	"large"		},
    {	"huge", 	},
    {	"giant" 	},
    {	NULL		}
};

/* various flag tables */
const struct flag_type act_flags[] =
{
    {	"npc",			A,	FALSE	},
    {	"sentinel",		B,	TRUE	},
    {	"scavenger",		C,	TRUE	},
    {	"aggressive",		F,	TRUE	},
    {	"stay_area",		G,	TRUE	},
    {	"wimpy",		H,	TRUE	},
    {	"pet",			I,	TRUE	},
    {	"train",		J,	TRUE	},
    {	"practice",		K,	TRUE	},
    {   "no_track",		L,	TRUE	},
    {	"undead",		O,	TRUE	},
    {	"cleric",		Q,	TRUE	},
    {	"mage",			R,	TRUE	},
    {	"thief",		S,	TRUE	},
    {	"warrior",		T,	TRUE	},
    {	"noalign",		U,	TRUE	},
    {	"nopurge",		V,	TRUE	},
    {	"outdoors",		W,	TRUE	},
    {	"indoors",		Y,	TRUE	},
    {	"healer",		aa,	TRUE	},
    {	"gain",			bb,	TRUE	},
    {	"update_always",	cc,	TRUE	},
    {	"changer",		dd,	TRUE	},
    {	NULL,			0,	FALSE	}
};

const struct flag_type plr_flags[] =
{
    {	"npc",			A,	FALSE	},
    {	"autoassist",		C,	FALSE	},
    {	"autoexit",		D,	FALSE	},
    {	"autoloot",		E,	FALSE	},
    {	"autosac",		F,	FALSE	},
    {	"autogold",		G,	FALSE	},
    {	"autosplit",		H,	FALSE	},
    {   "color",                I,      FALSE   },
    {   "betrayer",             K,      TRUE    },
    {   "heroimm",              M,      TRUE    },
    {	"holylight",		N,	FALSE	},
    {	"empowered",		O,	FALSE	},
    {	"can_loot",		P,	FALSE	},
    {	"nosummon",		Q,	FALSE	},
    {	"nofollow",		R,	FALSE	},
    {	"permit",		U,	TRUE	},
    {	"log",			W,	FALSE	},
    {	"deny",			X,	FALSE	},
    {	"freeze",		Y,	FALSE	},
    {	"thief",		Z,	FALSE	},
    {	"killer",		aa,	FALSE	},
    {	NULL,			0,	0	}
};

const struct flag_type affect_flags[] =
{
    {	"blind",		A,	TRUE	},
    {	"invisible",		B,	TRUE	},
    {	"detect_evil",		C,	TRUE	},
    {	"detect_invis",		D,	TRUE	},
    {	"detect_magic",		E,	TRUE	},
    {	"detect_hidden",	F,	TRUE	},
    {	"detect_good",		G,	TRUE	},
    {	"sanctuary",		H,	TRUE	},
    {	"faerie_fire",		I,	TRUE	},
    {	"infrared",		J,	TRUE	},
    {	"curse",		K,	TRUE	},
    {	"poison",		M,	TRUE	},
    {	"protect_evil",		N,	TRUE	},
    {	"protect_good",		O,	TRUE	},
    {	"sneak",		P,	TRUE	},
    {	"hide",			Q,	TRUE	},
    {	"sleep",		R,	TRUE	},
    {	"charm",		S,	TRUE	},
    {	"flying",		T,	TRUE	},
    {	"pass_door",		U,	TRUE	},
    {	"haste",		V,	TRUE	},
    {	"calm",			W,	TRUE	},
    {	"plague",		X,	TRUE	},
    {	"weaken",		Y,	TRUE	},
    {	"dark_vision",		Z,	TRUE	},
    {	"berserk",		aa,	TRUE	},
    {	"swim",			bb,	TRUE	},
    {	"regeneration",		cc,	TRUE	},
    {	"slow",			dd,	TRUE	},
    {	"camouflage",		ee,	TRUE	},
    {	NULL,			0,	0	}
};

const struct flag_type off_flags[] =
{
    {	"area_attack",		A,	TRUE	},
    {	"backstab",		B,	TRUE	},
    {	"bash",			C,	TRUE	},
    {	"berserk",		D,	TRUE	},
    {	"disarm",		E,	TRUE	},
    {	"dodge",		F,	TRUE	},
    {	"fade",			G,	TRUE	},
    {	"fast",			H,	TRUE	},
    {	"kick",			I,	TRUE	},
    {	"dirt_kick",		J,	TRUE	},
    {	"parry",		K,	TRUE	},
    {	"rescue",		L,	TRUE	},
    {	"tail",			M,	TRUE	},
    {	"trip",			N,	TRUE	},
    {	"crush",		O,	TRUE	},
    {	"assist_all",		P,	TRUE	},
    {	"assist_align",		Q,	TRUE	},
    {	"assist_race",		R,	TRUE	},
    {	"assist_players",	S,	TRUE	},
    {	"assist_guard",		T,	TRUE	},
    {	"assist_vnum",		U,	TRUE	},
    {	"parting_blow",		aa,	TRUE	},
    {	NULL,			0,	0	}
};

const struct flag_type imm_flags[] =
{
    {	"summon",		A,	TRUE	},
    {	"charm",		B,	TRUE	},
    {	"magic",		C,	TRUE	},
    {	"weapon",		D,	TRUE	},
    {	"bash",			E,	TRUE	},
    {	"pierce",		F,	TRUE	},
    {	"slash",		G,	TRUE	},
    {	"fire",			H,	TRUE	},
    {	"cold",			I,	TRUE	},
    {	"lightning",		J,	TRUE	},
    {	"acid",			K,	TRUE	},
    {	"poison",		L,	TRUE	},
    {	"negative",		M,	TRUE	},
    {	"holy",			N,	TRUE	},
    {	"energy",		O,	TRUE	},
    {	"mental",		P,	TRUE	},
    {	"disease",		Q,	TRUE	},
    {	"drowning",		R,	TRUE	},
    {	"light",		S,	TRUE	},
    {	"sound",		T,	TRUE	},
    {	"wood",			X,	TRUE	},
    {	"silver",		Y,	TRUE	},
    {	"iron",			Z,	TRUE	},
    {	NULL,			0,	0	}
};

const struct flag_type form_flags[] =
{
    {	"edible",		FORM_EDIBLE,		TRUE	},
    {	"poison",		FORM_POISON,		TRUE	},
    {	"magical",		FORM_MAGICAL,		TRUE	},
    {	"instant_decay",	FORM_INSTANT_DECAY,	TRUE	},
    {	"other",		FORM_OTHER,		TRUE	},
    {	"animal",		FORM_ANIMAL,		TRUE	},
    {	"sentient",		FORM_SENTIENT,		TRUE	},
    {	"undead",		FORM_UNDEAD,		TRUE	},
    {	"construct",		FORM_CONSTRUCT,		TRUE	},
    {	"mist",			FORM_MIST,		TRUE	},
    {	"intangible",		FORM_INTANGIBLE,	TRUE	},
    {	"biped",		FORM_BIPED,		TRUE	},
    {	"centaur",		FORM_CENTAUR,		TRUE	},
    {	"insect",		FORM_INSECT,		TRUE	},
    {	"spider",		FORM_SPIDER,		TRUE	},
    {	"crustacean",		FORM_CRUSTACEAN,	TRUE	},
    {	"worm",			FORM_WORM,		TRUE	},
    {	"blob",			FORM_BLOB,		TRUE	},
    {	"mammal",		FORM_MAMMAL,		TRUE	},
    {	"bird",			FORM_BIRD,		TRUE	},
    {	"reptile",		FORM_REPTILE,		TRUE	},
    {	"snake",		FORM_SNAKE,		TRUE	},
    {	"dragon",		FORM_DRAGON,		TRUE	},
    {	"amphibian",		FORM_AMPHIBIAN,		TRUE	},
    {	"fish",			FORM_FISH ,		TRUE	},
    {	"cold_blood",		FORM_COLD_BLOOD,	TRUE	},
    {	NULL,			0,			0	}
};

const struct flag_type part_flags[] =
{
    {	"head",			PART_HEAD,		TRUE	},
    {	"arms",			PART_ARMS,		TRUE	},
    {	"legs",			PART_LEGS,		TRUE	},
    {	"heart",		PART_HEART,		TRUE	},
    {	"brains",		PART_BRAINS,		TRUE	},
    {	"guts",			PART_GUTS,		TRUE	},
    {	"hands",		PART_HANDS,		TRUE	},
    {	"feet",			PART_FEET,		TRUE	},
    {	"fingers",		PART_FINGERS,		TRUE	},
    {	"ear",			PART_EAR,		TRUE	},
    {	"eye",			PART_EYE,		TRUE	},
    {	"long_tongue",		PART_LONG_TONGUE,	TRUE	},
    {	"eyestalks",		PART_EYESTALKS,		TRUE	},
    {	"tentacles",		PART_TENTACLES,		TRUE	},
    {	"fins",			PART_FINS,		TRUE	},
    {	"wings",		PART_WINGS,		TRUE	},
    {	"tail",			PART_TAIL,		TRUE	},
    {	"claws",		PART_CLAWS,		TRUE	},
    {	"fangs",		PART_FANGS,		TRUE	},
    {	"horns",		PART_HORNS,		TRUE	},
    {	"scales",		PART_SCALES,		TRUE	},
    {	"tusks",		PART_TUSKS,		TRUE	},
    {	NULL,			0,			0	}
};

const struct flag_type comm_flags[] =
{
    {	"quiet",		COMM_QUIET,		TRUE	},
    {	"nocast",		COMM_NOCAST,	TRUE	},
    {   "deaf",			COMM_DEAF,		TRUE	},
    {   "nowiz",		COMM_NOWIZ,		TRUE	},
    {   "noauction",            COMM_NOAUCTION,         TRUE    },
    {   "noquestion",		COMM_NOQUESTION,	TRUE	},
    {   "nocabal",              COMM_NOCABAL,           TRUE    },
    {   "noquote",		COMM_NOQUOTE,		TRUE	},
    {   "shoutsoff",		COMM_SHOUTSOFF,		TRUE	},
    {   "compact",		COMM_COMPACT,		TRUE	},
    {   "brief",		COMM_BRIEF,		TRUE	},
    {   "prompt",		COMM_PROMPT,		TRUE	},
    {   "combine",		COMM_COMBINE,		TRUE	},
    {   "telnet_ga",		COMM_TELNET_GA,		TRUE	},
    {   "show_affects",		COMM_SHOW_AFFECTS,	TRUE	},
    {   "noemote",		COMM_NOEMOTE,		FALSE	},
    {   "noshout",		COMM_NOSHOUT,		FALSE	},
    {   "notell",		COMM_NOTELL,		FALSE	},
    {   "nochannels",		COMM_NOCHANNELS,	FALSE	},
    {   "snoop_proof",		COMM_SNOOP_PROOF,	FALSE	},
    {   "afk",			COMM_AFK,		TRUE	},
    {   "olcedit",		COMM_OLCEDIT,		TRUE	},
    {	"mounted",		COMM_MOUNTED,		TRUE	},
    {	NULL,			0,			0	}
};

/***************************** OLC TABLES BELOW **********************************/

const struct flag_type area_flags[] =
 {
     {	"none",			AREA_NONE,		FALSE	},
     {	"changed",		AREA_CHANGED,		TRUE	},
     {	"added",		AREA_ADDED,		TRUE	},
     {	"loading",		AREA_LOADING,		FALSE	},
     {	"closed",		AREA_CLOSED,		TRUE	},
     {	NULL,			0,			0	}
 };



const struct flag_type sex_flags[] =
 {
     {	"male",			SEX_MALE,		TRUE	},
     {	"female",		SEX_FEMALE,		TRUE	},
     {	"neutral",		SEX_NEUTRAL,		TRUE	},
     {   "random",               3,                      TRUE    },   /* ROM */
     {	"none",			SEX_NEUTRAL,		TRUE	},
     {	NULL,			0,			0	}
 };



const struct flag_type exit_flags[] =
 {
     {   "door",			EX_ISDOOR,		TRUE    },
     {	"closed",		EX_CLOSED,		TRUE	},
     {	"locked",		EX_LOCKED,		TRUE	},
     {	"pickproof",		EX_PICKPROOF,		TRUE	},
     {   "nopass",		EX_NOPASS,		TRUE	},
     {   "easy",			EX_EASY,		TRUE	},
     {   "hard",			EX_HARD,		TRUE	},
     {	"infuriating",		EX_INFURIATING,		TRUE	},
     {	"noclose",		EX_NOCLOSE,		TRUE	},
     {	"nolock",		EX_NOLOCK,		TRUE	},
     {	NULL,			0,			0	}
 };



const struct flag_type door_resets[] =
 {
     {	"open and unlocked",	0,		TRUE	},
     {	"closed and unlocked",	1,		TRUE	},
     {	"closed and locked",	2,		TRUE	},
     {	NULL,			0,		0	}
 };



const struct flag_type room_flags[] =
 {
     {	"dark",			ROOM_DARK,		TRUE	},
     {	"no_mob",		ROOM_NO_MOB,		TRUE	},
     {	"indoors",		ROOM_INDOORS,		TRUE	},
     {	"private",		ROOM_PRIVATE,		TRUE    },
     {	"safe",			ROOM_SAFE,		TRUE	},
     {	"solitary",		ROOM_SOLITARY,		TRUE	},
     {	"pet_shop",		ROOM_PET_SHOP,		TRUE	},
     {	"no_recall",		ROOM_NO_RECALL,		TRUE	},
     {  "NO_SUMMON",		ROOM_NO_SUMMON,		TRUE	},
     {	"imp_only",		ROOM_IMP_ONLY,		TRUE    },
     {	"gods_only",	        ROOM_GODS_ONLY,		TRUE    },
     {	"heroes_only",		ROOM_HEROES_ONLY,	TRUE	},
     {	"newbie_only",		ROOM_NEWBIES_ONLY,	TRUE	},
     {	"law",			ROOM_LAW,		TRUE	},
     {   "nowhere",		ROOM_NOWHERE,		TRUE	},
     {	NULL,			0,			0	}
 };



const struct flag_type sector_flags[] =
 {
     {	"inside",	SECT_INSIDE,		TRUE	},
     {	"city",		SECT_CITY,		TRUE	},
     {	"field",	SECT_FIELD,		TRUE	},
     {	"forest",	SECT_FOREST,		TRUE	},
     {	"hills",	SECT_HILLS,		TRUE	},
     {	"mountain",	SECT_MOUNTAIN,		TRUE	},
     {	"swim",		SECT_WATER_SWIM,	TRUE	},
     {	"noswim",	SECT_WATER_NOSWIM,	TRUE	},
     {   "unused",	SECT_UNUSED,		TRUE	},
     {	"air",		SECT_AIR,		TRUE	},
     {	"desert",	SECT_DESERT,		TRUE	},
     {	NULL,		0,			0	}
 };



const struct flag_type type_flags[] =
 {
     {	"light",		ITEM_LIGHT,		TRUE	},
     {	"scroll",		ITEM_SCROLL,		TRUE	},
     {	"wand",			ITEM_WAND,		TRUE	},
     {	"staff",		ITEM_STAFF,		TRUE	},
     {	"weapon",		ITEM_WEAPON,		TRUE	},
     {	"treasure",		ITEM_TREASURE,		TRUE	},
     {	"armor",		ITEM_ARMOR,		TRUE	},
     {	"potion",		ITEM_POTION,		TRUE	},
     {	"furniture",		ITEM_FURNITURE,		TRUE	},
     {	"trash",		ITEM_TRASH,		TRUE	},
     {	"container",		ITEM_CONTAINER,		TRUE	},
     {	"drinkcontainer",	ITEM_DRINK_CON,		TRUE	},
     {	"key",			ITEM_KEY,		TRUE	},
     {	"food",			ITEM_FOOD,		TRUE	},
     {	"money",		ITEM_MONEY,		TRUE	},
     {	"boat",			ITEM_BOAT,		TRUE	},
     {	"npccorpse",		ITEM_CORPSE_NPC,	TRUE	},
     {	"pc corpse",		ITEM_CORPSE_PC,		FALSE	},
     {	"fountain",		ITEM_FOUNTAIN,		TRUE	},
     {	"pill",			ITEM_PILL,		TRUE	},
     {	"protect",		ITEM_PROTECT,		TRUE	},
     {	"map",			ITEM_MAP,		TRUE	},
     {   "portal",		ITEM_PORTAL,		TRUE	},
     {   "warpstone",		ITEM_WARP_STONE,	TRUE	},
     {	"roomkey",		ITEM_ROOM_KEY,		TRUE	},
     { 	"gem",			ITEM_GEM,		TRUE	},
     {	"jewelry",		ITEM_JEWELRY,		TRUE	},
     {	"jukebox",		ITEM_JUKEBOX,		TRUE	},
     {	NULL,			0,			0	}
 };


const struct flag_type extra_flags[] =
 {
     {	"glow",			ITEM_GLOW,		TRUE	},
     {	"hum",			ITEM_HUM,		TRUE	},
     {	"dark",			ITEM_DARK,		TRUE	},
     {	"lock",			ITEM_LOCK,		TRUE	},
     {	"evil",			ITEM_EVIL,		TRUE	},
     {	"invis",		ITEM_INVIS,		TRUE	},
     {	"magic",		ITEM_MAGIC,		TRUE	},
     {	"nodrop",		ITEM_NODROP,		TRUE	},
     {	"bless",		ITEM_BLESS,		TRUE	},
     {	"antigood",		ITEM_ANTI_GOOD,		TRUE	},
     {	"antievil",		ITEM_ANTI_EVIL,		TRUE	},
     {	"antineutral",		ITEM_ANTI_NEUTRAL,	TRUE	},
     {	"noremove",		ITEM_NOREMOVE,		TRUE	},
     {	"inventory",		ITEM_INVENTORY,		TRUE	},
     {	"nopurge",		ITEM_NOPURGE,		TRUE	},
     {	"rotdeath",		ITEM_ROT_DEATH,		TRUE	},
     {	"visdeath",		ITEM_VIS_DEATH,		TRUE	},
     {	"meltdrop",		ITEM_MELT_DROP,		TRUE	},
     {	"hadtimer",		ITEM_HAD_TIMER,		TRUE	},
     {	"sellextract",		ITEM_SELL_EXTRACT,	TRUE	},
     {	"burnproof",		ITEM_BURN_PROOF,	TRUE	},
     {	"nouncurse",		ITEM_NOUNCURSE,		TRUE	},
     {  "metal",            	ITEM_METAL,	        TRUE    },
     {	NULL,			0,			0	}
 };

const struct flag_type restrict_flags[] =
{
	{ "human_only",		HUMAN_ONLY,		TRUE	},
    	{ "dwarf_only",		DWARF_ONLY,		TRUE	},
    	{ "elf_only",		ELF_ONLY,		TRUE	},
    	{ "drow_only",		DARK_ONLY,		TRUE	},
    	{ "giant_only",		GIANT_ONLY,		TRUE	},
	{ "centuar_only",	CENTAUR_ONLY,		TRUE	},
	{ "troll_only",		TROLL_ONLY,		TRUE	},
	{ "draconian_only",	DRACONIAN_ONLY,		TRUE	},
	{ "gnome_only",		GNOME_ONLY,		TRUE	},
	{ "warrior_only",	WARRIOR_ONLY,		TRUE	},
	{ "thief_only",		THIEF_ONLY,		TRUE	},
	{ "priest_only",	PRIEST_ONLY,		TRUE	},
	{ "AP_only",		ANTI_PALADIN_ONLY,	TRUE	},
	{ "bard_only",		BARD_ONLY,		TRUE	},
	{ "ranger_only",	RANGER_ONLY,		TRUE	},
	{ "monk_only",		MONK_ONLY,		TRUE	},
	{ "transmuter_only",	TRANSMUTER_ONLY,	TRUE	},
	{ "assassin_only",	ASSASSIN_ONLY,		TRUE	},
	{ "necromancer_only",	NECROMANCER_ONLY,	TRUE	},
	{ "elementalist_only",	ELEMENTALIST_ONLY,	TRUE	},
	{ "ravager_only",		RAVAGER_ONLY,		TRUE	},
	{ "acadian_only",	ACADIAN_ONLY,		TRUE	},
	{ "mage_only",		MAGE_ONLY,		TRUE	},
	{ "coven_only",		COVEN_ONLY,		TRUE	},
	{ "protector_only",	PROTECTOR_ONLY,		TRUE	},
	{ "briar_only",		BRIAR_ONLY, 		TRUE	},
	{ "newbie_only",	NEWBIE_ONLY,		TRUE	},
	{ NULL,			0,			0	}
};

const struct flag_type wear_flags[] =
{
     {	"take",			ITEM_TAKE,		TRUE	},
     {	"finger",		ITEM_WEAR_FINGER,	TRUE	},
     {	"neck",			ITEM_WEAR_NECK,		TRUE	},
     {	"body",			ITEM_WEAR_BODY,		TRUE	},
     {	"head",			ITEM_WEAR_HEAD,		TRUE	},
     {	"legs",			ITEM_WEAR_LEGS,		TRUE	},
     {	"feet",			ITEM_WEAR_FEET,		TRUE	},
     {	"hands",		ITEM_WEAR_HANDS,	TRUE	},
     {	"arms",			ITEM_WEAR_ARMS,		TRUE	},
     {	"shield",		ITEM_WEAR_SHIELD,	TRUE	},
     {	"about",		ITEM_WEAR_ABOUT,	TRUE	},
     {	"waist",		ITEM_WEAR_WAIST,	TRUE	},
     {	"wrist",		ITEM_WEAR_WRIST,	TRUE	},
     {	"wield",		ITEM_WIELD,		TRUE	},
     {	"hold",			ITEM_HOLD,		TRUE	},
     {  "nosac",		ITEM_NO_SAC,		TRUE	},
     {	"wearfloat",		ITEM_WEAR_FLOAT,	TRUE	},
     {  "brand",		ITEM_WEAR_BRAND,	TRUE	},
     {	"bound",		ITEM_WEAR_BOUND,	TRUE	},
     {	NULL,			0,			0	}
};

 /*
  * Used when adding an affect to tell where it goes.
  * See addaffect and delaffect in act_olc.c
  */
 const struct flag_type apply_flags[] =
 {
     {	"none",			APPLY_NONE,		TRUE	},
     {	"strength",		APPLY_STR,		TRUE	},
     {	"dexterity",		APPLY_DEX,		TRUE	},
     {	"intelligence",		APPLY_INT,		TRUE	},
     {	"wisdom",		APPLY_WIS,		TRUE	},
     {	"constitution",		APPLY_CON,		TRUE	},
     {  "charisma",		APPLY_CHR,		TRUE	},
     {	"sex",			APPLY_SEX,		TRUE	},
     {	"class",		APPLY_CLASS,		TRUE	},
     {	"level",		APPLY_LEVEL,		TRUE	},
     {	"age",			APPLY_AGE,		TRUE	},
     {	"height",		APPLY_HEIGHT,		TRUE	},
     {	"weight",		APPLY_WEIGHT,		TRUE	},
     {	"mana",			APPLY_MANA,		TRUE	},
     {	"hp",			APPLY_HIT,		TRUE	},
     {	"move",			APPLY_MOVE,		TRUE	},
     {	"gold",			APPLY_GOLD,		TRUE	},
     {	"experience",		APPLY_EXP,		TRUE	},
     {	"ac",			APPLY_AC,		TRUE	},
     {	"hitroll",		APPLY_HITROLL,		TRUE	},
     {	"damroll",		APPLY_DAMROLL,		TRUE	},
     {	"saves",		APPLY_SAVES,		TRUE	},
     {	"savingpara",		APPLY_SAVING_PARA,	TRUE	},
     {	"savingrod",		APPLY_SAVING_ROD,	TRUE	},
     {	"savingpetri",		APPLY_SAVING_PETRI,	TRUE	},
     {	"savingbreath",		APPLY_SAVING_BREATH,	TRUE	},
     {	"savingspell",		APPLY_SAVING_SPELL,	TRUE	},
     {	"spellaffect",		APPLY_SPELL_AFFECT,	FALSE	},
     {	NULL,			0,			0	}
 };



 /*
  * What is seen.
  */
const struct flag_type wear_loc_strings[] =
 {
     {	"in the inventory",	WEAR_NONE,	TRUE	},
     {	"as a light",		WEAR_LIGHT,	TRUE	},
     {	"on the left finger",	WEAR_FINGER_L,	TRUE	},
     {	"on the right finger",	WEAR_FINGER_R,	TRUE	},
     {	"around the neck (1)",	WEAR_NECK_1,	TRUE	},
     {	"around the neck (2)",	WEAR_NECK_2,	TRUE	},
     {	"on the body",		WEAR_BODY,	TRUE	},
     {	"over the head",	WEAR_HEAD,	TRUE	},
     {	"on the legs",		WEAR_LEGS,	TRUE	},
     {	"on the feet",		WEAR_FEET,	TRUE	},
     {	"on the hands",		WEAR_HANDS,	TRUE	},
     {	"on the arms",		WEAR_ARMS,	TRUE	},
     {	"as a shield",		WEAR_SHIELD,	TRUE	},
     {	"about the shoulders",	WEAR_ABOUT,	TRUE	},
     {	"around the waist",	WEAR_WAIST,	TRUE	},
     {	"on the left wrist",	WEAR_WRIST_L,	TRUE	},
     {	"on the right wrist",	WEAR_WRIST_R,	TRUE	},
     {	"wielded",		WEAR_WIELD,	TRUE	},
     {	"held in the hands",	WEAR_HOLD,	TRUE	},
     {	"floating nearby",	WEAR_FLOAT,	TRUE	},
     {	NULL,			0	      , 0	}
 };


const struct flag_type wear_loc_flags[] =
 {
     {	"none",		WEAR_NONE,	TRUE	},
     {	"light",	WEAR_LIGHT,	TRUE	},
     {	"lfinger",	WEAR_FINGER_L,	TRUE	},
     {	"rfinger",	WEAR_FINGER_R,	TRUE	},
     {	"neck1",	WEAR_NECK_1,	TRUE	},
     {	"neck2",	WEAR_NECK_2,	TRUE	},
     {	"body",		WEAR_BODY,	TRUE	},
     {	"head",		WEAR_HEAD,	TRUE	},
     {	"legs",		WEAR_LEGS,	TRUE	},
     {	"feet",		WEAR_FEET,	TRUE	},
     {	"hands",	WEAR_HANDS,	TRUE	},
     {	"arms",		WEAR_ARMS,	TRUE	},
     {	"shield",	WEAR_SHIELD,	TRUE	},
     {	"about",	WEAR_ABOUT,	TRUE	},
     {	"waist",	WEAR_WAIST,	TRUE	},
     {	"lwrist",	WEAR_WRIST_L,	TRUE	},
     {	"rwrist",	WEAR_WRIST_R,	TRUE	},
     {	"wielded",	WEAR_WIELD,	TRUE	},
     {	"hold",		WEAR_HOLD,	TRUE	},
     {	"floating",	WEAR_FLOAT,	TRUE	},
     {	NULL,		0,		0	}
 };

const struct flag_type container_flags[] =
 {
     {	"closeable",		1,		TRUE	},
     {	"pickproof",		2,		TRUE	},
     {	"closed",		4,		TRUE	},
     {	"locked",		8,		TRUE	},
     {	"puton",		16,		TRUE	},
     {	NULL,			0,		0	}
 };

 /*****************************************************************************
                       ROM - specific tables:
  ****************************************************************************/




const struct flag_type ac_type[] =
 {
     {   "pierce",        AC_PIERCE,            TRUE    },
     {   "bash",          AC_BASH,              TRUE    },
     {   "slash",         AC_SLASH,             TRUE    },
     {   "exotic",        AC_EXOTIC,            TRUE    },
     {   NULL,              0,                    0       }
 };


const struct flag_type size_flags[] =
 {
     {   "tiny",          SIZE_TINY,            TRUE    },
     {   "small",         SIZE_SMALL,           TRUE    },
     {   "medium",        SIZE_MEDIUM,          TRUE    },
     {   "large",         SIZE_LARGE,           TRUE    },
     {   "huge",          SIZE_HUGE,            TRUE    },
     {   "giant",         SIZE_GIANT,           TRUE    },
     {   NULL,              0,                    0       },
 };


const struct flag_type weapon_class[] =
 {
     {   "exotic",	WEAPON_EXOTIC,		TRUE    },
     {   "sword",	WEAPON_SWORD,		TRUE    },
     {   "dagger",	WEAPON_DAGGER,		TRUE    },
     {   "spear",	WEAPON_SPEAR,		TRUE    },
     {   "mace",	WEAPON_MACE,		TRUE    },
     {   "axe",		WEAPON_AXE,		TRUE    },
     {   "flail",	WEAPON_FLAIL,		TRUE    },
     {   "whip",	WEAPON_WHIP,		TRUE    },
     {   "polearm",	WEAPON_POLEARM,		TRUE    },
     {	 "staff",	WEAPON_STAFF,		TRUE	},
     {   NULL,		0,			0       }
 };


const struct flag_type weapon_type2[] =
 {
     {   "flaming",       WEAPON_FLAMING,       TRUE    },
     {   "frost",         WEAPON_FROST,         TRUE    },
     {   "vampiric",      WEAPON_VAMPIRIC,      TRUE    },
     {   "sharp",         WEAPON_SHARP,         TRUE    },
     {   "vorpal",        WEAPON_VORPAL,        TRUE    },
     {   "twohands",     WEAPON_TWO_HANDS,     TRUE    },
     {	"shocking",	 WEAPON_SHOCKING,      TRUE    },
     {	"poison",	WEAPON_POISON,		TRUE	},
     {	"leader",	WEAPON_LEADER,		TRUE	},
     {   NULL,              0,                    0       }
 };

const struct flag_type res_flags[] =
 {
     {	"summon",	 RES_SUMMON,		TRUE	},
     {   "charm",         RES_CHARM,            TRUE    },
     {   "magic",         RES_MAGIC,            TRUE    },
     {   "weapon",        RES_WEAPON,           TRUE    },
     {   "bash",          RES_BASH,             TRUE    },
     {   "pierce",        RES_PIERCE,           TRUE    },
     {   "slash",         RES_SLASH,            TRUE    },
     {   "fire",          RES_FIRE,             TRUE    },
     {   "cold",          RES_COLD,             TRUE    },
     {   "lightning",     RES_LIGHTNING,        TRUE    },
     {   "acid",          RES_ACID,             TRUE    },
     {   "poison",        RES_POISON,           TRUE    },
     {   "negative",      RES_NEGATIVE,         TRUE    },
     {   "holy",          RES_HOLY,             TRUE    },
     {   "energy",        RES_ENERGY,           TRUE    },
     {   "mental",        RES_MENTAL,           TRUE    },
     {   "disease",       RES_DISEASE,          TRUE    },
     {   "drowning",      RES_DROWNING,         TRUE    },
     {   "light",         RES_LIGHT,            TRUE    },
     {	"sound",	RES_SOUND,		TRUE	},
     {	"wood",		RES_WOOD,		TRUE	},
     {	"silver",	RES_SILVER,		TRUE	},
     {	"iron",		RES_IRON,		TRUE	},
     {   NULL,          0,            0    }
 };


const struct flag_type vuln_flags[] =
 {
     {	"summon",	 VULN_SUMMON,		TRUE	},
     {	"charm",	VULN_CHARM,		TRUE	},
     {   "magic",         VULN_MAGIC,           TRUE    },
     {   "weapon",        VULN_WEAPON,          TRUE    },
     {   "bash",          VULN_BASH,            TRUE    },
     {   "pierce",        VULN_PIERCE,          TRUE    },
     {   "slash",         VULN_SLASH,           TRUE    },
     {   "fire",          VULN_FIRE,            TRUE    },
     {   "cold",          VULN_COLD,            TRUE    },
     {   "lightning",     VULN_LIGHTNING,       TRUE    },
     {   "acid",          VULN_ACID,            TRUE    },
     {   "poison",        VULN_POISON,          TRUE    },
     {   "negative",      VULN_NEGATIVE,        TRUE    },
     {   "holy",          VULN_HOLY,            TRUE    },
     {   "energy",        VULN_ENERGY,          TRUE    },
     {   "mental",        VULN_MENTAL,          TRUE    },
     {   "disease",       VULN_DISEASE,         TRUE    },
     {   "drowning",      VULN_DROWNING,        TRUE    },
     {   "light",         VULN_LIGHT,           TRUE    },
     {	"sound",	 VULN_SOUND,		TRUE	},
     {   "wood",          VULN_WOOD,            TRUE    },
     {   "silver",        VULN_SILVER,          TRUE    },
     {   "iron",          VULN_IRON,            TRUE    },
     {   NULL,              0,                    0       }
 };

 const struct flag_type position_flags[] =
 {
     {   "dead",           POS_DEAD,            FALSE   },
     {   "mortal",         POS_MORTAL,          FALSE   },
     {   "incap",          POS_INCAP,           FALSE   },
     {   "stunned",        POS_STUNNED,         FALSE   },
     {   "sleeping",       POS_SLEEPING,        TRUE    },
     {   "resting",        POS_RESTING,         TRUE    },
     {   "sitting",        POS_SITTING,         TRUE    },
     {   "fighting",       POS_FIGHTING,        FALSE   },
     {   "standing",       POS_STANDING,        TRUE    },
     {   NULL,              0,                    0       }
 };

const struct flag_type portal_flags[] =
{
     {   "normal_exit",	  GATE_NORMAL_EXIT,	TRUE	},
     {	"no_curse",	  GATE_NOCURSE,		TRUE	},
     {   "go_with",	  GATE_GOWITH,		TRUE	},
     {   "buggy",	  GATE_BUGGY,		TRUE	},
     {	"random",	  GATE_RANDOM,		TRUE	},
     {   NULL,		  0,			0	}
 };

const struct flag_type furniture_flags[] =
{
     {   "stand_at",	  STAND_AT,		TRUE	},
     {	"stand_on",	  STAND_ON,		TRUE	},
     {	"stand_in",	  STAND_IN,		TRUE	},
     {	"sit_at",	  SIT_AT,		TRUE	},
     {	"sit_on",	  SIT_ON,		TRUE	},
     {	"sit_in",	  SIT_IN,		TRUE	},
     {	"rest_at",	  REST_AT,		TRUE	},
     {	"rest_on",	  REST_ON,		TRUE	},
     {	"rest_in",	  REST_IN,		TRUE	},
     {	"sleep_at",	  SLEEP_AT,		TRUE	},
     {	"sleep_on",	  SLEEP_ON,		TRUE	},
     {	"sleep_in",	  SLEEP_IN,		TRUE	},
     {	"put_at",	  PUT_AT,		TRUE	},
     {	"put_on",	  PUT_ON,		TRUE	},
     {	"put_in",	  PUT_IN,		TRUE	},
     {	"put_inside",	  PUT_INSIDE,		TRUE	},
     {	NULL,		  0,			0	}
 };

const struct flag_type     apply_types [] =
{
        {	"affects",	TO_AFFECTS,	TRUE	},
 	{	"object",	TO_OBJECT,	TRUE	},
 	{	"immune",	TO_IMMUNE,	TRUE	},
 	{	"resist",	TO_RESIST,	TRUE	},
 	{	"vuln",		TO_VULN,	TRUE	},
 	{	"weapon",	TO_WEAPON,	TRUE	},
 	{	NULL,		0,		TRUE	}
};

const struct bit_type bitvector_type [] =
{
    	{    	affect_flags,  	"affect"        },
 	{	apply_flags,	"apply"		},
 	{	imm_flags,	"imm"		},
 	{	res_flags,	"res"		},
 	{	vuln_flags,	"vuln"		},
 	{	weapon_type2,	"weapon"	}
};

const struct empire_type empire_table[MAX_EMPIRE] =
{
    /*  name,		who entry  */
    {   "anathema",	"[ANATHEMA] "	},
    {   "nonempire",	""	},
    {   "prebloodoath",	""	},
    {   "bloodoath",	"[BLOODOATH] "	},
    {   "citizen",	"[CITIZEN] "	},
    {   "leader",	"[LEADER] "	},
    {   "emperor",	"[EMPEROR] "	},
    {   "immortal",	""		}

};

const struct color_type color_table[MAX_COLORS] =
{
	/* Color name	Associated Ascii Value */
	{"uncolored",	"\x01B[0m" 		},
	{"red",			"\x01B[0;31m" 	},
    {"green",		"\x01B[0;32m" 	},
    {"yellow",		"\x01B[0;33m" 	},
	{"blue",		"\x01B[0;34m" 	},
	{"magenta",		"\x01B[0;35m" 	},
	{"cyan",		"\x01B[0;36m" 	},
	{"lightgrey",	"\x01B[0;37m" 	},
	{"darkgrey",	"\x01B[1;30m" 	},
	{"lightred",	"\x01B[1;31m" 	},
	{"lightgreen",	"\x01B[1;32m" 	},
	{"lightyellow",	"\x01B[1;33m" 	},
	{"lightblue",	"\x01B[1;34m" 	},
	{"lightmagenta","\x01B[1;35m" 	},
	{"lightcyan",	"\x01B[1;36m" 	},
	{"white",		"\x01B[1;37m" 	},
};
const struct color_event_type color_event[MAX_EVENTS] =
{
	/* Event Name		Default Color   Minimum Level */
        {"prompt",		"uncolored",	1  },
		{"channels",	"darkgrey",		1  },
		{"tells",		"lightgreen",	1  },
		{"grouptells",	"magenta",		1  },
		{"speech",		"lightyellow",	1  },
		{"damage",		"lightred",		1  },
		{"yells",		"lightcyan",	1  },
		{"immtalk",		"lightblue",	52 },
		{"wiznet",		"uncolored",	52 },
		{"prays",		"lightred",		53 },
};

const struct flag_type extended_flags[] =
{
    {	"cheap_shot",		A,	TRUE	},
    {	"parting_blow",		B,	TRUE	},
    {	"assassin",		C,	TRUE	},
    {	"sword_spec",		D,	TRUE	},
    {	"axe_spec",		E,	TRUE	},
    {	"hth_spec",		F,	TRUE	},
    {	"dagger_spec",		G,	TRUE	},
    {	"spear_spec",		H,	TRUE	},
    {	"mace_spec",		I,	TRUE	},
    {	"whip_spec",		J,	TRUE	},
    {	"polearm_spec",		K,	TRUE	},
};

const struct flag_type mprog_flags[] =
{
    {	"act",			TRIG_ACT,		TRUE	},
    {	"bribe",		TRIG_BRIBE,		TRUE 	},
    {	"death",		TRIG_DEATH,		TRUE    },
    {	"entry",		TRIG_ENTRY,		TRUE	},
    {	"fight",		TRIG_FIGHT,		TRUE	},
    {	"give",			TRIG_GIVE,		TRUE	},
    {	"greet",		TRIG_GREET,		TRUE    },
    {	"grall",		TRIG_GRALL,		TRUE	},
    {	"kill",			TRIG_KILL,		TRUE	},
    {	"hpcnt",		TRIG_HPCNT,		TRUE    },
    {	"random",		TRIG_RANDOM,		TRUE	},
    {	"speech",		TRIG_SPEECH,		TRUE	},
    {	"exit",			TRIG_EXIT,		TRUE    },
    {	"exall",		TRIG_EXALL,		TRUE    },
    {	"delay",		TRIG_DELAY,		TRUE    },
    {	"surr",			TRIG_SURR,		TRUE    },
    {   NULL,                   0,                      0       }
};

/* New Affect Types */

const struct flag_type aftype_table[] =
{
    {   "AFT_SPELL",    AFT_SPELL,              TRUE    },
    {   "AFT_SKILL",    AFT_SKILL,              TRUE    },
    {   "AFT_POWER",    AFT_POWER,              TRUE    },
    {   "AFT_MALADY",   AFT_MALADY,             TRUE    },
    {   "AFT_COMMUNE",  AFT_COMMUNE,    	TRUE    },
    {   "AFT_INVIS",    AFT_INVIS,              TRUE    },
    {   "AFT_SONG",     AFT_SONG,               TRUE    },
	{	"AFT_HIDDEN",	AFT_HIDDEN,				TRUE	},
    {	NULL,		0,			FALSE	},
};

const struct display_type apply_locations[] =
{
    {   "none",			APPLY_NONE,			"none"				},
    {	"str",			APPLY_STR,     		"strength"			},
    {	"dex",   		APPLY_DEX,     		"dexterity"			},
    { 	"int",	 		APPLY_INT,    	 	"intelligence"		},
    {   "wis",   		APPLY_WIS,     		"wisdom"			},
    {   "con",   		APPLY_CON,     		"constitution"		},
    {   "cha",			APPLY_CHR,		"charisma"		},
    {	"sex",   		APPLY_SEX,     		"sex"				},
    {	"class", 		APPLY_CLASS,   		"class"				},
    {   "age",  	 	APPLY_AGE,     		"age"				},
    {   "height",		APPLY_HEIGHT,     	"none"				},
    {   "weight",		APPLY_WEIGHT,     	"none"				},
    {   "mana",  		APPLY_MANA,     	"mana"				},
    {   "hp",			APPLY_HIT,     		"health"			},
    {   "move",  		APPLY_MOVE,     	"moves"				},
    {   "gold",	 		APPLY_GOLD,     	"none"				},
    {   "exp",	 		APPLY_EXP,     		"none"				},
    {	"ac",			APPLY_AC,     		"armor class"		},
    {   "hitroll",		APPLY_HITROLL,   	"hitroll"  			},
	{	"damroll",		APPLY_DAMROLL,      "damroll"			},
    {	"saves",		APPLY_SAVES,        "saves"				},
    {	"spara",		APPLY_SAVING_PARA,  "save vs paralysis"	},
    {	"srod",			APPLY_SAVING_ROD,   "save vs rod"		},
    {	"spetri",		APPLY_SAVING_PETRI, "save vs petrification"},
    {	"sbreath",		APPLY_SAVING_BREATH,"save vs breath"	},
    {   "svspell",		APPLY_SAVING_SPELL, "save vs spell" 	},
    {	"affect",		APPLY_SPELL_AFFECT, "none"				},
    {	"regen",		APPLY_REGENERATION, "regeneration"  	},
    {   NULL,			0,					FALSE		},
};
