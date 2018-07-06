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

/***************************************************************************
*       POSmud code is copyright (C) 1999-2000 and for exclusive use of    *
*       POSmud only. Hah!                                                  *
***************************************************************************/

/* DIO'S STUFF HERE. */

#include "include.h"

bool isCabalGuard( CHAR_DATA *ch );
void    one_hit         args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ));
bool    check_chargeset args( ( CHAR_DATA *ch, CHAR_DATA *victim ));
int	is_spec_skill	args((int sn));
bool	is_specced	args((CHAR_DATA *ch, int spec));

DECLARE_DO_FUN(do_myell);
DECLARE_DO_FUN(do_look);
DECLARE_DO_FUN(do_yell);
DECLARE_DO_FUN(do_say);
DECLARE_DO_FUN(easy_induct);

void do_prep( CHAR_DATA *ch )
{
    CHAR_DATA *victim=0;
    int chance;
    AFFECT_DATA af;

    if ( (chance = get_skill(ch,gsn_asscounter)) == 0
    ||   (!IS_NPC(ch)
    &&    ch->level < skill_table[gsn_asscounter].skill_level[ch->class]))
    {
        send_to_char("You don't know how to do that.\n\r",ch);
        return;
    }

    if (is_affected(ch,gsn_asscounter))
    {
        send_to_char("You cannot counter again so soon.\n\r",ch);
        return;
    }

    if (ch->mana < 50)
    {
        send_to_char("You can't find the energy to concentrate on countering.\n\r",ch);
        return;
    }

    if ((victim = ch->fighting) == NULL)
        {
            send_to_char("But you aren't fighting anyone!\n\r",ch);
            return;
        }

    /* modifiers */

    /* size */
    if (ch->size < victim->size)
        chance -= 20;

    /* dex */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= get_curr_stat(victim,STAT_DEX) * 3 / 2;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || is_affected(ch,skill_lookup("haste")))
        chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || is_affected(victim,skill_lookup("haste")))
        chance -= 40;

    /* level */
    chance += (ch->drain_level + ch->level - victim->level - victim->drain_level) * 2;

    if (number_percent() < chance)
    {

        WAIT_STATE(ch,PULSE_VIOLENCE);
        ch->mana -= 50;

        send_to_char("Heightening your senses you concentrate on countering your foe!\n\r",ch);
        act("$n face grows grim as he focuses his concentration.",ch,NULL,NULL,TO_ROOM);
        check_improve(ch,gsn_asscounter,TRUE,1);

	init_affect(&af);
        af.where        = TO_AFFECTS;
	af.aftype	= AFT_SKILL;
        af.type         = gsn_asscounter;
        af.level        = ch->level;
        af.duration     = 1;
	af.location     = 0;
        af.modifier     = 0;
        af.bitvector    = 0;
        affect_to_char(ch,&af);

        }
        else
    {
        WAIT_STATE(ch,PULSE_VIOLENCE);
        ch->mana -= 25;

        send_to_char("You attempt to focus your concentration, but fail.\n\r",ch);
        check_improve(ch,gsn_asscounter,FALSE,1);
    }
}

void do_flurry(CHAR_DATA *ch,char *argument)
{
      CHAR_DATA *victim;
      int chance, numhits, i, skill, dam;
	OBJ_DATA *weapon;
	OBJ_DATA *weapon2;

	skill = get_skill(ch,gsn_flurry);

      if ( skill == 0 )
	{
		send_to_char("Huh?\n\r",ch);
		return;
      }

	if ((victim = ch->fighting) == NULL)
	{
		send_to_char("You aren't fighting anyone.\n\r",ch);
		return;
	}

	/* CLS - Need to check to see if they are even wielding a primary or secondary weapon. */
	weapon = get_eq_char(ch,WEAR_WIELD);
        if(weapon == NULL)
	{
	 send_to_char("You first need to find a weapon to flurry with.\n\r",ch);
	 return;
 	}

	weapon2 = get_eq_char(ch,WEAR_DUAL_WIELD);
        if(weapon2 == NULL)
        {
         send_to_char("You need to find another weapon to flurry with.\n\r",ch);
         return;
        }

	if ((weapon->value[0] != WEAPON_SWORD) || (weapon2->value[0] != WEAPON_SWORD) )
	{
	send_to_char("You must be wielding two swords to flurry.\n\r",ch);
	return;
	}

	chance = number_percent();

	if (chance > skill) {
		act("You attempt to start a flurry, but fail.",ch,0,victim,TO_CHAR);
		act("$n flails out wildly with $s swords but blunders.",ch,0,0,TO_ROOM);
		check_improve(ch,gsn_flurry,FALSE,2);
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
		return;
	}



	if ((chance + skill) > 175) {
		numhits = 7;
	} else if ((chance + skill) > 160) {
		numhits = 6;
	} else if ((chance + skill) > 145) {
		numhits = 5;
	} else if ((chance + skill) > 130) {
		numhits = 4;
	} else if ((chance + skill) > 115) {
		numhits = 3;
	} else if ((chance + skill) > 100) {
		numhits = 2;
	} else if ((chance + skill) > 85) {
		numhits = 2;
	} else {
		numhits = 2;
	}

      act("You begin a wild flurry of attacks!",ch,0,victim,TO_CHAR);
      act("$n begins a wild flurry of attacks!",ch,0,0,TO_ROOM);
	for ( i=0; i< numhits; i++ ) {
		if (number_percent() > 80) {
			dam = dice( 45/4, 7 );
			damage( ch, victim, dam, gsn_flurry,DAM_NONE,TRUE);
			dam = dam - dam/5;
			continue;
		}
		one_hit(ch,victim,gsn_flurry);

	}
	check_improve(ch,gsn_flurry,TRUE,1);
	WAIT_STATE(ch,2*PULSE_VIOLENCE);
	if ((numhits = 2))
	{
		ch->move -= 25;
	}
	else if ((numhits = 3))
	{
		ch->move -= 50;
	}
	else if ((numhits = 4))
	{
		ch->move -= 75;
	}
	else if ((numhits = 5))
	{
		ch->move -= 100;
	}
	else if ((numhits = 6))
	{
		ch->move -= 125;
	}
	else if ((numhits = 7))
	{
		ch->move -= 150;
	}else{
		ch->move -= 25;
	}
    return;
}

void do_drum(CHAR_DATA *ch,char *argument)
{
      CHAR_DATA *victim;
      int chance, numhits, i, dam, skill;
	OBJ_DATA *weapon;
	OBJ_DATA *weapon2;

	skill = get_skill(ch,gsn_drum);

      if ( skill == 0 )
	{
		send_to_char("Huh?\n\r",ch);
		return;
      }

	if ((victim = ch->fighting) == NULL)
	{
		send_to_char("You aren't fighting anyone.\n\r",ch);
		return;
	}

        /* CLS - Need to check to see if they are even wielding a primary or secondary weapon. */
        weapon = get_eq_char(ch,WEAR_WIELD);
        if(weapon == NULL)
        {
         send_to_char("You first need to find a weapon to drum with.\n\r",ch);
         return;
        }

        weapon2 = get_eq_char(ch,WEAR_DUAL_WIELD);
        if(weapon2 == NULL)
        {
         send_to_char("You need to find another weapon to drum with.\n\r",ch);
         return;
        }

	if ((weapon->value[0] != WEAPON_MACE) || (weapon2->value[0] != WEAPON_MACE) )
	{
		send_to_char("You must be wielding two maces to drum.\n\r",ch);
		return;
	}

	chance = number_percent();

	dam = dice( 51/3, 7 );

	if (chance > skill) {
		act("You attempt to start drumming with your maces, but fail.",ch,0,victim,TO_CHAR);
		act("$n attempts to start drumming with two maces, but fails.",ch,0,0,TO_ROOM);
		check_improve(ch,gsn_drum,FALSE,1);
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
		return;
	}

	if ((chance + skill) > 175) {
		numhits = 4;
	} else if ((chance + skill) > 160) {
		numhits = 3;
	} else if ((chance + skill) > 145) {
		numhits = 2;
	} else {
		numhits = 2;
	}

	act("$n drums at $N with $s maces.",ch,0,victim,TO_NOTVICT);
	act("$n drums at you with $s maces.",ch,0,victim,TO_VICT);
	act("You drum at $N with your maces.",ch,0,victim,TO_CHAR);

	for ( i=0; i < numhits; i++ ) {
		if (number_percent() > 95) {
			damage( ch, victim, 0, gsn_drum,DAM_NONE,TRUE);
			dam = dam - dam/5;
			continue;
		}
		one_hit(ch,victim,gsn_drum);
	}
	damage_old(ch,victim,dam,gsn_drum,DAM_BASH,TRUE);
	check_improve(ch,gsn_drum,TRUE,1);
    WAIT_STATE(ch,2*PULSE_VIOLENCE);
	return;
}

