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

bool    check_chargeset args( ( CHAR_DATA *ch, CHAR_DATA *victim ));
void    one_hit         args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ));
void set_fighting args(( CHAR_DATA *ch, CHAR_DATA *victim ));
DECLARE_DO_FUN(do_myell);
//bool	is_wielded	args((CHAR_DATA *ch, int weapon, int type));

bool is_wielded(CHAR_DATA *ch,int weapon,int type) {

	OBJ_DATA *wield;
	bool primary = FALSE;
        if (IS_NPC(ch))
		return TRUE;

	wield = get_eq_char(ch,WEAR_WIELD);
	if(IS_IMP(ch) && wield)
	{
                send_to_char("You're an Implementor, you can do anything!\n\r",ch);
                return TRUE;
        }
	if (wield == NULL)
	{
		if (weapon == WEAPON_HAND && (get_eq_char(ch,WEAR_DUAL_WIELD) == NULL))
			return TRUE;
		else
			return FALSE;
	}
	if (wield->value[0] == weapon)
	{
		if (type == WIELD_ONE || type == WIELD_PRIMARY) {
			return TRUE;
		}
		primary = TRUE;
	} else {
		if (type == WIELD_PRIMARY) {
			return FALSE;
		}
	}
	wield = get_eq_char(ch,WEAR_DUAL_WIELD);
	if (wield == NULL)
	{
		return FALSE;
	}
	if (wield->value[0] == weapon)
	{
		if (type == WIELD_TWO && primary) {
			return TRUE;
		}
	} else {
		return FALSE;
	}
	return TRUE;
}

