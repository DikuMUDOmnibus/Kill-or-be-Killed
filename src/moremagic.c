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

DECLARE_DO_FUN(do_myell);
DECLARE_DO_FUN(do_look);
DECLARE_DO_FUN(do_stand);
//local fun's
int     spellcraft_dam  args( ( int num, int dice) );
bool    check_spellcraft args( (CHAR_DATA *ch, int sn) );
bool cleansed args ((CHAR_DATA *ch, CHAR_DATA *victim,int diffmodifier, int sn));
int get_affect_level args ((CHAR_DATA *ch, int sn));
char	*target_name;
int     find_door       args( ( CHAR_DATA *ch, char *arg ) );

void spell_self_immolation(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (is_affected(ch,sn))
	{
	    send_to_char("You are already immolating.\n\r",ch);
	    return;
	}

        init_affect(&af);
	af.type = sn;
        af.aftype    = AFT_SPELL;
	af.duration = ch->level / 4;
	af.modifier = 0;
	af.bitvector = 0;
	af.where = TO_AFFECTS;
	af.location = 0;
	affect_to_char(ch, &af);

	act("$n suddenly bursts into flames!", ch,NULL,NULL,TO_ROOM);
	act("You suddenly burst into flames!", ch,NULL,NULL,TO_CHAR);
}

void spell_shield_of_bubbles( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected(victim,gsn_shield_of_bubbles))
    {
	if (victim == ch)
	  send_to_char("You are already surrounded in a shield of bubbles.\n\r",ch);
	else
	  act("$N is already surrouned by a shield of bubbles.",ch,NULL,victim,TO_CHAR);
	return;
    }

        init_affect(&af);
    af.where     = TO_AFFECTS;
        af.aftype    = AFT_SPELL;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 5;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n is surrounded by a shield of bubbles.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are surrounded by a shield of bubbles.\n\r", victim );
    return;
}

void spell_shield_of_flames( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected(victim,gsn_shield_of_flames))
    {
	if (victim == ch)
	  send_to_char("You are already surrounded in a shield of flames.\n\r",ch);
	else
	  act("$N is already surrouned by a shield of flames.",ch,NULL,victim,TO_CHAR);
	return;
    }

        init_affect(&af);
    af.where     = TO_AFFECTS;
        af.aftype    = AFT_SPELL;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 5;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n is surrounded by a shield of flames.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are surrounded by a shield of flames.\n\r", victim );
    return;
}

void spell_shield_of_frost( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected(victim,gsn_shield_of_frost))
    {
	if (victim == ch)
	  send_to_char("You are already surrounded in a shield of frost.\n\r",ch);
	else
	  act("$N is already surrouned by a shield of frost.",ch,NULL,victim,TO_CHAR);
	return;
    }

        init_affect(&af);
    af.where     = TO_AFFECTS;
        af.aftype    = AFT_SPELL;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 5;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n is surrounded by a shield of frost.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are surrounded by a shield of frost.\n\r", victim );
    return;
}

void spell_shield_of_lightning( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected(victim,gsn_shield_of_lightning))
    {
	if (victim == ch)
	  send_to_char("You are already surrounded in a shield of lightning.\n\r",ch);
	else
	  act("$N is already surrouned by a shield of lightning.",ch,NULL,victim,TO_CHAR);
	return;
    }

        init_affect(&af);
    af.where     = TO_AFFECTS;
        af.aftype    = AFT_SPELL;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 5;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n is surrounded by a shield of lightning.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are surrounded by a shield of lightning.\n\r", victim );
    return;
}

void spell_shield_of_wind( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected(victim,gsn_shield_of_wind))
    {
	if (victim == ch)
	  send_to_char("You are already surrounded in a shield of wind.\n\r",ch);
	else
	  act("$N is already surrouned by a shield of wind.",ch,NULL,victim,TO_CHAR);
	return;
    }

        init_affect(&af);
    af.where     = TO_AFFECTS;
        af.aftype    = AFT_SPELL;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 5;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n is surrounded by a shield of wind.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are surrounded by a shield of wind.\n\r", victim );
    return;
}

void spell_shield_of_dust( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected(victim,gsn_shield_of_dust))
    {
	if (victim == ch)
	  send_to_char("You are already surrounded in a shield of dust.\n\r",ch);
	else
	  act("$N is already surrouned by a shield of dust.",ch,NULL,victim,TO_CHAR);
	return;
    }

        init_affect(&af);
    af.where     = TO_AFFECTS;
        af.aftype    = AFT_SPELL;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 5;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n is surrounded by a shield of dust.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are surrounded by a shield of dust.\n\r", victim );
    return;
}

void spell_hydrolic_ram( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
	CHAR_DATA *victim;
//	char arg[MAX_INPUT_LENGTH];
//	int door;
	int dam;
	int chance;
//	char *temp_arg[MAX_INPUT_LENGTH];
//	door = -1;
/*
	temp_arg = (char *) vo;

	one_argument(temp_arg,arg);


	if ( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) ) door = 0;
    else if ( !str_cmp( arg, "e" ) || !str_cmp( arg, "east"  ) ) door = 1;
    else if ( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) ) door = 2;
    else if ( !str_cmp( arg, "w" ) || !str_cmp( arg, "west"  ) ) door = 3;
    else if ( !str_cmp( arg, "u" ) || !str_cmp( arg, "up"    ) ) door = 4;
    else if ( !str_cmp( arg, "d" ) || !str_cmp( arg, "down"  ) ) door = 5;

    if (door != -1)
    {
	chance = get_skill(ch,sn);

	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;
	pexit = ch->in_room->exit[door];
	if (!IS_SET(pexit->exit_info,EX_CLOSED))
	{
          send_to_char("It's not closed.\n\r",ch);
          return;
	}

    if (number_percent() > chance || IS_SET(pexit->exit_info,EX_NOBASH) )
    {
    act("$n fires a concentrated jet of water at the $T door but the door fails to budge!",ch,0,dir_name[door],TO_ROOM);
    act("You fire a concentrated jet of water at the $T door but the door fails to budge!",ch,0,dir_name[door],TO_CHAR);
    return;
    }

    act("$n fire a concentrated jet of water at the $T door and smashes it open!",ch,0,dir_name[door],TO_ROOM);
    act("You fire a concentrated jet of water at the $T door and smash it open!",ch,0,dir_name[door],TO_CHAR);
    REMOVE_BIT(pexit->exit_info,EX_LOCKED);
    REMOVE_BIT(pexit->exit_info,EX_CLOSED);
    if ( ( (to_room = pexit->u1.to_room) != NULL)
    && ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
    && pexit_rev->u1.to_room == ch->in_room)
    {
        REMOVE_BIT(pexit_rev->exit_info,EX_LOCKED);
        REMOVE_BIT(pexit_rev->exit_info,EX_CLOSED);
    }
   return;
    }	//end the door bustin section
*/
	victim = (CHAR_DATA *) vo;
	if (victim == NULL)
		victim = ch->fighting;
	dam = dice(level,3) + level;
	act("You fire a concentrated jet of water at $N!",ch,NULL,victim,TO_CHAR);
	act("$n fires a concentrated jet of water at $N!",ch,NULL,victim,TO_NOTVICT);
	act("$n fires a concentrated jet of water at you!",ch,NULL,victim,TO_VICT);
	damage_old(ch,victim,dam,sn, DAM_DROWNING, TRUE);

	chance = get_skill(ch,sn);
	if (number_percent() > chance)
		return;

	act("Your hydrolic ram blasts $N off $S feet!",ch,NULL,victim,TO_CHAR);
	act("$n's hydrolic ram blasts you off your feet!",ch,NULL,victim,TO_VICT);
	act("$n's hydrolic ram knocks $N off $S feet!",ch,NULL,victim,TO_NOTVICT);


	WAIT_STATE(victim, PULSE_VIOLENCE);
}