void do_siton(CHAR_DATA *ch,char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance=0, dam, sn_fart;
    char buf[MAX_STRING_LENGTH];

    one_argument(argument,arg);

     if ( (get_skill(ch,gsn_siton) == 0 )
        || ch->level < skill_table[gsn_siton].skill_level[ch->class])
    {
        if (!IS_NPC(ch))
        {
        send_to_char("You don't have the skill to sit on people.\n\r",ch);
        return;
        }
    }
	if(ch->position==POS_FIGHTING) {
		act("You can't sit on people while you're fighting!",ch,0,0,TO_CHAR);
		return;
	}

    if (arg[0] == '\0')
    {
		send_to_char("Sit on who?\n\r",ch);
		return;
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    if (victim == ch)
    {
        send_to_char("You can't sit on yourself.\n\r",ch);
        return;
    }

    if (is_safe(ch,victim))
        return;

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
        act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
        return;
    }

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
        chance += 5;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
        chance -= 15;
    chance += get_curr_stat(ch,STAT_DEX)/2;
    chance -= get_curr_stat(victim,STAT_DEX)/2;
    if (IS_AFFECTED(victim,AFF_FLYING))
        chance -= dice(2,5);

    /* level */
    chance += (ch->level - victim->level)*3;
    chance -= 55;

        act("$n shrieks, waddles furiously, and leaps forward!",ch,0,victim,TO_NOTVICT);
        act("$n shrieks, waddles furiously, and leaps forward!",ch,0,victim,TO_VICT);
        act("You shriek, waddle furiously, and leap foward!",ch,0,victim,TO_CHAR);

        act("$N's eyes widen in pure terror as $n's shadow descends apon $M.",ch,0,victim,TO_NOTVICT);
        act("Your eyes widen in pure terror as $n's shadow descends apon you!",ch,0,victim,TO_VICT);
        act("$N's eyes widen in pure terror as your shadow descends apon $M.",ch,0,victim,TO_CHAR);

        if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (victim->fighting == NULL || ch->fighting == NULL))
	{
        sprintf(buf,"Help! %s is sitting on me!",PERS(ch,victim));
	do_myell(victim,buf);
	}

        dam = ch->level*2 + number_percent()*2 - 50;
        damage_old(ch,victim,dam,gsn_siton,DAM_BASH,TRUE);
        check_improve(ch,gsn_siton,TRUE,1);

        if (number_percent() > chance)
        {
	dam = dam+100;
        act("Seizing upon $N's weakness, $n sits on the head of $N and releases a spurt of methane!",ch,0,victim,TO_NOTVICT);
        act("Seizing upon your weakness, $n sits on your head and releases a spurt of methane!",ch,0,victim,TO_VICT);
        act("Seizing upon $N's weakness, you sit on $S head and release a spurt of methane!",ch,0,victim,TO_CHAR);
       if (!IS_NPC(ch) && !IS_NPC(victim))
	{
        sprintf(buf,"Help! %s is farting on me!",PERS(ch,victim));
	do_myell(victim,buf);
	}
	sn_fart = skill_lookup("gas breath");
        damage_old(ch,victim,dam,sn_fart,DAM_BASH,TRUE);
        }
        WAIT_STATE(ch,PULSE_VIOLENCE*3);
        WAIT_STATE(victim,PULSE_VIOLENCE*2);
        return;
}

void do_knife( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj, *weapon;
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );


    if ( (get_skill(ch,gsn_knife) == 0 )
	|| ch->level < skill_table[gsn_knife].skill_level[ch->class])
    {
	if (!IS_NPC(ch))
	{
	send_to_char("You don't know how to knife.\n\r",ch);
	return;
	}
    }

    if (arg[0] == '\0')
    {
        send_to_char("Knife whom?\n\r",ch);
        return;
    }

    if (ch->fighting != NULL)
    {
        send_to_char("No way! You're still fighting!\n\r",ch);
        return;
    }

    if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    if ( victim == ch )
    {
        send_to_char( "Bah, you can't knife yourself.\n\r", ch );
        return;
    }


    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL)
    {
        send_to_char( "You need to wield a weapon to knife.\n\r", ch );
        return;
    }

	weapon = get_eq_char(ch,WEAR_WIELD);


	if (weapon == NULL)
	{
	send_to_char("You must be wielding a weapon to knife.\n\r",ch);
	return;
	}

	if (weapon->value[0] != WEAPON_DAGGER)
	{
	weapon = get_eq_char(ch,WEAR_DUAL_WIELD);
	if (weapon == NULL)
	{
	send_to_char("You must be wielding a weapon to knife.\n\r",ch);
	return;
	}
	}

	if (weapon->value[0] != WEAPON_DAGGER)
	{
	send_to_char("You need to be wielding a dagger to knife.\n\r",ch);
	return;
	}

    if ( is_safe( ch, victim ) )
      return;

	if (check_chargeset(ch,victim)) {return;}

    WAIT_STATE( ch, PULSE_VIOLENCE);
    if ( number_percent( ) < get_skill(ch,gsn_knife))
    {
        check_improve(ch,gsn_knife,TRUE,1);
	if (!IS_NPC(ch) && !IS_NPC(victim))
	{
	switch(number_range(0,1))
	{
	case (0):
	sprintf(buf,"Die, %s you knifing fool!",PERS(ch,victim));
	break;
	case (1):
	sprintf(buf,"Help! %s is knifing me!",PERS(ch,victim));
	break;
	}
	do_myell(victim,buf);
	}


        one_hit( ch, victim, gsn_knife );
    }
    else
    {
        check_improve(ch,gsn_knife,FALSE,2);
	if (!IS_NPC(ch) && !IS_NPC(victim))
	{
	switch(number_range(0,1))
	{
	case (0):
	sprintf(buf,"Die, %s you knifing fool!",PERS(ch,victim));
	break;
	case (1):
	sprintf(buf,"Help! %s knifed me!",PERS(ch,victim));
	break;
	}
	do_myell(victim,buf);
	}


        damage( ch, victim, 0, gsn_knife,DAM_NONE,TRUE);
    }
    return;
}

void do_praclist( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int sn;
    CHAR_DATA *victim;
    int col;

    col = 0;
    argument = one_argument( argument, arg1 );

    if ( arg1[0] == '\0')
    {
	send_to_char( "See whose praclist?\n\r", ch );
	return;
    }
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
	return;
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name == NULL )
		break;

	    if (skill_table[sn].skill_level[victim->class] > 52)
		continue;
	    if ( victim->level < skill_table[sn].skill_level[victim->class]
	      || victim->pcdata->learned[sn] < 1 /* skill is not known */)
		continue;
	if (is_spec_skill(sn) != -1 && !is_specced(victim,is_spec_skill(sn))) {
	    	sprintf( buf, "{W%-18s{x {R%3d%%{x  ",
		skill_table[sn].name, victim->pcdata->learned[sn] );
	    	send_to_char( buf, ch );
	    	if ( ++col % 3 == 0 )
		send_to_char( "\n\r", ch );

			continue;
        }
	    sprintf( buf, "%-18s %3d%%  ",
		skill_table[sn].name, victim->pcdata->learned[sn] );
	    send_to_char( buf, ch );
	    if ( ++col % 3 == 0 )
		send_to_char( "\n\r", ch );
	}

	if ( col % 3 != 0 )
	    send_to_char( "\n\r", ch );

	sprintf( buf, "%d practice sessions left.\n\r",
	    victim->practice );
	send_to_char( buf, ch );
  return;
}

