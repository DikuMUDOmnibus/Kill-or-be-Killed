#include "include.h"


int     spellcraft_dam  args( ( int num, int dice) );
bool    check_spellcraft args( (CHAR_DATA *ch, int sn) );
char 	*target_name;

void spell_transcendence(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (cabal_down(ch,CABAL_COVEN))
	return;

	if (is_affected(ch,sn))
	return;

	if (IS_NPC(ch))
	return;

	init_affect(&af);
	af.type 		= sn;
	af.aftype       = AFT_SPELL;
	af.duration 	= 24;
	af.modifier 	= ch->max_mana / 2;
	af.bitvector 	= 0;
	af.where 		= TO_AFFECTS;
	af.location 	= APPLY_HIT;
	affect_to_char(ch, &af);
	af.location		= APPLY_MANA;
	af.modifier		= ch->max_hit / 2;
	affect_to_char(ch, &af);
	send_to_char("You focus as your mind and body become one.\n\r", ch );
	return;
}

void spell_mana_rip(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam;

	if (cabal_down(ch,CABAL_COVEN))
	return;

	if (victim == ch)
	{
		send_to_char("You can't drain your mind with your mind.\n\r", ch );
		return;
	}

	if (IS_NPC(victim))
	{
		dam = number_range(100,150);

		act("$n narrows $s eyes at $N and drains their mind mercilessly.",ch,0,victim,TO_NOTVICT);
		act("$n narrows $s eyes at you and drains your mind mercilessly.",ch,0,victim,TO_VICT);
	   	act("You lash out at $N with your mind and drain their energy.",ch,0,victim,  TO_CHAR);
	   	damage_old(ch,victim,dam,sn,DAM_MENTAL,TRUE);
	   	return;
	}

	dam = number_range(50,60);

	if (saves_spell(level,victim,DAM_MENTAL))
	{
		act("$n shivers slightly but isn't drained.",victim,0,0,TO_ROOM);
	    damage_old(ch,victim,dam,sn,DAM_MENTAL,TRUE);
	    return;
    }



    act("$n narrows $s eyes at $N and drains their mind mercilessly.",ch,0,victim,TO_NOTVICT);
    act("$n narrows $s eyes at you and drains your mind mercilessly.",ch,0,victim,TO_VICT);
    act("You lash out at $N with your mind and drain their energy.",ch,0,victim,  TO_CHAR);
    damage_old(ch,victim,dam,sn,DAM_MENTAL,TRUE);

    if ( (victim->mana > victim->max_mana)
    || (victim->mana > dam) )
    {
		victim->mana -= dam;
    	ch->mana += dam;
	}else{
		act("$N's mind has been completely drained.",ch,0,victim, TO_CHAR);
	}

    return;
}