void do_pincer(CHAR_DATA *ch,char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	//bool attempt_dual;
	OBJ_DATA *obj;
//	char buf[MAX_STRING_LENGTH]; discloak
	bool exeresult;
	int exedam;

	one_argument(argument,arg);
	if (get_skill(ch,gsn_pincer) == 0)
	{
		send_to_char("Huh?\n\r",ch);
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

	if (victim == ch)
	{
		send_to_char("Wouldn't that hurt?\n\r",ch);
		return;
	}

	obj = get_eq_char(ch,WEAR_WIELD);

        if (obj == NULL || obj->value[0] != WEAPON_AXE)
        {
        send_to_char("You must be dual wielding axes to do that.\n\r",ch);
        return;
        }

	obj = get_eq_char(ch,WEAR_DUAL_WIELD);

        if (obj == NULL || obj->value[0] != WEAPON_AXE)
        {
        send_to_char("You must be dual wielding axes to do that.\n\r",ch);
        return;
        }

	if ( is_safe( ch, victim ) )
	return;

	if (check_chargeset(ch,victim)) {return;}
/* discloak
	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (victim->fighting == NULL || ch->fighting == NULL))
	{
		sprintf(buf,"Help! %s is hacking me to pieces!",get_descr_form( ch,victim,FALSE));
		do_myell(victim,buf);
	}
*/
	act("You pincer $N between your axes.",ch,0,victim,TO_CHAR);
	act("$n pincers you between $s axes.",ch,0,victim,TO_VICT);
	act("$n pincers $N between $s axes.",ch,0,victim,TO_NOTVICT);

	exedam=GET_DAMROLL(ch)+250;
	exeresult = damage(ch, victim, exedam, gsn_pincer, DAM_SLASH, TRUE );
	exeresult = damage(ch, victim, exedam, gsn_pincer, DAM_SLASH, TRUE );
	check_improve(ch,gsn_pincer,TRUE,2);

	WAIT_STATE(ch,3*PULSE_VIOLENCE);
	WAIT_STATE(victim,2*PULSE_VIOLENCE);

	return;
}

/* STRIP -- Graelik */

void strip( CHAR_DATA *ch, CHAR_DATA *victim )
{
    OBJ_DATA *obj;
    OBJ_DATA *secondary;

    if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
        return;

    if (is_affected(victim,gsn_spiderhands) )
    {
        act("Your try to strip $N of his weapon, but $s grip is too tight.",
            ch,NULL,victim,TO_CHAR);
        act("$n tries to strip you of your weapon, but your grip is too tight.",
            ch,NULL,victim,TO_VICT);
        act("$n tries to strip $N of his weapon, but $s grip is too tight.",
            ch,NULL,victim,TO_NOTVICT);
        return;
    }

    if ( IS_OBJ_STAT(obj,ITEM_NOREMOVE))
    {
        act("You try to strip $N of $s weapon but it won't budge!",
                ch,NULL,victim,TO_CHAR);
        act("$n tries to strip you of your weapon, but your weapon won't budge!",
            ch,NULL,victim,TO_VICT);
        act("$n tries to strip $N of $s weapon, but $s weapon won't budge.",
                ch,NULL,victim,TO_NOTVICT);
        return;
    }

    act( "$n strips your weapon from your hands and sends it flying!",
         ch, NULL, victim, TO_VICT    );
    act( "You strip $N's weapon from $s hands and sends it flying!",
           ch, NULL, victim, TO_CHAR    );
    act( "$n strips $N's weapon from $s hands and sends it flying!",
           ch, NULL, victim, TO_NOTVICT );


        if (get_skill(ch,gsn_strip) >= 95 && number_percent() > 60)
        {
    act( "$n snatches your weapon in midflight!",
         ch, NULL, victim, TO_VICT    );
    act( "You snatch $N's weapon in midflight!",
           ch, NULL, victim, TO_CHAR    );
    act( "$n snatches $N's weapon in midflight!",
           ch, NULL, victim, TO_NOTVICT );
        }
        {
          obj_from_char( obj );
          obj_to_char( obj, ch );
        }

    if (get_skill(ch,gsn_strip) < 95)
    {
    obj_from_char( obj );
    {
        obj_to_room( obj, victim->in_room );
        if (IS_NPC(victim) && victim->wait == 0 && can_see_obj(victim,obj))
            get_obj(victim,obj,NULL);
    }
    }

reslot_weapon(victim);

    if ((secondary = get_eq_char(victim,WEAR_DUAL_WIELD)) != NULL)
    {
        unequip_char(victim,secondary);
        equip_char(victim,secondary,WEAR_WIELD);
    }
    return;
}

void do_strip( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *wield;
    OBJ_DATA *obj;
    int chance,ch_weapon,vict_weapon,ch_vict_weapon;

    one_argument (argument, arg );


    if ((chance = get_skill(ch,gsn_strip)) == 0)
    {
        send_to_char( "Huh?\n\r", ch );
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

    wield = get_eq_char( ch, WEAR_WIELD );
    if (wield == NULL || wield->value[0] != WEAPON_WHIP)
        {
        send_to_char("You must be wielding a whip to do that.\n\r",ch);
        return;
        }

    if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
    {
        send_to_char( "Your opponent is not wielding a weapon.\n\r", ch );
        return;
    }

    ch_weapon = get_weapon_skill(ch,get_weapon_sn(ch));
    vict_weapon = get_weapon_skill(victim,get_weapon_sn(victim));
    ch_vict_weapon = get_weapon_skill(ch,get_weapon_sn(victim));

    chance = chance * ch_weapon/100;

    chance += (ch_vict_weapon/2 - vict_weapon) / 2;

    chance += get_curr_stat(ch,STAT_DEX);
    chance -= 1.6 * get_curr_stat(victim,STAT_STR);

    chance += (ch->drain_level + ch->level - victim->drain_level - victim->level) * 2;

    if (number_percent() < chance)
    {
        WAIT_STATE( ch,2*PULSE_VIOLENCE);
        strip( ch, victim );
        check_improve(ch,gsn_strip,TRUE,1);
    }
    else
    {
        WAIT_STATE(ch,2*PULSE_VIOLENCE);
        act("You fail to strip $N of $s weapon.",
        ch,NULL,victim,TO_CHAR);
        act("$n tries to strip you of your weapon, but fails.",
        ch,NULL,victim,TO_VICT);
        act("$n tries to strip $N of $s weapon, but fails.",
        ch,NULL,victim,TO_NOTVICT);
        check_improve(ch,gsn_strip,FALSE,1);
    }
        if (IS_AWAKE(victim) && !IS_NPC(victim) && (victim->fighting != ch))
        {
        sprintf(buf,"%s is trying to strip me of my weapon!",PERS(ch,victim) );
        do_myell( victim, buf );
        multi_hit( victim, ch, TYPE_UNDEFINED );
        }
        else if (!IS_AWAKE(victim) && !IS_NPC(victim))
        {
        sprintf(buf,"%s is trying to strip me of my weapon!",PERS(ch,victim) );
        do_myell( victim, buf );
        }
        else if ( !IS_NPC(ch))
        {
        if ( IS_NPC(victim) && IS_AWAKE(victim))
        {
        multi_hit( victim, ch, TYPE_UNDEFINED );
        }
}
    check_killer(ch,victim);
    return;
}

void do_choke( CHAR_DATA *ch,char *argument)
{
	AFFECT_DATA af;
	CHAR_DATA *victim;
	int chance;
	char arg[MAX_STRING_LENGTH];
	OBJ_DATA *wield;

	if ((chance = get_skill(ch,gsn_choke)) == 0 )
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}
	one_argument(argument,arg);

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
		if (victim->position == POS_SLEEPING)
		{
			send_to_char("But that person is already asleep!\n\r",ch);
			return;
		}
	}

	if (victim == ch)
	{
		send_to_char("You can't choke yourself.\n\r",ch);
		return;
	}

	wield = get_eq_char(ch,WEAR_WIELD);

	if (wield == NULL || wield->value[0] != WEAPON_WHIP)
	{
		send_to_char("You must be wielding a whip to do that.\n\r",ch);
		return;
	}

	if (is_safe(ch,victim))
	return;

	if (is_affected(victim,gsn_choke))
	{
		send_to_char("They are already choked.\n\r",ch);
		return;
	}

	chance += (ch->drain_level - victim->level);
	if (is_affected(victim,skill_lookup("haste")))
		chance -= 10;
	if (is_affected(ch,skill_lookup("haste")))
		chance += 10;

	chance -= 20;

	if (!(str_cmp(race_table[victim->race].name,"lich")))
	{
		chance = 0;
	}

	if (ch->level == MAX_LEVEL) {
		chance = 100;
	}

	act("You lash out $p and wrap it around $N's neck!",ch,wield,victim,TO_CHAR);
	act("$n lashes out $p and wraps it around $N's neck!",ch,wield,victim,TO_NOTVICT);
	act("$n lashes out $p and wraps it around your neck!",ch,wield,victim,TO_VICT);

	if (number_percent() < chance)
	{
		act("You pass out from the choking hold of $p.",victim,wield,0,TO_CHAR);
		act("$n passes out from the choking hold of $p.",victim,wield,0,TO_ROOM);
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_SKILL;
		af.level = ch->level;
		af.location = 0;
		af.type = gsn_choke;
		af.duration = 2;
		af.bitvector = AFF_SLEEP;
		af.modifier = 0;
		affect_to_char(victim,&af);
		stop_fighting(victim, TRUE);
		victim->position = POS_SLEEPING;
		check_improve(ch,gsn_choke,TRUE,2);
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
		return;
	} else {
		damage(ch, victim, 0, gsn_choke, DAM_NONE,TRUE);
		check_improve(ch,gsn_choke,FALSE,3);
		WAIT_STATE(ch, 3*PULSE_VIOLENCE);
		WAIT_STATE(victim, PULSE_VIOLENCE);
	}
	return;
}

