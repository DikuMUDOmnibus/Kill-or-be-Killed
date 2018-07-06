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

/* command procedures needed */
DECLARE_DO_FUN(do_wear);
DECLARE_DO_FUN(do_look		);
DECLARE_DO_FUN(do_cb );
DECLARE_DO_FUN(do_recall	);
DECLARE_DO_FUN(do_stand		);
DECLARE_DO_FUN(do_camp		);
DECLARE_DO_FUN(do_animal_call	);
DECLARE_DO_FUN(do_say		);
DECLARE_DO_FUN(do_slay		);
DECLARE_DO_FUN(do_yell		);
DECLARE_DO_FUN(do_murder        );
DECLARE_DO_FUN(do_chameleon	);
DECLARE_DO_FUN(do_goto		);
DECLARE_DO_FUN(do_acadian_track	);

char *	const	dir_name	[]		=
{
    "north", "east", "south", "west", "up", "down"
};

const	sh_int	rev_dir		[]		=
{
    2, 3, 0, 1, 5, 4
};

const	sh_int	movement_loss	[SECT_MAX]	=
{
    1, 2, 2, 3, 4, 6, 4, 1, 6, 10, 6
};



/*
 * Local functions.
 */
int	find_door	args( ( CHAR_DATA *ch, char *arg ) );
bool	has_key		args( ( CHAR_DATA *ch, int key ) );
void 	check_guardian	args( (CHAR_DATA *ch,int in_room) );
void 	enforcer_entry_trigger  args(( CHAR_DATA *ch) );
void	enforcer_key_trigger  args((CHAR_DATA *ch,CHAR_DATA *statue,OBJ_DATA *key));
void	outlaw_key_trigger  args((CHAR_DATA *ch,CHAR_DATA *statue,OBJ_DATA *key));
void 	rager_entry_trigger	args((CHAR_DATA *ch));
void 	knight_entry_trigger	args((CHAR_DATA *ch));
void 	outlaw_entry_trigger	args((CHAR_DATA *ch));
void 	ancient_entry_trigger	args((CHAR_DATA *ch));
void 	sylvan_entry_trigger	args((CHAR_DATA *ch));
void 	arcana_entry_trigger	args((CHAR_DATA *ch));
void    empire_entry_trigger    args((CHAR_DATA *ch));
void 	murder_entry_trigger	args((CHAR_DATA *ch));
void 	move_prog_hydra		args((CHAR_DATA *ch));
void 	move_prog_bones		args((CHAR_DATA *ch));
void 	mob_entry_wraith	args((CHAR_DATA *ch));
bool    check_slick		args((CHAR_DATA *ch));
bool	pay_howmuch		args((CHAR_DATA *ch));

void move_char( CHAR_DATA *ch, int door, bool follow )
{
    CHAR_DATA *fch, *fch_next, *tch, *tch_next, *snaremaker, *wch, *mount;
    ROOM_INDEX_DATA *in_room, *to_room;
    EXIT_DATA *pexit;
    AFFECT_DATA snareaf;
    ROOM_AFFECT_DATA *snareaffect;
    bool snaretripped;
    int odir, centcount, c, invnum;
    char buf[MAX_STRING_LENGTH], buf2[MSL];
    snaremaker=NULL;

    if ( door < 0 || door > 5 )
    {
	bug( "Do_move: bad door %d.", door );
	return;
    }

     if(!ch->in_room)
        return;

        invnum = ch->in_room->vnum;

    if (is_centurion(ch))
		return;

       if (!IS_NPC(ch))
    {
	if(is_affected(ch,gsn_chameleon))
		affect_strip(ch,gsn_chameleon);
      if (is_affected(ch, gsn_confuse))
      {
       if (is_affected(ch, gsn_confuse))
        {
          act("You are so confused.. not to mention kind of lost..",
              ch,NULL,NULL,TO_CHAR);
          act("$n looks so confused.. not to mention kind of lost..",
              ch,NULL,NULL,TO_ROOM);
          door = number_range(0,5);
        }
        else
        {
          act("You feel very confuse..",ch,NULL,NULL,TO_CHAR);
          act("$n looks very confuse..",ch,NULL,NULL,TO_ROOM);
        }
      }
    }

    /*
     * Exit trigger, if activated, bail out. Only PCs are triggered.
     */
    if ( !IS_NPC(ch) && mp_exit_trigger( ch, door ) )
        return;

/*    in_room = ch->in_room;
    if ( ( pexit   = in_room->exit[door] ) == NULL
    ||   ( to_room = pexit->u1.to_room   ) == NULL
    ||	 !can_see_room(ch,pexit->u1.to_room))
    {
	send_to_char( "Alas, you cannot go that way.\n\r", ch );
	return;
    }
*/

    in_room = ch->in_room;
    if ( ( pexit   = in_room->exit[door] ) == NULL
    ||	 pexit->u1.vnum == 0
    ||   ( to_room = pexit->u1.to_room   ) == NULL
    ||	 !can_see_room(ch,pexit->u1.to_room))
    {
	send_to_char( "Alas, you cannot go that way.\n\r", ch );
	return;
    }

if (IS_ROOM_AFFECTED(in_room,AFF_ROOM_RANDOMIZER))
{
     int d0;
     int i;

     for(i=0; i< 1000; i++)
     {
     d0 = number_range(0,5);
     if ( ( pexit   = in_room->exit[d0] ) == NULL
     ||   ( to_room = pexit->u1.to_room   ) == NULL
     ||    !can_see_room(ch,pexit->u1.to_room))
     continue;
     door = d0;
     break;
     }
}

/* Centurions! */
if ((centcount = room_centurions(ch)) > 0) {
	if (!IS_IMMORTAL(ch) && !isNewbie(ch)) {
		if (!IS_NPC(ch) && ch->ghost == 0) {
	for (c = 0; c < centcount; c++) {
			/*if (ch->pcdata->empire == EMPIRE_ANATH) {
				act("An Imperial Centurion is infuriated and blocks your path!",ch,NULL,NULL,TO_CHAR);
				act("An Imperial Centurion is infuriated and blocks $n's path!",ch,NULL,NULL,TO_ROOM);
				return;
			}*/
			if (ch->pcdata->empire >= EMPIRE_BLOOD ||
				(ch->pcdata->empire < EMPIRE_BLOOD && ch->pcdata->empire_pay >= pay_howmuch(ch)))
			{
				act("An Imperial Centurion steps aside and allows you to pass.",ch,NULL,NULL,TO_CHAR);
				act("An Imperial Centurion steps aside and allows $n to pass.",ch,NULL,NULL,TO_ROOM);
				break;
			} else {
				if (ch->pcdata->empire_prev && ch->ghost==0) {
				odir = ch->pcdata->empire_prev;
				if (odir == ch->in_room->exit[door]->u1.to_room->vnum)
				{
					act("You turn away from the Centurion and return the way you came.",ch,NULL,NULL,TO_CHAR);
					act("$n turns away from the Centurion and returns the way $e came.",ch,NULL,NULL,TO_ROOM);
					break;
				} else {
					act("An Imperial Centurion blocks the way.",ch,NULL,NULL,TO_CHAR);
					return;
				}
				}
			}
		}
		ch->pcdata->empire_pay = 0;
	}
	}
}
    if (IS_SET(pexit->exit_info, EX_CLOSED)
    &&  (!IS_AFFECTED(ch, AFF_PASS_DOOR) || IS_SET(pexit->exit_info,EX_NOPASS))
    &&   !IS_TRUSTED(ch,ANGEL))
    {
	act( "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM)
    &&   ch->master != NULL
    &&   in_room == ch->master->in_room )
    {
	send_to_char( "What?  And leave your beloved master?\n\r", ch );
	return;
    }

    if ( !is_room_owner(ch,to_room) && room_is_private( to_room ) )
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }
 	if ( to_room->guild != 0 && ((to_room->guild - 1) != ch->class))
	{
		if (!IS_NPC(ch))
		{
		send_to_char("You aren't allowed in there.\n\r",ch);
		return;
		}
	}
    if ( to_room->guild !=0 && ch->pause > 0
	&& ch->in_room->guild != to_room->guild)
    {
	send_to_char("You feel to bloody to go in there.\n\r",ch);
    	return;
    }

    if ( !IS_NPC(ch) )
    {
	int iClass, iGuild;
	int move;
	for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
	{
	    for ( iGuild = 0; iGuild < MAX_GUILD; iGuild ++)
	    {
	    	if ( iClass != ch->class
	    	&&   to_room->vnum == class_table[iClass].guild[iGuild] )
	    	{
		    send_to_char( "You aren't allowed in there.\n\r", ch );
		    return;
		}
	    }
	}

	for ( iGuild = 0; iGuild < MAX_GUILD; iGuild ++)
	{
	    if ( ch->pause > 0 && to_room->vnum == class_table[iClass].guild[iGuild] )
	    {
		send_to_char( "You feel too bloody to go in there.\n\r", ch );
		return;
	    }
	}

	if ( in_room->sector_type == SECT_AIR
	||   to_room->sector_type == SECT_AIR )
	{
	    if ( !IS_AFFECTED(ch, AFF_FLYING) && !IS_IMMORTAL(ch))
	    {
		send_to_char( "You can't fly.\n\r", ch );
		return;
	    }
	}

	if (( in_room->sector_type == SECT_WATER_NOSWIM
	||    to_room->sector_type == SECT_WATER_NOSWIM )
  	&&    (!IS_AFFECTED(ch,AFF_FLYING) && !IS_AFFECTED(ch,AFF_SWIM)) )
	{
	    OBJ_DATA *obj;
	    bool found;

	    /*
	     * Look for a boat.
	     */
	    found = FALSE;

	    if (IS_IMMORTAL(ch))
		found = TRUE;

	    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
	    {
		if ( obj->item_type == ITEM_BOAT )
		{
		    found = TRUE;
		    break;
		}
	    }
	    if ( !found )
	    {
		send_to_char( "You need a boat to go there.\n\r", ch );
		return;
	    }
	}

	move = movement_loss[UMIN(SECT_MAX-1, in_room->sector_type)]
	     + movement_loss[UMIN(SECT_MAX-1, to_room->sector_type)]
	     ;

        move /= 2;  /* i.e. the average */


	/* conditional effects */
	if ( (get_skill(ch,gsn_trek) > 0)
	|| (ch->level < skill_table[gsn_trek].skill_level[ch->class]) )
		move /= 2;

	if (IS_AFFECTED(ch,AFF_FLYING) || IS_AFFECTED(ch,AFF_HASTE))
	    move /= 2;

	if (IS_AFFECTED(ch,AFF_SLOW))
	    move *= 2;

	if ( ch->move < move )
	{
	    send_to_char( "You are too exhausted.\n\r", ch );
	    return;
	}

	WAIT_STATE( ch, 1 );
	if (IS_SET(ch->comm, COMM_MOUNTED))
	{
		;
	}else{
		ch->move -= move;
		}
    }

    if (is_affected(ch,gsn_forest_blending))
	un_forest_blend(ch);

    if ( IS_AFFECTED(ch, AFF_CAMOUFLAGE) && !is_affected(ch,gsn_creep))
    {
	un_camouflage(ch, NULL);
    }
    un_earthfade(ch,NULL);

    if ( IS_AFFECTED(ch, AFF_HIDE) && to_room->sector_type != SECT_CITY && to_room->sector_type != SECT_INSIDE && !is_affected(ch,gsn_creep))
    {
	un_hide(ch,NULL);
    }

    if ( !IS_AFFECTED(ch, AFF_SNEAK)
    &&   ch->invis_level < LEVEL_HERO
    &&   !is_affected(ch,gsn_creep))
    {
	un_hide(ch, NULL);
	if (IS_SET(ch->comm, COMM_MOUNTED))
		act( "$n leaves $T on $s mount.", ch, NULL, dir_name[door], TO_ROOM );
	else if (is_affected(ch,gsn_door_bash))
        act("$n goes crashing through the door $T.",ch,0,dir_name[door],TO_ROOM);
    else
	act( "$n leaves $T.", ch, NULL, dir_name[door], TO_ROOM );

    } else {
	act( "IMM: $n sneaks $T.", ch, NULL, dir_name[door], TO_IMMINROOM );
    }

    if ( (to_room->sector_type != SECT_FOREST) && (to_room->sector_type != SECT_MOUNTAIN) && (to_room->sector_type != SECT_HILLS ) )
    {
	un_camouflage(ch, NULL);
    }

/* tracking stuff...very inefficient tracking method but basic. */
    if (!IS_NPC(ch))
	{
	ch->pcdata->track_dir[ch->in_room->vnum] = door + 1;

    if (ch->in_room)

	ch->pcdata->empire_prev = ch->in_room->vnum;
	}

    char_from_room( ch );
    char_to_room( ch, to_room );
    snaretripped=FALSE;
    if ( !IS_AFFECTED(ch, AFF_SNEAK)
    &&   ch->invis_level < LEVEL_HERO
    &&   !is_affected(ch,gsn_creep))
    {
	if (IS_SET(ch->comm, COMM_MOUNTED))
		act( "$n has arrived on $s mount.", ch, NULL, NULL, TO_ROOM );
    else if (is_affected(ch,gsn_door_bash))
        act("The $T door bursts open and $n comes crashing in!",ch,0,dir_name[door],TO_ROOM);
    else
	act( "$n has arrived.", ch, NULL, NULL, TO_ROOM );
    } else {
	act( "IMM: $n sneaks in.", ch, NULL, NULL, TO_IMMINROOM );
    }

    do_look( ch, "auto" );

    if (is_affected_room(to_room, gsn_snare) && !IS_NPC(ch))
    {
	for(snareaffect = to_room->affected; snareaffect!=NULL; snareaffect = snareaffect->next)
		if(snareaffect->type == gsn_snare)
			break;
	snaremaker = snareaffect->owner;
	if(snareaffect && snaremaker!=NULL && snaremaker!=ch && !is_safe(snaremaker,ch) && !is_same_group(ch,snaremaker) &&
		!is_same_cabal(ch,snaremaker) && (!IS_IMMORTAL(ch) || IS_IMMORTAL(snaremaker)))
	{
	act("A tangled mass of vines descends upon you, entangling you and rendering you immobile!",ch,0,0,TO_CHAR);
	act("A tangled mass of vines descends upon $n, entangling $m and rendering $m immobile!",ch,0,0,TO_ROOM);
	init_affect(&snareaf);
      	snareaf.where     = TO_CHAR;
	snareaf.type      = gsn_snare;
	snareaf.aftype	  = AFT_SKILL;
      	snareaf.level     = ch->level;
      	snareaf.duration  = 1;
      	snareaf.location  = APPLY_NONE;
      	snareaf.modifier  = 0;
      	snareaf.bitvector = 0;
      	affect_to_char( ch, &snareaf );
	snaretripped = TRUE;
	sprintf(buf2,"%sYou hear a loud cracking noise as %s stumbles into your snares.%s\n\r",
		get_char_color(snaremaker,"green"),ch->name,END_COLOR(snaremaker));
	send_to_char(buf2,snaremaker);
	}
    }

    if (IS_ROOM_AFFECTED(ch->in_room, AFF_ROOM_ICE_SLICK))
        send_to_char("The ground is very icy!\n\r",ch);
    if (IS_ROOM_AFFECTED(ch->in_room, AFF_ROOM_BLIZZARD))
        send_to_char("A blizzard rages on here!\n\r",ch);
    if (IS_ROOM_AFFECTED(ch->in_room, AFF_ROOM_ELECTRIC_STORM))
        send_to_char("A lightning storm rages overhead!\n\r",ch);
    if (IS_ROOM_AFFECTED(ch->in_room,AFF_ROOM_VACUME))
        send_to_char("You can't breathe here!\n\r",ch);

    if (is_affected(ch,gsn_trip_wire))
    {
    	send_to_char("Your trip wire snaps back in your face!\n\r",ch);
    	WAIT_STATE(ch,24);
    	affect_strip(ch,gsn_trip_wire);
    }

    if (!IS_NPC(ch))
    {
    	for (wch = to_room->people;wch != NULL;wch = wch->next_in_room)
    	{
	    CHAR_DATA *victim;
    	    victim     = ch;
	    if (ch == wch)
		continue;
    	    if ( !IS_NPC(wch) && is_affected(wch,gsn_trip_wire) && wch->fighting == NULL && IS_AWAKE(wch)
                && !(IS_AFFECTED(victim,AFF_FLYING) && !(!IS_NPC(victim) && victim->position > POS_SITTING)))
            {
                AFFECT_DATA naf;
                if (number_percent() < get_skill(wch,gsn_trip_wire))
		{
                    sprintf(buf,"Help! I was tripped by %s's trip wire!",PERS(wch,victim));
                    do_myell(victim,buf);
                    if (number_percent() < get_skill(victim, gsn_balance)/2 && victim->mana > 0)
                    {
                        act("You trip $N with your wire but $E keeps $S balance!",wch,NULL,victim,TO_CHAR);
                        act("$n trips $N with $s wire but $N keeps $S balance!",wch,NULL,victim,TO_NOTVICT);
                        act("$n trips you with $s wire but you manage to stay balanced!",wch,NULL,victim,TO_VICT);
                        victim->mana -= URANGE(0,25,victim->mana);
                    }
                    else
                    {
                        act("Your trip wire sends $N to the ground!",wch,NULL,victim,TO_CHAR);
                        act("$n's trip wire sends $N to the ground!",wch,NULL,victim,TO_NOTVICT);
                        act("$n's trip wire sends you to the ground!",wch,NULL,victim,TO_VICT);
                        victim->position = POS_RESTING;
			WAIT_STATE(victim,48);
                    }
                }
                else
                {
                    act("Your trip wire snaps back in your face!",wch,NULL,NULL,TO_CHAR);
                    act("$n's trip wire snaps back in $s face!",wch,NULL,NULL,TO_ROOM);
                    wch->position = POS_RESTING;
                    WAIT_STATE(wch,24);
                }
		init_affect(&naf);
                affect_strip(wch,gsn_trip_wire);
                naf.where            = TO_AFFECTS;
                naf.type             = gsn_wire_delay;
		naf.aftype	     = AFT_SKILL;
                naf.level            = wch->level;
                naf.duration         = 15 - (wch->level/10);
                naf.location         = 0;
                naf.modifier         = 0;
                naf.bitvector        = 0;
                affect_to_char(wch,&naf);
		break;
            }
        }
    }

    if (ch->cabal == CABAL_BRIAR)
	{
	if (IS_ROOM_AFFECTED(ch->in_room,AFF_ROOM_WALL_THORNS))
	send_to_char("{yYou step through a hole in the wall of thorns.{x\n\r",ch);
	}

    if (in_room == to_room) /* no circular follows */
	return;



	for ( fch = in_room->people; fch != NULL; fch = fch_next )
    {
	fch_next = fch->next_in_room;


	if ( fch->master == ch && IS_AFFECTED(fch,AFF_CHARM)
	&&   fch->position < POS_STANDING)
	    do_stand(fch,"");

	if ( fch->master == ch && fch->position == POS_STANDING
	&&   can_see_room(fch,to_room))
	{

	    if (IS_SET(ch->in_room->room_flags,ROOM_LAW)
	    &&  (IS_NPC(fch) && IS_SET(fch->act,ACT_AGGRESSIVE)))
	    {
		act("You can't bring $N into the city.",
		    ch,NULL,fch,TO_CHAR);
		act("You aren't allowed in the city.",
		    fch,NULL,NULL,TO_CHAR);
		continue;
	    }

	    act( "You follow $N.", fch, NULL, ch, TO_CHAR );
	    move_char( fch, door, TRUE );
	}
    }

   // Can't have people runnin around mounted without the steed

   mount = ch->in_room->people;

   if ( (!IS_NPC(ch))
   && (IS_SET(ch->comm, COMM_MOUNTED)) )
   {
   		if (mount->master == ch)
   		{
   			if ( (mount->pIndexData->vnum == MOB_VNUM_BLACK_STEED)
   			|| (mount->pIndexData->vnum == MOB_VNUM_RED_STEED)
   			|| (mount->pIndexData->vnum == MOB_VNUM_BLUE_STEED)
   			|| (mount->pIndexData->vnum == MOB_VNUM_GREEN_STEED) )
   			{
   				send_to_char("You arrive on your steed.\n\r",ch);
   			}
   			else
   			{
   				send_to_char("You seem to have misplaced your steed and are forced to walk.\n\r" , ch);
   				REMOVE_BIT(ch->comm, COMM_MOUNTED);
   			}
		}
		else
		{
			send_to_char("You seem to have misplaced your steed and are forced to walk.\n\r" , ch);
   			REMOVE_BIT(ch->comm, COMM_MOUNTED);
		}

   }


if(is_affected_room(to_room,gsn_snare) && snaretripped)
		affect_strip_room(to_room,gsn_snare);

    if (!IS_NPC(ch))
		ch->pcdata->track_dir[ch->in_room->vnum] = 0;

	if (is_affected(ch, gsn_target))
	{
		do_acadian_track(ch, ch->pcdata->target);
	}

    check_guardian(ch,ch->in_room->vnum);

    for (tch = char_list; tch != NULL; tch = tch_next)
    {
	tch_next = tch->next;
/*    gas_chance = 0;
   if (is_affected(ch,gsn_gaseous_form))
{
    gas_chance = get_skill(ch,gsn_gaseous_form)/2;
    gas_chance += (ch->level - tch->level)*2;
    if (gas_chance > 75)
	gas_chance = 75;
}
    if (number_percent() < gas_chance)
	continue;

	if (IS_NPC(tch) && tch->last_fought == ch && can_see(tch,ch)
	&& (ch->in_room == tch->in_room) && (tch->fighting == NULL)
	&& (tch->position != POS_FIGHTING))
	{
if (is_affected(ch,gsn_gaseous_form))
{
act("$N manages to intercept you and forces you out of gaseous form.",ch,0,tch,TO_CHAR);
 	un_gaseous(ch);
}
	sprintf(buf, "%s, now you die!",ch->name);
	do_yell(tch,buf);
	multi_hit(tch,ch,TYPE_UNDEFINED);
	}*/
    }

    /*
     * If someone is following the char, these triggers get activated
     * for the followers before the char, but it's safer this way...
     */

    murder_entry_trigger(ch); /* For the OFF_MURDER flag */
    mob_entry_wraith(ch); /* For the drannor forest. Remove if not using the drannor areas */

    for (tch = char_list; tch != NULL; tch = tch->next)
    {
	if (IS_NPC(ch))
		continue;
	if (tch->alarm == NULL)
		continue;
	if (tch->alarm != ch->in_room)
		continue;
	sprintf(buf,"%s has entered %s.\n\r",PERS(ch,tch),ch->in_room->name);
	send_to_char(buf,tch);
	tch->alarm = NULL;
    }

    if ( IS_NPC( ch ) && HAS_TRIGGER( ch, TRIG_ENTRY ) && ch->last_fought == NULL )
        mp_percent_trigger( ch, NULL, NULL, NULL, TRIG_ENTRY );
if (is_affected(ch,gsn_gaseous_form) && number_percent() < 70)
		return;

    if ( !IS_NPC( ch ) )
    {
        mp_greet_trigger( ch );
	move_prog_bones(ch);
	move_prog_hydra(ch);
    }
	if (is_affected(ch,gsn_parting_blow)) {
		affect_strip(ch,gsn_parting_blow);
	}

	return;
}

