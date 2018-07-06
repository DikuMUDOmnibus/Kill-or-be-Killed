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

/* command procedures needed */
DECLARE_DO_FUN(do_ram);
DECLARE_DO_FUN(do_bite);
DECLARE_DO_FUN(do_palm);
DECLARE_DO_FUN(do_kick);
DECLARE_DO_FUN(do_throw);
DECLARE_DO_FUN(do_crush);
DECLARE_DO_FUN(do_yell		);
DECLARE_DO_FUN(do_open		);
DECLARE_DO_FUN(do_close		);
DECLARE_DO_FUN(do_say	);
DECLARE_DO_FUN(do_tail);
DECLARE_DO_FUN(do_backstab);
DECLARE_DO_FUN(do_flee);
DECLARE_DO_FUN(do_murder);
DECLARE_DO_FUN(do_wanted	);
DECLARE_DO_FUN(do_look);
DECLARE_DO_FUN(do_follow);
DECLARE_SPELL_FUN(spell_unholy_fire);
DECLARE_DO_FUN(do_myell);
DECLARE_DO_FUN(do_goto);
DECLARE_DO_FUN(do_trip);
DECLARE_DO_FUN(do_bash);
DECLARE_DO_FUN(do_dirt);
DECLARE_DO_FUN(do_shove);

/*
 * The following special functions are available for mobiles.
 */
DECLARE_SPEC_FUN(	spec_breath_any		);
DECLARE_SPEC_FUN(	spec_breath_acid	);
DECLARE_SPEC_FUN(	spec_breath_fire	);
DECLARE_SPEC_FUN(	spec_breath_frost	);
DECLARE_SPEC_FUN(	spec_breath_gas		);
DECLARE_SPEC_FUN(	spec_breath_lightning	);
DECLARE_SPEC_FUN(	spec_cast_adept		);
DECLARE_SPEC_FUN(	spec_bishop	);
DECLARE_SPEC_FUN(	spec_cast_cleric	);
DECLARE_SPEC_FUN(	spec_cast_judge		);
DECLARE_SPEC_FUN(	spec_cast_mage		);
DECLARE_SPEC_FUN(	spec_cast_undead	);
DECLARE_SPEC_FUN(	spec_executioner	);
DECLARE_SPEC_FUN(	spec_fido		);
DECLARE_SPEC_FUN(	spec_guard		);
DECLARE_SPEC_FUN(	spec_janitor		);
DECLARE_SPEC_FUN(	spec_mayor		);
DECLARE_SPEC_FUN(	spec_poison		);
DECLARE_SPEC_FUN(	spec_thief		);
DECLARE_SPEC_FUN(	spec_nasty		);
DECLARE_SPEC_FUN(	spec_troll_member	);
DECLARE_SPEC_FUN(	spec_ogre_member	);
DECLARE_SPEC_FUN(	spec_patrolman		);
DECLARE_SPEC_FUN(	spec_goodie		);

/* The following were all added by Ceran. There may be some bugs in some
of them since I was too lazy to fully go back over them all. If you have
problems with any just don't run them on mobiles. Also refer to
update.doc file to get infomation on which ones require certain areas.
*/
DECLARE_SPEC_FUN(	spec_enforcer		);
DECLARE_SPEC_FUN(	spec_preserver		);
DECLARE_SPEC_FUN(	spec_fallen_angel	);
DECLARE_SPEC_FUN(	spec_troll		);
DECLARE_SPEC_FUN(	spec_chimera		);
DECLARE_SPEC_FUN(	spec_bane);
DECLARE_SPEC_FUN(	spec_monk	);
DECLARE_SPEC_FUN(	spec_darokin	);
DECLARE_SPEC_FUN(	spec_godfather	);
DECLARE_SPEC_FUN(	spec_rakshasa	);
DECLARE_SPEC_FUN(	spec_rakshasa_lich);
DECLARE_SPEC_FUN(	spec_rakshasa_royal);
DECLARE_SPEC_FUN(	spec_beholder	);
DECLARE_SPEC_FUN(	spec_deathknight);
DECLARE_SPEC_FUN(	spec_room_thow	);
DECLARE_SPEC_FUN(	spec_room_throw);
DECLARE_SPEC_FUN(	spec_spectre_king);
DECLARE_SPEC_FUN(	spec_hydra	);
DECLARE_SPEC_FUN(	spec_bone_demon);
DECLARE_SPEC_FUN(	spec_valere_knight);
DECLARE_SPEC_FUN(	spec_drannor_wraith);
DECLARE_SPEC_FUN(	spec_titan);
DECLARE_SPEC_FUN(	spec_necromancer);
DECLARE_SPEC_FUN(	spec_rager);
DECLARE_SPEC_FUN(	spec_demonologist);
DECLARE_SPEC_FUN(	spec_legionlord);
DECLARE_SPEC_FUN(	spec_gold_dragon);
DECLARE_SPEC_FUN(	spec_black_dragon);
DECLARE_SPEC_FUN(	spec_red_dragon);
DECLARE_SPEC_FUN( spec_seneschal);
DECLARE_SPEC_FUN( spec_warder);
DECLARE_SPEC_FUN( spec_sage);
DECLARE_SPEC_FUN( spec_pincer);
DECLARE_SPEC_FUN( spec_outlaw);
DECLARE_SPEC_FUN( spec_flurry);
DECLARE_SPEC_FUN( spec_drum);
DECLARE_SPEC_FUN( spec_ishmael);
DECLARE_SPEC_FUN( spec_agamemnon);
DECLARE_SPEC_FUN( spec_meph);
DECLARE_SPEC_FUN( spec_baalzebub);
DECLARE_SPEC_FUN( spec_necruvian);
DECLARE_SPEC_FUN( spec_obsidian_prince);
DECLARE_SPEC_FUN( spec_golden_knight);
DECLARE_SPEC_FUN( spec_templar);
//DECLARE_SPEC_FUN( spec_blitz);
DECLARE_SPEC_FUN( spec_lothar);
DECLARE_SPEC_FUN( spec_cire);
DECLARE_SPEC_FUN( spec_familiar);
DECLARE_SPEC_FUN( spec_omegus );
DECLARE_SPEC_FUN( spec_stalker );
DECLARE_SPEC_FUN( spec_phantom );

void    one_hit         args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ));
bool check_dispel( int dis_level, CHAR_DATA *victim, int sn);
bool area_act args((CHAR_DATA *ch,char *msg,int min_sect,int max_sector));

/* the function table */
const   struct  spec_type    spec_table[] =
{
    {	"spec_breath_any",		spec_breath_any		},
    {	"spec_breath_acid",		spec_breath_acid	},
    {	"spec_breath_fire",		spec_breath_fire	},
    {	"spec_breath_frost",		spec_breath_frost	},
    {	"spec_breath_gas",		spec_breath_gas		},
    {	"spec_breath_lightning",	spec_breath_lightning	},
    {	"spec_cast_adept",		spec_cast_adept		},
    {	"spec_cast_cleric",		spec_cast_cleric	},
    { "spec_bane",	spec_bane	},
    {	"spec_cast_judge",		spec_cast_judge		},
    {	"spec_cast_mage",		spec_cast_mage		},
    {	"spec_cast_undead",		spec_cast_undead	},
    {	"spec_executioner",		spec_executioner	},
    {	"spec_fido",			spec_fido		},
    {	"spec_guard",			spec_guard		},
    {	"spec_janitor",			spec_janitor		},
    {	"spec_mayor",			spec_mayor		},
    {	"spec_poison",			spec_poison		},
    {	"spec_thief",			spec_thief		},
    {	"spec_nasty",			spec_nasty		},
    {	"spec_troll",			spec_troll		},
    {	"spec_troll_member",		spec_troll_member	},
    {	"spec_ogre_member",		spec_ogre_member	},
    {	"spec_patrolman",		spec_patrolman		},
    { "spec_bishop",	spec_bishop		},
    {   "spec_enforcer",		spec_enforcer		},
    {   "spec_preserver",		spec_preserver		},
    {  "spec_fallen_angel",		spec_fallen_angel	},
    {	"spec_monk",		spec_monk	},
    {	"spec_chimera",	spec_chimera},
    {	"spec_darokin",	spec_darokin			},
    { "spec_godfather",	spec_godfather	},
    { "spec_rakshasa",	spec_rakshasa	},
    { "spec_rakshasa_royal",	spec_rakshasa_royal},
    { "spec_rakshasa_lich",	spec_rakshasa_lich},
    { "spec_room_throw",	spec_room_throw},
    { "spec_beholder",	spec_beholder },
    { "spec_deathknight",	spec_deathknight},
    { "spec_red_dragon", spec_red_dragon},
    {"spec_gold_dragon", spec_gold_dragon},
    { "spec_black_dragon", spec_black_dragon},
    {"spec_spectre_king",	spec_spectre_king},
    { "spec_hydra",	spec_hydra	},
    { "spec_bone_demon",	spec_bone_demon},
    { "spec_valere_knight",	spec_valere_knight},
    { "spec_drannor_wraith",	spec_drannor_wraith},
    { "spec_necromancer", spec_necromancer},
    { "spec_demonologist", spec_demonologist},
    { "spec_rager", spec_rager},
    { "spec_titan", spec_titan},
    { "spec_legionlord", spec_legionlord},
    { "spec_seneschal", spec_seneschal},
    { "spec_warder", spec_warder},
    { "spec_outlaw", spec_outlaw},
    { "spec_sage", spec_sage},
    { "spec_pincer", spec_pincer},
    { "spec_flurry", spec_flurry},
    { "spec_drum", spec_drum},
    { "spec_ishmael", spec_ishmael},
    { "spec_agamemnon", spec_agamemnon},
    { "spec_meph", spec_meph},
    { "spec_baalzebub", spec_baalzebub},
    { "spec_necruvian", spec_necruvian},
    { "spec_obsidian_prince", spec_obsidian_prince},
    { "spec_golden_knight", spec_golden_knight},
    { "spec_templar", spec_templar},
    { "spec_goodie", spec_goodie},
    //{ "spec_blitz", spec_blitz},
    { "spec_cire", spec_cire},
    { "spec_lothar", spec_cire},
    { "spec_familiar", spec_familiar},
    { "spec_omegus", spec_omegus},
    { "spec_stalker", spec_stalker},
    { "spec_phantom", spec_phantom},
    {	NULL,				NULL			}
};

/*
 * Given a name, return the appropriate spec fun.
 */
SPEC_FUN *spec_lookup( const char *name )
{
   int i;

   for ( i = 0; spec_table[i].name != NULL; i++)
   {
        if (LOWER(name[0]) == LOWER(spec_table[i].name[0])
        &&  !str_prefix( name,spec_table[i].name))
            return spec_table[i].function;
   }

    return 0;
}

char *spec_name( SPEC_FUN *function)
{
    int i;

    for (i = 0; spec_table[i].function != NULL; i++)
    {
	if (function == spec_table[i].function)
	    return spec_table[i].name;
    }

    return NULL;
}

bool spec_troll_member( CHAR_DATA *ch)
{
    CHAR_DATA *vch, *victim = NULL;
    int count = 0;
    char *message;

    if (!IS_AWAKE(ch) || IS_AFFECTED(ch,AFF_CALM) || ch->in_room == NULL
    ||  IS_AFFECTED(ch,AFF_CHARM) || ch->fighting != NULL)
	return FALSE;

    /* find an ogre to beat up */
    for (vch = ch->in_room->people;  vch != NULL;  vch = vch->next_in_room)
    {
	if (!IS_NPC(vch) || ch == vch)
	    continue;

	if (vch->pIndexData->vnum == MOB_VNUM_PATROLMAN)
	    return FALSE;

	if (vch->pIndexData->group == GROUP_VNUM_OGRES
	&&  ch->level > vch->level - 2 && !is_safe(ch,vch))
	{
	    if (number_range(0,count) == 0)
		victim = vch;

	    count++;
	}
    }

    if (victim == NULL)
	return FALSE;

    /* say something, then raise hell */
    switch (number_range(0,6))
    {
	default:  message = NULL; 	break;
	case 0:	message = "$n yells 'I've been looking for you, punk!'";
		break;
	case 1: message = "With a scream of rage, $n attacks $N.";
		break;
	case 2: message =
		"$n says 'What's slimy Ogre trash like you doing around here?'";
		break;
	case 3: message = "$n cracks his knuckles and says 'Do ya feel lucky?'";
		break;
	case 4: message = "$n says 'There's no cops to save you this time!'";
		break;
	case 5: message = "$n says 'Time to join your brother, spud.'";
		break;
	case 6: message = "$n says 'Let's rock.'";
		break;
    }

    if (message != NULL)
    	act(message,ch,NULL,victim,TO_ALL);
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return TRUE;
}

bool spec_ogre_member( CHAR_DATA *ch)
{
    CHAR_DATA *vch, *victim = NULL;
    int count = 0;
    char *message;

    if (!IS_AWAKE(ch) || IS_AFFECTED(ch,AFF_CALM) || ch->in_room == NULL
    ||  IS_AFFECTED(ch,AFF_CHARM) || ch->fighting != NULL)
        return FALSE;

    /* find an troll to beat up */
    for (vch = ch->in_room->people;  vch != NULL;  vch = vch->next_in_room)
    {
        if (!IS_NPC(vch) || ch == vch)
            continue;

        if (vch->pIndexData->vnum == MOB_VNUM_PATROLMAN)
            return FALSE;

        if (vch->pIndexData->group == GROUP_VNUM_TROLLS
        &&  ch->level > vch->level - 2 && !is_safe(ch,vch))
        {
            if (number_range(0,count) == 0)
                victim = vch;

            count++;
        }
    }

    if (victim == NULL)
        return FALSE;

    /* say something, then raise hell */
    switch (number_range(0,6))
    {
	default: message = NULL;	break;
        case 0: message = "$n yells 'I've been looking for you, punk!'";
                break;
        case 1: message = "With a scream of rage, $n attacks $N.'";
                break;
        case 2: message =
                "$n says 'What's Troll filth like you doing around here?'";
                break;
        case 3: message = "$n cracks his knuckles and says 'Do ya feel lucky?'";
                break;
        case 4: message = "$n says 'There's no cops to save you this time!'";
                break;
        case 5: message = "$n says 'Time to join your brother, spud.'";
                break;
        case 6: message = "$n says 'Let's rock.'";
                break;
    }

    if (message != NULL)
    	act(message,ch,NULL,victim,TO_ALL);
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return TRUE;
}