void do_overhead(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;

        if (get_skill(ch,gsn_overhead) == 0)
        {
                send_to_char("Huh?\n\r",ch);
                return;
        }
        if ((victim = ch->fighting) == NULL) {
                send_to_char("You need to be fighting someone to overhead them.\n\r",ch);
                return;
        }
        if (victim == ch)
        {
                send_to_char("Wouldn't that hurt?\n\r",ch);
                return;
        }
        if (!is_wielded(ch,WEAPON_AXE,WIELD_ONE)) {
        send_to_char("You must be wielding an axe to do that.\n\r",ch);
        return;
        }
        if ( is_safe( ch, victim ) )
        return;
        act("You attempt to bring your axe down in a wild overhead swing!",ch,0,victim,TO_CHAR);
        act("$n attempts to bring $s axe down in a wild overhead swing!",ch,0,0,TO_ROOM);
        if(number_percent()>(get_skill(ch,gsn_overhead) - 10))
        {
                damage( ch, victim, 0, gsn_overhead,DAM_NONE,TRUE);
                check_improve(ch,gsn_overhead,FALSE,2);
                WAIT_STATE(ch,PULSE_VIOLENCE*2);
                return;
        }
        check_improve(ch,gsn_overhead,TRUE,2);
        damage( ch, victim, ch->level*get_skill(ch,gsn_overhead)*get_skill(ch,gsn_axe)/1000, gsn_overhead,DAM_NONE,TRUE);
        WAIT_STATE(ch,4*PULSE_VIOLENCE);
        return;
}

void do_bearcharge( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;
    char buf[MAX_STRING_LENGTH];

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_bearcharge)) == 0
    ||   (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BASH) && ch->pIndexData->vnum!=MOB_VNUM_TEMPLAR)
    ||   (!IS_NPC(ch)
    &&    ch->level < skill_table[gsn_bearcharge].skill_level[ch->class]))
    {
        send_to_char("You dont know how to Bear Charge.\n\r",ch);
        return;
    }

    if (IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM))
	return;

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
        send_to_char("You cannot charge yourself.\n\r",ch);
        return;
    }

    if (is_safe(ch,victim))
        return;

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
        act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (is_affected(victim,gsn_protective_shield))
    {
	act("$N's charge seems to slide around you.",victim,0,ch,TO_CHAR);
	act("$N's charge seems to slide around $n.",victim,0,ch,TO_NOTVICT);
	act("Your charge seems to slide around $n.",victim,0,ch,TO_VICT);
	WAIT_STATE(ch,PULSE_VIOLENCE*.5);
	return;
    }
    if (is_affected(victim,gsn_wraithform))
    {
	act("$N's tries to charge you but stumbles through thin air.",victim,0,ch,TO_CHAR);
	act("$N's tries to charge $n but stumbles through thin air.",victim,0,ch,TO_NOTVICT);
	act("You try to charge $n but stumble through thin air.",victim,0,ch,TO_VICT);
	WAIT_STATE(ch,PULSE_VIOLENCE*1);
	return;
    }

    if(is_affected(ch,gsn_shadowstrike)) {
	send_to_char("The shadowy arms restrict you.\n\r",ch);
	act("$N's tries to bash, but is restrained by shadowy arms!",ch,NULL,NULL,TO_ROOM);
	WAIT_STATE(ch,20);
	return;
    }

    /* modifiers */

    /* size  and weight */
    chance += ch->carry_weight / 250;
    chance -= victim->carry_weight / 200;

    if (ch->size < victim->size)
        chance += (ch->size - victim->size) * 20;
    else
        chance += (ch->size - victim->size) * 10;


    /* stats */
    chance += get_curr_stat(ch,STAT_STR);
    chance -= (get_curr_stat(victim,STAT_DEX) * 4)/3;
    chance -= GET_AC(victim,AC_BASH) /25;
    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || is_affected(ch,skill_lookup("haste")))
        chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || is_affected(victim,skill_lookup("haste")))
        chance -= 40;

    /* level */
    chance += (ch->drain_level + ch->level - victim->level - victim->drain_level);
	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (victim->fighting == NULL || ch->fighting == NULL))
	{
	sprintf(buf,"Help! %s is charging me like a bear!",PERS(ch,victim));
	do_myell(victim,buf);
	}


    if (!IS_NPC(victim)
        && chance < get_skill(victim,gsn_dodge) )
    {   /*
        act("$n tries to charge you, but you dodge it.",ch,NULL,victim,TO_VICT);
        act("$N dodges your charge, you fall flat on your face.",ch,NULL,victim,TO_CHAR);
        WAIT_STATE(ch,PULSE_VIOLENCE*1.5);
        return;*/
        chance -= 3 * (get_skill(victim,gsn_dodge) - chance);
    }

    /* now the attack */
    if (number_percent() < chance )
    {

        act("$n sends you to the ground with a powerful bear charge!",
                ch,NULL,victim,TO_VICT);
        act("You slam into $N, and send $M flying!",ch,NULL,victim,TO_CHAR);
        act("$n sends $N sprawling with a powerful bear charge.",
                ch,NULL,victim,TO_NOTVICT);
        check_improve(ch,gsn_bearcharge,TRUE,1);
        WAIT_STATE(victim, 2 * PULSE_VIOLENCE);
        damage_old(ch,victim,number_range(2,2 + 2 * ch->size + chance/20),gsn_bearcharge,
       DAM_BASH,TRUE);

        WAIT_STATE(ch,skill_table[gsn_bearcharge].beats);
        victim->position = POS_RESTING;
	if (!IS_NPC(ch) && (ch->class == CLASS_WARRIOR)
	&& (number_percent() < ch->level))
		ch->pcdata->sp++;
    }
    else
    {
        damage_old(ch,victim,0,gsn_bearcharge,DAM_BASH,FALSE);
        act("You tumble to the ground!",
            ch,NULL,victim,TO_CHAR);
        act("$n tumbles to the ground.",
            ch,NULL,victim,TO_NOTVICT);
        act("You evade $n's charge, causing $m to tumble to the ground.",
            ch,NULL,victim,TO_VICT);
        check_improve(ch,gsn_bearcharge,FALSE,1);

        ch->position = POS_RESTING;
        WAIT_STATE(ch,skill_table[gsn_bearcharge].beats);
    }
        check_killer(ch,victim);
}

