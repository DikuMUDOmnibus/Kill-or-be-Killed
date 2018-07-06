#include "include.h"




void do_battlepaint(CHAR_DATA *ch,char *argument)
{
	AFFECT_DATA af;

	if (cabal_down(ch,CABAL_RAVAGER))
	return;

	if ( (get_skill(ch,gsn_battlepaint) == 0 )
	|| ch->level < skill_table[gsn_battlepaint].skill_level[ch->class])
	{
		send_to_char("Huh?\n\r",ch);
		return;
 	}

 	if (ch->mana < 25)
	{
		send_to_char("You don't have the mana.\n\r",ch);
		return;
	}

	if (is_affected(ch,gsn_battlepaint))
	{
		send_to_char("Your face is already covered with battle paint.\n\r",ch);
		return;
	}
	if (number_percent() > ch->pcdata->learned[gsn_battlepaint])
	{
		send_to_char("You fail to coat your face with battlepaint.\n\r",ch);
		act("$n fumbles with messily with a pouch of paint.",ch,0,0,TO_ROOM);
		ch->mana -= 15;
		check_improve(ch,gsn_battlepaint,FALSE,2);
		return;
    }

	ch->mana -= 25;
	init_affect(&af);
	af.where        = TO_AFFECTS;
	af.type         = gsn_battlepaint;
	af.level        = ch->level;
	af.duration 	= ch->level;
	if (ch->level <= 10)
	{
		af.modifier     = 2;
	}
	else if (ch->level <= 20)
	{
		af.modifier     = 4;
	}
	else if (ch->level <= 30)
	{
		af.modifier     = 6;
	}
	else if (ch->level <= 40)
	{
		af.modifier     = 8;
	}
	else if (ch->level <= 50)
	{
		af.modifier     = 10;
	}
	else
	{
		af.modifier     = 12;
	}
	af.bitvector    = 0;
	af.aftype = AFT_SKILL;
	af.location     = APPLY_HITROLL;
	affect_to_char(ch,&af);
	af.location     = APPLY_DAMROLL;
	affect_to_char(ch,&af);
	af.modifier = ch->level * 2;
	af.location     = APPLY_HIT;
	affect_to_char(ch,&af);
    send_to_char("You paint your face with blue and white battle paint.\n\r", ch );
    act("$n paints $s face with blue and white battle paint.",ch,0,0,TO_ROOM);
    return;
}

void do_resist(CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA af;
    int chance;

    if (cabal_down(ch,CABAL_RAVAGER))
	return;

    if ( (chance = get_skill(ch,gsn_ressist)) == 0
    ||   (ch->level < skill_table[gsn_ressist].skill_level[ch->class]))
    {
		send_to_char("Huh?\n\r",ch);
        return;
    }

    if (is_affected(ch,gsn_ressist))
    {
	send_to_char("You are already resisting.\n\r",ch);
	return;
    }

    if (ch->mana < 50)
    {
	send_to_char("You don't have the mana.\n\r",ch);
	return;
    }

    if (number_percent() > ch->pcdata->learned[gsn_ressist])
    {
	ch->mana -= 25;
	send_to_char("You try to resist, but fail.\n\r",ch);
	return;
    }

	init_affect(&af);
    af.where = TO_AFFECTS;
    af.duration = (ch->level)/10;
    af.location = 0;
    af.modifier = 0;
    af.level = ch->level;
    af.type = gsn_ressist;
	af.aftype = AFT_POWER;
	affect_to_char(ch,&af);
	af.aftype = AFT_POWER;
    af.modifier = -ch->level / 5;
	af.location = APPLY_SAVES;
	affect_to_char(ch,&af);
    send_to_char("You focus on resisting physical and magical attacks.\n\r", ch );
    act("$n's face becomes clouded as $s focuses deeply.",ch,NULL,NULL,TO_ROOM);
    ch->mana -= 50;
    return;
}

