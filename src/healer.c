/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
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

DECLARE_DO_FUN(do_say);

/* This is all from Rom except the restoration spell */
void do_heal(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *mob;
    char arg[MAX_INPUT_LENGTH];
    int cost,sn;
    SPELL_FUN *spell;
    char *words;
	int vnum;

    /* check for healer */
    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
        if ( IS_NPC(mob) && IS_SET(mob->act, ACT_IS_HEALER) )
            break;
    }

    if ( mob == NULL )
    {
        send_to_char( "You can't do that here.\n\r", ch );
        return;
    }

if(mob->position==POS_SLEEPING) {
	send_to_char("The healer must be awake to heal you.\n\r",ch);
	return;
    }

    one_argument(argument,arg);

	//Fix for Healers healing intruders. -- Ceial

	vnum = ch->in_room->vnum;
    if (vnum == 3804 && ch->cabal != CABAL_ACADIAN)
	{
	   act("$N says '{YI don't heal intruders in my cabal!{x'",ch,NULL,mob,TO_CHAR);
	   return;
	}
	else if (vnum == 9904 && ch->cabal != CABAL_OUTLAW)
	{
	   act("$N says '{YI don't heal intruders in my cabal!{x'",ch,NULL,mob,TO_CHAR);
	   return;
	}
	else if (vnum == 23603 && ch->cabal != CABAL_VIGILANTE)
	{
	   act("$N says '{YI don't heal intruders in my cabal!{x'",ch,NULL,mob,TO_CHAR);
	   return;
	}
	else if (vnum == 5800 && ch->cabal != CABAL_COVEN)
	{
	   act("$N says '{YI don't heal intruders in my cabal!{x'",ch,NULL,mob,TO_CHAR);
	   return;
	}
	else if (vnum == 4504 && ch->cabal != CABAL_PROTECTOR)
	{
	   act("$N says '{YI don't heal intruders in my cabal!{x'",ch,NULL,mob,TO_CHAR);
	   return;
	}
    else if (vnum == 5709 && ch->cabal != CABAL_RAVAGER)
	{
		act("$N says '{YI don't heal intruders in my cabal!{x'",ch,NULL,mob,TO_CHAR);
		return;
	}
	else if (vnum == 8108 && ch->cabal != CABAL_EMPIRE)
	{
	   act("$N says '{YI don't heal intruders in my cabal!{x'",ch,NULL,mob,TO_CHAR);
	   return;
	}
	else if (vnum == 8800 && ch->cabal != CABAL_BRIAR)
	{
	   act("$N says '{YI don't heal intruders in my cabal!{x'",ch,NULL,mob,TO_CHAR);
	   return;
	}


    if (arg[0] == '\0')
    {
        /* display price list */
	act("$N says '{YI offer the following spells:{x'",ch,NULL,mob,TO_CHAR);
	send_to_char("  Light       :  Cure light wounds      100 silver\n\r",ch);
	send_to_char("  Serious     :  Cure serious wounds    150 silver\n\r",ch);
	send_to_char("  Critic      :  Cure critical wounds   250 silver\n\r",ch);
	send_to_char("  Heal        :  Healing spell          400 silver\n\r",ch);
	send_to_char("  Blind       :  Cure blindness         150 silver\n\r",ch);
	send_to_char("  Disease     :  Cure disease           250 silver\n\r",ch);
	send_to_char("  Poison      :  Cure poison            250 silver\n\r",ch);
	send_to_char("  Cleanse     :  Cleanse                500 silver\n\r",ch);
	send_to_char("  Uncurse     :  Remove curse           100 silver\n\r",ch);
	send_to_char("  Restoration :  Restoration            150 silver\n\r",ch);
	send_to_char("  Refresh     :  Restore movement       150 silver\n\r",ch);
    send_to_char("  Mana        :  Restore mana           100 silver\n\r",ch);
	send_to_char(" Type heal <type> to be healed.\n\r",ch);
	return;
    }

    if (!str_prefix(arg,"light"))
    {
        spell = spell_cure_light;
	sn    = skill_lookup("cure light");
        words = "cure light";
	 cost  = 100;
    }

    else if (!str_prefix(arg,"serious"))
    {
	spell = spell_cure_serious;
	sn    = skill_lookup("cure serious");
        words = "cure serious";
	cost  = 150;
    }

    else if (!str_prefix(arg,"critical"))
    {
	spell = spell_cure_critical;
	sn    = skill_lookup("cure critical");
        words = "cure critical";
	cost  = 250;
    }

    else if (!str_prefix(arg,"cleanse"))
    {
	spell = spell_cleanse;
	sn = skill_lookup("cleanse");
        words = "cleanse";
	cost  = 500;
    }

    else if (!str_prefix(arg,"heal"))
    {
	spell = spell_heal;
	sn = skill_lookup("heal");
        words = "heal";
	cost  = 400;
    }

    else if (!str_prefix(arg,"blindness"))
    {
	spell = spell_cure_blindness;
	sn    = skill_lookup("cure blindness");
        words = "cure blindness";
        cost  = 150;
    }

    else if (!str_prefix(arg,"disease"))
    {
	spell = spell_cure_disease;
	sn    = skill_lookup("cure disease");
        words = "cure disease";
	cost = 250;
    }

    else if (!str_prefix(arg,"poison"))
    {
	spell = spell_cure_poison;
	sn    = skill_lookup("cure poison");
        words = "cure poison";
	cost  = 250;
    }

    else if (!str_prefix(arg,"uncurse") || !str_prefix(arg,"curse"))
    {
	spell = spell_remove_curse;
	sn    = skill_lookup("remove curse");
        words = "remove curse";
	cost  = 100;
    }

    else if (!str_prefix(arg,"restoration") || !str_prefix(arg,"restore"))
    {
	spell = spell_restoration;
	sn = skill_lookup("restoration");
	words = "restoration";
	cost = 150;
    }

    else if (!str_prefix(arg,"mana") || !str_prefix(arg,"energize"))
    {
        spell = NULL;
        sn = -1;
        words = "energizer";
        cost = 100;
    }


    else if (!str_prefix(arg,"refresh") || !str_prefix(arg,"moves"))
    {
	spell =  spell_refresh;
	sn    = skill_lookup("refresh");
        words = "refresh";
	cost  = 150;
    }

    else
    {
	act("$N says 'Type 'heal' for a list of spells.'",
	    ch,NULL,mob,TO_CHAR);
	return;
    }

    if (cost > (ch->gold * 100 + ch->silver))
    {
	act("$N says '{YYou do not have enough gold for my services.{x'",
	    ch,NULL,mob,TO_CHAR);
	return;
    }

    WAIT_STATE(ch,PULSE_VIOLENCE);

    deduct_cost(ch,cost);
/*    mob->gold += cost;*/
    act("$n closes $s eyes for a moment and nods at $N.",mob,NULL,ch,TO_NOTVICT);
	act("$n closes $s eyes for a moment and nods at you.",mob,NULL,ch,TO_VICT);

    if (spell == NULL)  /* restore mana trap...kinda hackish */
    {
    ch->mana += dice(4,8) + mob->level / 3;
    ch->mana = UMIN(ch->mana,ch->max_mana);
	send_to_char("A warm glow passes through you.\n\r",ch);
	return;
     }

     if (sn == -1)
	return;

     spell(sn,mob->level,mob,ch,TARGET_CHAR);
}