void do_shoot( CHAR_DATA *ch, char *argument )
{
    //in order of input
    char accuracy[MAX_INPUT_LENGTH];
    char direction[MAX_INPUT_LENGTH];
    char range[MAX_INPUT_LENGTH];
    char target[MAX_INPUT_LENGTH];
    char *next_arg;
    int i;
    int dir;
    int lag;
   // int dam;
    int distance;
    int chance;
   // int num_targets;
   // int target_num;
    CHAR_DATA* victim;
  //  CHAR_DATA *vch;
  //  CHAR_DATA *vch_next;
    bool targeted;
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;
    ROOM_INDEX_DATA *original_room;

	victim = get_char_room(ch, target);

    /*  this will be implemented later
    OBJ_DATA *arrow;
    OBJ_DATA *bow;
    arrow = get_eq_char(ch, WEAR_NOCK);
    bow = get_eq_char(ch, WEAR_WIELD);
    if (arrow == NULL)
    {
	send_to_char("You need to nock an arrow before you can shoot!\n\r,ch);
	return;
    }
    if (bow == NULL || bow->value[0] != WEAPON_BOW)
    {
	send_to_char("You need to use a bow to shoot someone!\n\r",ch);
	return;
    }
    dam = dice(arrow->value[1],arrow->value[2]) * arrow->value[3];
    */
    if (get_skill(ch,gsn_archery) < 2)
    {
	send_to_char("Shooting... whats that?\n\r",ch);
	return;
    }
    if (argument[0] == '\0')
    {
	send_to_char("Syntax: shoot <speed> <direction> <range> <target>.\n\r",ch);
	return;
    }
    next_arg = one_argument(argument, accuracy);
    next_arg = one_argument(next_arg, direction);
    next_arg = one_argument(next_arg, range);
    one_argument(next_arg, target);

    chance = get_skill(ch,gsn_archery);
    //check for proper accuracy
    if (!str_cmp(accuracy,"q") || !str_cmp(accuracy,"quick"))
    {
	chance /= 2;
	lag = PULSE_VIOLENCE;
    }
    else if (!str_cmp(accuracy,"n") || !str_cmp(accuracy,"normal"))
    {
	chance *= .75;
	lag = 1.5 * PULSE_VIOLENCE;
    }
    else if (!str_cmp(accuracy,"a") || !str_cmp(accuracy,"aimed"))
    {
	chance *= .9;
	lag = 2.5 * PULSE_VIOLENCE;
    }
    else
    {
	send_to_char("Aiming style must be specified as quick, normal, or aimed.\n\r",ch);
	return;
    }

    //check for distance
    if (!str_cmp(range,"s") || !str_cmp(range,"short"))
	distance = 1;
    else if (!str_cmp(range,"m") || !str_cmp(range,"medium"))
	distance = 2;
    else if (!str_cmp(range,"l") || !str_cmp(range,"long"))
	distance = 3;
    else
    {
	send_to_char("distance must be specified as short, medium, or long.\n\r",ch);
	return;
    }

    //check for direction
    if (!str_cmp(direction,"n") || !str_cmp(direction,"north"))
	dir = 0;
    else if (!str_cmp(direction,"e") || !str_cmp(direction,"east"))
	dir = 1;
    else if (!str_cmp(direction,"s") || !str_cmp(direction,"south"))
	dir = 2;
    else if (!str_cmp(direction,"w") || !str_cmp(direction,"west"))
	dir = 3;
    else if (!str_cmp(direction,"u") || !str_cmp(direction,"up"))
	dir = 4;
    else if (!str_cmp(direction,"d") || !str_cmp(direction,"down"))
	dir = 5;
    else
    {
	send_to_char("What direction would that be?\n\r",ch);
	return;
    }
    targeted = TRUE;
    //check target, targets can be players or none
   /* if (!str_cmp(target, "n") || !str_cmp(target,"none"))
	targeted = false;*/

    to_room = ch->in_room;
    pexit = ch->in_room->exit[dir];
    for (i = 0; i < distance; i++)
    {
	send_to_char("debug-> cycle.\n\r",ch);
	pexit = to_room->exit[dir];
	if (IS_SET(pexit->exit_info,EX_CLOSED))
	{
            send_to_char("You cannot shoot through a door.\n\r",ch);
            return;
	}

	if ( ( pexit   = ch->in_room->exit[dir] ) == NULL
	    ||   ( to_room = pexit->u1.to_room   ) == NULL
	    ||	 !can_see_room(ch,pexit->u1.to_room))
        {
	    send_to_char( "There is nothing at that range.\n\r", ch );
            return;
	}
    }


    if (targeted)
    {
	original_room = ch->in_room;
        ch->in_room = to_room;
	victim = get_char_room(ch, target);
	ch->in_room = original_room;
	if (victim == NULL)
	{
	    send_to_char("You can't seem to locate your target.\n\r",ch);
	    return;
	}
    }
    /*else //not targeted
    {



    }*/

    act("$n draws $s bow and shoots it!",ch,NULL,NULL,TO_ROOM);
    act("You draw your bow and shoot your arrow!",ch,NULL,NULL,TO_CHAR);
    WAIT_STATE(ch,lag);

    if (number_percent() < chance) // A HIT
    {
	//once arrows and bows are made this will be better
        act("An arrow drops out of the sky and strikes $n!",victim,NULL,NULL,TO_ROOM);
	act("An arrow drops out of the sky and strikes you!",victim,NULL,NULL,TO_CHAR);
        act("Off in the distance you see your arrow strike its target!",ch,NULL,NULL,TO_CHAR);
	damage_old(ch,victim,number_range(25,100),gsn_archery,DAM_PIERCE,TRUE);
	return;
    }
    //if we get here then they misssed the target
    //later this will be filled with code for hitting a random target
    //instead but for now make do with this

    act("An arrow drops out of the sky and falls harmlessly to the ground.",victim,NULL,NULL,TO_ROOM);
    act("An arrow drops out of the sky and falls harmlessly to the ground.",victim,NULL,NULL,TO_CHAR);
    act("Your arrow misses its target.",ch,NULL,NULL,TO_CHAR);
    return;
}

