#include "include.h"

char *target_name;

void do_supps( CHAR_DATA *ch, char *argument )
{
    BUFFER *buffer;
    char arg[MAX_INPUT_LENGTH];
    char spell_list[LEVEL_HERO + 1][MAX_STRING_LENGTH];
    char spell_columns[LEVEL_HERO + 1];
    int sn, level, min_lev = 1, max_lev = LEVEL_HERO, mana;
    bool fAll = FALSE, found = FALSE;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
      return;
    if(class_table[ch->class].ctype!=CLASS_COMMUNER && !IS_IMMORTAL(ch))
    {
	send_to_char("Your class knows no prayers.\n\r",ch);
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
		    sprintf(buf,
			"Levels must be between 1 and %d.\n\r",LEVEL_HERO);
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
        spell_columns[level] = 0;
        spell_list[level][0] = '\0';
    }

    for (sn = 0; sn < MAX_SKILL; sn++)
    {
        if (skill_table[sn].name == NULL )
	    break;

	if ((level = skill_table[sn].skill_level[ch->class]) < LEVEL_HERO + 1
	&&  level >= min_lev && level <= max_lev
	&&  skill_table[sn].spell_fun != spell_null
	&&  ch->pcdata->learned[sn] > 0
	&&  (skill_table[sn].ctype==CMD_COMMUNE || skill_table[sn].ctype==CMD_BOTH))
        {
	    found = TRUE;
	    level = skill_table[sn].skill_level[ch->class];
	    if (ch->level < level)
	    	sprintf(buf,"%-18s n/a      ", skill_table[sn].name);
	    else
	    {
        mana = UMAX(skill_table[sn].min_mana,
		    100/(2 + ch->level - level));
            sprintf(buf,"%-18s  %3d mana  ",skill_table[sn].name,mana);
	    }

	    if (spell_list[level][0] == '\0')
          	sprintf(spell_list[level],"\n\rLevel %2d: %s",level,buf);
	    else /* append */
	    {
          	if ( ++spell_columns[level] % 2 == 0)
		    strcat(spell_list[level],"\n\r          ");
          	strcat(spell_list[level],buf);
	    }
	}
    }

    /* return results */

    if (!found)
    {
      	send_to_char("No supplications found.\n\r",ch);
      	return;
    }

    buffer = new_buf();
    for (level = 0; level < LEVEL_HERO + 1; level++)
      	if (spell_list[level][0] != '\0')
	    add_buf(buffer,spell_list[level]);
    add_buf(buffer,"\n\r");
    page_to_char(buf_string(buffer),ch);
    free_buf(buffer);
}
void do_commune( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    void *vo;
    int mana;
    int sn;
    AFFECT_DATA af;
    int target;
    if ( IS_NPC(ch) && ch->desc == NULL)
	return;
    target_name = one_argument( argument, arg1 );
    one_argument( target_name, arg2 );

    if(class_table[ch->class].ctype!=CLASS_COMMUNER && !IS_IMMORTAL(ch))
	return send_to_char("You aren't in touch well enough with the gods to commune prayers.\n\r",ch);

    if (IS_SET(ch->act,PLR_BETRAYER))
	return send_to_char("Your god has forsaken you!\n\r",ch);

    if(!IS_NPC(ch) && ch->pcdata->oalign==3 && ch->alignment==-1000)
	return send_to_char("Your god has forsaken you!\n\r",ch);

    if ( arg1[0] == '\0' )
	return send_to_char( "Commune which what where?\n\r", ch );

    if ((sn = find_spell(ch,arg1)) < 1
    ||  skill_table[sn].spell_fun == spell_null
    || (!IS_NPC(ch) && get_skill(ch,sn) < 5)
    || (!IS_NPC(ch) && ch->pcdata->learned[sn] == 0))
	return send_to_char( "You don't know any prayers of that name.\n\r", ch );

//    if(skill_table[sn].skill_level[ch->class]>=51 && !IS_SET(ch->act,PLR_EMPOWERED) && !IS_IMMORTAL(ch))
//	return send_to_char("The gods do not find you worthy of a prayer of that magnitude.\n\r",ch);

    if ( ch->position < skill_table[sn].minimum_position )
	return send_to_char( "You can't concentrate enough.\n\r", ch );

    if((skill_table[sn].ctype==CMD_SPELL
     || skill_table[sn].ctype==CMD_POWER
     || skill_table[sn].ctype==CMD_SONG)
		&& !IS_IMMORTAL(ch))
	return send_to_char("You can't commune that.\n\r",ch);

    if (ch->level + 2 == skill_table[sn].skill_level[ch->class])
	mana = 50;
    else
    {
	if(is_affected(ch,gsn_garble))
	{
    		mana = UMAX(
	    		(skill_table[sn].min_mana*2),
	    		100 / ( 2 + ch->level - skill_table[sn].skill_level[ch->class] ) );
	}
	else
	{
                mana = UMAX(
                        skill_table[sn].min_mana,
                        100 / ( 2 + ch->level - skill_table[sn].skill_level[ch->class] ) );
	}
    }
    /*
     * Locate targets.
     */
    victim	= NULL;
    obj		= NULL;
    vo		= NULL;
    target	= TARGET_NONE;
	af.aftype = AFT_COMMUNE;
    switch ( skill_table[sn].target )
    {
    default:
	bug( "Do_cast: bad target for sn %d.", sn );
	return;
    case TAR_IGNORE:
	break;
    case TAR_CHAR_OFFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    if ( ( victim = ch->fighting ) == NULL )
	    {
		send_to_char( "Commune the prayer on whom?\n\r", ch );
		return;
	    }
	}
	else
	{
	    if ( ( victim = get_char_room( ch, target_name ) ) == NULL )
	    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
	    }
	}
	if ( !IS_NPC(ch) )
	{
	check_killer(ch,victim);
	}
        if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	{
	    send_to_char( "You can't do that on your own master.\n\r",
		ch );
	    return;
	}
	vo = (void *) victim;
	target = TARGET_CHAR;
	break;
    case TAR_CHAR_DEFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    victim = ch;
	}
	else
	{
	    if ( ( victim = get_char_room( ch, target_name ) ) == NULL )
	    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
	    }
	}

	vo = (void *) victim;
	target = TARGET_CHAR;
	break;
    case TAR_CHAR_SELF:
	if ( arg2[0] != '\0' && !is_name( target_name, ch->name ) )
	{
	    send_to_char( "You cannot commune this prayer on another.\n\r", ch );
	    return;
	}

	vo = (void *) ch;
	target = TARGET_CHAR;
	break;
    case TAR_OBJ_INV:
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "What should the prayer be communed upon?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_carry( ch, target_name, ch ) ) == NULL )
	{
	    send_to_char( "You are not carrying that.\n\r", ch );
	    return;
	}

	vo = (void *) obj;
	target = TARGET_OBJ;
	break;
    case TAR_OBJ_CHAR_OFF:
	if (arg2[0] == '\0')
	{
	    if ((victim = ch->fighting) == NULL)
	    {
		send_to_char("Commune the prayer on whom or what?\n\r",ch);
		return;
	    }
	    target = TARGET_CHAR;
	}
	else if ((victim = get_char_room(ch,target_name)) != NULL)
	{
	    target = TARGET_CHAR;
	}
	if (target == TARGET_CHAR) /* check the sanity of the attack */
	{
            if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
			return send_to_char( "You can't do that on your own follower.\n\r", ch );

	    if (!IS_NPC(ch))
		check_killer(ch,victim);

	    vo = (void *) victim;
 	}
	else if ((obj = get_obj_here(ch,target_name)) != NULL)
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}
	else
		return send_to_char("You don't see that here.\n\r",ch);

	break;
    case TAR_OBJ_CHAR_DEF:
        if (arg2[0] == '\0')
        {
            vo = (void *) ch;
            target = TARGET_CHAR;
        }
        else if ((victim = get_char_room(ch,target_name)) != NULL)
        {
            vo = (void *) victim;
            target = TARGET_CHAR;
	}
	else if ((obj = get_obj_carry(ch,target_name,ch)) != NULL)
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}
	else
		return send_to_char("You don't see that here.\n\r",ch);

	break;
    }
    if ( !IS_NPC(ch) && ch->mana < mana )
	return send_to_char( "You don't have enough mana.\n\r", ch );

    WAIT_STATE( ch, skill_table[sn].beats );
    if ( !IS_NPC(ch) && (number_percent( ) > get_skill(ch,sn)))
    {
	send_to_char( "You lost your concentration.\n\r", ch );
	check_improve(ch,sn,FALSE,1);
	ch->mana -= mana / 2;
    }
    else
    {
        ch->mana -= mana;
/*	if (IS_SET(ch->in_room->room_flags,ROOM_NO_MAGIC) && !(ch->level > LEVEL_HERO))
	{
	act("$n's spell fizzles.",ch,0,0,TO_ROOM);
	send_to_char("Your prayer fizzles and dies.\n\r",ch);
	return;
	}
*/	if (skill_table[sn].target == TAR_CHAR_OFFENSIVE
	&& is_safe(ch,victim))
		return;

	if (skill_table[sn].target == TAR_CHAR_OFFENSIVE)
	{
	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (ch->fighting == NULL || victim->fighting == NULL))
		{
		switch(number_range(0,2))
		{
		case (0):
		case (1):
		sprintf(buf,"Die, %s you filthy dog!",PERS(ch,victim));
		break;
		case (2):
		sprintf(buf,"Help! %s is communing a supplication on me!",PERS(ch,victim));
		}
	if (victim != ch && !IS_NPC(ch))
		do_myell(victim,buf);
	}
	}
	if(skill_table[sn].target==TAR_CHAR_OFFENSIVE && victim!=ch)
	{
		act("You narrow your eyes and glare in $N's direction.",ch,0,victim,TO_CHAR);
		act("$n narrows $s eyes and glares in $N's direction.",ch,0,victim,TO_NOTVICT);
		act("$n narrows $s eyes and glares in your direction.",ch,0,victim,TO_VICT);
                if(check_volley(ch,victim))
                {
                        act("$N reflects your spell right back at you!",ch,0,victim,TO_CHAR);
                        act("You reflect $n's spell right back at $m!",ch,0,victim,TO_VICT);
                        act("$N reflects $n's spell right back at $m!",ch,0,victim,TO_NOTVICT);
                        (*skill_table[sn].spell_fun) ( sn, ch->level*2, victim, ch, target);
                        return;
                }
	}
	if(skill_table[sn].target==TAR_CHAR_SELF || skill_table[sn].target==TAR_IGNORE ||
skill_table[sn].target==TAR_OBJ_CHAR_DEF || skill_table[sn].target==TAR_OBJ_CHAR_OFF ||
(skill_table[sn].target==TAR_CHAR_DEFENSIVE && victim==ch))
	{
		act("You close your eyes and concentrate for a moment.",ch,0,0,TO_CHAR);
		act("$n closes $s eyes with a look of concentration for a moment.",ch,0,0,TO_ROOM);
	}
	if(skill_table[sn].target==TAR_CHAR_DEFENSIVE && victim!=ch)
	{
		act("You close your eyes for a moment and nod at $N.",ch,0,victim,TO_CHAR);
		act("$n closes $s eyes for a moment and nods at $N.",ch,0,victim,TO_NOTVICT);
		act("$n closes $s eyes for a moment and nods at you.",ch,0,victim,TO_VICT);
	}
	if(skill_table[sn].target==TAR_OBJ_INV)
	{
		act("You furrow your brow as you look through your possessions.",ch,0,0,TO_CHAR);
		act("$n furrows $s brow as $e looks through $s possessions.",ch,0,0,TO_ROOM);
	}
            (*skill_table[sn].spell_fun) ( sn, ch->level, ch, vo,target);
        check_improve(ch,sn,TRUE,1);
    }
    if ((skill_table[sn].target == TAR_CHAR_OFFENSIVE
    ||   (skill_table[sn].target == TAR_OBJ_CHAR_OFF && target == TARGET_CHAR))
    &&   victim != ch
    &&   victim->master != ch)
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next = vch->next_in_room;
	    if ( victim == vch && victim->fighting == NULL )
	    {	check_killer(victim,ch);
		multi_hit( victim, ch, TYPE_UNDEFINED );
		break;
	    }
	}
    }
    return;
}

bool check_volley(CHAR_DATA *ch, CHAR_DATA *victim)
{
	int skill, chance;
	return FALSE;
	if(!ch || !victim)
		return FALSE;
	if(!(skill=get_skill(victim,gsn_volley)) || victim->fighting || !IS_AWAKE(victim) || victim==ch)
		return FALSE;
	if(get_trust(victim) == MAX_LEVEL)
		return TRUE;
	chance = skill * .6;
	chance -= get_curr_stat(ch,STAT_INT);
	chance += get_curr_stat(victim,STAT_INT);
	if(number_percent()<chance)
		return TRUE;
	return FALSE;
}
