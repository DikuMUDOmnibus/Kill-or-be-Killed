#include "include.h"

DECLARE_DO_FUN(do_myell);
DECLARE_DO_FUN(do_yell);

void do_affadd(CHAR_DATA *ch, char *argument) {

	char arg1[MSL];
	char arg2[MSL];
	char arg3[MSL];
	char arg4[MSL];
	char buf[MSL];
	CHAR_DATA *victim;
	AFFECT_DATA af;
	int sn, duration, affect_type, affect_location;

	argument = one_argument(argument,arg1);
	argument = one_argument(argument,arg2);
	argument = one_argument(argument,arg3);
	argument = one_argument(argument,arg4);

	if ((arg1[0] == '\0') || (arg2[0] == '\0') || (arg3[0] == '\0') || (arg4[0] == '\0') || (argument[0] == '\0'))
	{
		send_to_char("Syntax: affadd <char> <type> <affect> <duration> <message>\n\r",ch);
		return;
	}

	if ((victim = get_char_world(ch,arg1)) == NULL)
	{
		send_to_char("They aren't playing.\n\r",ch);
		return;
	}

	if (!str_prefix(arg2,"song"))
	{
		affect_type=AFT_SONG;
	}
	else if (!str_prefix(arg2,"spell"))
	{
		affect_type=AFT_SPELL;
	}
	else if (!str_prefix(arg2,"commune"))
	{
		affect_type=AFT_COMMUNE;
	}
	else if (!str_prefix(arg2,"malady"))
	{
		affect_type=AFT_MALADY;
	}
	else if (!str_prefix(arg2,"power"))
	{
		affect_type=AFT_POWER;
	}
	else if (!str_prefix(arg2,"skill"))
	{
		affect_type=AFT_SKILL;
	}
	else if (!str_prefix(arg2,"hidden"))
	{
		affect_type=AFT_HIDDEN;
	}
	else
	{
		send_to_char("Valid types are: skill song spell commune malady power hidden\n\r",ch);
		return;
	}


	sn = skill_lookup(arg3);

	if (sn < 0)
	{
		send_to_char("No such skill or spell.\n\r",ch);
		return;
	}

	if (is_affected(victim,sn))
	{
		send_to_char("Target is already affected by that.\n\r",ch);
		return;
	}

	if (!str_prefix(arg3,"rot"))
	{
		affect_location = APPLY_CON;
	}
	else
	{
		affect_location = 0;
	}

	duration = atoi(arg4);

	init_affect(&af);
	af.aftype		= affect_type;
	af.where        = TO_AFFECTS;
	af.type         = sn;
	af.level        = ch->level;
	af.duration     = duration;
	af.owner		= ch;
	af.location     = affect_location;
	af.modifier     = 0;
	af.bitvector    = 0;
	affect_to_char(victim,&af);

	smash_tilde( argument );
	sprintf(buf,"%s\n\r",argument);
	send_to_char(buf,victim);
}