bool spec_patrolman(CHAR_DATA *ch)
{
    CHAR_DATA *vch,*victim = NULL;
    OBJ_DATA *obj;
    char *message;
    int count = 0;

    if (!IS_AWAKE(ch) || IS_AFFECTED(ch,AFF_CALM) || ch->in_room == NULL
    ||  IS_AFFECTED(ch,AFF_CHARM) || ch->fighting != NULL)
        return FALSE;

    /* look for a fight in the room */
    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
	if (vch == ch)
	    continue;

	if (vch->fighting != NULL)  /* break it up! */
	{
	    if (number_range(0,count) == 0)
	        victim = (vch->level > vch->fighting->level)
		    ? vch : vch->fighting;
	    count++;
	}
    }

    if (victim == NULL || (IS_NPC(victim) && victim->spec_fun == ch->spec_fun))
	return FALSE;

    if (((obj = get_eq_char(ch,WEAR_NECK_1)) != NULL
    &&   obj->pIndexData->vnum == OBJ_VNUM_WHISTLE)
    ||  ((obj = get_eq_char(ch,WEAR_NECK_2)) != NULL
    &&   obj->pIndexData->vnum == OBJ_VNUM_WHISTLE))
    {
	act("You blow down hard on $p.",ch,obj,NULL,TO_CHAR);
	act("$n blows on $p, ***WHEEEEEEEEEEEET***",ch,obj,NULL,TO_ROOM);

    	for ( vch = char_list; vch != NULL; vch = vch->next )
    	{
            if ( vch->in_room == NULL )
            	continue;

            if (vch->in_room != ch->in_room
	    &&  vch->in_room->area == ch->in_room->area)
            	send_to_char( "You hear a shrill whistling sound.\n\r", vch );
    	}
    }

    switch (number_range(0,6))
    {
	default:	message = NULL;		break;
	case 0:	message = "$n yells 'All roit! All roit! break it up!'";
		break;
	case 1: message =
		"$n says 'Society's to blame, but what's a bloke to do?'";
		break;
	case 2: message =
		"$n mumbles 'bloody kids will be the death of us all.'";
		break;
	case 3: message = "$n shouts 'Stop that! Stop that!' and attacks.";
		break;
	case 4: message = "$n pulls out his billy and goes to work.";
		break;
	case 5: message =
		"$n sighs in resignation and proceeds to break up the fight.";
		break;
	case 6: message = "$n says 'Settle down, you hooligans!'";
		break;
    }

    if (message != NULL)
	act(message,ch,NULL,NULL,TO_ALL);

    multi_hit(ch,victim,TYPE_UNDEFINED);

    return TRUE;
}


bool spec_nasty( CHAR_DATA *ch )
{
    CHAR_DATA *victim, *v_next;
    long gold;

    if (!IS_AWAKE(ch)) {
       return FALSE;
    }

    if (ch->position != POS_FIGHTING) {
       for ( victim = ch->in_room->people; victim != NULL; victim = v_next)
       {
          v_next = victim->next_in_room;
          if (!IS_NPC(victim)
             && (victim->level > ch->level)
             && (victim->level < ch->level + 10))
          {
	     do_backstab(ch,victim->name);
             if (ch->position != POS_FIGHTING)
                 do_murder(ch,victim->name);
             /* should steal some coins right away? :) */
             return TRUE;
          }
       }
       return FALSE;    /*  No one to attack */
    }

    /* okay, we must be fighting.... steal some coins and flee */
    if ( (victim = ch->fighting) == NULL)
        return FALSE;   /* let's be paranoid.... */

    switch ( number_bits(2) )
    {
        case 0:  act( "$n rips apart your coin purse, spilling your gold!",
                     ch, NULL, victim, TO_VICT);
                 act( "You slash apart $N's coin purse and gather his gold.",
                     ch, NULL, victim, TO_CHAR);
                 act( "$N's coin purse is ripped apart!",
                     ch, NULL, victim, TO_NOTVICT);
                 gold = victim->gold / 10;  /* steal 10% of his gold */
                 victim->gold -= gold;
                 ch->gold     += gold;
                 return TRUE;

        case 1:  do_flee( ch, "");
                 return TRUE;

        default: return FALSE;
    }
}

/*
 * Core procedure for dragons.
 */
bool dragon( CHAR_DATA *ch, char *spell_name )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 3 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    if ( ( sn = skill_lookup( spell_name ) ) < 0 )
	return FALSE;
    if (check_absorb(ch,victim,sn))
	return FALSE;

    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim, TARGET_CHAR);
    return TRUE;
}



/*
 * Special procedures for mobiles.
 */
bool spec_breath_any( CHAR_DATA *ch )
{
    if ( ch->position != POS_FIGHTING )
	return FALSE;

    switch ( number_bits( 3 ) )
    {
    case 0: return spec_breath_fire		( ch );
    case 1:
    case 2: return spec_breath_lightning	( ch );
    case 3: return spec_breath_gas		( ch );
    case 4: return spec_breath_acid		( ch );
    case 5:
    case 6:
    case 7: return spec_breath_frost		( ch );
    }

    return FALSE;
}



bool spec_breath_acid( CHAR_DATA *ch )
{
    return dragon( ch, "acid breath" );
}



bool spec_breath_fire( CHAR_DATA *ch )
{
    return dragon( ch, "fire breath" );
}



bool spec_breath_frost( CHAR_DATA *ch )
{
    return dragon( ch, "frost breath" );
}



bool spec_breath_gas( CHAR_DATA *ch )
{
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    if ( ( sn = skill_lookup( "gas breath" ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, NULL,TARGET_CHAR);
    return TRUE;
}



bool spec_breath_lightning( CHAR_DATA *ch )
{
    return dragon( ch, "lightning breath" );
}



bool spec_cast_adept( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;

    if ( !IS_AWAKE(ch) )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim != ch && can_see( ch, victim ) && number_bits( 1 ) == 0
	     && !IS_NPC(victim) && victim->level < 11)
	    break;
    }

    if ( victim == NULL )
	return FALSE;

	act("$n closes his eyes for a moment and nods at $N",ch,NULL,victim,TO_ROOM);
	act("$n closes his eyes for a moment and nods at you",ch,NULL,victim,TO_VICT);

    switch ( number_bits( 4 ) )
    {
    case 0:
	spell_armor( skill_lookup( "armor" ), ch->level,ch,victim,TARGET_CHAR);
	return TRUE;

    case 1:
	spell_bless( skill_lookup( "bless" ), ch->level,ch,victim,TARGET_CHAR);
	return TRUE;

    case 2:
	spell_cure_blindness( skill_lookup( "cure blindness" ),
	    ch->level, ch, victim,TARGET_CHAR);
	return TRUE;

    case 3:
	spell_cure_light( skill_lookup( "cure light" ),
	    ch->level, ch, victim,TARGET_CHAR);
	return TRUE;

    case 4:
	spell_cure_poison( skill_lookup( "cure poison" ),
	    ch->level, ch, victim,TARGET_CHAR);
	return TRUE;

    case 5:
	spell_refresh( skill_lookup("refresh"),
		ch->level,ch,victim,TARGET_CHAR);
	return TRUE;

    case 6:
	spell_cure_disease(skill_lookup("cure disease"),
	    ch->level,ch,victim,TARGET_CHAR);

    }

    return FALSE;
}



bool spec_cast_cleric( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  0; spell = "blindness";      break;
	case  1: min_level =  3; spell = "cause serious";  break;
	case  2: min_level =  7; spell = "earthquake";     break;
	case  3: min_level =  9; spell = "cause critical"; break;
	case  4: min_level = 10; spell = "dispel evil";    break;
	case  5: min_level = 12; spell = "curse";          break;
	case  6: min_level = 12; spell = "change sex";     break;
	case  7: min_level = 13; spell = "flamestrike";    break;
	case  8:
	case  9:
	case 10: min_level = 15; spell = "harm";           break;
	case 11: min_level = 15; spell = "plague";	   break;
	default: min_level = 16; spell = "dispel magic";   break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;

    if (check_absorb(ch,victim,sn))
	return FALSE;

    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}

bool spec_cast_judge( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
        return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
        v_next = victim->next_in_room;
        if ( victim->fighting == ch && number_bits( 2 ) == 0 )
            break;
    }

    if ( victim == NULL )
        return FALSE;

    spell = "high explosive";
    if ( ( sn = skill_lookup( spell ) ) < 0 )
        return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}



bool spec_cast_mage( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  0; spell = "blindness";      break;
	case  1: min_level =  3; spell = "chill touch";    break;
	case  2: min_level =  7; spell = "weaken";         break;
	case  3: min_level =  8; spell = "burning hands";  break;
	case  4: min_level = 11; spell = "colour spray";   break;
	case  5: min_level = 12; spell = "change sex";     break;
	case  6: min_level = 13; spell = "energy drain";   break;
	case  7: min_level = 13; spell = "earthmaw";   break;
	case  8: min_level = 15; spell = "fire and ice";	   break;
	case  9: min_level = 18; spell = "teleport";       break;
	case 10: min_level = 20; spell = "plague";	   break;
	default: min_level = 20; spell = "acid blast";     break;
	}

	if ( ch->level >= min_level )
	    break;
    }


    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    if (check_absorb(ch,victim,sn))
	return FALSE;

    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}



bool spec_cast_undead( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  0; spell = "curse";          break;
	case  1: min_level =  3; spell = "weaken";         break;
	case  2: min_level =  6; spell = "chill touch";    break;
	case  3: min_level =  9; spell = "blindness";      break;
	case  4: min_level = 12; spell = "poison";         break;
	case  5: min_level = 15; spell = "energy drain";   break;
	case  6: min_level = 18; spell = "harm";           break;
	case  7: min_level = 21; spell = "teleport";       break;
	case  8: min_level = 20; spell = "plague";	   break;
	default: min_level = 18; spell = "harm";           break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    if (check_absorb(ch,victim,sn))
	return FALSE;

    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}


bool spec_executioner( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *crime;

    if ( !IS_AWAKE(ch) || ch->fighting != NULL )
	return FALSE;

    crime = "";
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

	if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_CRIMINAL)
	&&   can_see(ch,victim))
	    { crime = "KILLER"; break; }

	if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_CRIMINAL)
	&&   can_see(ch,victim))
	    { crime = "THIEF"; break; }
    }

    if ( victim == NULL )
	return FALSE;

    sprintf( buf, "%s is a criminal!  PROTECT THE INNOCENT!  MORE BLOOOOD!!!",
	victim->name);
    REMOVE_BIT(ch->comm,COMM_NOSHOUT);
    do_yell( ch, buf );
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return TRUE;
}



bool spec_fido( CHAR_DATA *ch )
{
    OBJ_DATA *corpse;
    OBJ_DATA *c_next;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    if ( !IS_AWAKE(ch) )
	return FALSE;

    for ( corpse = ch->in_room->contents; corpse != NULL; corpse = c_next )
    {
	c_next = corpse->next_content;
	if ( corpse->item_type != ITEM_CORPSE_NPC )
	    continue;

	act( "$n savagely devours a corpse.", ch, NULL, NULL, TO_ROOM );
	for ( obj = corpse->contains; obj; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    obj_from_obj( obj );
	    obj_to_room( obj, ch->in_room );
	}
	extract_obj( corpse );
	return TRUE;
    }

    return FALSE;
}



bool spec_guard( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    CHAR_DATA *ech;
    char *crime;
    int max_evil;

    if ( !IS_AWAKE(ch) || ch->fighting != NULL )
	return FALSE;

    max_evil = 300;
    ech      = NULL;
    crime    = "";

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

	if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_CRIMINAL)
	&&   can_see(ch,victim))
	    { crime = "KILLER"; break; }

	if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_CRIMINAL)
	&&   can_see(ch,victim))
	    { crime = "THIEF"; break; }

	if ( victim->fighting != NULL
	&&   victim->fighting != ch
	&&   victim->alignment < max_evil )
	{
	    max_evil = victim->alignment;
	    ech      = victim;
	}
    }

    if ( victim != NULL )
    {
	sprintf( buf, "%s is a criminal!  PROTECT THE INNOCENT!!  BANZAI!!",
	    victim->name);
 	REMOVE_BIT(ch->comm,COMM_NOSHOUT);
	do_yell( ch, buf );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	return TRUE;
    }

    if ( ech != NULL )
    {
	act( "$n screams 'PROTECT THE INNOCENT!!  BANZAI!!",
	    ch, NULL, NULL, TO_ROOM );
	multi_hit( ch, ech, TYPE_UNDEFINED );
	return TRUE;
    }

    return FALSE;
}



bool spec_janitor( CHAR_DATA *ch )
{
    OBJ_DATA *trash;
    OBJ_DATA *trash_next;

    if ( !IS_AWAKE(ch) )
	return FALSE;

    for ( trash = ch->in_room->contents; trash != NULL; trash = trash_next )
    {
	trash_next = trash->next_content;
	if ( !IS_SET( trash->wear_flags, ITEM_TAKE ) || !can_loot(ch,trash))
	    continue;
	if ( trash->item_type == ITEM_DRINK_CON
	||   trash->item_type == ITEM_TRASH
	||   trash->cost < 10 )
	{
	    act( "$n picks up some trash.", ch, NULL, NULL, TO_ROOM );
	    obj_from_room( trash );
	    obj_to_char( trash, ch );
	    return TRUE;
	}
    }

    return FALSE;
}



bool spec_mayor( CHAR_DATA *ch )
{
    static const char open_path[] =
	"W3a3003b33000c111d0d111Oe333333Oe22c222112212111a1S.";

    static const char close_path[] =
	"W3a3003b33000c111d0d111CE333333CE22c222112212111a1S.";

    static const char *path;
    static int pos;
    static bool move;

    if ( !move )
    {
	if ( time_info.hour ==  6 )
	{
	    path = open_path;
	    move = TRUE;
	    pos  = 0;
	}

	if ( time_info.hour == 20 )
	{
	    path = close_path;
	    move = TRUE;
	    pos  = 0;
	}
    }

    if ( ch->fighting != NULL )
	return spec_cast_mage( ch );
    if ( !move || ch->position < POS_SLEEPING )
	return FALSE;

    switch ( path[pos] )
    {
    case '0':
    case '1':
    case '2':
    case '3':
	move_char( ch, path[pos] - '0', FALSE );
	break;

    case 'W':
	ch->position = POS_STANDING;
	act( "$n awakens and groans loudly.", ch, NULL, NULL, TO_ROOM );
	break;

    case 'S':
	ch->position = POS_SLEEPING;
	act( "$n lies down and falls asleep.", ch, NULL, NULL, TO_ROOM );
	break;

    case 'a':
	act( "$n says 'Hello Honey!'", ch, NULL, NULL, TO_ROOM );
	break;

    case 'b':
	act( "$n says 'What a view!  I must do something about that dump!'",
	    ch, NULL, NULL, TO_ROOM );
	break;

    case 'c':
	act( "$n says 'Vandals!  Youngsters have no respect for anything!'",
	    ch, NULL, NULL, TO_ROOM );
	break;

    case 'd':
	act( "$n says 'Good day, citizens!'", ch, NULL, NULL, TO_ROOM );
	break;

    case 'e':
	act( "$n says 'I hereby declare the city of Midgaard open!'",
	    ch, NULL, NULL, TO_ROOM );
	break;

    case 'E':
	act( "$n says 'I hereby declare the city of Midgaard closed!'",
	    ch, NULL, NULL, TO_ROOM );
	break;

    case 'O':
/*	do_unlock( ch, "gate" ); */
	do_open( ch, "gate" );
	break;

    case 'C':
	do_close( ch, "gate" );
/*	do_lock( ch, "gate" ); */
	break;

    case '.' :
	move = FALSE;
	break;
    }

    pos++;
    return FALSE;
}



bool spec_poison( CHAR_DATA *ch )
{
    CHAR_DATA *victim;

    if ( ch->position != POS_FIGHTING
    || ( victim = ch->fighting ) == NULL
    ||   number_percent( ) > 2 * ch->level )
	return FALSE;

    act( "You bite $N!",  ch, NULL, victim, TO_CHAR    );
    act( "$n bites $N!",  ch, NULL, victim, TO_NOTVICT );
    act( "$n bites you!", ch, NULL, victim, TO_VICT    );
    spell_poison( gsn_poison, ch->level, ch, victim,TARGET_CHAR);
    return TRUE;
}