bool check_ice_slick(CHAR_DATA *ch)
{
    int chance;
    if (!IS_ROOM_AFFECTED(ch->in_room,AFF_ROOM_ICE_SLICK))
	return FALSE;
    chance = number_range(80,100);
    chance -= (get_curr_stat(ch,STAT_DEX) * 3.25);
    chance += get_carry_weight(ch) / 20;
    if (IS_AFFECTED(ch,AFF_HASTE))
	chance += 20;
    if (IS_AFFECTED(ch,AFF_SLOW))
	chance -= 15;
    chance = UMIN(chance, 90);
    //well flying things don't slip on the ground
    if (IS_AFFECTED(ch,AFF_FLYING))
	return FALSE;
    if (is_affected(ch,gsn_shield_of_frost))
	return FALSE;
    if (number_percent() > chance)
	return FALSE;    //they don't slip
    //they slip
    act("$n slips on the ice and falls to the ground!",ch,NULL,NULL,TO_ROOM);
    act("You slip on the ice and fall to the ground!",ch,NULL,NULL,TO_CHAR);
    damage_old(ch,ch,number_range(20,40),skill_lookup("ice slick"),DAM_BASH, TRUE);
    WAIT_STATE(ch, 1 * PULSE_VIOLENCE);
    return TRUE;
}

bool check_wall_thorns(CHAR_DATA *ch)
{
    //int chance;

    if (ch->cabal == CABAL_BRIAR)
        return FALSE;

    if (!IS_ROOM_AFFECTED(ch->in_room,AFF_ROOM_WALL_THORNS))
	return FALSE;
    /*chance = number_range(80,100);
    chance -= (get_curr_stat(ch,STAT_DEX) * 3.25);
    chance += get_carry_weight(ch) / 20;
    chance = UMIN(chance, 90); */
    if (number_percent() < 25)
	return FALSE;
    act("A wall of thorns entraps $n, shredding it unprotected skin!",ch,NULL,NULL,TO_ROOM);
    act("A wall of thorns entraps you, shredding any unprotected skin!",ch,NULL,NULL,TO_CHAR);
    damage_old(ch,ch,number_range(20,40),skill_lookup("wall of thorns"),DAM_BASH,TRUE);
    WAIT_STATE(ch, 1 * PULSE_VIOLENCE);
    return TRUE;
}

bool check_bloody(CHAR_DATA *ch)
{
	damage_old(ch,ch,number_range(140,160),skill_lookup("bloody shackles"),DAM_BASH,TRUE);
	return TRUE;
}

void do_north( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *original_room;
    original_room = ch->in_room;
    if (check_ice_slick(ch))
	return;
    if (check_wall_thorns(ch))
	return;
    if (is_affected(ch,gsn_bloody_shackles))
	{
	damage_old(ch,ch,number_range(140,160),skill_lookup("bloody shackles"),DAM_BASH,TRUE);
	}
    move_char( ch, DIR_NORTH, FALSE );
    if (ch->in_room != original_room)
	if (check_ice_slick(ch))
		return;
    if (ch->in_room != original_room)
	if (check_wall_thorns(ch))
		return;
    return;
}



void do_east( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *original_room;
    original_room = ch->in_room;
    if (check_ice_slick(ch))
        return;
    if (check_wall_thorns(ch))
        return;
    if (is_affected(ch,gsn_bloody_shackles))
        {
        damage_old(ch,ch,number_range(140,160),skill_lookup("bloody shackles"),DAM_BASH,TRUE);
        }
    move_char( ch, DIR_EAST, FALSE );
    if (ch->in_room != original_room)
        if (check_ice_slick(ch))
		return;
    if (ch->in_room != original_room)
        if (check_wall_thorns(ch))
                return;
    return;
}



void do_south( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *original_room;
    original_room = ch->in_room;
    if (check_ice_slick(ch))
        return;
    if (check_wall_thorns(ch))
        return;
    if (is_affected(ch,gsn_bloody_shackles))
        {
        damage_old(ch,ch,number_range(140,160),skill_lookup("bloody shackles"),DAM_BASH,TRUE);
        }
    move_char( ch, DIR_SOUTH, FALSE );
    if (ch->in_room != original_room)
       if ( check_ice_slick(ch))
		return;
    if (ch->in_room != original_room)
        if (check_wall_thorns(ch))
                return;
    return;
}



void do_west( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *original_room;
    original_room = ch->in_room;
    if (check_ice_slick(ch))
        return;
    if (check_wall_thorns(ch))
        return;
    if (is_affected(ch,gsn_bloody_shackles))
        {
        damage_old(ch,ch,number_range(140,160),skill_lookup("bloody shackles"),DAM_BASH,TRUE);
        }
    move_char( ch, DIR_WEST, FALSE );
    if (ch->in_room != original_room)
        if (check_ice_slick(ch))
		return;
    if (ch->in_room != original_room)
        if (check_wall_thorns(ch))
                return;
    return;
}



void do_up( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *original_room;
    original_room = ch->in_room;
    if (check_ice_slick(ch))
        return;
    if (check_wall_thorns(ch))
        return;
    if (is_affected(ch,gsn_bloody_shackles))
        {
        damage_old(ch,ch,number_range(140,160),skill_lookup("bloody shackles"),DAM_BASH,TRUE);
        }
    move_char( ch, DIR_UP, FALSE );
    if (ch->in_room != original_room)
        if (check_ice_slick(ch))
		return;
    if (ch->in_room != original_room)
        if (check_wall_thorns(ch))
                return;
    return;
}



void do_down( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *original_room;
    original_room = ch->in_room;
    if (check_ice_slick(ch))
        return;
    if (check_wall_thorns(ch))
        return;
    if (is_affected(ch,gsn_bloody_shackles))
        {
        damage_old(ch,ch,number_range(140,160),skill_lookup("bloody shackles"),DAM_BASH,TRUE);
        }
    move_char( ch, DIR_DOWN, FALSE );
    if (ch->in_room != original_room)
        if (check_ice_slick(ch))
		return;
    if (ch->in_room != original_room)
        if (check_wall_thorns(ch))
                return;
    return;
}



