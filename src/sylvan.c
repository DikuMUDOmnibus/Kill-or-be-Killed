#include "include.h"

void do_myell args((CHAR_DATA *victim, char buf[MAX_STRING_LENGTH]));

DECLARE_DO_FUN(do_myell);


void do_chameleon( CHAR_DATA *ch, char *argument )
{
    int sn_fog, sn_fire;

    sn_fog = skill_lookup("faerie fog");
    sn_fire = skill_lookup("faerie fire");

    if (is_affected(ch,sn_fog) || is_affected(ch,sn_fire) || is_affected(ch,gsn_dew))
    {
	send_to_char("You stand out to much to blend with your surroundings.\n\r",ch);
	return;
    }

    if ((ch->in_room->sector_type == SECT_CITY) || (ch->in_room->sector_type == SECT_INSIDE))
    {
	send_to_char("You would feel too dirty if you took on the colors of civilization.\n\r",ch);
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CAMOUFLAGE) )
        return send_to_char("You are already camouflaged.\n\r",ch);
    {
	send_to_char( "You change color to blend with your surroundings.\n\r", ch );
	SET_BIT(ch->affected_by, AFF_CAMOUFLAGE);
	check_improve(ch,gsn_chameleon,TRUE,1);
	WAIT_STATE(ch,12);
    }
    return;
}

void do_insectswarm( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;
    AFFECT_DATA af;
    char buf[MAX_STRING_LENGTH];


    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_insect_swarm)) == 0 ||   (!IS_NPC(ch)
    &&    ch->level < skill_table[gsn_insect_swarm].skill_level[ch->class]))
    {
        send_to_char("You don't know how to do that.\n\r",ch);
        return;
    }


    if (cabal_down(ch,CABAL_BRIAR))
    {
         send_to_char("You cannot find the cabal power within you!\n\r",ch);
         return;
    }

    if (arg[0] == '\0')
    {
		send_to_char("Call insects on who?\n\r",ch);
		return;
    }

    if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    if (is_affected(victim,gsn_insect_swarm))
    {
        send_to_char("They are already being swarmed.\n\r",ch);
        return;
    }

    if (ch->mana < 50)
    {
        send_to_char("You can't find the energy.\n\r",ch);
        return;
    }

    if (victim == ch)
    {
        send_to_char("You can't call that on yourself!\n\r",ch);
        return;
    }
    if (is_safe(ch,victim))
	return;

     if(!(ch->in_room->sector_type == SECT_FIELD || ch->in_room->sector_type == SECT_FOREST))
	{
	send_to_char("You must be in more of a wilderness area to call insects.\n\r",ch);
	return;
	}

     chance += (ch->drain_level + ch->level - victim->level - victim->drain_level) * 2;


     if (number_percent( ) < (get_skill(ch,gsn_insect_swarm) - 15))
    {
        sprintf(buf,"Help! %s is calling down insects on me!",PERS(ch,victim));
	do_myell(victim,buf);

        WAIT_STATE(ch,PULSE_VIOLENCE);
        ch->mana -= 50;
        init_affect(&af);
        af.where     = TO_AFFECTS;
        af.aftype    = AFT_POWER;
    	  af.type      = gsn_insect_swarm;
    	  af.level     = ch->level;
    	  af.duration  = ch->level / 6;
    	  af.location  = APPLY_DEX;
    	  af.modifier  = -3;
    	  af.bitvector = 0;
    	  affect_to_char( victim, &af );
    	  send_to_char( "You frantically wave your arms as a cloud of insects descends upon you.\n\r", victim );
    	  act("$n waves his arms as a cloud of insects descends upon him.",victim,NULL,NULL,TO_ROOM);
    	  check_improve(ch,gsn_insect_swarm,TRUE,1);
          return;
         }
          else
         {
          WAIT_STATE(ch,PULSE_VIOLENCE);
          ch->mana -= 25;

          send_to_char("You fail to call down insects.\n\r",ch);
          check_improve(ch,gsn_insect_swarm,FALSE,1);


    }

}