void spell_spontanious_combustion(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    act("$n screams in agony as flames engulf $s body.",victim,0,0,TO_ROOM);
    send_to_char("You feel a sudden intense pain throughout your body as you are engulfed in flames!\n\r",victim);
    dam = dice(level,10) + 3 * ch->level;
    damage_old(ch,victim,saves_spell(level,victim,DAM_FIRE) ? dam/2 : dam,sn,DAM_FIRE,TRUE);
    return;

}

void spell_shockwave(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char buf[MAX_STRING_LENGTH];
    int lag;

    WAIT_STATE(ch,2 * PULSE_VIOLENCE);

    act("$n stomps $s foot and the ground begins to shake!",ch,NULL,NULL,TO_ROOM);
    act("You stomp your foot and the ground begins to shake!",ch,NULL,NULL,TO_CHAR);

    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
	vch_next = vch->next_in_room;
	if (is_same_group(vch,ch) )
	    continue;
	if (!IS_NPC(ch) && !IS_NPC(vch)
	&& (ch->fighting == NULL || vch->fighting == NULL))
	{
		switch(number_range(0,2))
		{
		case (0):
		case (1):
		sprintf(buf,"Die, %s you sorcerous dog!",PERS(ch,vch));
		break;
		case (2):
		sprintf(buf,"Help! %s is casting a spell on me!",PERS(ch,vch));
		}
	if (vch != ch)
		do_myell(vch,buf);
	}
	if (vch->position != POS_FIGHTING)
	{
		vch->position = POS_FIGHTING;
		vch->fighting = ch;
	}
	if (ch->position != POS_FIGHTING)
	{
		ch->position = POS_FIGHTING;
		ch->fighting = vch;
	}
	lag = number_range(0,3);
	if (lag && !IS_AFFECTED(vch,AFF_FLYING))
	{
		act("$n is knocked of $s feet by the shockwave!",vch,NULL,NULL,TO_ROOM);
		act("You are knocked off your feet by the shockwave!",vch,NULL,NULL,TO_CHAR);
		WAIT_STATE(vch,lag * PULSE_VIOLENCE);
	}
    }
}


void spell_static_charge( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (is_affected(victim,gsn_static_charge) )
	{
	send_to_char("You are already fully charged!\n\r",victim);
	return;
	}

        init_affect(&af);
    af.where 	= TO_AFFECTS;
        af.aftype    = AFT_SPELL;
    af.type 	= gsn_static_charge;
    af.level 	= level;
    af.duration = number_range(1,5);
    af.modifier = 0;
    af.location = 0;
    af.bitvector = 0;
    affect_to_char(victim, &af);
    send_to_char("You charge yourself with static electricity!\n\r",victim);
    act("$n begins to glow softly as $s hair begins to stand on end.",victim,NULL,NULL,TO_ROOM);
    return;
}