void do_crescent_kick(CHAR_DATA *ch,char *argument)
{
	char arg[MIL];
	CHAR_DATA *victim;
    OBJ_DATA *obj;
    OBJ_DATA *secondary;
	int dam, chance;

	one_argument(argument,arg);

	if ((chance = get_skill(ch,gsn_crescent_kick)) == 0)
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

    if (arg[0] == '\0')
        victim = ch->fighting;
    else
        victim = get_char_room(ch,arg);

    if (victim == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }
    if (victim == ch)
    {
        send_to_char("Now that would be a bit stupid.\n\r",ch);
        return;
    }
    if ((victim->fighting != ch) && (ch->fighting != victim))
    {
        send_to_char("But you aren't engaged in combat with them.\n\r",ch);
        return;
    }

    if (is_affected(ch,AFF_BLIND))
    {
        chance /= 2;
    }

	if (chance > number_percent())
	{
		act("You begin the fluid motions of the crescent kick.",ch,NULL,victim,TO_CHAR);
		act("$n begins the fluid motions of the crescent kick.",ch,NULL,victim,TO_VICT);
		act("$n begins the fluid motions of the crescent kick.",ch,NULL,victim,TO_NOTVICT);

		dam = (ch->drain_level + ch->level)/2;
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(ch->level/5,ch->level/4);

		WAIT_STATE(ch,PULSE_VIOLENCE*2);
		damage(ch,victim,dam,gsn_crescent_kick,DAM_BASH,TRUE);

		chance /= 1.4;

		obj = get_eq_char(victim,WEAR_WIELD);

		if ( (chance > number_percent()) && (obj != NULL) )
		{

		    act("As you complete the motions of your crescent kick you aim for $N's weapon.",ch,NULL,victim,TO_CHAR);
		    act("As $n completes the motions of $s crescent kick $m aims for your weapon.",ch,NULL,victim,TO_VICT);
		    act("As $n completes the motions of $s crescent kick $m aims for $n's weapon.",ch,NULL,victim,TO_NOTVICT);

			if (IS_OBJ_STAT(obj,ITEM_NOREMOVE) || IS_OBJ_STAT(obj,ITEM_NODISARM) || is_affected(victim,gsn_spiderhands))
			{
			    act("Your heel makes solid contact with $N's weapon, but it doesn't budge!",ch,NULL,victim,TO_CHAR);
			    act("$n's heel makes solid contact with your weapon, but it doesn't budge!",ch,NULL,victim,TO_VICT);
			    act("$n's heel makes solid contact with $N's weapon, but it doesn't budge!",ch,NULL,victim,TO_NOTVICT);
			    return;
			}
			else
			{
				act("You strike $N's weapon with your heel and send it flying!",ch,NULL,victim,TO_CHAR);
				act("$N strikes your weapon with $s heel and sends it flying!",ch,NULL,victim,TO_VICT);
				act("$n strikes $N's weapon with $s heel and sends it flying!",ch,NULL,victim,TO_NOTVICT);

				obj_from_char( obj );
				if (IS_OBJ_STAT(obj,ITEM_NODROP))
				{
					obj_to_char(obj,victim);
				}
				else
				{
					obj_to_room( obj, victim->in_room );
					reslot_weapon(victim);

					if (!IS_NPC(victim) && (secondary = get_eq_char(victim,WEAR_DUAL_WIELD)) != NULL)
					{
						unequip_char(victim,secondary);
						equip_char(victim,secondary,WEAR_WIELD);
					}
					else if (IS_NPC(victim) && victim->wait == 0 && can_see_obj(victim,obj))
					{
						WAIT_STATE(victim,PULSE_VIOLENCE*3);
						get_obj(victim,obj,NULL);
						do_wear(victim,obj->name);
					}
				}
			}
		}
		check_improve(ch,gsn_crescent_kick,TRUE,1);
	}
	else
	{
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
		damage(ch,victim,0,gsn_crescent_kick,DAM_BASH,TRUE);
		check_improve(ch,gsn_crescent_kick,FALSE,1);
	}
}

void do_axe_kick(CHAR_DATA *ch,char *argument)
{
	char arg[MIL];
	AFFECT_DATA af;
	CHAR_DATA *victim;
	int dam, chance;

	one_argument(argument,arg);

	if ((chance = get_skill(ch,gsn_axe_kick)) == 0)
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

    if (arg[0] == '\0')
        victim = ch->fighting;
    else
        victim = get_char_room(ch,arg);

    if (victim == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }
    if (victim == ch)
    {
        send_to_char("Now that would be a bit stupid.\n\r",ch);
        return;
    }
    if ((victim->fighting != ch) && (ch->fighting != victim))
    {
        send_to_char("But you aren't engaged in combat with them.\n\r",ch);
        return;
    }

    if (is_affected(ch,AFF_BLIND))
    {
        chance /= 2;
    }

	if (chance > number_percent() && IS_SET(victim->parts,PART_ARMS))
	{
		act("You raise your leg high above $n's shoulder and lower it in a swift strike.",ch,NULL,victim,TO_CHAR);
		act("$n raises $s leg high above your shoulder and lowers it in a swift strike.",ch,NULL,victim,TO_VICT);
		act("$n raises $s leg high above $S's shoulder and lowers it in a swift strike.",ch,NULL,victim,TO_NOTVICT);

		dam = (ch->drain_level + ch->level)/1.5;
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(ch->level/5,ch->level/4);

		WAIT_STATE(ch,PULSE_VIOLENCE*2);
		damage(ch,victim,dam,gsn_axe_kick,DAM_BASH,TRUE);

		chance -= (victim->size - ch->size) * 20;

		if (chance > number_percent())
		{
			if (!is_affected(victim,gsn_axe_kick))
			{
				act("The sound of bones cracking echos from $N's shoulder as you shatter $S shoulder blade.",ch,NULL,victim,TO_CHAR);
				act("The sound of bones cracking echos from your shoulder as $n shatters your shoulder blade.",ch,NULL,victim,TO_VICT);
				act("The sound of bones cracking echos from $N's shoulder as $n shatters $S shoulder blade.",ch,NULL,victim,TO_NOTVICT);

				init_affect(&af);
				af.where        = TO_AFFECTS;
				af.type         = gsn_axe_kick;
				af.aftype		= AFT_MALADY;
				af.level        = ch->level;
				af.duration     = ch->level/8;
				af.modifier     = -(ch->level/6);
				af.bitvector    = 0;
				af.location     = APPLY_STR;
				affect_to_char(victim,&af);
				af.location     = APPLY_DEX;
				affect_to_char(victim,&af);
			}
			else
			{
				act("You hear the shattered bone in $N's shoulder shifting as you strike $S shoulder blade squarely again.",ch,NULL,victim,TO_CHAR);
				act("You hear the shattered bone in your shoulder shifting as $n strikes your shoulder blade squarely again.",ch,NULL,victim,TO_VICT);
				act("You hear the shattered bone in $N's shoulder shifting as $n strikes $S shoulder blade squarely again.",ch,NULL,victim,TO_NOTVICT);
			}

		}
		check_improve(ch,gsn_axe_kick,TRUE,1);
	}
	else
	{
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
		damage(ch,victim,0,gsn_axe_kick,DAM_BASH,TRUE);
		check_improve(ch,gsn_axe_kick,FALSE,1);
	}
}