bool spec_thief( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    long gold,silver;

    if ( ch->position != POS_STANDING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

	if ( IS_NPC(victim)
	||   victim->level >= LEVEL_IMMORTAL
	||   number_bits( 5 ) != 0
	||   !can_see(ch,victim))
	    continue;

	if ( IS_AWAKE(victim) && number_range( 0, ch->level ) == 0 )
	{
	    act( "You discover $n's hands in your wallet!",
		ch, NULL, victim, TO_VICT );
	    act( "$N discovers $n's hands in $S wallet!",
		ch, NULL, victim, TO_NOTVICT );
	    return TRUE;
	}
	else
	{
	    gold = victim->gold * UMIN(number_range(1,20),ch->level / 2) / 100;
	    gold = UMIN(gold, ch->level * ch->level * 10 );
	    ch->gold     += gold;
	    victim->gold -= gold;
	    silver = victim->silver * UMIN(number_range(1,20),ch->level/2)/100;
	    silver = UMIN(silver,ch->level*ch->level * 25);
	    ch->silver	+= silver;
	    victim->silver -= silver;
	    return TRUE;
	}
    }

    return FALSE;
}


bool spec_enforcer( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *v_next;

    if ( !IS_AWAKE(ch) || ch->fighting != NULL )
	return FALSE;

    if (ch->leader != NULL && IS_SET(ch->leader->act,PLR_CRIMINAL))
    {
	send_to_char("Guards will not follow a CRIMINAL!\n\r",ch->leader);
	REMOVE_BIT(ch->affected_by,AFF_CHARM);
	do_follow(ch,"self");
    }

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

	if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_CRIMINAL)
	&&   can_see(ch,victim))
		break;
    }

    if ( victim != NULL )
    {
	sprintf( buf, "%s is a criminal!  PROTECT THE INNOCENT!!  BANZAI!!",
	    victim->name);
 	REMOVE_BIT(ch->comm,COMM_NOSHOUT);
	do_yell( ch, buf );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	return TRUE;
    }

    return FALSE;
}

/* Spec fun for preserver */

bool spec_preserver(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn, level;

    level = 56;

    if (ch->position != POS_FIGHTING)
	return FALSE;
    for (victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
	v_next = victim->next_in_room;
	if (victim->fighting == ch && number_bits(2) == 0)
		break;
    }

    if (victim == NULL)
	return FALSE;
	spell = "acid blast";

	switch(number_range(0,6))
  	{
	case (0):
	case (1):
	case (2):
	level = 60;	spell = "acid blast";	break;
	case (3):
	level = 63;	spell = "firestream";	break;
	case (4):
	level = 57;	spell = "earthmaw";	break;
	case (5):
	level = 55;	spell = "blindness";	break;
	case (6):
	level = 55;	spell = "concatenate";	break;
    	}

    if ( (sn = skill_lookup(spell)) < 0)
	return FALSE;
    if (check_absorb(ch,victim,sn))
	return FALSE;

    (*skill_table[sn].spell_fun) ( sn, level, ch, victim,TARGET_CHAR);
    return TRUE;
}


bool spec_protector(CHAR_DATA *ch)
{
    char buf[MAX_STRING_LENGTH];
    int sn_hand, number;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    if (ch->fighting == NULL)
        return FALSE;

    for (; ;)
    {
        for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
	{
            vch_next = vch->next_in_room;
            if (vch->fighting == ch && number_bits(2) == 0)
                break;
	}
        if (vch != NULL)
            break;
    }

    number = number_percent();
    if (number < 5)
    {
    do_say(ch,"You are breaking the Law! Leave now or be punished!");
    return TRUE;
    }
    else if (number < 10 && number_bits(2) == 0)
    {
        sprintf(buf,"%s, you must pay for your crimes!",vch->name);
        do_say(ch,buf);
        do_wanted(ch,vch->name);
    }

    if (IS_SET(vch->act,PLR_CRIMINAL))
    {
        sn_hand = skill_lookup("hand of vengeance");
        act("$n points at $N and says, 'Defend the Protectorate!'",ch,0,vch,TO_NOTVICT);
        act("$n points at you and says, 'Defend the Protectorate!",ch,0,vch,TO_VICT);
        (*skill_table[sn_hand].spell_fun) (sn_hand,ch->level,ch,vch,TARGET_CHAR);
        return TRUE;
    }

    return spec_breath_lightning(ch);
    return FALSE;
}



bool spec_fallen_angel(CHAR_DATA *ch)
{
    int num, sn_unholy_fire, sn_windwall;

    if (ch->fighting == NULL)
        return FALSE;
    num = number_percent();

    if (num >= 50)
	return FALSE;

    sn_unholy_fire = skill_lookup("unholy fire");
    sn_windwall = skill_lookup("windwall");

    if (num < 10)
    {
    do_say(ch,"Burn in Hell!");
    (*skill_table[sn_unholy_fire].spell_fun) ( sn_unholy_fire, 60, ch, NULL, TARGET_CHAR);
    return TRUE;
    }
    else if (num < 30)
    {
        act("$n rise up on $s blackened wings and beats them into the air!",ch,0,0,TO_ROOM);
        (*skill_table[sn_windwall].spell_fun) (sn_windwall, 60,ch,NULL,TARGET_CHAR);
        return TRUE;
    }
    else if (num < 50)
    {
        act("$n hisses and a steaming green gas explodes forth!",ch,0,0,TO_ROOM);
        return spec_breath_gas(ch);
    }

    return FALSE;
}

void spell_unholy_fire(int sn, int level, CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam, tmp_dam;

    act("Black flames erupt from the ground to engulf the room!",ch,0,0,TO_ROOM);
    send_to_char("Black flames erupt from the ground to engulf the room!\n\r",ch);
    dam = dice(level,6);

    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next_in_room;
        if (vch == ch)  continue;
        tmp_dam = dam;
        if (saves_spell(level,vch,DAM_NEGATIVE))
            tmp_dam /= 2;
        damage_old(ch,vch,dam,sn,DAM_NEGATIVE,TRUE);
    }
    return;
}




/* Spec_fun for Dead Forest area */

bool spec_troll(CHAR_DATA *ch)
{
    int action;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;

    return FALSE;

    if (!IS_AWAKE(ch))
	return FALSE;

    action = number_range(0,3);

    if (ch->fighting != NULL)
        action += 4;

    if (action <= 3 && number_percent() > 10)
        return FALSE;
    vch = NULL;
    if (action >= 4)
    {
        for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
        {
            vch_next = vch->next_in_room;
            if (vch->fighting == ch && number_bits(2) == 0)
                break;
        }
    }

    if (action >= 4 && vch == NULL)
	return FALSE;

    switch(action)
    {
        case (0):
        act("$n picks up a piece of dead bark and licks the ichor off it.",ch,0,0,TO_ROOM);
        break;
        case (1):
        act("$n snarls and sniffs the air expecantly.",ch,0,0,TO_ROOM);
        break;
        case (2):
        act("$n licks at a pool of dark green blood.",ch,0,0,TO_ROOM);
        break;
        case (3):
        act("$n scratches at one of it's bloody scabs.",ch,0,0,TO_ROOM);
        break;
        case (4):  /* Combat */
        case (5):
        (*skill_table[gsn_acid_spit].spell_fun) (gsn_acid_spit,ch->level,ch,vch,TARGET_CHAR);
        break;
        case (6): /* Combat */
        case (7):
        do_crush(ch,"");
        break;
    }
    return TRUE;
}

/* For spec_troll  */
void spell_acid_spit(int sn,int level, CHAR_DATA *ch,void *vo, int target)
{
    int dam;
    AFFECT_DATA af;
    CHAR_DATA *victim = (CHAR_DATA *) vo;

        init_affect(&af);
    af.where = TO_AFFECTS;
        af.aftype    = AFT_MALADY;
    af.type = sn;
    af.duration = 1;
    af.modifier = -2;
    af.location = APPLY_HITROLL;
    af.bitvector = AFF_BLIND;

    act("$n spits out a spray of burning acid!",ch,0,0,TO_ROOM);
    send_to_char("You spit forth a spray of acid!\n\r",ch);
    if (saves_spell(level,victim,DAM_ACID))
        damage_old(ch,victim,0,sn,DAM_ACID,TRUE);
    else
        {
        dam = dice(4,5);
        damage_old(ch,victim,dam,sn,DAM_ACID,TRUE);
        if (!IS_AFFECTED(victim,AFF_BLIND))
            {
            affect_to_char(victim,&af);
            send_to_char("You are blinded by the searing acid!\n\r",victim);
            act("$n appears blinded by the acid.",victim,0,0,TO_ROOM);
            }
        }

   return;
}


bool spec_monk(CHAR_DATA *ch)
{
    CHAR_DATA *vch;
    int action;


    if ( (vch = ch->fighting) == NULL)
        return FALSE;

    action = number_range(0,2);
    if (is_affected(ch,gsn_palm))
        action += 1;

    switch(action)
    {
        case (0):   do_palm(ch,""); break;
        case (1):   do_throw(ch,"");    break;
        case (2):   case (3):
        do_kick(ch,""); break;
    }
    return TRUE;
}

bool spec_chimera(CHAR_DATA *ch)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int action, sn;

    if (ch->fighting == NULL)
        return FALSE;

    action = number_range(0,3);
    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next_in_room;
        if (vch->fighting == ch && number_bits(2) == 0)
            break;
    }
	if (vch == NULL)
		vch = ch->fighting;

    switch(action)
    {
        case (0):
        do_ram(ch,vch->name);
        break;
        case (1):
        act("$n's dragon head opens it's giant maw.",ch,0,0,TO_ROOM);
        sn = skill_lookup("fire breath");
        (*skill_table[sn].spell_fun) (sn,ch->level,ch,vch,TARGET_CHAR);
        break;
        case (2):
        do_tail(ch,vch->name);
        break;
    }

    return TRUE;
}



bool evades(CHAR_DATA *ch, CHAR_DATA *victim)
{
    int chance;

    chance = 50 + ((ch->level - victim->level) * 2);
    chance += get_curr_stat(ch,STAT_DEX)/3;
    chance -= get_curr_stat(victim,STAT_DEX)/2;
    if (number_percent() > chance)
        return TRUE;
    return FALSE;
}

void do_ram(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *vch;
    char arg[MAX_INPUT_LENGTH];
    int dam;

    one_argument(argument,arg);

    vch = get_char_room(ch,arg);
	if (arg[0] == '\0')
		vch = ch->fighting;

    if (IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM))
	return;

    if (!IS_NPC(ch))
    {
        if (get_skill(ch,gsn_chimera_goat) == 0)
        {
            send_to_char("Huh?\n\r",ch);
            return;
        }
    }
    if (vch == NULL)
    {
        send_to_char("You aren't fighting anyone.\n\r",ch);
        return;
    }
    if (!IS_NPC(ch) && is_safe(ch,vch))
        return;

        if (evades(ch,vch))
        {
            act("$n brings $s's goat head around in a violent swing but misses.",ch,0,0,TO_ROOM);
            send_to_char("You bring your goat's head around violently but miss.\n\r",ch);
            dam = 0;
        }
        else
        {
            act("$n slams $s's goat head into $N, sending $S sprawling!",ch,0,vch,TO_NOTVICT);
            act("$n slams $s's goat head into you, sending you sprawling!",ch,0,vch,TO_VICT);
            act("You slam your goat head into $N, sending $M sprawling!",ch,0,vch,TO_CHAR);
            dam = dice(ch->level,4);
            WAIT_STATE(vch,2*PULSE_VIOLENCE);
        }
        damage_old(ch,vch,dam,gsn_chimera_goat,DAM_BASH,TRUE);
        WAIT_STATE(ch,2*PULSE_VIOLENCE);
     return;
}


void do_bite(CHAR_DATA *ch, char *argument)
{
        CHAR_DATA *vch;
        char arg[MAX_INPUT_LENGTH];

    int dam;
        one_argument(argument,arg);

    if (IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM))
	return;

        if (!IS_NPC(ch) && get_skill(ch,gsn_chimera_lion) == 0)
        {
            send_to_char("Huh?\n\r",ch);
            return;
        }
	vch = get_char_room(ch,arg);
	if (arg[0] == '\0')
		vch = ch->fighting;
	if (vch == NULL)
	{
	send_to_char("They aren't here.\n\r",ch);
	return;
	}


        act("$n brings $s head around to bite $N!",ch,0,vch,TO_NOTVICT);
        act("$n brings $s head around to bit you!",ch,0,vch,TO_VICT);
        act("You bite down savagely at $N!",ch,0,vch,TO_CHAR);
        if (evades(ch,vch))
            dam = 0;
        else
            dam = dice(ch->level,5);
        damage_old(ch,vch,dam,gsn_chimera_lion,DAM_PIERCE,TRUE);
	WAIT_STATE(ch,2*PULSE_VIOLENCE);
        return;
}


bool spec_darokin(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;
    int sn_elemental;
    int val;
    char buf[MAX_STRING_LENGTH];

    sn_elemental = 0;
    val = number_range(0,8);
    if (val < 6)
	sn_elemental = skill_lookup("familiar");
/*
    sn_elemental = skill_lookup("summon earth elemental");
    else if (val == 1)
    sn_elemental = skill_lookup("summon air elemental");
    else if (val == 2)
    sn_elemental = skill_lookup("summon fire elemental");
    else if (val == 3)
        sn_elemental = skill_lookup("familiar");
    else
        sn_elemental = 0;
*/
    if (ch->position != POS_FIGHTING)
    {
        if (ch->last_fought == NULL)
            return FALSE;
        if (number_percent() < 20)
            return FALSE;
        if (ch->last_fought->in_room->area != ch->in_room->area)
            return FALSE;
            victim = ch->last_fought;
	if (victim->fighting != NULL)
		return FALSE;
	if (victim->position == POS_FIGHTING)
	return FALSE;
	if (saves_spell(ch->level,victim,DAM_OTHER))
		return FALSE;
        act("$n has summoned you!",ch,0,victim,TO_VICT);
        act("$n disappears!",victim,0,0,TO_ROOM);
        char_from_room(victim);
        char_to_room(victim,ch->in_room);
	do_look(victim,"auto");
        sprintf(buf,"Help! I'm being summoned by %s!",ch->short_descr);
        do_yell(victim,buf);
        multi_hit(ch,victim,-1);
        return TRUE;
    }


    for (victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
        v_next = victim->next_in_room;
        if (victim->fighting == ch && number_bits(2)  == 0)
            break;
    }
    if (victim == NULL && sn_elemental == 0)
        return FALSE;
    if (victim == NULL)
    {
        (*skill_table[sn_elemental].spell_fun) (sn_elemental,ch->level,ch,NULL,TARGET_CHAR);
        return TRUE;
    }

    switch (number_range(0,9))
    {
        default: spell = "concatenate"; break;
        case 0: case 1: spell = "concatenate";  break;
        case 2: case 3: spell = "fireball"; break;
        case 4: spell = "windwall"; break;
        case 5: spell = "blindness";    break;
        case 6: spell = "cone of cold"; break;
        case 7: spell = "acid blast";    break;
        case 8: case 9: spell = "firestream";   break;
    }

    if ( (sn = skill_lookup(spell)) < 0)
        return FALSE;
    (*skill_table[sn].spell_fun) (sn,ch->level,ch,victim,TARGET_CHAR);
    return TRUE;
}