void spell_flicker(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    ROOM_INDEX_DATA *pRoomIndex;
    int chance;
    int count;
    chance = get_skill(ch,sn);

    chance -= 10;
    if (ch->position==POS_FIGHTING)
    {
	chance -= 50;
    }

    if (number_percent() > chance)
    {
	send_to_char("You begin to flicker but nothing happens.\n\r",ch);
	act("$n seems to flicker for a moment but nothing happens.",ch,0,0,TO_ROOM);
	return;
    }
    if (ch->in_room->vnum ==  23610 || ch->in_room->vnum == ROOM_FORCE_DUEL)
    {
	send_to_char("You fail.\n\r",ch);
	return;
    }
    count = 0; //keep the game from hanging if it never finds a room
    while (TRUE)
    {
        count++;
        if (count > 30000)
        {
	    send_to_char("you fail.\n\r",ch);
	    return;
	}
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

    act("$n begins to flicker and suddenly dissapears!",ch,0,0,TO_ROOM);
    send_to_char("You begin to flicker and suddenly find yourself somewhere else!\n\r",ch);
    char_from_room(ch);
    char_to_room(ch,pRoomIndex);
    act("The likeness of $n flickers into being.",ch,0,0,TO_ROOM);
    do_look(ch,"auto");
    return;
}


void spell_smoke(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (IS_AFFECTED(victim,AFF_BLIND))
    {
	send_to_char("They are already blind.\n\r",ch);
	return;
    }

    act("$n breathes a puff of smoke into $N's face!",ch,NULL,victim,TO_NOTVICT);
    act("$n breaths a puff of smoke into your face!",ch,NULL,victim,TO_VICT);
    act("You breath a puff of smoke into $N's face!",ch,NULL,victim,TO_CHAR);

    if (saves_spell(level,victim,DAM_OTHER))
    {
	act("$n manages to keep the smoke out of their eyes.",victim,NULL,NULL,TO_ROOM);
	act("You manage to keep the smoke out of your eyes.",victim,NULL,NULL,TO_CHAR);
	return;
    }
        init_affect(&af);
    af.where     = TO_AFFECTS;
        af.aftype    = AFT_SPELL;
    af.type      = sn;
    af.level     = level;
    af.location  = APPLY_HITROLL;
    af.modifier  = -4;
    af.duration  = 1;
    af.bitvector = AFF_BLIND;
    affect_to_char( victim, &af );
    send_to_char( "The smoke gets in your eyes and blinds you!\n\r", victim );
    act("$n is blinded by the smoke.",victim,NULL,NULL,TO_ROOM);
    return;

}

void spell_nullify_sound(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        AFFECT_DATA af;

        if (ch == victim)
        {
        send_to_char("That would not be wise.\n\r",ch);
        return;
        }

        if (is_affected(victim,sn))
        {
        act("$N cannot hear anything already.",ch,NULL,victim,TO_CHAR);
        return;
        }

        if (saves_spell(level,victim, DAM_NONE))
        return;

        init_affect(&af);
        af.where     = TO_AFFECTS;
        af.aftype    = AFT_SPELL;
        af.type      = sn;
        af.level     = level;
        af.duration  = 3;
        af.modifier  = 0;
        af.location  = 0;
        af.bitvector = 0;
        affect_to_char(victim,&af);

        act("You have destroyed all sound near $N!",ch,NULL,victim,TO_CHAR);
        send_to_char("The world suddenly grows very silent!\n\r",victim);
}


void spell_displacement(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        AFFECT_DATA af;

        if (ch != victim)
        {
        send_to_char("You can only cast this spell on yourself.\n\r",ch);
        return;
        }

        if (is_affected(ch,sn))
        {
        act("Your true location is already masked.",ch,NULL,NULL,TO_CHAR);
        return;
        }

        init_affect(&af);
        af.where     = TO_AFFECTS;
        af.aftype    = AFT_SPELL;
        af.type      = sn;
        af.level     = level;
        af.duration  = 10;
        af.modifier  = 0;
        af.location  = 0;
        af.bitvector = 0;
        affect_to_char(victim,&af);

        act("You mask your true location!",ch,NULL,victim,TO_CHAR);
}

void spell_detect_illusion(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
        AFFECT_DATA af;
	int dur;

	dur = ch->level / 3;
	if (victim != ch)
		dur /= 2;
        if (is_affected(ch,sn))
        {
        act("Your senses are already enhanced.",ch,NULL,NULL,TO_CHAR);
        return;
        }

        init_affect(&af);
        af.where     = TO_AFFECTS;
        af.aftype    = AFT_SPELL;
        af.type      = sn;
        af.level     = level;
        af.duration  = dur;
        af.modifier  = 0;
        af.location  = 0;
        af.bitvector = 0;
        affect_to_char(victim,&af);

        act("Your senses of perception heighten",victim,NULL,NULL,TO_CHAR);
}

void spell_ice_slick(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	ROOM_AFFECT_DATA raf;

    if ( is_affected( ch, sn ) )
    {
      send_to_char("You cannot make another ice slick so soon.\n\r", ch);
      return;
    }

    if ( is_affected_room( ch->in_room, sn ))
    {
        send_to_char("The ground is already frozen.\n\r",ch);
        return;
    }

	if (number_range(1,100) > ch->pcdata->learned[sn])
	{
		send_to_char("You attempt to freeze the ground but fail.\n\r",ch);
		//WAIT_STATE(ch, PULSE_TICK / 2);
	}


        init_affect(&af);
    af.where     = TO_AFFECTS;
    af.type      = sn;
        af.aftype    = AFT_SPELL;
    af.level     = ch->level;
    af.duration  = 48;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( ch, &af );

        init_affect_room(&raf);
    raf.where     = TO_ROOM_AFFECTS;
        raf.aftype    = AFT_SPELL;
    raf.type      = sn;
    raf.level     = ch->level;
    raf.duration  = ch->level/5;
    raf.modifier  = 0;
    raf.bitvector = AFF_ROOM_ICE_SLICK;
    affect_to_room( ch->in_room, &raf );


    send_to_char("You freeze the ground around you!\n\r", ch);
    act("The ground becomes frozen and slick!",ch,NULL,NULL,TO_ROOM);

   //WAIT_STATE(ch, PULSE_TICK);
}

void spell_create_vacume(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    AFFECT_DATA af;
    ROOM_AFFECT_DATA raf;

    if ( is_affected( ch, sn ) )
    {
      send_to_char("You cannot create another vacume yet.\n\r", ch);
      return;
    }

    if ( is_affected_room( ch->in_room, sn ))
    {
        send_to_char("There already is a vacume here.\n\r",ch);
        return;
    }

    if (number_range(1,100) > ch->pcdata->learned[sn])
    {
	send_to_char("You attempt to pull all the air out of the room but fail.\n\r",ch);
	WAIT_STATE(ch, PULSE_TICK / 2);
    }

        init_affect(&af);
    af.where     = TO_AFFECTS;
        af.aftype    = AFT_SPELL;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = 60;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( ch, &af );

        init_affect_room(&raf);
    raf.where     = TO_ROOM_AFFECTS;
        raf.aftype    = AFT_SPELL;
    raf.type      = sn;
    raf.level     = ch->level;
    raf.duration  = 4;
    raf.modifier  = 0;
    raf.bitvector = AFF_ROOM_VACUME;
    affect_to_room( ch->in_room, &raf );


    send_to_char("You suck all the air out of this place and make a vacume!\n\r", ch);
    act("You feel a slight breeze and relize that there is no longer any air around you!",ch,NULL,NULL,TO_ROOM);
   WAIT_STATE(ch, PULSE_TICK);
}

void spell_hellfire(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    int dam;
    int dambase;
    dambase=320;
    if (!IS_NPC(ch) && check_spellcraft(ch,sn))
    {
		dambase=350;
		level+=2;
    }
    dam=dambase+number_percent();
    dam+=number_percent()*1.3;
    if (saves_spell(level+1,victim,DAM_FIRE))
	dam-=125;
    act("$N is consumed by hellish flames.",ch,0,victim,TO_NOTVICT);
    act("You are consumed by hellish flames.",ch,0,victim,TO_VICT);
    act("$N is consumed by hellish flames.",ch,0,victim,TO_CHAR);
    damage_old(ch,victim,dam,sn,DAM_FIRE,TRUE);
    if (is_affected(victim,sn))
        return;
        init_affect(&af);
    af.where = TO_AFFECTS;
        af.aftype    = AFT_SPELL;
    af.type = sn;
    af.modifier = 0;
    af.location = 0;
    af.bitvector = 0;
    af.duration = 4;
    af.level = level;
    affect_to_char(victim,&af);
    return;
}

void spell_pyrotechnics(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char buf[MAX_STRING_LENGTH];
    int dam;
    int dambase;
    dambase=150;
    if (!IS_NPC(ch) && check_spellcraft(ch,sn))
        {
		dambase=350;
		level+=2;
        }
        act("Sparks and flames begin to spew forth from $n's outstreched hands.",ch,0,0,TO_ROOM);
        act("The air fills with sparks and fire scorching everything in sight!",ch,0,0,TO_ROOM);
        for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
        {
	    vch_next = vch->next_in_room;
           if (is_same_group(vch,ch) || is_safe(ch,vch))
                continue;
	     if (ch->cabal>0 && vch->cabal>0 && ch->cabal==vch->cabal)
		    continue;
	     if (!IS_NPC(vch) && (ch->fighting == NULL || vch->fighting == NULL))
	     {
  		    sprintf(buf,"Die, %s, you sorcerous dog!",PERS(ch,vch));
		    do_myell(vch,buf);
	     }
	     dam=dambase+number_percent();
	     dam+=number_percent()*1.3;
           if (saves_spell(level+1,vch,DAM_FIRE))
                dam*=.66;
           damage_old(ch,vch,dam,sn,DAM_FIRE,TRUE);
        }
	  WAIT_STATE(ch,PULSE_VIOLENCE*2);
        return;
}

void spell_ball_lightning(int sn,int level,CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    act("$n hurls a ball of lightning at $N!",ch,NULL,victim,TO_NOTVICT);
    act("$n hurls a ball of lightning at you!",ch,NULL,victim,TO_VICT);
    act("You hurl a ball of lightning at $N.",ch,NULL,victim,TO_CHAR);
	WAIT_STATE(ch,PULSE_VIOLENCE*2);


    if (level <= 35)
    {
    if (check_spellcraft(ch,sn))
	dam = spellcraft_dam(level,11);
    else
	    dam = dice(level,11);
    }
    else if (level <= 40)
    {
    if (check_spellcraft(ch,sn))
	dam = spellcraft_dam(level,12);
    else
	dam = dice(level,12);
    }
    else if (level <= 48)
    {
    if (check_spellcraft(ch,sn))
	dam = spellcraft_dam(level,13);
    else
	dam = dice(level,13);
    }
    else
    {
    if (check_spellcraft(ch,sn))
	dam = spellcraft_dam(level, 13);
    else
	dam = dice(level,13);

	dam += number_range(0,level);
    }


    if (IS_NPC(victim))
	{
	if (level <= 35)
	dam += level;
	else if (level <=40)
	dam += (3*level);
	else
	dam += 4*level;
	}

    act("$n's ball lightning strikes $N and explodes into a dazzling shower of sparks.",ch,NULL,victim,TO_NOTVICT);
    act("$n's ball lightning strikes you and explodes into a dazzling shower of sparks.",ch,NULL,victim,TO_ROOM);


    if (saves_spell(level,victim, DAM_LIGHTNING) );
	dam /= 2;

    damage_old( ch, victim, dam, sn, DAM_LIGHTNING, TRUE);

    return;

}

void spell_fatigue( int sn, int level, CHAR_DATA *ch, void *vo, int target
)
{
   CHAR_DATA *victim = (CHAR_DATA *) vo;
   int dam = number_range(100,150);
   int saved = FALSE;
   if(saves_spell(level,victim,DAM_MENTAL))
	{ dam-=40; saved=TRUE; }
   if(!saved)
   {
	act("$n looks fatigued.",victim,0,0,TO_ROOM);
	act("You feel very fatigued.",victim,0,0,TO_CHAR);
	victim->move/=2;
   }

damage_old(ch,victim,dam,sn,DAM_MENTAL,TRUE);
}

void spell_cleanse(int sn,int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	//Difficulty modifier: 1 easy 2 medium 3 hard 4 very hard
	sn=skill_lookup("blindness");

	if(number_percent() < number_range(40,60))
	{
	if(is_affected(victim,sn) && cleansed(ch,victim,1,sn))
	{
                if(victim!=ch)
			act("$n is no longer blinded.",victim,0,ch,TO_ROOM);
                send_to_char(skill_table[sn].msg_off, victim );
                send_to_char("\n\r", victim);
		affect_strip(victim,sn);
	}
        sn=skill_lookup("poison");
        if(is_affected(victim,sn) && cleansed(ch,victim,2,sn))
        {
                if(victim!=ch)
	                act("$n looks less ill.",victim,0,ch,TO_ROOM);
                send_to_char(skill_table[sn].msg_off, victim );
                send_to_char("\n\r", victim);
                affect_strip(victim,sn);
        }
        sn=skill_lookup("weaken");
        if(is_affected(victim,sn) && cleansed(ch,victim,1,sn))
        {
		if(victim!=ch)
	                act("$n looks stronger.",victim,0,ch,TO_ROOM);
                send_to_char(skill_table[sn].msg_off, victim );
                send_to_char("\n\r", victim);
                affect_strip(victim,sn);
        }
        sn=skill_lookup("plague");
        if(is_affected(victim,sn) && cleansed(ch,victim,2,sn))
        {
                if(victim!=ch)
                        act("$n looks less diseased.",victim,0,ch,TO_ROOM);
                send_to_char(skill_table[sn].msg_off, victim );
                send_to_char("\n\r", victim);
                affect_strip(victim,sn);
        }
        sn=skill_lookup("slow");
        if(is_affected(victim,sn) && cleansed(ch,victim,2,sn))
        {
                if(victim!=ch)
                        act("$n starts moving at normal speed again.",victim,0,ch,TO_ROOM);
                send_to_char(skill_table[sn].msg_off, victim );
                send_to_char("\n\r", victim);
                affect_strip(victim,sn);
        }
        sn=skill_lookup("forget");
        if(is_affected(victim,sn) && cleansed(ch,victim,4,sn))
        {
                if(victim!=ch)
                        act("$n looks like $e's just had a rush of memory.",victim,0,ch,TO_ROOM);
                send_to_char(skill_table[sn].msg_off, victim );
                send_to_char("\n\r", victim);
                affect_strip(victim,sn);
        }
        sn=skill_lookup("rot");
        if(is_affected(victim,sn) && cleansed(ch,victim,3,sn))
        {
                if(victim!=ch)
                act("$n's body stops wasting away.",victim,0,ch,TO_ROOM);
                send_to_char(skill_table[sn].msg_off, victim );
                send_to_char("\n\r", victim);
                affect_strip(victim,sn);
        }
        sn=skill_lookup("garble");
        if(is_affected(victim,sn) && cleansed(ch,victim,3,sn))
        {
                if(victim!=ch)
                        act("$n's tongue uncontorts.",victim,0,ch,TO_ROOM);
                send_to_char(skill_table[sn].msg_off, victim );
                send_to_char("\n\r", victim);
                affect_strip(victim,sn);
        }
        sn=skill_lookup("confuse");
        if(is_affected(victim,sn) && cleansed(ch,victim,3,sn))
        {
                if(victim!=ch)
                        act("$n looks less confused.",victim,0,ch,TO_ROOM);
                send_to_char(skill_table[sn].msg_off, victim );
                send_to_char("\n\r", victim);
                affect_strip(victim,sn);
        }
        sn=skill_lookup("curse");
        if(is_affected(victim,sn) && cleansed(ch,victim,2,sn))
        {
                if(victim!=ch)
                        act("$n's curse wears off.",victim,0,ch,TO_ROOM);
                send_to_char(skill_table[sn].msg_off, victim );
                send_to_char("\n\r", victim);
                affect_strip(victim,sn);
        }
        sn=skill_lookup("incinerate");
        if(is_affected(victim,sn) && cleansed(ch,victim,4,sn))
        {
                if(victim!=ch)
                        act("$n stops burning.",victim,0,ch,TO_ROOM);
                send_to_char(skill_table[sn].msg_off, victim );
                send_to_char("\n\r", victim);
                affect_strip(victim,sn);
        }
	sn=skill_lookup("impale");
	if(is_affected(victim,sn) && cleansed(ch,victim,4,sn))
	{
		if(victim!=ch)
			act("$n stops bleeding.",victim,0,ch,TO_ROOM);
		send_to_char(skill_table[sn].msg_off, victim);
		send_to_char("\n\r",victim);
		affect_strip(victim,sn);
	}
	}
        check_improve(ch,skill_lookup("cleanse"),TRUE,1);
	return;
}

bool cleansed(CHAR_DATA *ch, CHAR_DATA *victim,int diffmodifier, int sn)
{
	int chance=0, snlevel;
	chance=100/diffmodifier;
	snlevel=get_affect_level(victim,sn);
	if(snlevel<0)
		return FALSE;
	chance-=(snlevel-ch->level)*2;
	if(number_percent()>chance)
		return FALSE;
return TRUE;
}
int get_affect_level(CHAR_DATA *ch, int sn)
{
	AFFECT_DATA *paf;
//Go through all affects on ch and return the level of the one that matches sn.
        for ( paf = ch->affected; paf != NULL; paf = paf->next )
		if(paf->type==sn)
			return paf->level;
return -1;
}

void spell_rot(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int recovery;

	if (is_affected(victim,sn)) {
		act("$N is already decomposing.",ch,NULL,victim,TO_CHAR);
		return;
	}

	if (saves_spell(level,victim,DAM_DISEASE) ||
	(IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD)))
	{
		act("$N's body resists the decomposition.",ch,NULL,victim,TO_CHAR);
		return;
	}

	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.aftype	= AFT_MALADY;
	af.type		= sn;
	af.level	= level;
	af.duration	= (get_curr_stat(victim,STAT_CON)*6)/5;
	af.location	= APPLY_CON;
	af.modifier	= -1;
	af.bitvector	= 0;
	af.owner	= ch;
	af.end_fun	= NULL;
	affect_to_char(victim,&af);

	recovery = skill_lookup("body reconstruction");

	if (is_affected(victim,recovery))
	{
		affect_strip(victim,recovery);
	}

	send_to_char("You feel your body begin to decompose.\n\r",victim);
	act("$n looks like $e is decomposing.",victim,NULL,NULL,TO_ROOM);
	return;
}