void do_scissor_kick(CHAR_DATA *ch,char *argument)
{
	char arg[MIL];
	CHAR_DATA *victim;
	int dam, chance;

	one_argument(argument,arg);

	if ((chance = get_skill(ch,gsn_scissor_kick)) == 0)
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

    if (arg[0] == '\0')
        victim = ch->fighting;
    else
        victim = get_char_room(ch,arg);

    if (victim == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }
    if (victim == ch)
    {
        send_to_char("Now that would be a bit stupid.\n\r",ch);
        return;
    }
    if ((victim->fighting != ch) && (ch->fighting != victim))
    {
        send_to_char("But you aren't engaged in combat with them.\n\r",ch);
        return;
    }

    if (is_affected(ch,AFF_BLIND))
    {
        chance /= 2;
    }

	if (chance > number_percent())
	{
		act("You leap towards $N in an attempt to wrap your legs around $M.",ch,NULL,victim,TO_CHAR);
		act("$n leaps towards you in an attempt to wrap $s legs around you.",ch,NULL,victim,TO_VICT);
		act("$n leaps towards $N in an attempt to wrap $s legs around $M.",ch,NULL,victim,TO_NOTVICT);

		dam = (ch->drain_level + ch->level)/1.5;
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(ch->level/5,ch->level/4);

		damage(ch,victim,dam,gsn_scissor_kick,DAM_BASH,TRUE);

	    chance -= victim->carry_weight / 200;
		chance -= (victim->size - ch->size) * 10;

		if (chance > number_percent())
		{
			act("Upon successfully enclosing $N's upper body in your legs, you bring $M to the ground!",ch,NULL,victim,TO_CHAR);
			act("Upon successfully enclosing you in $s legs, $n brings you to the ground!",ch,NULL,victim,TO_VICT);
			act("Upon successfully enclosing $N in $s legs, $n brings $N to the ground!.",ch,NULL,victim,TO_NOTVICT);
			WAIT_STATE(victim,PULSE_VIOLENCE*2);
		}
		check_improve(ch,gsn_scissor_kick,TRUE,1);
	}
	else
	{
		damage(ch,victim,0,gsn_scissor_kick,DAM_BASH,TRUE);
		check_improve(ch,gsn_scissor_kick,FALSE,1);
	}
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
}