void do_decapitate(CHAR_DATA *ch,char *argument)
{
	char buf[MAX_STRING_LENGTH];
        CHAR_DATA *victim;
        int chance, skill;
	OBJ_DATA *disarmed;
	char arg[MAX_INPUT_LENGTH];
	AFFECT_DATA af;

	one_argument( argument, arg );

        if ( (chance = get_skill(ch,gsn_decapitate)) == 0 )
	{
      	send_to_char("Huh?\n\r",ch);
      	return;
	}

	if (arg[0] == '\0') {
		victim = ch->fighting;
		if (victim == NULL) {
			send_to_char("You aren't fighting anyone.\n\r",ch);
			return;
		}
	}
	else if ((victim = get_char_room(ch,arg)) == NULL)
	{
		send_to_char("They aren't here.\n\r",ch);
		return;
	}

	if (!is_wielded(ch,WEAPON_AXE,WIELD_TWO)) {
		send_to_char("You must be wielding two axes to attempt to decapitate.\n\r",ch);
		return;
	}
	disarmed=get_eq_char(ch,WEAR_WIELD);
	act("$n whirls $s axes impressively with $s hands, and brings their blades together at $N's neck!",ch,0,victim,TO_NOTVICT);
	act("$n whirls $s axes impressively with $s hands, and brings their blades together at your neck!",ch,0,victim,TO_VICT);
	act("You whirl your axes impressively with your hands, and bring their blades together at $N's neck!",ch,0,victim,TO_CHAR);

	/* IF NO DECAPITATE CODE */
	chance = number_percent();
	skill = get_skill(ch,gsn_decapitate);

	if (get_trust(ch) == MAX_LEVEL) {
		chance = 0;
	}

	if ((chance > (skill/10)) && (chance - (skill/10)) < 50) {
		sprintf(buf,"Help! %s is slicing my head off!",PERS(ch,victim));
		do_myell(victim,buf);
		one_hit(ch, victim, gsn_decapitate);
		check_improve(ch,gsn_decapitate,FALSE,1);
		WAIT_STATE(ch,3*PULSE_VIOLENCE);
		return;
	}
	if (chance > (skill/10)) {
		sprintf(buf,"Die, %s you decapitating fool!",PERS(ch,victim));
		do_myell(victim,buf);
		damage( ch, victim, 0, gsn_decapitate,DAM_NONE,TRUE);

		switch(number_range(0,1)) {
		case (0):
			disarmed = get_eq_char(ch,WEAR_WIELD);
		case (1):
			disarmed = get_eq_char(ch,WEAR_DUAL_WIELD);
		}
		if (IS_OBJ_STAT(disarmed,ITEM_NOREMOVE)) {
			act("Your weapon won't budge!",ch,disarmed,0,TO_CHAR);
			act("$n's weapon won't budge!",ch,disarmed,0,TO_ROOM);
		} else {
			act("Your grip on $p loosens and you send it flying!",ch,disarmed,0,TO_CHAR);
			act("$n's grip on $p loosens and $m sends it flying!",ch,disarmed,0,TO_ROOM);
			obj_from_char(disarmed);
			if (IS_OBJ_STAT(disarmed,ITEM_NODROP)) {
				obj_to_char(disarmed, ch);
			} else {
				obj_to_room(disarmed, ch->in_room);
			}
		}
		check_improve(ch,gsn_decapitate,FALSE,1);
		WAIT_STATE(ch,3*PULSE_VIOLENCE);
		return;
	}

	act("$n \x01B[1;31m+++ DECAPITATES +++\x01B[0;37m $N!",ch,0,victim,TO_NOTVICT);
	act("$n \x01B[1;31m+++ DECAPITATES +++\x01B[0;37m you!",ch,0,victim,TO_VICT);
	act("You \x01B[1;31m+++ DECAPITATE +++\x01B[0;37m $N!",ch,0,victim,TO_CHAR);
	init_affect(&af);
        af.where     = TO_AFFECTS;
        af.type      = gsn_decapitate;
        af.aftype    = AFT_SKILL;
        af.bitvector = 0;
	af.modifier  = 0;
	af.duration  = 1;
	af.location  = 0;
        af.level     = ch->level;
        affect_to_char(victim, &af);
	act("You see the world from a new perspective as your head hits the ground.",victim,0,0,TO_CHAR);
	raw_kill(ch,victim);
	check_improve(ch,gsn_decapitate,TRUE,3);
	WAIT_STATE(ch,PULSE_VIOLENCE);
	return;
}

void do_lightwalk(CHAR_DATA *ch,char *argument)
{
	CHAR_DATA *victim;
	char arg1[MAX_INPUT_LENGTH];
	int level;

	one_argument(argument,arg1);

      if (get_skill(ch,gsn_lightwalk) == 0)
      {
      	send_to_char("Huh?\n\r",ch);
	      return;
      }

	if (cabal_down(ch,CABAL_PROTECTOR))
                return;

	if (is_affected(ch,gsn_insect_swarm))
	{
	  send_to_char("You can't concentrate with all these insects swarming you.\n\r",ch);
		return;
	}

	if (ch->mana < 70)
	{
		send_to_char("You do not have the mana.\n\r",ch);
		return;
	}

	if (number_percent() > ch->pcdata->learned[gsn_lightwalk])
	{
		send_to_char("You fail to be transported.\n\r",ch);
		check_improve(ch,gsn_lightwalk,FALSE,1);
		ch->mana -= 35;
		return;
	}
	level=ch->level;
	victim = get_char_world(ch,arg1);
    if ( victim == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   !can_see_room(ch,victim->in_room)
    ||   (victim->in_room->cabal != 0 && victim->in_room->cabal != CABAL_PROTECTOR)
    ||   ch->in_room->guild != 0
    ||   victim->in_room->guild != 0
    ||   (IS_NPC(victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   victim->level >= level + 9
    ||   (saves_spell( level, victim,DAM_OTHER) )
    ||   (!IS_NPC(victim) && !can_pk(ch,victim) && IS_SET(victim->act,PLR_NOSUMMON)))
    {		ch->mana -= 35;
		send_to_char("You failed.\n\r",ch);
		WAIT_STATE(ch,12);
		return;
	}

	ch->mana -= 70;
	act("$n becomes enshrouded in a bright white light and vanishes!",ch,0,0,TO_ROOM);
	char_from_room(ch);
	char_to_room(ch,victim->in_room);
	send_to_char("You become enshrouded in a bright white light and appear at your target.\n\r",ch);
	act("$n appears in a bright flash of light.",ch,0,0,TO_ROOM);
	do_look(ch,"auto");
	WAIT_STATE(ch,12);
	return;
}

void do_bobbit(CHAR_DATA *ch,char *argument)
{
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	OBJ_DATA *weapon;
	OBJ_DATA *weapon2;
	char arg1[MAX_INPUT_LENGTH];
	AFFECT_DATA af;

	argument = one_argument( argument, arg1 );

      if (get_skill(ch,gsn_bobbit) == 0)
      {
      	send_to_char("Huh?\n\r",ch);
	      return;
      }

	if ( arg1[0] == '\0')
	{
		send_to_char("Perform the Bobbit on who?\n\r", ch );
		return;
	}

	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}

	if ( IS_NPC(victim) )
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}
	if ( victim == ch)
	{
		send_to_char( "You can't cut off your own wanker.\n\r", ch);
		return;
	}

	/* CHECK IF WIELDING TWO DAGGERS */
	weapon = get_eq_char(ch,WEAR_WIELD);
	weapon2 = get_eq_char(ch,WEAR_DUAL_WIELD);

        if(weapon == NULL)
        {
         send_to_char("You first need to find a weapon to Bobbit with.\n\r",ch);
         return;
        }

        if(weapon2 == NULL)
        {
         send_to_char("You need to find another weapon to Bobbit with.\n\r",ch);
         return;
        }

	if (weapon == NULL || weapon2 == NULL || (weapon->value[0] != WEAPON_DAGGER && weapon2->value[0] != WEAPON_DAGGER) )
	{
		send_to_char("You must be wielding two daggers to attempt to Bobbit.\n\r",ch);
		return;
	}

	if (victim->position != POS_SLEEPING && !(ch->level > LEVEL_HERO))
	{
		send_to_char("You cannot do this when that person is awake!\n\r",ch);
		return;
	}

	if (victim->sex != SEX_MALE) {
		send_to_char("But that isn't a guy!\n\r",ch);
		return;
	}

	act("$n slowly lowers $N's pants, whips out two glinting daggers, and slashes madly!",ch,0,victim,TO_NOTVICT);
	act("You feel your pants being lowered. Curious.",ch,0,victim,TO_VICT);
	act("You slower lower $N's pants, brandish two glinting daggers, and slash madly!",ch,0,victim,TO_CHAR);

	switch(number_range(0,3)) {
	case (0):
		sprintf(buf,"Help! %s is severing my private parts!",PERS(ch,victim));
		break;
	case (1):
		sprintf(buf,"Help! %s is slicing off my genitalia!",PERS(ch,victim));
		break;
	case (2):
		sprintf(buf,"Help! %s is chopping off my organs of generation!",PERS(ch,victim));
		break;
	case (3):
		sprintf(buf,"Help! %s is chopping off my reproductive organs!",PERS(ch,victim));
		break;
	}
	act("You shriek in pure horror!",victim,0,0,TO_CHAR);
	act("$n shrieks in pure horror!",victim,0,0,TO_ROOM);


	do_myell(victim,buf);

	one_hit(ch, victim, gsn_bobbit);
	check_improve(ch,gsn_bobbit,TRUE,1);
	WAIT_STATE(ch,2*PULSE_VIOLENCE);

	init_affect(&af);
	af.where     = TO_AFFECTS;
	af.type      = gsn_bobbit;
	af.aftype    = AFT_SKILL;
	af.level     = ch->level;
	af.duration  = ch->level/5;
	af.bitvector = 0;
	af.location  = APPLY_DEX;
	af.modifier  = -15;
	affect_to_char(victim,&af);
	af.location  = APPLY_SEX;
	af.modifier  = 2;
	af.bitvector = 0;
	affect_to_char(victim,&af);
	return;
}