void spell_cure_deafness(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;

	if (!is_affected(victim,gsn_deafen))
	{
		if (victim == ch)
		send_to_char("You aren't deaf.\n\r",ch);
		else
		act("$N doesn't appear to be deafened.",ch,NULL,victim,TO_CHAR);
		return;
	}

	if (check_dispel(level,victim,gsn_deafen))
	{
		send_to_char("Your hearing returns!\n\r", victim );
		act("$n is no longer deafened.",victim,NULL,NULL,TO_ROOM);
	} else {
		send_to_char("You failed.\n\r",ch);
	}
	return;
}

void spell_remove_paralysis(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;

	if (!is_affected(victim,skill_lookup("hold person")))
	{
		if (victim == ch)
		send_to_char("You aren't paralyzed.\n\r",ch);
		else
		act("$N doesn't appear to be paralyzed.",ch,NULL,victim,TO_CHAR);
		return;
	}

	if (check_dispel(level,victim,skill_lookup("hold person")))
	{
		send_to_char("The feeling in your body returns!\n\r", victim );
		act("$n is no longer paraylzed.",victim,NULL,NULL,TO_ROOM);
	} else {
		send_to_char("You failed.\n\r",ch);
	}
	return;
}


void spell_awaken(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;

	if (IS_AWAKE(victim))
	{
		act("$N is already awake.", ch, NULL, victim, TO_CHAR);
		return;
	}

	if (is_affected(victim,skill_lookup("sleep")))
	{
		if (check_dispel(level,victim,skill_lookup("sleep")))
		{
			send_to_char( "You suddenly awaken.\n\r", victim );
			act("$n suddenly awakens.",victim,NULL,NULL,TO_ROOM);
			do_stand(victim,"");
		} else {
			send_to_char("You failed.\n\r",ch);
		}
		return;
	} else if (is_affected(victim,gsn_strangle)) {
		if (check_dispel(level,victim,gsn_strangle))
		{
			send_to_char( "You suddenly awaken.\n\r", victim );
			act("$n suddenly awakens.",victim,NULL,NULL,TO_ROOM);
			do_stand(victim,"");
		} else {
			send_to_char("You failed.\n\r",ch);
		}
		return;
	} else if (is_affected(victim,gsn_blackjack)) {
		if (check_dispel(level,victim,gsn_blackjack))
		{
			send_to_char( "You suddenly awaken.\n\r", victim );
			act("$n suddenly awakens.",victim,NULL,NULL,TO_ROOM);
			do_stand(victim,"");
		} else {
			send_to_char("You failed.\n\r",ch);
		}
		return;
	} else if (is_affected(victim,gsn_choke)) {
		if (check_dispel(level,victim,gsn_choke))
		{
			send_to_char( "You suddenly awaken.\n\r", victim );
			act("$n suddenly awakens.",victim,NULL,NULL,TO_ROOM);
			do_stand(victim,"");
		} else {
			send_to_char("You failed.\n\r",ch);
		}
		return;
	} else if (is_affected(victim,gsn_choke)) {
		if (check_dispel(level,victim,gsn_choke))
		{
			send_to_char( "You suddenly awaken.\n\r", victim );
			act("$n suddenly awakens.",victim,NULL,NULL,TO_ROOM);
			do_stand(victim,"");
		} else {
			send_to_char("You failed.\n\r",ch);
		}
		return;
	} else if (is_affected(victim,gsn_stun)) {
		if (check_dispel(level,victim,gsn_stun))
		{
			send_to_char( "You suddenly awaken.\n\r", victim );
			act("$n suddenly awakens.",victim,NULL,NULL,TO_ROOM);
			do_stand(victim,"");
		} else {
			send_to_char("You failed.\n\r",ch);
		}
		return;
	} else {
		send_to_char("They don't seem to be in deep sleep.\n\r",ch);
		return;
	}
	return;
}