void do_tigerclaw (CHAR_DATA *ch,char *argument)
{
	char arg[MIL];
	char buf[MSL];
	CHAR_DATA *victim;
	int dam, chance;
	AFFECT_DATA af;

	one_argument(argument,arg);

	if ((chance = get_skill(ch,gsn_tigerclaw)) == 0)
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

    if (arg[0] == '\0')
        victim = ch->fighting;
    else
        victim = get_char_room(ch,arg);

    if (victim == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }
    if (victim == ch)
    {
        send_to_char("You jab at your own throat like an idiot.\n\r",ch);
        return;
    }

	if (is_affected(victim,gsn_tigerclaw))
	{
		send_to_char("Their throat is already constricted.\n\r",ch);
		return;
	}

    if (is_affected(ch,AFF_BLIND))
    {
        chance /= 5;
    }

    chance -= ( (get_curr_stat(victim,STAT_DEX))-(get_curr_stat(ch,STAT_DEX)) );

	if (chance > number_percent())
	{
		act("You tighten your fist with a knuckle extruding and strike at $N's throat.",ch,NULL,victim,TO_CHAR);
		act("$n tightens $s fist with a knuckle extruding and strikes at your throat.",ch,NULL,victim,TO_VICT);
		act("$n tightens $s fist with a knuckle extruding and strikes at $N's throat.",ch,NULL,victim,TO_NOTVICT);

		dam = number_range(0,(victim->level + ch->level));

		damage(ch,victim,dam,gsn_tigerclaw,DAM_BASH,TRUE);


		act("You strike $N's throat, temporarily closing $S vocal passage!",ch,NULL,victim,TO_CHAR);
		act("$n strikes your throat, temporarily closing your vocal passage!",ch,NULL,victim,TO_VICT);
		act("$n strikes $N's throat, temporarily closing $S vocal passage!",ch,NULL,victim,TO_NOTVICT);

		init_affect(&af);
		af.where        = TO_AFFECTS;
		af.type         = gsn_tigerclaw;
		af.aftype		= AFT_MALADY;
		af.level        = ch->level;
		af.duration     = ch->level/12;
		af.modifier     = 0;
		af.bitvector    = 0;
		affect_to_char(victim,&af);
		check_improve(ch,gsn_tigerclaw,TRUE,1);
	}
	else
	{
		damage(ch,victim,0,gsn_tigerclaw,DAM_BASH,TRUE);
		check_improve(ch,gsn_tigerclaw,FALSE,1);
	}

	sprintf(buf,"Help! %s is jabbing at my throat!",PERS(ch,victim));
	do_myell(victim,buf);

	WAIT_STATE(ch,PULSE_VIOLENCE*2);
}

void do_bindwounds(CHAR_DATA *ch,char *argument)
{
	AFFECT_DATA af;

	if ( (get_skill(ch,gsn_bandage) == 0) || (ch->level < skill_table[gsn_bandage].skill_level[ch->class]) )
	{
        send_to_char("Huh?\n\r",ch);
        return;
	}

	if (is_affected(ch,gsn_bindwounds))
	{
        send_to_char("You can't apply more aid yet.\n\r",ch);
		return;
	}
	if (ch->mana < 15)
	{
        send_to_char("You don't have the mana.\n\r",ch);
        return;
	}

	if (number_percent() > ch->pcdata->learned[gsn_bindwounds])
	{
        send_to_char("You fail to focus on your injuries.\n\r",ch);
        act("$n fails to focus on $s injuries and bind $s wounds.",ch,0,0,TO_ROOM);
        ch->mana -= 12;
        check_improve(ch,gsn_bindwounds,FALSE,3);
        return;
	}

	ch->mana -= 25;

	act("$n focuses on $s injuries and binds $s wounds.",ch,0,0,TO_ROOM);
	send_to_char("You focus on your injuries and bind your wounds.\n\r",ch);
	send_to_char("You feel better.\n\r",ch);

	ch->hit += ch->max_hit/10;
	ch->hit  = UMIN(ch->hit,ch->max_hit);

	if ( number_percent() < UMAX(1,(ch->drain_level + ch->level)/4) && is_affected(ch,AFF_PLAGUE) )
	{
		affect_strip(ch,gsn_plague);
		act("The sores on $n's body vanish.\n\r",ch,0,0,TO_ROOM);
		send_to_char("The sores on your body vanish.\n\r",ch);
	}

	if ( number_percent() < UMAX(1,(ch->drain_level + ch->level)) && is_affected(ch, gsn_blindness) )
	{
		affect_strip(ch, gsn_blindness);
		send_to_char("Your vision returns!\n\r",ch);
	}

	if ( number_percent() < UMAX(1,(ch->drain_level + ch->level)) && is_affected(ch, gsn_blindness_dust) )
	{
		affect_strip(ch, gsn_blindness_dust);
		send_to_char("You wipe the dust from your eyes.\n\r",ch);
	}

	if ( number_percent() < UMAX(1,(ch->drain_level + ch->level)/2) && is_affected(ch,gsn_poison))
	{
		affect_strip(ch,gsn_poison);
		send_to_char("A warm feeling goes through your body.\n\r",ch);
		act("$n looks better.",ch,0,0,TO_ROOM);
	}

	if ( number_percent() < UMAX(1,(ch->drain_level + ch->level)/2) && is_affected(ch,gsn_poison_dust))
	{
		affect_strip(ch,gsn_poison_dust);
		send_to_char("The dust in your eyes is cleaned out.\n\r",ch);
		act("The dust in $n's eyes looks cleaned out.",ch,0,0,TO_ROOM);
	}

	check_improve(ch,gsn_bindwounds,TRUE,3);
	init_affect(&af);
	af.where		= TO_AFFECTS;
	af.type			= gsn_bindwounds;
	af.location		= 0;
	af.duration		= 2;
	af.modifier		= 0;
	af.bitvector	= 0;
	af.level		= ch->level;
	af.aftype		= AFT_SKILL;
	affect_to_char(ch,&af);
	return;
}