void do_whirl( CHAR_DATA *ch, char *argument)
{
        AFFECT_DATA af;
        CHAR_DATA *victim;
        char arg[MAX_INPUT_LENGTH];
        int chance;
	bool exeresult;
	int exedam;
        one_argument(argument,arg);
        if ((chance = get_skill(ch,gsn_whirl) ) == 0
        || (ch->level < skill_table[gsn_whirl].skill_level[ch->class]
        && !IS_NPC(ch)))
        {
        send_to_char("Huh??\n\r",ch);
        return;
        }
        if ( ( victim = ch->fighting ) == NULL )
        {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
        }
	if (!is_wielded(ch,WEAPON_AXE,WIELD_ONE))
        {
        send_to_char("You must be wielding an axe to whirl.\n\r",ch);
        return;
        }
        if (is_safe(ch,victim) )
        return;
        chance += ch->carry_weight/25;
        chance -= victim->carry_weight/20;
        chance += (ch->size - victim->size)*20;
        chance -= get_curr_stat(victim,STAT_DEX);
        chance += get_curr_stat(ch,STAT_STR)/3;
        chance += get_curr_stat(ch,STAT_DEX)/2;
        if (IS_AFFECTED(ch,AFF_HASTE) )
                chance += 10;
        if (IS_AFFECTED(victim,AFF_HASTE) )
                chance -= 15;
        act("$n whirls $s axe in an attempt to strike $N with its points!",ch,0,victim,TO_NOTVICT);
        act("You whirl your axe in an attempt to strike $N with its points!",ch,0,victim,TO_CHAR);
        act("$n whirls $s axe in an attempt to strike you with its points!",ch,0,victim,TO_VICT);
        if (number_percent() < chance && !is_affected(victim,gsn_whirl))
        {
	exedam=GET_DAMROLL(ch)+200;
	exeresult = damage(ch, victim, exedam, gsn_whirl, DAM_SLASH, TRUE );
	init_affect(&af);
	af.aftype = AFT_SKILL;
	af.level = ch->level;
        af.type = gsn_whirl;
        af.modifier = -(ch->level/10);
        af.duration = 5;
        af.location = APPLY_DEX;
        af.bitvector = 0;
        affect_to_char(victim,&af);
        af.location = APPLY_STR;
        af.bitvector = 0;
        affect_to_char(victim,&af);
        check_improve(ch,gsn_whirl,TRUE,1);
        }
        else if (number_percent() <chance && is_affected(victim,gsn_whirl))
        {
	exedam=GET_DAMROLL(ch)+200;
	exeresult = damage(ch, victim, exedam, gsn_pincer, DAM_SLASH, TRUE );
        check_improve(ch,gsn_whirl,TRUE,1);
        }
        else
        {
	exedam=GET_DAMROLL(ch)+200;
	exeresult = damage(ch, victim, exedam, gsn_pincer, DAM_SLASH, TRUE );
        check_improve(ch,gsn_whirl,FALSE,1);
        }
        WAIT_STATE(ch,2*PULSE_VIOLENCE);
        return;
}

void do_crushing_blow(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim = ch->fighting;
	int dam, chance;

	dam=375;

	if (get_skill(ch,gsn_crushing_blow) == 0) {
		send_to_char("You don't know how to land crushing blows!\n\r",ch);
		return;
	}
	if (victim == NULL) {
		send_to_char("You must be in combat to land a crushing blow!\n\r",ch);
		return;
	}
	if (!is_wielded(ch,WEAPON_HAND,WIELD_ONE)) {
		send_to_char("You need your hands free to be able to land a crushing blow!\n\r",ch);
		return;
	}
	chance = get_skill(ch,gsn_crushing_blow);
	chance -= (get_curr_stat(victim,STAT_DEX) + get_curr_stat(victim,STAT_STR))*2 - (get_curr_stat(ch,STAT_STR) + get_curr_stat(ch,STAT_DEX))*2;

	if (number_percent() > chance) {
		act("$n laces $s fingers together to jab at $N but misses.",ch,0,victim,TO_NOTVICT);
		act("$n laces $s fingers together to jab at you but misses.",ch,0,victim,TO_VICT);
		act("You lace your fingers together to jab at $N but miss.",ch,0,victim,TO_CHAR);
		damage_old(ch,victim,0,gsn_crushing_blow,DAM_BASH,TRUE);
		check_improve(ch,gsn_crushing_blow,FALSE,2);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
	} else {
		act("$n laces $s fingers together and hammers $s fists at $N!",ch,0,victim,TO_NOTVICT);
		act("$n laces $s fingers together and hammers $s fists at you!",ch,0,victim,TO_VICT);
		act("You lace your fingers together and hammer your fists at $N!",ch,0,victim,TO_CHAR);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);

		if (number_percent() > 60 )
		{
			act("$n crushing blow knocks $N down!",ch,0,victim,TO_NOTVICT);
			act("Your crushing blow knocks $N down!",ch,0,victim,TO_CHAR);
			act("$n crushing blow knocks you down!",ch,0,victim,TO_VICT);
			WAIT_STATE(victim,PULSE_VIOLENCE*2);
			WAIT_STATE(ch,PULSE_VIOLENCE*3);
		}
		damage_old(ch,victim,dam,gsn_crushing_blow,DAM_BASH,TRUE);
		check_improve(ch,gsn_crushing_blow,TRUE,1);
	}
	return;
}