void do_impale(CHAR_DATA *ch,char *argument)
{
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	int chance, skill;
	OBJ_DATA *weapon;
	char arg[MAX_INPUT_LENGTH];
	AFFECT_DATA af;

	one_argument( argument, arg );

	if ( (chance = get_skill(ch,gsn_impale)) == 0 )
	{
	      	send_to_char("Huh?\n\r",ch);
      		return;
	}


	if (ch->fighting != NULL)
	{
		send_to_char("No way! You're still fighting!\n\r",ch);
		return;
	}

	/* CHECK IF PRIMARY WIELDING SPEAR */
	weapon = get_eq_char(ch,WEAR_WIELD);

        if (weapon == NULL)
        {
		send_to_char("You first need to find a weapon to impale with.\n\r",ch);
		return;
        }

	if (weapon->value[0] != WEAPON_SPEAR )
	{
		send_to_char("You must be wielding a spear to be able to impale.\n\r",ch);
		return;
	}

	if ( arg[0] == '\0')
	{
		send_to_char("Impale who?\n\r", ch );
		return;
	}

	if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;

	}
        if (victim==ch)
	{
          send_to_char("Trying to impale yourself ?\n\r", ch);
          return;
	}
	if (victim->fighting != NULL) //Impale Fix to stop lamers *Ceial*
	{
	    send_to_char("They are moving too much to impale.\n\r",ch);
	    return;
	}
	    if (victim->hit < victim->max_hit*8/10)
	{
	    send_to_char("They are too hurt and watchful to impale right now...\n\r",ch);
	    return;
    }

	if (is_safe( ch, victim ))
	return;


	if (is_affected(victim,gsn_impale)) {
                send_to_char( "They already have a chest wound.\n\r", ch );
                return;
	}

	if (check_chargeset(ch,victim)) {return;}

	act("$n drives $s spear forward into $N's chest, attempting to pierce $S heart!",ch,0,victim,TO_NOTVICT);
	act("$n drives $s spear forward into your chest, attempting to pierce your heart!",ch,0,victim,TO_VICT);
	act("You drive your spear forward into $N's chest, attempting to pierce $S heart!",ch,0,victim,TO_CHAR);

	chance = number_percent();
	skill = get_skill(ch,gsn_impale);

	if ((skill > 90) && (chance > 95)) {
		act("$n \x01B[1;31m+++ IMPALES +++\x01B[0;37m $N!",ch,0,victim,TO_NOTVICT);
		act("$n \x01B[1;31m+++ IMPALES +++\x01B[0;37m you!",ch,0,victim,TO_VICT);
		act("You \x01B[1;31m+++ IMPALE +++\x01B[0;37m $N!",ch,0,victim,TO_CHAR);

		raw_kill(ch,victim);
		act("You gasp for breath as you feel your heart explode.",victim,0,0,TO_CHAR);
		check_improve(ch,gsn_impale,TRUE,3);
		WAIT_STATE(ch,PULSE_VIOLENCE);
		return;
	}

	if (!IS_NPC(victim)) {
	switch(number_range(0,2)) {
	case (0):
		sprintf(buf,"Help! %s is skewering me on a spear!",PERS(ch,victim));
		break;
	case (1):
		sprintf(buf,"Help! %s is spiking my heart!",PERS(ch,victim));
		break;
	case (2):
		sprintf(buf,"Help! %s is impaling me!",PERS(ch,victim));
		break;
	}
	do_myell(victim,buf);
	}

	if (skill < chance+5) {
		check_improve(ch,gsn_impale,FALSE,2);
		damage( ch, victim, 0, gsn_impale,DAM_NONE,TRUE);
		multi_hit( victim, ch, TYPE_UNDEFINED );
		return;
	} else {
		one_hit(ch, victim, gsn_impale);
		check_improve(ch,gsn_impale,TRUE,1);
		WAIT_STATE(ch,2*PULSE_VIOLENCE);

		init_affect(&af);
		af.where     = TO_AFFECTS;
		af.type      = gsn_impale;
		af.aftype    = AFT_MALADY;
		af.level     = ch->level;
		af.duration  = ch->level/5;
		af.bitvector = 0;
		af.location  = APPLY_DEX;
		af.modifier  = -(ch->level/5);
		affect_to_char(victim,&af);
		af.location  = APPLY_STR;
		af.modifier  = -(ch->level/5);
		af.bitvector = 0;
		affect_to_char(victim,&af);
		return;
	}
}

void do_affrem(CHAR_DATA *ch, char *argument) {
	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	int sn;

	argument = one_argument(argument,arg1);
	argument = one_argument(argument,arg2);

	if (arg1[0] == '\0')
	{
		send_to_char("Syntax: affrem <char> <affect>\n\r",ch);
		return;
	}

	if ((victim = get_char_world(ch,arg1)) == NULL)
	{
		send_to_char( "They aren't playing.\n\r",ch);
		return;
	}

	if (arg2[0] == '\0')
	{
		send_to_char("Syntax: affrem <char> <affect>\n\r",ch);
		return;
	}

	sn = 0;
	if ((sn = skill_lookup(arg2)) < 0)
	{
		send_to_char("No such skill or spell.\n\r", ch);
		return;
	}

	if (!(is_affected(victim,sn)))
	{
		send_to_char("Target is not affected by that.\n\r", ch);
		return;
	}

	affect_strip(victim, sn);
	if (skill_table[sn].msg_off )
	{
		send_to_char(skill_table[sn].msg_off, victim );
		send_to_char("\n\r", victim);
	}

	sprintf(buf,"The %s affect is removed from %s.\n\r",skill_table[sn].name,victim->name);
	send_to_char(buf, ch);
	return;
}