void do_vanish(CHAR_DATA *ch,char *argument)
{
    ROOM_INDEX_DATA *pRoomIndex;
    int chance;

    if ( (chance = get_skill(ch,gsn_vanish)) == 0 || ch->level < skill_table[gsn_vanish].skill_level[ch->class])
    {
		send_to_char("Huh?\n\r",ch);
		return;
    }

	if (ch->mana < 20)
    {
		send_to_char("You don't have the mana.",ch);
		return;
    }

	if (ch->position==POS_FIGHTING)
	{
		chance /= 2;
	}

	send_to_char("You throw down a small globe which produces a cloud of smoke.\n\r",ch);
	act("$n throws down a small globe which produces a cloud of smoke.",ch,0,0,TO_ROOM);

	WAIT_STATE(ch,PULSE_VIOLENCE*1);
    if (number_percent() > chance)
    {
		send_to_char("The smoke clears to quickly for you to do anything.\n\r",ch);
		act("The smoke clears to quickly for $n to do anything.",ch,0,0,TO_ROOM);
		check_improve(ch,gsn_vanish,FALSE,2);
		ch->mana -= 10;
		return;
    }

	ch->mana -= 20;

	if (ch->in_room->vnum ==  23610 || ch->in_room->vnum == ROOM_FORCE_DUEL)
    {
		send_to_char("You fail.\n\r",ch);
		return;
    }

    for (; ;)
    {
		pRoomIndex = get_room_index(number_range(0,30000));
		if (pRoomIndex != NULL)
			if (pRoomIndex->area == ch->in_room->area
				&& !IS_SET(pRoomIndex->room_flags,ROOM_PRIVATE)
				&& (pRoomIndex->guild == 0)
				&& can_see_room(ch,pRoomIndex)
				&& !IS_SET(pRoomIndex->room_flags,ROOM_SOLITARY)
				&& ch->in_room->vnum != 1212
				&& pRoomIndex->vnum != 1212)
				break;
    }

    act("As the smoke clears $n is gone!",ch,0,0,TO_ROOM);
    send_to_char("You vanish into the smoke!\n\r",ch);
	check_improve(ch,gsn_vanish,TRUE,2);
    char_from_room(ch);
    char_to_room(ch,pRoomIndex);
    act("$n has arrived.",ch,0,0,TO_ROOM);
    do_look(ch,"auto");

    return;
}

void do_endure(CHAR_DATA *ch,char *argument)
{
	AFFECT_DATA af;
	int chance;

	if ( (chance = get_skill(ch,gsn_endure)) == 0 || ch->level < skill_table[gsn_endure].skill_level[ch->class])
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

	if (ch->fighting != NULL)
	{
		send_to_char("You can't concentrate enough.\n\r",ch);
		return;
	}

	if (ch->mana < 30)
	{
		send_to_char("You don't have enough energy.\n\r",ch);
		return;
	}

	if (is_affected(ch,gsn_endure))
	{
		send_to_char("You are already prepared for magical encounters.\n\r",ch);
		return;
	}

	WAIT_STATE(ch,PULSE_VIOLENCE*2);

	if (number_percent() < chance)
	{
		act("$n concentrates and resumes $s position.\n\r",ch,0,0,TO_ROOM);
		send_to_char("You prepare yourself for magical encounters.\n\r",ch);

		ch->mana -= 30;

		init_affect(&af);
		af.where		= TO_AFFECTS;
		af.aftype		= AFT_SKILL;
		af.type			= gsn_endure;
		af.location		= APPLY_SAVING_SPELL;
		af.duration		= ch->level/2;
		af.modifier		= -(ch->level/2);
		af.bitvector	= 0;
		af.level		= ch->level;
		affect_to_char(ch,&af);
		check_improve(ch,gsn_endure,TRUE,1);
	}
	else
	{
		send_to_char("You fail to prepare yourself for magical encounters.\n\r",ch);

		ch->mana -= 15;

		check_improve(ch,gsn_endure,FALSE,1);
	}
	return;
}

