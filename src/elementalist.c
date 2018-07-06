#include "include.h"


int     spellcraft_dam  args( ( int num, int dice) );
bool    check_spellcraft args( (CHAR_DATA *ch, int sn) );


#define SUBCLASS_NONE 	0
#define SUBCLASS_AIR  	1
#define SUBCLASS_EARTH	2
#define SUBCLASS_ICE	3
#define SUBCLASS_FIRE	4


void do_element(CHAR_DATA *ch,char *argument)
{

	char arg[MAX_INPUT_LENGTH];

    one_argument(argument,arg);


	if (IS_NPC(ch))
	return;

    if (ch->class != CLASS_ELEMENTALIST)
    {
		send_to_char("Huh?\n\r",ch);
		return;
	}

	if (ch->pcdata->element != SUBCLASS_NONE)
	{
		send_to_char("You have chosen an element to specialize in.\n\r",ch);
		return;
	}

	if (!strcmp(arg,"air"))
	{
		group_add(ch,"aeromancer",FALSE);
		ch->pcdata->element = SUBCLASS_AIR;
		send_to_char("Your now specialized in the element of air.\n\r",ch);
	}
	else if (!strcmp(arg,"earth"))
	{
		group_add(ch,"geomancer",FALSE);
		ch->pcdata->element = SUBCLASS_EARTH;
		send_to_char("Your now specialized in the element of earth.\n\r",ch);
	}
	else
	{
		send_to_char("You must choose either:\n\r",ch);
		send_to_char("\n\rAir, Water, Fire or Earth.\n\r",ch);
	}

	return;

}

// Air Elementalists Section

void spell_air_shield(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (is_affected(ch,sn))
	{
	    send_to_char("You are already surrounded by a current of air.\n\r",ch);
	    return;
	}

	init_affect(&af);
	af.where	 = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
	af.type      = sn;
	af.level	 = ch->level;
	af.duration  = ch->level;
	af.modifier  = -ch->level;
	af.location  = APPLY_AC;
	af.bitvector = 0;
	affect_to_char( ch, &af );

	act("$n shields $s self with a current of air!", ch,NULL,NULL,TO_ROOM);
	act("You shield yourself with a current of air!", ch,NULL,NULL,TO_CHAR);
}

void spell_turbulence(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam, count, save_num;

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


	save_num = 0;
    for (count = 0; count < 5; count++)
    {
        if (saves_spell(level,victim,DAM_BASH) )
            save_num++;
    }


	send_to_char("You summon forth a turbulent mass of air.\n\r",ch );
	act("A swirling mass of turbulent air strikes you!",ch,NULL,victim,TO_VICT);
	act("A swirling mass of turbulent air strikes $N!",ch,NULL,victim,TO_CHAR);

	if (save_num == 0)
	{
		act("You are sent flying from the blunt force of the strike!",ch,NULL,victim,TO_VICT);
 		act("$N is sent flying from the blunt force of the strike!",ch,NULL,victim,TO_CHAR);
		damage_old( ch, victim, dam, sn, DAM_BASH, TRUE);
		WAIT_STATE(ch,PULSE_VIOLENCE);
		WAIT_STATE(victim,2*PULSE_VIOLENCE);
		return;
	}

		dam /= save_num * 2;
  		damage_old( ch, victim, dam, sn, DAM_BASH, TRUE);
		return;
}

void spell_mass_fly(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
   CHAR_DATA *gch;
   AFFECT_DATA af;


   for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
   {
   		if ( !is_same_group( gch, ch ) || IS_AFFECTED(gch, AFF_FLYING) )
   	    continue;

   	    act( "$n slowly rises off the ground.", gch, NULL, NULL, TO_ROOM );
		send_to_char( "You slowly rise off the ground.\n\r", gch );


   		init_affect(&af);
	    af.where     = TO_AFFECTS;
		af.aftype	 = get_spell_aftype(gch);
	    af.type      = sn;
	    af.level	 = level;
	    af.duration  = level + 3;
	    af.location  = 0;
	    af.modifier  = 0;
	    af.bitvector = AFF_FLYING;
	    affect_to_char( gch, &af );
	}

	return;
}