int find_door( CHAR_DATA *ch, char *arg )
{
    EXIT_DATA *pexit;
    int door;

	 if ( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) ) door = 0;
    else if ( !str_cmp( arg, "e" ) || !str_cmp( arg, "east"  ) ) door = 1;
    else if ( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) ) door = 2;
    else if ( !str_cmp( arg, "w" ) || !str_cmp( arg, "west"  ) ) door = 3;
    else if ( !str_cmp( arg, "u" ) || !str_cmp( arg, "up"    ) ) door = 4;
    else if ( !str_cmp( arg, "d" ) || !str_cmp( arg, "down"  ) ) door = 5;
    else
    {
	for ( door = 0; door <= 5; door++ )
	{
	    if ( ( pexit = ch->in_room->exit[door] ) != NULL
	    &&   IS_SET(pexit->exit_info, EX_ISDOOR)
	    &&   pexit->keyword != NULL
	    &&   is_name( arg, pexit->keyword ) )
		return door;
	}
	act( "I see no $T here.", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
	act( "I see no door $T here.", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    if ( !IS_SET(pexit->exit_info, EX_ISDOOR) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return -1;
    }

    return door;
}



void do_open( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Open what?\n\r", ch );
	return;
    }


    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
 	/* open portal */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (!IS_SET(obj->value[1], EX_ISDOOR))
	    {
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1], EX_CLOSED))
	    {
		send_to_char("It's already open.\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1], EX_LOCKED))
	    {
		send_to_char("It's locked.\n\r",ch);
		return;
	    }

	    REMOVE_BIT(obj->value[1], EX_CLOSED);
	    act("You open $p.",ch,obj,NULL,TO_CHAR);
	    act("$n opens $p.",ch,obj,NULL,TO_ROOM);
	    return;
 	}

	/* 'open object' */
	if ( obj->item_type != ITEM_CONTAINER)
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's already open.\n\r",      ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSEABLE) )
	    { send_to_char( "You can't do that.\n\r",      ch ); return; }
	if ( IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's locked.\n\r",            ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_CLOSED);
	act("You open $p.",ch,obj,NULL,TO_CHAR);
	act( "$n opens $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'open door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's already open.\n\r",      ch ); return; }
	if (  IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's locked.\n\r",            ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_CLOSED);
	act( "$n opens the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	send_to_char( "Ok.\n\r", ch );

	/* open the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "The $d opens.", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
        return;
    }

    return;
}



void do_close( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Close what?\n\r", ch );
	return;
    }


    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{

	    if (!IS_SET(obj->value[1],EX_ISDOOR)
	    ||   IS_SET(obj->value[1],EX_NOCLOSE))
	    {
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("It's already closed.\n\r",ch);
		return;
	    }

	    SET_BIT(obj->value[1],EX_CLOSED);
	    act("You close $p.",ch,obj,NULL,TO_CHAR);
	    act("$n closes $p.",ch,obj,NULL,TO_ROOM);
	    return;
	}

	/* 'close object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's already closed.\n\r",    ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSEABLE) )
	    { send_to_char( "You can't do that.\n\r",      ch ); return; }

	SET_BIT(obj->value[1], CONT_CLOSED);
	act("You close $p.",ch,obj,NULL,TO_CHAR);
	act( "$n closes $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'close door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's already closed.\n\r",    ch ); return; }

	SET_BIT(pexit->exit_info, EX_CLOSED);
	act( "$n closes the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	send_to_char( "Ok.\n\r", ch );

	/* close the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	    SET_BIT( pexit_rev->exit_info, EX_CLOSED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "The $d closes.", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
        return;
    }

    return;
}



bool has_key( CHAR_DATA *ch, int key )
{
    OBJ_DATA *obj;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->pIndexData->vnum == key )
	    return TRUE;
    }

    return FALSE;
}



void do_lock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Lock what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (!IS_SET(obj->value[1],EX_ISDOOR)
	    ||  IS_SET(obj->value[1],EX_NOCLOSE))
	    {
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }
	    if (!IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("It's not closed.\n\r",ch);
	 	return;
	    }

	    if (obj->value[4] < 0 || IS_SET(obj->value[1],EX_NOLOCK))
	    {
		send_to_char("It can't be locked.\n\r",ch);
		return;
	    }

	    if (!has_key(ch,obj->value[4]))
	    {
		send_to_char("You lack the key.\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1],EX_LOCKED))
	    {
		send_to_char("It's already locked.\n\r",ch);
		return;
	    }

	    SET_BIT(obj->value[1],EX_LOCKED);
	    act("You lock $p.",ch,obj,NULL,TO_CHAR);
	    act("$n locks $p.",ch,obj,NULL,TO_ROOM);
	    return;
	}

	/* 'lock object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be locked.\n\r",     ch ); return; }
	if ( !has_key( ch, obj->value[2] ) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already locked.\n\r",    ch ); return; }

	SET_BIT(obj->value[1], CONT_LOCKED);
	act("You lock $p.",ch,obj,NULL,TO_CHAR);
	act( "$n locks $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'lock door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 )
	    { send_to_char( "It can't be locked.\n\r",     ch ); return; }
	if ( !has_key( ch, pexit->key) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's already locked.\n\r",    ch ); return; }

	SET_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n locks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

	/* lock the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    SET_BIT( pexit_rev->exit_info, EX_LOCKED );
	}
    }

    return;
}



void do_unlock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Unlock what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
 	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (!IS_SET(obj->value[1],EX_ISDOOR))
	    {
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("It's not closed.\n\r",ch);
		return;
	    }

	    if (obj->value[4] < 0)
	    {
		send_to_char("It can't be unlocked.\n\r",ch);
		return;
	    }

	    if (!has_key(ch,obj->value[4]))
	    {
		send_to_char("You lack the key.\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1],EX_LOCKED))
	    {
		send_to_char("It's already unlocked.\n\r",ch);
		return;
	    }

	    REMOVE_BIT(obj->value[1],EX_LOCKED);
	    act("You unlock $p.",ch,obj,NULL,TO_CHAR);
	    act("$n unlocks $p.",ch,obj,NULL,TO_ROOM);
	    return;
	}

	/* 'unlock object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !has_key( ch, obj->value[2] ) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_LOCKED);
	act("You unlock $p.",ch,obj,NULL,TO_CHAR);
	act( "$n unlocks $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'unlock door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !has_key( ch, pexit->key) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n unlocks the $d.", ch, NULL, pexit->keyword, TO_ROOM );

	/* unlock the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	}
    }

    return;
}



void do_pick( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Pick what?\n\r", ch );
	return;
    }

    WAIT_STATE( ch, skill_table[gsn_pick_lock].beats );

    /* look for guards */
    for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
    {
	if ( IS_NPC(gch) && IS_AWAKE(gch) && ch->level + 5 < gch->level )
	{
	    act( "$N is standing too close to the lock.",
		ch, NULL, gch, TO_CHAR );
	    return;
	}
    }

    if ( !IS_NPC(ch) && number_percent( ) > get_skill(ch,gsn_pick_lock))
    {
	send_to_char( "You failed.\n\r", ch);
	check_improve(ch,gsn_pick_lock,FALSE,2);
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* portal stuff */
	if (obj->item_type == ITEM_PORTAL)
	{
	    if (!IS_SET(obj->value[1],EX_ISDOOR))
	    {
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }

	    if (!IS_SET(obj->value[1],EX_CLOSED))
	    {
		send_to_char("It's not closed.\n\r",ch);
		return;
	    }

	    if (obj->value[4] < 0)
	    {
		send_to_char("It can't be unlocked.\n\r",ch);
		return;
	    }

	    if (IS_SET(obj->value[1],EX_PICKPROOF))
	    {
		send_to_char("You failed.\n\r",ch);
		return;
	    }

	    REMOVE_BIT(obj->value[1],EX_LOCKED);
	    act("You pick the lock on $p.",ch,obj,NULL,TO_CHAR);
	    act("$n picks the lock on $p.",ch,obj,NULL,TO_ROOM);
	    check_improve(ch,gsn_pick_lock,TRUE,2);
	    return;
	}





	/* 'pick object' */
	if ( obj->item_type != ITEM_CONTAINER )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->value[2] < 0 )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !IS_SET(obj->value[1], CONT_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }
	if ( IS_SET(obj->value[1], CONT_PICKPROOF) )
	    { send_to_char( "You failed.\n\r",             ch ); return; }

	REMOVE_BIT(obj->value[1], CONT_LOCKED);
        act("You pick the lock on $p.",ch,obj,NULL,TO_CHAR);
        act("$n picks the lock on $p.",ch,obj,NULL,TO_ROOM);
	check_improve(ch,gsn_pick_lock,TRUE,2);
	return;
    }

    if ( ( door = find_door( ch, arg ) ) >= 0 )
    {
	/* 'pick door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) && !IS_IMMORTAL(ch))
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->key < 0 && !IS_IMMORTAL(ch))
	    { send_to_char( "It can't be picked.\n\r",     ch ); return; }
	if ( !IS_SET(pexit->exit_info, EX_LOCKED) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }
	if ( IS_SET(pexit->exit_info, EX_PICKPROOF) && !IS_IMMORTAL(ch))
	    { send_to_char( "You failed.\n\r",             ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_LOCKED);
	send_to_char( "*Click*\n\r", ch );
	act( "$n picks the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	check_improve(ch,gsn_pick_lock,TRUE,2);

	/* pick the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    REMOVE_BIT( pexit_rev->exit_info, EX_LOCKED );
	}
    }

    return;
}




void do_stand( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj = NULL;

    if (argument[0] != '\0')
    {
	if (ch->position == POS_FIGHTING)
	{
	    send_to_char("Maybe you should finish fighting first?\n\r",ch);
	    return;
	}
	obj = get_obj_list(ch,argument,ch->in_room->contents);
	if (obj == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
	if (obj->item_type != ITEM_FURNITURE
	||  (!IS_SET(obj->value[2],STAND_AT)
	&&   !IS_SET(obj->value[2],STAND_ON)
	&&   !IS_SET(obj->value[2],STAND_IN)))
	{
	    send_to_char("You can't seem to find a place to stand.\n\r",ch);
	    return;
	}
	if (ch->on != obj && count_users(obj) >= obj->value[0])
	{
	    act_new("There's no room to stand on $p.",
		ch,obj,NULL,TO_CHAR,POS_DEAD);
	    return;
	}
 	ch->on = obj;
    }

    switch ( ch->position )
    {
    case POS_SLEEPING:
	if ( IS_AFFECTED(ch, AFF_SLEEP) || is_affected(ch,gsn_blackjack) ||
is_affected(ch,gsn_stun))
	    { send_to_char( "You can't wake up!\n\r", ch ); return; }

	if (obj == NULL)
	{
	    if (is_affected(ch,gsn_camp))
	    {
		send_to_char("You break camp feeling more refreshed.\n\r",ch);
		affect_strip(ch,gsn_camp);
	    }
	    send_to_char( "You wake and stand up.\n\r", ch );
	    act( "$n wakes and stands up.", ch, NULL, NULL, TO_ROOM );
	    ch->on = NULL;
	}
	else if (IS_SET(obj->value[2],STAND_AT))
	{
	   act_new("You wake and stand at $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	   act("$n wakes and stands at $p.",ch,obj,NULL,TO_ROOM);
	}
	else if (IS_SET(obj->value[2],STAND_ON))
	{
	    act_new("You wake and stand on $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	    act("$n wakes and stands on $p.",ch,obj,NULL,TO_ROOM);
	}
	else
	{
	    act_new("You wake and stand in $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	    act("$n wakes and stands in $p.",ch,obj,NULL,TO_ROOM);
	}
	ch->position = POS_STANDING;
	break;

    case POS_RESTING: case POS_SITTING:
	if (obj == NULL)
	{
	    send_to_char( "You stand up.\n\r", ch );
	    act( "$n stands up.", ch, NULL, NULL, TO_ROOM );
	    ch->on = NULL;
	}
	else if (IS_SET(obj->value[2],STAND_AT))
	{
	    act("You stand at $p.",ch,obj,NULL,TO_CHAR);
	    act("$n stands at $p.",ch,obj,NULL,TO_ROOM);
	}
	else if (IS_SET(obj->value[2],STAND_ON))
	{
	    act("You stand on $p.",ch,obj,NULL,TO_CHAR);
	    act("$n stands on $p.",ch,obj,NULL,TO_ROOM);
	}
	else
	{
	    act("You stand in $p.",ch,obj,NULL,TO_CHAR);
	    act("$n stands on $p.",ch,obj,NULL,TO_ROOM);
	}
	ch->position = POS_STANDING;
	break;

    case POS_STANDING:
	send_to_char( "You are already standing.\n\r", ch );
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_rest( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj = NULL;

    if (ch->position == POS_FIGHTING)
    {
	send_to_char("You are already fighting!\n\r",ch);
	return;
    }

            if (is_affected(ch, gsn_bloodthirst))
            {
                send_to_char("No way! You are still bloodthirsty!\n\r",ch);
                return;
            }


    /* okay, now that we know we can rest, find an object to rest on */
    if (argument[0] != '\0')
    {
	obj = get_obj_list(ch,argument,ch->in_room->contents);
	if (obj == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
    }
    else obj = ch->on;

    if (obj != NULL)
    {
        if (!IS_SET(obj->item_type,ITEM_FURNITURE)
    	||  (!IS_SET(obj->value[2],REST_ON)
    	&&   !IS_SET(obj->value[2],REST_IN)
    	&&   !IS_SET(obj->value[2],REST_AT)))
    	{
	    send_to_char("You can't rest on that.\n\r",ch);
	    return;
    	}

        if (obj != NULL && ch->on != obj && count_users(obj) >= obj->value[0])
        {
	    act_new("There's no more room on $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	    return;
    	}

	if (obj->pIndexData->vnum == OBJ_VNUM_EMPERORTHRONE) {
        if (!IS_NPC(ch) && ch->pcdata->empire !=EMPIRE_EMPEROR)
        {
	act("$n is rejected by $p!",ch,obj,NULL,TO_ROOM);
        damage( ch, ch, ((ch->level)*200), TYPE_HIT, DAM_OTHER, TRUE );
        return;
        }
}
	ch->on = obj;
    }

    switch ( ch->position )
    {
    case POS_SLEEPING:
	if (IS_AFFECTED(ch,AFF_SLEEP) || is_affected(ch,gsn_blackjack) ||
is_affected(ch,gsn_stun))
	{
	    send_to_char("You can't wake up!\n\r",ch);
	    return;
	}

	if (obj == NULL)
	{
	    if (is_affected(ch,gsn_camp))
	    {
		send_to_char("You break camp feeling more refreshed.\n\r",ch);
		affect_strip(ch,gsn_camp);
	    }
	    send_to_char( "You wake up and start resting.\n\r", ch );
	    act ("$n wakes up and starts resting.",ch,NULL,NULL,TO_ROOM);
	}
	else if (IS_SET(obj->value[2],REST_AT))
	{
	    act_new("You wake up and rest at $p.",
		    ch,obj,NULL,TO_CHAR,POS_SLEEPING);
	    act("$n wakes up and rests at $p.",ch,obj,NULL,TO_ROOM);
	}
        else if (IS_SET(obj->value[2],REST_ON))
        {
            act_new("You wake up and rest on $p.",
                    ch,obj,NULL,TO_CHAR,POS_SLEEPING);
            act("$n wakes up and rests on $p.",ch,obj,NULL,TO_ROOM);
        }
        else
        {
            act_new("You wake up and rest in $p.",
                    ch,obj,NULL,TO_CHAR,POS_SLEEPING);
            act("$n wakes up and rests in $p.",ch,obj,NULL,TO_ROOM);
        }
	ch->position = POS_RESTING;
	break;

    case POS_RESTING:
	send_to_char( "You are already resting.\n\r", ch );
	break;

    case POS_STANDING:
	if (obj == NULL)
	{
	    send_to_char( "You rest.\n\r", ch );
	    act( "$n sits down and rests.", ch, NULL, NULL, TO_ROOM );
	}
        else if (IS_SET(obj->value[2],REST_AT))
        {
	    act("You sit down at $p and rest.",ch,obj,NULL,TO_CHAR);
	    act("$n sits down at $p and rests.",ch,obj,NULL,TO_ROOM);
        }
        else if (IS_SET(obj->value[2],REST_ON))
        {
	    act("You sit on $p and rest.",ch,obj,NULL,TO_CHAR);
	    act("$n sits on $p and rests.",ch,obj,NULL,TO_ROOM);
        }
        else
        {
	    act("You rest in $p.",ch,obj,NULL,TO_CHAR);
	    act("$n rests in $p.",ch,obj,NULL,TO_ROOM);
        }
	ch->position = POS_RESTING;
	break;

    case POS_SITTING:
	if (obj == NULL)
	{
	    send_to_char("You rest.\n\r",ch);
	    act("$n rests.",ch,NULL,NULL,TO_ROOM);
	}
        else if (IS_SET(obj->value[2],REST_AT))
        {
	    act("You rest at $p.",ch,obj,NULL,TO_CHAR);
	    act("$n rests at $p.",ch,obj,NULL,TO_ROOM);
        }
        else if (IS_SET(obj->value[2],REST_ON))
        {
	    act("You rest on $p.",ch,obj,NULL,TO_CHAR);
	    act("$n rests on $p.",ch,obj,NULL,TO_ROOM);
        }
        else
        {
	    act("You rest in $p.",ch,obj,NULL,TO_CHAR);
	    act("$n rests in $p.",ch,obj,NULL,TO_ROOM);
	}
	ch->position = POS_RESTING;
	break;
    }


    return;
}


void do_sit (CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj = NULL;

    if (ch->position == POS_FIGHTING)
    {
	send_to_char("Maybe you should finish this fight first?\n\r",ch);
	return;
    }

            if (is_affected(ch, gsn_bloodthirst))
            {
                send_to_char("No way! You are still bloodthirsty!\n\r",ch);
                return;
            }


    /* okay, now that we know we can sit, find an object to sit on */
    if (argument[0] != '\0')
    {
	obj = get_obj_list(ch,argument,ch->in_room->contents);
	if (obj == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
    }
    else obj = ch->on;

    if (obj != NULL)
    {
	if (!IS_SET(obj->item_type,ITEM_FURNITURE)
	||  (!IS_SET(obj->value[2],SIT_ON)
	&&   !IS_SET(obj->value[2],SIT_IN)
	&&   !IS_SET(obj->value[2],SIT_AT)))
	{
	    send_to_char("You can't sit on that.\n\r",ch);
	    return;
	}

	if (obj != NULL && ch->on != obj && count_users(obj) >= obj->value[0])
	{
	    act_new("There's no more room on $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
	    return;
	}

	if (obj->pIndexData->vnum == OBJ_VNUM_EMPERORTHRONE) {
	if (!IS_NPC(ch) && (ch->pcdata->empire == 0 || ch->pcdata->empire !=EMPIRE_EMPEROR))
		{
		act("$n is rejected by $p!",ch,obj,NULL,TO_ROOM);
		damage( ch, ch, ((ch->level)*200), TYPE_HIT, DAM_OTHER, TRUE );
		return;
		}
	}
	ch->on = obj;
    }
    switch (ch->position)
    {
	case POS_SLEEPING:
	    if (IS_AFFECTED(ch,AFF_SLEEP) || is_affected(ch,gsn_blackjack) || is_affected(ch,gsn_stun))
	    {
		send_to_char("You can't wake up!\n\r",ch);
		return;
	    }

            if (obj == NULL)
            {
            	send_to_char( "You wake and sit up.\n\r", ch );
            	act( "$n wakes and sits up.", ch, NULL, NULL, TO_ROOM );
            }
            else if (IS_SET(obj->value[2],SIT_AT))
            {
            	act_new("You wake and sit at $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
            	act("$n wakes and sits at $p.",ch,obj,NULL,TO_ROOM);
            }
            else if (IS_SET(obj->value[2],SIT_ON))
			{
			if (obj->pIndexData->vnum == 27)
			{
				if ((is_name("Graelik",ch->name)) || (is_name("Graelik",ch->original_name)))
				{
					act("You sit on $p and rest.",ch,obj,NULL,TO_CHAR);
					act("$n wakes and sits on $p and rests.",ch,obj,NULL,TO_ROOM);
				}
				else
				{
					act("The mere touch of $p burns you to a crisp.",ch,obj,NULL,TO_CHAR);
					act("$n wakes and tries to sit on $p, but merely touching it burns them to a crisp.",ch,obj,NULL,TO_ROOM);
					raw_kill(ch,ch);
				}
			}
			else
				{
            		act_new("You wake and sit in $p.",ch,obj,NULL,TO_CHAR,POS_DEAD);
            		act("$n wakes and sits in $p.",ch,obj,NULL,TO_ROOM);
				}
			}
	    ch->position = POS_SITTING;
	    break;
	case POS_RESTING:
	    if (obj == NULL)
		send_to_char("You stop resting.\n\r",ch);
	    else if (IS_SET(obj->value[2],SIT_AT))
	    {
		act("You sit at $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits at $p.",ch,obj,NULL,TO_ROOM);
	    }

	    else if (IS_SET(obj->value[2],SIT_ON))
	    {
			if (obj->pIndexData->vnum == 27)
			{
				if ((is_name("Graelik",ch->name)) || (is_name("Graelik",ch->original_name)))
				{
					act("You sit on $p and rest.",ch,obj,NULL,TO_CHAR);
					act("$n sits on $p and rests.",ch,obj,NULL,TO_ROOM);
				}
				else
				{
					act("The mere touch of $p burns you to a crisp.",ch,obj,NULL,TO_CHAR);
					act("$n tries to sit on $p, but merely touching it burns them to a crisp.",ch,obj,NULL,TO_ROOM);
					raw_kill(ch,ch);
				}
			}
			else
			{
				act("You sit on $p and rest.",ch,obj,NULL,TO_CHAR);
				act("$n sits on $p and rests.",ch,obj,NULL,TO_ROOM);
			}
	    }
	    ch->position = POS_SITTING;
	    break;
	case POS_SITTING:
	    send_to_char("You are already sitting down.\n\r",ch);
	    break;
	case POS_STANDING:
	    if (obj == NULL)
    	    {
		send_to_char("You sit down.\n\r",ch);
    	        act("$n sits down on the ground.",ch,NULL,NULL,TO_ROOM);
	    }
	    else if (IS_SET(obj->value[2],SIT_AT))
	    {
		act("You sit down at $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits down at $p.",ch,obj,NULL,TO_ROOM);
	    }
	    else if (IS_SET(obj->value[2],SIT_ON))
	    {
			if (obj->pIndexData->vnum == 27)
			{
				if ((is_name("Graelik",ch->name)) || (is_name("Graelik",ch->original_name)))
				{
					act("You sit on $p and rest.",ch,obj,NULL,TO_CHAR);
					act("$n sits on $p and rests.",ch,obj,NULL,TO_ROOM);
				}
				else
				{
					act("The mere touch of $p burns you to a crisp.",ch,obj,NULL,TO_CHAR);
					act("$n tries to sit on $p, but merely touching it burns them to a crisp.",ch,obj,NULL,TO_ROOM);
					raw_kill(ch,ch);
				}
			}
			else
			{
				act("You sit on $p and rest.",ch,obj,NULL,TO_CHAR);
				act("$n sits on $p and rests.",ch,obj,NULL,TO_ROOM);
			}
		}
	    else
	    {
		act("You sit down in $p.",ch,obj,NULL,TO_CHAR);
		act("$n sits down in $p.",ch,obj,NULL,TO_ROOM);
	    }
    	    ch->position = POS_SITTING;
    	    break;
    }
    return;
}


void do_sleep( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj = NULL;

    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are already sleeping.\n\r", ch );
	break;

    case POS_RESTING:
    case POS_SITTING:
    case POS_STANDING:

            if (is_affected(ch, gsn_bloodthirst))
            {
                send_to_char("No way! You are still bloodthirsty!\n\r",ch);
                return;
            }

	if (argument[0] == '\0' && ch->on == NULL)
	{
	    send_to_char( "You go to sleep.\n\r", ch );
	    act( "$n goes to sleep.", ch, NULL, NULL, TO_ROOM );
	    ch->position = POS_SLEEPING;
	}
	else  /* find an object and sleep on it */
	{
	    if (argument[0] == '\0')
		obj = ch->on;
	    else
	    	obj = get_obj_list( ch, argument,  ch->in_room->contents );

	    if (obj == NULL)
	    {
		send_to_char("You don't see that here.\n\r",ch);
		return;
	    }
	    if (obj->item_type != ITEM_FURNITURE
	    ||  (!IS_SET(obj->value[2],SLEEP_ON)
	    &&   !IS_SET(obj->value[2],SLEEP_IN)
	    &&	 !IS_SET(obj->value[2],SLEEP_AT)))
	    {
		send_to_char("You can't sleep on that!\n\r",ch);
		return;
	    }

	    if (ch->on != obj && count_users(obj) >= obj->value[0])
	    {
		act_new("There is no room on $p for you.",
		    ch,obj,NULL,TO_CHAR,POS_DEAD);
		return;
	    }

	    ch->on = obj;
	    if (IS_SET(obj->value[2],SLEEP_AT))
	    {
		act("You go to sleep at $p.",ch,obj,NULL,TO_CHAR);
		act("$n goes to sleep at $p.",ch,obj,NULL,TO_ROOM);
	    }
	    else if (IS_SET(obj->value[2],SLEEP_ON))
	    {
	        act("You go to sleep on $p.",ch,obj,NULL,TO_CHAR);
	        act("$n goes to sleep on $p.",ch,obj,NULL,TO_ROOM);
	    }
	    else
	    {
		act("You go to sleep in $p.",ch,obj,NULL,TO_CHAR);
		act("$n goes to sleep in $p.",ch,obj,NULL,TO_ROOM);
	    }
	    ch->position = POS_SLEEPING;
	}
	break;

    case POS_FIGHTING:
	send_to_char( "You are already fighting!\n\r", ch );
	break;
    }

    return;
}



void do_wake( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
	{ do_stand( ch, argument ); return; }

    if ( !IS_AWAKE(ch) )
	{ send_to_char( "You are asleep yourself!\n\r",       ch ); return; }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{ send_to_char( "They aren't here.\n\r",              ch ); return; }

    if ( IS_AWAKE(victim) )
	{ act( "$N is already awake.", ch, NULL, victim, TO_CHAR ); return; }

    if ( IS_AFFECTED(victim, AFF_SLEEP) || is_affected(ch,gsn_blackjack))
	{ act( "You can't wake $M!",   ch, NULL, victim, TO_CHAR );  return; }

    act_new( "$n wakes you.", ch, NULL, victim, TO_VICT,POS_SLEEPING );
    do_stand(victim,"");
    return;
}


void do_sneak( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA af;

    int sn_fog, sn_fire;

    sn_fog = skill_lookup("faerie fog");
    sn_fire = skill_lookup("faerie fire");
    if (is_affected(ch,sn_fog) || is_affected(ch,sn_fire) || is_affected(ch,gsn_incandescense))
    {
	send_to_char("You can't hide while glowing.\n\r",ch);
	return;
    }

    send_to_char( "You attempt to move silently.\n\r", ch );
    if (IS_AFFECTED(ch,AFF_SNEAK))
	return;

    if ( number_percent( ) < get_skill(ch,gsn_sneak))
    {
	check_improve(ch,gsn_sneak,TRUE,3);
	init_affect(&af);
	af.where     = TO_AFFECTS;
	af.type      = gsn_sneak;
	af.aftype    = AFT_SKILL;
	af.level     = ch->level;
	af.duration  = ch->level;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.bitvector = AFF_SNEAK;
	affect_to_char( ch, &af );
        send_to_char( "You begin sneaking.\n\r", ch);
    }
    else
	check_improve(ch,gsn_sneak,FALSE,3);

    return;
}

void do_cloak( CHAR_DATA *ch, char *argument )
{
	AFFECT_DATA af;
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_STRING_LENGTH];

	argument = one_argument(argument,arg);

	if (is_affected(ch,gsn_cloak_form))
	{
	    if(is_affected(ch, gsn_cloak_form))
	    {
		affect_strip(ch,gsn_cloak_form);
	    }
	    if(ch->original_name)
	    {
		free_string(ch->name);
		ch->name=str_dup(ch->original_name);
		free_string(ch->original_name);
		ch->original_name=NULL;
	    }
	    if(is_affected(ch, gsn_eye_of_the_predator)) {
		affect_strip(ch,gsn_eye_of_the_predator);
                }
	if (str_cmp(arg,"auto") )
	    send_to_char("You throw your cloak to the side, exposing yourself.\n\r",ch);

	    return;
	  }

	if (number_percent() < get_skill(ch,gsn_cloak_form))
	{
        init_affect(&af);
	  af.where    = TO_AFFECTS;
          af.aftype   = AFT_POWER;
	  af.type     = gsn_cloak_form;
 	  af.level    = ch->level;
	  af.location = APPLY_NONE;
	  af.modifier = 0;
	  af.bitvector = AFF_SNEAK;
	  af.duration = -1;
	  affect_to_char( ch, &af );
	  if(!(ch->original_name)) {
	 	free_string(ch->original_name);
		ch->original_name=str_dup(ch->name);
		free_string(ch->name);
		sprintf(buf, "cloaked figure");
		ch->name=str_dup(buf);
	  }
	  af.location = APPLY_HIT;
	  af.modifier = ch->level*4;
	  affect_to_char(ch,&af);
	  af.location = APPLY_HITROLL;
	  af.modifier = (ch->level)/6;
	  affect_to_char(ch,&af);
	  af.location = APPLY_DAMROLL;
	  affect_to_char(ch,&af);
	  af.location = APPLY_MOVE;
	  af.modifier = ch->level*2;
	  affect_to_char(ch,&af);
	  check_improve(ch,gsn_cloak_form,TRUE,6);
	  send_to_char("You cloak your presence.\n\r",ch);
	  return;
	}
	else
	{
	  check_improve(ch,gsn_cloak_form, FALSE,3);
	  send_to_char("You fail to cloak yourself.\n\r",ch);
	  return;
	}
}

void do_acute_vision( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA  af;

    if (get_skill(ch,gsn_acute_vision) == 0
	|| ch->level < skill_table[gsn_acute_vision].skill_level[ch->class])
    {
	send_to_char("You don't know which bushes to look at.\n\r",ch);
	return;
    }

    if (is_affected(ch,gsn_acute_vision) )
    {
	send_to_char("Your eyes are already as sharp as they'll get.\n\r", ch);
	return;
    }

   if (ch->mana < 25)
   {
    send_to_char("You don't have the mana.\n\r", ch);
	return;
    }

    if (number_percent() > get_skill(ch,gsn_acute_vision))
    {
	send_to_char("Your vision sharpens but then dulls.\n\r",ch);
	check_improve(ch,gsn_acute_vision,FALSE,1);
    ch->mana -= 12;
	return;
    }

        init_affect(&af);
    af.where = TO_AFFECTS;
    af.type = gsn_acute_vision;
        af.aftype    = AFT_SKILL;
    af.location = 0;
    af.modifier = 0;
    af.level = ch->level;
    af.duration = ch->level;
    af.bitvector = 0;
    affect_to_char(ch, &af);

    ch->mana -= 25;
    send_to_char( "Your vision sharpens.\n\r", ch);
    return;
    check_improve(ch,gsn_acute_vision,TRUE,1);
}

void do_camp(CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA af;
    if ( (get_skill(ch,gsn_camp) == 0) || ch->level < skill_table[gsn_camp].skill_level[ch->class] )
    {
	send_to_char("You don't know how to effectively camp.\n\r",ch);
	return;
    }

    if (ch->position == POS_SLEEPING )
    {
	send_to_char("You are already asleep.",ch);
	return;
    }
    if ((ch->in_room->sector_type != SECT_FOREST) && (ch->in_room->sector_type != SECT_HILLS)
    && (ch->in_room->sector_type != SECT_MOUNTAIN) )
    {
	send_to_char("This land is not wild enough for you to camp out in.\n\r",ch);
	return;
    }

        init_affect(&af);
    af.where = TO_AFFECTS;
    af.location = 0;
    af.duration = -1;
    af.modifier = 0;
    af.level = ch->level;
    af.type = gsn_camp;
        af.aftype    = AFT_SKILL;
    af.bitvector = 0;
    affect_to_char(ch,&af);

    act("$n settles down and sets up camp.",ch,NULL,NULL,TO_ROOM);
    send_to_char("You settle down and set up camp.\n\r",ch);
    do_sleep(ch,"");
    return;
}


void do_camouflage( CHAR_DATA *ch, char *argument )
{
    int sn_fog, sn_fire;

    sn_fog = skill_lookup("faerie fog");
    sn_fire = skill_lookup("faerie fire");
    if (is_affected(ch,sn_fog) || is_affected(ch,sn_fire) || is_affected(ch, gsn_incandescense) || is_affected(ch,gsn_dew))
    {
	send_to_char("You can't camouflage while revealed.\n\r",ch);
	return;
     }

    if ( (ch->in_room->sector_type != SECT_FOREST) && (ch->in_room->sector_type != SECT_MOUNTAIN) && (ch->in_room->sector_type != SECT_HILLS ) )
    {
	send_to_char("There is no cover here.\n\r", ch);
	act("$n tries to cover $mself on the single leaf on the ground.", ch, NULL, NULL, TO_ROOM);
	return;
    }

    send_to_char("You attempt to blend in with your surroundings.\n\r", ch);

    if ( number_percent( ) < get_skill(ch,gsn_camouflage) )
    {
	SET_BIT(ch->affected_by, AFF_CAMOUFLAGE);
	check_improve(ch, gsn_camouflage,TRUE,2);
	//WAIT_STATE(ch,8);
    }
    else
    {
	check_improve(ch,gsn_camouflage,FALSE,2);
	REMOVE_BIT(ch->affected_by,AFF_CAMOUFLAGE);
	//WAIT_STATE(ch,8);
    }

    return;
}

/* Creep - Camo sneak -- Dioxide */

void do_creep(CHAR_DATA *ch, char *argument)
{
	int direction=0, chance, wait;
	AFFECT_DATA af;

	if ((chance = get_skill(ch,gsn_creep)) == 0)
	{
      	send_to_char("Creeping? What's that?\n\r",ch);
      	return;
	}

	if (!IS_AFFECTED(ch,AFF_CAMOUFLAGE))
	{
      	send_to_char("You must be blended in with your surroundings to creep.\n\r",ch);
      	return;
	}

	if (argument[0] == '\0') {
      	send_to_char("Creep in which direction?\n\r",ch);
      	return;
	}

	if ( !str_cmp( argument, "n" ) || !str_cmp( argument, "north" ) ) {
		direction = 0;
	} else if ( !str_cmp( argument, "e" ) || !str_cmp( argument, "east"  ) ) {
		direction = 1;
	} else if ( !str_cmp( argument, "s" ) || !str_cmp( argument, "south" ) ) {
		direction = 2;
	} else if ( !str_cmp( argument, "w" ) || !str_cmp( argument, "west"  ) ) {
		direction = 3;
	} else if ( !str_cmp( argument, "u" ) || !str_cmp( argument, "up"    ) ) {
		direction = 4;
	} else if ( !str_cmp( argument, "d" ) || !str_cmp( argument, "down"  ) ) {
		direction = 5;
	} else {
		act("You can't creep there.", ch, NULL, 0, TO_CHAR );
	}
        init_affect(&af);
	af.where     = TO_AFFECTS;
	af.type      = gsn_creep;
        af.aftype    = AFT_SKILL;
	af.bitvector = 0;
	af.modifier  = 0;
	af.location  = 0;
	af.duration  = 1;
	af.level     = ch->level;
	affect_join(ch, &af);
	move_char(ch, direction, FALSE);
	affect_strip(ch,gsn_creep);
	check_improve(ch, gsn_creep,TRUE,2);

	wait = 100 - chance;
	WAIT_STATE(ch,wait);
}

void do_hide( CHAR_DATA *ch, char *argument )
{
    int sn_fog, sn_fire;

    sn_fog = skill_lookup("faerie fog");
    sn_fire = skill_lookup("faerie fire");

    if (is_affected(ch,sn_fog) || is_affected(ch,sn_fire) || is_affected(ch, gsn_incandescense) || is_affected(ch,gsn_dew))
    {
	send_to_char("You can't hide while revealed.\n\r",ch);
	return;
    }

    if ((ch->in_room->sector_type != SECT_CITY) && (ch->in_room->sector_type != SECT_INSIDE))
    {
	send_to_char("The shadows here are too natural to blend with.", ch);
	return;
    }

    send_to_char( "You attempt to hide.\n\r", ch );

    if ( IS_AFFECTED(ch, AFF_HIDE) )
        return;

    if ( number_percent( ) < get_skill(ch,gsn_hide))
    {
	SET_BIT(ch->affected_by, AFF_HIDE);
	check_improve(ch,gsn_hide,TRUE,3);
    }
    else
	check_improve(ch,gsn_hide,FALSE,3);

    return;
}

void un_camouflage( CHAR_DATA *ch, char *argument )
{
    if ( IS_AFFECTED(ch, AFF_CAMOUFLAGE) )
    {
	REMOVE_BIT( ch->affected_by, AFF_CAMOUFLAGE );
	act("$n steps out from their cover.",ch,NULL,NULL,TO_ROOM);
	send_to_char("You step out from your cover.\n\r",ch);
    }
    return;
}

void un_blackjack( CHAR_DATA *ch, char *argument )
{
    if ( is_affected(ch, gsn_blackjack) )
	affect_strip( ch, gsn_blackjack );
    return;
}

void un_strangle( CHAR_DATA *ch, char *argument )
{
    if ( is_affected(ch, gsn_strangle) )
	affect_strip( ch, gsn_strangle );
    return;
}

void un_hide( CHAR_DATA *ch, char *argument )
{
    if ( IS_AFFECTED(ch, AFF_HIDE) )
    {
    REMOVE_BIT   ( ch->affected_by, AFF_HIDE		);
    act("$n steps out of the shadows.", ch, NULL, NULL, TO_ROOM);
    send_to_char("You step out of the shadows.\n\r", ch);
    }
    return;
}

void un_invis( CHAR_DATA *ch, char *argument )
{
    if ( IS_AFFECTED(ch, AFF_INVISIBLE))
    {
    affect_strip ( ch, gsn_invis);
    affect_strip ( ch, gsn_mass_invis);
    REMOVE_BIT   ( ch->affected_by, AFF_INVISIBLE	);
    act("$n fades into existence.\n\r", ch, NULL, NULL, TO_ROOM);
    send_to_char("You fade into existence.\n\r", ch);
    }
    return;
}
void un_sneak( CHAR_DATA *ch, char *argument )
{
    if ( ch->race == 3 || ch->race == 4 || ch->race == 5 )
	return;

    if ( IS_AFFECTED(ch, AFF_SNEAK))
    {
    affect_strip ( ch, gsn_sneak);
    REMOVE_BIT   ( ch->affected_by, AFF_SNEAK		);
    send_to_char("You trample around loudly again.\n\r", ch);
    }
    return;
}

void un_earthfade(CHAR_DATA *ch,char *argument)
{
	if (is_affected(ch,gsn_earthfade)) {

	affect_strip(ch,gsn_earthfade);

	act("$n slowly emerges from the ground.",ch,0,0,TO_ROOM);
	send_to_char("You slowly emerge from the ground.\n\r",ch);
	WAIT_STATE(ch,8);
	}

	return;
}

void un_duo(CHAR_DATA *ch,char *argument)
{
	if (is_affected(ch,gsn_duo))
	{
	affect_strip(ch,gsn_duo);
	act("$n pops back into existence.",ch,0,0,TO_ROOM);
	send_to_char("You step back into three dimensions.\n\r",ch);
	WAIT_STATE(ch,2);
	}
	return;
}

void do_visible( CHAR_DATA *ch, char *argument )
{
	un_earthfade(ch,NULL);
	un_camouflage(ch, NULL);
        un_hide(ch, NULL);
        un_invis(ch, NULL);
        un_sneak(ch, NULL);
	un_duo(ch,NULL);
        return;
}

void do_recall( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *location;
    AFFECT_DATA af;

    if (IS_NPC(ch) && !IS_SET(ch->act,ACT_PET))
    {
	send_to_char("Only players can recall.\n\r",ch);
	return;
    }
    if (ch->level > 10 && ch->cabal != 4)
    {
        send_to_char("Those over level 10 cannot recall.\n\r",ch);
        return;
    }

    if (ch->level > 10 && ch->cabal == CABAL_RAVAGER)
    {

       if (is_affected(ch,gsn_recall))
	{
  	  send_to_char("You have already been granted recall by your Immortal recently.\n\r",ch);
	  return;
	}


	if (ch->fighting != NULL || ch->position == POS_FIGHTING)
	{
  	 send_to_char("You are a Rager..you don't ask to be taken from battles!\n\r",ch);
	 return;
 	}

       send_to_char("You pray to the Immortals of Ragers for recall!\n\r",ch);
       WAIT_STATE( ch, 5 );

       if (cabal_down(ch,CABAL_RAVAGER))
        {

        if ( number_percent() > 20)
        {
            sprintf( buf, "You try to recall but lack of power cases you to fail.\n\r");
            send_to_char( buf, ch );
            return;
        }
        else
        {
          send_to_char("Head is stolen but your desperation is rewarded.\n\r",ch);
        }
       }
    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_AFFECTED(ch, AFF_CURSE))
    {
        send_to_char( "The gods have forsaken you.\n\r", ch );
	return;
    }
	act("$n calls to $s Immortal leader and vanishes!",ch,0,0,TO_ROOM);

	location=get_room_index(TEMPLE_MIDGAARD);
	if (location == NULL)
		location = get_room_index(ROOM_VNUM_TEMPLE);

	if (location == NULL)
	{
	send_to_char("Your temple has been destroyed..pray to the Gods for help!\n\r",ch);
	return;
	}
	char_from_room(ch);
	char_to_room(ch,location);
	do_look(ch,"auto");
        init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = gsn_recall;
        af.aftype    = AFT_POWER;
	af.duration = 36;
	af.modifier = 0;
	af.bitvector = 0;
	af.location = 0;
	af.level=ch->level;
	affect_to_char(ch,&af);
	return;
    }

    act( "$n prays for transportation!", ch, 0, 0, TO_ROOM );

    location = get_room_index(ch->temple);
    if (location == NULL)
	location = get_room_index(ROOM_VNUM_TEMPLE);
/*
    if ( ( location = get_room_index( ROOM_VNUM_TEMPLE ) ) == NULL )
*/
    if (location == NULL)
    {
	send_to_char( "You are completely lost.\n\r", ch );
	return;
    }

    if ( ch->in_room == location )
	return;

    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_AFFECTED(ch, AFF_CURSE))
    {
        send_to_char( "The gods have forsaken you.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) != NULL )
    {
	int lose,skill;

	skill = get_skill(ch,gsn_recall);

	if ( number_percent() < 80 * skill / 100 )
	{
	    check_improve(ch,gsn_recall,FALSE,6);
	    WAIT_STATE( ch, 4 );
	    sprintf( buf, "You failed!.\n\r");
	    send_to_char( buf, ch );
	    return;
	}

	lose = (ch->desc != NULL) ? 25 : 50;
	gain_exp( ch, 0 - lose );
	check_improve(ch,gsn_recall,TRUE,4);
	sprintf( buf, "You recall from combat!  You lose %d exps.\n\r", lose );
	send_to_char( buf, ch );
	stop_fighting( ch, TRUE );

    }

    ch->move /= 2;
    act( "$n disappears.", ch, NULL, NULL, TO_ROOM );
    char_from_room( ch );
    char_to_room( ch, location );
    act( "$n appears in the room.", ch, NULL, NULL, TO_ROOM );
    do_look( ch, "auto" );

    if (ch->pet != NULL)
	do_recall(ch->pet,"");

    return;
}



void do_train( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *mob;
    sh_int stat = - 1;
    char *pOutput = NULL;
    int cost;

    if ( IS_NPC(ch) )
	return;

    /*
     * Check for trainer.
     */
    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
	if ( IS_NPC(mob) && IS_SET(mob->act, ACT_TRAIN) )
	    break;
    }

    if ( mob == NULL )
    {
	send_to_char( "You can't do that here.\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	sprintf( buf, "You have %d training sessions.\n\r", ch->train );
	send_to_char( buf, ch );
	argument = "foo";
    }

    cost = 1;

    if ( !str_cmp( argument, "str" ) )
    {
	stat        = STAT_STR;
	pOutput     = "strength";
    }

    else if ( !str_cmp( argument, "int" ) )
    {
	stat	    = STAT_INT;
	pOutput     = "intelligence";
    }

    else if ( !str_cmp( argument, "wis" ) )
    {
	stat	    = STAT_WIS;
	pOutput     = "wisdom";
    }

    else if ( !str_cmp( argument, "dex" ) )
    {
	stat  	    = STAT_DEX;
	pOutput     = "dexterity";
    }

    else if ( !str_cmp( argument, "con" ) )
    {
	stat	    = STAT_CON;
	pOutput     = "constitution";
    }

    else if ( !str_cmp( argument, "chr" ) )
    {
	stat	    = STAT_CHR;
	pOutput     = "charisma";
    }

    else if ( !str_cmp(argument, "hp" ) )
	cost = 1;

    else if ( !str_cmp(argument, "mana" ) )
	cost = 1;

    else
    {
	strcpy( buf, "You can train:" );
	if ( ch->perm_stat[STAT_STR] < get_max_train(ch,STAT_STR))
	    strcat( buf, " str" );
	if ( ch->perm_stat[STAT_INT] < get_max_train(ch,STAT_INT))
	    strcat( buf, " int" );
	if ( ch->perm_stat[STAT_WIS] < get_max_train(ch,STAT_WIS))
	    strcat( buf, " wis" );
	if ( ch->perm_stat[STAT_DEX] < get_max_train(ch,STAT_DEX))
	    strcat( buf, " dex" );
	if ( ch->perm_stat[STAT_CON] < get_max_train(ch,STAT_CON))
	    strcat( buf, " con" );
	if ( ch->perm_stat[STAT_CHR] < get_max_train(ch,STAT_CHR))
	    strcat( buf, " chr" );
    strcat( buf, " hp mana");

	if ( buf[strlen(buf)-1] != ':' )
	{
	    strcat( buf, ".\n\r" );
	    send_to_char( buf, ch );
	}
	else
	{
	    /*
	     * This message dedicated to Jordan ... you big stud!
	     */
	    act( "You have nothing left to train, you $T!",
		ch, NULL,
		ch->sex == SEX_MALE   ? "big stud" :
		ch->sex == SEX_FEMALE ? "hot babe" :
					"wild thing",
		TO_CHAR );
	}

	return;
    }

    if (!str_cmp("hp",argument))
    {
    	if ( cost > ch->train )
    	{
       	    send_to_char( "You don't have enough training sessions.\n\r", ch );
            return;
        }

	ch->train -= cost;
        ch->pcdata->perm_hit += 10;
        ch->max_hit += 10;
        ch->hit +=10;
        act( "Your durability increases!",ch,NULL,NULL,TO_CHAR);
        act( "$n's durability increases!",ch,NULL,NULL,TO_ROOM);
        return;
    }

    if (!str_cmp("mana",argument))
    {
        if ( cost > ch->train )
        {
            send_to_char( "You don't have enough training sessions.\n\r", ch );
            return;
        }

	ch->train -= cost;
        ch->pcdata->perm_mana += 10;
        ch->max_mana += 10;
        ch->mana += 10;
        act( "Your power increases!",ch,NULL,NULL,TO_CHAR);
        act( "$n's power increases!",ch,NULL,NULL,TO_ROOM);
        return;
    }

    if ( ch->perm_stat[stat]  >= get_max_train(ch,stat) )
    {
	act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }

    if ( cost > ch->train )
    {
	send_to_char( "You don't have enough training sessions.\n\r", ch );
	return;
    }

    ch->train		-= cost;

    ch->perm_stat[stat]		+= 1;
    act( "Your $T increases!", ch, NULL, pOutput, TO_CHAR );
    act( "$n's $T increases!", ch, NULL, pOutput, TO_ROOM );
    return;
}



void do_shadowgate(CHAR_DATA *ch,char *argument)
{
	CHAR_DATA *victim;
	char arg1[MAX_INPUT_LENGTH];

	one_argument(argument,arg1);

        if ( (get_skill(ch,gsn_shadowgate) == 0)
        || (ch->level < skill_table[gsn_shadowgate].skill_level[ch->class]) )
        {
        send_to_char("Huh?\n\r",ch);
        return;
        }

    if (cabal_down(ch,CABAL_ACADIAN))
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

        if (number_percent() > ch->pcdata->learned[gsn_shadowgate])
        {
        send_to_char("You fail to pass through the shadowplanes.\n\r",ch);
        check_improve(ch,gsn_shadowgate,FALSE,1);
        ch->mana -= 35;
        return;
        }

        if ( ((victim = get_char_world(ch,arg1)) == NULL)
        || victim == ch
	|| IS_SET(victim->in_room->room_flags,ROOM_NO_GATE)
        || victim->in_room == NULL
	|| victim->in_room->guild != 0
        || (victim->in_room->cabal != 0 && victim->in_room->cabal != CABAL_ACADIAN)
	|| (!IS_NPC(victim) && victim->level >= 52)
  	|| (!IS_NPC(victim) && !can_pk(ch,victim))
        || (saves_spell(ch->level - 1,victim, DAM_OTHER) && number_percent() > 25)
        ||   IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL) )
        {
                ch->mana -= 35;
                send_to_char("You failed.\n\r",ch);
		WAIT_STATE(ch,12);
                return;
        }

        ch->mana -= 70;

	if (ch->in_room->vnum == 1212) /* No shadowgating from realm of dead */
	{
	send_to_char("You failed.\n\r",ch);
	return;
	}

        act("$n becomes enshrouded in darkness and disappears!",ch,0,0,TO_ROOM);
        char_from_room(ch);
        char_to_room(ch,victim->in_room);
        send_to_char("You step into the shadowplane and vanish.\n\r",ch);
        act("A vortex of shadows blasts into the room and $n step forth!",ch,0,0,TO_ROOM);
        do_look(ch,"auto");
	WAIT_STATE(ch,12);
        return;
}


void do_bear_call( CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *animal;
    AFFECT_DATA af;
    int a_level;
    char arg1[MAX_INPUT_LENGTH];
    CHAR_DATA *check;
    bool found = FALSE;
    int count;

    one_argument(argument,arg1);

    if ( (get_skill(ch,gsn_bear_call) == 0)
    || (ch->level < skill_table[gsn_bear_call].skill_level[ch->class]) )
    {
	send_to_char("You don't know how to call bears?\n\r",ch);
        return;
    }

    if (is_affected(ch,gsn_bear_call))
    {
	send_to_char("You can't call more bears yet.\n\r",ch);
	return;
    }

    if (ch->mana <= 70)
    {
        send_to_char("You do not have the mana.\n\r",ch);
        return;
    }

    for (check = char_list; check != NULL; check = check->next)
    {
        if  ( (check->master == ch)
	 && check->pIndexData->vnum == MOB_VNUM_BEAR )
	found = TRUE;
    }

    if (found)
    {
	send_to_char("You already have bears to care for.\n\r",ch);
	return;
    }

    if ( (ch->in_room->sector_type != SECT_FOREST)
    && (ch->in_room->sector_type != SECT_HILLS)
    && (ch->in_room->sector_type != SECT_MOUNTAIN) )
    {
    act("$n calls out into the surroundings but nothing comes.",ch,0,0,TO_ROOM);
    send_to_char("You call a bear call but nothing responds.\n\r",ch);
    return;
    }

    if (number_percent() > ch->pcdata->learned[gsn_bear_call])
    {
	send_to_char("You call out for bears but none respond.\n\r",ch);
	check_improve(ch,gsn_bear_call,FALSE,1);
    ch->mana -= 35;
	return;
    }

    ch->mana -= 70;

    a_level = ch->level;

act("$n calls out to the wild and is heard!.",ch,0,0,TO_ROOM);
send_to_char("Your call to the wild is heard!\n\r",ch);

for (count = 0; count < 2; count++)
{
    animal = create_mobile(get_mob_index(MOB_VNUM_BEAR) );
    animal->level = a_level;
    animal->max_hit = ch->max_hit + dice(a_level, 5);
    animal->damroll += a_level*3/4;
    animal->hitroll += a_level/3;
    animal->alignment = ch->alignment;
    char_to_room(animal,ch->in_room);
    add_follower(animal,ch);
    animal->leader = ch;
    SET_BIT(animal->affected_by,AFF_CHARM);
    act("$n arrives to journey with $N.",animal,NULL,ch,TO_NOTVICT);
    animal->hit = animal->max_hit;
}
        init_affect(&af);
    af.where = TO_AFFECTS;
        af.aftype    = AFT_SKILL;
    af.level = ch->level;
    af.modifier = 0;
    af.duration = 24;
    af.bitvector = 0;
    af.location = 0;
    af.type = gsn_bear_call;
    affect_to_char(ch,&af);
    check_improve(ch,gsn_bear_call,TRUE,1);
    return;
}

/*void do_animal_call(CHAR_DATA *ch,char *argument)  --- Old Animal Call
{
    char arg[MAX_INPUT_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    CHAR_DATA *animal1;
    CHAR_DATA *animal2;
    CHAR_DATA *mob;
    ROOM_INDEX_DATA *pRoomIndex;
    int chance;
    int type;
    int i;
    bool cant_call = FALSE;
    AFFECT_DATA af;

    one_argument(argument,arg);

    if ((chance = get_skill(ch,gsn_animal_call)) == 0
    || ch->level < skill_table[gsn_animal_call].skill_level[ch->class])
    {
        send_to_char("You don't know how to call upon animals for aid.\n\r",ch);
        return;
    }

    if (arg[0] == '\0')
    {
        send_to_char("Call out for which animal?\n\r",ch);
        return;
    }

    if (!str_cmp(arg,"falcon"))     type = MOB_VNUM_FALCON;
    else if (!str_cmp(arg,"wolf"))
	{
	  type = MOB_VNUM_WOLF;
	if (ch->level < 25)
		cant_call = TRUE;
	}
    else if (!str_cmp(arg,"bear"))
	{
	type = MOB_VNUM_BEAR;
	if (ch->level < 30)
		cant_call = TRUE;
	}
    else if (!str_cmp(arg,"lion"))
	{
	type = MOB_VNUM_LION;
	if (ch->level < 35)
		cant_call = TRUE;
	}
    else
    {
        send_to_char("call out for which animal?\n\r",ch);
        return;
    }

    if (is_affected(ch,gsn_animal_call))
    {
        send_to_char("You aren't able to call upon anymore animals yet.\n\r",ch);
        return;
    }

    if (cant_call)
    {
	send_to_char("You are not skilled enough at woodsmanship to call that animal.\n\r",ch);
	return;
     }

    if (ch->mana < 50)
    {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
    }

    for (mob = char_list; mob != NULL; mob = mob->next)
    {
        if (IS_NPC(mob) && IS_AFFECTED(mob,AFF_CHARM)
        && (mob->master == ch)
        && ( (mob->pIndexData->vnum == MOB_VNUM_FALCON)
        || (mob->pIndexData->vnum == MOB_VNUM_WOLF)
        || (mob->pIndexData->vnum == MOB_VNUM_BEAR)
        || (mob->pIndexData->vnum == MOB_VNUM_LION) ) )
        break;
    }

    if (mob != NULL)
    {
        send_to_char("You already have animals you should care for.\n\r",ch);
        return;
    }

    if ( (ch->in_room->sector_type != SECT_FOREST)
    && ( ch->in_room->sector_type != SECT_HILLS)
    && ( ch->in_room->sector_type != SECT_MOUNTAIN) )
    {
           send_to_char("You are not within the right environment to call animals.\n\r",ch);
           return;
    }

    if (number_percent() > chance)
    {
    act("$n calls out to the wild but nothing responds.",ch,0,0,TO_ROOM);
    send_to_char("You call out to the wild but nothing responds.\n\r",ch);
    ch->mana -= 25;
    check_improve(ch,gsn_animal_call,FALSE,1);
    return;
    }

    pRoomIndex = ch->in_room;
    pMobIndex = get_mob_index(type);

    if (pMobIndex == NULL)
    {
        bug("Animal call: Bad mob vnum call %d.",type);
        send_to_char("You call out to the wild but nothing responds.\n\r",ch);
        act("$n calls out to the wild but nothing responds.",ch,0,0,TO_ROOM);
        ch->mana -= 25;
        return;
    }

    animal1 = create_mobile( pMobIndex );
    animal2 = create_mobile( pMobIndex );
    animal2->dam_type = animal1->dam_type;

    for (i = 0; i < 4; i++)
    {
        animal1->armor[i] -= (3*ch->level);
        animal2->armor[i] -= (3*ch->level);
    }

    if (type == MOB_VNUM_FALCON)
    {
        animal1->max_hit = ch->max_hit/3;
        animal1->hit = ch->max_hit/3;
        animal2->max_hit = ch->max_hit/3;
        animal2->hit = ch->max_hit/3;
        animal1->damroll = (5 + ch->level/3);
        animal2->damroll = (5 + ch->level/3);
        animal1->hitroll = ch->level/3;
        animal2->hitroll = ch->level/3;
    }

    else if (type == MOB_VNUM_WOLF)
    {
        animal1->max_hit = ch->max_hit/2;
        animal1->hit = ch->max_hit/2;
        animal2->max_hit = ch->max_hit/2;
        animal2->hit = ch->max_hit/2;
        animal1->damroll = (8 + ch->level/3);
        animal2->damroll = (8 + ch->level/3);
        animal1->hitroll = ch->level/3;
        animal2->hitroll = ch->level/3;
    }


    else if (type == MOB_VNUM_BEAR)
    {
        animal1->max_hit = ch->max_hit;
        animal1->hit = ch->max_hit;
        animal2->max_hit = ch->max_hit;
        animal2->hit = ch->max_hit;
        animal1->damroll = (2*ch->level/3);
        animal2->damroll = (2*ch->level/3);
        animal1->hitroll = ch->level/3;
        animal2->hitroll = ch->level/3;
    }


    else if (type == MOB_VNUM_LION)
    {
        animal1->max_hit = (3 * ch->max_hit / 4);
        animal1->hit = (3 * ch->max_hit / 4);
        animal2->max_hit = (3 * ch->max_hit / 4);
        animal2->hit = (3 * ch->max_hit / 4);
        animal1->damroll = (-2 + ch->level);
        animal2->damroll = (-2 + ch->level);
        animal1->hitroll = ch->level/3;
        animal2->hitroll = ch->level/3;
    }
    else
    {
    bug("Bad animal call: %d.",type);
    send_to_char("You call out to the wild but nothing responds.\n\r",ch);
    act("$n calls out to the wild but nothing comes.\n\r",ch,0,0,TO_ROOM);
    ch->mana -= 25;
    return;
    }

    char_to_room(animal1,pRoomIndex);
    char_to_room(animal2,pRoomIndex);
    animal1->level = ch->level;
    animal2->level = ch->level;

    act("Animals respond to $n's call!",ch,0,animal1,TO_ROOM);
    act("Animals respond to your call!",ch,0,animal1,TO_CHAR);
    ch->mana -= 50;
    SET_BIT(animal1->affected_by,AFF_CHARM);
    SET_BIT(animal2->affected_by,AFF_CHARM);
    animal1->leader = ch;
    animal2->leader = ch;
    animal1->master = ch;
    animal2->master = ch;

        init_affect(&af);
    af.where = TO_AFFECTS;
    af.type = gsn_animal_call;
        af.aftype    = AFT_SKILL;
    af.level = ch->level;
    af.duration = 24;
    af.location = 0;
    af.modifier = 0;
    af.bitvector = 0;
    affect_to_char(ch,&af);
    return;
} */


bool check_rangermob(CHAR_DATA *mob )
{
	if (mob->pIndexData->vnum == MOB_VNUM_RRATTLESNAKE)
	return TRUE;
	if (mob->pIndexData->vnum == MOB_VNUM_RSPRITE)
	return TRUE;
	if (mob->pIndexData->vnum == MOB_VNUM_RLYNX)
	return TRUE;
	if (mob->pIndexData->vnum == MOB_VNUM_RLEECH)
	return TRUE;
	if (mob->pIndexData->vnum == MOB_VNUM_RVINE)
	return TRUE;
	if (mob->pIndexData->vnum == MOB_VNUM_RALLIGATOR)
	return TRUE;
	if (mob->pIndexData->vnum == MOB_VNUM_RBAT)
	return TRUE;
	if (mob->pIndexData->vnum == MOB_VNUM_RGRIFFIN)
	return TRUE;
	if (mob->pIndexData->vnum == MOB_VNUM_RBEAR)
	return TRUE;
	if (mob->pIndexData->vnum == MOB_VNUM_RWOLF)
	return TRUE;
	if (mob->pIndexData->vnum == MOB_VNUM_RWOLVERINE)
	return TRUE;
	if (mob->pIndexData->vnum == MOB_VNUM_RWITCH)
	return TRUE;
	if (mob->pIndexData->vnum == MOB_VNUM_RJAGUAR)
	return TRUE;
	if (mob->pIndexData->vnum == MOB_VNUM_RWSNAKE)
	return TRUE;
	if (mob->pIndexData->vnum == MOB_VNUM_RFISH)
	return TRUE;
	if (mob->pIndexData->vnum == MOB_VNUM_RNAGA)
	return TRUE;
	if (mob->pIndexData->vnum == MOB_VNUM_RLION)
	return TRUE;
	if (mob->pIndexData->vnum == MOB_VNUM_RFALCON)
	return TRUE;
	else
	return FALSE;
}
// New Animal Call with karate chop action
void do_animal_call(CHAR_DATA *ch,char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    CHAR_DATA *animal1;
    CHAR_DATA *mob;
    ROOM_INDEX_DATA *pRoomIndex;
    int chance;
    int type;
    int i;
    int mob_count;
    bool wrong_sector = FALSE;
    bool rager_check = FALSE;
    AFFECT_DATA af;

    one_argument(argument,arg);

    if ((chance = get_skill(ch,gsn_animal_call)) == 0
    || ch->level < skill_table[gsn_animal_call].skill_level[ch->class])
    {
        send_to_char("You don't know how to call upon animals for aid.\n\r",ch);
        return;
    }

    if (arg[0] == '\0')
    {
        send_to_char("The following creatures are able to be called upon:\n\r",ch);
		send_to_char("In the hills: Lion, Rattlesnake, Sprite and Lynx.\n\r",ch);
		send_to_char("In the air: Falcon, Bat and Griffin.\n\r",ch);
		send_to_char("In the forest: Bear, Wolf, Wolverine, Witch and Jaguar.\n\r",ch);
		send_to_char("In the water: Water Snake(Or snake), Fish and Naga.\n\r",ch);
        return;
    }

	if (!str_cmp(arg,"lion"))
	{
		type = MOB_VNUM_RLION;
		if ( ch->in_room->sector_type != SECT_HILLS)
		wrong_sector = TRUE;
	}
	else if (!str_cmp(arg,"rattlesnake"))
	{
		type = MOB_VNUM_RRATTLESNAKE;
		if ( ch->in_room->sector_type != SECT_HILLS)
		wrong_sector = TRUE;
	}
	else if (!str_cmp(arg,"sprite"))
	{
		type = MOB_VNUM_RSPRITE;
		if ( ch->in_room->sector_type != SECT_HILLS)
		wrong_sector = TRUE;
		if (ch->cabal == CABAL_RAVAGER)
		rager_check = TRUE;
	}
	else if (!str_cmp(arg,"lynx"))
	{
		type = MOB_VNUM_RLYNX;
		if ( ch->in_room->sector_type != SECT_HILLS)
		wrong_sector = TRUE;
	}
	else if (!str_cmp(arg,"bat"))
	{
		type = MOB_VNUM_RBAT;
		if (ch->in_room->sector_type != SECT_AIR)
		wrong_sector = TRUE;
	}
	else if (!str_cmp(arg,"falcon"))
	{
		type = MOB_VNUM_RFALCON;
		if (ch->in_room->sector_type != SECT_AIR)
		wrong_sector = TRUE;
	}
	else if (!str_cmp(arg,"griffin"))
	{
		type = MOB_VNUM_RGRIFFIN;
		if (ch->in_room->sector_type != SECT_AIR)
		wrong_sector = TRUE;
	}
	else if (!str_cmp(arg,"bear"))
	{
		type = MOB_VNUM_RBEAR;
		if (ch->in_room->sector_type != SECT_FOREST)
		wrong_sector = TRUE;
	}
	else if (!str_cmp(arg,"wolf"))
	{
		type = MOB_VNUM_RWOLF;
		if (ch->in_room->sector_type != SECT_FOREST)
		wrong_sector = TRUE;
	}
	else if (!str_cmp(arg,"wolverine"))
	{
		type = MOB_VNUM_RWOLVERINE;
		if (ch->in_room->sector_type != SECT_FOREST)
		wrong_sector = TRUE;
	}
	else if (!str_cmp(arg,"witch"))
	{
		type = MOB_VNUM_RWITCH;
		if (ch->in_room->sector_type != SECT_FOREST)
		wrong_sector = TRUE;
		if (ch->cabal == CABAL_RAVAGER)
		rager_check = TRUE;
	}
	else if (!str_cmp(arg,"jaguar"))
	{
		type = MOB_VNUM_RJAGUAR;
		if (ch->in_room->sector_type != SECT_FOREST)
		wrong_sector = TRUE;
	}
	else if (!str_cmp(arg,"snake"))
	{
		type = MOB_VNUM_RWSNAKE;
		if (ch->in_room->sector_type != SECT_WATER_NOSWIM)
		wrong_sector = TRUE;
	}
	else if (!str_cmp(arg,"fish"))
	{
		type = MOB_VNUM_RFISH;
		if (ch->in_room->sector_type != SECT_WATER_NOSWIM)
		wrong_sector = TRUE;
	}
	else if (!str_cmp(arg,"naga"))
	{
		type = MOB_VNUM_RNAGA;
		if (ch->in_room->sector_type != SECT_WATER_NOSWIM)
		wrong_sector = TRUE;
		if (ch->cabal == CABAL_RAVAGER)
		rager_check = TRUE;
	}
	else
	{
	    send_to_char("The following creatures are able to be called upon:\n\r",ch);
		send_to_char("In the hills: Lion, Rattlesnake, Sprite and Lynx.\n\r",ch);
		send_to_char("In the air: Falcon, Bat and Griffin.\n\r",ch);
		send_to_char("In the forest: Bear, Wolf, Wolverine, Witch and Jaguar.\n\r",ch);
		send_to_char("In the water: Water Snake(Or snake), Fish and Naga.\n\r",ch);
        return;
    }

    if (is_affected(ch,gsn_animal_call))
    {
        send_to_char("You aren't able to call upon anymore animals yet.\n\r",ch);
        return;
    }

    if (ch->mana < 50)
    {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
    }
	mob_count = 0;
    for (mob = char_list; mob != NULL; mob = mob->next)
    {
        if (IS_NPC(mob) && IS_AFFECTED(mob,AFF_CHARM)
        && (mob->master == ch))
        {
        if (mob->pIndexData->vnum == MOB_VNUM_RRATTLESNAKE )
        {
        	mob_count += 1;
		}
		else if (mob->pIndexData->vnum == MOB_VNUM_RSPRITE )
        {
        	mob_count += 3;
		}
		else if (mob->pIndexData->vnum == MOB_VNUM_RLYNX )
		{
			mob_count += 1;
		}
		else if (mob->pIndexData->vnum == MOB_VNUM_RBAT)
		{
			mob_count += 1;
		}
		else if (mob->pIndexData->vnum == MOB_VNUM_RGRIFFIN)
        {
			mob_count += 3;
		}
        else if (mob->pIndexData->vnum == MOB_VNUM_RBEAR)
        {
			mob_count += 1;
		}
		else if (mob->pIndexData->vnum == MOB_VNUM_RWOLF)
		{
			mob_count += 1;
		}
		else if (mob->pIndexData->vnum == MOB_VNUM_RWOLVERINE)
		{
			mob_count += 3;
		}
		else if (mob->pIndexData->vnum == MOB_VNUM_RWITCH)
		{
			mob_count += 3;
		}
		else if (mob->pIndexData->vnum == MOB_VNUM_RJAGUAR)
		{
			mob_count += 1;
		}
		else if (mob->pIndexData->vnum == MOB_VNUM_RWSNAKE)
		{
			mob_count += 1;
		}
		else if (mob->pIndexData->vnum == MOB_VNUM_RFISH)
		{
			mob_count += 1;
		}
		else if (mob->pIndexData->vnum == MOB_VNUM_RNAGA)
		{
			mob_count += 3;
		}
		else if (mob->pIndexData->vnum == MOB_VNUM_RLION)
		{
			mob_count += 3;
		}
		else if (mob->pIndexData->vnum == MOB_VNUM_RFALCON)
		{
			mob_count += 1;
		}
	}
    }
	if (mob_count > 5)
    {
        send_to_char("You couldn't handle another of those.\n\r",ch);
        return;
    }

    if (rager_check)
    {
		send_to_char("Ragers can't call magical animals!\n\r",ch);
		return;
	}

    if (wrong_sector)
    {
      	send_to_char("You aren't in the right place to call that creature.\n\r",ch);
        return;
    }

    if (number_percent() > chance)
    {
    act("$n calls out but nothing responds.",ch,0,0,TO_ROOM);
    send_to_char("You call out but nothing responds.\n\r",ch);
    ch->mana -= 25;
    check_improve(ch,gsn_animal_call,FALSE,1);
    return;
    }

    pRoomIndex = ch->in_room;
    pMobIndex = get_mob_index(type);

    if (pMobIndex == NULL)
    {
        bug("Animal call: Bad mob vnum call %d.",type);
        send_to_char("You call out but nothing responds.\n\r",ch);
        act("$n calls out but nothing responds.",ch,0,0,TO_ROOM);
        ch->mana -= 25;
        return;
    }

    animal1 = create_mobile( pMobIndex );

    for (i = 0; i < 4; i++)
    {
        animal1->armor[i] -= (3*ch->level);
    }

    if (type == MOB_VNUM_RRATTLESNAKE)
    {
        animal1->max_hit = ch->max_hit/2;
        animal1->hit = ch->max_hit/2;
        animal1->damroll = (5 + ch->level/2);
        animal1->hitroll = ch->level/2;
     }
	else if (type == MOB_VNUM_RSPRITE)
    {
        animal1->max_hit = (1.5*ch->max_hit/2);
        animal1->hit = (1.5*ch->max_hit/2);
        animal1->damroll = (8 + ch->level/3);
        animal1->hitroll = ch->level/3;

    }
	else if (type == MOB_VNUM_RLYNX)
    {
        animal1->max_hit = ch->max_hit;
        animal1->hit = ch->max_hit;
        animal1->damroll = (2*ch->level/3);
        animal1->hitroll = ch->level/3;

    }
	else if (type == MOB_VNUM_RBAT)
	{
	    animal1->max_hit = ch->max_hit/2;
	    animal1->hit = ch->max_hit/2;
	    animal1->damroll = (5 + ch->level/2);
	    animal1->hitroll = ch->level/2;

    }
    else if (type == MOB_VNUM_RGRIFFIN)
	{
	    animal1->max_hit = (1.5*ch->max_hit/2);
	    animal1->hit = (1.5*ch->max_hit/2);
	    animal1->damroll = (12 + ch->level/3);
	    animal1->hitroll = ch->level/3;

    }
    else if (type == MOB_VNUM_RBEAR)
	{
	    animal1->max_hit = (2.5*ch->max_hit/2);
	    animal1->hit = (2.5*ch->max_hit/2);
	    animal1->damroll = (8 + ch->level/3);
	    animal1->hitroll = ch->level/3;

    }
    else if (type == MOB_VNUM_RWOLF)
	{
	    animal1->max_hit = (1.2*ch->max_hit/2);
	    animal1->hit = (1.2*ch->max_hit/2);
	    animal1->damroll = ch->level/3;
	    animal1->hitroll = (8 + ch->level/3);

	}
	else if (type == MOB_VNUM_RWOLVERINE)
	{
	    animal1->max_hit = ch->max_hit;
	    animal1->hit = ch->max_hit;
	    animal1->damroll = ch->level/3;
	    animal1->hitroll = (8 + ch->level/3);

	}
	else if (type == MOB_VNUM_RWITCH)
	{
	    animal1->max_hit = (ch->max_hit/2);
	    animal1->hit = (ch->max_hit/2);
	    animal1->damroll = ch->level/3;
	    animal1->hitroll = (8 + ch->level/3);

	}
	else if (type == MOB_VNUM_RJAGUAR)
	{
	    animal1->max_hit = (ch->max_hit/2);
	    animal1->hit = (ch->max_hit/2);
	    animal1->damroll = ch->level/3;
	    animal1->hitroll = (8 + ch->level/3);

	}
	else if (type == MOB_VNUM_RWSNAKE)
	{
	    animal1->max_hit = (ch->max_hit/3);
	    animal1->hit = (ch->max_hit/3);
	    animal1->damroll = ch->level/3;
	    animal1->hitroll = (12 + ch->level/3);

	}
	else if (type == MOB_VNUM_RFISH)
	{
	    animal1->max_hit = (ch->max_hit/4);
	    animal1->hit = (ch->max_hit/4);
	    animal1->damroll = ch->level/3;
	    animal1->hitroll = (8 + ch->level/3);

	}
	else if (type == MOB_VNUM_RNAGA)
	{
	    animal1->max_hit = (ch->max_hit);
	    animal1->hit = (ch->max_hit);
	    animal1->damroll = (8 + ch->level/3);
	    animal1->hitroll = (8 + ch->level/3);

	}
	else if (type == MOB_VNUM_RLION)
	{
	    animal1->max_hit = (ch->max_hit/1.5);
	    animal1->hit = (ch->max_hit/1.5);
	    animal1->damroll = (10 + ch->level/3);
	    animal1->hitroll = (14 + ch->level/3);

	}
	else
    {
    bug("Bad animal call: %d.",type);
    send_to_char("You call out but nothing responds.\n\r",ch);
    act("$n calls out but nothing comes.\n\r",ch,0,0,TO_ROOM);
    ch->mana -= 25;
    return;
    }

    char_to_room(animal1,pRoomIndex);
    animal1->level = ch->level;


    act("A creature stumbles out to join $n!",ch,0,animal1,TO_ROOM);
    act("A creature stumbles out to join you!",ch,0,animal1,TO_CHAR);
    ch->mana -= 50;
    SET_BIT(animal1->affected_by,AFF_CHARM);
    animal1->leader = ch;
    animal1->master = ch;


        init_affect(&af);
    af.where = TO_AFFECTS;
    af.type = gsn_animal_call;
        af.aftype    = AFT_SKILL;
    af.level = ch->level;
    af.duration = 10;
    af.location = 0;
    af.modifier = 0;
    af.bitvector = 0;
    affect_to_char(ch,&af);
    return;
}



void track_char(CHAR_DATA *ch, CHAR_DATA *tracking, int in_room)
{
    int track_dir;

    track_dir = tracking->pcdata->track_dir[in_room];
    track_dir -= 1;

    if (IS_AFFECTED(ch,AFF_CHARM))
	return;

    act("$n checks the ground for tracks.",ch,0,0,TO_ROOM);
    if (IS_SET(ch->off_flags,STATIC_TRACKING) || track_dir == -1)
    	return;

    if (ch->in_room->exit[track_dir]->u1.to_room->area == ch->in_room->area)
	move_char(ch,track_dir,TRUE);
	if (ch->pIndexData->vnum == MOB_VNUM_PHANTOM)
	{
	   do_goto(ch,ch->last_fought->name);
	}
    return;


}

/*
Removed because its bugged. -- Ceial
void track_char(CHAR_DATA *ch, CHAR_DATA *tracking, int in_room)
{
    int track_dir;

    track_dir = tracking->pcdata->track_dir[in_room];
    track_dir -= 1;

    if (IS_AFFECTED(ch,AFF_CHARM))
	return;

    act("$n checks the ground for tracks.",ch,0,0,TO_ROOM);
    if (IS_SET(ch->off_flags,STATIC_TRACKING) || track_dir == -1)
    	return;

    if (ch==NULL || ch->in_room==NULL ||
        ch->in_room->exit[track_dir]==NULL)
    { bug("In track_char: NULL structure",0); return;}
    if (ch->in_room->exit[track_dir]->u1.to_room->area == ch->in_room->area)
    {
	move_char(ch,track_dir,TRUE);
    }
    if (ch->pIndexData->vnum == MOB_VNUM_PHANTOM)
    {
        do_goto(ch,ch->last_fought->name);
    }
    return;
}*/


void do_aura_of_sustenance(CHAR_DATA *ch,char *argument)
{
    AFFECT_DATA af;

    if ((get_skill(ch,gsn_aura_of_sustenance)) == 0
	|| ch->level < skill_table[gsn_aura_of_sustenance].skill_level[ch->class])
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (is_affected(ch,gsn_aura_of_sustenance))
    {
	send_to_char("You are already as sustained as you can get.\n\r",ch);
	return;
    }
    if (ch->mana < 40)
    {
    send_to_char("You do not have the mana\n\r",ch);
	return;
    }

        init_affect(&af);
    af.where = TO_AFFECTS;
    af.location = 0;
    af.level = ch->level;
    af.duration = ch->level;
    af.bitvector = 0;
    af.type = gsn_aura_of_sustenance;
        af.aftype    = AFT_SKILL;
    af.modifier = 0;
    affect_to_char(ch,&af);
    ch->pcdata->condition[COND_THIRST] = 38;
    ch->pcdata->condition[COND_HUNGER] = 38;

    send_to_char("Your mind goes into a trance as you sustain your body in a state of health.\n\r",ch);
    ch->mana -= 40;
	return;
}


void check_guardian(CHAR_DATA *ch,int in_room)
{
      CHAR_DATA *rch;
      CHAR_DATA *rch_next;

    if (ch->ghost <= 0)
	return;

    for (rch = ch->in_room->people; rch != NULL; rch = rch_next)
    {
	rch_next = rch->next_in_room;
	if (!IS_NPC(rch))
	   continue;
	if ((rch->pIndexData->vnum == MOB_VNUM_ACADIAN
    && ch->cabal != CABAL_ACADIAN)
	|| ( rch->pIndexData->vnum == MOB_VNUM_COVEN
    && ch->cabal != CABAL_COVEN)
      || ( rch->pIndexData->vnum == MOB_VNUM_BRIAR
    && ch->cabal != CABAL_BRIAR)
	|| (rch->pIndexData->vnum == MOB_VNUM_RAVAGER
    && ch->cabal != CABAL_RAVAGER)
	|| ( rch->pIndexData->vnum == MOB_VNUM_VIGILANTE
    && ch->cabal != CABAL_VIGILANTE)
	|| (rch->pIndexData->vnum == MOB_VNUM_PROTECTOR
    && ch->cabal != CABAL_PROTECTOR) )
	{
	do_say(rch,"Ghosts aren't allowed in here!");
	ch->ghost = 0;
	do_murder(rch,ch->name);
	return;
	}
    }
    return;
}

void murder_entry_trigger(CHAR_DATA *ch)
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *rch;
    CHAR_DATA *rch_next;

    if (IS_NPC(ch))
	return;

    for (rch = ch->in_room->people; rch != NULL; rch = rch_next)
    {
	rch_next = rch->next_in_room;
	if (!IS_NPC(rch))	continue;
	if (!IS_SET(rch->off_flags,SPAM_MURDER)) 	continue;
	if (rch->fighting != NULL)	continue;
	if (!can_see(rch,ch))	continue;
	if (rch->last_fought != NULL)	continue;
	if (is_safe(rch,ch))	continue;
	if (rch->level < (ch->level - 5))	continue;

	sprintf(buf,"Help! I'm being attacked by %s!",rch->short_descr);
	do_yell(ch,buf);
	multi_hit(rch,ch,TYPE_UNDEFINED);
    }
    return;
}



void vigilante_entry_trigger(CHAR_DATA *ch)
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *protector;

    if (IS_NPC(ch)) return;

    if (IS_IMMORTAL(ch)) return;

    for (protector = ch->in_room->people; protector != NULL; protector = protector->next_in_room)
    {
        if (!IS_NPC(protector)) continue;
        if (protector->pIndexData->vnum == MOB_VNUM_PROTECTOR)
        break;
    }
    if (protector == NULL)  return;
    un_gaseous(ch);

    if (ch->cabal == CABAL_VIGILANTE)
    {
	do_say(protector, "Greetings, Keeper of Justice.");
        return;
    }

    sprintf(buf,"Help! I'm being attacked by %s!",protector->short_descr);
    do_yell(ch,buf);
    sprintf(buf,"Intruder! Intruder! %s is raiding the cabal!",ch->name);
    do_cb(protector,buf);
    multi_hit(protector,ch,TYPE_UNDEFINED);
    return;
}

void vigilante_key_trigger(CHAR_DATA *ch,CHAR_DATA *statue,OBJ_DATA *key)
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *book;
    CHAR_DATA *hch;
    bool is_gone;
    OBJ_DATA *check;

    is_gone = FALSE;
    if (key->pIndexData->vnum != 23601)
	return;

    if (!IS_NPC(statue))     return;
    if (statue->pIndexData->vnum != MOB_VNUM_ENFORCER_STATUE)   return;

    for (check = object_list; check != NULL; check = check->next)
    {
	if (check->pIndexData->vnum == OBJ_VNUM_VIGILANTE)
		is_gone = TRUE;
    }

    book = create_object(get_obj_index(OBJ_VNUM_VIGILANTE),0);

    if (is_gone)
    {
        sprintf(buf,"%s has already been stolen, search elsewhere thief!",book->short_descr);
        do_say(statue,buf);
        extract_obj(book);
        return;
    }

    do_say(statue,"To the holder of the Key of Law I grant the Book of the Vigilantes.");
    act("$n gives $N $p.",statue,book,ch,TO_NOTVICT);
    act("$n gives you $p.",statue,book,ch,TO_VICT);
    obj_to_char(book,ch);
    extract_obj(key);
    sprintf(buf,"[VIGILANTE] %s: %s has stolen the Book of the Vigilantes!\n\r",statue->short_descr,ch->name);
    for (hch = char_list; hch != NULL; hch = hch->next)
    {
    if (hch->cabal == CABAL_VIGILANTE)
        send_to_char(buf,hch);
    }

    return;
}

/*void outlaw_key_trigger(CHAR_DATA *ch,CHAR_DATA *statue,OBJ_DATA *key)
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *book;
    CHAR_DATA *hch;

    if (key->pIndexData->vnum != 9900)
	return;

    if (!IS_NPC(statue))     return;
    if (statue->pIndexData->vnum != 9901)   return;

    book = create_object(get_obj_index(9902),0);
    if (book->pIndexData->limcount != 1)
    {
        sprintf(buf,"%s has already been stolen, search elsewhere thief!",book->short_descr);
        do_say(statue,buf);
        extract_obj(book);
        return;
    }
    act("$n gives $N $p.",statue,book,ch,TO_NOTVICT);
    act("$n gives you $p.",statue,book,ch,TO_VICT);
    obj_to_char(book,ch);
    extract_obj(key);

    sprintf(buf,"[OUTLAW] %s: %s has stolen the Declaration!\n\r",statue->short_descr,ch->name);

    for (hch = char_list; hch != NULL; hch = hch->next)
    {
    if (hch->cabal == CABAL_OUTLAW)
        send_to_char(buf,hch);
    }
    return;
}*/


/* MOBProgs for Cabals */

void ravager_entry_trigger(CHAR_DATA *ch)
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    CHAR_DATA *protector;

    if (IS_NPC(ch)) return;
    if (IS_IMMORTAL(ch)) return;
    for (protector = ch->in_room->people; protector != NULL; protector = protector->next_in_room)
    {
        if (!IS_NPC(protector)) continue;
        if (protector->pIndexData->vnum == MOB_VNUM_RAVAGER)
        break;
    }
    if (protector == NULL)  return;

    un_gaseous(ch);
    if (ch->cabal == CABAL_RAVAGER)
    {
        do_say(protector,"Hail, Great Warrior!");
        return;
    }
    sprintf(buf,"Help! I'm being attacked by %s!",protector->short_descr);
    do_yell(ch,buf);
    sprintf(buf2,"Intruder! Intruder! %s is raiding the cabal!",ch->name);
    do_cb(protector,buf2);
    multi_hit(protector,ch,TYPE_UNDEFINED);
    return;
}