void do_claymore(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	OBJ_DATA *claymore;
	int dice_1, dice_2, chance;

	if (cabal_down(ch,CABAL_RAVAGER))
	return;

	if ( (chance = get_skill(ch,gsn_claymore)) == 0
	||   (ch->level < skill_table[gsn_claymore].skill_level[ch->class]))
	{
		send_to_char("Huh?\n\r",ch);
	    return;
	}

	if (is_affected(ch,gsn_claymore))
	{
		send_to_char("You already have forged a claymore.\n\r",ch);
		return;
	}

	if (ch->mana < 100)
	{
		send_to_char("You don't have the mana.\n\r",ch);
		return;
	}

	if (number_percent() > ch->pcdata->learned[gsn_claymore])
	{
		ch->mana -= 50;
		send_to_char("You try to forge a claymore, but fail.\n\r",ch);
		return;
    }

    if (ch->level < 25)
    {
		dice_1 = 5; dice_2 = 7;
	}
	else if (ch->level < 30)
	{
		dice_1 = 7; dice_2 = 6;
	}
	else if (ch->level < 35)
	{
		dice_1 = 5; dice_2 = 9;
	}
	else if (ch->level < 40)
	{
		dice_1 = 5; dice_2 = 10;
	}
	else if (ch->level < 45)
	{
		dice_1 = 8; dice_2 = 6;
	}
	else
	{
		dice_1 = 7; dice_2 = 8;
	}


    claymore = create_object(get_obj_index(OBJ_VNUM_CLAYMORE), ch->level);
	claymore->timer 	= ch->level * 5;
	claymore->value[1] 	= dice_1;
    claymore->value[2] 	= dice_2;
    claymore->level 	= ch->level;


    init_affect(&af);
	af.where 		= TO_OBJECT;
	af.aftype 	    = AFT_SPELL;
	af.type 		= gsn_claymore;
	af.level	    = ch->level;
	af.duration 	= -1;
	af.location 	= APPLY_HITROLL;
	af.modifier 	= ch->level / 4;
	af.bitvector 	= 0;
	affect_to_obj(claymore,&af);
	af.location 	= APPLY_DAMROLL;
	affect_to_obj(claymore,&af);
    obj_to_char(claymore,ch);

	init_affect(&af);
	af.where 		= TO_AFFECTS;
	af.aftype       = AFT_POWER;
	af.type 		= gsn_claymore;
	af.duration 	= 60;
	af.location 	= 0;
	af.bitvector    = 0;
	af.modifier 	= 0;
	af.level 		= ch->level;
	affect_to_char(ch,&af);

    act("$n is blessed by $s clans deity with a beautiful $p!",ch ,claymore,0,TO_ROOM);
    act("You are blessed by your clan deity with a beautiful $p!",ch, claymore,0,TO_CHAR);
    check_improve(ch,gsn_claymore,TRUE,1);
    return;
}