bool spec_godfather(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if (IS_NPC(victim))
	continue;
	if ( victim->fighting == ch && number_bits(2) == 0)
	    break;
    }

    if ( victim == NULL )
	return FALSE;

sn = -1;

	sn = gsn_revolt;

    if ( sn < 0 )
	return FALSE;
    if (check_absorb(ch,victim,sn))
	return FALSE;

    (*skill_table[sn].spell_fun) ( sn, 52, ch, victim,TARGET_CHAR);
    return TRUE;
}


bool spec_bishop(CHAR_DATA *ch)
{
        OBJ_DATA *sceptre;
        int sn_frenzy;

        if (ch->fighting == NULL)
                return FALSE;

        sceptre = get_eq_char(ch,WEAR_HOLD);

        if (sceptre != NULL && sceptre->pIndexData->vnum == OBJ_VNUM_HEAVENLY_SCEPTRE
	&& number_bits(2) == 0)
        {
        sn_frenzy = skill_lookup("heavenly wrath");
        if (!is_affected(ch,sn_frenzy) && number_percent() < 50)
do_say(ch,"I am the wrath of god");
        else
do_say(ch,"Feel the force of god");
        return TRUE;
        }

        return (spec_cast_cleric(ch));

        return FALSE;
}



bool spec_bane(CHAR_DATA *ch)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int num, sn;
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *mob;

    num = 0;
    sn = -1;
    if (ch->fighting == NULL && ch->last_fought == NULL)
    {
    if (number_percent() > 2
    && number_percent() > 10)
        return FALSE;

    for (vch = char_list;vch != NULL; vch = vch->next)
        {
	if (vch->in_room ==NULL)
		continue;
        if (vch->in_room->area != ch->in_room->area
	|| IS_NPC(vch))
		continue;
        num = number_range(0,40);
        if (num < 15)
        send_to_char("You feel a cold chill pass over your soul as something unspeakable stirs nearby.\n\r",vch);
        else if (num < 20)
        send_to_char("A demonic laugh echoes softly around you, only to fade as a blue light flares in the distance.\n\r",vch);
        else if (num <= 25)
        send_to_char("You feel something powerful and malevolent watching you from the darkness around you.\n\r",vch);
        else if (num < 30)
        send_to_char("An awesome force of pure darkness clutches your soul but then passes on...\n\r",vch);
        else if (num < 35)
        send_to_char("Something powerful demands your servitude, but it quickly fades as a green light flashes in a nearby tower.\n\r",vch);
        else if (num <= 40)
        send_to_char("You feel the prescence of a divine power seeking something in the ruins...\n\r",vch);
        return TRUE;
        }
    }

    if (ch->position == POS_SLEEPING)
        return FALSE;

    if (ch->fighting == NULL)
    {
        for (vch = char_list; vch != NULL; vch = vch_next)
        {
	vch_next = vch->next_in_room;
	if (vch->in_room != ch->in_room)
		continue;
	if (!is_same_group(ch,vch))
		continue;
	if (number_bits(2) != 0)
		continue;
	sprintf(buf,"Help! %s is attacking me!",ch->short_descr);
	do_myell(vch,buf);

	multi_hit(ch,vch,TYPE_UNDEFINED);
	return TRUE;
	}
     }

    if (ch->fighting == NULL && ch->last_fought != NULL)
    {
        if (number_percent() > 5)
            return FALSE;
        for (vch = char_list; vch != NULL; vch = vch_next)
        {
            vch_next = vch->next;
            if (vch->in_room->area != ch->in_room->area)
                continue;
            if (is_same_group(vch,ch->last_fought)
            && !saves_spell(ch->level,vch,DAM_OTHER) )
            {
act("$n suddenly disappears!",vch,0,0,TO_ROOM);
send_to_char("You have been summoned!\n\r",vch);
char_from_room(vch);
char_to_room(vch,ch->in_room);
do_look(vch,"auto");
act("$n arrives suddenly!",vch,0,0,TO_ROOM);
num = number_percent();
if (num < 30)
sprintf(buf,"Help! I've been summoned by %s!",PERS(ch,vch));
else if (num < 70)
sprintf(buf,"Help! I'm being attacked by %s!",PERS(ch,vch));
else
sprintf(buf,"NO! I'm being slayed by %s!",PERS(ch,vch));
do_myell(vch,buf);
multi_hit(ch,vch,TYPE_UNDEFINED);
            }
        }
        return TRUE;
    }


    num = number_percent();
    if (num < 10)
        sn = skill_lookup("deathspell");
    else if (num < 20)
        sn = skill_lookup("acid blast");
    else if (num < 30)
        sn = skill_lookup("demonfire");
    else if (num < 40)
        sn = skill_lookup("iceball");
    else if (num < 50)
        sn = skill_lookup("fireball");
    else if (num < 80)
        sn = -1;
    else if (num < 90)
	{
	do_say(ch,"Burn in the fires of hell!");
        sn = skill_lookup("unholy fire");
	}

    if (sn == -1)
    {
        for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
        {
            if (IS_NPC(vch) && vch->pIndexData->vnum == MOB_VNUM_SLAYER)
                    break;
        }
        if (vch != NULL)
            return FALSE;
        if (number_percent() > 80)
        {
            act("$n's eye's glow bright red but nothing happens.",ch,0,0,TO_ROOM);
            return TRUE;
        }
        act("$n's eyes glow bright red and a dark figure materialises beside $s!",ch,0,0,TO_ROOM);
mob = create_mobile(get_mob_index(MOB_VNUM_SLAYER));
char_to_room(mob,ch->in_room);
mob->leader = ch;
mob->master = ch;
SET_BIT(mob->affected_by, AFF_CHARM);
return TRUE;
    }


    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next_in_room;
        if (vch->fighting == ch && number_bits(2) == 0)
            break;
    }

    if (vch == NULL && sn != skill_lookup("deathspell")
    && sn != skill_lookup("iceball") && sn != skill_lookup("fireball"))
        return FALSE;

    (*skill_table[sn].spell_fun) (sn,ch->level,ch,vch,skill_table[sn].target);
    return TRUE;
}



bool spec_rakshasa(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;

    if (ch->position != POS_FIGHTING)
        return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
    return FALSE;
    if (number_percent() < 20)
        do_bite(ch,victim->name);
    else return (spec_cast_cleric(ch));

    return TRUE;
}

bool spec_rakshasa_royal(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    int num;
    int sn, lvl;
    char buf[MAX_STRING_LENGTH];

    if (ch->position != POS_FIGHTING)
        return FALSE;
    sn = -1;
    return (spec_rakshasa(ch));
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
    return FALSE;

    if (number_percent() < 20)
    {
        do_bite(ch,victim->name);
        return TRUE;
    }
     buf[0] = '\0';
    for (; ;)
    {
    num = number_range(0,11);
    if (num == 12)
        return FALSE;
    if ((num == 0 || num == 1) && !IS_AFFECTED(victim,AFF_CURSE) )
        break;
    if ((num == 2 || num == 3) && !IS_AFFECTED(victim,AFF_SLOW))
        break;
    if (num >= 4)
        break;
    }

    lvl = ch->level;
    if (num == 0 || num == 1)
    {
do_say(ch,"I curse thee mortal fool!");
sn = gsn_curse;
    }
    else if (num == 2 || num == 3)
    {
        sprintf(buf,"Stay still weakling mortal!");
        do_say(ch,buf);
        sn = skill_lookup("slow");
        lvl += 5;
    }
    else if (num == 4 || num == 5 )
    {
do_say(ch,"What a tasty meal you will make!");
    sn = skill_lookup("acid blast");
    lvl = 55;
    }
    else if (num == 6)
    {
    do_say(ch,"Begone from here intruder!");
    sn = skill_lookup("teleport");
    lvl = 50;
    }
    else if (num == 7 || num == 8 )
    {
act("$n opens $s mouth and a raging fireball explodes forth!",ch,0,0,TO_ROOM);
    sn = skill_lookup("fireball");
    lvl = 55;
    }
    else if (num == 9 || num == 10)
    {
        sn = skill_lookup("blindness");
        lvl = 55;
    }
    else if (num == 11)
    {
        sn = skill_lookup("energy drain");
    }

    if (sn < 0)
        return FALSE;

    (*skill_table[sn].spell_fun) (sn,lvl,ch,victim,skill_table[sn].target);
    return TRUE;
}

bool spec_rakshasa_lich(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    int num;
   int sn,lvl;
    char buf[MAX_STRING_LENGTH];

    if (ch->position != POS_FIGHTING)
        return FALSE;
    sn = -1;
    return (spec_rakshasa(ch));

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
    return FALSE;

    for (; ;)
    {
    num = number_range(0,11);
    if (num == 12)
        return FALSE;
    if ((num == 0 || num == 1) && !IS_AFFECTED(victim,AFF_CURSE) )
        break;
    if ((num == 2 || num == 3) && !IS_AFFECTED(victim,AFF_SLOW))
        break;
    if (num >= 4)
        break;
    }
     buf[0] = '\0';

    lvl = ch->level;
    if (num == 0 || num == 1)
    {
do_say(ch,"I curse thee mortal fool!");
sn = gsn_curse;
    }
    else if (num == 2 || num == 3)
    {
        do_say(ch,"Stay still impudent mortal!");
        sn = skill_lookup("slow");
        lvl += 5;
    }
    else if (num == 4 || num == 5 )
    {
    if (!IS_NPC(victim))
    return TRUE;
    if (number_percent() < 50)
sprintf(buf,"Your corpse shall make a fine servant!");
else
sprintf(buf,"It is time for you to be taught a lesson!");

do_say(ch,buf);
    sn = skill_lookup("acid blast");
    lvl = 60;
    }
    else if (num == 6)
    {
    sn = skill_lookup("demonfire");
    lvl = 50;
    }
    else if (num == 7 || num == 8 )
    {
act("$n opens $s mouth and a raging fireball explodes forth!",ch,0,0,TO_ROOM);
    sn = skill_lookup("fireball");
    lvl = 55;
    }
    else if (num == 9 || num == 10)
    {
        sn = skill_lookup("blindness");
        lvl = 55;
    }
    else if (num == 11)
    {
        sn = skill_lookup("energy drain");
    }

    if (sn < 0)
        return FALSE;

    (*skill_table[sn].spell_fun) (sn,lvl,ch,victim,skill_table[sn].target);
    return TRUE;
}

/*
 * spec_fun for beholders, can use up to 3 eyes per round. Had to
 * mess with the spell selection to prevent same spell being used by mutliple
 * eyes though. Messy chunky code.
*/
bool spec_beholder(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *victim1;
    CHAR_DATA *victim2;
    CHAR_DATA *victim3;
    CHAR_DATA *v_next;
    char *spell1;
    char *spell2;
    char *spell3;
    int i;
    int sn1, sn2, sn3;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    sn1 = -1;    sn2 = -1;    sn3 = -1;
    victim1 = NULL;
    victim2 = NULL;
    victim3 = NULL;
    for (i = 0; i < 3; i++)
    {
        if (victim3 != NULL)
            break;
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
    if ( victim->fighting == ch && number_percent() < 30 )
        {
        if (victim1 == NULL)
            victim1 = victim;
        else if (victim2 == NULL)
            victim2 = victim;
        else if (victim3 == NULL)
            victim3 = victim;
        else
            break;
        }
    }
    }

    if ( victim1 == NULL )
	return FALSE;

    switch(number_bits(4))
    {
        case 0: spell1 = "blindness";    break;
        case 1: spell1 = "energy drain"; break;
        case 2: spell1 = "harm"; break;
        case 3: spell1 = "weaken";   break;
	case 4: spell1 = "harm"; break;
        case 5: spell1 = "slow"; break;
        case 6: spell1 = "change sex";   break;
        case 7: spell1 = "fireball"; break;
        case 8: spell1 = "iceball";  break;
        default: spell1 = "dispel magic";    break;
    }
    sn1 = skill_lookup(spell1);


    if (victim2 != NULL)
    {
        for (; ;)
        {
        switch(number_bits(4))
        {
            case 0: spell2 = "blindness";    break;
            case 1: spell2 = "energy drain"; break;
            case 2: spell2 = "harm"; break;
            case 3: spell2 = "weaken";   break;
	    case 4: spell2 = "harm"; break;
            case 5: spell2 = "slow"; break;
            case 6: spell2 = "change sex";   break;
            case 7: spell2 = "fireball"; break;
            case 8: spell2 = "iceball";  break;
            default: spell2 = "dispel magic";    break;
        }
        sn2 = skill_lookup(spell2);
        if (sn2 != sn1)
            break;
        }
    }

    if (victim3 != NULL)
    {
    for (; ;)
    {
            switch(number_bits(4))
        {
            case 0: spell3 = "blindness";    break;
            case 1: spell3 = "energy drain"; break;
            case 2: spell3 = "harm"; break;
            case 3: spell3 = "weaken";   break;
	    case 4: spell3 = "harm"; break;
            case 5: spell3 = "slow"; break;
            case 6: spell3 = "change sex";   break;
            case 7: spell3 = "fireball"; break;
            case 8: spell3 = "iceball";  break;
            default: spell3 = "dispel magic";    break;
        }
        sn3 = skill_lookup(spell3);
        if (sn3 != sn2 && sn3 != sn1)
            break;
    }
    }

    act("$n turns one of it's eyestalks upon $N.",ch,0,victim1,TO_NOTVICT);
    act("$n turns one of it's eyestalks upon you.",ch,0,victim1,TO_VICT);

    if ( sn1 >= 0 )
        (*skill_table[sn1].spell_fun) ( sn1, ch->level, ch, victim1,TARGET_CHAR );

    if (victim2 != NULL)
    {
    if ( sn2 >= 0 )
        {
        (*skill_table[sn2].spell_fun) ( sn1, ch->level, ch,victim2,skill_table[sn2].target );
        }
    }
    if (victim3 != NULL)
    {
    if ( sn3 >= 0 )
        {
    act("$n turns one of it's eyestalks upon $N.",ch,0,victim1,TO_NOTVICT);
    act("$n turns one of it's eyestalks upon you.",ch,0,victim1,TO_VICT);
        (*skill_table[sn3].spell_fun) ( sn3, ch->level, ch,victim3,skill_table[sn3].target );
        }
    }

    return TRUE;
}

/* deathkngiht special */

bool spec_deathknight(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    int sn, lvl;
    sn = -1;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
        return (spec_room_throw(ch));

    lvl = ch->level;

    switch(number_range(0,16))
    {
    case (0):
do_say(ch,"Die!");
    sn = skill_lookup("power word kill");   lvl = 50;   break;
    case (1):
    case (2):
    case (3):
    case (4):
    case (5):
do_say(ch,"Fire!");
    sn = skill_lookup("fireball");   break;
    case (6):
    case (7):
    case (8):
    case (9):
do_say(ch,"Ice!");
    sn = skill_lookup("iceball");    break;
    case (10):
    case (11):
    case (12):
    sn = skill_lookup("curse");  break;
    case (13):
    case (14):
    sn = skill_lookup("energy drain");   break;
    case (15):
    case (16):
    sn = skill_lookup("wither"); lvl = 51;   break;
    }

    if (sn < 0)
        return FALSE;
    (*skill_table[sn].spell_fun) (sn,lvl,ch,victim,skill_table[sn].target);
    return TRUE;
}