void spell_beast_call(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
        CHAR_DATA *gch;
	CHAR_DATA *mob, *victim = (CHAR_DATA *) vo, *wolf;
	char buf[MAX_STRING_LENGTH];

    if (cabal_down(ch,CABAL_BRIAR))
    {
         send_to_char("You cannot find the cabal power within you!\n\r",ch);
         return;
    }

	for(wolf=char_list;wolf!=NULL;wolf=wolf->next) {
		if((wolf->master == ch) && (wolf->pIndexData->vnum == MOB_VNUM_BEAST)) {
			return send_to_char("You can't call for more than one beast at a time.\n\r",ch);
			break;
		} else {
			break;
		}
	}

        for (gch = char_list; gch != NULL; gch = gch->next)
        {
        if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
          gch->pIndexData->vnum == MOB_VNUM_BEAST)
        {
          send_to_char("You cannot call upon more than one beast at a time!\n\r",ch);
          return;
        }
    }

	if(!(ch->in_room->sector_type == SECT_CITY || ch->in_room->sector_type ==SECT_INSIDE)) {

		mob = create_mobile(get_mob_index(MOB_VNUM_BEAST));
		mob->level = ch->level;
		mob->max_hit = dice(ch->level, 2*ch->level);
		mob->hit = mob->max_hit;
		mob->damroll = dice(ch->level,3);
		char_to_room(mob,ch->in_room);
		add_follower(mob,ch);
		mob->leader = ch;
		SET_BIT(mob->affected_by,AFF_CHARM);
		act("You call out to nature and $N responds.",ch,0,mob,TO_CHAR);
		act("$N responds to $n's call!",ch,0,mob,TO_ROOM);

               if(!IS_NPC(victim)) {
	sprintf(buf,"Die, %s, you sorcerous dog!",ch->name);
	do_myell(victim,buf);
	}
	multi_hit(mob,victim,TYPE_UNDEFINED);
	return;
	} else {
		return send_to_char("You must be in a wilderness area to call beasts.\n\r",ch);
	}
}

void spell_wall_of_thorns(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	ROOM_AFFECT_DATA raf;

    if (cabal_down(ch,CABAL_BRIAR))
    {
         send_to_char("You cannot find the cabal power within you!\n\r",ch);
         return;
    }

    if ( is_affected( ch, sn ) )
    {
      send_to_char("It's too soon to make another wall of thorns.\n\r", ch);
      return;
    }

    if(!(ch->in_room->sector_type == SECT_FIELD || ch->in_room->sector_type == SECT_FOREST))
	{
	send_to_char("There isn't enough foliage here to call a wall of thorns.\n\r",ch);
	return;
	}

    if ( is_affected_room( ch->in_room, sn ))
    {
        send_to_char("It's already here.\n\r",ch);
        return;
    }

	if (number_range(1,100) > ch->pcdata->learned[sn])
	{
		send_to_char("You attempt to make the wall of thorns but fail.\n\r",ch);
	}

        init_affect(&af);
    af.where     = TO_AFFECTS;
        af.aftype    = AFT_POWER;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = 48;
    af.modifier  = 0;
    af.bitvector = 0;
    affect_to_char( ch, &af );

        init_affect(&af);
    raf.where     = TO_ROOM_AFFECTS;
        raf.aftype    = AFT_POWER;
    raf.type      = sn;
    raf.level     = ch->level;
    raf.duration  = ch->level/5;
    raf.modifier  = 0;
    raf.bitvector = AFF_ROOM_WALL_THORNS;
    affect_to_room( ch->in_room, &raf );


    send_to_char("You command a wall of thorns to grow to attack intruders.\n\r", ch);
    act("$n commands a wall of thorns to grow to attack intruders.\n\r",ch,NULL,NULL,TO_ROOM);
}