void do_heightened_awareness(CHAR_DATA *ch,char *argument)
{
	AFFECT_DATA af;
	int chance;

	if ( (chance = get_skill(ch,gsn_heightened_awareness)) == 0 || ch->level < skill_table[gsn_heightened_awareness].skill_level[ch->class])
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

	if (ch->fighting != NULL)
	{
		send_to_char("You can't concentrate enough.\n\r",ch);
		return;
	}

	if (ch->mana < 20)
	{
		send_to_char("You don't have enough energy.\n\r",ch);
		return;
	}

	if (is_affected(ch,gsn_heightened_awareness))
	{
		send_to_char("You cannot heighten your awareness any further.\n\r",ch);
		return;
	}

	WAIT_STATE(ch,PULSE_VIOLENCE*1);

	if (number_percent() < chance)
	{
		send_to_char("You feel more aware of your surroundings.\n\r",ch);

		ch->mana -= 20;

		init_affect(&af);
		af.where		= TO_AFFECTS;
		af.aftype		= AFT_SKILL;
		af.type			= gsn_heightened_awareness;
		af.location		= 0;
		af.duration		= ch->level/2;
		af.modifier		= 0;
		af.bitvector	= 0;
		af.level		= ch->level;
		affect_to_char(ch,&af);
		check_improve(ch,gsn_heightened_awareness,TRUE,1);
	}
	else
	{
		send_to_char("You fail to heighten your awareness.\n\r",ch);

		ch->mana -= 10;

		check_improve(ch,gsn_heightened_awareness,FALSE,1);
	}
	return;
}

void do_jujitsu(CHAR_DATA *ch,char *argument)
{
	AFFECT_DATA af;
	char arg[MIL];
	CHAR_DATA *victim;
	int chance;

	one_argument(argument,arg);

	if ((chance = get_skill(ch,gsn_jujitsu)) == 0)
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

    if (arg[0] == '\0')
        victim = ch->fighting;
    else
        victim = get_char_room(ch,arg);

    if (victim == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }
    if (victim == ch)
    {
        send_to_char("Now that would be a bit stupid.\n\r",ch);
        return;
    }
    if ((victim->fighting != ch) && (ch->fighting != victim))
    {
        send_to_char("But you aren't engaged in combat with them.\n\r",ch);
        return;
    }

    if (is_affected(ch,AFF_BLIND))
    {
        chance /= 2;
    }

	if (chance > number_percent())
	{
		act("You prepare yourself for a complex manuever.",ch,NULL,victim,TO_CHAR);
		act("$n prepares $mself for a complex manuever.",ch,NULL,victim,TO_VICT);
		act("$n prepares $mself for a complex manuever.",ch,NULL,victim,TO_NOTVICT);

		affect_strip(ch,gsn_jujitsu);

		init_affect(&af);
		af.where		= TO_AFFECTS;
		af.aftype		= AFT_HIDDEN;
		af.type			= gsn_jujitsu;
		af.wait			= 3;
		af.location		= 0;
		af.duration		= 5;
		af.modifier		= 0;
		af.owner		= victim;
		af.bitvector	= 0;
		af.level		= ch->level;
		affect_to_char(ch,&af);
	}
	WAIT_STATE(ch,PULSE_VIOLENCE*3);
	return;
}

void do_autodamage(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch))
    return;

    if (IS_SET(ch->act,PLR_AUTODAMAGE))
    {
    	send_to_char("Autodamage is currently OFF.\n\r",ch);
        REMOVE_BIT(ch->act,PLR_AUTODAMAGE);
    }
    else
    {
    	send_to_char("Autodamage is currently ON.\n\r",ch);
        SET_BIT(ch->act,PLR_AUTODAMAGE);
    }
 }