/* for spectre king in Myth Drannor */
bool spec_spectre_king(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    switch ( number_range(0,8) )
	{
    case  0: spell = "blindness";      break;
    case  1: spell = "chill touch";    break;
    case  2: spell = "iceball";         break;
    case  3: spell = "energy drain";       break;
    case  4: spell = "cone of cold";   break;
    case  5: spell = "acid blast";   break;
    default: spell = "frost breath";     break;
    }


    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim, TAR_CHAR_OFFENSIVE );
    return TRUE;
}

/* throws a victim out of room..problems with this somewhere but I'm too
lazy to fix 'em (Ceran) */

bool spec_room_throw(CHAR_DATA *ch)
{
    ROOM_INDEX_DATA *was_in;
    CHAR_DATA *victim;
    int attempt, chance, dam, level;
/*    int rev_door; */


    if (ch->position != POS_FIGHTING)
        return FALSE;
    if (number_bits(2) != 0)
	return FALSE;
    return FALSE;
    victim = ch->fighting;
    level = ch->level + 3;
    chance = (ch->level - victim->level)*3;
    chance += number_range(20,50);
    chance -= get_curr_stat(victim,STAT_DEX)/2;
    chance -= get_curr_stat(victim,STAT_STR)/3;
    chance += get_curr_stat(ch,STAT_STR);

    if (IS_AFFECTED(victim,AFF_HASTE))
        chance -= 30;
    if (IS_AFFECTED(ch,AFF_HASTE))
        chance += 15;
    if (IS_AFFECTED(victim,AFF_SLOW))
        chance += 20;
    if (IS_AFFECTED(ch,AFF_SLOW))
        chance -= 30;
    if (IS_AFFECTED(victim,AFF_FLYING))
        chance += 5;
    chance += (ch->size - victim->size)*10;

        if (number_percent() > chance)
        {
        act("$n grabs at $N but looses hold.",ch,0,victim,TO_NOTVICT);
        act("$n grabs at you but looses hold.",ch,0,victim,TO_VICT);
        return FALSE;
        }

    was_in = victim->in_room;
    dam = dice(level,5);

    for ( attempt = 0; attempt < 6; attempt++ )
    {
	EXIT_DATA *pexit;
	int door;

	door = number_door( );

	if ( ( pexit = was_in->exit[door] ) == 0
	||   pexit->u1.to_room == NULL
        ||  ( IS_SET(pexit->exit_info, EX_CLOSED) )
	|| ( IS_NPC(victim)
	&&   IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB) ) )
	    continue;

        if ( ( was_in->sector_type == SECT_AIR)
        ||  ( (pexit->u1.to_room->sector_type == SECT_AIR )
        &&  ( !IS_AFFECTED(victim, AFF_FLYING) ) ) )
            continue;

        if (IS_SET(pexit->exit_info,EX_CLOSED))
        {
        act(
"$n grabs $N and sends $M crashing through the door with incredible force!",ch,0,victim,TO_NOTVICT);
act("$n grabs you and sends you crashing through the door with incredible force!",ch,0,victim,TO_VICT);
        }
        else
        {
        act( "$n grabs $N and hurls $M from the room with incredible force!", ch, NULL, victim, TO_NOTVICT);
        act("$n grabs you and hurls you from the room with incredible force!",ch,0,victim,TO_VICT);
        dam = dice(level,7);
        }
        char_from_room(victim);
        char_to_room(victim, pexit->u1.to_room);
	do_look(victim,"auto");
if (IS_SET(pexit->exit_info,EX_CLOSED))
{
/*
	if (door == 0)
		rev_door = 2;
	else if (door == 1)
		rev_door = 3;
	else if (door == 2)
		rev_door = 0;
	else if (door == 3)
		rev_door = 1;
	else if (door == 4)
		rev_door = 5;
	else
		rev_door = 4;
*/
        REMOVE_BIT(pexit->exit_info,EX_CLOSED);
/*
	if (victim->in_room->exit[rev_door] != NULL)
	REMOVE_BIT(victim->in_room->exit[rev_door]->exit_info,EX_CLOSED);
*/
        act("With a deafening crash the door explodes open and $n comes flying into the room!",ch,0,0,TO_ROOM);
}
else
{
        act("$n comes flying into the room with incredible force!",ch,0,0,TO_ROOM);
}
        if (saves_spell(ch->level - 3,victim,DAM_BASH))
                dam /= 2;
        damage_old(ch,victim,dam,gsn_throw,DAM_BASH,TRUE);
        stop_fighting( ch, TRUE );
        return TRUE;
    }

    act("$n grabs $N and hurls $M brutally into a wall!",ch,0,victim,TO_NOTVICT);
    act("$n grabs you and hurtls you brutally into a wall!",ch,0,victim,TO_VICT);

        if (saves_spell(ch->level - 3,victim,DAM_BASH))
                dam /= 2;
        damage_old(ch,victim,dam,gsn_throw,DAM_BASH,TRUE);

    return TRUE;
}





/* hydra spec fun for myth drannor */

bool spec_hydra(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *victim1;
    CHAR_DATA *victim2;
    CHAR_DATA *victim3;
    CHAR_DATA *v_next;
    char *spell1;
    char *spell2;
    char *spell3;
    int level, i;
    int sn1, sn2, sn3;

    if ( ch->position != POS_FIGHTING  && ch->last_fought != NULL)
    return FALSE;

    if (ch->position != POS_FIGHTING)
    {
    if (number_percent() < 30)
        return FALSE;
        act("$n swings it's heads about in anger then turns and slips back to where it came.",ch,0,0,TO_ROOM);
        extract_char(ch,TRUE);
        return TRUE;
    }

    sn1 = 0;    sn2 = 0;    sn3 = 0;
    victim1 = NULL;
    victim2 = NULL;
    victim3 = NULL;
    level = ch->level;

    for (i = 0; i < 3; i++)
    {
        if (victim3 != NULL)
            break;
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
    if ( victim->fighting == ch && number_percent() < 30 )
        {
        if (victim1 == NULL)
            victim1 = victim;
        else if (victim2 == NULL)
            victim2 = victim;
        else if (victim3 == NULL)
            victim3 = victim;
        else
            break;
        }
    }
    }

    if ( victim1 == NULL )
	return FALSE;

    switch(number_range(0,3))
    {
        case 0:
        spell1 = "acid blast"; break;
        case 1:
        spell1 = "fireball"; level = 50;   break;
        case 3:
        spell1 = "flame scorch"; break;
        default:
        spell1 = "flame scorch"; break;
    }
    sn1 = skill_lookup(spell1);
    spell2 = "weaken";

    if (victim2 != NULL && (sn1 != gsn_flame_scorch))
    {
        switch(number_range(0,3))
        {
        case 1:
        spell2 = "poison"; level = 53; break;
        case 2:
        spell2 = "slow"; level = 55;   break;
        case 3:
        spell2 = "weaken";  level = 55; break;
        }
        sn2 = skill_lookup(spell2);
    }

    if (victim3 != NULL && (sn1 != gsn_flame_scorch))
    {
        switch(number_range(0,4))
        {
            case 1: spell3 = "harm"; level = 58; break;
            case 2: spell3 = "iceball";  level = 55; break;
            default: spell3 = "dispel magic"; level = 54;   break;
        }
        sn3 = skill_lookup(spell3);
    }

if (sn1 == gsn_flame_scorch)
    {
        act("The hydra's heads swing around and rise up above you.",ch,0,0,TO_ROOM);
        (*skill_table[sn1].spell_fun) ( sn1, ch->level, ch, victim1,skill_table[sn1].target );
        return TRUE;
    }
else if (  sn1 >= 0 )
{
    act("$n brings it's massive red head down to bear upon $N!",ch,0,victim1,TO_NOTVICT);
    act("$n brings it's massive red head around to bear upon you!",ch,0,victim1,TO_VICT);
        (*skill_table[sn1].spell_fun) ( sn1, level, ch, victim1,skill_table[sn1].target );
}
    if (victim2 != NULL)
    {
    if ( ( sn2 >= 0 ) && victim2->in_room == ch->in_room)
        {
    act("$n brings it's massive green head down to bear upon $N!",ch,0,victim2,TO_NOTVICT);
    act("$n brings it's massive green head around to bear upon you!",ch,0,victim2,TO_VICT);
        (*skill_table[sn2].spell_fun) ( sn1, level, ch, victim2,skill_table[sn2].target);
        }
    }
    if (victim3 != NULL && victim3->in_room == ch->in_room)
    {
    if (  sn3 >= 0 )
        {
    act("$n brings it's massive blue head down to bear upon $N!",ch,0,victim3,TO_NOTVICT);
    act("$n brings it's massive blue head around to bear upon you!",ch,0,victim3,TO_VICT);
        (*skill_table[sn3].spell_fun) ( sn3, level, ch, victim3, skill_table[sn3].target);
        }
    }

    return TRUE;
}

void spell_flame_scorch(int sn, int level, CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam, tmp_dam;

    act("$n strafes the room in a crossfire of searing blue-green fire!",ch,0,0,TO_ROOM);

    dam = dice(ch->level, 12);

    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next_in_room;
        if (is_safe(vch,ch))    continue;
        if (vch == ch)  continue;
        if (IS_NPC(vch) && vch->pIndexData->vnum == MOB_VNUM_BONES)
        {
            act("$n explodes into bits of shattered bone!",vch,0,0,TO_ROOM);
            extract_char(vch,TRUE);
        }
        else
        {
        if (saves_spell(level,vch,DAM_ENERGY))
            tmp_dam = dam/2;
        else tmp_dam = dam;
        if (saves_spell(level +5, vch,DAM_ENERGY))
            tmp_dam  -= 50;
    damage_old(ch,vch,tmp_dam,sn,DAM_ENERGY,TRUE);
        }
    }
    return;
}

/* horn valere knight spec */
bool spec_valere_knight(CHAR_DATA *ch)
{
    CHAR_DATA *bane;
    int sn;

    if (ch->position != POS_FIGHTING)
    {
        if (number_percent() < 90)
            return FALSE;
        act("$n sighs softly and $s body slowly fades away once again.",ch,0,0,TO_ROOM);
        extract_char(ch,TRUE);
        return TRUE;
    }
    if (number_percent() > 5)
        return FALSE;

    bane = ch->fighting;
    if (!IS_NPC(bane))
        return FALSE;

    if (bane->pIndexData->vnum == 13718)
    {
        switch(number_range(0,3))
        {
            case 0: case 1:
            do_say(ch,"Pay for your evil against my people Lord of Darkness!");
            multi_hit(ch,bane,TYPE_UNDEFINED);
            multi_hit(ch,bane,TYPE_UNDEFINED);
            break;
            case 2:
            act("$n is surrounded in a bright halo of burning white light!",ch,0,0,TO_ROOM);
    sn = skill_lookup("concatenate");
    if (sn <= 0)
        return FALSE;
    (*skill_table[sn].spell_fun) (sn,ch->level,ch,bane,skill_table[sn].target);
        break;
            case 3:
    sn = skill_lookup("wrath");
    if (sn <= 0)
        return FALSE;
    (*skill_table[sn].spell_fun) (sn,ch->level,ch,bane,skill_table[sn].target);
        break;
        }
    }
        return TRUE;
}


/* Bone demon in drannor */

bool spec_bone_demon(CHAR_DATA *ch)
{
    OBJ_DATA *obj;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam, sn;
    int chance;

    if (ch->position != POS_FIGHTING)
        return FALSE;

    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next_in_room;
        if (vch->fighting == ch && number_bits(2) == 0)
            break;
    }
    if (vch == NULL)
        return FALSE;

    chance = (ch->level - vch->level)* 5;
    chance += 50;

    switch(number_range(1,7))
    {
        case 1: case 2:
    if (number_percent() > chance)
    {
        act("$n's arms reach out and flail at $N but miss.",ch,0,vch,TO_NOTVICT);
        act("$n's arms reach out and flail at you but miss.",ch,0,vch,TO_VICT);
        return FALSE;
    }
    act("$n's arms reach out and flail across $N's body!",ch,0,vch,TO_NOTVICT);
    act("$n's arms reach out and flail across your body!",ch,0,vch,TO_VICT);
    dam = dice(ch->level,6);
    damage_old(ch,vch,(saves_spell(ch->level,vch,DAM_SLASH) ? dam/2 : dam),gsn_flail_arms,DAM_SLASH,TRUE);
    if (ch->in_room == vch->in_room)
    {
        dam = dice(ch->level, 4);
    damage_old(ch,vch,(saves_spell(ch->level,vch,DAM_SLASH) ? dam/2 : dam),gsn_flail_arms,DAM_SLASH,TRUE);
    }
    if (ch->in_room == vch->in_room)
    {
        dam = dice(ch->level, 2);
    damage_old(ch,vch,(saves_spell(ch->level,vch,DAM_SLASH) ? dam/2 : dam),gsn_flail_arms,DAM_SLASH,TRUE);
    }
    return TRUE;
    break;
    case 3: case 4:
    do_kick(ch,"");
    break;
    case 5: case 6:
    act("$n raises it's bone arms and calls forth the demons of hell!",ch,0,0,TO_ROOM);
    sn = skill_lookup("demonfire");
    if (sn <= 0)
        return FALSE;
        dam = dice(ch->level,7);
        if (saves_spell(ch->level,vch,DAM_NEGATIVE))
            dam /= 2;
/*
    if (spellbaned(ch,vch,sn) || (absorbed(ch,vch,sn))

      break;
    else
*/
        damage_old(ch,vch,dam,sn,DAM_NEGATIVE,TRUE);
    break;
    case 7:
    if (ch->hit > 250)
        {
            do_say(ch,"Come share your bones mortal!");
    act("$n raises it's bone arms and the bloody waters surge up around you!",ch,0,0,TO_ROOM);
    sn = skill_lookup("blood tide");
    dam = dice(ch->level,7);
    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
        if (vch == ch)
            continue;
        if (is_safe(vch,ch))    continue;
damage_old(ch,vch,(saves_spell(ch->level,vch,DAM_OTHER) ? dam/2 : dam), sn,DAM_COLD,TRUE);
    }
    break;
            }
act("$n's body suddenly shudders and explodes into shards of shattered bone!",ch,0,0,TO_ROOM);
dam = dice(ch->level,12);
sn = gsn_shattered_bone;
for (vch = char_list; vch != NULL; vch = vch_next)
{
    vch_next = vch->next;
    if (vch->in_room->area != ch->in_room->area)
        continue;
    if (vch == ch)  continue;
    if (vch->in_room == ch->in_room)
           damage_old(ch,vch,(saves_spell(ch->level,vch,DAM_PIERCE) ?
		dam/2 : dam),sn,DAM_PIERCE,TRUE);
    else
send_to_char("You hear an explosive detonation and shattered bits of bone fall around you.\n\r",vch);
}
    for (obj = ch->carrying; obj != NULL; obj = obj->next_content)
        obj_to_room(obj,ch->in_room);
    extract_char(ch,TRUE);
    break;
    }
    return TRUE;
}

