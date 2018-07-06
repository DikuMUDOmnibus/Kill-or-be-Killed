#include "include.h"

void spell_minion_tactics( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *gch;
	
	for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
	{
		if ( !is_same_group(gch, ch) || !(IS_NPC(gch)))
		    continue;

		if ((IS_AFFECTED(gch,AFF_CHARM)) && (gch->pIndexData->vnum==MOB_VNUM_ZOMBIE) && (gch->master == ch)) {
			act("$n's eyes widen with newly-acquired tactics.", gch, NULL, NULL, TO_ROOM );
			send_to_char("Your eyes widen with newly-acquired tactics.\n\r", gch );
			SET_BIT(gch->off_flags,OFF_BASH);
			SET_BIT(gch->off_flags,OFF_TRIP);
			SET_BIT(gch->off_flags,OFF_CRUSH);
		}
	}

	send_to_char( "Ok.\n\r", ch );

	return;
}

void spell_minion_sanc( int sn, int level, CHAR_DATA *ch, void *vo, int
target )
{
	CHAR_DATA *gch;

	sn = skill_lookup("sanctuary");

	for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
	{
		if ( !is_same_group(gch, ch) || !(IS_NPC(gch)))
		    continue;

		if ((IS_AFFECTED(gch,AFF_CHARM)) && (gch->pIndexData->vnum==MOB_VNUM_ZOMBIE) && (gch->master == ch)) {
			spell_sanctuary(sn,ch->level,ch,gch,TARGET_CHAR);
		}
	}

	spell_sanctuary(sn,ch->level,ch,ch,TARGET_CHAR);

	send_to_char( "Ok.\n\r", ch );

	return;
}

void spell_minion_flight( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *gch;

	sn= skill_lookup("fly");

	for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
	{
		if ( !is_same_group(gch, ch) || !(IS_NPC(gch)))
		    continue;

		if ((IS_AFFECTED(gch,AFF_CHARM)) && (gch->pIndexData->vnum==MOB_VNUM_ZOMBIE) && (gch->master == ch)) {
			spell_fly(sn,ch->level,ch,gch,TARGET_CHAR);
		}
	}

	spell_fly(sn,ch->level,ch,ch,TARGET_CHAR);

	send_to_char( "Ok.\n\r", ch );

	return;
}

void spell_minion_sneak( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *gch;

	for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
	{
		if ( !is_same_group(gch, ch) || !(IS_NPC(gch)))
		    continue;

		if ((IS_AFFECTED(gch,AFF_CHARM)) && (gch->pIndexData->vnum==MOB_VNUM_ZOMBIE) && (gch->master == ch)) {
			act("$n begins moving silently.", gch, NULL, NULL, TO_ROOM );
			send_to_char("You begin moving silently.\n\r", gch );
			SET_BIT(gch->affected_by,AFF_SNEAK);
		}
	}

	SET_BIT(ch->affected_by,AFF_SNEAK);
	act("$n begins moving silently.", ch, NULL, NULL, TO_ROOM );
	send_to_char("You begin moving silently.\n\r", ch );

	return;
}

void spell_minion_recall( int sn, int level, CHAR_DATA *ch, void *vo, int
target )
{
	CHAR_DATA *gch, *gch_next;
	ROOM_INDEX_DATA *room;

	sn= skill_lookup("word of recall");

	room = ch->in_room;
	spell_word_of_recall(sn,ch->level,ch,ch,TARGET_CHAR);

	for ( gch = room->people; gch != NULL; gch = gch_next)
	{
		gch_next = gch->next_in_room;
		if ( !is_same_group(gch, ch) || !(IS_NPC(gch)))
		    continue;

		if ((IS_AFFECTED(gch,AFF_CHARM)) && (gch->pIndexData->vnum==MOB_VNUM_ZOMBIE) && (gch->master == ch)) {		
			char_from_room(gch);
			char_to_room(gch,ch->in_room);
			act("$n appears in the room.", gch, NULL, NULL, TO_ROOM );
		}
	}

	send_to_char( "Ok.\n\r", ch );

	return;
}