/*void outlaw_entry_trigger(CHAR_DATA *ch)
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *protector;

    if (IS_NPC(ch)) return;
    if (IS_IMMORTAL(ch)) return;
    for (protector = ch->in_room->people; protector != NULL; protector = protector->next_in_room)
    {
        if (!IS_NPC(protector)) continue;
        if (protector->pIndexData->vnum == MOB_VNUM_OUTLAW)
        break;
    }
    if (protector == NULL)  return;

    un_gaseous(ch);
    if (ch->cabal == CABAL_OUTLAW)
    {
        do_say(protector, "Enter the hideout outlaw friend.");
        return;
    }
    sprintf(buf,"Help! I'm being attacked by %s!",protector->short_descr);
    do_yell(ch,buf);
    sprintf(buf,"Intruder! Intruder! %s is raiding the cabal!",ch->name);
    do_cb(protector,buf);
    multi_hit(protector,ch,TYPE_UNDEFINED);
    return;
}*/


void briar_entry_trigger(CHAR_DATA *ch)
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *protector;

    if (IS_NPC(ch)) return;
    if (IS_IMMORTAL(ch)) return;
    for (protector = ch->in_room->people; protector != NULL; protector = protector->next_in_room)
    {
        if (!IS_NPC(protector)) continue;
        if (protector->pIndexData->vnum == MOB_VNUM_BRIAR)
        break;
    }
    if (protector == NULL)  return;

    un_gaseous(ch);
    if (ch->cabal == CABAL_BRIAR)
    {
        do_say(protector, "Greetings thing of the wild.");
        return;
    }
    sprintf(buf,"Help! I'm being attacked by %s!",protector->short_descr);
    do_yell(ch,buf);
    sprintf(buf,"Intruder! Intruder! %s is raiding the cabal!",ch->name);
    do_cb(protector,buf);
    multi_hit(protector,ch,TYPE_UNDEFINED);
    return;
}