bool spec_drannor_wraith(CHAR_DATA *ch)
{
    if (ch->position == POS_FIGHTING)
        return FALSE;

    if (ch->position != POS_FIGHTING
    && ch->last_fought == NULL
    && number_percent() > 20)
    {
        act("$n slowly sinks into the ground with a long sad sigh.",ch,0,0,TO_ROOM);
        extract_char(ch,TRUE);
        return TRUE;
    }
    return FALSE;
}

/* For arkham legion...Ceran */
bool spec_necromancer( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;
    CHAR_DATA *zombie;
    OBJ_DATA *corpse;
    OBJ_DATA *obj_next;
    OBJ_DATA *obj;
    CHAR_DATA *search;
    AFFECT_DATA af;
    char *name;
    char *last_name;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int z_level, count;

    if ( ch->position != POS_FIGHTING )
    {
        if (ch->level < 45)
            return FALSE;
        if (ch->pIndexData->vnum != 29723
        && ch->pIndexData->vnum != 29731)
            return FALSE;

        for (victim = ch->in_room->people; victim != NULL; victim = v_next)
        {
            v_next = victim->next_in_room;
            if (victim != ch && victim->leader != ch
            && victim->master != ch && can_see(victim,ch)
            && number_bits( 2) == 0)
                break;
        }
        if (victim == NULL)
            return FALSE;
        sn = skill_lookup("power word kill");
        if (sn <= 0)
            return FALSE;
    act("$n raises $s cowled face and burning red eyes turn upon you in hatred!",ch,0,0,TO_ROOM);
    if (!IS_NPC(victim))
	{
	sprintf(buf1,"Help! %s is casting on me!",ch->short_descr);
	do_yell(victim,buf1);
 	}

    (*skill_table[sn].spell_fun) ( sn,IS_NPC(victim) ?  60 : ch->level - 2, ch, victim , TAR_CHAR_OFFENSIVE);
        return TRUE;
    }

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

    switch ( number_range(0,8) )
	{
        case 1: min_level = 0;  spell = "blindness";    break;
        case 2: min_level = 5;  spell = "curse";        break;
        case 3: min_level = 15; spell = "energy drain"; break;
        case 4: min_level = 24; spell = "cremate";      break;
        case 5: min_level = 28; spell = "acid blast";   break;
	case 6: min_level = 35; spell = "frostbolt";	break;
    default: min_level = 28; spell = "acid blast";     break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;


    if (sn == skill_lookup("animate dead"))
    {
    if (is_affected(ch,sn))
    {
    return FALSE;
    }
    count = 0;
    for (search = char_list; search != NULL; search = search->next)
    {
    if (IS_NPC(search) && (search->master == ch)
    && search->pIndexData->vnum == MOB_VNUM_ZOMBIE)
        count++;
    }

    if ((ch->level < 30 && count > 1) || (ch->level < 35 && count > 2)
    || (ch->level < 40 && count > 3) || (ch->level < 51 && count > 4)
    || count > 4)
    {
    return FALSE;
    }

    corpse = get_obj_here(ch,"corpse");

    if (corpse == NULL)
    {
    return FALSE;
    }

    if ((corpse->item_type != ITEM_CORPSE_NPC) && (corpse->item_type != ITEM_CORPSE_PC) )
    {
    return FALSE;
    }

   if (IS_SET(corpse->extra_flags,CORPSE_NO_ANIMATE))
    {
    return FALSE;
    }

    name = corpse->short_descr;
    for (obj = corpse->contains; obj != NULL; obj = obj_next)
    {
    obj_next = obj->next_content;
    obj_from_obj(obj);
    obj_to_room(obj,ch->in_room);
    }
        init_affect(&af);
    af.where = TO_AFFECTS;
        af.aftype    = AFT_SPELL;
    af.type = sn;
    af.level = ch->level;
    af.duration = 4;
    af.modifier = 0;
    af.location = 0;
    af.bitvector = 0;
    affect_to_char(ch,&af);

    if (number_percent() > 95)
    {
    act("$n tries to animate a corpse but destroys it.",ch,NULL,NULL,TO_ROOM);
    extract_obj(corpse);
    return TRUE;
    }

    act("$n utters an incantation and a burning red glow flares into the eyes of $p.",ch,corpse,NULL,TO_ROOM);
    act("$p shudders and comes to life!",ch,corpse,NULL,TO_ROOM);
    act("You call upon the powers of the dark to give life to $p.",ch,corpse,NULL,TO_CHAR);

    zombie = create_mobile(get_mob_index(MOB_VNUM_ZOMBIE));
    char_to_room(zombie,ch->in_room);

    z_level = UMAX(1,corpse->level - 5);
    zombie->level = z_level;
    zombie->max_hit = (dice(z_level, 15));
    zombie->max_hit += (z_level * 18);
    zombie->hit = zombie->max_hit;
    zombie->damroll += z_level*2/3;
    zombie->alignment = -1000;
    last_name = name;

    last_name = one_argument(corpse->short_descr, name);
    last_name = one_argument(last_name,name);
    last_name = one_argument(last_name,name);
    name = last_name;

    extract_obj(corpse);

    sprintf( buf1, "the zombie of %s", name);
    sprintf( buf2, "a zombie of %s is standing here.\n\r", name);
    free_string(zombie->short_descr);
    free_string(zombie->long_descr);
    zombie->short_descr = str_dup(buf1);
    zombie->long_descr = str_dup(buf2);

    add_follower(zombie,ch);
    zombie->leader = ch;

    af.duration = -1;
    af.bitvector = AFF_CHARM;
    affect_to_char(zombie,&af);
    return TRUE;
    }
    else
    {
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim , TAR_CHAR_OFFENSIVE);
    }

    return TRUE;
}

/* For arkham legion...Ceran */
bool spec_rager(CHAR_DATA *ch)
{
    AFFECT_DATA af;

    if (ch->position != POS_FIGHTING)
        return FALSE;

	switch(number_range(0,3))
	{
	case 0:
    if (!is_affected(ch,gsn_steel_nerves))
    {
        init_affect(&af);
    af.where = TO_AFFECTS;
        af.aftype    = AFT_POWER;
    af.duration = 3 + ch->level/10;
    af.location = 0;
    af.modifier = 0;
    af.level = ch->level;
    af.type = gsn_steel_nerves;
    af.bitvector = 0;
    affect_to_char(ch,&af);

    act("$n seems to glower and look deadly.",ch,NULL,NULL,TO_ROOM);
        return TRUE;
    }

    else if (!is_affected(ch,gsn_battlecry))
    {
        init_affect(&af);
    af.where = TO_AFFECTS;
        af.aftype    = AFT_POWER;
	af.type = gsn_battlecry;
	af.level = ch->level;
	af.duration = (5 + ch->level/5);
	af.location = APPLY_SAVES;
	af.modifier = (-3 -ch->level/5);
	af.bitvector = 0;
	affect_to_char(ch,&af);

    send_to_char("You are filled with total fury as you let out a battlecry!\n\r",ch);
    act("$n lets out a chilling battlecry and begins frothing at the mouth.",ch,NULL,NULL,TO_ROOM);

    return TRUE;
    }
	break;
	case 1: spec_flurry(ch); return TRUE; break;
	case 2: spec_pincer(ch); return TRUE; break;
	default: spec_flurry(ch); return TRUE; break;
	}
	return FALSE;
}

/* For arkham legion...Ceran */
bool spec_demonologist(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    CHAR_DATA *mob;
    char buf1[MAX_STRING_LENGTH];
    char *spell;
    int sn;
    int lvl;

    spell = "bash";
    if ( ch->position != POS_FIGHTING )
    {
        if (ch->level < 51)
            return FALSE;
        if ( ch->pIndexData->vnum != 29732)
            return FALSE;

        for (victim = ch->in_room->people; victim != NULL; victim = v_next)
        {
            v_next = victim->next_in_room;
            if (victim != ch && victim->leader != ch
            && victim->master != ch && can_see(victim,ch)
	&& !IS_NPC(victim) )
                break;
        }
        if (victim == NULL)
            return FALSE;
        sn = skill_lookup("fireball");
        if (sn <= 0)
            return FALSE;
    act("$n intones a word of death and the pentegram flares up!",ch,0,0,TO_ROOM);

    for (victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
            v_next = victim->next_in_room;
            if (victim != ch && victim->leader != ch
            && victim->master != ch)
	{
	sprintf(buf1,"Help! %s is casting on me!",ch->short_descr);
	do_yell(victim,buf1);
 	}
    }

    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim + 4 , TAR_CHAR_OFFENSIVE);
        return TRUE;
    }

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }
    sn = 0;
    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

    switch ( number_range(0,10) )
	{
        case 1: min_level = 0;  spell = "blindness";    break;
        case 2: min_level = 5; spell = "disease";   break;
        case 3: min_level = 25; spell = "demonfire"; break;
        case 4: min_level = 20; spell = "fireball"; break;
        case 5: min_level = 35; spell = "deathspell";   break;
        case 6: min_level = 10; spell = "power word fear"; break;
        case 7: case 8: min_level = 45; sn = -1; break;
        case 9 : min_level = 20; spell = "cone of cold"; break;
	default: min_level = 20; spell = "acid blast";     break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if (sn == -1)
    {
        for (victim = ch->in_room->people; victim != NULL; victim = victim->next_in_room)
        {
            if (IS_NPC(victim) && victim->pIndexData->vnum == MOB_VNUM_SLAYER)
                    break;
        }
        if (victim != NULL)
            return FALSE;

mob = create_mobile(get_mob_index(MOB_VNUM_SLAYER));
if (mob == NULL)
    return FALSE;
act("$n raises $s arms and calls forth a demonic servant!",ch,0,0,TO_ROOM);

char_to_room(mob,ch->in_room);
mob->leader = ch;
mob->master = ch;
mob->level = ch->level;
SET_BIT(mob->affected_by, AFF_CHARM);
act("A gate opens up and $n steps forth.",mob,0,0,TO_ROOM);
return TRUE;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
        return FALSE;
    lvl = ch->level;

    if (sn == skill_lookup("deathspell"))
    {
        act("$n gestures and a flaming hexagram burns into the air!",ch,0,0,TO_ROOM);
        send_to_char("You gesture and a flaming hexagram burns into the air!\n\r",ch);
        lvl += 3;
    }
    else if (sn == skill_lookup("fireball") && number_percent() > 65)
    {
        act("$n gestures and red runes etched into the floor explode!",ch,0,0,TO_ROOM);
        lvl += 4;
    }
    else if (sn == skill_lookup("cone of cold") && number_percent() > 75)
    {
       act("$n gestures and blue runes etched into the floor explode!",ch,0,0,TO_ROOM);
       lvl += 3;
    }

    (*skill_table[sn].spell_fun) ( sn,lvl, ch, victim , TAR_CHAR_OFFENSIVE);
    return TRUE;
}

/* For titan in Arkham legion..Ceran */
bool spec_titan(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    int num, dam;
    AFFECT_DATA af;
    int chance;
    int sn;

    if (ch->fighting == NULL)
    {
        if (ch->last_fought != NULL)
        {
        if (number_percent() > 5
        || is_affected(ch,gsn_trophy))
            return FALSE;
        if (number_percent() > 50)
        do_yell(ch,"Come back puny mortals and feed my hungry stomach!");
            else
        do_yell(ch,"You fight like mortals and flee like children!");

        init_affect(&af);
        af.where = TO_AFFECTS;
        af.aftype    = AFT_SPELL;
        af.type = gsn_trophy;   /* Just using as trigger */
        af.modifier = 0;
        af.bitvector = 0;
        af.level = 0;
        af.duration = 2;
        af.location = 0;
        affect_to_char(ch,&af);
            return TRUE;
        }

    if (number_percent() > 5 || is_affected(ch,gsn_trophy))
        return FALSE;
    num = number_range(0,5);
    if (num == 0 || num == 1)
    do_yell(ch,"Release me mortals or suffer my wrath!");
    else if (num == 3)
    do_yell(ch,"I shall crush you like twigs and feast on your corpses mortal fools!");
    else
    do_yell(ch,"Ni Sothoth Den Cael Yog Nar Sethan!");

        init_affect(&af);
        af.where = TO_AFFECTS;
        af.aftype    = AFT_SPELL;
        af.type = gsn_trophy;   /* Just using as trigger */
        af.modifier = 0;
        af.bitvector = 0;
        af.level = 0;
        af.duration = 2;
        af.location = 0;
        affect_to_char(ch,&af);
            return TRUE;

    return TRUE;
    }

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
        return FALSE;

    num = number_range(0,4);

    switch( num )
    {
        case 1: sn = gsn_crush; break;
        case 2: sn = skill_lookup("windwall"); break;
        case 3: sn = skill_lookup("acid blast");    break;
        case 4: sn = -1;    break;
        default: sn = -1; break;
    }

    if (sn == -1)
        return FALSE;

    if (sn == skill_lookup("acid blast"))
        {
        act("$n hurls a bolt of seething acid at $N!",ch,0,victim,TO_NOTVICT);
        act("$n hurls a bolt of seething acid at you!",ch,0,victim,TO_VICT);
        act("You hurl a bolt of seething acid at $N!",ch,0,victim,TO_CHAR);
        }
    else if (sn == skill_lookup("windwall"))
        {
        act("$n breathes out and releases a blast of gale force wind!",ch,0,0,TO_ROOM);
        act("You breath out and release a blast of gale force wind!",ch,0,0,TO_CHAR);
        }
    else
    {
        chance = 50 + (ch->level - victim->level) * 3;

        if (number_percent() > chance)
            return FALSE;

        dam = dice(ch->level,8);
        if (saves_spell(ch->level,victim,DAM_BASH))
            dam /= 2;
        act("$n grabs hold of $N and slams $M to the ground!",ch,0,victim,TO_NOTVICT);
        act("You grab hold of $N and slam $M to the ground!",ch,0,victim,TO_CHAR);
        act("$n grabs hold of you and slams you to the ground!",ch,0,victim,TO_VICT);

        damage_old(ch,victim,dam,gsn_crush,DAM_BASH,TRUE);
        WAIT_STATE(ch,24);
        WAIT_STATE(victim,12);
        return TRUE;
    }

        (*skill_table[sn].spell_fun) ( sn,ch->level, ch, victim , TAR_CHAR_OFFENSIVE);
    return TRUE;
}


/* Real nasty spec..Ceran */
bool spec_legionlord(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    char *msg;
    CHAR_DATA *v_next;
    int sn, dam, level, num;

    num = number_range(0,5);
    level = ch->level;
    msg = "none";

    if (ch->position == POS_SLEEPING) /* stops blackjack/strangle */
    {
        act("$n shudders and slowly rises to $s feet.",ch,0,0,TO_ROOM);
        ch->position = POS_STANDING;
    }
    if (ch->position != POS_FIGHTING)
    {
        for (victim = ch->in_room->people; victim != NULL; victim = v_next)
        {
	v_next = victim->next_in_room;
            if (victim == ch
            || ( IS_NPC(victim) && victim->pIndexData->vnum == 29782) )
                continue;
		if (number_percent() < 80)
		break;
        }
	if (victim == NULL)
		return FALSE;
    sn = skill_lookup("demonfire");
    if (sn < 0)
        return FALSE;

    do_say(ch,"Time to die puny mortal!");

    dam = 400 + dice(ch->level,5);
    if (saves_spell(ch->level,victim,DAM_NEGATIVE))
        dam /= 2;
    damage_old(ch,victim,dam,sn,DAM_NEGATIVE,TRUE);
    return TRUE;
    }

    switch(number_range(0,4))
    {
        case 0:
            return (spec_necromancer(ch));  break;
        case 1:
            return (spec_demonologist(ch)); break;
        case 2:
            return (spec_breath_frost(ch)); break;
	case 3:
	    return (spec_breath_acid(ch)); break;
        default:
            return (spec_breath_frost(ch)); break;
    }

    return FALSE;
}

/* For special functions that use area messaging .. Ceran */
bool area_act(CHAR_DATA *ch,char *msg, int min_sect, int max_sect)
{
    CHAR_DATA *ach;
    CHAR_DATA *ach_next;
    CHAR_DATA *mob;
    int count;
    bool found = FALSE;
	return FALSE;
    count = 0;

    if (!str_cmp(msg,"zombie call"))
    {
        for (ach = char_list; ach != NULL; ach = ach_next)
        {
	ach_next = ach->next;
        if (ach->in_room == NULL)
            continue;
        if (ach->in_room->area != ch->in_room->area)
            continue;
        if (ach == ch || IS_NPC(ach))
            continue;
        if (ach->in_room->sector_type >= min_sect
        && ach->in_room->sector_type < max_sect)
               found = TRUE;
        }
        if (!found)
            return FALSE;
        for (; ;)
        {
            count++;
        for (ach = char_list; ach != NULL; ach = ach_next)
        {
	ach_next = ach->next;
        if (ach->in_room == NULL)
            continue;
        if (ach->in_room->area != ch->in_room->area)
            continue;
        if (ach == ch || IS_NPC(ach))
            continue;
        if (ach->in_room->sector_type >= min_sect
        && ach->in_room->sector_type < max_sect
        && number_bits(2) == 0)
                break;
        }
        if ((ach != NULL) || count >= 20)
            break;
        }

        if (ach == NULL)
            return FALSE;
        mob = create_mobile(get_mob_index(29708));
        if (mob == NULL)
            return FALSE;
        char_to_room(mob,ach->in_room);
        act("The ground heaves and $n claws it's way out!",mob,0,0,TO_ROOM);
        multi_hit(mob,ach,TYPE_UNDEFINED);
        return TRUE;
    }


        for (ach = char_list; ach != NULL; ach = ach_next)
        {
	ach_next = ach->next;
        if (ach->in_room == NULL)
            continue;
        if (ach->in_room->area != ch->in_room->area)
            continue;
        if (ach == ch || IS_NPC(ach))
            continue;
        if (ach->in_room->sector_type >= min_sect
        && ach->in_room->sector_type < max_sect
        && IS_AWAKE(ach))
            act(msg,ch,0,ach,TO_VICT);
        }

        return TRUE;
}

/* new specials for dragons .. Ceran */
bool spec_wings(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    int dam;
    int sn;

    sn = skill_lookup("windwall");
    if (sn <= 0)
        return FALSE;

    act("$n spreads $s wings and beats violently at the air!",ch,0,0,TO_ROOM);
    dam = dice(ch->level,6);
    for (victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
        v_next = victim->next_in_room;
        if (is_safe(victim,ch) || (victim == ch) || is_same_group(victim,ch))
            continue;
damage_old(ch,victim,(saves_spell(ch->level,victim,DAM_OTHER) ? dam/2 : dam),sn, DAM_BASH,TRUE);
    }
    return TRUE;
}


bool spec_red_dragon(CHAR_DATA *ch)
{
    int num;
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    CHAR_DATA *tmp_vict,*last_vict,*next_vict;
    bool found;
    int dam, sn;
    int level;

    if (ch->position != POS_FIGHTING)
        return FALSE;

    level = ch->level;
    num = number_range(0,5);
	sn = skill_lookup("chain lightning");
	if (sn <= 0)
	sn = skill_lookup("lightning bolt");

    if (num == 0)
    {
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim != ch && can_see( ch, victim ) && number_bits( 1 ) == 0 )
	    break;
    }

    if ( victim == NULL )
        return FALSE;

	if (sn <= 0)
	return FALSE;

    act("$n raises $s talon and blue lightning streaks forth!",
        ch,NULL,victim,TO_ROOM);
    act("A lightning bolt leaps from $n's claw and arcs to $N.",
        ch,NULL,victim,TO_ROOM);
    act("A lightning bolt leaps from your claw and arcs to $N.",
	ch,NULL,victim,TO_CHAR);
    act("A lightning bolt leaps from $n's claw and hits you!",
	ch,NULL,victim,TO_VICT);

    dam = dice(level,6);
    if (saves_spell(level,victim,DAM_LIGHTNING))
    dam /= 2;
    damage_old(ch,victim,dam,sn,DAM_LIGHTNING,TRUE);
    last_vict = victim;
    level -= 3;   /* decrement damage */

    /* new targets */
    while (level > 0)
    {
	found = FALSE;
	for (tmp_vict = ch->in_room->people; tmp_vict != NULL;  tmp_vict = next_vict)
	{
	  next_vict = tmp_vict->next_in_room;
	  if (!is_safe_spell(ch,tmp_vict,TRUE) && tmp_vict != last_vict)
	  {
	    found = TRUE;
	    last_vict = tmp_vict;
	    if (last_vict == ch)
	    {
	     if (number_percent()>=ch->level)
	      {
	       act("The bolt arcs back to $n, and it stabilizes!",tmp_vict,NULL,NULL,TO_ROOM);
	       act("The bolt arcs back to you and you stabilize its energy!",tmp_vict,NULL,NULL,TO_ROOM);
	       level+=7;
	      }
	     else
	      {
	       act("The bolt arcs back to $n!",tmp_vict,NULL,NULL,TO_ROOM);
	       act("The bolt arcs back to you!",tmp_vict,NULL,NULL,TO_ROOM);
	      }
	    }
	    else
	    {
	    act("The bolt arcs to $n!",tmp_vict,NULL,NULL,TO_ROOM);
	    act("The bolt hits you!",tmp_vict,NULL,NULL,TO_CHAR);
	    dam = dice(level,6);
	    if (saves_spell(level,tmp_vict,DAM_LIGHTNING))
        dam /= 2;
	    damage_old(ch,tmp_vict,dam,sn,DAM_LIGHTNING,TRUE);
        level -= 3;  /* decrement damage */
	    }
	  }
	}   /* end target searching loop */

	if (!found) /* no target found, hit the caster */
	{
	  if (ch == NULL)
            return TRUE;

	  if (last_vict == ch) /* no double hits */
	  {
        act("The bolt fades away in a flickering spark.",ch,NULL,NULL,TO_ROOM);
	    act("The bolt grounds out harmlessly through your body.",
		ch,NULL,NULL,TO_CHAR);
        return TRUE;
	  }

	  last_vict = ch;
	  act("The bolt arcs back to $n!",ch,NULL,NULL,TO_ROOM);
	  send_to_char("The bolt arcs harmlessly back to you.\n\r",ch);
	  level -= 4;  /* decrement damage */
	  if (ch == NULL)
        return TRUE;
	}
    }
    }
    else if (num == 1 || num == 2 || num == 3)
        return spec_breath_fire(ch);
    else if (num == 4)
        return spec_wings(ch);
    else
        return spec_cast_mage(ch);

    return TRUE;
}