void do_divine_intervention(CHAR_DATA *ch, char *argument) {
	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	char arg3[MAX_STRING_LENGTH];
	char arg4[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	int enchant_type, value, duration;
	bool isgoodaffect;
	AFFECT_DATA af;

	argument = one_argument(argument,arg1);
	argument = one_argument(argument,arg2);
	argument = one_argument(argument,arg3);
	argument = one_argument(argument,arg4);

	if ((arg1[0] == '\0') || (arg2[0] == '\0') || (arg3[0] == '\0') || arg4[0] == '\0')
	{
		send_to_char("Syntax:  intervention <char> <affect> <value> <duration>\n\r",ch);
		send_to_char("Affects: hp mana move ac hitroll damroll saves\n\r", ch);
		return;
	}

	if ((victim = get_char_world(ch,arg1)) == NULL)
	{
		send_to_char("They aren't playing.\n\r",ch);
		return;
	}

	if (!(is_number(arg3))) {
		send_to_char("Value must be numeric.\n\r",ch);
		return;
	}

	if (!(is_number(arg4))) {
		send_to_char("Duration must be numeric.\n\r",ch);
		return;
	}

	isgoodaffect = FALSE;

	value = atoi(arg3);
	duration = atoi(arg4);

	if (!str_prefix(arg2,"hp")) {
		if (value > 0) {isgoodaffect = TRUE;}
		enchant_type=APPLY_HIT;
	} else if (!str_prefix(arg2,"mana")) {
		if (value > 0) {isgoodaffect = TRUE;}
		enchant_type=APPLY_MANA;
	} else if (!str_prefix(arg2,"move")) {
		if (value > 0) {isgoodaffect = TRUE;}
		enchant_type=APPLY_MOVE;
	} else if (!str_prefix(arg2,"ac")) {
		if (value < 0) {isgoodaffect = TRUE;}
		enchant_type=APPLY_AC;
	} else if (!str_prefix(arg2,"hitroll")) {
		if (value > 0) {isgoodaffect = TRUE;}
		enchant_type=APPLY_HITROLL;
	} else if (!str_prefix(arg2,"damroll")) {
		if (value > 0) {isgoodaffect = TRUE;}
		enchant_type=APPLY_DAMROLL;
	} else if (!str_prefix(arg2,"saves")) {
		if (value < 0) {isgoodaffect = TRUE;}
		enchant_type=APPLY_SAVING_SPELL;
	} else {
		send_to_char("No affect of that name.\n\r",ch);
		return;
	}

        init_affect(&af);
	af.where        = TO_AFFECTS;
	af.type         = gsn_divine_intervention;
	af.aftype	= AFT_SKILL;
	af.level        = ch->level;
	af.duration     = duration;
	af.modifier     = value;
	af.bitvector    = 0;
	af.location     = enchant_type;
	affect_to_char(victim,&af);

	if (isgoodaffect == TRUE)
	{
		sprintf(buf,"%s is blessed with %s.\n\r",victim->name,arg2);
		send_to_char(buf,ch);
		send_to_char("You have been blessed by the Immortals.\n\r", victim);
	} else {
		sprintf(buf,"%s is cursed with %s.\n\r",victim->name,arg2);
		send_to_char(buf,ch);
		send_to_char("You have been cursed by the Immortals.\n\r", victim);
	}

	return;
}


void do_ccb( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int cabal;
	DESCRIPTOR_DATA *d;
	argument = one_argument(argument,arg1);
	strcpy(arg2, argument);

	if (arg1[0] == '\0' )
	{
		send_to_char("Syntax: ccb <cabal> <message>\n\r",ch);
		return;
	}

	if (arg2[0] == '\0' )
	{
		send_to_char("Syntax: ccb <cabal> <message>\n\r",ch);
		return;
	}

	cabal = cabal_lookup(arg1);
	if (!(cabal)) {
		send_to_char("That's not a valid cabal.\n\r",ch);
		return;
	}

	if (IS_SET(ch->act,PLR_COLOR))
	{
	      sprintf(buf, "\x01B[1;37m%s%s: %s\x01B[0;37m\n\r",
			cabal_table[cabal].who_name,
			IS_NPC(ch) ? ch->short_descr : ch->name,
			arg2
		);
      } else {
      	sprintf(buf, "%s%s: %s\n\r",
			cabal_table[cabal].who_name,
			IS_NPC(ch) ? ch->short_descr : ch->name,
			arg2
		);
	}

	send_to_char(buf, ch);

	for (d = descriptor_list; d != NULL; d = d->next)
	{
		if (d->connected == CON_PLAYING)
		{
			if (((d->character != ch) && (!IS_SET(d->character->comm,COMM_NOCABAL)) && (d->character->cabal) && (d->character->cabal == cabal))
			|| ((IS_SET(d->character->comm,COMM_ALL_CABALS)) && (d->character != ch)))
			{
				/* if (d->character->cabal != cabal) {
					continue;
				} */
				if (IS_SET(d->character->act,PLR_COLOR))
				{
					sprintf(buf, "\x01B[1;37m%s%s: %s\x01B[0;37m\n\r",
						cabal_table[cabal].who_name,
						PERS(ch,d->character),
						arg2
					);
				} else {
					sprintf(buf, "%s%s: %s\n\r",
						cabal_table[cabal].who_name,
						PERS(ch,d->character),
						arg2
					);
				}
				send_to_char(buf, d->character);
			}
		}
	}
	return;
}

void do_stun( CHAR_DATA *ch,char *argument)
{
	AFFECT_DATA af;
	CHAR_DATA *victim;
	int chance;
	OBJ_DATA *weapon;

	if ((chance = get_skill(ch,gsn_stun)) == 0
	|| ( ch->level < skill_table[gsn_stun].skill_level[ch->class]) )
	{
		send_to_char("You don't know how to stun people.\n\r",ch);
		return;
	}

	if ((victim = ch->fighting) == NULL)
	{
		send_to_char("You can't stun anyone if you're not fighting.\n\r",ch);
		return;
	}

	if (victim == ch)
	{
		send_to_char("You pummel yourself repeatedly to no avail.\n\r",ch);
		return;
	}
	weapon = get_eq_char(ch,WEAR_WIELD);

	if (weapon != NULL) {
		send_to_char("You cannot stun while wielding a weapon.\n\r",ch);
		return;
	}

	if (is_safe(ch,victim))
	return;

	if (victim->position == POS_SLEEPING)
	{
		send_to_char("But that person is already asleep!\n\r",ch);
		return;
	}

	if (is_affected(victim,gsn_stun))
	{
		send_to_char("They are already quite stunned.\n\r",ch);
		return;
	}

	chance += (ch->drain_level - victim->level);
	if (is_affected(victim,skill_lookup("haste")))
		chance -= 20;
	if (is_affected(ch,skill_lookup("haste")))
		chance += 10;

	chance -= 25;

	if (ch->level == MAX_LEVEL) {
		chance = 100;
	}

	if (!(str_cmp(race_table[victim->race].name,"lich")))
	{
		chance = 0;
	}

	act("You pummel $N with your fists, stunning $m.",ch,0,victim,TO_CHAR);
	act("$n pummmels $N with $s fists, stunning $M.",ch,0,victim,TO_NOTVICT);
	act("$n pummels at you with $s fists, stunning you.",ch,0,victim,TO_VICT);

	if (number_percent() < chance)
	{
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.level = ch->level;
		af.location = 0;
		af.type = gsn_stun;
		af.aftype = AFT_SKILL;
		af.duration = 2;
		af.bitvector = 0;
		af.modifier = 0;
		affect_to_char(victim,&af);
		stop_fighting(victim, TRUE);
		victim->position = POS_SLEEPING;
		check_improve(ch,gsn_stun,TRUE,2);
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
		return;
	} else {
		one_hit(ch, victim, gsn_stun);
		check_improve(ch,gsn_stun,FALSE,3);
		WAIT_STATE(ch, 3*PULSE_VIOLENCE);
		WAIT_STATE(victim, PULSE_VIOLENCE);
	}
	return;
}

void spell_divine_aid(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam, eng=0, gsn, add;

	dam = level * 3;

	if (IS_GOOD(ch)) {
		eng = 1;
	} else if (IS_EVIL(ch)) {
		eng = -1;
	} else {
		switch(number_range(0,1)) {
		case (0):
			eng = 1;
			break;
		case (1):
			eng = -1;
			break;
		}
		dam -= level + 25;
	}

	if (eng == 1) {
		act("$n summons forth an angel to aid $m!",ch,NULL,victim,TO_NOTVICT);
		act("$n summons forth an angel to aid $m!",ch,NULL,victim,TO_VICT);
		act("You summon forth an angel to aid you!",ch,NULL,victim,TO_CHAR);
		gsn = skill_lookup("wrath");
		dam += add = number_percent();
		damage_old(ch,victim,dam,gsn,DAM_HOLY,TRUE);
		dam -= add;
		dam += add = number_percent();
		damage_old(ch,victim,dam,gsn,DAM_HOLY,TRUE);
		dam -= add;
		dam += add = number_percent();
		damage_old(ch,victim,dam,gsn,DAM_HOLY,TRUE);
	} else {
		act("$n summons forth a demon to aid $m!",ch,NULL,victim,TO_NOTVICT);
		act("$n summons forth a demon to aid $m!",ch,NULL,victim,TO_VICT);
		act("You summon forth a demon to aid you!",ch,NULL,victim,TO_CHAR);
		gsn = skill_lookup("dark wrath");
		dam += add = number_percent();
		damage_old(ch,victim,dam,gsn,DAM_NEGATIVE,TRUE);
		dam -= add;
		dam += add = number_percent();
		damage_old(ch,victim,dam,gsn,DAM_NEGATIVE,TRUE);
		dam -= add;
		dam += add = number_percent();
		damage_old(ch,victim,dam,gsn,DAM_NEGATIVE,TRUE);
	}
	return;

}

void do_shieldbash( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance, wait;
    char buf[MAX_STRING_LENGTH];
    bool FightingCheck;
    OBJ_DATA *obj;

    if (ch->fighting != NULL)
	FightingCheck = TRUE;
    else
	FightingCheck = FALSE;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_shieldbash)) == 0
    ||	 (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BASH))
    ||	 (!IS_NPC(ch)
    &&	  ch->level < skill_table[gsn_shieldbash].skill_level[ch->class]))
    {
	send_to_char("Shield bashing? What's that?\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("But you aren't fighting anyone!\n\r",ch);
	    return;
	}
    }


    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
	return;
    }

    if (victim->position < POS_FIGHTING)
    {
	act("You'll have to let $M get back up first.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim == ch)
    {
	send_to_char("You try to bash your head with your shield, but fail.\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;

    if ( ( obj = get_eq_char( ch, WEAR_SHIELD ) ) == NULL )
	{
	send_to_char("You need a shield to bash with it!\n\r",ch);
	return;
	}

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
	act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if (is_affected(victim, gsn_protective_shield))
     {
	act("Your shield bash seems to slide around $N.", ch, NULL, victim, TO_CHAR);
	act("$n's bash slides off your protective shield.", ch, NULL, victim,
	    TO_VICT);
	act("$n's bash seems to slide around $N.",ch,NULL,victim,TO_NOTVICT);
	return;
     }

    /* modifiers */

    /* size  and weight */
    chance += ch->carry_weight / 25;
    chance -= victim->carry_weight / 20;

    if (ch->size < victim->size)
	chance += (ch->size - victim->size) * 25;
    else
	chance += (ch->size - victim->size) * 10;


    /* stats */
    chance += get_curr_stat(ch,STAT_STR);
    chance -= get_curr_stat(victim,STAT_DEX) * 4/3;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST))
	chance -= 20;

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* now the attack */
    if (number_percent() < chance)
    {

	act("$n sends you sprawling with a powerful shield bash!",
		ch,NULL,victim,TO_VICT);
	act("You slam into $N with your shield, and send $M flying!",ch,NULL,victim,TO_CHAR);
	act("$n sends $N sprawling with a powerful bash with his shield.",
		ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_shieldbash,TRUE,1);

	wait = 3;

	switch(number_bits(2)) {
	case 0: wait = 1; break;
	case 1: wait = 2; break;
	case 2: wait = 4; break;
	case 3: wait = 3; break;
	}

	WAIT_STATE(victim, wait * PULSE_VIOLENCE);
	WAIT_STATE(ch,skill_table[gsn_shieldbash].beats+PULSE_VIOLENCE);
	victim->position = POS_RESTING;
	damage(ch,victim,number_range(4,4 + 4 * ch->size + chance/3),gsn_shieldbash,
	    DAM_BASH, TRUE);

    }
    else
    {
	damage(ch,victim,0,gsn_shieldbash,DAM_BASH, TRUE);
	act("You fall flat on your face!",
	    ch,NULL,victim,TO_CHAR);
	act("$n falls flat on $s face.",
	    ch,NULL,victim,TO_NOTVICT);
	act("You evade $n's shield bash, causing $m to fall flat on $s face.",
	    ch,NULL,victim,TO_VICT);
	check_improve(ch,gsn_shieldbash,FALSE,1);
	ch->position = POS_RESTING;
	WAIT_STATE(ch,skill_table[gsn_shieldbash].beats * 3/2);
    }
    if (!(IS_NPC(victim)) && !(IS_NPC(ch)) && victim->position > POS_STUNNED
		&& !FightingCheck)
      {
	if (!can_see(victim, ch))
	  do_yell(victim, "Help! Someone is bashing me with a shield!");
	else
	  {
	    sprintf(buf, "Help! %s is bashing me with a shield!",
		PERS(ch,victim));
	    do_yell(victim, buf);
	  }
      }
}

void do_boneshatter( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	OBJ_DATA *weapon;
	int skill;
	AFFECT_DATA af;

	if ((skill = get_skill(ch,gsn_boneshatter)) == 0) {
		send_to_char("You don't know how to boneshatter properly.\n\r",ch);
		return;
	}

	if ((victim = ch->fighting) == NULL)
	{
		send_to_char("You can't shatter bones when you're not fighting.\n\r",ch);
		return;
	}

	weapon = get_eq_char(ch,WEAR_WIELD);

	if (weapon == NULL)
	{
		send_to_char("You must be wielding a weapon for that.\n\r",ch);
		return;
	}

	if (weapon->value[0] != WEAPON_MACE)
	{
		weapon = get_eq_char(ch,WEAR_DUAL_WIELD);
		if (weapon == NULL)
		{
			send_to_char("You must be wielding a weapon to for that.\n\r",ch);
			return;
		}
	}

	if (weapon->value[0] != WEAPON_MACE)
	{
		send_to_char("You need to be wielding a mace to shatter bones.\n\r",ch);
		return;
	}

	if (is_affected(victim,gsn_boneshatter)) {
		send_to_char("Those bones are already shattered.\n\r",ch);
		return;
	}

	act("You bring $p around with bone-shattering force!",ch,weapon,victim,TO_CHAR);
	act("$n brings $p around with bone-shattering force!",ch,weapon,0,TO_ROOM);

	if (number_percent( ) < (get_skill(ch,gsn_boneshatter) - 15))
	{
		one_hit(ch, victim, gsn_boneshatter);
		init_affect(&af);
		af.where     = TO_AFFECTS;
		af.type      = gsn_boneshatter;
		af.aftype    = AFT_SKILL;
		af.level     = ch->level;
		af.duration  = ch->level/5;
		af.bitvector = 0;
		af.location  = APPLY_DEX;
		af.modifier  = 1-(ch->level/5);
		affect_to_char(victim,&af);
		af.location  = APPLY_STR;
		af.modifier  = 1-(ch->level/5);
		af.bitvector = 0;
		affect_to_char(victim,&af);
		check_improve(ch,gsn_boneshatter,TRUE,1);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
	} else {
		damage(ch, victim, 0, gsn_boneshatter,DAM_NONE,TRUE);
		check_improve(ch,gsn_boneshatter,FALSE,2);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
	}

	return;
}


void spell_rejuvenate( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	victim->hit = UMIN( victim->hit + 300, victim->max_hit );
	update_pos( victim );
	send_to_char( "A very warm feeling fills your body.\n\r", victim );
	if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
	return;
}

void do_powers(CHAR_DATA *ch, char *argument)
{
	BUFFER *buffer;
	char arg[MAX_INPUT_LENGTH];
	char skill_list[LEVEL_HERO + 1][MAX_STRING_LENGTH];
	char skill_columns[LEVEL_HERO + 1];
	int gn, gns, sn, level, min_lev = 1, max_lev = LEVEL_HERO;
	bool fAll = FALSE, found = FALSE;
	char buf[MAX_STRING_LENGTH];


	if (IS_NPC(ch))
	return;

	if (ch->cabal == 0) {
		send_to_char("You are homeless; you have no powers.\n\r",ch);
		return;
	}

	if (argument[0] != '\0')
	{
		fAll = TRUE;

		if (str_prefix(argument,"all"))
		{
			argument = one_argument(argument,arg);
			if (!is_number(arg))
			{
				send_to_char("Arguments must be numerical or all.\n\r",ch);
				return;
			}

			max_lev = atoi(arg);

			if (max_lev < 1 || max_lev > LEVEL_HERO)
			{
				sprintf(buf,"Levels must be between 1 and %d.\n\r",LEVEL_HERO);
				send_to_char(buf,ch);
				return;
			}

			if (argument[0] != '\0')
			{
				argument = one_argument(argument,arg);
				if (!is_number(arg))
				{
					send_to_char("Arguments must be numerical or all.\n\r",ch);
					return;
				}
				min_lev = max_lev;
				max_lev = atoi(arg);

				if (max_lev < 1 || max_lev > LEVEL_HERO)
				{
					sprintf(buf,"Levels must be between 1 and %d.\n\r",LEVEL_HERO);
					send_to_char(buf,ch);
					return;
				}

				if (min_lev > max_lev)
				{
					send_to_char("That would be silly.\n\r",ch);
					return;
				}
			}
		}
	}

	/* initialize data */
	for (level = 0; level < LEVEL_HERO + 1; level++)
	{
		skill_columns[level] = 0;
		skill_list[level][0] = '\0';
	}

	gn = group_lookup(cabal_table[ch->cabal].name);

	for (gns = 0; gns < MAX_SKILL; gns++)
	{
		if (group_table[gn].spells[gns] == NULL )
		break;

		sn = skill_lookup(group_table[gn].spells[gns]);

		if ((level = skill_table[sn].skill_level[ch->class]) < LEVEL_HERO + 1
		&&  level >= min_lev && level <= max_lev
		&&  ch->pcdata->learned[sn] > 0)
		{
			found = TRUE;
			level = skill_table[sn].skill_level[ch->class];
			if (ch->level < level) {
				sprintf(buf,"%-18s n/a      ", skill_table[sn].name);
			} else {
				sprintf(buf,"%-18s %3d%%      ",skill_table[sn].name,	ch->pcdata->learned[sn]);
 			}

			if (skill_list[level][0] == '\0')
			{
				sprintf(skill_list[level],"\n\rLevel %2d: %s",level,buf);
			} else /* append */ {
				if ( ++skill_columns[level] % 2 == 0)
				{
				strcat(skill_list[level],"\n\r          ");
				}
				strcat(skill_list[level],buf);
			}
		}
	}

	/* return results */

	if (!found)
	{
		send_to_char("No powers found.\n\r",ch);
      	return;
	}

	buffer = new_buf();
	for (level = 0; level < LEVEL_HERO + 1; level++)
	{
		if (skill_list[level][0] != '\0')
		{
			add_buf(buffer,skill_list[level]);
		}
	}
	add_buf(buffer,"\n\r");
	page_to_char(buf_string(buffer),ch);
	free_buf(buffer);
	return;
}

void spell_spiderhands( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
		send_to_char("You is already affected by spiderhands.\n\r",ch);
		else
		act("$N is already affected by spiderhands.",ch,NULL,victim,TO_CHAR);
		return;
	}
	init_affect(&af);
	af.where     = TO_AFFECTS;
	af.type      = sn;
	af.aftype    = AFT_POWER;
	af.level     = level;
	af.duration  = 10;
	af.modifier  = 0;
	af.bitvector = 0;
	affect_to_char( victim, &af );
	send_to_char( "Your hands are covered in a sticky coating.\n\r", victim );
	if ( ch != victim )
	act("$N's hands are covered in a sticky coating.",ch,NULL,victim,TO_CHAR);

	return;
}