void do_findwood(CHAR_DATA *ch,char *argument)
{
        OBJ_DATA *maple;
        OBJ_DATA *magic;
        OBJ_DATA *oak;
        char arg[MAX_STRING_LENGTH];
        int chance;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_fwood)) == 0
    || (ch->level < skill_table[gsn_fwood].skill_level[ch->class]) )
    {
        send_to_char("Huh?\n\r",ch);
		return;
    }

    if (ch->in_room->sector_type != SECT_FOREST)
    {
    send_to_char("Does it look like theres wood here?\n\r",ch);
    return;
    }

    if (arg[0] == '\0')
    {
		send_to_char("You can find: Maple, Magic or Oak.\n\r",ch);
		return;
	}

    if (chance > 90)
        chance = 90;

    if (!str_cmp(arg,"maple") && (number_percent() < chance))
    {
    	act("$n makes some noise and comes up with a large log.",ch,0,0,TO_ROOM);
    	send_to_char("You search about and find yourself a large log.\n\r",ch);
    	check_improve(ch,gsn_fwood,TRUE,2);
		maple = create_object(get_obj_index(OBJ_VNUM_WOOD_MAPLE),1);
		obj_to_char(maple,ch);
		return;
	}
    else if (!str_cmp(arg,"magic") && (number_percent() < chance))
	{
	    act("$n makes some noise and comes up with a large log.",ch,0,0,TO_ROOM);
	    send_to_char("You search about and find yourself a large log.\n\r",ch);
	    check_improve(ch,gsn_fwood,TRUE,2);
		magic = create_object(get_obj_index(OBJ_VNUM_WOOD_MAGIC),1);
		obj_to_char(magic,ch);
		return;
	}
	else if (!str_cmp(arg,"oak") && (number_percent() < chance))
	{
	    act("$n makes some noise and comes up with a large log.",ch,0,0,TO_ROOM);
	    send_to_char("You search about and find yourself a large log.\n\r",ch);
	    check_improve(ch,gsn_fwood,TRUE,2);
		oak = create_object(get_obj_index(OBJ_VNUM_WOOD_OAK),1);
		obj_to_char(oak,ch);
		return;
	}
	else
	{
		send_to_char("You search about but fail to find that.\n\r",ch);
		return;
	}
}


void do_carve(CHAR_DATA *ch,char *argument)
{
        OBJ_DATA *maple_staff;
        OBJ_DATA *oak_staff;
        OBJ_DATA *magic_staff;
        OBJ_DATA *wood;
        char arg[MAX_STRING_LENGTH];
		int chance, number, dice;

	  one_argument(argument,arg);

  	if ( (chance = get_skill(ch,gsn_carve)) == 0
  	|| (ch->level < skill_table[gsn_carve].skill_level[ch->class]) )
  	{
			send_to_char("Huh?\n\r",ch);
			return;
	}

	if (arg[0] == '\0')
	{
			send_to_char("You must carve either a chunk of maple, oak or magic wood.\n\r",ch);
			return;
	}

	if (chance > 90)
	chance = 90;

	wood =  get_obj_list(ch,argument,ch->carrying);

	if (wood == NULL)
	{
		send_to_char("You aren't carrying that.\n\r",ch);
		return;
	}

	if (number_percent() > chance)
	{
		send_to_char("You fail and destroy the wood!\n\r",ch);
		obj_from_char(wood);
		return;
	}

	if (!str_cmp(arg,"maple"))
	{
	   	if (wood->pIndexData->vnum != OBJ_VNUM_WOOD_MAPLE)
	   	{
			send_to_char("You can't carve that!\n\r",ch);
			return;
		}
	   	act("$n carves a beautiful staff out of wood.",ch,0,0,TO_ROOM);
	   	send_to_char("You make yourself a beautiful staff out of wood.\n\r",ch);
	   	check_improve(ch,gsn_fwood,TRUE,2);
		maple_staff = create_object(get_obj_index(OBJ_VNUM_RANGER_MAPLE),1);
		number = 6;     dice = 9;
		maple_staff->level = ch->level;
		maple_staff->value[1] = number;
        maple_staff->value[2] = dice;
		obj_to_char(maple_staff,ch);
		obj_from_char(wood);
		return;
	}
	else if (!str_cmp(arg,"magic"))
	{
	   	if (wood->pIndexData->vnum != OBJ_VNUM_WOOD_MAGIC)
	   	{
			send_to_char("You can't carve that!\n\r",ch);
			return;
		}
	   	act("$n carves a beautiful staff out of wood.",ch,0,0,TO_ROOM);
	   	send_to_char("You make yourself a beautiful staff out of wood.\n\r",ch);
	   	check_improve(ch,gsn_fwood,TRUE,2);
		magic_staff = create_object(get_obj_index(OBJ_VNUM_RANGER_MAGIC),1);
		number = 8;     dice = 7;
		magic_staff->level = ch->level;
		magic_staff->value[1] = number;
	    magic_staff->value[2] = dice;
		obj_to_char(magic_staff,ch);
		obj_from_char(wood);
		return;
	}
	else if (!str_cmp(arg,"oak"))
	{
	   	if (wood->pIndexData->vnum != OBJ_VNUM_WOOD_OAK)
	   	{
			send_to_char("You can't carve that!\n\r",ch);
			return;
		}
	   	act("$n carves a beautiful staff out of wood.",ch,0,0,TO_ROOM);
	   	send_to_char("You make yourself a beautiful staff out of wood.\n\r",ch);
	   	check_improve(ch,gsn_fwood,TRUE,2);
		oak_staff = create_object(get_obj_index(OBJ_VNUM_RANGER_OAK),1);
		number = 8;     dice = 7;
		oak_staff->level = ch->level;
		oak_staff->value[1] = number;
	    oak_staff->value[2] = dice;
		obj_to_char(oak_staff,ch);
		obj_from_char(wood);
		return;
	}
	else
	{
		send_to_char("You must either choose: oak, maple or magic.\n\r",ch);
		return;
	}
}