bool spec_black_dragon(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    int sn, dam, num;
    int sn_p, sn_b, level;

    if (ch->position != POS_FIGHTING)
        return FALSE;

    num = number_range(0,5);
    level = ch->level;

    if (num == 0)
    {
        sn = skill_lookup("acid breath");
        sn_p = skill_lookup("poison");
        sn_b = skill_lookup("blindness");

        if ((sn <= 0)
        || (sn_p <= 0) || (sn_b <= 0))
            return FALSE;

        act("$n hisses out a word of draconic power!",ch,NULL,NULL,TO_ROOM);
        dam = dice(level,5);
        for (victim = ch->in_room->people; victim != NULL; victim = v_next)
        {
            v_next = victim->next_in_room;
            if (is_safe(victim,ch) || victim == ch)
                continue;
            damage_old(ch,victim,saves_spell(level,victim,DAM_ACID) ? dam/2 : dam, sn, DAM_ACID,TRUE);
            (*skill_table[sn_p].spell_fun) (sn_p,level,ch,victim,TAR_CHAR_OFFENSIVE);
            (*skill_table[sn_b].spell_fun) (sn_b,level,ch,victim,TAR_CHAR_OFFENSIVE);
        }
    return TRUE;
    }
    else if (num == 1 || num == 2 || num== 3)
        return spec_breath_acid(ch);
    else if (num == 4)
        return spec_wings(ch);
    else
        return spec_cast_mage(ch);

    return TRUE;
}

bool spec_gold_dragon(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    int sn, dam;
    int num;

    if (ch->position != POS_FIGHTING)
        return FALSE;

    num = number_range(0,5);

    if (num == 0)
    {
        sn = skill_lookup("concatenate");

        if (sn <= 0)
            return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
    v_next = victim->next_in_room;
    if ( victim != ch && can_see( ch, victim ) && number_bits( 1 ) == 0 )
        break;
    }

    if ( victim == NULL )
        return FALSE;

        act("$n points a talon at $N and unleashes a blast of light!",ch,0,victim,TO_NOTVICT);
        act("You point a talon at $N and unleash a blast of light!",ch,0,victim,TO_CHAR);
        act("$n points a talon at you and unleashes a blast of light!",ch,0,victim,TO_VICT);
        dam = dice(ch->level,13);
            damage_old(ch,victim,saves_spell(ch->level,victim,DAM_LIGHT) ? dam/2 : dam, sn, DAM_LIGHT,TRUE);
    return TRUE;
    }
    else if (num == 1 || num == 2 || num == 3)
        return spec_breath_fire(ch);
    else if (num == 4)
        return spec_wings(ch);
    else
        return spec_cast_cleric(ch);

    return TRUE;
}

bool spec_warder(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;
    if (number_percent() > 65)
	return FALSE;
    if ( ch->position != POS_FIGHTING )
	return FALSE;
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
	v_next = victim->next_in_room;
	if(victim->fighting==ch) {
		break;
	}
    }
    if ( victim == NULL )
	return FALSE;
    spell = "earthmaw";
    switch ( number_range(0,8) )
	{
    case  0: spell = "beast call"; break;
    case  1: spell = "insectswarm";    break;
    case  2: spell = "earthmaw";  break;
    case  3: spell = "icelance"; break;
    case  4: spell = "breath fire";    break;
    if (is_affected(victim,gsn_sanctuary) || is_affected(victim,gsn_wraithform) || is_affected(victim,gsn_fireshield))
    {
    case  5: spell = "flamestrike"; break;
    case  6: spell = "flamestrike"; break;
    }
    default: spell = "windwall";      break;
    }


     if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim, TAR_CHAR_OFFENSIVE );
    return TRUE;
}

bool spec_seneschal(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;
    if (number_percent() > 65)
	return FALSE;
    if ( ch->position != POS_FIGHTING )
	return FALSE;
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
	v_next = victim->next_in_room;
	if(victim->fighting==ch) {
		break;
	}
    }
    if ( victim == NULL )
	return FALSE;
    spell = "crushing hand";
    switch ( number_range(0,3) )
	{
    case  0: spell = "ball lightning"; break;
    case  1: spell = "concatenate";     break;
    case  2: spell = "crushing hand";  break;
    case  3: spell = "hellfire"; break;
    case  4: spell = "fire and ice";    break;
    default: spell = "crushing hand";      break;
    }

     if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim, TAR_CHAR_OFFENSIVE );
    return TRUE;
}

bool spec_outlaw(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;
    if (number_percent() > 50)
	return FALSE;
    if ( ch->position != POS_FIGHTING )
	return FALSE;
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
	v_next = victim->next_in_room;
	if(victim->fighting==ch) {
		break;
	}
    }
    if ( victim == NULL )
	return FALSE;

    switch ( number_range(0,3) )
	{
    case  0: spell = "flamestrike"; break;
    case  1: spell = "ball lightning";    break;
    case  2: spell = "crushing hand";     break;
    case  3: spell = "flamestrike";     break;
    case  4: spell = "faerie fire";     break;
    case  5: spell = "faerie fire";     break;
    default: spell = "weaken";      break;
    }


    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim, TAR_CHAR_OFFENSIVE );
    return TRUE;
}

bool spec_flurry(CHAR_DATA *ch)
{
	CHAR_DATA *victim;
	CHAR_DATA *v_next;
	int numhits;
	int i;
	numhits=number_range(4,9);
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
	v_next = victim->next_in_room;
	if(victim->fighting==ch) {
		break;
	}
    }
      if (ch->position != POS_FIGHTING || victim == NULL)
      	return FALSE;
      if (number_percent() > 30)
		return TRUE;
      act("$n begins a wild flurry of attacks!",ch,0,0,TO_ROOM);
	for ( i=0; i< numhits; i++ ) {
		if (number_percent() > 80) {
			damage( ch, victim, 0, gsn_flurry, DAM_NONE,TRUE);
			continue;
		}
		one_hit(ch,victim,gsn_flurry);
	}
	WAIT_STATE(ch,PULSE_VIOLENCE*3);
	return TRUE;

}
bool spec_pincer(CHAR_DATA *ch)
{
	CHAR_DATA *victim;
	CHAR_DATA *v_next;
	int numhits;
	int i;
	numhits=number_range(2,2);
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
	v_next = victim->next_in_room;
	if(victim->fighting==ch) {
		break;
	}
    }
      if (ch->position != POS_FIGHTING || victim == NULL)
      	return FALSE;
      if (number_percent() > 30)
		return TRUE;
      act("$n pincers $s between his axes!",ch,0,0,TO_ROOM);
	for ( i=0; i< numhits; i++ ) {
		if (number_percent() > 80) {
			damage( ch, victim, 0, gsn_pincer, DAM_NONE,TRUE);
			continue;
		}
		one_hit(ch,victim,gsn_pincer);
	}
	WAIT_STATE(ch,PULSE_VIOLENCE*1);
	return TRUE;

}

bool spec_drum(CHAR_DATA *ch)
{
	CHAR_DATA *victim;
	CHAR_DATA *v_next;
	int numhits;
	int i;
	int dam;
	numhits=number_range(3,6);
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
	v_next = victim->next_in_room;
	if(victim->fighting==ch) {
		break;
	}
    }
      if (ch->position != POS_FIGHTING || victim == NULL)
      	return FALSE;
      if (number_percent() > 30)
		return TRUE;
	act("$n drums at $N with $s maces.",ch,0,victim,TO_NOTVICT);
	act("$n drums at you with $s maces.",ch,0,victim,TO_VICT);
	for ( i=0; i < numhits; i++ ) {
		if (number_percent() > 95) {
			damage( ch, victim, 0, gsn_drum,DAM_NONE,TRUE);
			continue;
		}
		one_hit(ch,victim,gsn_drum);
	}
	dam = ch->level*1.8;
	dam += number_percent();
	dam = dam * 1.5;
	damage_old(ch,victim,dam,gsn_drum,DAM_BASH,TRUE);
      WAIT_STATE(ch,3*PULSE_VIOLENCE);
	return TRUE;
}