void spell_resist_heat(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (is_affected(victim,sn))
	{
		if (victim == ch)
		send_to_char("You are already resisting heat.\n\r",ch);
		else
		act("$E is already resisting heat.",ch,NULL,victim,TO_CHAR);
		return;
	}

	send_to_char("You feel resistant to heat.\n\r",victim);
	act("$n is resistant to heat.",victim,NULL,NULL,TO_ROOM);
	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level/5;
	af.bitvector = RES_FIRE;
	af.location = 0;
	affect_to_char(victim,&af);
	return;
}

void spell_resist_cold(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (is_affected(victim,sn))
	{
		if (victim == ch)
		send_to_char("You are already resisting cold.\n\r",ch);
		else
		act("$E is already resisting cold.",ch,NULL,victim,TO_CHAR);
		return;
	}

	send_to_char("You feel resistant to cold.\n\r",victim);
	act("$n is resistant to cold.",victim,NULL,NULL,TO_ROOM);
	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level/5;
	af.bitvector = RES_COLD;
	af.location = 0;
	affect_to_char(victim,&af);
	return;
}

void spell_resist_lightning(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (is_affected(victim,sn))
	{
		if (victim == ch)
		send_to_char("You are already resisting lightning.\n\r",ch);
		else
		act("$E is already resisting lightning.",ch,NULL,victim,TO_CHAR);
		return;
	}

	send_to_char("You feel resistant to lightning.\n\r",victim);
	act("$n is resistant to lightning.",victim,NULL,NULL,TO_ROOM);
	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level/5;
	af.bitvector = RES_LIGHTNING;
	af.location = 0;
	affect_to_char(victim,&af);
	return;
}


