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
DECLARE_DO_FUN(do_split		);
DECLARE_DO_FUN(do_yell		);
DECLARE_DO_FUN(do_say		);
DECLARE_DO_FUN(do_wake		);
DECLARE_DO_FUN(do_cb            );
DECLARE_DO_FUN(do_murder        );
DECLARE_DO_FUN(do_tell		);

/*
 * Local functions.
 */
#define CD CHAR_DATA
#define OD OBJ_DATA
#define CABAL_ITEMS_FILE	"citems.txt"
bool	remove_obj	args( (CHAR_DATA *ch, int iWear, bool fReplace ) );
void	wear_obj	args( (CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace ) );
CD *	find_keeper	args( (CHAR_DATA *ch ) );
int	get_cost	args( (CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy ) );
void 	obj_to_keeper	args( (OBJ_DATA *obj, CHAR_DATA *ch ) );
OD *	get_obj_keeper	args( (CHAR_DATA *ch,CHAR_DATA *keeper,char *argument));

/* A pile of obj progs for use in legion.are, and the drannor areas.
Remove these if you don't want to use those areas
*/
bool sacrifice_obj_bracelet args((CHAR_DATA *ch,OBJ_DATA *obj));
void wear_obj_tiara     args( ( CHAR_DATA *ch, OBJ_DATA *obj) );
void remove_obj_tiara   args( ( CHAR_DATA *ch, OBJ_DATA *obj) );
void wear_obj_wizardry  args( ( CHAR_DATA *ch, OBJ_DATA *obj) );
void remove_obj_wizardry args( ( CHAR_DATA *ch, OBJ_DATA *obj) );
void remove_obj_sceptre args((CHAR_DATA *ch,OBJ_DATA *obj));
void obj_brandish_horn_valere args((CHAR_DATA *ch,OBJ_DATA *obj));
void obj_brandish_phylactery args((CHAR_DATA *ch,OBJ_DATA *obj));
void obj_give_hydra_head args((CHAR_DATA *ch,CHAR_DATA *lady,OBJ_DATA *obj));
void wear_obj_zhentil_robe args((CHAR_DATA *ch,OBJ_DATA *obj));
void wear_obj_fallen_wings args((CHAR_DATA *ch,OBJ_DATA *obj));
void remove_obj_fallen_wings args((CHAR_DATA *ch,OBJ_DATA *obj));
void wear_obj_black_hide args((CHAR_DATA *ch,OBJ_DATA *obj));
void remove_obj_black_hide args((CHAR_DATA *ch,OBJ_DATA *obj));
void wear_obj_symbol_magic args((CHAR_DATA *ch,OBJ_DATA *obj));
void remove_obj_symbol_magic args((CHAR_DATA *ch,OBJ_DATA *obj));
void wear_obj_red_dragonstar args((CHAR_DATA *ch,OBJ_DATA *obj));
void wear_obj_black_dragonstar args((CHAR_DATA *ch,OBJ_DATA *obj));
void remove_obj_red_dragonstar args((CHAR_DATA *ch,OBJ_DATA *obj));
void remove_obj_black_dragonstar args((CHAR_DATA *ch,OBJ_DATA *obj));
void recite_libram_conjuration args((CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *scroll,OBJ_DATA *obj,bool fRead));
void brandish_sceptre_dead args((CHAR_DATA *ch,OBJ_DATA *obj));
void wear_obj_cloak_underworld args((CHAR_DATA *ch,OBJ_DATA *obj));
void remove_obj_cloak_underworld args((CHAR_DATA *ch,OBJ_DATA *obj));
void wear_obj_misty_cloak args((CHAR_DATA *ch,OBJ_DATA *obj));
void remove_obj_misty_cloak args((CHAR_DATA *ch,OBJ_DATA *obj));
void wear_obj_gauntlets_striking args((CHAR_DATA *ch, OBJ_DATA *obj));
void remove_obj_gauntlets_striking args((CHAR_DATA *ch,OBJ_DATA *obj));
void save_cabal_items args( ( void ) );
bool arena;
bool    isCabalGuard( CHAR_DATA *ch );
void check_eagle_eyes(CHAR_DATA *ch, CHAR_DATA *victim);



#undef OD
#undef	CD

/* RT part of the corpse looting code */

bool can_loot(CHAR_DATA *ch, OBJ_DATA *obj)
{
    CHAR_DATA *owner, *wch;
    char buf[MAX_STRING_LENGTH];

    if (obj->item_type == ITEM_CORPSE_PC)
    {
	sprintf(buf,"%s looting %s.",IS_NPC(ch) ?
((ch->master == NULL) ? "Unknown mob" : ch->master->name) : ch->name,
obj->short_descr);
	log_string(buf);
	ch->pause = 5;
    }

    return TRUE;

    if (!obj->owner || obj->owner == NULL)
	return TRUE;

    if(!IS_NPC(ch) && ch->pcdata->newbie==TRUE)
	{
		send_to_char("You can't loot other players as a newbie.\n\r",ch);
		return FALSE;
	}
    return TRUE;

    owner = NULL;
    for ( wch = char_list; wch != NULL ; wch = wch->next )
        if (!str_cmp(wch->name,obj->owner))
            owner = wch;

    if (owner == NULL)
	return TRUE;

    if (!str_cmp(ch->name,owner->name))
	return TRUE;

    if (!IS_NPC(owner) && IS_SET(owner->act,PLR_CANLOOT))
	return TRUE;

    if (is_same_group(ch,owner))
	return TRUE;

    return FALSE;
}


void get_obj( CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *container )
{
    /* variables for AUTOSPLIT */
    CHAR_DATA *gch;
    int members;
    char buffer[100];

    if ( !CAN_WEAR(obj, ITEM_TAKE) )
    {
	send_to_char( "You can't take that.\n\r", ch );
	return;
    }

    if ( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
    {
	act( "$d: you can't carry that many items.",
	    ch, NULL, obj->name, TO_CHAR );
	return;
    }

    if ((!obj->in_obj || obj->in_obj->carried_by != ch)
    &&  (get_carry_weight(ch) + get_obj_weight(obj) > can_carry_w(ch)))
    {
	act( "$d: you can't carry that much weight.",
	    ch, NULL, obj->name, TO_CHAR );
	return;
    }

    if (!can_loot(ch,obj))
    {
	act("Corpse looting is not permitted.",ch,NULL,NULL,TO_CHAR );
	return;
    }

    if (obj->in_room != NULL)
    {
	for (gch = obj->in_room->people; gch != NULL; gch = gch->next_in_room)
	    if (gch->on == obj)
	    {
		act("$N appears to be using $p.",
		    ch,obj,gch,TO_CHAR);
		return;
	    }
    }


    if ( container != NULL )
    {
/*
    	if (container->pIndexData->vnum == OBJ_VNUM_PIT
	&&  get_trust(ch) < obj->level)
	{
	    send_to_char("You are not powerful enough to use it.\n\r",ch);
	    return;
	}
*/
    	if (container->pIndexData->vnum == OBJ_VNUM_PIT
	&&  !CAN_WEAR(container, ITEM_TAKE)
	&&  !IS_OBJ_STAT(obj,ITEM_HAD_TIMER))
	    obj->timer = 0;
	act( "You get $p from $P.", ch, obj, container, TO_CHAR );
	if (!IS_AFFECTED(ch,AFF_SNEAK))
	act( "$n gets $p from $P.", ch, obj, container, TO_ROOM );
	REMOVE_BIT(obj->extra_flags,ITEM_HAD_TIMER);
	obj_from_obj( obj );
    }
    else
    {
	act( "You get $p.", ch, obj, container, TO_CHAR );
	if (!IS_AFFECTED(ch,AFF_SNEAK))
	act( "$n gets $p.", ch, obj, container, TO_ROOM );
	obj_from_room( obj );
    }

    if ( obj->item_type == ITEM_MONEY)
    {
	ch->gold += obj->value[1];
        if (IS_SET(ch->act,PLR_AUTOSPLIT))
        { /* AUTOSPLIT code */
    	  members = 0;
    	  for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    	  {
            if (!IS_AFFECTED(gch,AFF_CHARM) && is_same_group( gch, ch ) )
              members++;
    	  }

	  if ( members > 1 && (obj->value[0] > 1 || obj->value[1]))
	  {
	    sprintf(buffer,"%d",obj->value[1]);
	    do_split(ch,buffer);
	  }
        }

	extract_obj( obj );
    }
    else
    {
	obj_to_char( obj, ch );
    }

    return;
}

void do_get( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    OBJ_DATA *container;
    bool found;
    found = FALSE;
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (isShifted(ch)  && !IS_IMMORTAL(ch))
    {
	send_to_char("You can only do this in your human form.\n\r",ch);
       	return;
    }

    if (!str_cmp(arg2,"from"))
	argument = one_argument(argument,arg2);

    /* Get type. */
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Get what?\n\r", ch );
	return;
    }

/*	if (strstr(arg1,"all") || strstr(arg1,"All") && strstr(arg2, "corpse") || strstr(arg2, "Corpse"))
	{
	    send_to_char( "You can't do that.\n\r", ch );
	    return;
	}
*/
	if (!IS_NPC(ch))
	{
	if (ch->pcdata->death_status == HAS_DIED)
	{
	send_to_char("You are a hovering spirit, you can't pick up anything!\n\r",ch);
	return;
	}
	}

    if ( arg2[0] == '\0' )
    {
	if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
	{
	    /* 'get obj' */
	    obj = get_obj_list( ch, arg1, ch->in_room->contents );
	    if ( obj == NULL )
	    {
		act( "I see no $T here.", ch, NULL, arg1, TO_CHAR );
		return;
	    }

	    get_obj( ch, obj, NULL );

    if (cant_carry(ch,obj))
    {
	act("$n is burnt by $p and drops it.",ch,obj,0,TO_ROOM);
	act("You are burnt by $p and drop it.",ch,obj,0,TO_CHAR);
	obj_from_char(obj);
	obj_to_room(obj,ch->in_room);
	if (IS_NPC(ch))
	{
	act("$n is vaporised by the power of $p!",ch,obj,0,TO_ROOM);
	raw_kill(ch,ch);
	}
    }

	}
	else
	{
	    /* 'get all' or 'get all.obj' */
	    found = FALSE;
	    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	    {
		obj_next = obj->next_content;
		if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
		&&   can_see_obj( ch, obj ) )
		{
		    found = TRUE;
		    get_obj( ch, obj, NULL );

    if (cant_carry(ch,obj))
    {
	act("$n is burnt by $p and drops it.",ch,obj,0,TO_ROOM);
	act("You are burnt by $p and drop it.",ch,obj,0,TO_CHAR);
	obj_from_char(obj);
	obj_to_room(obj,ch->in_room);
	if (IS_NPC(ch))
	{
	act("$n is vaporised by the power of $p!",ch,obj,0,TO_ROOM);
	raw_kill(ch,ch);
	}
    }

		}
	    }

	    if ( !found )
	    {
		if ( arg1[3] == '\0' )
		    send_to_char( "I see nothing here.\n\r", ch );
		else
		    act( "I see no $T here.", ch, NULL, &arg1[4], TO_CHAR );
	    }
	}
    }
    else
    {
	/* 'get ... container' */
	if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
	{
	    send_to_char( "You can't do that.\n\r", ch );
	    return;
	}

	if ( ( container = get_obj_here( ch, arg2 ) ) == NULL )
	{
	    act( "I see no $T here.", ch, NULL, arg2, TO_CHAR );
	    return;
	}

	switch ( container->item_type )
	{
	default:
	    send_to_char( "That's not a container.\n\r", ch );
	    return;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	    break;

	case ITEM_CORPSE_PC:
	    {

		if (!can_loot(ch,container))
		{
		    send_to_char( "You can't do that.\n\r", ch );
		    return;
		}
	    }
	}

	if ( IS_SET(container->value[1], CONT_CLOSED) )
	{
	    act( "The $d is closed.", ch, NULL, container->name, TO_CHAR );
	    return;
	}

	if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
	{
	    /* 'get obj container' */
	    obj = get_obj_list( ch, arg1, container->contains );
	    if ( obj == NULL )
	    {
		act( "I see nothing like that in the $T.",
		    ch, NULL, arg2, TO_CHAR );
		return;
	    }
	    get_obj( ch, obj, container );

    if (cant_carry(ch,obj))
    {
	act("$n is burnt by $p and drops it.",ch,obj,0,TO_ROOM);
	act("You are burnt by $p and drop it.",ch,obj,0,TO_CHAR);
	obj_from_char(obj);
	obj_to_room(obj,ch->in_room);
	if (IS_NPC(ch))
	{
	act("$n is vaporised by the power of $p!",ch,obj,0,TO_ROOM);
	raw_kill(ch,ch);
	}
    }

	}
	else
	{
	    /* 'get all container' or 'get all.obj container' */
	    found = FALSE;
	    for ( obj = container->contains; obj != NULL; obj = obj_next )
	    {
		obj_next = obj->next_content;
		if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
		&&   can_see_obj( ch, obj ) )
		{
		    found = TRUE;

		    get_obj( ch, obj, container );
    if (cant_carry(ch,obj))
    {
	act("$n is burnt by $p and drops it.",ch,obj,0,TO_ROOM);
	act("You are burnt by $p and drop it.",ch,obj,0,TO_CHAR);
	obj_from_char(obj);
	obj_to_room(obj,ch->in_room);
	if (IS_NPC(ch))
	{
	act("$n is vaporised by the power of $p!",ch,obj,0,TO_ROOM);
	raw_kill(ch,ch);
	}
    }

		}
	    }

	    if ( !found )
	    {
		if ( arg1[3] == '\0' )
		    act( "I see nothing in the $T.",
			ch, NULL, arg2, TO_CHAR );
		else
		    act( "I see nothing like that in the $T.",
			ch, NULL, arg2, TO_CHAR );
	    }
	}
    }

    return;
}


void do_put( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *container;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (!str_cmp(arg2,"in") || !str_cmp(arg2,"on"))
	argument = one_argument(argument,arg2);

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Put what in what?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return;
    }

    if ( ( container = get_obj_here( ch, arg2 ) ) == NULL )
    {
	act( "I see no $T here.", ch, NULL, arg2, TO_CHAR );
	return;
    }

    if ( container->item_type != ITEM_CONTAINER )
    {
	send_to_char( "That's not a container.\n\r", ch );
	return;
    }

    if ( IS_SET(container->value[1], CONT_CLOSED) )
    {
	act( "The $d is closed.", ch, NULL, container->name, TO_CHAR );
	return;
    }

    if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
    {
	/* 'put obj container' */
	if ( ( obj = get_obj_carry( ch, arg1, ch ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}

	if ( obj == container )
	{
	    send_to_char( "You can't fold it into itself.\n\r", ch );
	    return;
	}

	if ( !can_drop_obj( ch, obj ) )
	{
	    send_to_char( "You can't let go of it.\n\r", ch );
	    return;
	}

	if (obj->pIndexData->limtotal > 0)
	{
	    send_to_char( "That item is too powerful to be contained.\n\t", ch );
	    return;
	}

    if (isCabalItem(obj))
	{
	    send_to_char( "That item is too powerful to be contained.\n\t", ch );
	    return;
	}


    	if (WEIGHT_MULT(obj) != 100)
    	{
           send_to_char("You have a feeling that would be a bad idea.\n\r",ch);
            return;
        }

	if (get_obj_weight( obj ) + get_true_weight( container )
	     > (container->value[0] * 10)
	||  get_obj_weight(obj) > (container->value[3] * 10))
	{
	    send_to_char( "It won't fit.\n\r", ch );
	    return;
	}

	if (container->pIndexData->vnum == OBJ_VNUM_PIT
	&&  !CAN_WEAR(container,ITEM_TAKE))
	{
	    if (obj->timer)
		SET_BIT(obj->extra_flags,ITEM_HAD_TIMER);
	    else
	        obj->timer = number_range(100,200);

	}
	obj_from_char( obj );
	obj_to_obj( obj, container );

	if (IS_SET(container->value[1],CONT_PUT_ON))
	{
	    act("$n puts $p on $P.",ch,obj,container, TO_ROOM);
	    act("You put $p on $P.",ch,obj,container, TO_CHAR);
	}
	else
	{
	    act( "$n puts $p in $P.", ch, obj, container, TO_ROOM );
	    act( "You put $p in $P.", ch, obj, container, TO_CHAR );
	}
    }
    else
    {
	/* 'put all container' or 'put all.obj container' */
	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;

	    if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
	    &&   can_see_obj( ch, obj )
	    &&   WEIGHT_MULT(obj) == 100
	    &&   obj->wear_loc == WEAR_NONE
	    &&   obj != container
	    &&   obj->pIndexData->limtotal <= 0
	    &&   can_drop_obj( ch, obj )
	    &&   get_obj_weight( obj ) + get_true_weight( container )
		 <= (container->value[0] * 10)
	    &&   get_obj_weight(obj) < (container->value[3] * 10)
    && (!isCabalItem(obj)))
	    {
	    	if (container->pIndexData->vnum == OBJ_VNUM_PIT
	    	&&  !CAN_WEAR(obj, ITEM_TAKE) )
		{ /*test*/
	    	    if (obj->timer)
			SET_BIT(obj->extra_flags,ITEM_HAD_TIMER);
	    	    else
	    	    	obj->timer = number_range(100,200);
		} /*test*/
		obj_from_char( obj );
		obj_to_obj( obj, container );

        	if (IS_SET(container->value[1],CONT_PUT_ON))
        	{
            	    act("$n puts $p on $P.",ch,obj,container, TO_ROOM);
            	    act("You put $p on $P.",ch,obj,container, TO_CHAR);
        	}
		else
		{
		    act( "$n puts $p in $P.", ch, obj, container, TO_ROOM );
		    act( "You put $p in $P.", ch, obj, container, TO_CHAR );
		}
	    }
	}
    }

    return;
}



void do_drop( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    bool found;

    argument = one_argument( argument, arg );

    if( isShifted(ch)  && !IS_IMMORTAL(ch))
    {
      	send_to_char("You can only do this in your human form.\n\r",ch);
     	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Drop what?\n\r", ch );
	return;
    }

    if ( is_number( arg ) )
    {
	/* 'drop NNNN coins' */
	int amount, gold = 0, silver = 0;

	amount   = atoi(arg);
	argument = one_argument( argument, arg );
	if ( amount <= 0
	|| ( str_cmp( arg, "coins" ) && str_cmp( arg, "coin" ) &&
	     str_cmp( arg, "gold"  ) && str_cmp( arg, "silver") ) )
	{
	    send_to_char( "Sorry, you can't do that.\n\r", ch );
	    return;
	}

	if ( !str_cmp( arg, "coins") || !str_cmp(arg,"coin")
	||   !str_cmp( arg, "silver"))
	{
	    if (ch->silver < amount)
	    {
		send_to_char("You don't have that much silver.\n\r",ch);
		return;
	    }

	    ch->silver -= amount;
	    silver = amount;
	}

	else
	{
	    if (ch->gold < amount)
	    {
		send_to_char("You don't have that much gold.\n\r",ch);
		return;
	    }

	    ch->gold -= amount;
  	    gold = amount;
	}

	for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;

	    switch ( obj->pIndexData->vnum )
	    {
	    case OBJ_VNUM_SILVER_ONE:
		silver += 1;
		extract_obj(obj);
		break;

	    case OBJ_VNUM_GOLD_ONE:
		gold += 1;
		extract_obj( obj );
		break;

	    case OBJ_VNUM_SILVER_SOME:
		silver += obj->value[0];
		extract_obj(obj);
		break;

	    case OBJ_VNUM_GOLD_SOME:
		gold += obj->value[1];
		extract_obj( obj );
		break;

	    case OBJ_VNUM_COINS:
		silver += obj->value[0];
		gold += obj->value[1];
		extract_obj(obj);
		break;
	    }
	}

	obj_to_room( create_money( gold, silver ), ch->in_room );
	if (!IS_AFFECTED(ch,AFF_SNEAK))
	act( "$n drops some coins.", ch, NULL, NULL, TO_ROOM );
	send_to_char( "OK.\n\r", ch );
	return;
    }

    if ( str_cmp( arg, "all" ) && str_prefix( "all.", arg ) )
    {
	/* 'drop obj' */
	if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}

	if ( !can_drop_obj( ch, obj ) )
	{
	    send_to_char( "You can't let go of it.\n\r", ch );
	    return;
	}
	if (isCabalItem(obj))
	{
	    send_to_char("Mr. T wouldn't like that.\n\r", ch);
	    return;
	}
	obj_from_char( obj );
	obj_to_room( obj, ch->in_room );
	if (!IS_AFFECTED(ch,AFF_SNEAK))
	act( "$n drops $p.", ch, obj, NULL, TO_ROOM );
	act( "You drop $p.", ch, obj, NULL, TO_CHAR );
	if (IS_OBJ_STAT(obj,ITEM_MELT_DROP))
	{
	    act("$p dissolves into smoke.",ch,obj,NULL,TO_ROOM);
	    act("$p dissolves into smoke.",ch,obj,NULL,TO_CHAR);
	    extract_obj(obj);
	}
    }
    else
    {
	/* 'drop all' or 'drop all.obj' */
	found = FALSE;
	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;

	    if ( ( arg[3] == '\0' || is_name( &arg[4], obj->name ) )
	    &&   can_see_obj( ch, obj )
	    &&   obj->wear_loc == WEAR_NONE
	    &&   can_drop_obj( ch, obj ) )
	    {
		found = TRUE;
		if (isCabalItem(obj))
		{
		    send_to_char("Mr. T wouldn't like that.\n\r", ch);
		    return;
 		}
		obj_from_char( obj );
		obj_to_room( obj, ch->in_room );
		if (!IS_AFFECTED(ch,AFF_SNEAK))
		act( "$n drops $p.", ch, obj, NULL, TO_ROOM );
		act( "You drop $p.", ch, obj, NULL, TO_CHAR );
        	if (IS_OBJ_STAT(obj,ITEM_MELT_DROP))
        	{
             	    act("$p dissolves into smoke.",ch,obj,NULL,TO_ROOM);
            	    act("$p dissolves into smoke.",ch,obj,NULL,TO_CHAR);
            	    extract_obj(obj);
        	}
	    }
	}

	if ( !found )
	{
	    if ( arg[3] == '\0' )
		act( "You are not carrying anything.",
		    ch, NULL, arg, TO_CHAR );
	    else
		act( "You are not carrying any $T.",
		    ch, NULL, &arg[4], TO_CHAR );
	}
    }

    return;
}