void protector_entry_trigger(CHAR_DATA *ch)
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *protector;

    if (IS_NPC(ch)) return;
    if (IS_IMMORTAL(ch)) return;
    for (protector = ch->in_room->people; protector != NULL; protector = protector->next_in_room)
    {
        if (!IS_NPC(protector)) continue;
        if (protector->pIndexData->vnum == MOB_VNUM_PROTECTOR)
        break;
    }
    if (protector == NULL)  return;
    un_gaseous(ch);

    if (ch->cabal == CABAL_PROTECTOR)
    {
	do_say(protector,"Greetings, brother Knight.");
        return;
    }
    sprintf(buf,"Help! I'm being attacked by %s!",protector->short_descr);
    do_yell(ch,buf);
    sprintf(buf,"Intruder! Intruder! %s is raiding the cabal!",ch->name);
    do_cb(protector,buf);
    multi_hit(protector,ch,TYPE_UNDEFINED);
    return;
}



void acadian_entry_trigger(CHAR_DATA *ch)
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *protector;

    if (IS_NPC(ch)) return;
    if (IS_IMMORTAL(ch)) return;
    for (protector = ch->in_room->people; protector != NULL; protector = protector->next_in_room)
    {
        if (!IS_NPC(protector)) continue;
        if (protector->pIndexData->vnum == MOB_VNUM_ACADIAN)
        break;
    }
    if (protector == NULL)  return;
    un_gaseous(ch);

    if (ch->cabal == CABAL_ACADIAN)
    {
        do_say(protector,"Greetings, Whelp.");
        return;
    }
    sprintf(buf,"Help! I'm being attacked by %s!",protector->short_descr);
    do_yell(ch,buf);
    sprintf(buf,"%s has come to die! Quickly whelps!",ch->name);
    do_cb(protector,buf);
    multi_hit(protector,ch,TYPE_UNDEFINED);
    return;
}