void do_vermin(CHAR_DATA *ch,char *argument) // New AP skill
{
	AFFECT_DATA af;
	CHAR_DATA *victim;
	CHAR_DATA *rat;
	char arg[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int i, chance;

	one_argument(argument,arg);

	if ( (chance = get_skill(ch,gsn_vermin)) == 0
	|| (ch->level < skill_table[gsn_vermin].skill_level[ch->class]) )
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

	if (IS_NPC(ch))
	return;

	if (is_affected(ch, gsn_vermin))
	{
		send_to_char("You don't feel up to calling another vermin swarm yet.\n\r",ch);
		return;
	}

	if (arg[0] == '\0')
	{
		send_to_char("You can't set loose a vermin untargeted.\n\r",ch);
		return;
	}
	else if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    if (is_safe(ch,victim))
    return;

	if (victim == ch)
	{
		send_to_char("Yeah. Thats smart.\n\r",ch);
		return;
	}

	if (ch->mana < 50)
	{
	    send_to_char("You don't have the mana.\n\r",ch);
	    return;
    }

	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (victim->fighting == NULL || ch->fighting == NULL))
	{
		sprintf(buf,"Ack! %s is setting loose a swarm of vermin!",PERS(ch,victim));
		do_myell(victim,buf);
	}

	for (i=0;i < 5; i++)
	{
		  rat = create_mobile(get_mob_index(MOB_VNUM_APRAT));
		  rat->max_hit = ch->max_hit/4;
		  rat->hit = ch->max_hit/4;
		  rat->damroll = (8 + ch->level/4);
		  rat->hitroll = ch->level/3;
          char_to_room(rat,victim->in_room);
	      rat->last_fought = victim;
	      do_murder(rat,victim->name);
	}
	init_affect(&af);
	af.where     =	TO_AFFECTS;
	af.type      =	gsn_vermin;
	af.aftype    = 	AFT_SKILL;
	af.level     =	ch->level;
	af.duration  = 	10;
	af.location  = 	0;
	af.modifier  = 	0;
	af.bitvector = 	0;
    affect_to_char(ch,&af);
	send_to_char("You scream in horror as vermin are unleashed upon you!\n\r",victim);
	act("$n screams in horror as vermin are unleashed upon them!\n\r",victim,NULL,NULL,TO_ROOM);
	send_to_char("You grin to yourself as you unleash vermin upon them.\n\r", ch);
	check_improve(ch,gsn_vermin,TRUE,3);
	WAIT_STATE(ch,PULSE_VIOLENCE*1);
	return;
}

/*void do_element(CHAR_DATA *ch,char *argument)   Ceial's work in progress
{
	char arg[MAX_STRING_LENGTH];

	one_argument(argument,arg);

	if ((ch->class != CLASS_ELEMENTALIST) || (!IS_IMMORTAL(ch)))
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

	if (ch->pcdata->element != '\0')
	{
		send_to_char("You have already chosen an element to control.",ch);
		return;
	}

	if (!str_cmp(arg,'\0'))
	{
		send_to_char("You can choose to control either:\n\r",ch);
		send_to_char("Air, Earth, Fire or Water.\n\r",ch);
		send_to_char("You can do this once. Choose wisely.",ch);
		return;
	}

	if (!str_cmp(arg, "air"))
	{
		send_to_char("You have chosen to control the air.\n\r",ch);
		ch->pcdata->element = 1;
		ch->pcdata->learned[skill_lookup("earthfade")] = 0;
		ch->pcdata->learned[skill_lookup("summon earth elemental")] = 0;
		ch->pcdata->learned[skill_lookup("summon fire elemental")] = 0;
		ch->pcdata->learned[skill_lookup("summon water elemental")] = 0;
		ch->pcdata->learned[skill_lookup("frost bolt")] = 0;
		ch->pcdata->learned[skill_lookup("ice slick")] = 0;
		return;
	}
	else if (!str_cmp(arg, "earth"))
	{
		send_to_char("You have chosen to control the earth.\n\r",ch);
		ch->pcdata->element = 2;
		return;
	}
	else if (!str_cmp(arg, "fire"))
	{
		send_to_char("You have chosen to control fire.\n\r",ch);
		ch->pcdata->element = 3;
		return;
	}
	else if (!str_cmp(arg, "water"))
	{
		send_to_char("You have chosen to control water.\n\r",ch);
		ch->pcdata->element = 4;
		return;
	}else{
		send_to_char("You can choose to control either:\n\r",ch);
		send_to_char("Air, Earth, Fire or Water.\n\r",ch);
		send_to_char("You can do this once. Choose wisely.",ch);
		return;
	}
}
*/