bool spec_ishmael(CHAR_DATA *ch)
{
	return TRUE;
}
bool spec_agamemnon(CHAR_DATA *ch)
{
	return TRUE;
}
bool spec_meph(CHAR_DATA *ch)
{
	return TRUE;
}
bool spec_baalzebub(CHAR_DATA *ch)
{
	return TRUE;
}
bool spec_necruvian(CHAR_DATA *ch)
{
	return TRUE;
}
bool spec_obsidian_prince(CHAR_DATA *ch)
{
	return TRUE;
}
bool spec_golden_knight(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    AFFECT_DATA af;
    char *spell;
    int sn;
    int numhits;
    int i;
    if ( ch->position != POS_FIGHTING )
	return FALSE;
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
	v_next = victim->next_in_room;
	if(victim->fighting==ch) {
		break;
	}
    }
    if ( victim == NULL )
	return FALSE;
    if(number_percent()>50)
    {
	/*Do a skill*/
	switch(number_range(0,2))
	{
	case 0:
	  if(is_affected(ch,gsn_trophy))
		return FALSE;
  	  numhits=number_range(3,7);
        act("$n begins a wild flurry of attacks!",ch,0,0,TO_ROOM);
	  for ( i=0; i< numhits; i++ ) {
		if (number_percent() > 80) {
			damage( ch, victim, 0, gsn_flurry, DAM_NONE,TRUE);
			continue;
		}
		one_hit(ch,victim,gsn_flurry);
	  }
        init_affect(&af);
        af.where = TO_AFFECTS;
        af.aftype    = AFT_SPELL;
        af.type = gsn_trophy;   /* Nobody likes mobs that flurry every round! */
        af.modifier = 0;
        af.bitvector = 0;
        af.level = 0;
        af.duration = 0;
        af.location = 0;
        affect_to_char(ch,&af);
	  return TRUE;
	  break;
	case 1:
	  act("$n jabs viciously at you with his sword!",ch,0,victim,TO_VICT);
	  act("$n jabs viciously at $N with his sword!",ch,0,victim,TO_NOTVICT);
	  if(number_percent()>90)
	  {
		damage(ch,victim,0,gsn_dragonsword,DAM_LIGHT,TRUE);
	  }
			/* We're using gsn_dragonsword as a placeholder for the 'jab' dam message. */
	  else
	  {
		damage(ch,victim,80+(number_percent()*2),gsn_dragonsword,DAM_LIGHT,TRUE);
	  }
	  return TRUE;
	  break;
	default:
	  act("$n brings both his swords together into your body in an attempt to doublethrust you!",ch,0,victim,TO_VICT);
	  act("$n brings both his swords together into $N's body in an attempt to doublethrust $M!",ch,0,victim,TO_NOTVICT);
	  if(number_percent()>90)
	  {
		damage(ch,victim,0,gsn_honorable_combat,DAM_LIGHT,TRUE);
	  }
			/* We're using gsn_honorable as a placeholder for the 'doublethrust' dam message. */
	  else
	  {
		damage(ch,victim,50+(number_percent()*2),gsn_honorable_combat,DAM_LIGHT,TRUE);
		damage(ch,victim,50+(number_percent()*2),gsn_honorable_combat,DAM_LIGHT,TRUE);
	  }
	  return TRUE;
	  break;
	}
    }
    else
    {
	switch(number_range(0,4))
	{
	case  0: spell = "crushing hand"; break;
	case  1: spell = "crushing hand"; break;
	case  2: spell = "ray of truth"; break;
      case  3: spell = "wrath"; break;
	default: spell = "wrath of purity"; break;
	}
     if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
     (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim, TAR_CHAR_OFFENSIVE );
      return TRUE;
    }

}
bool spec_templar(CHAR_DATA *ch)
{
    CHAR_DATA *victim, *v_next;
    if ( ch->position != POS_FIGHTING )
	return FALSE;
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
	v_next = victim->next_in_room;
	if(victim->fighting==ch) {
		break;
	}
    }
    if ( victim == NULL )
	return FALSE;
    switch(number_range(0,8))
    {
	case 0:
		do_trip(ch,"");
		return TRUE;
	case 1:
		do_bash(ch,"");
		return TRUE;
	case 2:
		return TRUE;
	case 3:
		return TRUE;
	case 4:
		return TRUE;
	case 5:
		return TRUE;
	case 6:
		return TRUE;
	case 7:
		return TRUE;
	case 8:
		return TRUE;
    }
return FALSE;
}

bool spec_goodie(CHAR_DATA *ch)
{
	CHAR_DATA *victim;
	CHAR_DATA *v_next;

	for ( victim = ch->in_room->people; victim != NULL; victim = v_next)
	{
		v_next = victim->next_in_room;
		if(victim->fighting==ch) {
			break;
		}
	}
      if (ch->position != POS_FIGHTING || victim == NULL)
      	return FALSE;

	if (victim->alignment == 1000) {
		act("$n sighs softly and calls the wrath of an Immortal onto $N.",ch,0,victim,TO_NOTVICT);
		act("$n sighs softly and calls the wrath of an Immortal onto you.",ch,0,victim,TO_VICT);
		raw_kill(ch,victim);
	}
	return TRUE;

}

/*bool spec_blitz(CHAR_DATA *ch)
{
	CHAR_DATA *victim;
	CHAR_DATA *v_next;
	int chance, imod;
	bool isDS;
//	char buf[MAX_STRING_LENGTH];

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
	v_next = victim->next_in_room;
	if(victim->fighting==ch) {
		break;
	}
    }

      if (ch->position != POS_FIGHTING || victim == NULL)
      	return FALSE;

      if (number_percent() > 75)
		return FALSE;
	chance = 95;
	chance += ch->carry_weight / 250;
	chance -= victim->carry_weight / 200;
	if (ch->size < victim->size)
	{
		chance += (ch->size - victim->size) * 20;
	} else {
		chance += (ch->size - victim->size) * 10;
	}
	if (is_affected(victim,gsn_protective_shield))
	{
		chance -= 5;
	}
	if (is_affected(victim,gsn_wraithform))
	{
		chance -= 5;
	}
	if (is_affected(victim,gsn_kinetic_shield))
	{
		chance -= 5;
	}
	if (is_affected(victim,gsn_fireshield))
	{
		chance -= 5;
	}
	chance += get_curr_stat(ch,STAT_STR) + 5;
	chance -= (get_curr_stat(victim,STAT_DEX) * 2);
	chance -= GET_AC(victim,AC_BASH) /20;
    	if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,skill_lookup("haste")))
	{
		chance += 10;
	}
    	if (is_affected(ch,gsn_bloodthirst))
	{
		chance += 10;
	}
    	if (IS_AFFECTED(ch,skill_lookup("battlecry")))
	{
		chance += 5;
	}
    	if (IS_AFFECTED(ch,skill_lookup("berserk")))
	{
		chance += 10;
	}
	if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,skill_lookup("haste")))
	{
		chance -= 5;
	}
	chance += (ch->level - victim->level);

	if (!IS_NPC(victim) && chance < get_skill(victim,gsn_dodge) )
	{
		chance -= 3 * (get_skill(victim,gsn_dodge) - chance);
	}
	imod = 15;

	isDS = FALSE;

	 now the attack
	if (number_percent() < chance - 10)
	{
		act("$n pummels you to the ground with excruciating force!", ch,NULL,victim,TO_VICT);
		act("You slam into $N, pummeling $M into the ground!",ch,NULL,victim,TO_CHAR);
		act("$n pummels $N into the ground with a powerful blitz.",ch,NULL,victim,TO_NOTVICT);
		damage_old(ch,victim,(3 * ch->size + chance),gsn_blitz,DAM_BASH,TRUE);

		WAIT_STATE(victim,PULSE_VIOLENCE*2);
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
		if (number_percent() < chance - imod)
		{
			if (is_affected(victim,gsn_sanctuary) && (number_percent() < chance-imod))
			{
				if (check_dispel(ch->level,victim,skill_lookup("sanctuary")))
				{
					act("$n looks stunned as $s mind wanders from $s spells.",victim,0,0,TO_ROOM);
					act("Your head throbs with pain as you lose control over your spells.",victim,0,0,TO_CHAR);
					act("The white aura about $n vanishes!",victim,NULL,NULL,TO_ROOM);
					isDS = TRUE;
				}
			}
			if ((isDS == FALSE) && is_affected(victim,gsn_fireshield) && (number_percent() < chance-imod))
			{
				if (check_dispel(ch->level,victim,skill_lookup("fireshield")))
				{
					act("$n looks stunned as $s mind wanders from $s spells.",victim,0,0,TO_ROOM);
					act("Your head throbs with pain as you lose control over your spells.",victim,0,0,TO_CHAR);
					act("The fire around $n vanishes!",victim,NULL,NULL,TO_ROOM);
					isDS = TRUE;

				}
			}

			if ((isDS == FALSE) && is_affected(victim,gsn_stoneskin) && (number_percent() < chance-imod))
			{
				if (check_dispel(ch->level,victim,skill_lookup("stone skin")))
				{
					act("$n looks stunned as $s mind wanders from $s spells.",victim,0,0,TO_ROOM);
					act("Your head throbs with pain as you lose control over your spells.",victim,0,0,TO_CHAR);
					act("The skin of $n softens into flesh!",victim,NULL,NULL,TO_ROOM);
					isDS = TRUE;

				}
			}
			if ((isDS == FALSE) && is_affected(victim,gsn_wraithform) && (number_percent() < chance-imod))
			{
				if (check_dispel(ch->level,victim,skill_lookup("wraithform")))
				{
					act("$n looks stunned as $s mind wanders from $s spells.",victim,0,0,TO_ROOM);
					act("Your head throbs with pain as you lose control over your spells.",victim,0,0,TO_CHAR);
					act("The body of $n fades back into the Prime Material Plane!",victim,NULL,NULL,TO_ROOM);
					isDS = TRUE;

				}
			}
			if ((isDS == FALSE) && is_affected(victim,gsn_shroud) && (number_percent() < chance-imod))
			{
				if (check_dispel(ch->level,victim,skill_lookup("shroud")))
				{
					act("$n looks stunned as $s mind wanders from $s spells.",victim,0,0,TO_ROOM);
					act("Your head throbs with pain as you lose control over your spells.",victim,0,0,TO_CHAR);
					act("The black aura about $n vanishes!",victim,NULL,NULL,TO_ROOM);
					isDS = TRUE;

				}
			}
		}
	} else {
		damage_old(ch,victim,0,gsn_blitz,DAM_BASH,FALSE);
		act("You fall over and bite the dust!",ch,NULL,victim,TO_CHAR);
		act("$n falls flat on $s face, eating a mouthful of dirt.",ch,NULL,victim,TO_NOTVICT);
		act("You evade $n's blitz, and $e falls flat on $s face.",ch,NULL,victim,TO_VICT);
		ch->position = POS_RESTING;
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
	}

	return TRUE;

}*/

bool spec_lothar(CHAR_DATA *ch)
{
	spec_goodie(ch);
	spec_flurry(ch);
	return TRUE;
}
bool spec_cire(CHAR_DATA *ch)
{
	spec_goodie(ch);
	spec_drum(ch);
	return TRUE;
}

bool spec_sage(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;
    if (number_percent() > 65)
	return FALSE;
    if ( ch->position != POS_FIGHTING )
	return FALSE;
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
	v_next = victim->next_in_room;
	if(victim->fighting==ch) {
		break;
	}
    }
    if ( victim == NULL )
	return FALSE;
    spell = "divine aid";
    switch ( number_range(0,8) )
	{
    case  0: spell = "divine aid"; break;
    case  1: spell = "incinerate";    break;
    case  2: spell = "ray of truth";     break;
    case  3: spell = "rot";  break;
    case  4: spell = "mental knife"; break;
    case  5: spell = "mental knife";    break;
    if (is_affected(victim,gsn_sanctuary) || is_affected(victim,gsn_wraithform) || is_affected(victim,gsn_fireshield))
    {
    case  6: spell = "dispel magic"; break;
    case  7: spell = "dispel magic"; break;
    }
    default: spell = "divine aid";      break;
    }

     if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim, TAR_CHAR_OFFENSIVE );
    return TRUE;
}

bool spec_familiar(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;
    if (number_percent() > 65)
	return FALSE;

    if (is_affected(ch, AFF_CHARM))
    return TRUE;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
	v_next = victim->next_in_room;
	if(victim->fighting==ch) {
		break;
	}
    }
    if ( victim == NULL )
	return FALSE;
    spell = "mental knife";
    switch ( number_range(0,8) )
	{
    case  0: spell = "mental knife"; break;
    case  1: spell = "acid blast";    break;
    case  2: spell = "concatenate";     break;
    case  3: spell = "firestream";  break;
    if (is_affected(victim,gsn_sanctuary) || is_affected(victim,gsn_wraithform) || is_affected(victim,gsn_fireshield))
    {
    case  4: spell = "dispel magic"; break;
    case  5: spell = "dispel magic"; break;
    }
    default: spell = "mental knife";      break;
    }

     if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim, TAR_CHAR_OFFENSIVE );
    return TRUE;
}

bool spec_omegus(CHAR_DATA *ch)
{
    CHAR_DATA *victim, *v_next;

    if ( ch->position != POS_FIGHTING )
	return FALSE;
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
	v_next = victim->next_in_room;
	if(victim->fighting==ch) {
		break;
	}
    }
    if ( victim == NULL )
	return FALSE;
    switch(number_range(0,4))
    {
	case 0:
		do_trip(ch,"");
		return TRUE;
	case 1:
		do_shove(ch,victim->name);
		return TRUE;
	case 2:
		do_dirt(ch,"");
		return TRUE;
	case 3:
		spec_flurry(ch);
		return TRUE;
	case 4:
		spec_drum(ch);
		return TRUE;
    }
return FALSE;
}

bool spec_stalker(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if (number_percent() > 65)
	return FALSE;
    if ( ch->position != POS_FIGHTING )
	return FALSE;
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
	v_next = victim->next_in_room;
	if(victim->fighting==ch) {
		break;
	}
    }
    if ( victim == NULL )
	return FALSE;
    spell = "crushing hand";
    switch ( number_range(0,3) )
	{
    case  0: spell = "crushing hand";  break;
    case  1: spell = "windwall";  break;
    case  2: spell = "flamestrike";		break;

    default: spell = "crushing hand";      break;
    }

     if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim, TAR_CHAR_OFFENSIVE );
    return TRUE;
}

bool spec_phantom(CHAR_DATA *ch)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if (number_percent() > 65)
	return FALSE;
    if ( ch->position != POS_FIGHTING )
	return FALSE;
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
	v_next = victim->next_in_room;
	if(victim->fighting==ch) {
		break;
	}
    }

    if ( victim == NULL )
	return FALSE;

    spell = "plague";
    switch ( number_range(0,2) )
	{
    case  0: spell = "slow";  break;
    case  1: spell = "weaken";  break;

    default: spell = "slow";      break;
    }

     if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim, TAR_CHAR_OFFENSIVE );
    return TRUE;
}