void coven_entry_trigger(CHAR_DATA *ch)
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *protector;

    if (IS_NPC(ch)) return;
    if (IS_IMMORTAL(ch)) return;
    for (protector = ch->in_room->people; protector != NULL; protector = protector->next_in_room)
    {
        if (!IS_NPC(protector)) continue;
        if (protector->pIndexData->vnum == MOB_VNUM_COVEN)
        break;
    }
    if (protector == NULL)  return;
    un_gaseous(ch);

    if (ch->cabal == CABAL_COVEN)
    {
        do_say(protector,"Hail, Master of Magic.");
        return;
    }
    sprintf(buf,"Help! I'm being attacked by %s!",protector->short_descr);
    do_yell(ch,buf);
    sprintf(buf,"Intruder! Intruder! %s is raiding the cabal!",ch->name);
    do_cb(protector,buf);
    multi_hit(protector,ch,TYPE_UNDEFINED);
    return;
}

/*void empire_entry_trigger(CHAR_DATA *ch)
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *protector;

    if (IS_NPC(ch)) return;
    if (IS_IMMORTAL(ch)) return;
    for (protector = ch->in_room->people; protector != NULL; protector = protector->next_in_room)
    {
        if (!IS_NPC(protector)) continue;
        if (protector->pIndexData->vnum == MOB_VNUM_EMPIRE)
        break;
    }
    if (protector == NULL)  return;
    un_gaseous(ch);

    if (ch->cabal == CABAL_EMPIRE)
    {
        do_say(protector,"Greetings, citizens.");
        return;
    }
    sprintf(buf,"Help! I'm being attacked by %s!",protector->short_descr);
    do_yell(ch,buf);
    sprintf(buf,"Intruder! Intruder! %s is raiding the cabal!",ch->name);
    do_cb(protector,buf);
    multi_hit(protector,ch,TYPE_UNDEFINED);
    return;
} */