void do_coerce(CHAR_DATA *ch,char *argument)
{
	AFFECT_DATA af;
	CHAR_DATA *victim;
	CHAR_DATA *check;
	char arg[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int chance, miss, dam, skill;
	bool found = FALSE;

	one_argument(argument,arg);

	if ( (chance = get_skill(ch,gsn_coerce)) == 0
	|| (ch->level < skill_table[gsn_coerce].skill_level[ch->class]) )
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

	for (check = char_list; check != NULL; check = check->next)
	{
	  if  ( (check->master == ch)
	  && check->pIndexData->vnum == MOB_VNUM_SLAYER )
	  found = TRUE;
    }

	if (arg[0] == '\0')
	{
		send_to_char("Coerce who?\n\r",ch);
		return;
	}
	else if ((victim = get_char_room(ch,arg)) == NULL)
	{
	    send_to_char("They aren't here.\n\r",ch);
	    return;
    }

   	if (!found)
   	{
		send_to_char("You need slaves to do that.\n\r",ch);
		return;
	}

   	if (ch->mana < 50)
	{
	    send_to_char("You don't have the mana.\n\r",ch);
	    return;
	}

	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (victim->fighting == NULL || ch->fighting == NULL))
	{
		sprintf(buf,"Help! %s is trying to coerce me!",PERS(ch,victim));
		do_myell(victim,buf);
	}

	dam = dice( 51/2, 7 );
	miss = number_percent();
	skill = get_skill(ch,gsn_coerce);

	if (miss > skill)
	{
		act("You and your slaves close in on $N but stumble.",ch,0,victim,TO_CHAR);
    	act("$n and their slaves close in on $N but stumble.",ch,0,victim,TO_NOTVICT);
    	act("$n and their slaves close in on you but stumble.",ch,0,0,TO_VICT);
    	check_improve(ch,gsn_coerce,FALSE,2);
    	WAIT_STATE(ch,PULSE_VIOLENCE);
    	return;
	}
	else if ((number_percent() < 25)  && (!is_affected(victim,gsn_coerce)))
	{
		act("You and your slaves close in on $N and brutually coerce them!",ch,0,victim,TO_CHAR);
    	act("$n and their slaves close in on $N and brutually coerce them!",ch,0,victim,TO_NOTVICT);
		act("$n and their slaves close in on you and brutually coerce you!",ch,0,0,TO_VICT);
		damage( ch, victim, dam, gsn_coerce, DAM_MENTAL,TRUE);
		if (!is_affected(victim,gsn_coerce))
		{
			init_affect(&af);
			af.where     = TO_AFFECTS;
			af.aftype    = AFT_POWER;
			af.location  = APPLY_SAVES;
			af.type      = gsn_coerce;
			af.level     = ch->level;
			af.duration  = 5;
			af.modifier  = 10;
			af.bitvector = 0;
			affect_to_char( victim, &af );
			send_to_char("Your feel your self confidence lessen!\n\r",victim);
			send_to_char("They look less confident.\n\r",ch);
		}
		check_improve(ch,gsn_coerce,TRUE,1);
		WAIT_STATE(victim, 2 * PULSE_VIOLENCE);
		WAIT_STATE(ch,PULSE_VIOLENCE);
		return;
	}else{
		act("You and your slaves close in on $N and brutually coerce them!",ch,0,victim,TO_CHAR);
		act("$n and their slaves close in on $N and brutually coerce them!",ch,0,victim,TO_NOTVICT);
		act("$n and their slaves close in on you and brutually coerce you!",ch,0,0,TO_VICT);
		damage( ch, victim, dam, gsn_coerce,DAM_NONE,TRUE);
		if (!is_affected(victim,gsn_coerce))
		{
			send_to_char("You fail to affect their self confidence.\n\r",ch);
		}
		check_improve(ch,gsn_coerce,TRUE,1);
		WAIT_STATE(ch,PULSE_VIOLENCE);
		return;
	}
}

void do_psionic_blast(CHAR_DATA *ch,char *argument)
{
	AFFECT_DATA af;
	CHAR_DATA *victim;
	char arg[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int chance, dam;


	one_argument(argument,arg);

	if ((chance = get_skill(ch,gsn_psi_blast)) == 0)
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

	if (is_affected(ch, gsn_psi_blast))
	{
		send_to_char("You aren't ready to pummel minds again.\n\r",ch);
		return;
	}

	if ((arg[0] == '\0' && ch->position != POS_FIGHTING))
	{
		send_to_char("Pummel who's mind?\n\r",ch);
		return;
	}
	else if ((victim = get_char_room(ch,arg)) == NULL)
	{
	    send_to_char("They aren't here.\n\r",ch);
	    return;
    }

   	if (ch->mana < 50)
	{
	    send_to_char("You don't have the mana.\n\r",ch);
	    return;
	}

	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (victim->fighting == NULL || ch->fighting == NULL))
	{
		sprintf(buf,"Help! %s is blasting my mind!",PERS(ch,victim));
		do_myell(victim,buf);
	}

	dam = dice(ch->level,5);

	if ( saves_spell( ch->level, victim, DAM_MENTAL ) )
	{
		dam /= 2;
		send_to_char("You feel your mind being pummeled viciously.\n\r",victim);
		act("$n reels, but isn't over come by the blast.",victim,NULL,NULL,TO_ROOM);
    	damage_old( ch, victim, dam, gsn_psi_blast,DAM_MENTAL,TRUE);
    	WAIT_STATE(ch,PULSE_VIOLENCE*1);
	}
	else
	{

		if ( saves_spell( ch->level, victim, DAM_MENTAL ) )
		{
			if (!is_affected(victim,skill_lookup("blindness")))
			{
				init_affect(&af);
				af.where     = TO_AFFECTS;
				af.aftype	 = get_spell_aftype(ch);
				af.type      = gsn_psi_blast;
				af.level     = ch->level;
				af.location  = APPLY_HITROLL;
				af.modifier  = -4;
				af.duration  = 8;
				af.bitvector = AFF_BLIND;
				affect_to_char( victim, &af );
				send_to_char( "You are blinded!\n\r", victim );
    			act("$n appears to be blinded.",victim,NULL,NULL,TO_ROOM);
			}
		}

		if ( saves_spell( ch->level, victim, DAM_MENTAL ) )
		{
			if (!is_affected(victim,skill_lookup("weaken")))
			{
				init_affect(&af);
				af.where     = TO_AFFECTS;
				af.aftype	 = get_spell_aftype(ch);
				af.type      = gsn_psi_blast;
				af.level     = ch->level;
				af.duration  = 8;
				af.location  = APPLY_STR;
				af.modifier  = -10;
				af.bitvector = AFF_WEAKEN;
				affect_to_char( victim, &af );
				send_to_char( "You feel your strength slip away.\n\r", victim );
    			act("$n looks tired and weak.",victim,NULL,NULL,TO_ROOM);
			}
		}

		init_affect(&af);
		af.where     = TO_AFFECTS;
		af.aftype	 = AFT_SKILL;
		af.type      = gsn_psi_blast;
		af.level     = ch->level;
		af.duration  = 8;
		af.location  = 0;
		af.modifier  = 0;
		af.bitvector = 0;
		affect_to_char( ch, &af );

		send_to_char("You reel as your mind is pummeled visiously.\n\r",victim);
		act("$n reels, and is over come by the mental blast.",victim,NULL,NULL,TO_ROOM);
    	damage_old( ch, victim, dam, gsn_psi_blast,DAM_MENTAL,TRUE);
    	WAIT_STATE(victim, 2 * PULSE_VIOLENCE);
		WAIT_STATE(ch,PULSE_VIOLENCE*1);
		return;
	}

	return;
}