void spell_resist_mental(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (is_affected(victim,sn))
	{
		if (victim == ch)
		send_to_char("You are already resisting mental attacks.\n\r",ch);
		else
		act("$E is already resisting mental attacks.",ch,NULL,victim,TO_CHAR);
		return;
	}

	send_to_char("You feel resistant to mental attacks.\n\r",victim);
	act("$n is resistant to mental attacks.",victim,NULL,NULL,TO_ROOM);
	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level/5;
	af.bitvector = RES_MENTAL;
	af.location = 0;
	affect_to_char(victim,&af);
	return;
}


void spell_resist_acid(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (is_affected(victim,sn))
	{
		if (victim == ch)
		send_to_char("You are already resisting acid.\n\r",ch);
		else
		act("$E is already resisting acid.",ch,NULL,victim,TO_CHAR);
		return;
	}

	send_to_char("You feel resistant to acid.\n\r",victim);
	act("$n is resistant to acid.",victim,NULL,NULL,TO_ROOM);
	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level/5;
	af.bitvector = RES_ACID;
	af.location = 0;
	affect_to_char(victim,&af);
	return;
}

void spell_resist_negative(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (is_affected(victim,sn))
	{
		if (victim == ch)
		send_to_char("You are already resisting negative damage.\n\r",ch);
		else
		act("$E is already resisting negative damage.",ch,NULL,victim,TO_CHAR);
		return;
	}

	send_to_char("You feel resistant to negative damage.\n\r",victim);
	act("$n is resistant to negative damage.",victim,NULL,NULL,TO_ROOM);
	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level/5;
	af.bitvector = RES_NEGATIVE;
	af.location = 0;
	affect_to_char(victim,&af);
	return;
}

void spell_resist_paralysis(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (is_affected(victim,sn))
	{
		if (victim == ch)
		send_to_char("You are already resisting paralysis.\n\r",ch);
		else
		act("$E is already resisting paralysis.",ch,NULL,victim,TO_CHAR);
		return;
	}

	send_to_char("You feel resistant to paralysis.\n\r",victim);
	act("$n is resistant to paralysis.",victim,NULL,NULL,TO_ROOM);
	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level/5;
	af.bitvector = RES_MAGIC;
	af.location = 0;
	affect_to_char(victim,&af);
	return;
}

void spell_resist_weapon(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (is_affected(victim,sn))
	{
		if (victim == ch)
		send_to_char("You are already resisting weapons.\n\r",ch);
		else
		act("$E is already resisting weapons.",ch,NULL,victim,TO_CHAR);
		return;
	}

	send_to_char("You feel resistant to weapons.\n\r",victim);
	act("$n is resistant to weapons.",victim,NULL,NULL,TO_ROOM);
	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level/5;
	af.bitvector = RES_WEAPON|RES_BASH|RES_SLASH|RES_PIERCE;
	af.location = 0;
	affect_to_char(victim,&af);
	return;
}

/* Psi Blast for Illithids -- Ceial

void spell_psi_blast(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int dam;

	if (is_affected(ch,gsn_psi_blast))
	{
		send_to_char("Your mind hasn't regained strength enough to assault minds.",ch);
		return;
	}

	if (saves_spell(level-4,victim,DAM_NEGATIVE) )
	{
		act("$n reels but is not over come by your mind.",victim,NULL,NULL,TO_ROOM);
	 	act("Your mind is brutally assaulted, but you resist.",victim,NULL,NULL,TO_CHAR);
	    WAIT_STATE(ch,PULSE_VIOLENCE*1);
		return;
    }


	if (check_spellcraft(ch,sn))
		dam = spellcraft_dam(6 + level / 1.5,9);
	    else
	    dam = dice(6 + level / 2, 8);
	    if ( saves_spell( level, victim,DAM_MENTAL) )
		dam /= 2;
	    damage_old( ch, victim, dam, sn, DAM_MENTAL ,TRUE);
	    act("$N reels as their mind is brutally assaulted!",ch,NULL,victim,TO_NOTVICT);
		act("$N reels from your mental blast!",ch,NULL,victim,TO_CHAR);
    	send_to_char("Your reels from the brutal assault.",victim);
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype    = AFT_SPELL;
		af.type = gsn_psi_blast;
		af.bitvector = 0;
		af.level = level;
		af.duration = 10;
        affect_to_char(ch,&af);
		WAIT_STATE(victim,PULSE_VIOLENCE*3);
  		return;
}*/

void spell_group_teleport(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	ROOM_INDEX_DATA *old_room;
	CHAR_DATA *group;
	CHAR_DATA *g_next;

	old_room = ch->in_room;

	spell_teleport(skill_lookup("teleport"), level, ch, ch, (int)NULL);

	if (ch->in_room != old_room) {
		for (group = old_room->people; group != NULL; group = g_next)
		{
			g_next = group->next_in_room;
			if (!is_same_group(group,ch) || (group->fighting != NULL) || group == ch)
				continue;
			if (group == ch )
				continue;
			send_to_char("You have been teleported!\n\r",group);
			act("$n vanishes!", group, NULL, NULL, TO_ROOM );
			char_from_room(group);
			char_to_room(group, ch->in_room);
			act("$n slowly fades into existence.", group, NULL, NULL, TO_ROOM);
			do_look(group, "auto");
		}
	}
	return;
}

void spell_strength( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
   AFFECT_DATA af;
   if(is_affected(ch,sn))
   {
	send_to_char("You are as sure of your faith as you will ever get.\n\r",ch);
	return;
   }
   init_affect(&af);
   af.where = TO_RESIST;
   af.aftype = AFT_COMMUNE;
   af.duration = ch->level*2;
   af.location = 0;
   af.modifier = 0;
   af.bitvector = RES_HOLY;
   af.level = level;
   af.type = sn;
   affect_to_char(ch,&af);
   send_to_char("You fast for a period of time, building up your absolute faith in the strength of your Deity.\n\r",ch);
   act("$n meditates for a period of time, building up $s faith in $s Deity.",ch,0,0,TO_ROOM);
}

void spell_soften( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
	if(is_affected(victim,sn))
	{
	send_to_char("They are already softened.\n\r",ch);
	ch->mana-=30;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
	}
	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.type		= sn;
	af.level	= level;
	af.location	= APPLY_NONE;
	af.aftype	= AFT_SPELL;
	af.modifier	= 0;
	af.duration	= 1;
	af.bitvector	= 0;
	affect_to_char(victim,&af);
	act("$n looks frail.",victim,0,0,TO_ROOM);
	send_to_char("You feel more frail.\n\r",victim);
}