/*
Add this to the can_see_char:


if (IS_SET(victim->also_affected,AFF_FOREST_BLENDING) )
{
        int blend_chance;

        blend_chance = (victim->level - ch->level) * 2;
        blend_chance += 40;

        if (blend_chance > 75)
                blend_chance = 75;

        if (!is_affected(ch,gsn_awareness)
        && number_percent() < blend_chance)
                return FALSE;
}
*/

void do_forest_blending(CHAR_DATA *ch, char *argument)
{
        int chance;
        AFFECT_DATA af;

        chance = get_skill(ch,gsn_forest_blending);
        if (chance < 5
        || ch->level < skill_table[gsn_forest_blending].skill_level[ch->class])
        {
        send_to_char("You don't know how to blend in with the forests.\n\r",ch);
        return;
        }
        if (ch->in_room->sector_type != SECT_FOREST)
        {
        send_to_char("You aren't within a suitable forest environment.\n\r",ch);
        return;
        }
   	if (is_affected(ch,gsn_forest_blending))
	{
	send_to_char("You're already trying to blend in with your surroundings.\n\r",ch);
	return;
	}

send_to_char("You attempt to blend in with the forests.\n\r",ch);
        if (number_percent() < chance)
        {
        check_improve(ch,gsn_forest_blending,TRUE,1);
        init_affect(&af);
af.where = TO_AFFECTS;
        af.aftype    = AFT_SKILL;
af.level = ch->level;
af.modifier = 0;
af.location = 0;
af.bitvector = 0;
af.duration = 5;
af.type = gsn_forest_blending;
affect_to_char(ch,&af);
        }
        else
        {
                check_improve(ch,gsn_forest_blending,FALSE,1);
        affect_strip(ch,gsn_forest_blending);
        }

        WAIT_STATE(ch,6);
        return;
}