void do_give( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    bool boolCabalItem = FALSE;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if( isShifted(ch)  && !IS_IMMORTAL(ch))
    {
      	send_to_char("You can only do this in your human form.\n\r",ch);
      	return;
    }

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Give what to whom?\n\r", ch );
	return;
    }


    if ( is_number( arg1 ) )
    {
	/* 'give NNNN coins victim' */
	int amount;
	bool silver;

	amount   = atoi(arg1);
	if ( amount <= 0
	|| ( str_cmp( arg2, "coins" ) && str_cmp( arg2, "coin" ) &&
	     str_cmp( arg2, "gold"  ) && str_cmp( arg2, "silver")) )
	{
	    send_to_char( "Sorry, you can't do that.\n\r", ch );
	    return;
	}

	silver = str_cmp(arg2,"gold");

	argument = one_argument( argument, arg2 );
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Give what to whom?\n\r", ch );
	    return;
	}

	if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	if ( (!silver && ch->gold < amount) || (silver && ch->silver < amount) )
	{
	    send_to_char( "You haven't got that much.\n\r", ch );
	    return;
	}

	if (silver)
	{
	    ch->silver		-= amount;
	    victim->silver 	+= amount;
	}
	else
	{
	    ch->gold		-= amount;
	    victim->gold	+= amount;
	}

	sprintf(buf,"$n gives you %d %s.",amount, silver ? "silver" : "gold");
	act( buf, ch, NULL, victim, TO_VICT    );
	act( "$n gives $N some coins.",  ch, NULL, victim, TO_NOTVICT );
	sprintf(buf,"You give $N %d %s.",amount, silver ? "silver" : "gold");
	act( buf, ch, NULL, victim, TO_CHAR    );

        /*
         * Bribe trigger
         */
        if ( IS_NPC(victim) && HAS_TRIGGER( victim, TRIG_BRIBE ) )
            mp_bribe_trigger( victim, ch, silver ? amount : amount * 100 );

	if (IS_NPC(victim) && IS_SET(victim->act,ACT_IS_CHANGER))
	{
	    int change;

	    change = (silver ? 95 * amount / 100 / 100
		 	     : 95 * amount);


	    if (!silver && change > victim->silver)
	    	victim->silver += change;

	    if (silver && change > victim->gold)
		victim->gold += change;

	    if (change < 1 && can_see(victim,ch))
	    {
		act(
	"$n tells you 'I'm sorry, you did not give me enough to change.'"
		    ,victim,NULL,ch,TO_VICT);
		ch->reply = victim;
		sprintf(buf,"%d %s %s",
			amount, silver ? "silver" : "gold",ch->name);
		do_give(victim,buf);
	    }
	    else if (can_see(victim,ch))
	    {
		sprintf(buf,"%d %s %s",
			change, silver ? "gold" : "silver",ch->name);
		do_give(victim,buf);
		if (silver)
		{
		    sprintf(buf,"%d silver %s",
			(95 * amount / 100 - change * 100),ch->name);
		    do_give(victim,buf);
		}
		act("$n tells you 'Thank you, come again.'",
		    victim,NULL,ch,TO_VICT);
		ch->reply = victim;
	    }
	}
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg1, ch ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if ( obj->wear_loc != WEAR_NONE )
    {
	send_to_char( "You must remove it first.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (IS_NPC(victim) && victim->pIndexData->pShop != NULL)
    {
	act("$N tells you 'Sorry, you'll have to sell that.'",
	    ch,NULL,victim,TO_CHAR);
	ch->reply = victim;
	return;
    }

    if ( !can_drop_obj( ch, obj ) )
    {
	send_to_char( "You can't let go of it.\n\r", ch );
	return;
    }

    if ( victim->carry_number + get_obj_number( obj ) > can_carry_n( victim ) )
    {
	act( "$N has $S hands full.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if (get_carry_weight(victim) + get_obj_weight(obj) > can_carry_w( victim ) )
    {
	act( "$N can't carry that much weight.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( !can_see_obj( victim, obj ) )
    {
	act( "$N can't see it.", ch, NULL, victim, TO_CHAR );
	return;
    }

	if ((obj->pIndexData->vnum == OBJ_VNUM_ANCIENT_SCALP)
	&& ((victim->pIndexData->vnum == MOB_VNUM_POINTMASTER)))
	{
	if (IS_NPC(victim))
	{
		if (victim->pIndexData->vnum != MOB_VNUM_POINTMASTER)
		{
			send_to_char("You can only give scalps to the pointsmaster.",ch);
			return;
		}

		if (ch->cabal != CABAL_ACADIAN)
		{
			send_to_char("You aren't an assassin!\n\r",ch);
			return;
		}

		send_to_char("Your kill has been recorded.\n\r",ch);
		ch->pcdata->bkills ++;
		obj_from_char( obj );
		return;
	}
	else
	{
		send_to_char("You can only give scalps to the pointsmaster.",ch);
		return;
	}
}

	if (isCabalItem(obj)==TRUE)
	{
	if(IS_NPC(victim))
	{
		if(victim->pIndexData->vnum == MOB_VNUM_PROTECTOR
		|| victim->pIndexData->vnum == MOB_VNUM_ACADIAN
		|| victim->pIndexData->vnum == MOB_VNUM_COVEN
		|| victim->pIndexData->vnum == MOB_VNUM_PROTECTOR
		|| victim->pIndexData->vnum == MOB_VNUM_RAVAGER
        || victim->pIndexData->vnum == MOB_VNUM_BRIAR)
			{
  		      cabal_shudder(obj->pIndexData->cabal);
 		      sprintf(buf,"%s has downed the %s.",ch->name,obj->short_descr);
  		      do_cb(victim,buf);
		      obj_from_char( obj );
		      obj_to_char( obj, victim );
			boolCabalItem=TRUE;
			save_cabal_items();
			}
		else
		{
			act("A bolt from the heavens comes down and smites $n!",ch,obj,0,TO_ROOM);
			act("A bolt from the heavens comes down and smites your bitch ass!",ch,obj,0,TO_CHAR);
			raw_kill(victim,ch);
/*			ch->hit = 1;
			damage( victim, ch, 2000, TYPE_HIT, DAM_HOLY, TRUE );
			damage( victim, ch, 2000, TYPE_HIT, DAM_HOLY, TRUE );
			damage( victim, ch, 2000, TYPE_HIT, DAM_HOLY, TRUE );
			damage( victim, ch, 2000, TYPE_HIT, DAM_HOLY, TRUE );
			damage( victim, ch, 2000, TYPE_HIT, DAM_HOLY, TRUE );
			damage( victim, ch, 2000, TYPE_HIT, DAM_HOLY, TRUE );
			damage( victim, ch, 2000, TYPE_HIT, DAM_HOLY, TRUE );
*/			return;
		}
      }
      }
    if(!boolCabalItem) {
    obj_from_char( obj );
    obj_to_char( obj, victim ); }
    MOBtrigger = FALSE;
    act( "$n gives $p to $N.", ch, obj, victim, TO_NOTVICT );
    act( "$n gives you $p.",   ch, obj, victim, TO_VICT    );
    act( "You give $p to $N.", ch, obj, victim, TO_CHAR    );
    /*if (cant_carry(victim,obj))
    {
	act("$n is burnt by $p and drops it.",victim,obj,0,TO_ROOM);
	act("You are burnt by $p and drop it.",victim,obj,0,TO_CHAR);
	obj_from_char(obj);
	obj_to_room(obj,victim->in_room);
    }*/

    MOBtrigger = TRUE;

    /*
     * Give trigger
     */
    if ( IS_NPC(victim) && HAS_TRIGGER( victim, TRIG_GIVE ) )
        mp_give_trigger( victim, ch, obj );

    return;
}

void do_skin( CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *find_corpse;
    OBJ_DATA *corpse;
    OBJ_DATA *sack;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    int number, count;
    AFFECT_DATA af;
    char *name;
    char *last_name;
    char arg1[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];

    corpse = NULL;
    if ( (get_skill(ch,gsn_skin) == 0)
	|| (ch->level < skill_table[gsn_skin].skill_level[ch->class]) )
    {
	send_to_char("But you don't know how to skin a corpse.\n\r",ch);
	return;
    }
    if (is_affected(ch,gsn_skin))
    {
	send_to_char("You do not feel up to fashioning a new skin yet.\n\r",ch);
	return;
    }
    if (ch->mana < 25)
    {
	send_to_char("You don't have the mana.\n\r",ch);
	return;
    }

    one_argument(argument,arg1);
    number = number_argument(argument,arg1);
    count = 0;
    for (find_corpse = ch->carrying; find_corpse != NULL; find_corpse = find_corpse->next_content)
    {
	if (find_corpse->wear_loc == WEAR_NONE && (can_see_obj(ch,find_corpse) )
    	    && ( arg1[0] == '\0' || is_name(arg1,find_corpse->name)) )
       {
	if (++count == number )
	corpse = find_corpse;
       }
    }

    if (corpse == NULL)
    {
	send_to_char("You don't have that.\n\r",ch);
	return;
    }
    if ((corpse->item_type != ITEM_CORPSE_NPC) && (corpse->item_type != ITEM_CORPSE_PC))
    {
 	send_to_char("You can only skin a corpse you are carrying.\n\r",ch);
	return;
    }

   if (number_percent() > ch->pcdata->learned[gsn_skin])
   {
	act("You fail and destroy $p.",ch,corpse,NULL,TO_CHAR);
	act("$n tries to skin a corpse but fails and destroys it.",ch,NULL,NULL,TO_ROOM);
	extract_obj(corpse);
	ch->mana -= 25;
	return;
    }

    act("$n brutally skins $p and fashions $mself a bloody sack.",ch,corpse,NULL,TO_ROOM);
    act("You skin $p and make a new rangers sack!",ch,corpse,NULL,TO_CHAR);

    name = corpse->short_descr;
    for (obj = corpse->contains; obj != NULL; obj = obj_next)
    {
	obj_next = obj->next_content;
	obj_from_obj(obj);
	obj_to_room(obj,ch->in_room);
    }

    sack = create_object(get_obj_index(OBJ_VNUM_SACK),1);
    sack->level = corpse->level;

    last_name = name;
    last_name = one_argument(last_name,name);
    last_name = one_argument(last_name,name);
    last_name = one_argument(last_name,name);
    name = last_name;
    sprintf(buf1, "a sack made from the skin of %s",name);
    sprintf(buf2, "a bloody sack fashioned from the torn skin of %s is here.",name);
    free_string(sack->short_descr);
    free_string(sack->description);
    sack->short_descr = str_dup(buf1);
    sack->description = str_dup(buf2);

    obj_to_char(sack,ch);
    extract_obj(corpse);
    ch->mana -= 25;

        init_affect(&af);
    af.where = TO_AFFECTS;
    af.type = gsn_skin;
        af.aftype    = AFT_SKILL;
    af.modifier = 0;
    af.location = 0;
    af.duration = 24;
    af.level = ch->level;
    af.bitvector = 0;
    affect_to_char(ch,&af);

    return;
}

void do_butcher( CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *find_corpse;
    OBJ_DATA *corpse;
    OBJ_DATA *steak;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    int number, count;
    char *name;
    char *last_name;
    char arg1[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];

    corpse = NULL;
    if ( (get_skill(ch,gsn_butcher) == 0)
	|| (ch->level < skill_table[gsn_butcher].skill_level[ch->class]) )
    {
	send_to_char("But you don't know how to butcher.\n\r",ch);
	return;
    }

    one_argument(argument,arg1);
    number = number_argument(argument,arg1);
    count = 0;
    for (find_corpse = ch->in_room->contents;  find_corpse != NULL; find_corpse = find_corpse->next_content)
    {
	if ((can_see_obj(ch,find_corpse) )
    	    && ( arg1[0] == '\0' || is_name(arg1,find_corpse->name)) )
       {
	if (++count == number )
	corpse = find_corpse;
       }
    }

    if (corpse == NULL)
    {
	send_to_char("You don't have that.\n\r",ch);
	return;
    }
    if ((corpse->item_type != ITEM_CORPSE_NPC) && (corpse->item_type != ITEM_CORPSE_PC))
    {
 	send_to_char("You can only butcher a corpse.\n\r",ch);
	return;
    }

   if (number_percent() > ch->pcdata->learned[gsn_butcher])
   {
	act("You fail and destroy $p.",ch,corpse,NULL,TO_CHAR);
	act("$n tries to butcher a corpse but fails and destroys it.",ch,NULL,NULL,TO_ROOM);
	extract_obj(corpse);
	return;
    }

    act("$n butchers $p.",ch,corpse,0,TO_ROOM);
    act("You butcher $p.",ch,corpse,0,TO_CHAR);

    name = corpse->short_descr;
    for (obj = corpse->contains; obj != NULL; obj = obj_next)
    {
	obj_next = obj->next_content;
	obj_from_obj(obj);
	obj_to_room(obj,ch->in_room);
    }

    steak = create_object(get_obj_index(OBJ_VNUM_STEAK),1);
    steak->level = corpse->level;

    last_name = name;
    last_name = one_argument(last_name,name);
    last_name = one_argument(last_name,name);
    last_name = one_argument(last_name,name);
    name = last_name;
    sprintf(buf1, "a steak of %s",name);
    sprintf(buf2, "a steak of %s is here.",name);
    free_string(steak->short_descr);
    free_string(steak->description);
    steak->short_descr = str_dup(buf1);
    steak->description = str_dup(buf2);

    obj_to_room(steak,ch->in_room);
    extract_obj(corpse);
    return;
}


/* for poisoning weapons and food/drink */
void do_envenom(CHAR_DATA *ch, char *argument)
{
    OBJ_DATA *obj;
    AFFECT_DATA af;
    int percent,skill;

    /* find out what */
    if (argument == '\0')
    {
	send_to_char("Envenom what item?\n\r",ch);
	return;
    }

    obj =  get_obj_list(ch,argument,ch->carrying);

    if (obj== NULL)
    {
	send_to_char("You don't have that item.\n\r",ch);
	return;
    }

    if ((skill = get_skill(ch,gsn_envenom)) < 1)
    {
	send_to_char("Are you crazy? You'd poison yourself!\n\r",ch);
	return;
    }

    if (obj->item_type == ITEM_FOOD || obj->item_type == ITEM_DRINK_CON)
    {
	if (IS_OBJ_STAT(obj,ITEM_BLESS) || IS_OBJ_STAT(obj,ITEM_BURN_PROOF))
	{
	    act("You fail to poison $p.",ch,obj,NULL,TO_CHAR);
	    return;
	}

	if (number_percent() < skill)  /* success! */
	{
	    act("$n treats $p with deadly poison.",ch,obj,NULL,TO_ROOM);
	    act("You treat $p with deadly poison.",ch,obj,NULL,TO_CHAR);
	    if (!obj->value[3])
	    {
		obj->value[3] = 1;
		check_improve(ch,gsn_envenom,TRUE,4);
	    }
	    WAIT_STATE(ch,skill_table[gsn_envenom].beats);
	    return;
	}

	act("You fail to poison $p.",ch,obj,NULL,TO_CHAR);
	if (!obj->value[3])
	    check_improve(ch,gsn_envenom,FALSE,4);
	WAIT_STATE(ch,skill_table[gsn_envenom].beats);
	return;
     }

    if (obj->item_type == ITEM_WEAPON)
    {
        if (IS_WEAPON_STAT(obj,WEAPON_FLAMING)
        ||  IS_WEAPON_STAT(obj,WEAPON_FROST)
        ||  IS_WEAPON_STAT(obj,WEAPON_VAMPIRIC)
        ||  IS_WEAPON_STAT(obj,WEAPON_SHARP)
        ||  IS_WEAPON_STAT(obj,WEAPON_VORPAL)
        ||  IS_WEAPON_STAT(obj,WEAPON_SHOCKING)
        ||  IS_OBJ_STAT(obj,ITEM_BLESS) || IS_OBJ_STAT(obj,ITEM_BURN_PROOF))
        {
            act("You can't seem to envenom $p.",ch,obj,NULL,TO_CHAR);
            return;
        }

	if (obj->value[3] < 0
	||  attack_table[obj->value[3]].damage == DAM_BASH)
	{
	    send_to_char("You can only envenom edged weapons.\n\r",ch);
	    return;
	}

        if (IS_WEAPON_STAT(obj,WEAPON_POISON))
        {
            act("$p is already envenomed.",ch,obj,NULL,TO_CHAR);
            return;
        }

	percent = number_percent();
	if (percent < skill)
	{

        init_affect(&af);
            af.where     = TO_WEAPON;
            af.type      = gsn_poison;
        af.aftype    = AFT_SKILL;
            af.level     = ch->level * percent / 100;
            af.duration  = ch->level/2 * percent / 100;
            af.location  = 0;
            af.modifier  = 0;
            af.bitvector = WEAPON_POISON;
            affect_to_obj(obj,&af);

            act("$n coats $p with deadly venom.",ch,obj,NULL,TO_ROOM);
	    act("You coat $p with venom.",ch,obj,NULL,TO_CHAR);
	    check_improve(ch,gsn_envenom,TRUE,3);
	    WAIT_STATE(ch,skill_table[gsn_envenom].beats);
            return;
        }
	else
	{
	    act("You fail to envenom $p.",ch,obj,NULL,TO_CHAR);
	    check_improve(ch,gsn_envenom,FALSE,3);
	    WAIT_STATE(ch,skill_table[gsn_envenom].beats);
	    return;
	}
    }

    act("You can't poison $p.",ch,obj,NULL,TO_CHAR);
    return;
}

void do_fill( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *fountain;
    bool found;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Fill what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    found = FALSE;
    for ( fountain = ch->in_room->contents; fountain != NULL;
	fountain = fountain->next_content )
    {
	if ( fountain->item_type == ITEM_FOUNTAIN )
	{
	    found = TRUE;
	    break;
	}
    }

    if ( !found )
    {
	send_to_char( "There is no fountain here!\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_DRINK_CON )
    {
	send_to_char( "You can't fill that.\n\r", ch );
	return;
    }

    if ( obj->value[1] != 0 && obj->value[2] != fountain->value[2] )
    {
	send_to_char( "There is already another liquid in it.\n\r", ch );
	return;
    }

    if ( obj->value[1] >= obj->value[0] )
    {
	send_to_char( "Your container is full.\n\r", ch );
	return;
    }

    sprintf(buf,"You fill $p with %s from $P.",
	liq_table[fountain->value[2]].liq_name);
    act( buf, ch, obj,fountain, TO_CHAR );
    sprintf(buf,"$n fills $p with %s from $P.",
	liq_table[fountain->value[2]].liq_name);
    act(buf,ch,obj,fountain,TO_ROOM);
    obj->value[2] = fountain->value[2];
    obj->value[1] = obj->value[0];
    return;
}

void do_pour (CHAR_DATA *ch, char *argument)
{
    char arg[MAX_STRING_LENGTH],buf[MAX_STRING_LENGTH];
    OBJ_DATA *out, *in;
    CHAR_DATA *vch = NULL;
    int amount;

    argument = one_argument(argument,arg);

    if (arg[0] == '\0' || argument[0] == '\0')
    {
	send_to_char("Pour what into what?\n\r",ch);
	return;
    }


    if ((out = get_obj_carry(ch,arg, ch)) == NULL)
    {
	send_to_char("You don't have that item.\n\r",ch);
	return;
    }

    if (out->item_type != ITEM_DRINK_CON)
    {
	send_to_char("That's not a drink container.\n\r",ch);
	return;
    }

    if (!str_cmp(argument,"out"))
    {
	if (out->value[1] == 0)
	{
	    send_to_char("It's already empty.\n\r",ch);
	    return;
	}

	out->value[1] = 0;
	out->value[3] = 0;
	sprintf(buf,"You invert $p, spilling %s all over the ground.",
		liq_table[out->value[2]].liq_name);
	act(buf,ch,out,NULL,TO_CHAR);

	sprintf(buf,"$n inverts $p, spilling %s all over the ground.",
		liq_table[out->value[2]].liq_name);
	act(buf,ch,out,NULL,TO_ROOM);
	return;
    }

    if ((in = get_obj_here(ch,argument)) == NULL)
    {
	vch = get_char_room(ch,argument);

	if (vch == NULL)
	{
	    send_to_char("Pour into what?\n\r",ch);
	    return;
	}

	in = get_eq_char(vch,WEAR_HOLD);

	if (in == NULL)
	{
	    send_to_char("They aren't holding anything.",ch);
 	    return;
	}
    }

    if (in->item_type != ITEM_DRINK_CON)
    {
	send_to_char("You can only pour into other drink containers.\n\r",ch);
	return;
    }

    if (in == out)
    {
	send_to_char("You cannot change the laws of physics!\n\r",ch);
	return;
    }

    if (in->value[1] != 0 && in->value[2] != out->value[2])
    {
	send_to_char("They don't hold the same liquid.\n\r",ch);
	return;
    }

    if (out->value[1] == 0)
    {
	act("There's nothing in $p to pour.",ch,out,NULL,TO_CHAR);
	return;
    }

    if (in->value[1] >= in->value[0])
    {
	act("$p is already filled to the top.",ch,in,NULL,TO_CHAR);
	return;
    }

    amount = UMIN(out->value[1],in->value[0] - in->value[1]);

    in->value[1] += amount;
    out->value[1] -= amount;
    in->value[2] = out->value[2];

    if (vch == NULL)
    {
    	sprintf(buf,"You pour %s from $p into $P.",
	    liq_table[out->value[2]].liq_name);
    	act(buf,ch,out,in,TO_CHAR);
    	sprintf(buf,"$n pours %s from $p into $P.",
	    liq_table[out->value[2]].liq_name);
    	act(buf,ch,out,in,TO_ROOM);
    }
    else
    {
        sprintf(buf,"You pour some %s for $N.",
            liq_table[out->value[2]].liq_name);
        act(buf,ch,NULL,vch,TO_CHAR);
	sprintf(buf,"$n pours you some %s.",
	    liq_table[out->value[2]].liq_name);
	act(buf,ch,NULL,vch,TO_VICT);
        sprintf(buf,"$n pours some %s for $N.",
            liq_table[out->value[2]].liq_name);
        act(buf,ch,NULL,vch,TO_NOTVICT);

    }
}

void do_drink( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int amount;
    int liquid;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
	{
	    if ( obj->item_type == ITEM_FOUNTAIN )
		break;
	}

	if ( obj == NULL )
	{
	    send_to_char( "Drink what?\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
	{
	    send_to_char( "You can't find it.\n\r", ch );
	    return;
	}
    }

    if (ch->fighting != NULL)
    {
	send_to_char("You're too busy fighting to drink anything.\n\r", ch);
	return;
    }

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10 )
    {
	send_to_char( "You fail to reach your mouth.  *Hic*\n\r", ch );
	return;
    }

    switch ( obj->item_type )
    {
    default:
	send_to_char( "You can't drink from that.\n\r", ch );
	return;

    case ITEM_FOUNTAIN:
        if ( ( liquid = obj->value[2] )  < 0 )
        {
	    sprintf(buf,"Do_drink: bad liquid number, %d, on %s.",
	liquid, obj->short_descr);
		bug(buf,0);
/*
            bug( "Do_drink: bad liquid number %d.", liquid );
*/
            liquid = obj->value[2] = 0;
        }
	amount = liq_table[liquid].liq_affect[4] * 3;
	break;

    case ITEM_DRINK_CON:
	if ( obj->value[1] <= 0 )
	{
	    send_to_char( "It is already empty.\n\r", ch );
	    return;
	}

	if ( ( liquid = obj->value[2] )  < 0 )
	{
	    sprintf(buf,"Do_drink: bad liquid number, %d, on %s.",
	liquid, obj->short_descr);
		bug(buf,0);
/*
	    bug( "Do_drink: bad liquid number %d.", liquid );
*/
	    liquid = obj->value[2] = 0;
	}

        amount = liq_table[liquid].liq_affect[4];
        amount = UMIN(amount, obj->value[1]);
	break;
     }
    act( "$n drinks $T from $p.",
	ch, obj, liq_table[liquid].liq_name, TO_ROOM );
    act( "You drink $T from $p.",
	ch, obj, liq_table[liquid].liq_name, TO_CHAR );

    gain_condition( ch, COND_DRUNK,
	amount * liq_table[liquid].liq_affect[COND_DRUNK] / 36 );
    gain_condition( ch, COND_THIRST,
	amount * liq_table[liquid].liq_affect[COND_THIRST] / 3 );
    gain_condition(ch, COND_HUNGER,
	amount * liq_table[liquid].liq_affect[COND_HUNGER] / 3 );

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]  > 10 )
	send_to_char( "You feel drunk.\n\r", ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] > 40 )
	send_to_char( "Your thirst is quenched.\n\r", ch );

    if ( obj->value[3] != 0 )
    {
	/* The drink was poisoned ! */
	AFFECT_DATA af;

	act( "$n chokes and gags.", ch, NULL, NULL, TO_ROOM );
	send_to_char( "You choke and gag.\n\r", ch );
        init_affect(&af);
	af.where     = TO_AFFECTS;
	af.type      = gsn_poison;
        af.aftype    = AFT_SPELL;
	af.level	 = number_fuzzy(amount);
	af.duration  = 3 * amount;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.bitvector = AFF_POISON;
	affect_join( ch, &af );
    }

    if (obj->value[0] > 0)
        obj->value[1] -= amount;

    return;
}

void do_eat( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Eat what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if (ch->fighting != NULL)
    {
	send_to_char( "You're too busy fighting to worry about food.\n\r", ch);
	return;
    }

    if ( !IS_IMMORTAL(ch) )
    {
	if ( obj->item_type != ITEM_FOOD && obj->item_type != ITEM_PILL )
	{
	    send_to_char( "That's not edible.\n\r", ch );
	    return;
	}

	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_HUNGER] > 40 )
	{
	    send_to_char( "You are too full to eat more.\n\r", ch );
	    return;
	}

    }

	act( "$n eats $p.",  ch, obj, NULL, TO_ROOM );
        act( "You eat $p.", ch, obj, NULL, TO_CHAR );

    switch ( obj->item_type )
    {

    case ITEM_FOOD:
	if ( !IS_NPC(ch) )
	{
	    int condition;

	    condition = ch->pcdata->condition[COND_HUNGER];
	    gain_condition( ch, COND_HUNGER, obj->value[1]*3);
	    if ( condition <= 4 && ch->pcdata->condition[COND_HUNGER] > 4 )
		send_to_char( "You are no longer hungry.\n\r", ch );
	    else if ( ch->pcdata->condition[COND_HUNGER] > 40 )
		send_to_char( "You are full.\n\r", ch );
	}

	if ( obj->value[3] != 0 )
	{
	    /* The food was poisoned! */
	    AFFECT_DATA af;

	    act( "$n chokes and gags.", ch, 0, 0, TO_ROOM );
	    send_to_char( "You choke and gag.\n\r", ch );

        init_affect(&af);
	    af.where	 = TO_AFFECTS;
	    af.type      = gsn_poison;
        af.aftype    = AFT_SPELL;
	    af.level 	 = number_fuzzy(obj->value[0]);
	    af.duration  = 2 * obj->value[0];
	    af.location  = APPLY_NONE;
	    af.modifier  = 0;
	    af.bitvector = AFF_POISON;
	    affect_join( ch, &af );
	}
	break;

    case ITEM_PILL:
	obj_cast_spell( obj->value[1], obj->value[0], ch, ch, NULL );
	obj_cast_spell( obj->value[2], obj->value[0], ch, ch, NULL );
	obj_cast_spell( obj->value[3], obj->value[0], ch, ch, NULL );
	break;
    }

    extract_obj( obj );
    return;
}



/*
 * Remove an object.
 */
bool remove_obj( CHAR_DATA *ch, int iWear, bool fReplace )
{
    OBJ_DATA *obj;
    OBJ_DATA *secondary;
/* There's a pile of dual wield and trinal wield crap in here all over the
 * place...Very messy, but i was coding it in a rush...Ceran.
*/
    if ( ( obj = get_eq_char( ch, iWear ) ) == NULL )
	return TRUE;

    if ( !fReplace )
	return FALSE;

    if ( IS_SET(obj->extra_flags, ITEM_NOREMOVE) )
    {
	if (IS_IMMORTAL(ch)) {
	act( "You can't remove $p.", ch, obj, NULL, TO_CHAR );
	} else {
	act( "You can't remove $p.", ch, obj, NULL, TO_CHAR );
	return FALSE;
	}
    }

    unequip_char( ch, obj );
    secondary = get_eq_char(ch,WEAR_DUAL_WIELD);

    if (iWear == WEAR_WIELD && secondary != NULL)
	{
	unequip_char(ch,secondary);
	equip_char(ch,secondary,WEAR_WIELD);
	}
    if (iWear == WEAR_DUAL_WIELD)
	{
	act("$n stops dual wielding $p.",ch,obj,0,TO_ROOM);
	act("You stop dual wielding $p.",ch,obj,0,TO_CHAR);
	return TRUE;
	}

    act( "$n stops using $p.", ch, obj, NULL, TO_ROOM );
    act( "You stop using $p.", ch, obj, NULL, TO_CHAR );

    return TRUE;
}



/*
 * Wear one object.
 * Optional replacement of existing objects.
 * Big repetitive code, ick.
 */
void wear_obj( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace )
{
    OBJ_DATA *secondary;
    OBJ_DATA *obj_check;
    bool wield_primary;
    OBJ_DATA *primary;
    OBJ_DATA *weapon;
    OBJ_DATA *oldobj;
    int sn, skill;
    bool status = FALSE;

    if ( CAN_WEAR( obj, ITEM_WEAR_FINGER ) )
    {
	if ( get_eq_char( ch, WEAR_FINGER_L ) != NULL
	&&   get_eq_char( ch, WEAR_FINGER_R ) != NULL
	&&   !remove_obj( ch, WEAR_FINGER_L, fReplace )
	&&   !remove_obj( ch, WEAR_FINGER_R, fReplace ) )
	    return;

	if ( get_eq_char( ch, WEAR_FINGER_L ) == NULL )
	{
	    act( "$n wears $p on $s left finger.",    ch, obj, NULL, TO_ROOM );
	    act( "You wear $p on your left finger.",  ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_FINGER_L );
	    return;
	}

	if ( get_eq_char( ch, WEAR_FINGER_R ) == NULL )
	{
	    act( "$n wears $p on $s right finger.",   ch, obj, NULL, TO_ROOM );
	    act( "You wear $p on your right finger.", ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_FINGER_R );
	    return;
	}

	bug( "Wear_obj: no free finger.", 0 );
	send_to_char( "You already wear two rings.\n\r", ch );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_NECK ) )
    {
	if ( get_eq_char( ch, WEAR_NECK_1 ) != NULL
	&&   get_eq_char( ch, WEAR_NECK_2 ) != NULL
	&&   !remove_obj( ch, WEAR_NECK_1, fReplace )
	&&   !remove_obj( ch, WEAR_NECK_2, fReplace ) )
	    return;

    if (obj->pIndexData->vnum == OBJ_VNUM_BADGE)
    {
        obj_check = get_eq_char(ch,WEAR_NECK_1);
        if (obj_check != NULL && obj_check->pIndexData->vnum == OBJ_VNUM_BADGE)
                status = TRUE;
        obj_check = get_eq_char(ch,WEAR_NECK_2);
        if (obj_check != NULL && obj_check->pIndexData->vnum == OBJ_VNUM_BADGE)
                status = TRUE;

	if (status)
	{
        send_to_char("You are only able to wear one such item.\n\r",ch);
	return;
	}
	send_to_char("You feel at odds with the law!\n\r",ch);
    }

	if ( get_eq_char( ch, WEAR_NECK_1 ) == NULL )
	{
	    act( "$n wears $p around $s neck.",   ch, obj, NULL, TO_ROOM );
	    act( "You wear $p around your neck.", ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_NECK_1 );
	    return;
	}

	if ( get_eq_char( ch, WEAR_NECK_2 ) == NULL )
	{
	    act( "$n wears $p around $s neck.",   ch, obj, NULL, TO_ROOM );
	    act( "You wear $p around your neck.", ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_NECK_2 );
	    return;
	}

	bug( "Wear_obj: no free neck.", 0 );
	send_to_char( "You already wear two neck items.\n\r", ch );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_BODY ) )
    {
	if ( !remove_obj( ch, WEAR_BODY, fReplace ) )
	    return;
	act( "$n wears $p on $s torso.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p on your torso.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_BODY );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_HEAD ) )
    {
	oldobj =(get_eq_char(ch, WEAR_HEAD));
	if ( !remove_obj( ch, WEAR_HEAD, fReplace ) )
	    return;

	act( "$n wears $p on $s head.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p on your head.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_HEAD );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_LEGS ) )
    {
	if ( !remove_obj( ch, WEAR_LEGS, fReplace ) )
	    return;
	act( "$n wears $p on $s legs.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p on your legs.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_LEGS );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_FEET ) )
    {
	if ( !remove_obj( ch, WEAR_FEET, fReplace ) )
	    return;
	act( "$n wears $p on $s feet.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p on your feet.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_FEET );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_HANDS ) )
    {
	if ( !remove_obj( ch, WEAR_HANDS, fReplace ) )
	    return;
	act( "$n wears $p on $s hands.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p on your hands.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_HANDS );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_ARMS ) )
    {
	if ( !remove_obj( ch, WEAR_ARMS, fReplace ) )
	    return;
	act( "$n wears $p on $s arms.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p on your arms.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_ARMS );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_ABOUT ) )
    {
	if ( !remove_obj( ch, WEAR_ABOUT, fReplace ) )
	    return;
	act( "$n wears $p about $s body.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p about your body.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_ABOUT );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_WAIST ) )
    {
	if ( !remove_obj( ch, WEAR_WAIST, fReplace ) )
	    return;
	act( "$n wears $p about $s waist.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p about your waist.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_WAIST );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_WRIST ) )
    {
	if ( get_eq_char( ch, WEAR_WRIST_L ) != NULL
	&&   get_eq_char( ch, WEAR_WRIST_R ) != NULL
	&&   !remove_obj( ch, WEAR_WRIST_L, fReplace )
	&&   !remove_obj( ch, WEAR_WRIST_R, fReplace ) )
	    return;

	if ( get_eq_char( ch, WEAR_WRIST_L ) == NULL )
	{
	    act( "$n wears $p around $s left wrist.",
		ch, obj, NULL, TO_ROOM );
	    act( "You wear $p around your left wrist.",
		ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_WRIST_L );
	    return;
	}

	if ( get_eq_char( ch, WEAR_WRIST_R ) == NULL )
	{
	    act( "$n wears $p around $s right wrist.",
		ch, obj, NULL, TO_ROOM );
	    act( "You wear $p around your right wrist.",
		ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_WRIST_R );
	    return;
	}

	bug( "Wear_obj: no free wrist.", 0 );
	send_to_char( "You already wear two wrist items.\n\r", ch );
	return;
    }

/* ====== DUAL WIELD STUFF STARTS HERE ======= */
/* Dual wielding and limiting to 2 hands for objects */


    if (CAN_WEAR(obj,ITEM_HOLD))
    {
	if ((get_eq_char(ch,WEAR_HOLD) == NULL) && (hands_full(ch)) )
	{
		if (!fReplace)
			return;
		else
		{
		 send_to_char("Your hands are full.\n\r",ch);
		 return;
 		}
	}
    if (!remove_obj(ch,WEAR_HOLD,fReplace))
	return;
    weapon = get_eq_char(ch,WEAR_WIELD);
    if (weapon != NULL && ch->size < SIZE_LARGE
	&& IS_WEAPON_STAT(weapon,WEAPON_TWO_HANDS))
    {
	send_to_char("Your hands are tied up with your weapon.\n\r",ch);
	return;
    }

    act("$n holds $p in $s hands.",ch,obj,0,TO_ROOM);
    act("You hold $p in your hands.",ch,obj,0,TO_CHAR);
    equip_char(ch,obj,WEAR_HOLD);
    return;
    }

    if (CAN_WEAR(obj,ITEM_WEAR_SHIELD))
    {
	if ((get_eq_char(ch,WEAR_SHIELD) == NULL) && (hands_full(ch)) )
	{
		if (!fReplace)
			return;
		else
		{
		 send_to_char("Your hands are full.\n\r",ch);
		 return;
 		}
	}
    if (!remove_obj(ch,WEAR_SHIELD,fReplace))
	return;
    weapon = get_eq_char(ch,WEAR_WIELD);
    if (weapon != NULL && ch->size < SIZE_LARGE
	&& IS_WEAPON_STAT(weapon,WEAPON_TWO_HANDS))
    {
	send_to_char("Your hands are tied up with your weapon.\n\r",ch);
	return;
    }
    act("$n wears $p as a shield.",ch,obj,0,TO_ROOM);
    act("You wear $p as a shield.",ch,obj,0,TO_CHAR);
    equip_char(ch,obj,WEAR_SHIELD);
    return;
    }

/* Lots of dual wield stuff now */
    if (obj->item_type == ITEM_LIGHT)
    {
	if ((get_eq_char(ch,WEAR_LIGHT) == NULL) && (hands_full(ch)) )
	{
		if (!fReplace)
			return;
		else
		{
		 send_to_char("Your hands are full.\n\r",ch);
		 return;
 		}
	}
    if (!remove_obj(ch,WEAR_LIGHT,fReplace))
	return;
    weapon = get_eq_char(ch,WEAR_WIELD);
    if (weapon != NULL && ch->size < SIZE_LARGE
	&& IS_WEAPON_STAT(weapon,WEAPON_TWO_HANDS))
    {
	send_to_char("Your hands are tied up with your weapon.\n\r",ch);
	return;
    }
    act("$n lights $p and holds it.",ch,obj,0,TO_ROOM);
    act("You light $p and hold it.",ch,obj,0,TO_CHAR);
    equip_char(ch,obj,WEAR_LIGHT);
    return;
    }


    if (CAN_WEAR(obj,ITEM_WIELD))
    {
 	if (can_wield_tertiary(ch,obj,fReplace))
	return;

	wield_primary = TRUE;
	primary = get_eq_char(ch,WEAR_WIELD);
	secondary = get_eq_char(ch,WEAR_DUAL_WIELD);

	if (primary == NULL && (hands_full(ch)) )
	{
		if (!fReplace)
			return;
		else
		{
		 send_to_char("Your hands are full.\n\r",ch);
		 return;
		}
	}
	if (get_eq_char(ch, WEAR_DUAL_WIELD) != NULL)
		wield_primary = FALSE;

	if (primary != NULL && (!hands_full(ch)) )
		wield_primary = FALSE;
	if (get_skill(ch,gsn_dual_wield) < 3 && (!IS_NPC(ch)))
		wield_primary = TRUE;
	if (ch->level < skill_table[gsn_dual_wield].skill_level[ch->class]
	&& !IS_NPC(ch))
		wield_primary = TRUE;
	if (ch->size < SIZE_LARGE && IS_WEAPON_STAT(obj,WEAPON_TWO_HANDS))
		wield_primary = TRUE;
	if (primary != NULL && ch->size < SIZE_LARGE
	&& IS_WEAPON_STAT(obj,WEAPON_TWO_HANDS))
		wield_primary = TRUE;
	if (primary != NULL && ch->size < SIZE_LARGE
	&& IS_WEAPON_STAT(primary,WEAPON_TWO_HANDS))
		wield_primary = TRUE;
/* First attempt to dual wield if !primary. */

    if (!wield_primary)
    {
	if (secondary == NULL)
	 {
	   if (obj->weight < primary->weight * 75/100
	    || obj->weight < 50)
	   {
		act("$n dual wields $p.",ch,obj,0,TO_ROOM);
		act("You dual wield $p.",ch,obj,0,TO_CHAR);
		equip_char(ch,obj,WEAR_DUAL_WIELD);

	switch(obj->value[0])
	{
        default :               sn = -1;                break;
        case(WEAPON_SWORD):     sn = gsn_sword;         break;
        case(WEAPON_DAGGER):    sn = gsn_dagger;        break;
        case(WEAPON_SPEAR):     sn = gsn_spear;         break;
        case(WEAPON_MACE):      sn = gsn_mace;          break;
        case(WEAPON_AXE):       sn = gsn_axe;           break;
        case(WEAPON_FLAIL):     sn = gsn_flail;         break;
        case(WEAPON_WHIP):      sn = gsn_whip;          break;
        case(WEAPON_POLEARM):   sn = gsn_polearm;       break;
	case(WEAPON_STAFF):	sn = gsn_staff;	break;
	}

	skill = get_weapon_skill(ch,sn);
	if (skill >= 100)
		act("$p feels like a part of you!",ch,obj,0,TO_CHAR);
	else if (skill > 85)
		act("You feel quite confident with $p.",ch,obj,0,TO_CHAR);
	else if (skill > 70)
		act("You are skilled with $p.",ch,obj,0,TO_CHAR);
	else if (skill > 50)
		act("Your skill with $p is adequate.",ch,obj,0,TO_CHAR);
	else if (skill > 25)
		act("$p feels a little clumsy in your hands.",ch,obj,0,TO_CHAR);
	else if (skill > 1)
		act("You fumble and almost drop $p.",ch,obj,0,TO_CHAR);
	else
		act("You don't even know which end is up on $p.",ch,obj,0,TO_CHAR);

		return;
	   }
	  }
	else if (obj->weight < secondary->weight)
	{
	   if (!remove_obj(ch,WEAR_DUAL_WIELD,fReplace))
		return;
	    act("$n dual wields $p.",ch,obj,0,TO_ROOM);
	    act("You dual wield $p.",ch,obj,0,TO_CHAR);
	    equip_char(ch,obj,WEAR_DUAL_WIELD);

	switch(obj->value[0])
	{
        default :               sn = -1;                break;
        case(WEAPON_SWORD):     sn = gsn_sword;         break;
        case(WEAPON_DAGGER):    sn = gsn_dagger;        break;
        case(WEAPON_SPEAR):     sn = gsn_spear;         break;
        case(WEAPON_MACE):      sn = gsn_mace;          break;
        case(WEAPON_AXE):       sn = gsn_axe;           break;
        case(WEAPON_FLAIL):     sn = gsn_flail;         break;
        case(WEAPON_WHIP):      sn = gsn_whip;          break;
        case(WEAPON_POLEARM):   sn = gsn_polearm;       break;
	case(WEAPON_STAFF):	sn = gsn_staff;		break;
	}

	skill = get_weapon_skill(ch,sn);
	if (skill >= 100)
		act("$p feels like a part of you!",ch,obj,0,TO_CHAR);
	else if (skill > 85)
		act("You feel quite confident with $p.",ch,obj,0,TO_CHAR);
	else if (skill > 70)
		act("You are skilled with $p.",ch,obj,0,TO_CHAR);
	else if (skill > 50)
		act("Your skill with $p is adequate.",ch,obj,0,TO_CHAR);
	else if (skill > 25)
		act("$p feels a little clumsy in your hands.",ch,obj,0,TO_CHAR);
	else if (skill > 1)
		act("You fumble and almost drop $p.",ch,obj,0,TO_CHAR);
	else
		act("You don't even know which end is up on $p.",ch,obj,0,TO_CHAR);

	    return;
	}
	else
	{
	  if (!remove_obj(ch,WEAR_WIELD,fReplace))
		return;
	  if (secondary->weight < obj->weight*75/100
	    || secondary->weight < 30)
	  {
  	    if (get_obj_weight(obj) > (str_app[get_curr_stat(ch,STAT_STR)].wield * 10))
		 {
		    send_to_char("It is too heavy for you to wield.\n\r",ch);
		    return;
		 }
		 act("$n wields $p.",ch,obj,0,TO_ROOM);
		 act("You wield $p.",ch,obj,0,TO_CHAR);
		 unequip_char(ch,secondary);
		 equip_char(ch,obj,WEAR_WIELD);
		 equip_char(ch,secondary,WEAR_DUAL_WIELD);

	sn = get_weapon_sn(ch);
	if (sn == gsn_hand_to_hand)
		return;
	skill = get_weapon_skill(ch,sn);
	if (skill >= 100)
		act("$p feels like a part of you!",ch,obj,0,TO_CHAR);
	else if (skill > 85)
		act("You feel quite confident with $p.",ch,obj,0,TO_CHAR);
	else if (skill > 70)
		act("You are skilled with $p.",ch,obj,0,TO_CHAR);
	else if (skill > 50)
		act("Your skill with $p is adequate.",ch,obj,0,TO_CHAR);
	else if (skill > 25)
		act("$p feels a little clumsy in your hands.",ch,obj,0,TO_CHAR);
	else if (skill > 1)
		act("You fumble and almost drop $p.",ch,obj,0,TO_CHAR);
	else
		act("You don't even know which end is up on $p.",ch,obj,0,TO_CHAR);

	 	 return;
	  }
		act("$p is too heavy to dual wield,",ch,obj,0,TO_CHAR);
		return;
       }
     }

/* If you can't wield the dual weapon then see if you can replace primary */
	 if (!remove_obj(ch,WEAR_WIELD,fReplace))
		return;
	 if (get_obj_weight(obj) > (str_app[get_curr_stat(ch,STAT_STR)].wield * 10 ))
	 {
	    send_to_char("It is too heavy for you to wield.\n\r",ch);
	    return;
	 }

	if (ch->size < SIZE_LARGE && IS_WEAPON_STAT(obj,WEAPON_TWO_HANDS))
	{
	   if (get_eq_char(ch,WEAR_SHIELD) != NULL
	   || get_eq_char(ch,WEAR_HOLD) != NULL
	   || get_eq_char(ch,WEAR_LIGHT) != NULL
	   || get_eq_char(ch,WEAR_WIELD) != NULL)
	   {
	    send_to_char("You need both hands free for that weapon.\n\r",ch);
	    return;
	   }
	}
	 act("$n wields $p.",ch,obj,0,TO_ROOM);
	 act("You wield $p.",ch,obj,0,TO_CHAR);
	 equip_char(ch,obj,WEAR_WIELD);

	sn = get_weapon_sn(ch);
	if (sn == gsn_hand_to_hand)
		return;
	skill = get_weapon_skill(ch,sn);
	if (skill >= 100)
		act("$p feels like a part of you!",ch,obj,0,TO_CHAR);
	else if (skill > 85)
		act("You feel quite confident with $p.",ch,obj,0,TO_CHAR);
	else if (skill > 70)
		act("You are skilled with $p.",ch,obj,0,TO_CHAR);
	else if (skill > 50)
		act("Your skill with $p is adequate.",ch,obj,0,TO_CHAR);
	else if (skill > 25)
		act("$p feels a little clumsy in your hands.",ch,obj,0,TO_CHAR);
	else if (skill > 1)
		act("You fumble and almost drop $p.",ch,obj,0,TO_CHAR);
	else
		act("You don't even know which end is up on $p.",ch,obj,0,TO_CHAR);

 	 return;
}
/* ==== END DUAL_WIELD CODE ======= */
    if ( CAN_WEAR(obj,ITEM_WEAR_BRAND) )
    {
	if (!remove_obj(ch,WEAR_BRAND, fReplace) )
	    return;
	act("You wear it.",ch,obj,NULL,TO_CHAR);
	equip_char(ch,obj,WEAR_BRAND);
	return;
    }

    if ( CAN_WEAR(obj,ITEM_WEAR_BOUND) )
	{
		if (!remove_obj(ch,WEAR_BOUND, fReplace) )
	        return;
	    act( "$n wears $p in servitude to their god.",   ch, obj, NULL, TO_ROOM );
	    act( "You wear $p in servitude to your god.", ch, obj, NULL, TO_CHAR );
		equip_char(ch,obj,WEAR_BOUND);
	        return;
    }

    if ( fReplace )
	send_to_char( "You can't wear, wield, or hold that.\n\r", ch );

    return;
}



void do_wear( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Wear, wield, or hold what?\n\r", ch );
	return;
    }

    if ( IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM))
    {
	send_to_char( "A mob wearing eq? Ha!\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	OBJ_DATA *obj_next;

	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) )
	{
		wear_obj( ch, obj, FALSE );
	if (obj->wear_loc != WEAR_NONE)
	{
    	if (obj->pIndexData->vnum == 14739)
	wear_obj_symbol_magic(ch,obj);

        if (obj->pIndexData->vnum == OBJ_VNUM_WIZARDRY_1
        || obj->pIndexData->vnum == OBJ_VNUM_WIZARDRY_2)
                wear_obj_wizardry(ch,obj);
        if (obj->pIndexData->vnum == OBJ_VNUM_TIARA)
                wear_obj_tiara(ch,obj);
    if (obj->pIndexData->vnum == 5115) /* strange bracers */
    wear_obj_bracers(ch,obj);
    if (obj->pIndexData->vnum == 14005) /* black zhentil robes */
	wear_obj_zhentil_robe(ch,obj);
    if (obj->pIndexData->vnum == 29711) /* gauntlets of striking */
	wear_obj_gauntlets_striking(ch,obj);
    if (obj->pIndexData->vnum == 13711)
	wear_obj_fallen_wings(ch,obj);
    if (obj->pIndexData->vnum == 14728)	/* black dragon hide */
	wear_obj_black_hide(ch,obj);
    if (obj->pIndexData->vnum == 29805) /* misty cloak */
	wear_obj_misty_cloak(ch,obj);
    if (obj->pIndexData->vnum == 29749)	/* cloak of underworld */
	wear_obj_cloak_underworld(ch,obj);
    if (obj->pIndexData->vnum == 29788)	/* red dragonstar */
	wear_obj_red_dragonstar(ch,obj);
    if (obj->pIndexData->vnum == 29789)	/* black dragonstar */
	wear_obj_black_dragonstar(ch,obj);
	}
	}
	}
	return;
    }
    else
    {
	if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}

	wear_obj( ch, obj, TRUE );
    if (obj->wear_loc != WEAR_NONE)
    {
   	if (obj->pIndexData->vnum == 14739)
	wear_obj_symbol_magic(ch,obj);

        if (obj->pIndexData->vnum == OBJ_VNUM_WIZARDRY_1
        || obj->pIndexData->vnum == OBJ_VNUM_WIZARDRY_2)
                wear_obj_wizardry(ch,obj);

        if (obj->pIndexData->vnum == OBJ_VNUM_TIARA)
                wear_obj_tiara(ch,obj);
    if (obj->pIndexData->vnum == 5115) /* strange bracers */
    wear_obj_bracers(ch,obj);
        if (obj->pIndexData->vnum == 29788)	/* red dragonstar */
	wear_obj_red_dragonstar(ch,obj);
    if (obj->pIndexData->vnum == 29789)	/* black dragonstar */
	wear_obj_black_dragonstar(ch,obj);
    if (obj->pIndexData->vnum == 14005) /* black zhentil robes */
	wear_obj_zhentil_robe(ch,obj);
    if (obj->pIndexData->vnum == 29711) /* gauntlets of striking */
	wear_obj_gauntlets_striking(ch,obj);
    if (obj->pIndexData->vnum == 13711)
	wear_obj_fallen_wings(ch,obj);
    if (obj->pIndexData->vnum == 14728)	/* black dragon hide */
	wear_obj_black_hide(ch,obj);
    if (obj->pIndexData->vnum == 29805) /* misty cloak */
	wear_obj_misty_cloak(ch,obj);
    if (obj->pIndexData->vnum == 29749)	/* cloak of underworld */
	wear_obj_cloak_underworld(ch,obj);
    }
    }

    return;
}



void do_remove( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Remove what?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	OBJ_DATA *obj_next;

	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
            if ( obj->wear_loc != WEAR_NONE
              && can_see_obj( ch, obj )
              && obj->wear_loc != WEAR_BOUND
              && obj->wear_loc != WEAR_BRAND )
                remove_obj( ch, obj->wear_loc, TRUE );
if (obj->wear_loc == WEAR_NONE)
	{
    if (obj->pIndexData->vnum == 24418)
        remove_obj_girdle(ch,obj);
	if (obj->pIndexData->vnum == 14739)
	remove_obj_symbol_magic(ch,obj);
        if (obj->pIndexData->vnum == OBJ_VNUM_WIZARDRY_1
        || obj->pIndexData->vnum == OBJ_VNUM_WIZARDRY_2)
                remove_obj_wizardry(ch,obj);
        if (obj->pIndexData->vnum == OBJ_VNUM_TIARA)
                remove_obj_tiara(ch,obj);
     if (obj->pIndexData->vnum == 5115)
        remove_obj_bracers(ch,obj);
    if (obj->pIndexData->vnum == 29711) /* gauntlets of striking */
	remove_obj_gauntlets_striking(ch,obj);
    if (obj->pIndexData->vnum == OBJ_VNUM_HEAVENLY_SCEPTRE)
        remove_obj_sceptre(ch,obj);
    if (obj->pIndexData->vnum == 13711)
	remove_obj_fallen_wings(ch,obj);
    if (obj->pIndexData->vnum == 14728)	/* black dragon hide */
	remove_obj_black_hide(ch,obj);
    if (obj->pIndexData->vnum == 29805) /* misty cloak */
	remove_obj_misty_cloak(ch,obj);
    if (obj->pIndexData->vnum == 29749)	/* cloak of underworld */
	remove_obj_cloak_underworld(ch,obj);
    if (obj->pIndexData->vnum == 29788)	/* red dragonstar */
	remove_obj_red_dragonstar(ch,obj);
    if (obj->pIndexData->vnum == 29789)	/* black dragonstar */
	remove_obj_black_dragonstar(ch,obj);
	}
	}
	reslot_weapon(ch);
	return;
    }
    else if ( ( obj = get_obj_wear( ch, arg ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if ( ( obj->wear_loc == WEAR_BOUND )
    && (!IS_IMMORTAL(ch)) )
    {
		send_to_char( "You can't remove that.\n\r", ch );
		return;
	}

    if (obj->wear_loc == WEAR_BRAND && !IS_IMMORTAL(ch))
    {
	send_to_char( "You can't remove a tattoo!\n\r", ch );
	return;
    }
/*    if (obj->wear_loc == WEAR_STRAPPED)
    {
        send_to_char( "You can't remove that!\n\r", ch );
        return;
    }
*/
    remove_obj( ch, obj->wear_loc, TRUE );
	reslot_weapon(ch);
if (obj->wear_loc == WEAR_NONE)
    {
    if (obj->pIndexData->vnum == 24418)
        remove_obj_girdle(ch,obj);
	if (obj->pIndexData->vnum == 14739)
	remove_obj_symbol_magic(ch,obj);
     if (obj->pIndexData->vnum == 5115)
        remove_obj_bracers(ch,obj);
    if (obj->pIndexData->vnum == 29711) /* gauntlets of striking */
	remove_obj_gauntlets_striking(ch,obj);
        if (obj->pIndexData->vnum == OBJ_VNUM_WIZARDRY_1
        || obj->pIndexData->vnum == OBJ_VNUM_WIZARDRY_2)
                remove_obj_wizardry(ch,obj);
        if (obj->pIndexData->vnum == OBJ_VNUM_TIARA)
                remove_obj_tiara(ch,obj);
	if (obj->pIndexData->vnum == OBJ_VNUM_HEAVENLY_SCEPTRE)
	        remove_obj_sceptre(ch,obj);
	if (obj->pIndexData->vnum == 13711)
	remove_obj_fallen_wings(ch,obj);
    	if (obj->pIndexData->vnum == 14728)	/* black dragon hide */
	remove_obj_black_hide(ch,obj);
    	if (obj->pIndexData->vnum == 29805) /* misty cloak */
	remove_obj_misty_cloak(ch,obj);
    	if (obj->pIndexData->vnum == 29749)	/* cloak of underworld */
	remove_obj_cloak_underworld(ch,obj);
    	if (obj->pIndexData->vnum == 29788)	/* red dragonstar */
	remove_obj_red_dragonstar(ch,obj);
    	if (obj->pIndexData->vnum == 29789)	/* black dragonstar */
	wear_obj_black_dragonstar(ch,obj);
    }
    return;
}



void do_sacrifice( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *ibj;
    OBJ_DATA *ibj_next;
    int silver;

    /* variables for AUTOSPLIT */
    CHAR_DATA *gch;
    int members;
    char buffer[100];


    one_argument( argument, arg );

	if (!IS_NPC(ch))
	{
	if (ch->pcdata->death_status == HAS_DIED)
	{
	send_to_char("You are a hovering spirit, you can't sacrifice anything!\n\r",ch);
	return;
	}
	}

    if ( arg[0] == '\0' || !str_cmp( arg, ch->name ) )
    {
        act( "$n offers $mself to the gods, who graciously decline.",
	    ch, NULL, NULL, TO_ROOM );
	send_to_char(
            "The gods appreciate your offer and may accept it later.\n\r", ch );
	return;
    }

    obj = get_obj_list( ch, arg, ch->in_room->contents );
    if ( obj == NULL )
    {
	send_to_char( "You can't find it.\n\r", ch );
	return;
    }

    if ( obj->item_type == ITEM_CORPSE_PC )
    {
	   send_to_char(
             "The gods wouldn't like that.\n\r",ch);
	   return;
    }


    if ( !CAN_WEAR(obj, ITEM_TAKE) || CAN_WEAR(obj, ITEM_NO_SAC))
    {
	act( "$p is not an acceptable sacrifice.", ch, obj, 0, TO_CHAR );
	return;
    }

    if (obj->in_room != NULL)
    {
	for (gch = obj->in_room->people; gch != NULL; gch = gch->next_in_room)
	    if (gch->on == obj)
	    {
		act("$N appears to be using $p.",
		    ch,obj,gch,TO_CHAR);
		return;
	    }
    }

    silver = UMAX(1,obj->level * 3);
    if (isCabalItem(obj)==TRUE)
	{
		act("A bolt from the heavens comes down and smites $n!",ch,obj,0,TO_ROOM);
		act("A bolt from the heavens comes down and smites your bitch ass!",ch,obj,0,TO_CHAR);
		raw_kill(ch,ch);
		return;
	}
    if (obj->item_type != ITEM_CORPSE_NPC && obj->item_type != ITEM_CORPSE_PC)
    	silver = UMIN(silver,obj->cost);

    if (obj->pIndexData->vnum == OBJ_VNUM_BLOOD_BRACELET)
	if (sacrifice_obj_bracelet(ch,obj))
	return;

    if (obj->item_type == ITEM_CORPSE_NPC)
    {
        if (obj->contains)
        {
            for (ibj = obj->contains ; ibj != NULL ; ibj = ibj_next)
            {
                ibj_next = ibj->next_content;
                obj_from_obj(ibj);
                obj_to_room(ibj, obj->in_room);
            }
        }
    }

    if (silver == 1)
        send_to_char(
            "The gods give you one silver coin for your sacrifice.\n\r", ch );
    else
    {
        sprintf(buf,"The gods give you %d silver coins for your sacrifice.\n\r",
		silver);
	send_to_char(buf,ch);
    }

    ch->silver += silver;

    if (IS_SET(ch->act,PLR_AUTOSPLIT) )
    { /* AUTOSPLIT code */
    	members = 0;
	for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    	{
    	    if ( is_same_group( gch, ch ) )
            members++;
    	}

	if ( members > 1 && silver > 1)
	{
	    sprintf(buffer,"%d",silver);
	    do_split(ch,buffer);
	}
    }

    act( "$n sacrifices $p to the gods.", ch, obj, NULL, TO_ROOM );
    wiznet("$N sends up $p as a burnt offering.",
	   ch,obj,WIZ_SACCING,0,0);
    extract_obj( obj );
    return;
}



void do_quaff( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );
    if (ch->cabal == CABAL_RAVAGER && !IS_IMMORTAL(ch))
    {
	send_to_char("You are a Ravager, you don't use such things as potions!\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Quaff what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
    {
	send_to_char( "You do not have that potion.\n\r", ch );
	return;
    }

    if ( obj->item_type != ITEM_POTION )
    {
	send_to_char( "You can quaff only potions.\n\r", ch );
	return;
    }

    if ( ch->fighting != NULL )
    {
	send_to_char( "You're too busy fighting to quaff.\n\r", ch );
	return;
    }

    /*if ( obj->pIndexData->vnum == 2920 && ch->cabal != CABAL_COVEN )
	{
	 send_to_char( "You gag and choke as the mystical energies swirl around you in ways you cannot comprehend.\n\r", ch );
	 sn = skill_lookup("poison");
	 spell_poison(sn, 70, ch, ch, NULL);
  	 return;
	}
    if ( obj->pIndexData->vnum == 5803 && ch->cabal != CABAL_COVEN )
	{
	 send_to_char( "You gag and choke as the mystical energies swirl around you in ways you cannot comprehend.\n\r", ch );
	 sn = skill_lookup("poison");
	 spell_poison(sn, 70, ch, ch, NULL);
	 sn = skill_lookup("plague");
	 spell_plague(sn, 70, ch, ch, NULL);
  	 return;
	}
     */
    act( "$n quaffs $p.", ch, obj, NULL, TO_ROOM );
    act( "You quaff $p.", ch, obj, NULL ,TO_CHAR );
    WAIT_STATE(ch,PULSE_VIOLENCE);


    obj_cast_spell( obj->value[1], obj->value[0], ch, ch, NULL );
    obj_cast_spell( obj->value[2], obj->value[0], ch, ch, NULL );
    obj_cast_spell( obj->value[3], obj->value[0], ch, ch, NULL );

    extract_obj( obj );
    return;
}



void do_recite( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *scroll;
    OBJ_DATA *obj;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (ch->cabal == CABAL_RAVAGER && !IS_IMMORTAL(ch))
    {
	send_to_char("You are a Ravager, you don't use such things as scrolls!\n\r",ch);
	return;
    }

    if ( ( scroll = get_obj_carry( ch, arg1, ch ) ) == NULL )
    {
	send_to_char( "You do not have that scroll.\n\r", ch );
	return;
    }

    if ( scroll->item_type != ITEM_SCROLL )
    {
	send_to_char( "You can recite only scrolls.\n\r", ch );
	return;
    }

    if ( ch->level < scroll->level)
    {
	send_to_char(
		"This scroll is too complex for you to comprehend.\n\r",ch);
	return;
    }

    if (ch->fighting != NULL)
    {
	send_to_char( "You're too busy fighting to recite.\n\r", ch);
	return;
    }
/*
    if (obj->pIndexData->vnum == 29739
	&& ch->class != CLASS_NECROMANCER)
    {
	send_to_char("You can't recite this scroll of dark power.\n\r",ch);
	return;
    }
*/
    obj = NULL;
    if ( arg2[0] == '\0' )
    {
	victim = ch;
    }
    else
    {
	if ( ( victim = get_char_room ( ch, arg2 ) ) == NULL
	&&   ( obj    = get_obj_here  ( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You can't find it.\n\r", ch );
	    return;
	}
    }

    act( "$n recites $p.", ch, scroll, NULL, TO_ROOM );
    act( "You recite $p.", ch, scroll, NULL, TO_CHAR );

    if (number_percent() >= 20 + get_skill(ch,gsn_scrolls) * 4/5)
    {
	send_to_char("You mispronounce a syllable.\n\r",ch);
	check_improve(ch,gsn_scrolls,FALSE,2);
	if (scroll->pIndexData->vnum == 29806)
	{
	recite_libram_conjuration(ch,victim,scroll,obj, FALSE);
	return;
	}
    }
    else if (scroll->pIndexData->vnum == 29806)
    {
	recite_libram_conjuration(ch,victim,scroll,obj, TRUE);
  	return;
    }
    else
    {
    	obj_cast_spell( scroll->value[1], scroll->value[0], ch, victim, obj );
    	obj_cast_spell( scroll->value[2], scroll->value[0], ch, victim, obj );
    	obj_cast_spell( scroll->value[3], scroll->value[0], ch, victim, obj );
	check_improve(ch,gsn_scrolls,TRUE,2);
    }

    extract_obj( scroll );
    return;
}



void do_brandish( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    OBJ_DATA *staff;
    int sn;


    if ( ( staff = get_eq_char( ch, WEAR_HOLD ) ) == NULL )
    {
	send_to_char( "You hold nothing in your hand.\n\r", ch );
	return;
    }

    if (staff->pIndexData->vnum == 29750)
    {
	brandish_sceptre_dead(ch,staff);
	return;
    }

    if (staff->pIndexData->vnum == OBJ_VNUM_HORN_VALERE)
    {
	obj_brandish_horn_valere(ch,staff);
	return;
    }

    if (ch->cabal == CABAL_RAVAGER && !IS_IMMORTAL(ch))
    {
	send_to_char("You are a Ravager, you don't use such things as staves!\n\r",ch);
	return;
    }

    if ( staff->item_type != ITEM_STAFF )
    {
	send_to_char( "You can brandish only with a staff.\n\r", ch );
	return;
    }

    if (staff->value[4] == 1) {
	obj_brandish_phylactery(ch,staff);
	return;
    }

    if ( ( sn = staff->value[3] ) < 0
    ||   sn >= MAX_SKILL
    ||   skill_table[sn].spell_fun == 0 )
    {
	bug( "Do_brandish: bad sn %d.", sn );
	return;
    }

    WAIT_STATE( ch, 2 * PULSE_VIOLENCE );

    if ( staff->value[2] > 0 )
    {
	act( "$n brandishes $p.", ch, staff, NULL, TO_ROOM );
	act( "You brandish $p.",  ch, staff, NULL, TO_CHAR );
	if ( ch->level < staff->level
	||   number_percent() >= 20 + get_skill(ch,gsn_staves) * 4/5)
 	{
	    act ("You fail to invoke $p.",ch,staff,NULL,TO_CHAR);
	    act ("...and nothing happens.",ch,NULL,NULL,TO_ROOM);
	    check_improve(ch,gsn_staves,FALSE,2);
	}

	else for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next	= vch->next_in_room;

	    switch ( skill_table[sn].target )
	    {
	    default:
		bug( "Do_brandish: bad target for sn %d.", sn );
		return;

	    case TAR_IGNORE:
		if ( vch != ch )
		    continue;
		break;

	    case TAR_CHAR_OFFENSIVE:
		if ( IS_NPC(ch) ? IS_NPC(vch) : !IS_NPC(vch) )
		    continue;
		break;

	    case TAR_CHAR_DEFENSIVE:
		if ( IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch) )
		    continue;
		break;

	    case TAR_CHAR_SELF:
		if ( vch != ch )
		    continue;
		break;
	    }
	    obj_cast_spell( staff->value[3], staff->value[0], ch, vch, NULL );
	    check_improve(ch,gsn_staves,TRUE,2);
	}
    }

    if ( --staff->value[2] <= 0 )
    {
	act( "$n's $p blazes bright and is gone.", ch, staff, NULL, TO_ROOM );
	act( "Your $p blazes bright and is gone.", ch, staff, NULL, TO_CHAR );
	extract_obj( staff );
    }

    return;
}



void do_zap( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *wand;
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if (ch->cabal == CABAL_RAVAGER && !IS_IMMORTAL(ch))
    {
	send_to_char("You are a Ravager, you don't use such things as wands!\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' && ch->fighting == NULL )
    {
	send_to_char( "Zap whom or what?\n\r", ch );
	return;
    }

    if ( ( wand = get_eq_char( ch, WEAR_HOLD ) ) == NULL )
    {
	send_to_char( "You hold nothing in your hand.\n\r", ch );
	return;
    }

    if ( wand->item_type != ITEM_WAND )
    {
	send_to_char( "You can zap only with a wand.\n\r", ch );
	return;
    }

    obj = NULL;
    if ( arg[0] == '\0' )
    {
	if ( ch->fighting != NULL )
	{
	    victim = ch->fighting;
	}
	else
	{
	    send_to_char( "Zap whom or what?\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( ( victim = get_char_room ( ch, arg ) ) == NULL
	&&   ( obj    = get_obj_here  ( ch, arg ) ) == NULL )
	{
	    send_to_char( "You can't find it.\n\r", ch );
	    return;
	}
    }

    WAIT_STATE( ch, 2 * PULSE_VIOLENCE );

    if ( wand->value[2] > 0 )
    {
	if ( victim != NULL )
	{
	    act( "$n zaps $N with $p.", ch, wand, victim, TO_NOTVICT );
	    act( "You zap $N with $p.", ch, wand, victim, TO_CHAR );
	    act( "$n zaps you with $p.",ch, wand, victim, TO_VICT );
	}
	else
	{
	    act( "$n zaps $P with $p.", ch, wand, obj, TO_ROOM );
	    act( "You zap $P with $p.", ch, wand, obj, TO_CHAR );
	}

 	if (ch->level < wand->level
	||  number_percent() >= 20 + get_skill(ch,gsn_wands) * 4/5)
	{
	    act( "Your efforts with $p produce only smoke and sparks.",
		 ch,wand,NULL,TO_CHAR);
	    act( "$n's efforts with $p produce only smoke and sparks.",
		 ch,wand,NULL,TO_ROOM);
	    check_improve(ch,gsn_wands,FALSE,2);
	}
	else
	{
	    obj_cast_spell( wand->value[3], wand->value[0], ch, victim, obj );
	    check_improve(ch,gsn_wands,TRUE,2);
	}
    }

    if ( --wand->value[2] <= 0 )
    {
	act( "$n's $p explodes into fragments.", ch, wand, NULL, TO_ROOM );
	act( "Your $p explodes into fragments.", ch, wand, NULL, TO_CHAR );
	extract_obj( wand );
    }

    return;
}



void do_steal( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj=0;
    int percent;
    int chance;

    if(get_skill(ch,gsn_steal)==0||ch->level<skill_table[gsn_steal].skill_level[ch->class]) {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Steal what from whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "That's pointless.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim))
	return;

    if (victim->fighting != NULL)
    {
	send_to_char("Can't steal while that person is fighting.\n\r",ch);
	return;
    }

	if (isCabalGuard(victim)) {
	act("A bolt from the heavens comes down and smites $n!",ch,obj,0,TO_ROOM);
	act("A bolt from the heavens comes down and smites your bitch ass!",ch,obj,0,TO_CHAR);
	raw_kill(ch,ch);
	return;
	}

	check_eagle_eyes(ch,victim);

    WAIT_STATE( ch, skill_table[gsn_steal].beats );
    chance = get_skill(ch,gsn_steal);

    percent  = number_percent();
    percent += 25;
    percent += ((victim->level - ch->level)*2);
    if (IS_AWAKE(victim))
    {
    percent -= get_curr_stat(ch,STAT_DEX);
    percent += get_curr_stat(victim,STAT_INT)/2;
    percent += get_curr_stat(victim,STAT_DEX)/3;
    }

/*    if (ch->level < skill_table[gsn_steal].skill_level[ch->class])
	chance = 0;*/

    if (IS_NPC(victim) && IS_SET(victim->act,ACT_IS_CHANGER) )
	chance = 0;

    if (!IS_AWAKE(victim))
	{
/*	if (ch->level >= victim->level)
		chance += ( (victim->level - ch->level) * 25);
	else
		chance += 20;*/
	chance+=100;
	}

    if ( percent > chance)
    {
	/*
	 * Failure.
	 */
	send_to_char( "Oops.\n\r", ch );
	affect_strip(ch,gsn_sneak);
	REMOVE_BIT(ch->affected_by,AFF_SNEAK);

	check_improve(ch,gsn_steal,FALSE,2);
	act( "$n tried to steal from you.\n\r", ch, NULL, victim, TO_VICT    );
	act( "$n tried to steal from $N.\n\r",  ch, NULL, victim, TO_NOTVICT );
	switch(number_range(0,3))
	{
	case 0 :
	   sprintf( buf, "%s is a lousy thief!", PERS(ch,victim) );
	   break;
        case 1 :
	   sprintf( buf, "%s couldn't rob %s way out of a paper bag!",
		    PERS(ch,victim),(ch->sex == 2) ? "her" : "his");
	   break;
	case 2 :
	    sprintf( buf,"%s tried to rob me!",PERS(ch,victim) );
	    break;
	case 3 :
	    sprintf(buf,"Keep your hands out of there, %s!",PERS(ch,victim));
	    break;
        }
	if (IS_AWAKE(victim) && !IS_NPC(victim))
	    do_yell( victim, buf );
	if ( !IS_NPC(ch))
	{
	    if ( IS_NPC(victim) && IS_AWAKE(victim))
	    {
		multi_hit( victim, ch, TYPE_UNDEFINED );
	    }
	    else
	    {
		sprintf(buf,"$N tried to steal from %s.",victim->name);
		wiznet(buf,ch,NULL,WIZ_FLAGS,0,0);
	    }
	}

	return;
    }

    if ( !str_cmp( arg1, "coin"  )
    ||   !str_cmp( arg1, "coins" )
    ||   !str_cmp( arg1, "gold"  )
    ||	 !str_cmp( arg1, "silver"))
    {
	int gold, silver;

	gold = victim->gold * number_range(1, ch->level) / 60;
	silver = victim->silver * number_range(1,ch->level) / 60;
	if ( gold <= 0 && silver <= 0 )
	{
	    send_to_char( "You couldn't get any coins.\n\r", ch );
	    return;
	}

	ch->gold     	+= gold;
	ch->silver   	+= silver;
	victim->silver 	-= silver;
	victim->gold 	-= gold;
	if (silver <= 0)
	    sprintf( buf, "Bingo!  You got %d gold coins.\n\r", gold );
	else if (gold <= 0)
	    sprintf( buf, "Bingo!  You got %d silver coins.\n\r",silver);
	else
	    sprintf(buf, "Bingo!  You got %d silver and %d gold coins.\n\r",
		    silver,gold);

	send_to_char( buf, ch );
	check_improve(ch,gsn_steal,TRUE,2);
	return;
    }

    if ( ( obj = get_obj_carry( victim, arg1, ch ) ) == NULL )
    {
	send_to_char( "You can't find it.\n\r", ch );
	return;
    }

    if ( !can_drop_obj( ch, obj )
    ||   IS_SET(obj->extra_flags, ITEM_INVENTORY))
    {
	send_to_char( "You can't pry it away.\n\r", ch );
	return;
    }

    if ( ch->carry_number + get_obj_number( obj ) > can_carry_n( ch ) )
    {
	send_to_char( "You have your hands full.\n\r", ch );
	return;
    }

    if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
    {
	send_to_char( "You can't carry that much weight.\n\r", ch );
	return;
    }

    obj_from_char( obj );
    obj_to_char( obj, ch );
    check_improve(ch,gsn_steal,TRUE,2);
    send_to_char( "Got it!\n\r", ch );
    return;
}



/*
 * Shopping commands.
 */
CHAR_DATA *find_keeper( CHAR_DATA *ch )
{
    /*char buf[MAX_STRING_LENGTH];*/
    CHAR_DATA *keeper;
    SHOP_DATA *pShop;

    pShop = NULL;
    for ( keeper = ch->in_room->people; keeper; keeper = keeper->next_in_room )
    {
	if ( IS_NPC(keeper) && (pShop = keeper->pIndexData->pShop) != NULL )
	    break;
    }

    if ( pShop == NULL )
    {
	send_to_char( "You can't do that here.\n\r", ch );
	return NULL;
    }

    /*
     * Undesirables.
     *
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_KILLER) )
    {
	do_say( keeper, "Killers are not welcome!" );
	sprintf( buf, "%s the KILLER is over here!\n\r", ch->name );
	do_yell( keeper, buf );
	return NULL;
    }

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_THIEF) )
    {
	do_say( keeper, "Thieves are not welcome!" );
	sprintf( buf, "%s the THIEF is over here!\n\r", ch->name );
	do_yell( keeper, buf );
	return NULL;
    }
	*/
    /*
     * Shop hours.
     */
    if ( time_info.hour < pShop->open_hour )
    {
	do_say( keeper, "Sorry, I am closed. Come back later." );
	return NULL;
    }

    if ( time_info.hour > pShop->close_hour )
    {
	do_say( keeper, "Sorry, I am closed. Come back tomorrow." );
	return NULL;
    }

    /*
     * Invisible or hidden people.
     */
    if ( !can_see( keeper, ch ) )
    {
	do_say( keeper, "I don't trade with folks I can't see." );
	return NULL;
    }

    return keeper;
}

/* insert an object at the right spot for the keeper */
void obj_to_keeper( OBJ_DATA *obj, CHAR_DATA *ch )
{
    OBJ_DATA *t_obj, *t_obj_next;

    /* see if any duplicates are found */
    for (t_obj = ch->carrying; t_obj != NULL; t_obj = t_obj_next)
    {
	t_obj_next = t_obj->next_content;

	if (obj->pIndexData == t_obj->pIndexData
	&&  !str_cmp(obj->short_descr,t_obj->short_descr))
	{
	    /* if this is an unlimited item, destroy the new one */
	    if (IS_OBJ_STAT(t_obj,ITEM_INVENTORY))
	    {
		extract_obj(obj);
		return;
	    }
	    obj->cost = t_obj->cost; /* keep it standard */
	    break;
	}
    }

    if (t_obj == NULL)
    {
	obj->next_content = ch->carrying;
	ch->carrying = obj;
    }
    else
    {
	obj->next_content = t_obj->next_content;
	t_obj->next_content = obj;
    }

    obj->carried_by      = ch;
    obj->in_room         = NULL;
    obj->in_obj          = NULL;
    ch->carry_number    += get_obj_number( obj );
    ch->carry_weight    += get_obj_weight( obj );
}

/* get an object from a shopkeeper's list */
OBJ_DATA *get_obj_keeper( CHAR_DATA *ch, CHAR_DATA *keeper, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = keeper->carrying; obj != NULL; obj = obj->next_content )
    {
        if (obj->wear_loc == WEAR_NONE
        &&  can_see_obj( keeper, obj )
	&&  can_see_obj(ch,obj)
        &&  is_name( arg, obj->name ) )
        {
            if ( ++count == number )
                return obj;

	    /* skip other objects of the same name */
	    while (obj->next_content != NULL
	    && obj->pIndexData == obj->next_content->pIndexData
	    && !str_cmp(obj->short_descr,obj->next_content->short_descr))
		obj = obj->next_content;
        }
    }

    return NULL;
}

int get_cost( CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy )
{
    SHOP_DATA *pShop;
    int cost;

    if ( obj == NULL || ( pShop = keeper->pIndexData->pShop ) == NULL )
	return 0;

    if ( fBuy )
    {
	cost = obj->cost * pShop->profit_buy  / 100;
    }
    else
    {
	OBJ_DATA *obj2;
	int itype;

	cost = 0;
	for ( itype = 0; itype < MAX_TRADE; itype++ )
	{
	    if ( obj->item_type == pShop->buy_type[itype] )
	    {
		cost = obj->cost * pShop->profit_sell / 100;
		break;
	    }
	}

	if (!IS_OBJ_STAT(obj,ITEM_SELL_EXTRACT))
	    for ( obj2 = keeper->carrying; obj2; obj2 = obj2->next_content )
	    {
	    	if ( obj->pIndexData == obj2->pIndexData
		&&   !str_cmp(obj->short_descr,obj2->short_descr) )
		{
	 	    if (IS_OBJ_STAT(obj2,ITEM_INVENTORY))
			cost /= 2;
		    else
                    	cost = cost * 3 / 4;
		}
	    }
    }

    if ( obj->item_type == ITEM_STAFF || obj->item_type == ITEM_WAND )
    {
	if (obj->value[1] == 0)
	    cost /= 4;
	else
	    cost = cost * obj->value[2] / obj->value[1];
    }

    return cost;
}

void do_buy( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cost,roll;

    if( isShifted(ch)  && !IS_IMMORTAL(ch))
    {
      	send_to_char("You can only do this in your human form.\n\r",ch);
      	return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "Buy what?\n\r", ch );
	return;
    }

    if ( IS_SET(ch->in_room->room_flags, ROOM_PET_SHOP) )
    {
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *pet;
	ROOM_INDEX_DATA *pRoomIndexNext;
	ROOM_INDEX_DATA *in_room;

	if ( IS_NPC(ch) )
	    return;

	argument = one_argument(argument,arg);

	/* hack to make new thalos pets work */
	    pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );
	if ( pRoomIndexNext == NULL )
	{
	    bug( "Do_buy: bad pet shop at vnum %d.", ch->in_room->vnum );
	    send_to_char( "Sorry, you can't buy that here.\n\r", ch );
	    return;
	}

	in_room     = ch->in_room;
	ch->in_room = pRoomIndexNext;
	pet         = get_char_room( ch, arg );
	ch->in_room = in_room;

	if ( pet == NULL || !IS_SET(pet->act, ACT_PET) )
	{
	    send_to_char( "Sorry, you can't buy that here.\n\r", ch );
	    return;
	}

	if ( ch->pet != NULL )
	{
	    send_to_char("You already own a pet.\n\r",ch);
	    return;
	}

 	cost = 10 * pet->level * pet->level;

	if ( (ch->silver + 100 * ch->gold) < cost )
	{
	    send_to_char( "You can't afford it.\n\r", ch );
	    return;
	}

	if ( ch->level < pet->level )
	{
	    send_to_char(
		"You're not powerful enough to master this pet.\n\r", ch );
	    return;
	}

	/* haggle */
	roll = number_percent();
	if (roll < get_skill(ch,gsn_haggle))
	{
	    cost -= cost / 2 * roll / 100;
	    sprintf(buf,"You haggle the price down to %d coins.\n\r",cost);
	    send_to_char(buf,ch);
	    check_improve(ch,gsn_haggle,TRUE,4);

	}

	deduct_cost(ch,cost);
	pet			= create_mobile( pet->pIndexData );
	SET_BIT(pet->act, ACT_PET);
	SET_BIT(pet->affected_by, AFF_CHARM);
	pet->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;

	argument = one_argument( argument, arg );
	if ( arg[0] != '\0' )
	{
	    sprintf( buf, "%s %s", pet->name, arg );
	    free_string( pet->name );
	    pet->name = str_dup( buf );
	}
	sprintf( buf, "%sA neck tag says 'I belong to %s'.\n\r",
	    pet->description, ch->name );
	free_string( pet->description );
	pet->description = str_dup( buf );

	char_to_room( pet, ch->in_room );
	add_follower( pet, ch );
	pet->leader = ch;
	ch->pet = pet;
	send_to_char( "Enjoy your pet.\n\r", ch );
	act( "$n bought $N as a pet.", ch, NULL, pet, TO_ROOM );
	return;
    }
    else
    {
	CHAR_DATA *keeper;
	OBJ_DATA *obj,*t_obj;
	char arg[MAX_INPUT_LENGTH];
	int number, count = 1;

	if ( ( keeper = find_keeper( ch ) ) == NULL )
	    return;

	if (keeper == ch)
		return;

	number = mult_argument(argument,arg);
	obj  = get_obj_keeper( ch,keeper, arg );
	cost = get_cost( keeper, obj, TRUE );

	if (number < 1)
	{
	    act("$n tells you 'Get real!",keeper,NULL,ch,TO_VICT);
	    return;
	}

	if ( cost <= 0 || !can_see_obj( ch, obj ) )
	{
	    act( "$n tells you 'I don't sell that -- try 'list''.",
		keeper, NULL, ch, TO_VICT );
	    ch->reply = keeper;
	    return;
	}

	if (!IS_OBJ_STAT(obj,ITEM_INVENTORY))
	{
	    for (t_obj = obj->next_content;
	     	 count < number && t_obj != NULL;
	     	 t_obj = t_obj->next_content)
	    {
	    	if (t_obj->pIndexData == obj->pIndexData
	    	&&  !str_cmp(t_obj->short_descr,obj->short_descr))
		    count++;
	    	else
		    break;
	    }

	    if (count < number)
	    {
	    	act("$n tells you 'I don't have that many in stock.",
		    keeper,NULL,ch,TO_VICT);
	    	ch->reply = keeper;
	    	return;
	    }
	}

	if ( (ch->silver + ch->gold * 100) < cost * number )
	{
	    if (number > 1)
		act("$n tells you 'You can't afford to buy that many.",
		    keeper,obj,ch,TO_VICT);
	    else
	    	act( "$n tells you 'You can't afford to buy $p'.",
		    keeper, obj, ch, TO_VICT );
	    ch->reply = keeper;
	    return;
	}

	if ( obj->level > ch->level )
	{
	    act( "$n tells you 'You can't use $p yet'.",
		keeper, obj, ch, TO_VICT );
	    ch->reply = keeper;
	    return;
	}

	if (ch->carry_number +  number * get_obj_number(obj) > can_carry_n(ch))
	{
	    send_to_char( "You can't carry that many items.\n\r", ch );
	    return;
	}

	if ( ch->carry_weight + number * get_obj_weight(obj) > can_carry_w(ch))
	{
	    send_to_char( "You can't carry that much weight.\n\r", ch );
	    return;
	}

	/* haggle */
	roll = number_percent();
	if (!IS_OBJ_STAT(obj,ITEM_SELL_EXTRACT)
	&& roll < get_skill(ch,gsn_haggle))
	{
	    cost -= obj->cost / 2 * roll / 100;
	    act("You haggle with $N.",ch,NULL,keeper,TO_CHAR);
	    check_improve(ch,gsn_haggle,TRUE,4);
	}

	if (number > 1)
	{
	    sprintf(buf,"$n buys $p[%d].",number);
	    act(buf,ch,obj,NULL,TO_ROOM);
	    sprintf(buf,"You buy $p[%d] for %d silver.",number,cost * number);
	    act(buf,ch,obj,NULL,TO_CHAR);
	}
	else
	{
	    act( "$n buys $p.", ch, obj, NULL, TO_ROOM );
	    sprintf(buf,"You buy $p for %d silver.",cost);
	    act( buf, ch, obj, NULL, TO_CHAR );
	}
	deduct_cost(ch,cost * number);
	keeper->gold += cost * number/100;
	keeper->silver += cost * number - (cost * number/100) * 100;

	for (count = 0; count < number; count++)
	{
	    if ( IS_SET( obj->extra_flags, ITEM_INVENTORY ) )
	    	t_obj = create_object( obj->pIndexData, obj->level );
	    else
	    {
		t_obj = obj;
		obj = obj->next_content;
	    	obj_from_char( t_obj );
	    }

	    if (t_obj->timer > 0 && !IS_OBJ_STAT(t_obj,ITEM_HAD_TIMER))
	    	t_obj->timer = 0;
	    REMOVE_BIT(t_obj->extra_flags,ITEM_HAD_TIMER);
	    obj_to_char( t_obj, ch );
	    if (cost < t_obj->cost)
	    	t_obj->cost = cost;
	}
    }
}

void do_list( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if( isShifted(ch)  && !IS_IMMORTAL(ch))
    {
      	send_to_char("You can only do this in your human form.\n\r",ch);
      	return;
    }

    if ( IS_SET(ch->in_room->room_flags, ROOM_PET_SHOP) )
    {
	ROOM_INDEX_DATA *pRoomIndexNext;
	CHAR_DATA *pet;
	bool found;

        /* hack to make new thalos pets work */
        if (ch->in_room->vnum == 9621)
            pRoomIndexNext = get_room_index(9706);
        else
            pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );

	if ( pRoomIndexNext == NULL )
	{
	    bug( "Do_list: bad pet shop at vnum %d.", ch->in_room->vnum );
	    send_to_char( "You can't do that here.\n\r", ch );
	    return;
	}

	found = FALSE;
	for ( pet = pRoomIndexNext->people; pet; pet = pet->next_in_room )
	{
	    if ( IS_SET(pet->act, ACT_PET) )
	    {
		if ( !found )
		{
		    found = TRUE;
		    send_to_char( "Pets for sale:\n\r", ch );
		}
		sprintf( buf, "[%2d] %8d - %s\n\r",
		    pet->level,
		    10 * pet->level * pet->level,
		    pet->short_descr );
		send_to_char( buf, ch );
	    }
	}
	if ( !found )
	    send_to_char( "Sorry, we're out of pets right now.\n\r", ch );
	return;
    }
    else
    {
	CHAR_DATA *keeper;
	OBJ_DATA *obj;
	int cost,count;
	bool found;
	char arg[MAX_INPUT_LENGTH];

	if ( ( keeper = find_keeper( ch ) ) == NULL )
	    return;
        one_argument(argument,arg);

	found = FALSE;
	for ( obj = keeper->carrying; obj; obj = obj->next_content )
	{
	    if ( obj->wear_loc == WEAR_NONE
	    &&   can_see_obj( ch, obj )
	    &&   ( cost = get_cost( keeper, obj, TRUE ) ) > 0
	    &&   ( arg[0] == '\0'
 	       ||  is_name(arg,obj->name) ))
	    {
		if ( !found )
		{
		    found = TRUE;
		    send_to_char( "[Lv Price Qty] Item\n\r", ch );
		}

		if (IS_OBJ_STAT(obj,ITEM_INVENTORY))
		    sprintf(buf,"[%2d %5d -- ] %s\n\r",
			obj->level,cost,obj->short_descr);
		else
		{
		    count = 1;

		    while (obj->next_content != NULL
		    && obj->pIndexData == obj->next_content->pIndexData
		    && !str_cmp(obj->short_descr,
			        obj->next_content->short_descr))
		    {
			obj = obj->next_content;
			count++;
		    }
		    sprintf(buf,"[%2d %5d %2d ] %s\n\r",
			obj->level,cost,count,obj->short_descr);
		}
		send_to_char( buf, ch );
	    }
	}


	if ( !found )
	    send_to_char( "You can't buy anything here.\n\r", ch );
	return;
    }
}



void do_sell( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *keeper;
    OBJ_DATA *obj;
    int cost,roll;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Sell what?\n\r", ch );
       return;
    }

    if ( ( keeper = find_keeper( ch ) ) == NULL )
	return;

    if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
    {
	act( "$n tells you 'You don't have that item'.",
	    keeper, NULL, ch, TO_VICT );
	ch->reply = keeper;
	return;
    }

    if ( !can_drop_obj( ch, obj ) )
    {
	send_to_char( "You can't let go of it.\n\r", ch );
	return;
    }

    if (!can_see_obj(keeper,obj))
    {
	act("$n doesn't see what you are offering.",keeper,NULL,ch,TO_VICT);
	return;
    }

    if ( ( cost = get_cost( keeper, obj, FALSE ) ) <= 0 )
    {
	act( "$n looks uninterested in $p.", keeper, obj, ch, TO_VICT );
	return;
    }
    if ( cost > (keeper-> silver + 100 * keeper->gold) )
    {
	act("$n tells you 'I'm afraid I don't have enough wealth to buy $p.",
	    keeper,obj,ch,TO_VICT);
	return;
    }

    act( "$n sells $p.", ch, obj, NULL, TO_ROOM );
    /* haggle */
    roll = number_percent();
    if (!IS_OBJ_STAT(obj,ITEM_SELL_EXTRACT) && roll < get_skill(ch,gsn_haggle))
    {
        send_to_char("You haggle with the shopkeeper.\n\r",ch);
        cost += obj->cost / 2 * roll / 100;
        cost = UMIN(cost,95 * get_cost(keeper,obj,TRUE) / 100);
	cost = UMIN(cost,(keeper->silver + 100 * keeper->gold));
        check_improve(ch,gsn_haggle,TRUE,4);
    }
    sprintf( buf, "You sell $p for %d silver and %d gold piece%s.",
	cost - (cost/100) * 100, cost/100, cost == 1 ? "" : "s" );
    act( buf, ch, obj, NULL, TO_CHAR );
    ch->gold     += cost/100;
    ch->silver 	 += cost - (cost/100) * 100;
    deduct_cost(keeper,cost);
    if ( keeper->gold < 0 )
	keeper->gold = 0;
    if ( keeper->silver< 0)
	keeper->silver = 0;

    if ( obj->item_type == ITEM_TRASH || IS_OBJ_STAT(obj,ITEM_SELL_EXTRACT))
    {
	extract_obj( obj );
    }
    else
    {
	obj_from_char( obj );
	if (obj->timer)
	    SET_BIT(obj->extra_flags,ITEM_HAD_TIMER);
	else
	    obj->timer = number_range(50,100);
	obj_to_keeper( obj, keeper );
    }

    return;
}



void do_value( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *keeper;
    OBJ_DATA *obj;
    int cost;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Value what?\n\r", ch );
	return;
    }

    if ( ( keeper = find_keeper( ch ) ) == NULL )
	return;

    if ( ( obj = get_obj_carry( ch, arg, ch ) ) == NULL )
    {
	act( "$n tells you 'You don't have that item'.",
	    keeper, NULL, ch, TO_VICT );
	ch->reply = keeper;
	return;
    }

    if (!can_see_obj(keeper,obj))
    {
        act("$n doesn't see what you are offering.",keeper,NULL,ch,TO_VICT);
        return;
    }

    if ( !can_drop_obj( ch, obj ) )
    {
	send_to_char( "You can't let go of it.\n\r", ch );
	return;
    }

    if ( ( cost = get_cost( keeper, obj, FALSE ) ) <= 0 )
    {
	act( "$n looks uninterested in $p.", keeper, obj, ch, TO_VICT );
	return;
    }

    sprintf( buf,
	"$n tells you 'I'll give you %d silver and %d gold coins for $p'.",
	cost - (cost/100) * 100, cost/100 );
    act( buf, keeper, obj, ch, TO_VICT );
    ch->reply = keeper;

    return;
}


void do_request(CHAR_DATA *ch,char *argument)
{
    AFFECT_DATA af;
    CHAR_DATA  *victim;
    OBJ_DATA *obj;
    char i_name[MAX_INPUT_LENGTH];
    char m_name[MAX_INPUT_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *target_name;

    target_name = one_argument(argument,i_name);  /* syntax: request <obj>
<mob> */
    one_argument(target_name,m_name);

    if (IS_NPC(ch))
        return;     /* NPCs can't request */
    if (i_name[0] == '\0' || m_name[0] == '\0')
    {
	send_to_char("Request what of whom?\n\r",ch);
	return;
    }
    if ((victim = get_char_room(ch,m_name)) == NULL )
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    if (is_affected(ch,gsn_request))
	{
	send_to_char("You are still in conemplation of the last act of generosity.\n\r",ch);
	return;
	}

    if (!IS_NPC(victim))
    {
        send_to_char("Why not ask for it yourself?\n\r",ch);
        return;
    }

    if (victim->pIndexData->vnum == MOB_VNUM_PROTECTOR)
    {
	sprintf(buf1,"Do not dare to request my belongings!");
	sprintf(buf2,"Help! I'm being attacked by %s!",victim->short_descr);
	do_say(victim,buf1);
	do_yell(ch,buf2);
//	multi_hit(victim,ch,TYPE_UNDEFINED);
	do_murder(victim,ch->name);
	return;
    }

    if (!IS_GOOD(victim))
    {
	sprintf(buf1, "You dare to ask for my belongings!");
	sprintf(buf2, "Help! I'm being attacked by %s!",victim->short_descr);
	do_say(victim,buf1);
	do_yell(ch,buf2);
//	multi_hit(victim,ch,TYPE_UNDEFINED);
	do_murder(victim,ch->name);
	return;
    }

    if (!IS_GOOD(ch))
    {
        sprintf(buf1,"You are unworthy to ask for anything!");
        sprintf(buf2,"Help! I'm being attacked by %s!", victim->short_descr);
        do_say(victim,buf1);
        do_yell(ch,buf2);
//        multi_hit(victim,ch,TYPE_UNDEFINED);
	do_murder(victim,ch->name);
        return;
    }

    if ( (obj = get_obj_list(victim, i_name, victim->carrying)) == NULL)
    {
        send_to_char("They do not have that object.\n\r",ch);
        return;
    }
    if (!can_see_obj(ch,obj) )
    {
        send_to_char("They do not have that object.\n\r",ch);
        return;
    }
    if (!can_see(victim,ch))
    {
        act("$N tells you, 'I don't give to those I can't see.'",ch,0,victim,TO_CHAR);
        return;
    }

    if (!can_see_obj(victim,obj))
    {
        act("$N tells you, 'I'm afraid i can't see such an object.'",ch,0,victim,TO_CHAR);
        return;
    }

    if ((victim->level >= (ch->level + 8)) && victim->level < 58)
    {
        act("$N tells you, 'All in good time my child'.",ch,0,victim,TO_CHAR);
        return;
    }
    else if (victim->level >= 59 && ch->level < 51)
    {
        act("$N tells you, 'All in good time my child'.",ch,0,victim,TO_CHAR);
        return;
    }

    if (ch->move < obj->level)
    {
        act("$N tells you, 'You look tired, perhaps you should rest first'.",ch,0,victim,TO_CHAR);
        return;
    }
    if (ch->hit < (ch->max_hit*3/7))
    {
        act("$N tells you, 'You look worn out, perhaps you should rest and get better first'.",ch,0,victim,TO_CHAR);
        return;
    }
    if ((obj->wear_loc != WEAR_NONE) && IS_SET(obj->extra_flags, ITEM_NOREMOVE) )
    {
        act("$N tells you, 'Alas, i am unable to release $p'.",ch,obj,victim,TO_CHAR);
        return;
    }
    if (IS_SET(obj->extra_flags,ITEM_NODROP) )
    {
        act("$N tells you, 'Alas, i am unable to release $p.'",ch,obj,victim,TO_CHAR);
        return;
    }

    if ( ch->carry_weight + get_obj_weight(obj)  > can_carry_w(ch) )
    {
        act("$N tells you, 'You can't carry the weight $n.'",ch,0,victim,TO_CHAR);
        return;
    }
    if ( ch->carry_number + 1 > can_carry_n(ch) )
    {
        act("$N tells you, 'You can't carry that many items $n.'",ch,0,victim,TO_CHAR);
        return;
    }
	if (isCabalItem(obj)) {
	act("A bolt from the heavens comes down and smites $n!",ch,obj,0,TO_ROOM);
	act("A bolt from the heavens comes down and smites your bitch ass!",ch,obj,0,TO_CHAR);
	raw_kill(ch,ch);
	return;
	}
    act("$n requests off $N.",ch,0,victim,TO_ROOM);
    if (obj->wear_loc != WEAR_NONE)
    {
        act("$n stops using $p.",victim,obj,NULL,TO_ROOM);
        act("You stop using $p.",victim,obj,NULL,TO_CHAR);
    }
    act("$N gives $p to $n.",ch,obj,victim,TO_NOTVICT);
    act("$N gives you $p.",ch,obj,victim,TO_CHAR);
    obj_from_char(obj);
    obj_to_char(obj,ch);
    send_to_char("You stop and contemplate the goodness you have seen in the world.\n\r",ch);

        init_affect(&af);
af.where = TO_AFFECTS;
af.type = gsn_request;
        af.aftype    = AFT_SKILL;
af.modifier = 0;
af.duration = 5;
af.location = 0;
af.bitvector = 0;
af.level = ch->level;
affect_to_char(ch,&af);

    ch->move -= obj->level;
    ch->hit -= obj->level;
    return;
}




bool hands_full(CHAR_DATA *ch)
{
    int count;

    count = 0;

   if (get_eq_char(ch,WEAR_LIGHT) != NULL)
	count++;
   if (get_eq_char(ch,WEAR_WIELD) != NULL)
	count++;
   if (get_eq_char(ch,WEAR_HOLD) != NULL)
	count++;
   if (get_eq_char(ch,WEAR_SHIELD) != NULL)
	count++;
   if (get_eq_char(ch,WEAR_DUAL_WIELD) != NULL)
	count++;
   if (get_eq_char(ch,WEAR_TERTIARY_WIELD) != NULL)
	count++;
   if (count < 2)
	return FALSE;
   if (count > 2)
   	bug("Hands full: Character holding %d items.",count);

    return TRUE;

}


void do_battleshield(CHAR_DATA *ch,char *argument)
{
    AFFECT_DATA af;
    AFFECT_DATA paf;

    OBJ_DATA *shield;
    int mod;

    if ( (get_skill(ch,gsn_battleshield) == 0)
	|| ch->level < skill_table[gsn_battleshield].skill_level[ch->class])
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (is_affected(ch,gsn_battleshield))
    {
	send_to_char("You aren't up to forging a new shield yet.\n\r",ch);
	return;
    }

    if (ch->mana < 60)
    {
	send_to_char("You don't have the mana.\n\r",ch);
	return;
    }

    if (number_percent() > get_skill(ch,gsn_battleshield))
    {
	send_to_char("You fail to forge a new battleshield.\n\r",ch);
	ch->mana -= 30;
	return;
    }
    ch->mana -= 60;

    shield = create_object(get_obj_index(OBJ_VNUM_BATTLESHIELD),ch->level);
    shield->level = ch->level;
    shield->value[0] = 8 + ch->level/10;
    shield->value[1] = 8 + ch->level/10;
    shield->value[2] = 8 + ch->level/10;
    shield->value[3] = 8 + ch->level/10;
    shield->value[4] = 8 + ch->level/10;

        init_affect(&af);
    af.where = TO_AFFECTS;
    af.type = gsn_battleshield;
        af.aftype    = AFT_POWER;
    af.level = ch->level;
    af.duration = 24;
    af.bitvector = 0;
    af.modifier = 0;
    af.location = 0;
    affect_to_char(ch,&af);

    paf.where = TO_OBJECT;
    af.type = gsn_battleshield;
    af.level = ch->level;
    af.duration = -1;
    af.bitvector = 0;
    af.location = APPLY_DAMROLL;

    if (ch->level < 30)
	mod = 1;
    else if (ch->level < 40)
	mod = 2;
    else if (ch->level < 50)
	mod = 3;
    else
	mod = 4;

    af.modifier = 2 + mod;
    af.location = APPLY_DAMROLL;
    affect_to_obj(shield,&af);
    af.location = APPLY_HITROLL;
    affect_to_obj(shield,&af);

    af.location = APPLY_HIT;
    af.modifier = ch->level;
    affect_to_obj(shield,&af);

    af.location = APPLY_MOVE;
    affect_to_obj(shield,&af);

    af.location = APPLY_SAVES;
    af.modifier = -2*mod;
      affect_to_obj(shield,&af);

    obj_to_char(shield,ch);
    act("You forge $p!",ch,shield,0,TO_CHAR);
    act("$n forges $p!",ch,shield,0,TO_ROOM);
    return;
}

void do_embalm(CHAR_DATA *ch,char *argument)
{
    OBJ_DATA *corpse;
    char arg[100];

    if ( (get_skill(ch,gsn_embalm) == 0)
    || ch->level < skill_table[gsn_embalm].skill_level[ch->class])
    {
        send_to_char("You don't know how to embalm corpses.\n\r",ch);
        return;
    }
    one_argument(argument,arg);
    if (arg[0] == '\0')
    {
        send_to_char("Embalm what?\n\r",ch);
        return;
    }

    if ( (corpse = get_obj_here(ch,arg)) == NULL)
    {
        send_to_char("You don't have that item to embalm.\n\r",ch);
        return;
    }
    if (corpse->item_type != ITEM_CORPSE_NPC)
    {
        act("$p isn't a corpse you can embalm.\n\r",ch,corpse,0,TO_CHAR);
        return;
    }
    if (corpse->value[4] == 1)
    {
        send_to_char("That corpse has already been embalmed.\n\r",ch);
        return;
    }

    if (number_percent() > ch->pcdata->learned[gsn_embalm])
    {
        act("You spill your embalming fluids all over $p, ruining it.",ch,corpse,0,TO_CHAR);
        act("$n spills embalming fuilds over $p and ruins it.",ch,corpse,0,TO_ROOM);
        check_improve(ch,gsn_embalm,FALSE,4);
	extract_obj(corpse);
        return;
    }
    act("$n embalms $p!",ch,corpse,0,TO_ROOM);
    act("You succeed in embalming $p.",ch,corpse,0,TO_CHAR);
    corpse->timer += ch->level/2;
    corpse->value[4] = 1;
    check_improve(ch,gsn_embalm,FALSE,4);
    return;
}

void cabal_shudder(int cabal)
{
	CHAR_DATA *ch;
	for (ch = char_list; ch != NULL; ch = ch->next)
	{
        if (ch->cabal == cabal)
                send_to_char("{gYou feel a shudder in your Cabal power!{x\n\r",ch);
	}
	return;
}

bool cabal_down(CHAR_DATA *ch,int cabal )
{
    OBJ_DATA *obj;
    bool is_down;
    int rightvnum=0;
    int mobnum;

    if (cabal==CABAL_ACADIAN) 	{rightvnum=OBJ_VNUM_ACADIAN;}
    if (cabal==CABAL_PROTECTOR) {rightvnum=OBJ_VNUM_PROTECTOR;}
    if (cabal==CABAL_COVEN) 	{rightvnum=OBJ_VNUM_COVEN;}
    if (cabal==CABAL_RAVAGER) 	{rightvnum=OBJ_VNUM_RAVAGER;}
    if (cabal==CABAL_VIGILANTE) {rightvnum=OBJ_VNUM_VIGILANTE;}
    if (cabal==CABAL_BRIAR)  	{rightvnum=OBJ_VNUM_BRIAR;}
    is_down = FALSE;
    for (obj = object_list; obj != NULL; obj = obj->next)
    {
        if (obj->pIndexData->vnum == rightvnum)
		break;
    }

    if (obj == NULL)
     return FALSE;

    /* CLS - put a check for obj->carried_by just incase no one does */
    if(obj->carried_by == NULL)
     return FALSE;

    if(!IS_NPC(obj->carried_by))
     return FALSE;

    mobnum=obj->carried_by->pIndexData->vnum;
    switch(cabal)
    {
        case (CABAL_ACADIAN):
	if (mobnum == MOB_VNUM_PROTECTOR
	|| mobnum == MOB_VNUM_VIGILANTE
	|| mobnum == MOB_VNUM_RAVAGER
	|| mobnum == MOB_VNUM_COVEN
	|| mobnum == MOB_VNUM_BRIAR)
		is_down = TRUE;
	break;
        case (CABAL_PROTECTOR):
	if (mobnum == MOB_VNUM_PROTECTOR
	|| mobnum == MOB_VNUM_VIGILANTE
	|| mobnum == MOB_VNUM_RAVAGER
	|| mobnum == MOB_VNUM_COVEN
	|| mobnum == MOB_VNUM_BRIAR)
	is_down = TRUE;
	break;
        case (CABAL_COVEN):
	if (mobnum == MOB_VNUM_PROTECTOR
	|| mobnum == MOB_VNUM_VIGILANTE
	|| mobnum == MOB_VNUM_RAVAGER
	|| mobnum == MOB_VNUM_COVEN
	|| mobnum == MOB_VNUM_BRIAR)
	is_down = TRUE;
	break;
        case (CABAL_RAVAGER):
	if (mobnum == MOB_VNUM_PROTECTOR
	|| mobnum == MOB_VNUM_VIGILANTE
	|| mobnum == MOB_VNUM_RAVAGER
	|| mobnum == MOB_VNUM_COVEN
	|| mobnum == MOB_VNUM_BRIAR)
	is_down = TRUE;
	break;
        case (CABAL_VIGILANTE):
	if (mobnum == MOB_VNUM_PROTECTOR
	|| mobnum == MOB_VNUM_VIGILANTE
	|| mobnum == MOB_VNUM_RAVAGER
	|| mobnum == MOB_VNUM_COVEN
	|| mobnum == MOB_VNUM_BRIAR)
	is_down = TRUE;
	break;
    	case (CABAL_BRIAR):
	if (mobnum == MOB_VNUM_PROTECTOR
	|| mobnum == MOB_VNUM_VIGILANTE
	|| mobnum == MOB_VNUM_RAVAGER
	|| mobnum == MOB_VNUM_COVEN
	|| mobnum == MOB_VNUM_BRIAR)
	is_down = TRUE;
	break;

      }

    if (is_down)
    {
	if (IS_IMMORTAL(ch) || IS_HEROIMM(ch))
	{
		send_to_char("Your Cabal has no power but you can do that anyway.\n\r",ch);
	return FALSE;
        }
        	send_to_char("You cannot find the Cabal power within you.\n\r",ch);
    }
    return is_down;
}

bool cant_carry(CHAR_DATA *ch, OBJ_DATA *obj)
{
    bool status;
    status = FALSE;
    return FALSE;
/*    if (IS_NPC(ch))
    {
	if (obj->pIndexData->vnum == OBJ_VNUM_ANCIENT
	|| obj->pIndexData->vnum == OBJ_VNUM_ARCANA
	|| obj->pIndexData->vnum == OBJ_VNUM_KNIGHT
	|| obj->pIndexData->vnum == OBJ_VNUM_RAGER
        || obj->pIndexData->vnum == OBJ_VNUM_ENFORCER
        || obj->pIndexData->vnum == OBJ_VNUM_EMPIRE
	|| obj->pIndexData->vnum == OBJ_VNUM_OUTLAW
	|| obj->pIndexData->vnum == OBJ_VNUM_SYLVAN)
	status = TRUE;
    }
*/
    return FALSE;
}

bool is_restricted(CHAR_DATA *ch,OBJ_DATA *obj)
{
    int restrict;
    bool status;
    char *race;

    if (IS_NPC(ch) && !IS_AFFECTED(ch,AFF_CHARM))
	return FALSE;

    if (str_cmp(obj->owner,"none") && str_cmp(obj->owner,ch->name))
	return TRUE;

    race = race_table[ch->race].name;
    restrict = obj->pIndexData->restrict_flags;

    status = FALSE;

/*    if (!str_cmp(race,"changeling"))
    {
        if (IS_SET(obj->wear_flags,ITEM_WEAR_BODY))
	{
        send_to_char("Your race does not wear full body items.\n\r",ch);
	return TRUE;
	}
    }
*/
    if (!restrict)
	return FALSE;

    if ((IS_SET(restrict,ACADIAN_ONLY) && ch->cabal != CABAL_ACADIAN)
    || (IS_SET(restrict, COVEN_ONLY) && ch->cabal != CABAL_COVEN)
    || (IS_SET(restrict, RAVAGER_ONLY) && ch->cabal != CABAL_RAVAGER)
    || (IS_SET(restrict, VIGILANTE_ONLY) && ch->cabal != CABAL_VIGILANTE)
    || (IS_SET(restrict, PROTECTOR_ONLY) && ch->cabal != CABAL_PROTECTOR)
    || (IS_SET(restrict, HUMAN_ONLY) && str_cmp(race,"human"))
    || (IS_SET(restrict, ELF_ONLY) && str_cmp(race,"elf"))
    || (IS_SET(restrict, DARK_ONLY) && str_cmp(race,"dark-elf"))
    || (IS_SET(restrict, GIANT_ONLY) && (str_cmp(race,"cloud") || str_cmp(race,"storm") || str_cmp(race,"fire")))
    || (IS_SET(restrict, TROLL_ONLY) && str_cmp(race,"troll"))
    || (IS_SET(restrict, CENTAUR_ONLY) && str_cmp(race,"centaur"))
    || (IS_SET(restrict, GNOME_ONLY) && str_cmp(race,"gnome"))
    || (IS_SET(restrict, DWARF_ONLY) && (str_cmp(race,"dwarf") || str_cmp(race,"duergar")))
    || (IS_SET(restrict, DRACONIAN_ONLY) && str_cmp(race,"draconian")))
	status = TRUE;

    if ( IS_SET(restrict, MAGE_ONLY) )
    {
	if (ch->class != CLASS_PRIEST
	&& ch->class != CLASS_TRANSMUTER && ch->class != CLASS_NECROMANCER
	&& ch->class != CLASS_ELEMENTALIST)
	status = TRUE;
    }

    if ( (IS_SET(restrict, RANGER_ONLY) && ch->class != CLASS_RANGER)
    || (IS_SET(restrict, ANTI_PALADIN_ONLY) && ch->class != CLASS_ANTI_PALADIN)
     || (IS_SET(restrict, ASSASSIN_ONLY) && ch->class != CLASS_ASSASSIN)
     || (IS_SET(restrict, PALADIN_ONLY) && ch->class != CLASS_PALADIN)
     || (IS_SET(restrict, THIEF_ONLY) && ch->class != CLASS_THIEF)
     || (IS_SET(restrict, NECROMANCER_ONLY) && ch->class != CLASS_NECROMANCER)
     || (IS_SET(restrict, TRANSMUTER_ONLY) && ch->class != CLASS_TRANSMUTER)
     || (IS_SET(restrict, ELEMENTALIST_ONLY) && ch->class != CLASS_ELEMENTALIST)
     || (IS_SET(restrict, BARD_ONLY) && ch->class != CLASS_BARD) )
	status = TRUE;

    if (IS_SET(restrict,RANGER_ONLY) && ch->class != CLASS_RANGER)
	status = TRUE;
    if (IS_SET(restrict,BARD_ONLY) && ch->class != CLASS_BARD)
        status = TRUE;
    if (IS_SET(restrict,ANTI_PALADIN_ONLY) && ch->class != CLASS_ANTI_PALADIN)
	status = TRUE;
    if (IS_SET(restrict, ASSASSIN_ONLY) && ch->class != CLASS_ASSASSIN)
	status = TRUE;
    if (IS_SET(restrict, PALADIN_ONLY) && ch->class != CLASS_PALADIN )
	status = TRUE;
    if (IS_SET(restrict, MONK_ONLY) && ch->class != CLASS_MONK )
	status = TRUE;
    if (IS_SET(restrict, PRIEST_ONLY) && ch->class != CLASS_PRIEST )
	status = TRUE;
    if (IS_SET(restrict, TRANSMUTER_ONLY) && ch->class != CLASS_TRANSMUTER )
	status = TRUE;
    if (IS_SET(restrict, NECROMANCER_ONLY) && ch->class != CLASS_NECROMANCER )
	status = TRUE;
    if (IS_SET(restrict, ELEMENTALIST_ONLY) && ch->class != CLASS_ELEMENTALIST )
	status = TRUE;
    if (IS_SET(restrict, THIEF_ONLY) && ch->class != CLASS_THIEF )
	status = TRUE;
    if (IS_SET(restrict, WARRIOR_ONLY) && ch->class != CLASS_WARRIOR )
	status = TRUE;
    if (IS_SET(restrict, NEWBIE_ONLY) && ((ch->played + current_time - ch->logon) > 5) )
	status = TRUE;


    return status;
}

void do_trophy(CHAR_DATA *ch,char *argument)
{
    AFFECT_DATA af;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char *name;
    char *last_name;
    OBJ_DATA *obj;
    OBJ_DATA *poncho;
    int chance;
    int vnum;

    one_argument(argument,arg);

    if ( ((chance = get_skill(ch,gsn_trophy)) == 0)
	|| ch->level < skill_table[gsn_trophy].skill_level[ch->class])
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (is_affected(ch,gsn_trophy))
    {
	send_to_char("You don't feel up to making a new trophy yet.\n\r",ch);
	return;
    }
    if (ch->mana < 30)
    {
	send_to_char("You don't have the mana.\n\r",ch);
	return;
    }

    obj = get_obj_carry(ch,arg,ch);

    if (obj == NULL || arg[0] == '\0')
    {
	send_to_char("Make a trophy from what body part?\n\r",ch);
	return;
    }

	vnum = obj->pIndexData->vnum;
	if (vnum != OBJ_VNUM_SEVERED_HEAD
	&& vnum != OBJ_VNUM_TORN_HEART
	&& vnum != OBJ_VNUM_SLICED_ARM
	&& vnum != OBJ_VNUM_SLICED_LEG
	&& vnum != OBJ_VNUM_GUTS
	&& vnum != OBJ_VNUM_BRAINS)
	{
	send_to_char("You can't make a trophy from that.\n\r",ch);
	return;
	}

    if (number_percent() > chance)
    {
	act("$n tries to make a trophy from $p but destroys it.",ch,obj,0,TO_ROOM);
	send_to_char("You fail and destroy it.\n\r",ch);
	check_improve(ch,gsn_trophy,FALSE,1);
 	ch->mana -= 15;
	extract_obj(obj);
	return;
    }

   act("$n makes a poncho from $p.",ch,obj,0,TO_ROOM);
   act("You make a bloody trophy from $p.",ch,obj,0,TO_CHAR);
    ch->mana -= 30;
    poncho = create_object(get_obj_index(OBJ_VNUM_PONCHO),0);

    name = obj->short_descr;

    last_name = name;
    last_name = one_argument(last_name,name);
    last_name = one_argument(last_name,name);
    last_name = one_argument(last_name,name);
    name = last_name;

    poncho->level = ch->level;
    sprintf(buf,"a poncho from the carcass of %s",name);
    free_string(poncho->short_descr);
    poncho->short_descr = str_dup(buf);
    sprintf(buf,"A blood stained poncho from the remains of %s lies here.",name);
    free_string(poncho->description);
    poncho->description = str_dup(buf);
    poncho->timer = 58;
    obj_to_char(poncho,ch);
    extract_obj(obj);

    init_affect(&af);
	af.aftype = AFT_POWER;
	af.where = TO_AFFECTS;
    af.type = gsn_trophy;
    af.duration = 60;
    af.location = 0;
    af.bitvector = 0;
    af.modifier = 0;
    af.level = ch->level;
    affect_to_char(ch,&af);

    check_improve(ch,gsn_trophy,TRUE,1);
    return;
}

void wear_obj_bracers(CHAR_DATA *ch,OBJ_DATA *obj) /* strange bracers */
{
	AFFECT_DATA af;
	int sn_haste;

	sn_haste = skill_lookup("haste");
	affect_strip(ch,sn_haste);
	init_affect(&af);
    	af.type = sn_haste;
    	af.aftype = AFT_SPELL;
	af.level = 51;
	af.modifier = 4;
	af.location = APPLY_DEX;
	af.duration = -1;
	af.bitvector = AFF_HASTE;
	af.where = TO_AFFECTS;
	affect_to_char(ch,&af);
	send_to_char("Your arms feel lighter and free of constraints.\n\r",ch);
	send_to_char("You feel yourself moving faster!\n\r",ch);
	act("$n appears to be moving faster.",ch,0,0,TO_ROOM);
	return;
}

void remove_obj_bracers(CHAR_DATA *ch,OBJ_DATA *obj) /* strange bracers */
{
	int sn_haste;
	sn_haste = skill_lookup("haste");

    if (!is_affected(ch,sn_haste))
	return;
	affect_strip(ch,sn_haste);

	act("$n appears to be moving slower.",ch,0,0,TO_ROOM);
	send_to_char("Your arms suddenly feel much heavier.\n\r",ch);
	send_to_char("You are moving slowerr.\n\r",ch);
	return;
}
void remove_obj_girdle(CHAR_DATA *ch, OBJ_DATA *obj)
{
    if (!is_affected(ch,gsn_strange_form))
        return;
    act("$n's face twists and contorts.",ch,0,0,TO_ROOM);
    send_to_char("You feel your face melt back into it's normal shape.\n\r",ch);
    affect_strip(ch,gsn_strange_form);
    ch->pcdata->learned[gsn_chimera_lion] = 0;
    ch->pcdata->learned[gsn_chimera_goat] = 0;
    return;
}

void do_demand(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA  *victim;
    OBJ_DATA *obj;
    char i_name[MAX_INPUT_LENGTH];
    char m_name[MAX_INPUT_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *target_name;
    int chance, vlevel;

    target_name = one_argument(argument,i_name);
    one_argument(target_name,m_name);

    chance = ch->pcdata->learned[gsn_demand];
    if (chance == 0
    || ch->level < skill_table[gsn_demand].skill_level[ch->class])
    {
        send_to_char("You are hardly intimidating enough to demand off others.\n\r",ch);
        return;
    }

    if (IS_NPC(ch))
        return;
    if ((victim = get_char_room(ch,m_name)) == NULL )
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }
    if (!IS_NPC(victim))
    {
        send_to_char("Why not just threaten them in person?\n\r",ch);
        return;
    }
    vlevel = victim->level;
    chance /= 2;
    chance += (3*ch->level);
    chance -= 2*vlevel;

    if (IS_GOOD(victim))
        chance -= 4*vlevel;
    else if (IS_EVIL(victim))
        chance -= 2*vlevel;
    else
        chance -= 3*vlevel;

    vlevel += 8;

    if ((obj = get_obj_list(victim, i_name, victim->carrying)) == NULL)
    {
        send_to_char("They do not have that object.\n\r",ch);
        return;
    }
    if (!can_see_obj(ch,obj) )
    {
        send_to_char("They do not have that object.\n\r",ch);
        return;
    }

    if (vlevel > ch->level
    || number_percent() > chance)
    {
        check_improve(ch,gsn_demand,FALSE,2);
        sprintf(buf1,"I don't think i'd give my belongings to one as weak as you!");
        sprintf(buf2,"Help! I'm being attacked by %s!",victim->short_descr);
        do_say(victim,buf1);
        do_yell(ch,buf2);
        multi_hit(victim,ch,TYPE_UNDEFINED);
        return;
    }

    if (!can_see(victim,ch))
    {
        act("$N tells you, 'I can't give to those i can't see.'",ch,0,victim,TO_CHAR);
        return;
    }

    if (!can_see_obj(victim,obj))
    {
        act("$N tells you, 'I can't see such an object.'",ch,0,victim,TO_CHAR);
        return;
    }

    if (obj->level > ch->level + 8)
    {
        do_say(victim, "That item is far to precious to hand over to scum like you!");
        sprintf(buf1,"Help! I'm being attacked by %s!",victim->short_descr);
        do_yell(ch,buf1);
        multi_hit(victim,ch,TYPE_UNDEFINED);
        return;
    }

    if (ch->move < obj->level)
    {
        act("$N tells you, 'Hah! You couldn't even get away if i chased you!'.",ch,0,victim,TO_CHAR);
        sprintf(buf1,"Help! I'm being attacked by %s!",victim->short_descr);
        do_yell(ch,buf1);
        multi_hit(victim,ch,TYPE_UNDEFINED);
        return;
    }

    if (ch->hit < (ch->max_hit*3/7))
    {
        do_say(victim,"Hah! You look weak enough that even I could kill you!");
        sprintf(buf1,"Help! I'm being attacked by %s!",victim->short_descr);
        do_yell(ch,buf1);
        multi_hit(victim,ch,TYPE_UNDEFINED);
        return;
    }

    if (ch->mana < 30)
    {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
    }

    if ((obj->wear_loc != WEAR_NONE) && IS_SET(obj->extra_flags, ITEM_NOREMOVE) )
    {
        act("$N tells you, 'I'm unable to release $p'.",ch,obj,victim,TO_CHAR);
        act("$N cowers back from you in fright.",ch,0,victim,TO_CHAR);
        act("$N cowers back from $n in fright.",ch,0,victim,TO_NOTVICT);
        return;
    }
    if (IS_SET(obj->extra_flags,ITEM_NODROP) )
    {
        act("$N tells you, 'I'm unable to release $p'.",ch,obj,victim,TO_CHAR);
        act("$N cowers back from you in fright.",ch,0,victim,TO_CHAR);
        act("$N cowers back from $n in fright.",ch,0,victim,TO_NOTVICT);
        return;
    }

    if ( ch->carry_weight + get_obj_weight(obj)  > can_carry_w(ch) )
    {
        act("$N tells you, 'You can't carry the weight $n.'",ch,0,victim,TO_CHAR);
        return;
    }
    if ( ch->carry_number + 1 > can_carry_n(ch) )
    {
        act("$N tells you, 'You can't carry that many items $n.'",ch,0,victim,TO_CHAR);
        return;
    }

    act("$N caves in to the bully tactics of $n.",ch,0,victim,TO_NOTVICT);
    act("$N shivers in fright and caves in to your bully tactics.",ch,0,victim,TO_CHAR);

    if (obj->wear_loc != WEAR_NONE)
    {
        act("$n stops using $p.",victim,obj,NULL,TO_ROOM);
        act("You stop using $p.",victim,obj,NULL,TO_CHAR);
    }
    act("$N gives $p to $n.",ch,obj,victim,TO_NOTVICT);
    act("$N gives you $p.",ch,obj,victim,TO_CHAR);
    check_improve(ch,gsn_demand,TRUE,2);
    WAIT_STATE(ch,24);
    obj_from_char(obj);
    obj_to_char(obj,ch);
    ch->mana -= 30;
    ch->hit -= 30;
    return;
}

bool can_wield_tertiary(CHAR_DATA *ch,OBJ_DATA *obj,bool fReplace)
{
    OBJ_DATA *primary;
    OBJ_DATA *secondary;
    OBJ_DATA *tertiary;

    primary = get_eq_char(ch,WEAR_WIELD);
    secondary = get_eq_char(ch,WEAR_DUAL_WIELD);
    tertiary = get_eq_char(ch,WEAR_TERTIARY_WIELD);
/*
* This is pretty basic...it's identical to dual_wield slotting code but
* you check for the third hand valid wearing conditions and return FALSE
* if you can't tri-wield. This then automatically sends the weapon into
* normal dual wield slotting process.
*/
    reslot_weapon(ch);  /* verify weapon slots in case of problems */

    if (primary == NULL || secondary == NULL)
        return FALSE;

if (get_skill(ch,gsn_tertiary_wield) < 5)
	return FALSE;
    if (ch->level < skill_table[gsn_tertiary_wield].skill_level[ch->class])
	return FALSE;

    if (hands_full(ch)
	&& (get_eq_char(ch,WEAR_HOLD) != NULL
	|| get_eq_char(ch,WEAR_SHIELD) != NULL
	|| get_eq_char(ch,WEAR_LIGHT) != NULL) )
	return FALSE;

    if ( obj->weight > 60)
	return FALSE;
    if ( obj->weight > secondary->weight)
	return FALSE;

    if (!remove_obj(ch,WEAR_TERTIARY_WIELD,fReplace))
        return FALSE;

    act("$n wields $p in $s changeling hand.",ch,obj,0,TO_ROOM);
    act("You wield $p in your changeling hand.",ch,obj,0,TO_CHAR);
    equip_char(ch,obj,WEAR_TERTIARY_WIELD);

    report_weapon_skill(ch,obj);
    return TRUE;
}

void reslot_weapon(CHAR_DATA *ch)
{
    OBJ_DATA *primary;
    OBJ_DATA *secondary;
    OBJ_DATA *tertiary;
    primary = get_eq_char(ch,WEAR_WIELD);
    secondary = get_eq_char(ch,WEAR_DUAL_WIELD);
    tertiary = get_eq_char(ch,WEAR_TERTIARY_WIELD);

    if (primary == NULL && secondary != NULL)
    {
        unequip_char(ch,secondary);
        equip_char(ch,secondary,WEAR_WIELD);
        primary = secondary;
        secondary = NULL;
    }
    if (secondary == NULL && tertiary != NULL)
    {
        unequip_char(ch,tertiary);
        if (primary == NULL)
        {
            equip_char(ch,tertiary,WEAR_WIELD);
            primary = tertiary;
            tertiary = NULL;
        }
        else
        {
            equip_char(ch,tertiary,WEAR_DUAL_WIELD);
        }
    }
    return;
}

void report_weapon_skill(CHAR_DATA *ch,OBJ_DATA *obj)
{
    int skill, sn;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
        return;
    if (obj->item_type != ITEM_WEAPON)
        {
        sprintf(buf,"report_weapon_skill: Bad obj->type, %d, vnum %d, carried by %s.",
        obj->item_type, obj->pIndexData->vnum,ch->name);
        bug(buf,0);
        return;
        }

    if (obj->value[0] == WEAPON_DAGGER)
        sn = gsn_dagger;
    else if (obj->value[0] == WEAPON_SWORD)
        sn = gsn_sword;
    else if (obj->value[0] == WEAPON_MACE)
        sn = gsn_mace;
    else if (obj->value[0] == WEAPON_SPEAR)
        sn = gsn_spear;
    else if (obj->value[0] == WEAPON_AXE)
        sn = gsn_axe;
    else if (obj->value[0] == WEAPON_FLAIL)
        sn = gsn_flail;
    else if (obj->value[0] == WEAPON_STAFF)
	sn = gsn_staff;
	else
	sn = -1;

    if (sn == -1)
        skill = ch->level * 3;
    else
        skill = ch->pcdata->learned[sn];
    if (skill >= 100)
        act("$p feels like a part of you!",ch,obj,NULL,TO_CHAR);
    else if (skill > 85)
        act("You feel quite confident with $p.",ch,obj,0,TO_CHAR);
    else if (skill > 70)
        act("You are skilled with $p.",ch,obj,0,TO_CHAR);
    else if (skill > 50)
        act("Your skill with $p is adequate.",ch,obj,0,TO_CHAR);
    else if (skill > 25)
        act("$p feels a little clumsy in your hands.",ch,obj,0,TO_CHAR);
    else if (skill > 1)
        act("You fumble and almost drop $p.",ch,obj,0,TO_CHAR);
    else
        act("You don't even know which end is up on $p.",ch,obj,0,TO_CHAR);

return;
}

void wear_obj_gauntlets_striking(CHAR_DATA *ch, OBJ_DATA *obj)
{
   send_to_char("You feel as if you can strike with far deadlier power than before!\n\r",ch);
   return;
}
void remove_obj_gauntlets_striking(CHAR_DATA *ch, OBJ_DATA *obj)
{
   send_to_char("Your hands don't feel quite so powerful anymore.\n\r",ch);
   return;
}

/* In sacrifice for elf blood bracelet */

bool sacrifice_obj_bracelet(CHAR_DATA *ch, OBJ_DATA *obj)
{
        OBJ_DATA *new_obj;
        if (IS_EVIL(ch))
                return FALSE;
act("A white light slowly surrounds $p as the soul of the damned elf is freed.",ch,obj,0,TO_ROOM);
act("A white light slowly surrounds $p as the soul of the damned elf is freed.",ch,obj,0,TO_CHAR);
extract_obj(obj);
        new_obj = create_object(get_obj_index(OBJ_VNUM_HAIR_BRACELET),0);
	if (new_obj->pIndexData->limcount >= new_obj->pIndexData->limtotal)
		return TRUE;

obj_to_char(new_obj,ch);
act("When the glow fades $p has miraculously taken it's place!",ch,new_obj,0,TO_ROOM);
act("When the glow fades $p has miraculously taken it's place!",ch,new_obj,0,TO_CHAR);

        return TRUE;
}


/* tiara & rings of wizardry....do several unseen things:
Rings:
 1. Raises spellcraft percent by 5.
 2. Lowers mana cost of all spells by 10% (20% for high ring)
 3. Reduces magic damage past 200 points by 25%. (After sanc)
Tiara:
 1. Regens mana at 1d3 mana per PULSE_VIOLENCE.
 2. Reduces magic damage past 150 points by 50%. (after sanc)

 Remove these progs if you don't want to use the Drannor areas.
*/
void wear_obj_wizardry(CHAR_DATA *ch, OBJ_DATA *obj)
{
        send_to_char("You attuned to the forces of magic.\n\r",ch);
        return;
}

void remove_obj_wizardry(CHAR_DATA *ch,OBJ_DATA  *obj)
{
send_to_char("You feel less attuned to the forces of magic.\n\r",ch);
return;
}

void wear_obj_tiara(CHAR_DATA *ch,OBJ_DATA *obj)
{
act("$n is surrounded by a blue aura.",ch,0,0,TO_ROOM);
send_to_char("You are surrounded by a blue aura.\n\r",ch);
return;
}

void remove_obj_tiara(CHAR_DATA *ch, OBJ_DATA *obj)
{
send_to_char("The blue aura around you fades.\n\r",ch);
return;
}


void remove_obj_sceptre(CHAR_DATA *ch,OBJ_DATA *obj)
{
        int sn_frenzy;
        sn_frenzy = skill_lookup("heavenly wrath");
        if (is_affected(ch,sn_frenzy))
	{
        send_to_char("The heavenly wrath bestowed upon you fades away.\n\r",ch);
        affect_strip(ch,sn_frenzy);
	}

	return;
}


void obj_brandish_horn_valere(CHAR_DATA *ch,OBJ_DATA *obj)
{
        CHAR_DATA *warrior1;
        CHAR_DATA *warrior2 = NULL;
        CHAR_DATA *warrior3 = NULL;
        CHAR_DATA *bane;
        int num;

        for (bane = ch->in_room->people; bane != NULL; bane = bane->next_in_room)
        {
        if (!IS_NPC(bane))
                continue;
        if (bane->pIndexData->vnum == 13718 && (bane->fighting == ch
        || ch->fighting == bane) )
                break;
        }
        if (!str_cmp(class_table[ch->class].name,"warrior"))
                num = 3;
        else if (!str_cmp(class_table[ch->class].name,"paladin"))
                num = 3;
        else if (!str_cmp(class_table[ch->class].name,"ranger"))
                num = 2;
        else if (!str_cmp(class_table[ch->class].name,"healer"))
                num = 2;
        else if (!str_cmp(class_table[ch->class].name,"monk"))
                num = 3;
        else if (!str_cmp(class_table[ch->class].name,"anti-paladin"))
                num = 0;
        else if (!str_cmp(class_table[ch->class].name,"assassin"))
                num = 0;
        else if (!str_cmp(class_table[ch->class].name,"necromancer"))
                num = 0;
        else
                num = 1;

        if (bane == NULL || IS_NPC(ch) || num == 0)
        {
        act("$n raises $p and blows a resounding call but nothing happens.",ch,obj,0,TO_ROOM);
        act("You raise $p and blow a resounding call but nothing happens.",ch,obj,0,TO_CHAR);
        obj->value[1] -= 1;
        if (obj->value[1] > 0)
                return;
        act("$p crumbles to dust.",ch,obj,0,TO_ROOM);
        act("$p crumbles to dust.",ch,obj,0,TO_CHAR);
        extract_obj(obj);
        return;
        }

        act("$n raises $p and blows a resounding battle call!",ch,obj,0,TO_ROOM);
        act("You raise $p and blow a resounding battle call!",ch,obj,0,TO_CHAR);
        warrior1 = create_mobile(get_mob_index(MOB_VNUM_DRANNOR_KNIGHT));

        if (warrior1 == NULL)
        {
        obj->value[1] -= 1;
        if (obj->value[1] > 0)
                return;
        act("$p crumbles to dust.",ch,obj,0,TO_ROOM);
        act("$p crumbles to dust.",ch,obj,0,TO_CHAR);
        extract_obj(obj);
        return;
        }
        char_to_room(warrior1,ch->in_room);


        if (num >= 2 )
        {
        warrior2 = create_mobile(get_mob_index(MOB_VNUM_DRANNOR_KNIGHT));
        char_to_room(warrior2,ch->in_room);
        }
        if (num >= 3)
        {
        warrior3 = create_mobile(get_mob_index(MOB_VNUM_DRANNOR_KNIGHT));
        char_to_room(warrior3,ch->in_room);
        }

        act("$n appears in a burning halo of light!",warrior1,0,0,TO_ROOM);
        act("$n screams and attacks!",warrior1,0,0,TO_ROOM);
        multi_hit(warrior1,bane,TYPE_UNDEFINED);

	if (warrior2 != NULL)
        {
        act("$n appears in a burning halo of light!",warrior2,0,0,TO_ROOM);
        act("$n screams and attacks!",warrior2,0,0,TO_ROOM);
        multi_hit(warrior2,bane,TYPE_UNDEFINED);
        }

        if (warrior3 != NULL)
        {
        act("$n appears in a burning halo of light!",warrior3,0,0,TO_ROOM);
        act("$n screams and attacks!",warrior3,0,0,TO_ROOM);
        multi_hit(warrior3,bane,TYPE_UNDEFINED);
        }

        obj->value[1] -= 1;
        if (obj->value[1] > 0)
                return;
        act("$p crumbles to dust.",ch,obj,0,TO_ROOM);
        act("$p crumbles to dust.",ch,obj,0,TO_CHAR);
        extract_obj(obj);
        return;
}

void obj_give_hydra_head(CHAR_DATA *ch,CHAR_DATA *lady,OBJ_DATA *obj)
{
    OBJ_DATA *horn;

    if (!IS_NPC(lady))  return;
    if (lady->pIndexData->vnum != MOB_VNUM_DRANNOR_LADY)
        return;
    if (obj->pIndexData->vnum != OBJ_VNUM_HYDRA_HEAD)
        return;
    horn = create_object(get_obj_index(OBJ_VNUM_HORN_VALERE),0);
    if (horn == NULL)
        return;

    obj_to_char(horn,ch);
    extract_obj(obj);
    do_say(lady,"Your worth is proven great adventurer.");
    act("$n gives you $p.",lady,horn,ch,TO_VICT);
    act("$n gives $N $p.",lady,horn,ch,TO_NOTVICT);
    do_say(lady,"Take this artifact in your quest to slay the evil, brandish it when you face darkest evil.");
    act("$n flickers for a moment then fades away.",lady,0,0,TO_ROOM);
    extract_char(lady,TRUE);
    return;
}

void remove_obj_whitehelm(CHAR_DATA *ch, OBJ_DATA *obj)
{
   int sn;

   sn = skill_lookup("black guard");
   if (!is_affected(ch,sn))
	return;
   send_to_char("You once again turn a blind eye to treachery.\n\r", ch);
   affect_strip(ch,sn);
   return;
}

/* robes of zhentil mage */
void wear_obj_zhentil_robe(CHAR_DATA *ch,OBJ_DATA *obj)
{
    AFFECT_DATA af;

    if (IS_NPC(ch))
	return;
    send_to_char("You feel a darkness converge on your soul.\n\r",ch);
    if (!saves_spell(51,ch,DAM_OTHER))
    {
        affect_strip(ch,gsn_blindness);
        init_affect(&af);
        af.where = TO_AFFECTS;
        af.type = gsn_blindness;
        af.aftype    = AFT_SPELL;
        af.duration = -1;
        af.modifier = -3;
        af.location = APPLY_HITROLL;
        af.level = 51;
        af.bitvector = AFF_BLIND;
        affect_to_char(ch,&af);
    act("A veil of darkness seems to cover $n's eyes.",ch,0,0,TO_ROOM);
    send_to_char("You can't see a thing!\n\r",ch);
    }
    return;
}

void wear_obj_fallen_wings(CHAR_DATA *ch,OBJ_DATA *obj)
{
AFFECT_DATA af;

act("$n's shredded wings slowly beat and $s feet rise of the ground.",ch,0,0,TO_ROOM);
send_to_char("Your shredded angel wings slowly beat and your feet rise off the ground.\n\r",ch);
affect_strip(ch,skill_lookup("fly"));

        init_affect(&af);
	af.where = TO_AFFECTS;
af.type = skill_lookup("fly");
        af.aftype    = AFT_SKILL;
af.modifier = 0;
af.location = 0;
af.bitvector = AFF_FLYING;
af.level = 51;
af.duration = -1;
affect_to_char(ch,&af);

return;
}

void remove_obj_fallen_wings(CHAR_DATA *ch,OBJ_DATA *obj)
{
	int sn_fly;
	sn_fly = skill_lookup("fly");
if (!is_affected(ch,sn_fly))
	return;
act("$n slowly floats back to the ground.",ch,0,0,TO_ROOM);
send_to_char("Your feet slowly float back to the ground.\n\r",ch);
affect_strip(ch,sn_fly);
return;
}

void wear_obj_black_hide(CHAR_DATA *ch,OBJ_DATA *obj)
{
send_to_char("You feel more resistant to corrosive gasses and liquids.\n\r",ch);
return;
}

void remove_obj_black_hide(CHAR_DATA *ch,OBJ_DATA *obj)
{
send_to_char("You feel less resistant to acid.\n\r",ch);
return;
}

void wear_obj_symbol_magic(CHAR_DATA *ch,OBJ_DATA *obj)
{
	act("Your $p glows brightly.",ch,obj,NULL,TO_CHAR);
	send_to_char("You feel arcane power flowing through you.\n\r",ch);
	act("$n's $p glows brightly.",ch,obj,NULL,TO_ROOM);

	return;
}

void remove_obj_symbol_magic(CHAR_DATA *ch,OBJ_DATA *obj)
{
send_to_char("You feel less attuned to the powers of magic.\n\r",ch);
return;
}

void wear_obj_red_dragonstar(CHAR_DATA *ch,OBJ_DATA *obj)
{
send_to_char("You feel more resistant to fire.\n\r",ch);
return;
}

void wear_obj_black_dragonstar(CHAR_DATA *ch,OBJ_DATA *obj)
{
send_to_char("You feel more resistant to acid.\n\r",ch);
return;
}

void remove_obj_red_dragonstar(CHAR_DATA *ch,OBJ_DATA *obj)
{
send_to_char("You feel less resistant to fire.\n\r",ch);
return;
}

void remove_obj_black_dragonstar(CHAR_DATA *ch,OBJ_DATA *obj)
{
send_to_char("You feel less resistant to acid.\n\r",ch);
return;
}

void wear_obj_cloak_underworld(CHAR_DATA *ch,OBJ_DATA *obj)
{
	AFFECT_DATA af;
	int sn1;
	sn1 = skill_lookup("infravision");
	if (sn1 > 0)
	{
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn1;
	af.aftype = AFT_SKILL;
	af.modifier = 0;
	af.location = 0;
	af.level = 60;
	af.duration = -1;
	af.bitvector = AFF_INFRARED;
	affect_to_char(ch,&af);
	send_to_char("Your eyes glow red.\n\r",ch);
	}
	send_to_char("You feel immune to poisons and disease.\n\r",ch);
	return;
}

void remove_obj_cloak_underworld(CHAR_DATA *ch,OBJ_DATA *obj)
{
	int sn;
	sn = skill_lookup("infravision");

	if (is_affected(ch,sn))
	{
	send_to_char("You no longer see in the dark.\n\r",ch);
	affect_strip(ch,sn);
	}
	send_to_char("You feel more susceptible to poisons and diseases.\n\r",ch);
	return;
}

/* Libram of infinite conjuration in arkham legion (legion.are) */
void recite_libram_conjuration(CHAR_DATA *ch,CHAR_DATA *victim,OBJ_DATA *scroll,OBJ_DATA *obj, bool fRead)
{
    int sn1, sn2, sn3, sn;
    char *spell;
    sn1 = -1;
    sn2 = -1;
    sn3 = -1;

	if (fRead)
	{
        obj_cast_spell( scroll->value[1], scroll->value[0], ch, victim, obj );
        obj_cast_spell( scroll->value[2], scroll->value[0], ch, victim, obj );
        obj_cast_spell( scroll->value[3], scroll->value[0], ch, victim, obj );
        check_improve(ch,gsn_scrolls,TRUE,5);
	}

    if (number_percent() < 5)
    {
        act("$n's libram of infinite conjuration glows brightly and vanishes!", ch,0, 0, TO_ROOM);
        act("Your libram of infinite conjuration glows brightly then vanishes!",ch,0,0,TO_CHAR);
        extract_obj(scroll);
        return;
    }
    else
    {
    scroll->value[0] = number_range(44,56);
    scroll->value[1] = -1;
    scroll->value[2] = -1;
    scroll->value[3] = -1;
    for (; ;)
    {
        if (sn3 != -1)
            break;
        switch(number_range(0,53))
        {
            case 1: spell = "armor";    break;
            case 2: spell = "shield"; break;
            case 3: spell = "bless"; break;
            case 4: spell = "protection good"; break;
            case 5: spell = "protection evil"; break;
            case 6: spell = "protective shield"; break;
            case 7: spell = "detect invis"; break;
            case 8: spell = "detect evil"; break;
            case 9: spell = "detect magic"; break;
            case 10: spell = "fireball"; break;
            case 11: spell = "iceball"; break;
            case 12: spell = "cone of cold"; break;
            case 13: spell = "power word kill"; break;
            case 14: spell = "curse"; break;
            case 15: spell = "blindness"; break;
            case 16: spell = "cure blind"; break;
            case 17: spell = "poison"; break;
            case 18: spell = "cure poison"; break;
            case 19: spell = "plague"; break;
            case 20: spell = "cure disease"; break;
            case 21: spell = "word of recall"; break;
            case 22: spell = "haste"; break;
            case 23: spell = "slow"; break;
            case 24: spell = "absorb"; break;
            case 25: spell = "chain lightning"; break;
            case 26: spell = "lightning bolt"; break;
            case 27: spell = "magic missile"; break;
            case 28: spell = "acid blast"; break;
            case 29: spell = "cremate"; break;
            case 30: spell = "frostbolt"; break;
            case 31: spell = "grounding"; break;
            case 32: spell = "fly"; break;
            case 33: spell = "passdoor"; break;
            case 34: spell = "chill touch"; break;
            case 35: spell = "create spring"; break;
            case 36: spell = "pox"; break;
            case 37: spell = "charm"; break;
            case 38: spell = "energy drain"; break;
            case 39: spell = "teleport"; break;
            case 40: spell = "deathspell"; break;
            case 41: spell = "enchant weapon"; break;
            case 42: spell = "enchant armor"; break;
            case 43: spell = "invis"; break;
            case 44: spell = "mass invis"; break;
            case 45: spell = "timestop"; break;
            case 46: spell = "earthquake"; break;
            case 47: spell = "firestream"; break;
            case 48: spell = "sanctuary"; break;
            case 49: spell = "forget"; break;
            case 50: spell = "concatenate"; break;
            case 51: spell = "harm"; break;
            case 52: spell = "heal"; break;
            default:    spell = "detect invis"; break;
        }

        sn = skill_lookup(spell);
            if (sn <= 0)
                    continue;
        if (sn1 == -1)
            {
            sn1 = sn;
            scroll->value[1] = sn1;
            if (number_percent() < 75)
                break;
            }
        else if (sn2 == -1)
            {
            sn2 = sn;
            scroll->value[2] = sn2;
            if (number_percent() < 85)
                break;
            }
        else if (sn3 == -1)
            {
             sn3 = sn;
             scroll->value[3] = sn3;
             break;
            }
        }
        }

send_to_char("Your libram of infinite conjuration glows softly and new words appear on the page.\n\r",ch);

        return;
}

void brandish_sceptre_dead(CHAR_DATA *ch,OBJ_DATA *obj)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    int sn_stun, chance;
    AFFECT_DATA af;

    sn_stun = skill_lookup("power word stun");
    if (sn_stun > 0)
    {
        init_affect(&af);
    af.where = TO_AFFECTS;
    af.type = sn_stun;
        af.aftype    = AFT_SPELL;
    af.location = APPLY_DEX;
    af.modifier = -5;
    af.duration = 6;
    af.bitvector = 0;
   af.level = ch->level;
    }

    for (victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
	v_next = victim->next_in_room;
	if (!IS_NPC(victim))
		continue;
	if (!IS_SET(victim->act,ACT_UNDEAD))
		continue;
	chance = 65 + (ch->level - victim->level) * 3;
	if (number_percent() > chance)
	{
act("$n's eyes flicker as the sceptre draws their attention, but then they return to normal.",victim,0,0,TO_ROOM);
	 continue;
	}
	act("$n is transfixed by the power of the unholy sceptre of the dead and stops still.",victim,0,0,TO_ROOM);
	act("You are transfixed by the unholy power of the sceptre of dead.",victim,0,0,TO_CHAR);
	stop_fighting(victim,TRUE);
	if (ch->fighting == victim)
 		stop_fighting(ch,TRUE);
	if (sn_stun <= 0)
 	continue;
	if (!is_affected(victim,sn_stun) && !saves_spell(ch->level,victim,DAM_OTHER))
		affect_to_char(victim,&af);

    }
    return;
}

void wear_obj_misty_cloak(CHAR_DATA *ch,OBJ_DATA *obj)
{
	int sn;
	AFFECT_DATA af;

	sn = skill_lookup("pass door");
	if (sn <= 0)
	return;

	if (is_affected(ch,sn))
	affect_strip(ch,sn);
	if (IS_AFFECTED(ch,AFF_PASS_DOOR)
	|| !str_cmp(race_table[ch->race].name, "ethereal"))
		return;

        init_affect(&af);
	af.where = TO_AFFECTS;
        af.aftype    = AFT_SPELL;
	af.modifier = 0;
	af.location = 0;
	af.duration = -1;
	af.level = 60;
	af.bitvector = AFF_PASS_DOOR;
	af.type = sn;
	affect_to_char(ch,&af);

	send_to_char("You turn transulcent.\n\r",ch);
	return;
}

void remove_obj_misty_cloak(CHAR_DATA *ch,OBJ_DATA *obj)
{
	int sn;
	sn = skill_lookup("pass door");
	if (sn <= 0)
	return;
	if (!is_affected(ch,sn))
	return;
	send_to_char("You become more solid again.\n\r",ch);
	affect_strip(ch,sn);
return;
}

void save_cabal_items(void)
{
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	CHAR_DATA *ch=0;
        FILE *fp;
	int obj_loc_num[] = { 2, 2, 2, 2, 2, 2, 2, 2 };
	int i;

	for (obj = object_list; obj != NULL; obj = obj_next)
	{
	obj_next = obj->next;
	if (obj->pIndexData->vnum == OBJ_VNUM_ACADIAN) {
		if (obj->carried_by!=NULL)
			ch=obj->carried_by;
		if (ch!=NULL && IS_NPC(ch))
				obj_loc_num[0]=ch->pIndexData->vnum;
		}
	if (obj->pIndexData->vnum == OBJ_VNUM_COVEN) {
		if (obj->carried_by!=NULL)
			ch=obj->carried_by;
		if (ch!=NULL && IS_NPC(ch))
				obj_loc_num[1]=ch->pIndexData->vnum;
		}
	if (obj->pIndexData->vnum == OBJ_VNUM_PROTECTOR) {
		if (obj->carried_by!=NULL)
			ch=obj->carried_by;
		if (ch!=NULL && IS_NPC(ch))
				obj_loc_num[2]=ch->pIndexData->vnum;
		}
	if (obj->pIndexData->vnum == OBJ_VNUM_RAVAGER) {
		if (obj->carried_by!=NULL)
			ch=obj->carried_by;
		if (ch!=NULL && IS_NPC(ch))
				obj_loc_num[3]=ch->pIndexData->vnum;
		}
	if (obj->pIndexData->vnum == OBJ_VNUM_VIGILANTE) {
		if (obj->carried_by!=NULL)
			ch=obj->carried_by;
		if (ch!=NULL && IS_NPC(ch))
				obj_loc_num[4]=ch->pIndexData->vnum;
		}
    if (obj->pIndexData->vnum == OBJ_VNUM_BRIAR) {
                if (obj->carried_by!=NULL)
                        ch=obj->carried_by;
                if (ch!=NULL && IS_NPC(ch))
                                obj_loc_num[5]=ch->pIndexData->vnum;
                }

	}
    if ( ( fp = fopen( CABAL_ITEMS_FILE, "w" ) ) == NULL )
    {
	perror( CABAL_ITEMS_FILE );
    }
    else
    {
	for(i=0;i<8;i++)
		fprintf(fp,"%i\n",obj_loc_num[i]);
	fclose(fp);
    }
}

void do_clown_mallet(CHAR_DATA *ch)
{
        OBJ_DATA *mallet;
        int bonus,dice,number;
        int ch_level,  sn;
        AFFECT_DATA af;

    if (get_skill(ch,gsn_clown_mallet) == 0)
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

        mallet = create_object(get_obj_index(OBJ_VNUM_CLOWN_MALLET),0);
        ch_level = ch->level;
	sn = gsn_clown_mallet;

        if (number_percent() < 50)
                ch_level -= (number_range(0,3));
        else
                ch_level += (number_range(0,2));

        if (ch_level < 32)
        {
        dice = 5; number = 6;
        }
        else if (ch_level < 34)
        {
        dice = 3; number = 11;
        }
        else if (ch_level < 36)
        {
        number = 5; dice = 8;
        }
        else if (ch_level < 38)
        {
        number = 6; dice = 7;
        }
        else if (ch_level < 40)
        {
        number = 6; dice = 7;
        }
        else if (ch_level < 42)
        {
        number = 5; dice = 9;
        }
        else if (ch_level < 44)
        {
        number = 5;     dice = 9;
        }
        else if (ch_level < 46)
        {
        number = 5;     dice = 9;
        }
        else if (ch_level < 47)
        {
        number = 13;    dice = 3;
        }
        else if (ch_level < 48)
        {
        number = 6;     dice = 8;
        }
        else if (ch_level < 49)
        {
        number = 8;     dice = 6;
        }
        else if (ch_level < 50)
        {
        number = 8;     dice = 6;
        }
        else
        {
        number = 6;     dice = 9;
        }

        bonus = ch->level/4;
        init_affect(&af);
        af.where = TO_OBJECT;
        af.type = sn;
        af.aftype = AFT_SKILL;
        af.level = ch->level;
        af.duration = -1;
        af.location = APPLY_HITROLL;
        af.modifier = bonus;
        af.bitvector = 0;
        affect_to_obj(mallet,&af);
        af.location = APPLY_DAMROLL;
        affect_to_obj(mallet,&af);
        mallet->level = ch->level;
	mallet->timer = (20 + number_range(0,10));
        mallet->value[1] = number;
        mallet->value[2] = dice;
		mallet->owner = strdup(ch->name);
        obj_to_char(mallet,ch);
        act("$n produces $p from behind $s back!",ch,mallet,0,TO_ROOM);
        act("You produce $p!",ch,mallet,0,TO_CHAR);
        return;
 }

void do_ranger_staff(CHAR_DATA *ch)
{
        OBJ_DATA *staff;
        int bonus,dice,number;
        int ch_level,  sn;
        AFFECT_DATA af;

    if (get_skill(ch,gsn_ranger_staff) == 0)
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

        staff = create_object(get_obj_index(OBJ_VNUM_RANGER_STAFF),0);
        ch_level = ch->level;
	sn = gsn_ranger_staff;

        if (number_percent() < 50)
                ch_level -= (number_range(0,3));
        else
                ch_level += (number_range(0,2));

        if (ch_level < 32)
        {
        dice = 5; number = 6;
        }
        else if (ch_level < 34)
        {
        dice = 3; number = 11;
        }
        else if (ch_level < 36)
        {
        number = 5; dice = 8;
        }
        else if (ch_level < 38)
        {
        number = 6; dice = 7;
        }
        else if (ch_level < 40)
        {
        number = 6; dice = 7;
        }
        else if (ch_level < 42)
        {
        number = 5; dice = 9;
        }
        else if (ch_level < 44)
        {
        number = 5;     dice = 9;
        }
        else if (ch_level < 46)
        {
        number = 5;     dice = 9;
        }
        else if (ch_level < 47)
        {
        number = 13;    dice = 3;
        }
        else if (ch_level < 48)
        {
        number = 6;     dice = 8;
        }
        else if (ch_level < 49)
        {
        number = 8;     dice = 6;
        }
        else if (ch_level < 50)
        {
        number = 8;     dice = 6;
        }
        else
        {
        number = 6;     dice = 9;
        }

        bonus = ch->level/4;
        init_affect(&af);
        af.where = TO_OBJECT;
        af.aftype    = AFT_SKILL;
        af.type = sn;
        af.level = ch->level;
        af.duration = -1;
        af.location = APPLY_HITROLL;
        af.modifier = bonus;
        af.bitvector = 0;
        affect_to_obj(staff,&af);
        af.location = APPLY_DAMROLL;
        affect_to_obj(staff,&af);
        staff->level = ch->level;
/*	staff->timer = (20 + number_range(0,10)); */
        staff->value[1] = number;
        staff->value[2] = dice;
        obj_to_char(staff,ch);
        act("$n fashions $p!",ch,staff,0,TO_ROOM);
        act("You fashion $p!",ch,staff,0,TO_CHAR);
        return;
}

void obj_brandish_phylactery(CHAR_DATA *ch,OBJ_DATA *obj)
{
	WAIT_STATE(ch,PULSE_VIOLENCE);
	act("$n invokes $p.", ch, obj, NULL, TO_ROOM);
	act("You invoke $p.",  ch, obj, NULL, TO_CHAR);
	obj_cast_spell(obj->value[3], obj->value[0], ch, NULL, NULL);
	return;
}