void spell_fog(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{

	ROOM_AFFECT_DATA raf;
	AFFECT_DATA af;

	if ( is_affected_room( ch->in_room, sn ))
	{
		send_to_char("This room is already concealed in a dense fog.\n\r",ch);
	    return;
    }

    init_affect_room(&raf);
	raf.where     = TO_ROOM_AFFECTS;
	raf.aftype    = AFT_SPELL;
	raf.type      = sn;
	raf.level     = ch->level;
	raf.duration  = ch->level/5;
	raf.modifier  = 0;
	raf.bitvector = AFF_ROOM_FOG;
    affect_to_room( ch->in_room, &raf );

    init_affect(&af);
	af.where     = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
	af.type      = sn;
	af.level     = ch->level;
	af.duration  = 15;
	af.modifier  = 0;
	af.bitvector = 0;
    affect_to_char( ch, &af );

    send_to_char("You raise your arms and call forth a dense fog to conceal the room.\n\r", ch);
	act("The room becomes enshrouded in a dense magical fog!",ch,NULL,NULL,TO_ROOM);
}

void spell_tornado(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
    ROOM_INDEX_DATA *pRoomIndex;
    bool fail = FALSE;
    int dam;

    if (victim == ch)
    {
		send_to_char("That wouldn't be a good idea.\n\r", ch );
		return;
	}

	if (IS_NPC(victim))
	{
		if (victim->pIndexData->vnum == MOB_VNUM_ACADIAN
		|| victim->pIndexData->vnum == MOB_VNUM_BRIAR
		|| victim->pIndexData->vnum == MOB_VNUM_RAVAGER

		|| victim->pIndexData->vnum == MOB_VNUM_PROTECTOR
		|| victim->pIndexData->vnum == MOB_VNUM_COVEN
	 	|| victim->pIndexData->vnum == MOB_VNUM_VIGILANTE)
		{
	        send_to_char("You can't summon forth tornado's upon cabal guardians.\n\r", ch );
	        return;
	    }
    }

    if (saves_spell(level,victim,DAM_OTHER))
	{
	    act("$n failed to summon forth a tornado.",ch,0,0,TO_ROOM);
	    return;
    }

    act("$n summons forth a tornado upon $N!'",ch,0,victim,TO_NOTVICT);
	act("$n summons forth a tornado upon you!'",ch,0,victim,TO_VICT);
    act("You focus your energy and summon forth a tornado upon $N.",ch,0,victim,TO_CHAR);

    if (!saves_spell(level - 2,victim,DAM_OTHER))
	{
		fail = TRUE;
	}

    for (; ;)
	{
		pRoomIndex = get_room_index(number_range(0,30000));
		if (pRoomIndex != NULL)
			if (pRoomIndex->area == victim->in_room->area
			&& !IS_SET(pRoomIndex->room_flags,ROOM_PRIVATE)
			&& (pRoomIndex->guild == 0)
			&& can_see_room(ch,pRoomIndex)
			&& !IS_SET(pRoomIndex->room_flags,ROOM_SOLITARY)
			&& pRoomIndex->vnum != 1212)
			break;
    }

    if (fail)
    {
		act("A raging tornado violently thrashes $N into the ground!",ch,0,victim,TO_NOTVICT);
		act("A raging tornado violently thrashes $N into the ground!",ch,0,victim,TO_CHAR);
		act("A raging tornado violently thrashes you into the ground!",ch,0,victim,TO_VICT);
		dam = ch->level + number_percent() + 75;
		damage_old( ch, victim, dam, sn, DAM_OTHER,TRUE);
		return;
	}

	act("A raging tornado sweeps through and carries off $N!",ch,0,victim,TO_NOTVICT);
	act("A raging tornado sweeps through and carries off $N!",ch,0,victim,TO_CHAR);
	send_to_char("A raging tornado sweeps through and carries you off!\n\r",victim );
	char_from_room(victim);
	char_to_room(victim,pRoomIndex);
    return;
}


// Earth Elementalists

void spell_landslide(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{

	CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    int chance;
    int hits = 0;

    if (check_spellcraft(ch,sn))
		dam = spellcraft_dam(level,5);
    else
    	dam = dice( level, 5 );

    if ( saves_spell( level, victim, DAM_OTHER ) )
		dam /= 2;

    for (chance = 0; chance < 5; chance++)
    {
		if (!saves_spell( level, victim, DAM_OTHER ) )
		{
			hits++;
		}
	}

	act("The earth starts to rumble.\n\r",ch,NULL,NULL, TO_ROOM  );

	for ( ; hits != 0; hits--)
	{
		send_to_char("You are are caught up in a massive landslide.\n\r", victim );
		send_to_char("You focus and send forth a massive landslide.\n\r", ch 	 );
		damage_old( ch, victim, dam, sn,DAM_OTHER,TRUE);
		dam /= 2;
		dam *= 3;
        dam /=4;
	}

	act("The earth suddenly stops rumbling.\n\r",ch, NULL, NULL, TO_ROOM );
    return;
}