void un_forest_blend(CHAR_DATA *ch)
{
        if (!is_affected(ch,gsn_forest_blending))
                return;
        act("$n steps into the open from the surrounding forest.",ch,0,0,TO_ROOM);
        act("You step into the open from the surrounding forest.",ch,0,0,TO_CHAR);
        affect_strip(ch,gsn_forest_blending);
        return;
}

void do_awareness(CHAR_DATA *ch,char *argument)
{
        int chance;
        AFFECT_DATA af;

        if (is_affected(ch,gsn_awareness))
        {
        send_to_char("You are already alert to the forests.\n\r",ch);
        return;
        }
        chance = get_skill(ch,gsn_awareness);
        if (chance == 0
        || ch->level < skill_table[gsn_awareness].skill_level[ch->class])
        {
        send_to_char("You don't know which part of the forests to be alert to.\n\r",ch);
        return;
        }
        if (ch->mana < 10)
        {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
        }

        if (number_percent() > chance)
        {
        send_to_char("You try to watch your surroundings but become distracted.\n\r",ch);
        check_improve(ch,gsn_awareness,FALSE,1);
        ch->mana -= 5;
        return;
        }
        send_to_char("You start watching your surroundings more carefully.\n\r",ch);
        check_improve(ch,gsn_awareness,TRUE,1);
        ch->mana -= 10;
        init_affect(&af);
        af.where = TO_AFFECTS;
        af.location = 0;
        af.modifier = 0;
        af.type = gsn_awareness;
        af.aftype    = AFT_SKILL;
        af.bitvector = 0;
        af.level = ch->level;
        af.duration = ch->level;
        affect_to_char(ch,&af);
        return;
}

/*
 * This ethereal ability lets a player pass through a room in gaseous form.
 * In this state they can pass aggro and tracking mobs if they make a check
 * while passing through the room. Cabal guardians are never passed, nor are
 * aggros who's level is more than 10 above the player's.
 */

void un_gaseous(CHAR_DATA *ch)
{
        if (!is_affected(ch,gsn_gaseous_form))
		return;
        act("$n forms again as $s body solidifies from gaseous form.",ch,0,0,TO_ROOM);
        send_to_char("You reform into your normal body as you leave gaseous form.\n\r",ch);
        affect_strip(ch,gsn_gaseous_form);
	return;
}

/* Door bash for giants/trolls/centaurs */
void do_door_bash(CHAR_DATA *ch,char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    int door;
    int chance;
    AFFECT_DATA af;

    one_argument(argument,arg);
    if (arg[0] == '\0')
    {
        send_to_char("Bash door which door?\n\r",ch);
        return;
    }
    chance = get_skill(ch,gsn_door_bash);
    if (chance == 0
    || ch->level < skill_table[gsn_door_bash].skill_level[ch->class])
    {
        send_to_char("You'd hurt yourself doing that.\n\r",ch);
        return;
    }
    if (ch->move < 5)
    {
        send_to_char("You are too exhausted.\n\r",ch);
	return;
    }

    if ( (door = find_door(ch,arg) ) >= 0)
    {
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;
    EXIT_DATA *pexit_rev;
    pexit = ch->in_room->exit[door];
    if (!IS_SET(pexit->exit_info,EX_CLOSED))
    {
        send_to_char("It's not closed.\n\r",ch);
        return;
    }

    if (!IS_SET(pexit->exit_info,EX_LOCKED))
    {
        send_to_char("It's already unlocked, why not just use the knob?\n\r",ch);
        return;
    }

    chance /= 4;
    chance += ch->level/5;
    chance += get_curr_stat(ch,STAT_STR)/2;

    if (!str_cmp(pc_race_table[ch->race].name,"cloud") ||
     !str_cmp(pc_race_table[ch->race].name,"storm") ||
     !str_cmp(pc_race_table[ch->race].name,"fire"))
	chance += 15;

    do_visible(ch,"");
    if (number_percent() > chance || IS_SET(pexit->exit_info,EX_NOBASH) )
    {
    act("$n flies into the $T door and rebounds with a great lack of dignity!",ch,0,dir_name[door],TO_ROOM);
    act("You fly into the door $T but simply bounce off it like a lump of rock!",ch,0,dir_name[door],TO_CHAR);
    damage_old(ch,ch,dice(3,5),gsn_door_bash,DAM_BASH,TRUE);
    check_improve(ch,gsn_door_bash,FALSE,1);
    WAIT_STATE(ch,12);
    return;
    }
    act("$n slams into the $T door and throws it open with a mighty crash!",ch,0,dir_name[door],TO_ROOM);
    act("You slam into the $T door and it cracks open with a deafening sound!",ch,0,dir_name[door],TO_CHAR);
    check_improve(ch,gsn_door_bash,TRUE,1);
    WAIT_STATE(ch,12);
    REMOVE_BIT(pexit->exit_info,EX_LOCKED);
    REMOVE_BIT(pexit->exit_info,EX_CLOSED);

    if ( ( (to_room = pexit->u1.to_room) != NULL)
    && ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
    && pexit_rev->u1.to_room == ch->in_room)
        {
        REMOVE_BIT(pexit_rev->exit_info,EX_LOCKED);
        REMOVE_BIT(pexit_rev->exit_info,EX_CLOSED);
        }

    if (number_percent() < get_curr_stat(ch,STAT_DEX))
        return;

/*
 * Affect to char so in move_char you relay the right move message, then
 *  strip affect once moved.
 */
        init_affect(&af);
    af.where = TO_AFFECTS;
    af.type = gsn_door_bash;
        af.aftype    = AFT_SKILL;
    af.location = 0;
    af.modifier = 0;
    af.duration = -1;
    af.bitvector = 0;
    af.level = ch->level;
    affect_to_char(ch,&af);

    move_char(ch,door,FALSE);
    affect_strip(ch,gsn_door_bash);
    }

return;
}

/* For selecting new home */

void do_hometown(CHAR_DATA *ch, char *argument)
{
        int align;
        int cost;
        char arg[MAX_STRING_LENGTH];
        char arg2[MAX_STRING_LENGTH];
        char buf[MAX_STRING_LENGTH];

        if (IS_NPC(ch))
                return;

        if (ch->in_room->vnum != ROOM_VNUM_HOMETOWNS)
        {
        send_to_char("You can't do that here.\n\r",ch);
        return;
        }

        argument = one_argument(argument,arg);
        one_argument(argument,arg2);

        /* Cost is in silver right now */
        cost = (ch->level * ch->level);
        cost *=  50;

        align = ch->alignment;

if (arg[0] == '\0')
{
        send_to_char("Available hometowns are: \n\r",ch);
        if (ch->temple == TEMPLE_MIDGAARD)
                send_to_char("Midgaard          Current hometown.\n\r",ch);
        else
        {
                sprintf(buf, "Midgaard          %d silver.\n\r",cost);
                send_to_char(buf,ch);
        }

        if (align == 0)
        {
        if (ch->temple == TEMPLE_NEW_THALOS)
                send_to_char("New Thalos        Current hometown.\n\r",ch);
        else
        {
                sprintf(buf, "New Thalos        %d silver.\n\r",cost * 5/4);
                send_to_char(buf,ch);
        }
	}
        else if (align > 0)
        {
        if (ch->temple == TEMPLE_OFCOL)
                send_to_char("Ofcol             Current hometown.\n\r",ch);
        else
        {
                sprintf(buf, "Ofcol             %d silver.\n\r",cost * 5/4);
                send_to_char(buf,ch);
        }
	}
        else
        {
        if (ch->temple == TEMPLE_ARKHAM)
                send_to_char("Arkham            Current hometown.\n\r",ch);
        else
        	{
                sprintf(buf, "Arkham            %d silver.\n\r",cost * 5/4);
                send_to_char(buf,ch);
        	}
	}

	send_to_char("To pay using your bank account: hometown <town> bank\n\r",ch);
        return;
}

if (!str_cmp(arg,"midgaard"))
{
        if (ch->temple == TEMPLE_MIDGAARD)
        {
        send_to_char("But that's already your current hometown.\n\r",ch);
        return;
        }

        if (!str_cmp(arg2,"bank"))
        {
        if (ch->silver_bank < cost)
        {
        send_to_char("You don't have the silver in your bank account for this hometown.\n\r",ch);
        return;
        }
	else
	{
send_to_char("Your hometown is now Midgaard.\n\r",ch);
ch->silver_bank -= cost;
ch->temple = TEMPLE_MIDGAARD;
return;
	}
        }

        if (ch->silver < cost)
        {
        send_to_char("You don't have the silver for that hometown.\n\r",ch);
        return;
        }
        ch->temple = TEMPLE_MIDGAARD;
        ch->silver -= cost;
        send_to_char("Your hometown is now Midgaard.\n\r",ch);
        return;
}

if (!str_cmp(arg,"new") || !str_cmp(arg,"thalos")
        || !str_cmp(arg,"new thalos") )
{
        if (align != 0)
        {
        send_to_char("Only neutrals may choose New Thalos for a hometown.\n\r",ch);
        return;
        }
        if (ch->temple == TEMPLE_NEW_THALOS)
        {
        send_to_char("But that's already your current hometown.\n\r",ch);
        return;
        }
        cost *= 5;
        cost /= 4;
        if (!str_cmp(arg2,"bank"))
        {
        if (ch->silver_bank < cost)
        {
        send_to_char("You don't have the silver in your bank account for this hometown.\n\r",ch);
        return;
        }
	else
	{
send_to_char("Your hometown is now New Thalos.\n\r",ch);
ch->silver_bank -= cost;
ch->temple = TEMPLE_NEW_THALOS;
return;
	}

        }

        if (ch->silver < cost )
        {
        send_to_char("You don't have the silver for that hometown.\n\r",ch);
        return;
        }
        ch->temple = TEMPLE_NEW_THALOS;
        ch->silver -= cost;
        send_to_char("Your hometown is now New Thalos.\n\r",ch);
        return;
}

if (!str_cmp(arg,"ofcol"))
{
        if (align != 1000)
        {
        send_to_char("Only good aligns may choose Ofcol for a hometown.\n\r",ch);
        return;
        }

        if (ch->temple == TEMPLE_OFCOL)
        {
        send_to_char("But that's already your current hometown.\n\r",ch);
        return;
        }
        cost *= 5;
        cost /= 4;

        if (!str_cmp(arg2,"bank"))
        {
        if (ch->silver_bank < cost)
        {
        send_to_char("You don't have the silver in your bank account for this hometown.\n\r",ch);
        return;
        }
	else
	{
send_to_char("Your hometown is now Ofcol.\n\r",ch);
ch->silver_bank -= cost;
ch->temple = TEMPLE_OFCOL;
return;
	}

        }

        if (ch->silver < cost)
        {
        send_to_char("You don't have the silver for that hometown.\n\r",ch);
        return;
        }
        ch->temple = TEMPLE_OFCOL;
        ch->silver -= cost;
        send_to_char("Your hometown is now Ofcol.\n\r",ch);
        return;
}

if (!str_cmp(arg,"arkham"))
{
        if (align != -1000)
        {
        send_to_char("Only evil aligns may choose Arkham for a hometown.\n\r",ch);
        return;
        }

        if (ch->temple == TEMPLE_ARKHAM)
        {
        send_to_char("But that's already your current hometown.\n\r",ch);
        return;
        }
        cost *= 5;
        cost /= 4;

        if (!str_cmp(arg2,"bank"))
        {
        if (ch->silver_bank < cost)
        {
        send_to_char("You don't have the silver in your bank account for this hometown.\n\r",ch);
        return;
        }
	else
	{
send_to_char("Your hometown is now Arkham.\n\r",ch);
ch->silver_bank -= cost;
ch->temple = TEMPLE_ARKHAM;
return;
	}

        }

        if (ch->silver < cost)
        {
        send_to_char("You don't have the silver for that hometown.\n\r",ch);
        return;
        }
        ch->temple = TEMPLE_ARKHAM;
        ch->silver -= cost;
        send_to_char("Your hometown is now Arkham.\n\r",ch);
        return;
}

send_to_char("That city doesn't exist for choosing as a hometown. Type 'hometown' to see list.\n\r",ch);
return;
}

/* mob prog */

void move_prog_bones(CHAR_DATA *ch)
{
    CHAR_DATA *bone1;
    CHAR_DATA *bone2;
    CHAR_DATA *bone3;
    CHAR_DATA *bone4;
    int num;
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *vch1;
    CHAR_DATA *vch2;
    CHAR_DATA *vch3;

    if (ch->in_room->vnum != 13868
    && ch->in_room->vnum != 13869
    && ch->in_room->vnum != 13871
    && ch->in_room->vnum != 13872
    && ch->in_room->vnum != 13884
    && ch->in_room->vnum != 13873)
        return;
    if (number_percent() > 20)
        return;
    bone1 = create_mobile(get_mob_index(MOB_VNUM_BONES));
    if (bone1 == NULL)
        return;

    act("The bones around you suddenly stir and rise up!",ch,0,0,TO_CHAR);
    act("The bones around you suddenly stir and rise up!",ch,0,0,TO_ROOM);

    switch(number_range(0,5))
    {
        case 1:
        sprintf(buf,"Die, thou who invades out rest!"); break;
        case 2:
        sprintf(buf,"%s, you shall pay for your desecration of our bones!",ch->name);
        break;
        case 3:
        sprintf(buf,"Flesh...flesh for us at last...");break;
        case 4:
        sprintf(buf,"I feel the warmth of life...give me your body so i may live!");
        break;
        case 5:
        sprintf(buf,"I sense your warm blood...let me feed...");
        break;
    }

    for (vch1 = ch->in_room->people; vch1 != NULL; vch1 = vch1->next_in_room)
    {
        if (is_same_group(vch1,ch) && number_bits(2) == 0)
            break;
    }
    if (vch1 == NULL)
        vch1 = ch;

    for (vch2 = ch->in_room->people; vch2 != NULL; vch2 = vch2->next_in_room)
    {
        if (is_same_group(vch2,ch) && number_bits(2) == 0)
            break;
    }
    if (vch2 == NULL)
        vch2 = ch;

    for (vch3 = ch->in_room->people; vch3 != NULL; vch3 = vch3->next_in_room)
    {
        if (is_same_group(vch3,ch) && number_bits(2) == 0)
            break;
    }
    if (vch3 == NULL)
        vch3 = ch;

    num = number_range(1,4);

    char_to_room(bone1,ch->in_room);

    do_say(bone1,buf);
    multi_hit(bone1,ch,TYPE_UNDEFINED);

    if (num >= 2)
    {
    bone2 = create_mobile(get_mob_index(MOB_VNUM_BONES));
    char_to_room(bone2,ch->in_room);
    act("$n rises up and attacks!",bone2,0,0,TO_ROOM);
    multi_hit(bone2,vch1,TYPE_UNDEFINED);
    }
    if (num >= 3)
    {
    bone3 = create_mobile(get_mob_index(MOB_VNUM_BONES));
    char_to_room(bone3,ch->in_room);
    act("$n rises up and attacks!",bone3,0,0,TO_ROOM);
    multi_hit(bone3,vch2,TYPE_UNDEFINED);
    }
    if (num >= 4)
    {
    bone4 = create_mobile(get_mob_index(MOB_VNUM_BONES));
    char_to_room(bone4,ch->in_room);
    act("$n rises up and attacks!",bone4,0,0,TO_ROOM);
    multi_hit(bone4,vch3,TYPE_UNDEFINED);
    }
    return;
}

void move_prog_hydra(CHAR_DATA *ch)
{
    CHAR_DATA *hydra;
    CHAR_DATA *vch;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
        return;
    if (ch->in_room->vnum != 13874)
        return;

    for (hydra = char_list; hydra != NULL; hydra = hydra->next)
    {
        if (!IS_NPC(hydra)) continue;
        if (hydra->pIndexData->vnum == MOB_VNUM_HYDRA)
            break;
    }
    if (hydra != NULL)
        return;
    hydra = create_mobile(get_mob_index(MOB_VNUM_HYDRA));
    if (hydra == NULL)
        return;
    char_to_room(hydra,ch->in_room);

    act("The broken wood of the ships explodes as $n rises up in fury!",hydra,0,0,TO_ROOM);


    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
        if (is_same_group(vch,ch) && number_bits(2) == 0)
            break;
    }

    if (vch == NULL)
        vch = ch;

        sprintf(buf,"Help! %s is attacking me!",PERS(hydra,vch));
        do_yell(vch,buf);
        multi_hit(hydra,vch,TYPE_UNDEFINED);
        return;
}


void mob_entry_wraith(CHAR_DATA *ch)
{
    CHAR_DATA *wraith;

    if (IS_NPC(ch))
        return;
    if (str_cmp(ch->in_room->area->name,"mythforest.are")
    || number_percent() > 4)
        return;
    wraith = create_mobile(get_mob_index(13732));
    if (wraith == NULL)
        return;
    char_to_room(wraith,ch->in_room);
    act("A shimmering wraith rises from the damp forest ground in a swirling mist.",wraith,0,0,TO_ROOM);
    do_yell(ch,"Help! I'm being attacked by a mist wraith!");
    multi_hit(wraith,ch,TYPE_UNDEFINED);
    return;
}