void do_firstaid(CHAR_DATA *ch,char *argument)
{
	CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    AFFECT_DATA af;

    if (cabal_down(ch,CABAL_RAVAGER))
	return;

    one_argument(argument,arg);

	if ( (get_skill(ch,gsn_firstaid) == 0)
	|| (ch->level < skill_table[gsn_firstaid].skill_level[ch->class]) )
    {
	    send_to_char("Huh?\n\r",ch);
        return;
    }

    if (is_affected(ch,gsn_firstaid))
    {
        send_to_char("You can't apply more aid yet.\n\r",ch);
        return;
    }
    if (ch->mana < 50)
    {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
   	}

    if (arg[0] == '\0')
		victim = ch;
        else if ((victim = get_char_room(ch,arg)) == NULL)
        {
        send_to_char("They aren't here.\n\r",ch);
        return;
        }
        if (number_percent() > ch->pcdata->learned[gsn_firstaid])
        {
        act("You fail to apply battle dressing to $N's wounds.",ch,0,victim,TO_CHAR);
        act("$n fumbles with $s bandages but fails to use them effectively.",ch,0,0,TO_ROOM);
        ch->mana -= 25;
        check_improve(ch,gsn_firstaid,FALSE,3);
        return;
        }
        ch->mana -= 50;

        if (victim != ch)
        {
        act("$n applies bandages to $N's battle wounds.",ch,0,victim,TO_NOTVICT);
        act("You apply bandages to $N's battle wounds.",ch,0,victim,TO_CHAR);
        act("$n applies bandages to your battle wounds.",ch,0,victim,TO_VICT);
        }
        else
        {
        act("$n applies bandages to $mself.",ch,0,0,TO_ROOM);
        send_to_char("You apply battle dressing to yourself.\n\r",ch);
        }
        send_to_char("You feel better.\n\r",victim);

        victim->hit = UMIN(victim->hit + (4*ch->level), victim->max_hit);
	if (number_percent() < 60)
	{
	if (IS_AFFECTED(victim,AFF_PLAGUE))
	{
	affect_strip(victim,gsn_plague);
	act("The sores on $n's body vanish.\n\r",victim,0,0,TO_ROOM);
	send_to_char("The sores on your body vanish.\n\r",victim);
	}
	}
	if (number_percent() < 60)
	{
	 if (is_affected(victim, gsn_blindness))
	 {
	  affect_strip(victim, gsn_blindness);
	  send_to_char("Your vision returns!\n\r",victim);
	 }
	}
	if (number_percent() < 60)
	{
	if (is_affected(victim,gsn_poison))
	{
	affect_strip(victim,gsn_poison);
	send_to_char("A warm feeling goes through your body.\n\r",victim);
	act("$n looks better.",victim,0,0,TO_ROOM);
	}
	if (is_affected(victim,gsn_poison_dust))
	{
	affect_strip(victim,gsn_poison_dust);
	send_to_char("The dust in your eyes is cleaned out.\n\r",victim);
	act("The dust in $n's eyes looks cleaned out.",victim,0,0,TO_ROOM);
	}
	}

    check_improve(ch,gsn_firstaid,TRUE,3);
	init_affect(&af);
    af.where = TO_AFFECTS;
    af.type = gsn_bandage;
    af.location = 0;
    af.duration = 5;
    af.modifier = 0;
    af.bitvector = 0;
    af.level = ch->level;
	af.aftype = AFT_SKILL;
    affect_to_char(ch,&af);
    return;
}

void do_swing(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	OBJ_DATA *wield;
	int check_chstr = get_curr_stat(ch,STAT_STR);
	int check_chdex = get_curr_stat(ch,STAT_DEX);
	int max_hit, i, chance, dam;

	if (cabal_down(ch,CABAL_RAVAGER))
	return;

	if ( (chance = get_skill(ch,gsn_swing)) == 0
	||   (ch->level < skill_table[gsn_swing].skill_level[ch->class]))
	{
		send_to_char("Huh?\n\r",ch);
	    return;
	}

	if ((victim = ch->fighting) == NULL)
	{
		send_to_char("You aren't fighting anyone.\n\r",ch);
		return;
	}

	wield = get_eq_char(ch,WEAR_WIELD);

	if ( (wield == NULL)
	&& (get_eq_char(ch,WEAR_DUAL_WIELD) == NULL) )
	{
		send_to_char("You must be wielding a weapon to swing.\n\r", ch );
		return;
	}

	if (check_chstr == 25)
	{
		dam = dice(wield->value[1],wield->value[2]) * 5;
	}
	else if (check_chstr == 23)
	{
		dam = dice(wield->value[1],wield->value[2]) * 2 ;
	}
	else if (check_chstr < 23)
	{
		dam = dice(wield->value[1],wield->value[2]) * 1.5;
	}
	else
	{
		dam = dice(wield->value[1],wield->value[2]) + 30;
	}

	if (check_chdex == 25)
	{
		max_hit = 4;
	}
	else if (check_chdex == 24)
	{
		max_hit = 3;
	}
	else if (check_chdex < 24)
	{
		max_hit = 2;
	}
	else
	{
		max_hit = 1;
	}

	for ( i=0; i < max_hit; i++ )
	{
		act("$n violently swings out with $s weapon!",ch,0,victim,TO_NOTVICT);
		act("You violently swing out at $N with your weapon!",ch,0,victim,TO_CHAR);
        act("$n violently swings out at you with $s weapon!",ch,0,victim,TO_VICT);
        damage( ch, victim, dam, gsn_swing,DAM_NONE,TRUE);
	}
	check_improve(ch,gsn_swing,TRUE,1);
	WAIT_STATE(ch,2*PULSE_VIOLENCE);
	return;
}