void spell_minion_haste( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *gch;

	sn= skill_lookup("haste");

	for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
	{
		if ( !is_same_group(gch, ch) || !(IS_NPC(gch)))
		    continue;

		if ((IS_AFFECTED(gch,AFF_CHARM)) && (gch->pIndexData->vnum==MOB_VNUM_ZOMBIE) && (gch->master == ch)) {
			spell_haste(sn,ch->level,ch,gch,TARGET_CHAR);
		}
	}

	spell_haste(sn,ch->level,ch,ch,TARGET_CHAR);

	send_to_char( "Ok.\n\r", ch );

	return;
}

void spell_minion_trans( int sn, int level, CHAR_DATA *ch, void *vo, int
target )
{
	CHAR_DATA *gch;

	sn= skill_lookup("pass door");

	for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
	{
		if ( !is_same_group(gch, ch) || !(IS_NPC(gch)))
		    continue;

		if ((IS_AFFECTED(gch,AFF_CHARM)) && (gch->pIndexData->vnum==MOB_VNUM_ZOMBIE) && (gch->master == ch)) {
			spell_pass_door(sn,ch->level,gch,gch,TARGET_CHAR);
		}
	}

	spell_pass_door(sn,ch->level,ch,ch,TARGET_CHAR);

	send_to_char( "Ok.\n\r", ch );

	return;
}

void do_lich( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_INPUT_LENGTH];
     	char buf[MAX_STRING_LENGTH];
     	CHAR_DATA *victim;


     	if (IS_NPC(ch))
     	return;

	argument = one_argument( argument, arg1 );

	if ((ch->level < 56 && ch->pcdata->induct != CABAL_LEADER) || IS_NPC(ch))    
	{        
	send_to_char("Huh?\n\r", ch);        
	return;    
	}

	if ( arg1[0] == '\0' )    
	{	
	buf[0] = '\0';        
	send_to_char( "Syntax: lich <char>\n\r",ch);        
	return;    
	}


	if ((victim = get_char_world(ch, arg1)) == NULL)
	{
	send_to_char("They aren't playing.\n\r", ch);
	return;
	}
    
     	if (victim->race==race_lookup("lich"))
     	{
       	send_to_char("They are lich already.\n\r", ch);
       	return;
     	}

     	sprintf(buf,"%s has been qualified into Lich.\n\r", victim->name);
     	send_to_char(buf,ch);
     	sprintf(buf,"You have been qualified into Lich!\n\r");
     	send_to_char(buf,victim);
     
     	victim->pcdata->learned[skill_lookup("minion tactics")] = 1;
     	victim->pcdata->learned[skill_lookup("minion sanc")] =1;
     	victim->pcdata->learned[skill_lookup("minion flight")] =1;
     	victim->pcdata->learned[skill_lookup("minion sneak")] =1;
     	victim->pcdata->learned[skill_lookup("minion recall")] =1;
     	victim->pcdata->learned[skill_lookup("minion haste")] =1;
     	victim->pcdata->learned[skill_lookup("minion trans")] =1;
    
     	victim->race = race_lookup("lich");

    	victim->perm_stat[STAT_STR]=pc_race_table[ch->race].max_stats[STAT_STR];
        victim->perm_stat[STAT_INT]=pc_race_table[ch->race].max_stats[STAT_INT];
        victim->perm_stat[STAT_WIS]=pc_race_table[ch->race].max_stats[STAT_WIS];
        victim->perm_stat[STAT_DEX]=pc_race_table[ch->race].max_stats[STAT_DEX];
        victim->perm_stat[STAT_CON]=pc_race_table[ch->race].max_stats[STAT_CON];
    	victim->mod_stat[STAT_STR]=0;
    	victim->mod_stat[STAT_INT]=0;
    	victim->mod_stat[STAT_WIS]=0;
    	victim->mod_stat[STAT_DEX]=0;
    	victim->mod_stat[STAT_CON]=0;
    	sprintf(buf, "Your stats are Str: %2d Int: %2d Wis: %2d Dex: %2d Con: %2d\n\r",
                    ch->perm_stat[STAT_STR],
                    ch->perm_stat[STAT_INT],
                    ch->perm_stat[STAT_WIS],
                    ch->perm_stat[STAT_DEX],
		    ch->perm_stat[STAT_CON]);

	send_to_char(buf,victim);
	
}