bool check_chargeset(CHAR_DATA *ch, CHAR_DATA *victim)
{
	int chance;
	OBJ_DATA *weapon;

	if ((chance = get_skill(victim,gsn_chargeset)) == 0
		|| victim->level < skill_table[gsn_chargeset].skill_level[ch->class])
	return FALSE;

	/* check if primary wielding a polearm */
	weapon = get_eq_char(victim,WEAR_WIELD);

	if (weapon == NULL)
	{
		return FALSE;
	}

	if ((weapon->value[0] != WEAPON_POLEARM && weapon->value[0] != WEAPON_STAFF && weapon->value[0] != WEAPON_SPEAR))
	{
		return FALSE;
	}

	/* determine ups/downs in chance */

	if (!can_see(victim,ch))
		chance -= 30;

	chance += (victim->level - ch->level);

	if (victim->position != POS_FIGHTING && IS_AWAKE(victim))
	{
		if (number_percent() < chance)
		{
			act("$n's attack is suddenly halted by the tip of $p!",ch,weapon,victim,TO_VICT);
			act("Your attack is suddenly halted by the tip of $p!",ch,weapon,victim,TO_CHAR);
			act("$n's attack is suddenly halted by the tip of $p!",ch,weapon,victim,TO_NOTVICT);
			check_improve(victim,gsn_chargeset,TRUE,1);
			one_hit(victim,ch,gsn_chargeset);
			return TRUE;
		}
		act("Your $p fails to stop $n.",ch,weapon,victim,TO_VICT);
		check_improve(victim,gsn_chargeset,FALSE,2);
	}

	return FALSE;
}