void spell_duo_dimension(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    AFFECT_DATA af;
    int sn_fog, sn_fire, sn_dew;
    sn_fog = skill_lookup("faerie fog");
    sn_fire = skill_lookup("faerie fire");
    sn_dew = skill_lookup("dew");

    if (is_affected(ch,sn_fog) || is_affected(ch,sn_fire))
    {
    send_to_char("You are glowing too much for duo dimension.\n\r",ch);
    return;
    }

    if (is_affected(ch,sn_dew))
	{
	send_to_char("You cannot step out of the dimension while covered in dew.\n\r",ch);
	return;
	}

    act("You blink and $n is gone.",ch,0,0,TO_ROOM);
    send_to_char("You lose a dimension and disappear from view.\n\r",ch);

    init_affect(&af);
    af.where 	= TO_AFFECTS;
    af.aftype	= AFT_SPELL;
    af.type	= gsn_duo;
    af.level	= level;
    af.modifier = 0;
    af.bitvector = 0;
    af.duration = (level/5);
    af.location = 0;
    affect_to_char(ch,&af);
    return;
}

bool fumble_obj( CHAR_DATA *victim, OBJ_DATA *obj_drop, int level, bool drop )
{
    if ( drop )
    {
        if ( !can_drop_obj( victim, obj_drop ) )
            return FALSE;
    }
    else
    {
        if ( IS_OBJ_STAT( obj_drop, ITEM_NOREMOVE ) )
            return FALSE;
    }

    if ( saves_spell( level, victim, DAM_NONE ) )
    {
        act( "You nearly $T $p, but manage to keep your grip.",
            victim, obj_drop, drop ? "drop" : "lose hold of", TO_CHAR );
        act( "$n nearly $T $p, but manages to keep $s grip.",
            victim, obj_drop, drop ? "drops" : "loses hold of", TO_ROOM );
        return FALSE;
    }

    if ( drop )
    {
        obj_from_char( obj_drop );
        obj_to_room( obj_drop, victim->in_room );
    }
    else
    {
        unequip_char( victim, obj_drop );
    }

    act( "You fumble and $T $p!",
        victim, obj_drop, drop ? "drop" : "lose hold of", TO_CHAR );
    act( "$n fumbles and $T $p!",
        victim, obj_drop, drop ? "drops" : "loses hold of", TO_ROOM );
    return TRUE;
}

void spell_fumble( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA  *victim = (CHAR_DATA *) vo;
    OBJ_DATA   *obj;
    OBJ_DATA   *obj_drop;
    int         carry;
    int         check;
    int         count;
    int         drop;
    bool        fumbled = FALSE;

    if ( !IS_AWAKE( victim ) )
        return;

    carry = 0;
    for ( obj = victim->carrying; obj; obj = obj->next_content )
        carry++;

    drop = carry - can_carry_n( victim ) + 5;

    for ( check = 0; check < drop; check++ )
    {
        obj_drop = NULL;
        count = 0;

        for ( obj = victim->carrying; obj; obj = obj->next_content )
        {
            if ( obj->wear_loc == WEAR_NONE
            && number_range( 0, count++ ) == 0 )
                obj_drop = obj;
        }

        if ( !obj_drop )
            break;

        fumbled = fumble_obj( victim, obj_drop, level, TRUE ) || fumbled;
    }

    if ( ( obj_drop = get_eq_char( victim, WEAR_HOLD ) ) )
        fumbled = fumble_obj( victim, obj_drop, level, FALSE ) || fumbled;

    if ( ( obj_drop = get_eq_char( victim, WEAR_LIGHT ) ) )
        fumbled = fumble_obj( victim, obj_drop, level, FALSE ) || fumbled;

     if ( ( obj_drop = get_eq_char( victim, WEAR_WIELD ) ) )
        fumbled = fumble_obj( victim, obj_drop, level, FALSE ) || fumbled;

    if ( !fumbled )
    {
        send_to_char( "You stumble momentarily, but quickly recover.\n\r",
            victim );
        act( "$n stumbles momentarily, but quickly recovers.",
            victim, NULL, NULL, TO_ROOM );
    }

    return;
}

void spell_knock( int sn, int level, CHAR_DATA *ch, void *vo)
{

   char pick_obj[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   int door;

   obj = NULL;

   target_name = one_argument(target_name, pick_obj);

   if (pick_obj == '\0')
   {
     send_to_char("What would you like to try to unlock?\n\r",ch);
     return;
   }

   if ( (obj = get_obj_here( ch,pick_obj)) != NULL)
   {
       if (obj->item_type != ITEM_CONTAINER)
       {
          send_to_char("That's not a container.\n\r",ch);
          return;
       }
       if (!IS_SET(obj->value[1], CONT_CLOSED))
       {
          send_to_char( "It's not closed.\n\r",ch);
          return;
       }
       if ( obj->value[2] < 0)
       {
          send_to_char( "It can't be unlocked.\n\r",ch);
          return;
       }
       if ( !IS_SET(obj->value[1], CONT_LOCKED))
       {
          send_to_char( "It's already unlocked.\n\r",ch);
          return;
       }
       if ( IS_SET(obj->value[1], CONT_PICKPROOF))
       {
          send_to_char( "Spell failed.\n\r",ch);
          return;
       }
       REMOVE_BIT(obj->value[1], CONT_LOCKED);
       send_to_char( "*Click*\n\r",ch);
       return;
     }

     if ( ( door = find_door( ch, pick_obj) ) >= 0)
     {
       ROOM_INDEX_DATA *to_room;
       EXIT_DATA *pexit;
       EXIT_DATA *pexit_rev;
       pexit = ch->in_room->exit[door];
       if ( !IS_SET(pexit->exit_info, EX_CLOSED))
       {
          send_to_char( "It's not closed.\n\r",ch);
          return;
       }
       if ( pexit->key < 0 && !IS_IMMORTAL(ch))
       {
          send_to_char("That lock can't be opened.\n\r",ch);
          return;
       }
       if ( !IS_SET(pexit->exit_info, EX_LOCKED))
       {
          send_to_char("It's already unlocked.\n\r",ch);
          return;
       }
       if ( IS_SET(pexit->exit_info, EX_PICKPROOF) && !IS_IMMORTAL(ch))
       {
          send_to_char("Spell failed.\n\r",ch);
          return;
       }
       REMOVE_BIT(pexit->exit_info, EX_LOCKED);
       send_to_char("*Click*\n\r",ch);

       /* pick lock on other side of door as well */
       if ( (  to_room = pexit->u1.to_room ) != NULL
       &&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
       &&   pexit_rev->u1.to_room == ch->in_room)
       {
            REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED);
       }
     }
     return;
}

