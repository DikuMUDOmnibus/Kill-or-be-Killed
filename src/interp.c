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

bool	check_social	args( ( CHAR_DATA *ch, char *command,
			    char *argument ) );

/*
 * Command logging types.
 */
#define LOG_NORMAL	0
#define LOG_ALWAYS	1
#define LOG_NEVER	2


DECLARE_DO_FUN( do_prep );
DECLARE_DO_FUN( do_weaponbreak );
DECLARE_DO_FUN( do_unread );
DECLARE_DO_FUN(	do_ranger_staff	);

/*
 * Log-all switch.
 */
bool				fLogAll		= FALSE;



/*
 * Command table.
 */
const	struct	cmd_type	cmd_table	[] =
{
    /*
     * Common movement commands.
     */
    { "north",		do_north,	POS_STANDING,    0,  LOG_NEVER, 0, 0 },
    { "east",		do_east,	POS_STANDING,	 0,  LOG_NEVER, 0, 0 },
    { "south",		do_south,	POS_STANDING,	 0,  LOG_NEVER, 0, 0 },
    { "west",		do_west,	POS_STANDING,	 0,  LOG_NEVER, 0, 0 },
    { "up",		    do_up,		POS_STANDING,	 0,  LOG_NEVER, 0, 0 },
    { "down",		do_down,	POS_STANDING,	 0,  LOG_NEVER, 0, 0 },

    /*
     * Common other commands.
     * Placed here so one and two letter abbreviations work.
     */
    { "affects",	do_affects,	POS_DEAD,	0 ,  LOG_NORMAL, 1, 0 },
    { "cast",		do_cast,	POS_FIGHTING,   0,   LOG_NORMAL, 1, 1 },
    { "call",		do_call,	POS_FIGHTING,	0,   LOG_NORMAL, 1, 1 },
    { "commune",	do_commune,	POS_FIGHTING,	0,   LOG_NORMAL, 1, 1 },
    { "sing",		do_sing,	POS_FIGHTING,	0,   LOG_NORMAL, 1, 1 },
    { "auction",    	do_auction,     POS_RESTING,    0,   LOG_NORMAL, 1, 1 },
    { "buy",		do_buy,		POS_RESTING,    0,   LOG_NORMAL, 1, 1 },
    { "channels",   	do_channels,    POS_DEAD,       0,   LOG_NORMAL, 1, 0 },
    { "exits",		do_exits,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { "get",		do_get,		POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "goto",           do_goto,        POS_DEAD,       L8,  LOG_NORMAL, 1, 0 },
    { "group",          do_group,       POS_SLEEPING,    0,  LOG_NORMAL, 1, 0 },
    { "balance",	do_balance,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "deposit",	do_deposit,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "withdraw",	do_withdraw,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "ratings",	do_ratings,     POS_DEAD, 	 0,  LOG_NORMAL,1,0},
    { "hit",		do_hit,		POS_FIGHTING,	 0,  LOG_NORMAL, 0, 1 },
    { "l",              do_look,        POS_RESTING,     0,  LOG_NORMAL, 1, 0 },
    { "lo",             do_look,        POS_RESTING,     0,  LOG_NORMAL, 1, 0 },
    { "look",           do_look,        POS_RESTING,     0,  LOG_NORMAL, 1, 0 },
    { "lore",           do_lore,        POS_DEAD,        0,  LOG_NORMAL, 1, 0 },
    { "leader",         do_leader,      POS_DEAD,       L6,  LOG_ALWAYS, 1, 0 },
    { "i",		do_inventory,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "in",		do_inventory,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "inventory",	do_inventory,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "induct",         do_induct,      POS_DEAD,        0,  LOG_ALWAYS, 1, 0 },
    { "leaderpeek",     do_leaderpeek,  POS_DEAD,        0,  LOG_ALWAYS, 1, 0 },
    { "kill",		do_kill,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, 1 },
    { "cb",             do_cb,          POS_SLEEPING,    0,  LOG_NORMAL, 1, 0},
    { "enter",          do_enter,      POS_STANDING,    0,  LOG_NORMAL, 1, 1 },
    { "/",              do_recall,      POS_STANDING,    0,  LOG_NORMAL, 1, 1 },
    { "order",		do_order,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "practice",       do_practice,	POS_SLEEPING,    0,  LOG_NORMAL, 1, 0 },
    { "pray",           do_pray,        POS_SLEEPING,    0,  LOG_ALWAYS, 1, 0 },
	{ "wake",		do_wake,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, 0 },
    { "rest",		do_rest,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, 0 },
    { "sit",		do_sit,		POS_SLEEPING,    0,  LOG_NORMAL, 1, 0 },
    { "sockets",        do_sockets,	POS_DEAD,       57,  LOG_NORMAL, 1, 0 },
    { "history",        do_history,     POS_DEAD,       52,  LOG_NORMAL, 1, 0 },
    {"extitle", do_extitle, POS_DEAD, L5, LOG_NORMAL, 1, 0},
    {"multicheck", do_multicheck, POS_DEAD, L5, LOG_NORMAL, 1, 0},
    {"awareness", do_awareness, POS_RESTING,0,LOG_NORMAL, 1, 1},
    {"blending", do_forest_blending, POS_STANDING, 0, LOG_NORMAL, 1, 0},
    { "stand",		do_stand,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, 0 },
    { "tell",		do_tell,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "unlock",         do_unlock,      POS_RESTING,     0,  LOG_NORMAL, 1, 1 },
    { "wield",		do_wear,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "wizhelp",	do_wizhelp,	POS_DEAD,	IM,  LOG_NORMAL, 1, 0 },
    { "wizcheck",   do_wizcheck, POS_DEAD,  MAX_LEVEL, LOG_NORMAL, 1, 0 },
    { "request", do_request, POS_STANDING, 0, LOG_NORMAL, 1, 1},
    { "demand",	do_demand, POS_STANDING, 0 ,LOG_NORMAL, 1, 1},
    { "ooctell",        do_ooctell,     POS_SLEEPING,    0,  LOG_NORMAL, 1, 0},
    { "worship",	do_worship,		POS_DEAD,	0, LOG_ALWAYS,	1, 0 },
    /*
     * Informational commands.
     */
    { "ability",        do_ability,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "areas",		do_areas,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "bug",		do_bug,		POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "changes",	do_changes,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "compare",	do_compare,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { "consider",	do_consider,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { "count",		do_count,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, 0 },
    { "credits",	do_credits,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "equipment",	do_equipment,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "examine",	do_examine,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { "glance",         do_glance,    	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { "help",		do_help,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "idea",		do_idea,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "info",           do_groups,      POS_SLEEPING,    0,  LOG_NORMAL, 1, 0 },
    { "motd",		do_motd,	POS_DEAD,        0,  LOG_NORMAL, 1, 0 },
    { "news",		do_news,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "powers",		do_powers,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "read",		do_look,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { "rehearsal",	do_rehearsal,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "report",		do_report,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "rules",		do_rules,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "score",		do_score,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "skills",		do_skills,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "socials",	do_socials,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "show",		do_show,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "spells",		do_spells,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "story",		do_story,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "supplications",	do_supps,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "time",		do_time,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "typo",		do_typo,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "unread",		do_unread,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "weather",	do_weather,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { "who",		do_who,		POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "whois",		do_whois,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "wizlist",	do_wizlist,	POS_DEAD,        0,  LOG_NORMAL, 1, 0 },
    { "worth",		do_worth,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, 0 },

    /*
     * Configuration commands.
     */
    { "alia",		do_alia,	POS_DEAD,	 0,  LOG_NORMAL, 0, 0 },
    { "alias",		do_alias,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "autolist",	do_autolist,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "evaluate", do_evaluation, POS_DEAD, 0, LOG_NORMAL, 1, 0},
    { "notransfer", do_notransfer, POS_SLEEPING, 0, LOG_NORMAL, 1, 1},
    { "autoassist",	do_autoassist,	POS_DEAD,        0,  LOG_NORMAL, 1, 0 },
    { "autoexit",	do_autoexit,	POS_DEAD,        0,  LOG_NORMAL, 1, 0 },
    { "autogold",	do_autogold,	POS_DEAD,        0,  LOG_NORMAL, 1, 0 },
    { "autoloot",	do_autoloot,	POS_DEAD,        0,  LOG_NORMAL, 1, 0 },
    { "autosac",	do_autosac,	POS_DEAD,        0,  LOG_NORMAL, 1, 0 },
    { "autosplit",	do_autosplit,	POS_DEAD,        0,  LOG_NORMAL, 1, 0 },
    { "brief",		do_brief,	POS_DEAD,        0,  LOG_NORMAL, 1, 0 },
/*  { "channels",	do_channels,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 }, */
    { "color",		do_color,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "combine",	do_combine,	POS_DEAD,        0,  LOG_NORMAL, 1, 0 },
    { "compact",	do_compact,	POS_DEAD,        0,  LOG_NORMAL, 1, 0 },
    { "description",	do_description,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "delet",		do_delet,	POS_DEAD,	 0,  LOG_ALWAYS, 0, 0 },
    { "delete",		do_delete,	POS_STANDING,	 0,  LOG_ALWAYS, 1, 0 },
    { "nofollow",	do_nofollow,	POS_DEAD,        0,  LOG_NORMAL, 1, 0 },
    { "nosummon",	do_nosummon,	POS_DEAD,        0,  LOG_NORMAL, 1, 0 },
    { "outfit",		do_outfit,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { "password",	do_password,	POS_DEAD,	 0,  LOG_NEVER,  1, 0 },
    { "prompt",		do_prompt,	POS_DEAD,        0,  LOG_NORMAL, 1, 0 },
    { "scroll",		do_scroll,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "title",		do_title,	POS_DEAD,	 IM,  LOG_NORMAL, 1, 0 },
    { "unalias",	do_unalias,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "wimpy",		do_wimpy,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },

    /*
     * Communication commands.
     */
    { "answer",		do_answer,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
/*  { "auction",	do_auction,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, 1 }, */
    { "deaf",		do_deaf,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "emote",		do_emote,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { "pmote",		do_pmote,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { ",",		do_emote,	POS_RESTING,	 0,  LOG_NORMAL, 0, 0 },
    { "gtell",		do_gtell,	POS_DEAD,	 0,  LOG_NORMAL, 1, 1 },
    { ";",		do_gtell,	POS_DEAD,	 0,  LOG_NORMAL, 0, 1 },
    { "immtalk",	do_immtalk,	POS_DEAD,	0,  LOG_NORMAL, 1,0 },
    { "note",		do_note,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, 0 },
    { "pose",		do_pose,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { "newbie",         do_newbie,      POS_DEAD,        0,  LOG_NEVER,  1, 0 },
    { "question",	do_question,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "quiet",		do_quiet,	POS_SLEEPING, 	 0,  LOG_NORMAL, 1, 0 },
    { "reply",		do_reply,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "replay",		do_replay,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, 0 },
    { "say",		do_say,		POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "'",		do_say,		POS_RESTING,	 0,  LOG_NORMAL, 0, 1 },
    { "whisper",	do_whisper,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { "[",		do_whisper,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { "yell",		do_yell,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },

    /*
     * Object manipulation commands.
     */
    { "brandish",	do_brandish,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "close",		do_close,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "shadowform", do_shadowform, POS_RESTING, 0, LOG_NORMAL, 1, 0 },
    { "drink",		do_drink,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "drop",		do_drop,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { "embalm", do_embalm, POS_STANDING, 0 ,LOG_NORMAL, 0, 1},
    { "eat",		do_eat,		POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "empower",	do_empower,	POS_DEAD,        L5, LOG_ALWAYS, 1, 0 },
    { "envenom",	do_envenom,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "fill",		do_fill,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "forge",		do_forge,	POS_RESTING,	 0,	 LOG_NORMAL, 1, 1 },
    { "give",		do_give,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "heal",		do_heal,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { "hold",		do_wear,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "list",		do_list,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { "lock",		do_lock,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "open",		do_open,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "pick",		do_pick,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "pour",		do_pour,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "put",		do_put,		POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "quaff",		do_quaff,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "recite",		do_recite,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "remove",		do_remove,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "door", do_door_bash, POS_FIGHTING, 0, LOG_NORMAL, 1, 1},
    { "sell",		do_sell,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "take",		do_get,		POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "sacrifice",	do_sacrifice,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "junk",           do_sacrifice,   POS_RESTING,     0,  LOG_NORMAL, 0, 1 },
    { "butcher",	do_butcher,	POS_STANDING,	 0,  LOG_NORMAL, 0, 1 },
    { "skin",		do_skin,	POS_STANDING,	 0,  LOG_NORMAL, 0, 1 },
    { "devour",      	do_devour,      POS_RESTING,     0,  LOG_NORMAL, 0, 1 },
    { "value",		do_value,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { "wear",		do_wear,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "zap",		do_zap,		POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    {"aura", do_aura_of_sustenance, POS_RESTING, 0 , LOG_NORMAL, 1, 1},
    /*
     * Combat commands.
     */


    { "track",	do_acadian_track, POS_STANDING, 0, LOG_NORMAL, 1, 0 },
    { "siphon", do_siphon, POS_STANDING, 0, LOG_NORMAL, 1, 0 },
	{ "mount",  do_mount, POS_STANDING, 0, LOG_NORMAL, 1, 0 },
	{ "dismount", do_dismount, POS_FIGHTING, 0, LOG_NORMAL, 1, 0 },
	{ "steed", do_steed, POS_STANDING, 0, LOG_NORMAL, 1, 0 },

    { "weaponbreaker", do_weaponbreak, POS_FIGHTING, 0, LOG_NORMAL, 1, 0},
    { "siton", do_siton, POS_STANDING, 0, LOG_NORMAL, 1, 0},
    { "drum", do_drum, POS_FIGHTING, 0, LOG_NORMAL, 1, 0},
    { "flurry", do_flurry, POS_FIGHTING, 0, LOG_NORMAL, 1, 0},
    { "decapitate", do_decapitate, POS_FIGHTING, 0, LOG_NORMAL, 1, 0},
    { "cleave",	do_cleave, POS_STANDING, 0, LOG_NORMAL, 1, 0},
    { "claymore", do_claymore, POS_STANDING, 0, LOG_NORMAL, 1, 0 },
    { "firstaid", do_firstaid, POS_FIGHTING, 0, LOG_NORMAL, 1, 0 },
    { "herb", do_herb, POS_STANDING, 0, LOG_NORMAL, 1, 0},
    { "shackles", do_shackles, POS_FIGHTING, 0, LOG_NORMAL, 1, 0},
    { "records", do_records, POS_SLEEPING,0,LOG_NORMAL,0,0},
    { "deathstrike", do_deathstrike, POS_STANDING, 0 , LOG_NORMAL, 1, 0},
    { "spike", do_spike, POS_STANDING,25,LOG_NORMAL,0,0},
    { "ambush",		do_ambush,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, 1 },
    { "pugil", do_pugil, POS_FIGHTING, 0, LOG_NORMAL, 1, 1},
    { "lash", do_lash, POS_FIGHTING, 0, LOG_NORMAL, 1, 1},
    { "shieldbash", do_shieldbash,	POS_FIGHTING, 0, LOG_NORMAL, 1, 1},
    { "shieldcleave", do_shield_cleave,	POS_FIGHTING, 0, LOG_NORMAL, 1, 1},
    { "backstab",	do_backstab,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, 1 },
    { "knife",	do_knife,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, 1 },
    { "bash",		do_bash,	POS_FIGHTING,    0,  LOG_NORMAL, 1, 1 },
    { "bearcharge", do_bearcharge,    POS_FIGHTING,    0,  LOG_NORMAL, 1, 1 },
    { "throw",	do_throw,	POS_FIGHTING,	0,	LOG_NORMAL,	1, 1},
    { "strangle", do_strangle, POS_STANDING, 0, LOG_NORMAL, 1, 0},
    { "blindness", do_blindness_dust, POS_FIGHTING, 0, LOG_NORMAL, 1, 1},
    { "poisondust", do_poison_dust, POS_FIGHTING, 0, LOG_NORMAL, 1, 1},
    { "warcry", do_warcry, POS_FIGHTING, 0, LOG_NORMAL, 1, 1},
    { "prepatation", do_prep,        POS_FIGHTING,   0,      LOG_NORMAL, 1, 1 },
    { "insectswarm",   do_insectswarm,        POS_STANDING,   0,      LOG_NORMAL,     1, 1 },
    { "palm",	do_palm,	POS_FIGHTING,	0,	LOG_NORMAL,	1, 1 },
    { "nerve",	do_nerve,	POS_FIGHTING,	0,	LOG_NORMAL,	1, 1},
    { "ram",	do_ram,	POS_FIGHTING, 0, LOG_NORMAL,	0, 1},
    { "bite",	do_bite, POS_STANDING, 0, LOG_NORMAL, 0, 1},
    { "tail", do_tail, POS_FIGHTING, 0, LOG_NORMAL, 0, 1},
    { "battlecry",	do_battlecry,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, 1 },
    { "battlepaint", do_battlepaint, POS_FIGHTING, 0, LOG_NORMAL, 1, 1 },
    { "bloodthirst",	do_bloodthirst,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, 1 },
    { "spellbane", do_spellbane, POS_FIGHTING, 0, LOG_NORMAL, 0 ,1},
    { "battleshield",   do_battleshield,POS_STANDING, 0, LOG_NORMAL, 1, 1 },
    { "trophy",	do_trophy, POS_RESTING, 0, LOG_NORMAL, 1, 1},
    { "blackjack",	do_blackjack,	POS_STANDING,	 0,  LOG_NORMAL, 1, 0 },
    { "bobbit",	do_bobbit,	POS_STANDING,	 0,  LOG_NORMAL, 1, 1 },
    { "impale",	do_impale,	POS_STANDING,	 0,  LOG_NORMAL, 1, 1 },
    { "bj", do_blackjack, POS_STANDING, 0, LOG_NORMAL, 0, 0 },
    { "breath",		do_breath_fire,POS_FIGHTING,	 0 , LOG_NORMAL, 1, 0 },
    { "bs",		do_backstab,	POS_FIGHTING,	 0,  LOG_NORMAL, 0, 0 },
    { "berserk",	do_berserk,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, 1 },
    { "circle",		do_circle_stab,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, 1 },
    { "detect",        	do_detect_hidden, POS_STANDING,  0,  LOG_NORMAL, 1, 1 },
    { "blitz",		do_blitz, POS_FIGHTING, 0, LOG_NORMAL, 1, 1 },
    { "crush", do_crush, POS_FIGHTING, 0, LOG_NORMAL, 1, 1 },
    { "dirt",		do_dirt,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, 1 },
    { "disarm",		do_disarm,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, 1 },
    { "flee",		do_flee,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, 1 },
    { "kick",		do_kick,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, 1 },
	{ "crescentkick",	do_crescent_kick,	POS_FIGHTING,	0,	LOG_NORMAL,	1,	1 },
	{ "axekick",	do_axe_kick,	POS_FIGHTING,	0,	LOG_NORMAL,	1,	1 },
	{ "endure",	do_endure,	POS_STANDING,	0,	LOG_NORMAL,	0,	0 },
	{ "jujitsu",	do_jujitsu,	POS_FIGHTING,	0,	LOG_NORMAL,	0,	0 },
	{ "heightenedawareness",	do_heightened_awareness,	POS_STANDING,	0,	LOG_NORMAL,	0,	0 },
	{ "bindwounds",	do_bindwounds,	POS_FIGHTING,	0,	LOG_NORMAL,	1,	0 },
	{ "tigerclaw",	do_tigerclaw,	POS_FIGHTING,	0,	LOG_NORMAL,	1,	1 },
	{ "scissorkick",	do_scissor_kick,	POS_FIGHTING,	0,	LOG_NORMAL,	1,	1 },
    {"rear", do_rear_kick, POS_FIGHTING, 0,LOG_NORMAL, 1, 1},

    { "lunge",		do_lunge,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, 1 },
    { "murde",		do_murde,	POS_FIGHTING,	 0,  LOG_NORMAL, 0, 0 },
    { "murder",         do_murder,      POS_FIGHTING,    0,  LOG_NORMAL, 1, 1 },
    { "rescue",		do_rescue,	POS_FIGHTING,	 0,  LOG_NORMAL, 0, 1 },
    { "surrender",	do_surrender,	POS_FIGHTING,	0,	LOG_NORMAL, 1, 0 },
    { "trip",		do_trip,	POS_FIGHTING,    0,  LOG_NORMAL, 1, 1 },
    { "defend", do_defend, POS_FIGHTING, 0, LOG_NORMAL, 1, 0},
    { "assassinate", do_assassinate, POS_STANDING,  0, LOG_NORMAL, 1,1},
    { "shoot", do_shoot, POS_STANDING, LOG_NORMAL, 1, 1},
    { "forage", do_forage, POS_STANDING, 0, LOG_NORMAL, 1, 1},
    { "fwood",	do_findwood, POS_STANDING, 0, LOG_NORMAL, 1, 1 },
	{ "carve",	do_carve,	POS_STANDING, 0, LOG_NORMAL, 1, 1 },
	{ "protection", do_protection_heat_cold, POS_STANDING, 0, LOG_NORMAL, 1, 1},
    { "intimidate", do_intimidate, POS_FIGHTING, LOG_NORMAL,1, 1},
    { "disperse", do_disperse,	POS_STANDING,	 0,  LOG_NORMAL, 1, 1 },
    { "bind", do_bind, POS_STANDING, 0, LOG_NORMAL, 1, 0},
    { "unbind", do_unbind, POS_FIGHTING, 0, LOG_NORMAL, 1, 1},
    { "hacksaw", do_hacksaw,	POS_STANDING,	 0,  LOG_NORMAL, 1, 1 },
    { "boneshatter", do_boneshatter, POS_FIGHTING, 0, LOG_NORMAL, 1, 0},
    { "pincer", do_pincer, POS_FIGHTING, 0, LOG_NORMAL, 1, 0},
    { "shove", do_shove, POS_FIGHTING, 0, LOG_NORMAL, 1, 0},
    { "strip", do_strip, POS_FIGHTING, 0, LOG_NORMAL, 1, 0},
    { "resist", do_resist, POS_FIGHTING, 0, LOG_NORMAL, 1, 1 },
    { "choke", do_choke, POS_FIGHTING, 0, LOG_NORMAL, 1, 0},
    { "vanish", do_vanish, POS_FIGHTING, 0, LOG_NORMAL, 1, 0},
    { "assess", do_assess, POS_STANDING, 0, LOG_NORMAL, 1, 0},
    { "tripwire", do_trip_wire,	POS_STANDING, 0, LOG_NORMAL, 1, 0},
    { "gag", do_gag, POS_STANDING, 0, LOG_NORMAL, 1, 0 },
    { "ranger staff", do_ranger_staff, POS_STANDING, 0, LOG_NORMAL, 1, 0 },
    { "vermin",	do_vermin,	POS_FIGHTING, 0, LOG_NORMAL, 1, 0 },
    { "coerce", do_coerce,	POS_FIGHTING, 0, LOG_NORMAL, 1, 0 },
    { "psionic",do_psionic_blast, POS_FIGHTING, 0, LOG_NORMAL, 1, 0 },
    { "swing", do_swing,	POS_FIGHTING, 0, LOG_NORMAL, 1, 0 },



    /*
     * Mob command interpreter (placed here for faster scan...)
     */
    { "mob",            do_mob,         POS_DEAD,        0,  LOG_NEVER,  0, 0 },

    /*
     * Miscellaneous commands.
     */
	{ "wanted", do_wanted, POS_SLEEPING, 0, LOG_ALWAYS, 1, 0},
    { "follow",		do_follow,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { "creep",		do_creep,	POS_STANDING,	 0,  LOG_NORMAL, 1, 0 },
    { "scan",           do_scan,        POS_RESTING,     0,  LOG_NORMAL, 1, 1 },
    { "gain",		do_gain,	POS_STANDING,	 0,  LOG_NORMAL, 1, 0 },
    { "groups",		do_groups,	POS_SLEEPING,    0,  LOG_NORMAL, 1, 0 },
    { "acute", 		do_acute_vision,POS_STANDING,    0,  LOG_NORMAL, 1, 0 },
    { "barkskin",	do_barkskin,	POS_STANDING,	 0,  LOG_NORMAL, 1, 0 },
    { "eye",		do_eye_of_the_predator,POS_STANDING,0,LOG_NORMAL,1, 0 },
    { "shadowgate",	do_shadowgate,	 POS_STANDING,	 0,  LOG_NORMAL, 1, 0 },
    { "lightwalk",	do_lightwalk,	 POS_STANDING,	 0,  LOG_NORMAL, 1, 0 },
    { "animal",	do_animal_call, POS_STANDING, 0, LOG_NORMAL, 1, 0 },
    { "enlist",	do_enlist, POS_STANDING, 0, LOG_NORMAL, 1, 0},
    { "bear", do_bear_call, POS_STANDING, 0, LOG_ALWAYS, 1, 0},
    { "camouflage",	do_camouflage,	POS_STANDING,	 0,  LOG_NORMAL, 1, 0 },
    { "chameleon",	do_chameleon,   POS_STANDING,	 0,  LOG_NORMAL, 1, 0 },
    { "camp",		do_camp,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "hide",		do_hide,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { "laying",		do_laying_hands, POS_STANDING,   0,  LOG_NORMAL, 1, 0 },
    { "revert",		do_revert,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, 0 },
    { "lagout",		do_lagout,	POS_DEAD,	 60, LOG_NORMAL,0,0},
    { "qui",		do_qui,		POS_DEAD,	 0,  LOG_NORMAL, 0, 0 },
    { "quit",		do_quit,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "release",	do_release, POS_FIGHTING, 0, LOG_NORMAL, 1, 1},
    { "recall",		do_recall,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, 1 },
    { "tame", do_tame, POS_FIGHTING, 0,LOG_NORMAL, 1, 1},
    { "find water", do_find_water,  POS_STANDING, 0, LOG_NORMAL, 1, 1},
    { "fw", do_find_water, POS_STANDING, 0, LOG_NORMAL, 0, 1},
    { "track", do_track, POS_RESTING, 0, LOG_NORMAL, 1, 1},
    { "rent",		do_rent,	POS_DEAD,	 0,  LOG_NORMAL, 0, 0 },
    { "save",		do_save,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },
    { "sleep",		do_sleep,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, 0 },
    { "sneak",		do_sneak,	POS_STANDING,	 0,  LOG_NORMAL, 1, 0 },
    { "split",		do_split,	POS_RESTING,	 0,  LOG_NORMAL, 1, 1 },
    { "steal",		do_steal,	POS_STANDING,	 0,  LOG_NORMAL, 1, 1 },
    { "steel",		do_steel_nerves, POS_FIGHTING,	 0,  LOG_NORMAL, 1, 1 },
    { "train",		do_train,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { "visible",	do_visible,	POS_SLEEPING,	 0,  LOG_NORMAL, 1, 0 },
    { "where",		do_where,	POS_RESTING,	 0,  LOG_NORMAL, 1, 0 },
    { "hometown" ,do_hometown, POS_RESTING, 0 ,LOG_ALWAYS, 1, 1},
    { "invoke",		do_invoke,	POS_DEAD,	0,  LOG_NORMAL, 1, 1 },
    { "executioner",		do_executioners_grace,	POS_STANDING,	0,  LOG_NORMAL, 1, 0 },
    { "dragonsword",	do_dragonsword, POS_STANDING, 0, LOG_NORMAL, 1, 0 },
    { "dragonplate",	do_dragonplate, POS_STANDING, 0, LOG_NORMAL, 1, 0 },
    { "honorable",	do_honorable_combat, POS_STANDING, 0, LOG_NORMAL, 1, 0 },
    { "stun",	do_stun, POS_FIGHTING, 0, LOG_NORMAL, 1, 1 },
    { "gouge",  do_gouge, POS_FIGHTING, 0, LOG_NORMAL, 1, 1 },
    { "stab",   do_stab,  POS_FIGHTING, 0, LOG_NORMAL, 1, 1 },
    { "headclap",   do_headclap,  POS_FIGHTING, 0, LOG_NORMAL, 1, 1 },
    { "snare",	do_snare, POS_STANDING, 0, LOG_NORMAL, 1, 1 },
    { "whirl",  do_whirl, POS_FIGHTING, 0, LOG_NORMAL, 1, 1 },
    { "crushing blow", do_crushing_blow, POS_FIGHTING, 0, LOG_NORMAL, 1, 1 },
	{ "bounty", do_bounty, POS_DEAD, 0, LOG_ALWAYS, 1, 1 }, //Bounty for Ancient, Ceial
	{ "points", do_points, POS_DEAD, 0, LOG_NORMAL, 1, 0 },
	{ "chours", do_check_hours, POS_DEAD, 0, LOG_ALWAYS, 1, 1 }, // For leaders
	{ "target", do_target, POS_DEAD, 0, LOG_ALWAYS, 1, 1 },
	{ "swoop", 	do_swoop,  POS_FIGHTING, 0, LOG_NORMAL, 1, 0 },
    /*
     * Immortal commands.
     */
    { "advance",	do_advance,	POS_DEAD,	ML,  LOG_ALWAYS, 1, 0 },
    { "afk",            do_afk,         POS_DEAD,       L7,  LOG_NORMAL, 1, 0 },
    { "autodamage",	do_autodamage,	POS_DEAD,	IM,	LOG_NORMAL, 1, 0 },
    { "alist",	        do_alist,	POS_DEAD,       L3,  LOG_NORMAL, 1, 0},
    { "vlist",          do_vlist,       POS_DEAD,       L7,  LOG_NORMAL, 1, 0},
    { "dump",		do_dump,	POS_DEAD,	ML,  LOG_ALWAYS, 0, 0 },
    { "tickforce",	do_forcetick,	POS_DEAD,	ML,  LOG_ALWAYS, 1, 0 },
    { "trust",		do_trust,	POS_DEAD,	ML,  LOG_ALWAYS, 1, 0 },
    { "violate",	do_violate,	POS_DEAD,	ML,  LOG_ALWAYS, 1, 0 },

    { "allow",		do_allow,	POS_DEAD,	L2,  LOG_ALWAYS, 1, 0 },
    { "ban",		do_ban,		POS_DEAD,	L2,  LOG_ALWAYS, 1, 0 },
    { "access", do_access, POS_DEAD, ML, LOG_ALWAYS, 1, 1},
    { "deny",		do_deny,	POS_DEAD,	L5,  LOG_ALWAYS,
1, 0 },
    { "disconnect",	do_disconnect,	POS_DEAD,	ML,  LOG_ALWAYS,
1, 0 },
    { "flag",		do_flag,	POS_DEAD,	L4,  LOG_ALWAYS, 1, 0 },
    { "freeze",		do_freeze,	POS_DEAD,	L4,  LOG_ALWAYS, 1, 0 },
    { "global", do_global, POS_DEAD, L7, LOG_NORMAL, 0, 0},
    { "smite", 		do_smite, 	POS_DEAD,	L6,  LOG_ALWAYS, 1, 0 },
    { "smiteold", 		do_oldsmite, 	POS_DEAD,	L6, LOG_ALWAYS, 1, 0 },
    { "permban",	do_permban,	POS_DEAD,	L1,  LOG_ALWAYS, 1, 0 },
    { "protect",	do_protect,	POS_DEAD,	L1,  LOG_ALWAYS, 1, 0 },
    { "reboo",		do_reboo,	POS_DEAD,	L3,  LOG_NORMAL, 0, 0 },
    { "reboot",		do_reboot,	POS_DEAD,	L3,  LOG_ALWAYS, 1, 0 },
    { "wflag",		do_wflag,	POS_DEAD,	L1,  LOG_ALWAYS, 1, 0 },
    { "set",		do_set,		POS_DEAD,	L1,  LOG_ALWAYS, 1, 0 },
    { "shutdow",	do_shutdow,	POS_DEAD,	L1,  LOG_NORMAL, 0, 0 },
    { "shutdown",	do_shutdown,	POS_DEAD,	L1,  LOG_ALWAYS, 1, 0 },
    { "wizlock",	do_wizlock,	POS_DEAD,	L2,  LOG_ALWAYS, 1, 0 },
    { "gslay",          do_gslay,       POS_DEAD,       L2,  LOG_ALWAYS, 1, 0 },
    { "arena",	do_arena,	POS_DEAD,	L5,  LOG_ALWAYS, 1, 0 },
    { "classes", do_classes, POS_DEAD, IM, LOG_NORMAL, 1, 0 },
    { "force",		do_force,	POS_DEAD,	L6,  LOG_ALWAYS, 1, 0 },
    { "load",		do_load,	POS_DEAD,	L6,  LOG_ALWAYS, 1, 0 },
    { "newlock",	do_newlock,	POS_DEAD,	L4,  LOG_ALWAYS, 1, 0 },
    { "nocast",		do_nocast,	POS_DEAD,	L6,	 LOG_ALWAYS, 1, 0 },
    { "nochannels",	do_nochannels,	POS_DEAD,	L5,  LOG_ALWAYS, 1, 0 },
    { "noemote",	do_noemote,	POS_DEAD,	L5,  LOG_ALWAYS, 1, 0 },
    { "noshout",	do_noshout,	POS_DEAD,	L5,  LOG_ALWAYS, 1, 0 },
    { "notell",		do_notell,	POS_DEAD,	L5,  LOG_ALWAYS, 1, 0 },
    { "pecho",		do_pecho,	POS_DEAD,	L4,  LOG_ALWAYS, 1, 0 },
    { "pardon",		do_pardon,	POS_DEAD,	L3,  LOG_ALWAYS, 1, 0 },
    { "purge",		do_purge,	POS_DEAD,	L4,  LOG_ALWAYS, 1, 0 },
    { "restore",	do_restore,	POS_DEAD,	L5,  LOG_ALWAYS, 1, 0 },
    { "sla",		do_sla,		POS_DEAD,	L3,  LOG_NORMAL, 0, 0 },
    { "slay",		do_slay,	POS_DEAD,	L3,  LOG_ALWAYS, 1, 0 },
    { "teleport",	do_teleport,    POS_DEAD,	L5,  LOG_ALWAYS, 1, 0 },
    { "transfer",	do_transfer,	POS_DEAD,	L5,  LOG_ALWAYS, 1, 0 },

    { "at",         do_at,          POS_DEAD,       L6,  LOG_NORMAL, 1, 0 },
    { "raffects",   do_raffects,    POS_DEAD,       L6,  LOG_NORMAL, 1, 0 },
    { "poofin",		do_bamfin,	POS_DEAD,	L7,  LOG_NORMAL,
1, 0 },
    { "addapply",		do_addapply,	POS_DEAD,	ML,  LOG_NORMAL, 1, 0 },
    { "maxlimits", do_max_limits, POS_DEAD, L3, LOG_ALWAYS, 1, 0},
    { "poofout",	do_bamfout,	POS_DEAD,	L7,  LOG_NORMAL,
1, 0 },
    { "areset",		do_areset, POS_DEAD,	L5,  LOG_ALWAYS, 1, 0 },
    { "freset",		do_force_reset, POS_DEAD,	L8,  LOG_ALWAYS,
1, 0 },
    { "gecho",		do_echo,	POS_DEAD,	L7,  LOG_ALWAYS,
1, 0 },
    { "astrip",		do_astrip,	POS_DEAD,	L5,  LOG_ALWAYS, 1, 0 },
    { "crumble",		do_crumble,	POS_DEAD,	IM,  LOG_NORMAL, 1, 0 },
    { "qcrumble",		do_qcrumble,	POS_DEAD,	IM,  LOG_ALWAYS, 1, 0 },
    { "brands",		do_brands,	POS_DEAD,	L7,  LOG_ALWAYS,
1, 0 },
    { "unbrands",		do_unbrands,	POS_DEAD,	L7,
LOG_ALWAYS, 1, 0 },
    { "holylight",	do_holylight,	POS_DEAD,	IM,  LOG_NORMAL, 1, 0 },
    { "incognito",	do_incognito,	POS_DEAD,	L7,  LOG_NORMAL,
1, 0 },
    { "invis",		do_invis,	POS_DEAD,	IM,  LOG_NORMAL, 0, 0 },
    { "log",		do_log,		POS_DEAD,	L1,  LOG_ALWAYS, 1, 0 },
    { "memory",		do_memory,	POS_DEAD,	L7,  LOG_NORMAL,
1, 0 },
    { "mwhere",		do_mwhere,	POS_DEAD,	L7,  LOG_NORMAL,
1, 0 },
    { "owhere",		do_owhere,	POS_DEAD,	L7,  LOG_NORMAL,
1, 0 },
    { "peace",		do_peace,	POS_DEAD,	L6,  LOG_NORMAL, 1, 0 },
    { "penalty",	do_penalty,	POS_DEAD,	L7,  LOG_NORMAL, 1, 0 },
    { "praclist",	do_praclist,	POS_DEAD,	L7,  LOG_NORMAL,
1, 0 },
    { "ghost",		do_ghost,	POS_DEAD,	L6,  LOG_ALWAYS, 1, 0 },
    { "echo",		do_recho,	POS_DEAD,	L6,  LOG_ALWAYS, 1, 0 },
    { "return",         do_return,      POS_DEAD,       L6,  LOG_NORMAL, 1, 0 },
    { "snoop",		do_snoop,	POS_DEAD,	L6,  LOG_ALWAYS, 1, 0 },
    { "stat",		do_stat,	POS_DEAD,	IM,  LOG_NORMAL, 1, 0 },
    { "string",		do_string,	POS_DEAD,	L5,  LOG_ALWAYS, 1, 0 },
    { "switch",		do_switch,	POS_DEAD,	L6,  LOG_ALWAYS, 1, 0 },
    { "wizinvis",	do_invis,	POS_DEAD,	L7,  LOG_NORMAL,
1, 0 },
    { "vnum",		do_vnum,	POS_DEAD,	L5,  LOG_NORMAL, 1, 0 },
    { "vmstat",         do_vmstat,      POS_DEAD,       L5,  LOG_NORMAL, 1, 0 },
    { "vostat",         do_vostat,      POS_DEAD,       L5,  LOG_NORMAL, 1, 0 },
    { "zecho",		do_zecho,	POS_DEAD,	L4,  LOG_ALWAYS, 1, 0 },
    { "mpdump",         do_mpdump,      POS_DEAD,       L7,  LOG_NEVER,
1, 0 },
    { "mpstat",         do_mpstat,      POS_DEAD,       L7,  LOG_NEVER,
1, 0 },
    { "mplist",		do_mplist,	POS_DEAD,	L7,  LOG_NEVER,
1, 0 },
    { "otype",		do_otype,	POS_DEAD,	L2,  LOG_NEVER,  1, 0 },
    { "fstat",		do_fstat,	POS_DEAD,	L7,  LOG_NEVER,
1, 0 },

    { "slookup",	do_slookup,	POS_DEAD,	L5,  LOG_NORMAL, 1, 0 },


    /*
     * OLC
     */
    { "edit",		do_olc,		POS_DEAD,   L8,  LOG_NORMAL, 1, 0 },
    { "asave",          do_asave,	POS_DEAD,   L8,  LOG_NORMAL, 1, 0 },
    { "alist",		do_alist,	POS_DEAD,   L8,  LOG_NORMAL, 1, 0 },
    { "resets",		do_resets,	POS_DEAD,   L8,  LOG_NORMAL, 1, 0 },
    { "redit",		do_redit,	POS_DEAD,   L8,	 LOG_NORMAL, 1, 0 },
    { "medit",		do_medit,	POS_DEAD,   L8,	 LOG_NORMAL, 1, 0 },
    { "aedit",		do_aedit,	POS_DEAD,   L8,  LOG_NORMAL, 1, 0 },
    { "oedit",		do_oedit,	POS_DEAD,   L8,  LOG_NORMAL, 1, 0 },
    { "hedit",		do_hedit,	POS_DEAD,   L5,  LOG_NORMAL, 1, 0 },
    { "mpedit",		do_mpedit,	POS_DEAD,   L5,  LOG_NORMAL, 1, 0 },


    { "clone",          do_clone,       POS_DEAD,       L5,  LOG_ALWAYS, 1, 0 },

    { "wiznet",		do_wiznet,	POS_DEAD,	IM,  LOG_NORMAL, 1, 0 },
    { "imotd",          do_imotd,       POS_DEAD,       IM,  LOG_NORMAL, 1, 0 },
    { ":",		do_immtalk,	POS_DEAD,	0,  LOG_NORMAL, 0,0 },
    { "builder",	do_builder,	POS_SLEEPING,	0,LOG_NORMAL,0,0},
    { "renam", do_renam, POS_DEAD, 55, LOG_ALWAYS, 0,0},
    { "rename", do_rename, POS_DEAD, 55, LOG_ALWAYS, 0,0},
    { "bd",		do_builder, 	POS_SLEEPING,	0, LOG_NORMAL, 0,0 },
    { "smote",		do_smote,	POS_DEAD,	IM,  LOG_NORMAL, 1, 0 },
    { "prefi",		do_prefi,	POS_DEAD,	IM,  LOG_NORMAL, 0, 0 },
    { "prefix",		do_prefix,	POS_DEAD,	IM,  LOG_NORMAL, 1, 0 },
    { "bandage",        do_bandage,     POS_RESTING,     0,  LOG_NORMAL, 1, 1},
    { "xlook",          do_xlook,       POS_DEAD,       IM,  LOG_NORMAL, 1, 0},
    { "affadd",         do_affadd,      POS_DEAD,       L4,  LOG_ALWAYS, 1, 0},
    { "affrem",         do_affrem,      POS_DEAD,       L4,  LOG_ALWAYS, 1, 0},
    { "divine", do_divine_intervention,      POS_DEAD,       L3,  LOG_ALWAYS, 1, 0},
    { "ccb",            do_ccb,         POS_DEAD,       L7,  LOG_ALWAYS, 1, 0},
    { "heroimm",	do_heroimm,	POS_DEAD,	ML,  LOG_ALWAYS, 1, 0 },
    { "finger",		do_finger,	POS_DEAD,	L7,  LOG_ALWAYS, 1, 0 },
    { "otrack",		do_otrack,	POS_DEAD,	L5,  LOG_ALWAYS, 1, 0 },
    { "ltrack",		do_ltrack,	POS_DEAD,	L3,  LOG_ALWAYS, 1, 0 },
    { "pload",		do_pload,	POS_DEAD,	ML,  LOG_ALWAYS, 1, 0 },

/* lich */
    { "lich",           do_lich,        POS_DEAD,       ML,  LOG_ALWAYS, 1, 0},
/* swarrior */
    { "specialize",     do_specialize,  POS_DEAD,       0,   LOG_ALWAYS, 1, 0 },
    { "overhead",       do_overhead,    POS_FIGHTING,   0,   LOG_NORMAL, 1, 0 },

    { "shapeshift",	do_shapeshift,  POS_DEAD,	0,  LOG_NORMAL,1, 0 },
    { "forms",		do_forms,	POS_DEAD,	0,  LOG_NORMAL,1, 0 },
    { "element",	do_element, POS_DEAD,	0,	LOG_NORMAL,1, 0 },

/* empire */
    { "empire",		do_empire,	POS_RESTING,	 0,  LOG_ALWAYS, 1, 0 },
    { "bloodoath",	do_bloodoath,	POS_RESTING,	 0,  LOG_ALWAYS, 1, 0 },
    { "citizen",	do_citizen,	POS_RESTING,	 0,  LOG_ALWAYS, 1, 0 },
    { "anathema",	do_anathema,	POS_RESTING,	 0,  LOG_ALWAYS, 1, 0 },
    { "nightfist",	do_nightfist,	POS_FIGHTING,	 0,  LOG_NORMAL, 1, 0 },
    { "pay",		do_pay,		POS_FIGHTING,	 0,  LOG_NORMAL, 1, 0 },
    { "centurions",     do_centurion_call,POS_STANDING,	 0,  LOG_NORMAL, 1, 0 },
    { "imperial", 	do_imperial_training, 	POS_FIGHTING, 	 0,  LOG_NORMAL, 1, 0 },

/* fun for Graelik :P */
    { "piethrow",	do_piethrow,	POS_DEAD,	 0,  LOG_NORMAL, 1, 0 },

    /*
     * End of list.
     */
    { "",		0,		POS_DEAD,	 0,  LOG_NORMAL, 0, 0 }
};




/*
 * The main entry point for executing commands.
 * Can be recursively called from 'at', 'order', 'force'.
 */
void interpret( CHAR_DATA *ch, char *argument )
{
    char command[MAX_INPUT_LENGTH];
    char logline[MAX_INPUT_LENGTH];
    int cmd;
    int trust;
    bool found;

    /*
     * Strip leading spaces.
     */
    while ( isspace(*argument) )
	argument++;
    if ( argument[0] == '\0' )
	return;

    /*
     * Implement freeze command.
     */
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_FREEZE) && !IS_IMMORTAL(ch))
    {
	send_to_char( "You're totally frozen!\n\r", ch );
	return;
    }


    /*
     * Grab the command word.
     * Special parsing so ' can be a command,
     *   also no spaces needed after punctuation.
     */
    strcpy( logline, argument );
    if ( !isalpha(argument[0]) && !isdigit(argument[0]) )
    {
	command[0] = argument[0];
	command[1] = '\0';
	argument++;
	while ( isspace(*argument) )
	    argument++;
    }
    else
    {
	argument = one_argument( argument, command );
    }

    /*
     * Look for command in command table.
     */
    found = FALSE;
    trust = get_trust( ch );
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == cmd_table[cmd].name[0]
	&&   !str_prefix( command, cmd_table[cmd].name )
	&&   cmd_table[cmd].level <= trust )
	{
	    found = TRUE;
	    break;
	}
    }

    /*
     * Log and snoop.
     */
    if ( cmd_table[cmd].log == LOG_NEVER )
	strcpy( logline, "" );

    if ( ( !IS_NPC(ch) && IS_SET(ch->act, PLR_LOG) )
    ||   fLogAll
    ||   cmd_table[cmd].log == LOG_ALWAYS )
    {
	sprintf( log_buf, "Log %s: %s", ch->name, logline );
	wiznet(log_buf,ch,NULL,WIZ_SECURE,0,get_trust(ch));
	log_string( log_buf );
    }

    if ( ch->desc != NULL && ch->desc->snoop_by != NULL )
    {
	write_to_buffer( ch->desc->snoop_by, "% ",    2 );
	write_to_buffer( ch->desc->snoop_by, logline, 0 );
	write_to_buffer( ch->desc->snoop_by, "\n\r",  2 );
    }

    /* Timestop implementation */
    if (is_affected(ch,gsn_timestop)
	&& cmd_table[cmd].name != "immtalk"
	&& cmd_table[cmd].name != ":")
    {
	send_to_char("Time has frozen for you and you are trapped between ticks of the clock...\n\r",ch);
	return;
    }

    /* Power word Stun implementation */

    if (is_affected(ch,gsn_power_word_stun) )
    {
        if (((found) && (cmd_table[cmd].name != "score")
	&& (cmd_table[cmd].name != "look")
	&& (cmd_table[cmd].name != "who")
	&& (cmd_table[cmd].name != "affects")
	&& (cmd_table[cmd].name != "group")
	&& (cmd_table[cmd].name != "gt")
	&& (cmd_table[cmd].name != "gtell")
	&& (cmd_table[cmd].name != ":")
	&& (cmd_table[cmd].name != "astrip")
	&& (cmd_table[cmd].name != "immtalk") )
	|| (!found) )
        {
	    send_to_char("You are far too stunned by the power word to move.\n\r",ch);
	    return;
        }
    }

    /* Hold person Implementation -Detlef */

    if (is_affected(ch,gsn_hold_person)
	&& cmd_table[cmd].name != "immtalk"
        && cmd_table[cmd].name != "astrip")
    {
        send_to_char("You are totally frozen.\n\r",ch);
	return;
    }

    if (is_affected(ch,gsn_snare)
	&& cmd_table[cmd].name != "immtalk"
        && cmd_table[cmd].name != "astrip")
    {
	send_to_char("Your futile struggling only manages to entangle you further!\n\r",ch);
	return;
    }

    /*if (is_affected(ch,gsn_confuse)
	&& cmd_table[cmd].name != "score"
	&& cmd_table[cmd].name != "affects"
	&& cmd_table[cmd].name != "immtalk"
        && cmd_table[cmd].name != "astrip")
    {
	WAIT_STATE(ch,PULSE_VIOLENCE);
    }*/

/* This is to handle perm ghosts who have died due to age...let them run
some commands only */
    if (!IS_NPC(ch))
	{
	if (ch->pcdata->death_status == HAS_DIED)
    {
        if (((found) && (cmd_table[cmd].name != "score")
	&& (cmd_table[cmd].name != "look")
	&& (cmd_table[cmd].name != "who")
	&& (cmd_table[cmd].name != "affects")
	&& (cmd_table[cmd].name != "group")
	&& (cmd_table[cmd].name != "gt")
	&& (cmd_table[cmd].name != "reply")
	&& (cmd_table[cmd].name != "note")
	&& (cmd_table[cmd].name != "say")
	&& (cmd_table[cmd].name != "gtell")
	&& (cmd_table[cmd].name != ":")
	&& (cmd_table[cmd].name != "immtalk")
	&& (cmd_table[cmd].name != "tell")
        && (cmd_table[cmd].name != "cb")
	&& (cmd_table[cmd].name != "ooctell")
	&& (cmd_table[cmd].name != "ht")
	&& (cmd_table[cmd].name != "n")
	&& (cmd_table[cmd].name != "e")
	&& (cmd_table[cmd].name != "s")
	&& (cmd_table[cmd].name != "w")
	&& (cmd_table[cmd].name != "north")
	&& (cmd_table[cmd].name != "east")
	&& (cmd_table[cmd].name != "south")
	&& (cmd_table[cmd].name != "west")
	&& (cmd_table[cmd].name != "up")
	&& (cmd_table[cmd].name != "u")
	&& (cmd_table[cmd].name != "down")
	&& (cmd_table[cmd].name != "d")
	&& (cmd_table[cmd].name != "where")
	&& (cmd_table[cmd].name != "pray")
	&& (cmd_table[cmd].name != "set")
	&& (cmd_table[cmd].name != "open")
	&& (cmd_table[cmd].name != "recall")
	&& (cmd_table[cmd].name != "/")
	&& (cmd_table[cmd].name != "quit")
	&& (cmd_table[cmd].name != "delete"))
	|| (!found) )
        {
	send_to_char("You are a hovering spirit, you can't do that anymore.\n\r",ch);
	    return;
        }
	}
    }

    if (!found && !check_social( ch, command, argument ))
        return send_to_char( "Huh?\n\r", ch );
    else if (!found)
        return;

    /*
     * Character not in position for command?
     */
    if ( ch->position < cmd_table[cmd].position )
    {
	switch( ch->position )
	{
	case POS_DEAD:
	    send_to_char( "Lie still; you are DEAD.\n\r", ch );
	    break;

	case POS_MORTAL:
	case POS_INCAP:
	    send_to_char( "You are hurt far too bad for that.\n\r", ch );
	    break;

	case POS_STUNNED:
	    send_to_char( "You are too stunned to do that.\n\r", ch );
	    break;

	case POS_SLEEPING:
	    send_to_char( "In your dreams, or what?\n\r", ch );
	    break;

	case POS_RESTING:
	    send_to_char( "Nah... You feel too relaxed...\n\r", ch);
	    break;

	case POS_SITTING:
	    send_to_char( "Better stand up first.\n\r",ch);
	    break;

	case POS_FIGHTING:
	    send_to_char( "No way!  You are still fighting!\n\r", ch);
	    break;

	}
	return;
    }

    /*
     * Hide parsing.
     */
    if (cmd_table[cmd].hide)
    {
	un_hide(ch, NULL);
    }

    /*
     * Dispatch the command.
     */
    (*cmd_table[cmd].do_fun) ( ch, argument );

    tail_chain( );
    return;
}



bool check_social( CHAR_DATA *ch, char *command, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int cmd;
    bool found;

    found  = FALSE;
    for ( cmd = 0; social_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == social_table[cmd].name[0]
	&&   !str_prefix( command, social_table[cmd].name ) )
	{
	    found = TRUE;
	    break;
	}
    }

    if ( !found )
	return FALSE;

    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
    {
	send_to_char( "You are anti-social!\n\r", ch );
	return TRUE;
    }

    if (isShifted(ch) && !IS_IMMORTAL(ch))
    {
	send_to_char("You can only do this in your human form.\n\r",ch);
	return TRUE;
    }

    switch ( ch->position )
    {
    case POS_DEAD:
	send_to_char( "Lie still; you are DEAD.\n\r", ch );
	return TRUE;

    case POS_INCAP:
    case POS_MORTAL:
	send_to_char( "You are hurt far too bad for that.\n\r", ch );
	return TRUE;

    case POS_STUNNED:
	send_to_char( "You are too stunned to do that.\n\r", ch );
	return TRUE;

    case POS_SLEEPING:
	/*
	 * I just know this is the path to a 12" 'if' statement.  :(
	 * But two players asked for it already!  -- Furey
	 */
	if ( !str_cmp( social_table[cmd].name, "snore" ) )
	    break;
	send_to_char( "In your dreams, or what?\n\r", ch );
	return TRUE;

    }

    one_argument( argument, arg );
    victim = NULL;
    if ( arg[0] == '\0' )
    {
	act( social_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM    );
	act( social_table[cmd].char_no_arg,   ch, NULL, victim, TO_CHAR    );
    }
    else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
    }
    else if ( victim == ch )
    {
	act( social_table[cmd].others_auto,   ch, NULL, victim, TO_ROOM    );
	act( social_table[cmd].char_auto,     ch, NULL, victim, TO_CHAR    );
    }
    else
    {
	act( social_table[cmd].others_found,  ch, NULL, victim, TO_NOTVICT );
	act( social_table[cmd].char_found,    ch, NULL, victim, TO_CHAR    );
	act( social_table[cmd].vict_found,    ch, NULL, victim, TO_VICT    );

	if ( !IS_NPC(ch) && IS_NPC(victim)
	&&   !IS_AFFECTED(victim, AFF_CHARM)
	&&   IS_AWAKE(victim)
	&&   victim->desc == NULL)
	{
	    switch ( number_bits( 4 ) )
	    {
	    case 0:

	    case 1: case 2: case 3: case 4:
	    case 5: case 6: case 7: case 8:
		act( social_table[cmd].others_found,
		    victim, NULL, ch, TO_NOTVICT );
		act( social_table[cmd].char_found,
		    victim, NULL, ch, TO_CHAR    );
		act( social_table[cmd].vict_found,
		    victim, NULL, ch, TO_VICT    );
		break;

	    case 9: case 10: case 11: case 12:
		act( "$n slaps $N.",  victim, NULL, ch, TO_NOTVICT );
		act( "You slap $N.",  victim, NULL, ch, TO_CHAR    );
		act( "$n slaps you.", victim, NULL, ch, TO_VICT    );
		break;
	    }
	}
    }

    return TRUE;
}



/*
 * Return true if an argument is completely numeric.
 */
bool is_number ( char *arg )
{

    if ( *arg == '\0' )
        return FALSE;

    if ( *arg == '+' || *arg == '-' )
        arg++;

    for ( ; *arg != '\0'; arg++ )
    {
        if ( !isdigit( *arg ) )
            return FALSE;
    }

    return TRUE;
}



/*
 * Given a string like 14.foo, return 14 and 'foo'
 */
int number_argument( char *argument, char *arg )
{
    char *pdot;
    int number;

    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
	if ( *pdot == '.' )
	{
	    *pdot = '\0';
	    number = atoi( argument );
	    *pdot = '.';
	    strcpy( arg, pdot+1 );
	    return number;
	}
    }

    strcpy( arg, argument );
    return 1;
}

/*
 * Given a string like 14*foo, return 14 and 'foo'
*/
int mult_argument(char *argument, char *arg)
{
    char *pdot;
    int number;

    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
        if ( *pdot == '*' )
        {
            *pdot = '\0';
            number = atoi( argument );
            *pdot = '*';
            strcpy( arg, pdot+1 );
            return number;
        }
    }

    strcpy( arg, argument );
    return 1;
}



/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
char *one_argument( char *argument, char *arg_first )
{
    char cEnd;

    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*arg_first = LOWER(*argument);
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( isspace(*argument) )
	argument++;

    return argument;
}

/*
 * Contributed by Alander.
 */
/*void do_commands( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;

    col = 0;
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( cmd_table[cmd].level <  LEVEL_HERO
        &&   cmd_table[cmd].level <= get_trust( ch )
	&&   cmd_table[cmd].show)
	{
	    sprintf( buf, "%-12s", cmd_table[cmd].name );
	    send_to_char( buf, ch );
	    if ( ++col % 6 == 0 )
		send_to_char( "\n\r", ch );
	}
    }

    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}
*/

void do_wizhelp( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;
    int clevel;
    col = 0;

    for( clevel = LEVEL_HERO + 1; clevel < MAX_LEVEL + 1; clevel++ )
    {
        for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
        {
            if ( cmd_table[cmd].level >= LEVEL_HERO
            &&   cmd_table[cmd].show
            &&   cmd_table[cmd].level == clevel)
            {
                sprintf( buf, "{W[{Y%-2d{W] %-13s{x", cmd_table[cmd].level, cmd_table[cmd].name);
                send_to_char( buf, ch );
                if ( ++col % 4 == 0 )
                    send_to_char( "\n\r", ch );
            }
        }
    }

    if ( col % 4 != 0 )
        send_to_char( "\n\r", ch );
    return;
}


void do_wizcheck( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int cmd;
    CHAR_DATA *victim;
    int col, showval = 0;
    bool showlevel = FALSE;
    col = 0;
    argument = one_argument( argument, arg1 );

    if ( arg1[0] == '\0')
    {
	send_to_char( "See whose Wizcommands?\n\r", ch );
	return;
    }
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (!IS_IMMORTAL(victim))
    {
    send_to_char("They aren't an Immortal, Cornhole.\n\r", ch);
    return;
	}

    for (cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
	{
	  if ((!showlevel && cmd_table[cmd].level >= LEVEL_HERO && cmd_table[cmd].level <= get_trust(victim))
	  || (showlevel && cmd_table[cmd].level == showval))
	{
	  if (!showlevel && argument[0] != '\0' && str_prefix(argument,cmd_table[cmd].name))
	  continue;
	  sprintf(buf, "(%i)%-12s", cmd_table[cmd].level,cmd_table[cmd].name);
	  page_to_char( buf, ch );
	  if (++col % 15 == 0 )
	  send_to_char("\n\r", ch);
	  }
	  }

	  if ( col % 15 != 0 )
	  send_to_char( "\n\r", ch );
	    return;
}