void do_headclap( CHAR_DATA *ch,char *argument)
{
	AFFECT_DATA af;
	CHAR_DATA *victim;
	int chance;
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_STRING_LENGTH];
	OBJ_DATA *weapon;

	one_argument (argument, arg);

	if ((chance = get_skill(ch,gsn_headclap)) == 0)
	{
		send_to_char("You don't know how to headclap people.\n\r",ch);
		return;
	}

	if (arg[0] == '\0')
	{
		victim = ch->fighting;
		if (victim == NULL)
		{
			send_to_char("But you aren't fighting anyone.\n\r",ch);
			return;
		}
	}
	else if ((victim = get_char_room(ch,arg)) == NULL)
	{
		send_to_char("They aren't here.\n\r",ch);
		return;
	}

	if (is_affected(victim,gsn_noheadclap))
	{
		send_to_char("They are guarding their ears to well.\n\r",ch);
		return;
	}

	if (victim == ch)
	{
		send_to_char("You headclap yourself repeatedly to no avail.\n\r",ch);
		return;
	}

	weapon = get_eq_char(ch,WEAR_WIELD);

	if (weapon != NULL) {
		send_to_char("You cannot headclap while wielding a weapon.\n\r",ch);
		return;
	}

	if (is_safe(ch,victim))
	return;

	if (is_affected(victim,gsn_deafen))
	{
		send_to_char("Their ears are already deafened.\n\r",ch);
		return;
	}

	chance += ch->level - victim->level;
	chance += get_curr_stat(ch,STAT_STR);
	chance -= get_curr_stat(victim,STAT_DEX);
	if (is_affected(ch,skill_lookup("haste")))
		chance += 10;
	if (is_affected(victim,skill_lookup("haste")))
		chance -= 10;

	chance -= 45;

	if (chance > 60) {
		chance = 60;
	}

	if (ch->level == MAX_LEVEL) {
		chance = 100;
	}

	if (!IS_NPC(victim) && (victim->fighting == NULL)) {
		switch(number_range(0,1)) {
		case (0):
			sprintf(buf,"Help! %s is slapping my head!",PERS(ch,victim));
			break;
		case (1):
			sprintf(buf,"Help! %s is boxing my ears!",PERS(ch,victim));
			break;
		}
		do_myell(victim,buf);
	}

	act("You whirl your arms at $N, slamming your palms against $S head!",ch,0,victim,TO_CHAR);
	act("$n whirls $s arms at $N, slamming $s palms against $S head!",ch,0,victim,TO_NOTVICT);
	act("$n whirls $s arms at you, slamming $s palms against your head!",ch,0,victim,TO_VICT);

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.level = ch->level;
		af.location = 0;
		af.type = gsn_deafen;
		af.duration = 4;
		af.bitvector = 0;
		af.modifier = 0;
	if (number_percent() < chance)
	{
		one_hit(ch, victim, gsn_headclap);
		act("$n appears deafened.",victim,0,0,TO_ROOM);
		act("A loud ringing fills your ears... you can't hear anything!",victim,0,0,TO_CHAR);
		init_affect(&af);
		af.duration = 2;
		affect_to_char(victim,&af);
		af.where = TO_AFFECTS;
		af.location = 0;
		af.bitvector = 0;
		af.type=gsn_noheadclap;
		af.aftype = AFT_SKILL;
		af.duration=10;
		affect_to_char(victim,&af);
		check_improve(ch,gsn_headclap,TRUE,2);
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
		return;
	} else {
		one_hit(ch, victim, gsn_headclap);
		check_improve(ch,gsn_headclap,FALSE,3);
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
	}
	return;
}