void spell_protection(int sn,int level,CHAR_DATA *ch,void *vo, int target)
{
    AFFECT_DATA af;

    if(is_affected(ch,gsn_protection))
          return send_to_char("You are already protected.\n\r",ch);

    init_affect(&af);
    af.where     = TO_AFFECTS;
    af.aftype    = get_spell_aftype(ch);
    af.type      = gsn_protection;
    af.level     = level;
    af.duration  = 24;
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( ch, &af );
    send_to_char( "You feel protected.\n\r", ch);
    return;
}

void spell_prismatic_spray( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
  AFFECT_DATA af;
  CHAR_DATA *victim = (CHAR_DATA *) vo;
  int i;
  int type = 0;

  if (ch->fighting)
  {
   int dam = dice(level, 6 + (ch->level > 30) + (ch->level > 40));
   victim = ch->fighting;

    if (check_spellcraft(ch,sn))
        dam = spellcraft_dam(level, 13);
    else
        dam = dice(level,13);

    if ( saves_spell( level, victim, DAM_OTHER ) )
        dam /= 2;
    i = number_range(1, 8);
    switch(i)
    {
     case 1: type = DAM_OTHER;
  send_to_char("A rainbow of colors shoots out from your hand.\n\r",ch);
  act("A rainbow of colors shoots out from $n's hand.",ch,NULL,victim,TO_ROOM);
  act("A red beam of light hits $N!",ch,NULL,victim,TO_ROOM);
  act("A red beam of light hits you!",ch,NULL,victim,TO_VICT);
  act("A red beam of light hits $N!",ch,NULL,victim,TO_CHAR);
  damage_old(ch, victim, number_range(100,135), sn, type, TRUE);
      break;
      case 2: type = DAM_OTHER;
  send_to_char("A rainbow of colors shoots out from your hand.\n\r",ch);
  act("A rainbow of colors shoots out from $n's hand.", ch, NULL,victim,TO_ROOM);
  act("A orange beam of light hits $N!",ch,NULL,victim,TO_ROOM);
  act("A orange beam of light hits you!",ch,NULL,victim,TO_VICT);
  act("A orange beam of light hits $N!",ch,NULL,victim,TO_CHAR);
  damage_old(ch, victim, number_range(160,250), sn, type, TRUE);
      break;
     case 3: type = DAM_OTHER;
  send_to_char("A rainbow of colors shoots out from your hand.\n\r",ch);
  act("A rainbow of colors shoots out from $n's hand.", ch, NULL,victim,TO_ROOM);
  act("A yellow beam of light hits $N!",ch,NULL,victim,TO_ROOM);
  act("A yellow beam of light hits you!",ch,NULL,victim,TO_VICT);
  act("A yellow beam of light hits $N!",ch,NULL,victim,TO_CHAR);
  damage_old(ch, victim, number_range(250,330), sn, type, TRUE);
      break;
     case 4: type = DAM_OTHER;
  send_to_char("A rainbow of colors shoots out from your hand.\n\r",ch);
  act("A rainbow of colors shoots out from $n's hand.", ch, NULL,victim,TO_ROOM);
  act("A green beam of light hits $N!",ch,NULL,victim,TO_ROOM);
  act("A green beam of light hits you!",ch,NULL,victim,TO_VICT);
  act("A green beam of light hits $N!",ch,NULL,victim,TO_CHAR);
  spell_poison( gsn_poison, ch->level, ch, victim,TARGET_CHAR);
  damage_old(ch, victim, dam, sn, type, TRUE);
      break;
     case 5: type = DAM_OTHER;
  send_to_char("A rainbow of colors shoots out from your hand.\n\r",ch);
  act("A rainbow of colors shoots out from $n's hand.", ch, NULL,victim,TO_ROOM);
  act("A white beam of light hits $N!",ch,NULL,victim,TO_ROOM);
  act("A white beam of light hits you!",ch,NULL,victim,TO_VICT);
  act("A white beam of light hits $N!",ch,NULL,victim,TO_CHAR);
  spell_blindness( gsn_blindness, ch->level, ch, victim,TARGET_CHAR);
  damage_old(ch, victim, dam, sn, type, TRUE);
      break;
     case 6: type = DAM_OTHER;
  send_to_char("A rainbow of colors shoots out from your hand.\n\r",ch);
  act("A rainbow of colors shoots out from $n's hand.", ch, NULL,victim,TO_ROOM);
  act("A blue beam of light hits $N!",ch,NULL,victim,TO_ROOM);
  act("A blue beam of light hits you!",ch,NULL,victim,TO_VICT);
  act("A blue beam of light hits $N!",ch,NULL,victim,TO_CHAR);

  init_affect(&af);
  af.where     = TO_AFFECTS;
  af.type      = sn;
  af.aftype    = AFT_SPELL;
  af.level     = ch->level;
  af.location  = APPLY_SAVING_PETRI;
  af.modifier  = 60;
  affect_to_char( victim,&af );
  damage_old(ch, victim, dam, sn, type, TRUE);
      break;
     case 7: type = DAM_OTHER;
  send_to_char("A rainbow of colors shoots out from your hand.\n\r",ch);
  act("A rainbow of colors shoots out from $n's hand.", ch, NULL,victim,TO_ROOM);
  act("A indigo beam of light hits $N!",ch,NULL,victim,TO_ROOM);
  act("A indigo beam of light hits you!",ch,NULL,victim,TO_VICT);
  act("A indigo beam of light hits $N!",ch,NULL,victim,TO_CHAR);
  damage_old(ch, victim, number_range(330, 380), sn, type, TRUE);
      break;
     case 8: type = DAM_OTHER;
  send_to_char("A rainbow of colors shoots out from your hand.\n\r",ch);
  act("A rainbow of colors shoots out from $n's hand.", ch, NULL,victim,TO_ROOM);
  act("A violet beam of light hits $N!",ch,NULL,victim,TO_ROOM);
  act("A violet beam of light hits you!",ch,NULL,victim,TO_VICT);
  act("A violet beam of light hits $N!",ch,NULL,victim,TO_CHAR);
  damage_old(ch, victim, number_range(400, 600), sn, type, TRUE);
      break;


  }
 return;
}
}

void spell_haunting(int sn,int level,CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
	int dam;

    if (victim == ch)
    {
		send_to_char("Now thats just stupid talk.\n\r",ch);
		return;
	}

    dam = dice( level/3, 7 );

    damage_old( ch, victim, dam, gsn_haunting,DAM_MENTAL,TRUE);
    act("$n narrows $s eyes and plagues $N's mind with haunting images!",ch,NULL,victim,TO_ROOM);
	act("$n plagues your mind with haunting images!",ch,NULL,victim,TO_VICT);
  	act("You plague $N's mind with haunting images!",ch,NULL,victim,TO_CHAR);
    if (!is_affected(victim,gsn_haunting))
    {
		init_affect(&af);
		af.where     = TO_AFFECTS;
		af.aftype    = AFT_MALADY;
		af.type      = gsn_haunting;
		af.level     = level;
		af.duration  = 10;
		af.location  = 0;
		af.modifier  = 0;
    	af.bitvector = 0;
		affect_to_char( victim,&af );
	}
    return;
}