void spell_rift(int sn, int level,CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *victim;
    CHAR_DATA *group;
    CHAR_DATA *arrival;
    CHAR_DATA *g_next;
    ROOM_INDEX_DATA *VENUEPORT;
    CHAR_DATA *last_to_venue;
    int numb;
    char buf[MAX_STRING_LENGTH];

    if (cabal_down(ch,CABAL_COVEN))
	return;

    last_to_venue = ch;
    victim = get_char_world( ch, target_name );

    if ( victim == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   !can_see_room(ch,victim->in_room)
    ||   (victim->in_room->cabal != 0 && victim->in_room->cabal != CABAL_COVEN)
    ||   ch->in_room->guild != 0
    ||   victim->in_room->guild != 0
    ||   (IS_NPC(victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   victim->level >= level + 9
    ||   (saves_spell( level, victim,DAM_OTHER) )
    ||   (!IS_NPC(victim) && !can_pk(ch,victim) && IS_SET(victim->act,PLR_NOSUMMON)))
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }

    if (victim->in_room == ch->in_room)
    {
		send_to_char("You are standing right next to them.\n\r",ch);
		return;
    }

    VENUEPORT = get_room_index(ROOM_VNUM_VENUEPORT);
    numb = 1;
    for (group = ch->in_room->people; group != NULL; group = g_next)
    {
		g_next = group->next_in_room;
		if (!is_same_group(group,ch) || (group->fighting != NULL) )
			continue;
		if (group == ch )
			continue;
		numb++;
		affect_strip(group,gsn_ambush);
		sprintf(buf, "%s drains energy from the vortex and violently creates a rift.\n\r",
		ch->name );
		send_to_char( buf, group );
		sprintf(buf, "%s vanishes into the rift.\n\r", ch->name );
		send_to_char( buf, group );
		sprintf(buf, "You follow %s into the rift.\n\r", ch->name);
		send_to_char( buf, group );
   	    send_to_char("\n\r",group);
		char_from_room(group);
		char_to_room(group,victim->in_room);
        act("$n steps out of the rift!",group,NULL,NULL,TO_ROOM);
		char_from_room(group);
		char_to_room(group, VENUEPORT);
		last_to_venue = group;
		if (is_affected(group,gsn_earthfade))
		{
			affect_strip(group, gsn_earthfade);
		}
    }

   act("A dark rift opens up!",ch,NULL,NULL,TO_ROOM);
   act("$n steps into the rift and is gone.",ch,NULL,NULL,TO_ROOM);
   send_to_char("You drain energy from the vortex and violently create a rift.\n\r", ch );
   send_to_char("You enter into the rift.\n\r", ch );
   send_to_char("\n\r",ch);
   char_from_room(ch);
   char_to_room(ch,victim->in_room);
   act("A dark rift opens up.",ch,NULL,NULL,TO_ROOM);
   act("$n steps out of the rift!",ch,NULL,NULL,TO_ROOM);

   if (last_to_venue == ch)
   {
		do_look(ch,"auto");
		return;
   }

   for (arrival = last_to_venue->in_room->people; arrival != NULL; arrival = g_next)
   {
	g_next = arrival->next_in_room;
	if (arrival == ch)
		continue;
	if ((is_same_group(arrival,ch)) && (arrival != ch) )
	{
	char_from_room(arrival);
	char_to_room(arrival,ch->in_room);
	}
   }

   for (arrival = ch->in_room->people; arrival != NULL; arrival = arrival->next_in_room)
    {
	do_look(arrival,"auto");
	if (--numb == 0)
	break;
    }
return;
}

void spell_shifting_colors(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	OBJ_DATA *obj, *obj_next;
	int dam;

	if (cabal_down(ch,CABAL_COVEN))
	return;

	if (check_spellcraft(ch,sn))
		dam = spellcraft_dam( level, 9 );
	else
    	dam = dice( level, 9 );

	act("$n streams forth a brilliant beam of light at $N.", ch,0,victim, TO_NOTVICT);
	act("$n streams forth a brilliant beam of light at you.", ch,0,victim,TO_VICT);
	act("You focus a stream of brilliant light at $N.", ch,0,victim,      TO_CHAR);

	switch(number_range(1,6) )
	{

		case 1:

		act("A blazing beam of white light strikes $N.",ch,0,victim,  TO_NOTVICT);
		act("A blazing beam of white light strikes you.",ch,0,victim, TO_VICT);
		act("A blazing beam of white light strikes $N.",ch,0,victim,  TO_CHAR);
    	damage_old(ch,victim,dam,sn,DAM_LIGHT,TRUE);
    	if (number_percent() < 2 * level);
		    spell_blindness(gsn_blindness,level,ch,(void *) victim,TARGET_CHAR);
		break;

		case 2:

		act("An illuminating violet light strikes $N.",ch,0,victim,  TO_NOTVICT);
		act("An illuminating violet light strikes you.",ch,0,victim, TO_VICT);
		act("An illuminating violet light strikes $N.",ch,0,victim,  TO_CHAR);
		damage_old(ch,victim,dam,sn,DAM_FIRE,TRUE);

		if (!IS_AFFECTED(victim,AFF_BLIND)
		&&  !saves_spell(level / 4 + dam / 20, victim,DAM_FIRE))
		{
			AFFECT_DATA af;
		    act("$n eyes are seared by the violet light!",victim,NULL,NULL,TO_ROOM);
		    act("Your eyes are seared by the violet light...you can't see a thing!",
			victim,NULL,NULL,TO_CHAR);
		    init_affect(&af);
		    af.where        = TO_AFFECTS;
		    af.aftype    	= AFT_SPELL;
		    af.type         = skill_lookup("shifting colors");
		    af.level        = level;
		    af.duration     = number_range(0,level/10);
		    af.location     = APPLY_HITROLL;
		    af.modifier     = -4;
		    af.bitvector    = AFF_BLIND;
		    affect_to_char(victim,&af);
		}

		for (obj = victim->carrying; obj != NULL; obj = obj_next)
		{
		    obj_next = obj->next_content;
		    fire_effect(obj,level,dam,TARGET_OBJ);
		}

		break;

		case 3:

		act("A massive beam of pure red light strikes $N.",ch,0,victim,  TO_NOTVICT);
		act("A massive beam of pure red light strikes you.",ch,0,victim, TO_VICT);
		act("A massive beam of pure red light strikes $N.",ch,0,victim,  TO_CHAR);
		damage_old(ch,victim,dam,sn,DAM_MENTAL,TRUE);

		if (number_percent() > 25)
		{
			act("$N is knocked to the ground by the blast!",ch,NULL,victim,TO_ROOM);
		    send_to_char("You are knocked to the ground by the blast!\n\r",victim);
			WAIT_STATE(victim, PULSE_VIOLENCE * 2);
		}

		break;

		case 4:

		act("A cold beam of blue light strikes $N.",ch,0,victim,  TO_NOTVICT);
		act("A cold beam of blue light strikes you.",ch,0,victim, TO_VICT);
		act("A cold beam of blue light strikes $N.",ch,0,victim,  TO_CHAR);
		damage_old(ch,victim,dam,sn,DAM_COLD,TRUE);

		for (obj = victim->carrying; obj != NULL; obj = obj_next)
		{
		    obj_next = obj->next_content;
		    cold_effect(obj,level,dam,TARGET_OBJ);
		}

		break;

		case 5:

		act("A blazing beam of green light strikes $N.",ch,0,victim,  TO_NOTVICT);
		act("A blazing beam of green light strikes you.",ch,0,victim, TO_VICT);
		act("A blazing beam of green light strikes $N.",ch,0,victim,  TO_CHAR);
		damage_old(ch,victim,dam,sn,DAM_LIGHTNING,TRUE);

		if (saves_spell(level,victim,DAM_LIGHTNING))
		{
			shock_effect(victim,level/2,dam/4,TARGET_CHAR);
		}

		break;

		case 6:

		act("A dark beam of black light strikes $N.",ch,0,victim,  TO_NOTVICT);
		act("A dark beam of black light strikes you.",ch,0,victim, TO_VICT);
		act("A dark beam of black light strikes $N.",ch,0,victim,  TO_CHAR);
		damage_old(ch,victim,dam,sn,DAM_NEGATIVE,TRUE);

		if (!IS_AFFECTED(victim,gsn_curse)
		&&  !saves_spell(level / 4 + dam / 20, victim,DAM_NEGATIVE))
		{
			AFFECT_DATA af;
		    act("$n is consumed by the dark beam of light!",victim,NULL,NULL,TO_ROOM);
		    act("You are consumed by the dark beam of light and feel uncomfortable!",
			victim,NULL,NULL,TO_CHAR);
		    init_affect(&af);
		    af.where        = TO_AFFECTS;
		    af.aftype    	= AFT_SPELL;
		    af.type         = skill_lookup("shifting colors");
		    af.level        = level;
		    af.duration     = number_range(0,level/8);
		    af.location     = APPLY_HITROLL;
		    af.modifier     = -ch->level / 8;
		    af.bitvector    = AFF_CURSE;
		    affect_to_char(victim,&af);
		    af.location		= APPLY_SAVES;
		    af.modifier		= ch->level / 8;
		}

		break;
	}

	act("The beam of light flashes and fades.", ch,0,NULL, TO_ROOM);
	send_to_char("The beam of light flashes and fades.\n\r", ch );
	return;
}