void do_heroimm( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char arg[MAX_STRING_LENGTH];

	if (IS_NPC(ch))
	return;

	one_argument(argument,arg);

	if (ch->level < MAX_LEVEL) {
		return;
	}

	if (arg[0] == '\0')
	{
		send_to_char("Heroimm who?\n\r",ch);
		return;
	}

	if ((victim = get_char_world(ch,arg)) == NULL) {
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (victim == ch) {
		send_to_char("Uh, no?\n\r",ch);
		return;
	}

	if (IS_HEROIMM(victim)) {
		send_to_char("They are already a Hero Imm.\n\r", ch);
		return;
	}

	SET_BIT(victim->act,PLR_HEROIMM);
	SET_BIT(victim->act,PLR_HOLYLIGHT);

	victim->max_hit = 10000;
	victim->hit = victim->max_hit;
	victim->max_mana = 10000;
	victim->mana = victim->max_mana;
	victim->max_move = 10000;
	victim->move = victim->max_move;

	send_to_char("They are now a Hero Immortal.\n\r", ch);
	send_to_char("You are now a Hero Immortal.\n\r", victim);

	return;
}

void check_eagle_eyes(CHAR_DATA *ch, CHAR_DATA *victim)
{
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *gch;
	int chance;

	for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
	{
		if (gch == ch || gch == victim) {
			continue;
		}
		if (!can_see(gch,ch)) {
			continue;
		}
		chance = get_skill(gch,gsn_eagle_eyes);
		if (chance == 0) {
			continue;
		}
		if (number_percent() < chance)
		{
			sprintf(buf,"You notice %s slipping into %s's inventory.\n\r",PERS(ch,gch),PERS(victim,gch));
			send_to_char(buf,gch);
			check_improve(gch,gsn_eagle_eyes,TRUE,1);
		}
	}

	return;
}

bool check_cutoff(CHAR_DATA *ch, CHAR_DATA *victim)
{
	int chance;
	OBJ_DATA *weapon;

	if ((chance = get_skill(victim,gsn_cutoff)) == 0)
	return FALSE;

	/* check if primary wielding a polearm */
	weapon = get_eq_char(victim,WEAR_WIELD);

	if (weapon == NULL)
	{
		return FALSE;
	}

	if ((weapon->value[0] != WEAPON_POLEARM && weapon->value[0] != WEAPON_STAFF && weapon->value[0] != WEAPON_SPEAR))
	{
		return FALSE;
	}

	/* determine ups/downs in chance */

	if (!can_see(victim,ch))
		chance -= 30;

	chance += (victim->level - ch->level);

	if (chance > 65)
		chance = 65;

	if (number_percent() < chance)
	{
		act("$n's route of escape is cut off by $p.",ch,weapon,victim,TO_VICT);
		act("Your route of escape is cut off by $p.",ch,weapon,victim,TO_CHAR);
		act("$n's route of escape is cut off by $p.",ch,weapon,victim,TO_NOTVICT);
		check_improve(victim,gsn_cutoff,TRUE,1);
		return TRUE;
	} else {
		act("You fail to cut off $n's route of escape.",ch,weapon,victim,TO_VICT);
		check_improve(victim,gsn_cutoff,FALSE,2);
		return FALSE;
	}
}

void report_cabal_items(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char pbuf[MAX_STRING_LENGTH];
	CHAR_DATA *gch;
	CHAR_DATA *guardian;
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	bool found;
	bool afound;
	int total=0;
	if (strstr(argument,"which items") || strstr(argument,"Which items"))
	{

        if (ch->cabal == 0 && !IS_IMMORTAL(ch))
                return;

	found = FALSE;
	afound = FALSE;
	guardian = NULL;
	for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
	{
		if (isCabalGuard(gch)) {
			if (ch->cabal != gch->cabal && !IS_IMMORTAL(ch)) {
				continue;
			}
			sprintf(buf,"I hold ");
			for (obj = gch->carrying; obj != NULL; obj = obj_next )
			{
				obj_next = obj->next_content;
				if ((obj->pIndexData->vnum) && isCabalItem(obj))
				{
				total++;
				found = TRUE;
				if (obj_next != NULL) {
					sprintf(pbuf,"%s, ",obj->short_descr);
					strcat(buf,pbuf);
				} else {
					if (total == 1) {
						sprintf(pbuf,"%s.",obj->short_descr);
						strcat(buf,pbuf);
					} else {
						sprintf(pbuf,"and %s.",obj->short_descr);
						strcat(buf,pbuf);
					}
				}
				}
			}
			guardian=gch;
			if (!found) {
				strcat(buf,"nothing.");
			}
			afound = TRUE;
		}
	}
	if(afound && guardian!=NULL)
		do_say(guardian,buf);
	return;
	}

	guardian=NULL;
	if (strstr(argument,"where is") || strstr(argument,"Where is"))
	{
	if (ch->cabal == 0 && !IS_IMMORTAL(ch))
		return;
	for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
	{
		if (isCabalGuard(gch)) {
			if (ch->cabal == gch->cabal || IS_IMMORTAL(ch)) {
				guardian=gch;
			}
		}
	}
	if(guardian==NULL)
		return;
	for (obj = object_list; obj != NULL; obj = obj_next)
	{
	obj_next = obj->next;
		if ((obj->pIndexData->vnum) && obj->pIndexData->cabal>0 && obj->pIndexData->cabal==guardian->cabal && obj->carried_by!=NULL)
		{
			sprintf(pbuf,"%s is carried by %s.",obj->short_descr,IS_NPC(obj->carried_by) ? obj->carried_by->short_descr : obj->carried_by->name);
			break;
		}
	}
	if(guardian!=NULL && pbuf!=NULL && obj!=NULL)
		do_say(guardian,pbuf);
	}
}

