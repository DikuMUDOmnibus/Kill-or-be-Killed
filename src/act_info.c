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
DECLARE_DO_FUN(	do_exits	);
DECLARE_DO_FUN( do_look		);
DECLARE_DO_FUN( do_help		);
DECLARE_DO_FUN( do_affects	);
DECLARE_DO_FUN( do_play		);
DECLARE_DO_FUN( do_revert	);
DECLARE_DO_FUN( do_tell		);
DECLARE_DO_FUN( do_dismount );

#define DEBUG_LOG_FILE	"debug.txt"

char *	const	where_name	[] =
{
    "<used as light>     ",
    "<worn on finger>    ",
    "<worn on finger>    ",
    "<worn around neck>  ",
    "<worn around neck>  ",
    "<worn on torso>     ",
    "<worn on head>      ",
    "<worn on legs>      ",
    "<worn on feet>      ",
    "<worn on hands>     ",
    "<worn on arms>      ",
    "<worn as shield>    ",
    "<worn on body>      ",
    "<worn on waist>     ",
    "<worn around wrist> ",
    "<worn around wrist> ",
    "<wielded>           ",
    "<held>              ",
    "<dual wielded>      ",
    "<trinal wielded>    ",
    "<branded>           ",
    "<bound by>          ",
};


/* for do_count */
int max_on = 0;

extern bool isShifted(CHAR_DATA *ch);

/*
 * Local functions.
 */
char *	format_obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch,
				    bool fShort ) );
void	show_list_to_char	args( ( OBJ_DATA *list, CHAR_DATA *ch,
				    bool fShort, bool fShowNothing ) );
void	show_char_to_char_0	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char_1	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char_2 args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char	args( ( CHAR_DATA *list, CHAR_DATA *ch ) );
bool	check_blind		args( ( CHAR_DATA *ch ) );
void showDesc(CHAR_DATA *victim, CHAR_DATA *ch);
void showHealth(CHAR_DATA *victim, CHAR_DATA *ch);
void showEq(CHAR_DATA *victim, CHAR_DATA *ch);
void showInventory(CHAR_DATA *victim, CHAR_DATA *ch);

bool    isNewbie		args( ( CHAR_DATA *ch) );

char * weapon_name_lookup args((int type));

char *format_obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch, bool fShort )
{
    static char buf[MAX_STRING_LENGTH];

    buf[0] = '\0';

    if ((fShort && (obj->short_descr == NULL || obj->short_descr[0] == '\0'))
    ||  (obj->description == NULL || obj->description[0] == '\0'))
	return buf;

    if ( IS_OBJ_STAT(obj, ITEM_INVIS)     )   strcat( buf, "(Invis) "     );
    if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && IS_OBJ_STAT(obj, ITEM_EVIL)   )   strcat( buf, "(Red Aura) "  );
    if (IS_AFFECTED(ch, AFF_DETECT_GOOD)
    &&  IS_OBJ_STAT(obj,ITEM_BLESS))	      strcat( buf,"(Blue Aura) "	);
    if ( IS_AFFECTED(ch, AFF_DETECT_MAGIC)
         && IS_OBJ_STAT(obj, ITEM_MAGIC)  )   strcat( buf, "(Magical) "   );
    if ( IS_OBJ_STAT(obj, ITEM_GLOW)      )   strcat( buf, "(Glowing) "   );
    if ( IS_OBJ_STAT(obj, ITEM_HUM)       )   strcat( buf, "(Humming) "   );

    if ( fShort )
    {
	if ( obj->short_descr != NULL )
	    strcat( buf, obj->short_descr );
    }
    else
    {
	if ( obj->description != NULL)
	    strcat( buf, obj->description );
    }

    return buf;
}



/*
 * Show a list to a character.
 * Can coalesce duplicated items.
 */
void show_list_to_char( OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing )
{
    char buf[MAX_STRING_LENGTH];
    BUFFER *output;
    char **prgpstrShow;
    int *prgnShow;
    char *pstrShow;
    OBJ_DATA *obj;
    int nShow;
    int iShow;
    int count;
    int line;
    bool fCombine;
	char obj_vnum[MSL];
	char *obj_buf;

    if ( ch->desc == NULL )
	return;

    /*
     * Alloc space for output lines.
     */
    output = new_buf();
    line = ch->lines;
    ch->lines = 0;
    count = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
	count++;
    prgpstrShow	= alloc_mem( count * sizeof(char *) * 9 );
    prgnShow    = alloc_mem( count * sizeof(int)    );
    nShow	= 0;

    /*
     * Format the list of objects.
     */
    for ( obj = list; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ))
	{
		obj_buf = format_obj_to_char(obj,ch,fShort);

		if ( (IS_NPC(ch)) || (IS_SET(ch->act,PLR_HOLYLIGHT)) )
		{
			sprintf(obj_vnum,"%s  [{W%5d{x]",obj_buf,obj->pIndexData->vnum);
		}
		else
		{
			sprintf(obj_vnum,"%s",format_obj_to_char(obj,ch,fShort));
		}

		pstrShow = obj_vnum;

	    fCombine = FALSE;

	    if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )
	    {
		/*
		 * Look for duplicates, case sensitive.
		 * Matches tend to be near end so run loop backwords.
		 */
		for ( iShow = nShow - 1; iShow >= 0; iShow-- )
		{
		    if ( !strcmp( prgpstrShow[iShow], pstrShow ) )
		    {
			prgnShow[iShow]++;
			fCombine = TRUE;
			break;
		    }
		}
	    }

	    /*
	     * Couldn't combine, or didn't want to.
	     */
	    if ( !fCombine )
	    {
		prgpstrShow [nShow] = str_dup( pstrShow );
		prgnShow    [nShow] = 1;
		nShow++;
	    }
	}

    }

    /*
     * Output the formatted list.
     */
    for ( iShow = 0; iShow < nShow; iShow++ )
    {
	if (prgpstrShow[iShow][0] == '\0')
	{
	    free_string(prgpstrShow[iShow]);
	    continue;
	}

	if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )
	{
	    if ( prgnShow[iShow] != 1 )
	    {
		sprintf( buf, "(%2d) ", prgnShow[iShow] );
		add_buf(output,buf);
	    }
	    else
	    {
		add_buf(output,"     ");
	    }
	}
	add_buf(output,prgpstrShow[iShow]);
	add_buf(output,"\n\r");
	free_string( prgpstrShow[iShow] );
    }

    if ( fShowNothing && nShow == 0 )
    {
	if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )
	    send_to_char( "     ", ch );
	send_to_char( "Nothing.\n\r", ch );
    }
    page_to_char(buf_string(output),ch);

    /*
     * Clean up.
     */
    free_buf(output);
    free_mem( prgpstrShow, count * sizeof(char *) );
    free_mem( prgnShow,    count * sizeof(int)    );
    ch->lines = line;
    return;
}



void show_char_to_char_0( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH],message[MAX_STRING_LENGTH];
   int sn_faerie_fog;
    OBJ_DATA *obj;
    buf[0] = '\0';

    obj = get_eq_char(victim,WEAR_HEAD);

    sn_faerie_fog = skill_lookup("faerie fog");
    if ( IS_SET(victim->comm,COMM_AFK	  )   ) strcat( buf, "{W[{BAFK{W]{x "	     );
    if ( IS_SET(victim->comm,COMM_OLCEDIT )   ) strcat( buf, "{C[{WOLC Editing{C]{x "	);
    if ( !IS_NPC(victim))
	{
	if (victim->pcdata->death_status == HAS_DIED)
		strcat(buf,"(GHOST) ");
	}
    if (isNewbie(victim))
		strcat(buf,"(NEWBIE) ");
    if ( IS_AFFECTED(victim, AFF_INVISIBLE)   ) strcat( buf, "(Invis) "      );
    if ( victim->invis_level > LEVEL_HERO    ) strcat( buf, "(Wizi) "	     );
    if ( is_affected(victim,gsn_earthfade)	) strcat(buf,"(Earthfade) " );
    if ( IS_AFFECTED(victim, AFF_HIDE)        ) strcat( buf, "(Hide) "       );
    if ( IS_AFFECTED(victim, AFF_CAMOUFLAGE)  ) strcat( buf, "(Camouflage) " );
    if (is_affected(victim,gsn_forest_blending)) strcat(buf,"(Blending) " );
    if ( IS_AFFECTED(victim, AFF_CHARM)       ) strcat( buf, "(Charmed) "    );
    if ( IS_AFFECTED(victim, AFF_PASS_DOOR)   ) strcat( buf, "(Translucent) ");
    if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) ) strcat( buf, "(Pink Aura) "  );
    if ( is_affected(victim, gsn_incandescense)) strcat( buf, "(Glowing) "    );
    if (is_affected(victim,sn_faerie_fog) )	strcat( buf, "(Purple Aura) "	);
    if (victim->ghost > 0)			strcat( buf, "(Ghost) ");
    if ( IS_EVIL(victim)
    &&   IS_AFFECTED(ch, AFF_DETECT_EVIL)     ) strcat( buf, "(Red Aura) "   );
    if ( IS_GOOD(victim)
    &&   IS_AFFECTED(ch, AFF_DETECT_GOOD)     ) strcat( buf, "(Golden Aura) ");
if ( IS_AFFECTED(victim, AFF_SANCTUARY) && !is_affected(victim,gsn_shroud)  ) strcat( buf, "(White Aura) " );
    if (is_affected(victim,gsn_fireshield))  strcat( buf, "(Surrounded by Flames) " );
    if (is_affected(victim,gsn_wraithform))  strcat( buf, "(Ghostly) " );
    if (is_affected(victim,gsn_shroud))  strcat( buf, "(Black Aura) " );
    if (is_affected(victim,gsn_kinetic_shield))  strcat( buf, "(Crackling with Energy) " );
    if (is_affected(victim,gsn_body_of_wood)) strcat (buf, "(Body of Wood) ");
    if (is_affected(victim,gsn_body_of_clay)) strcat (buf, "(Body of Clay) ");
    if (is_affected(victim,gsn_body_of_iron)) strcat (buf, "(Body of Iron) ");
    if (is_affected(victim,gsn_body_of_steel)) strcat (buf, "(Body of Steel) ");
    if (is_affected(victim,gsn_body_of_stone)) strcat (buf, "(Body of Stone) ");
    if (is_affected(victim,gsn_body_of_diamond)) strcat (buf, "(Body of Diamond) ");
    if (is_affected(victim,gsn_self_immolation)) strcat (buf, "(Burning) ");
    if (is_affected(victim,gsn_static_charge)) strcat (buf, "(Glowing Softly) ");
    if (is_affected(victim,gsn_snare))  strcat( buf, "(Ensnared) " );
    if (is_affected(victim,gsn_maehslin)) strcat (buf, "(Amber Aura) ");
    if (is_affected(victim,gsn_dew)) strcat (buf, "(Covered in Dew) ");
    if (is_affected(victim,gsn_duo)) strcat (buf, "(Duo-Dimension) ");
	if (is_affected(victim,gsn_dark_blessing)) strcat (buf, "(Corrupted) ");

 if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_KILLER ) )
						strcat( buf, "(KILLER) "     );
    if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_THIEF  ) )
						strcat( buf, "(THIEF) "      );
    if ( !IS_NPC(victim) && IS_SET(victim->act, PLR_CRIMINAL ) )
                                                strcat( buf, "(WANTED) "   );

	/*if ( !IS_NPC(victim) && is_affected(victim,gsn_cloak_form) && ch->level > LEVEL_HERO )
	{
		strcat( buf, "[");
		strcat( buf, victim->original_name);
		strcat( buf, "] ");
	}

    if 	(
	isShifted(victim) &&
	victim->position == victim->start_pos &&
	victim->position == POS_STANDING &&
	!IS_NPC(victim)
	)
    {

	if ( IS_IMMORTAL(ch))
	{
		strcat(buf, "[");
		strcat(buf, victim->original_name);
		strcat(buf, "] ");
	}
	strcat(buf, victim->long_descr);
	send_to_char(buf, ch);
	return;

    }

    if (victim->position == victim->start_pos
        && victim->long_descr[0] != '\0' && IS_NPC(victim))
    {
        strcat (buf, victim->long_descr);
	send_to_char( buf, ch );
	return;
    }

	strcat (buf, PERS (victim, ch));
	if (!IS_NPC (victim) && !IS_SET (ch->comm, COMM_BRIEF)
	&& victim->position == POS_STANDING && ch->on == NULL)
	strcat( buf, victim->pcdata->title );*/

	if(IS_NPC(victim) || isShifted(victim) || (str_cmp(victim->long_descr,"") && victim->long_descr[0]!='\0'))
    {
	strcat(buf,victim->long_descr);
    	send_to_char(buf,ch);
    	return;
    }
    strcat(buf, PERS (victim,ch));
    if ( !IS_NPC(victim) && !IS_SET(ch->comm, COMM_BRIEF)
	&& victim->position == POS_STANDING && ch->on == NULL)
	strcat( buf, victim->pcdata->title );

	if (IS_SET(victim->comm, COMM_MOUNTED))
	{
		strcat( buf, " is riding upon a mount." );
		strcat( buf, "\n\r" );
		buf[0] = UPPER(buf[0]);
		send_to_char(buf,ch);
		return;
	}

	switch ( victim->position )
    {
    case POS_DEAD:     strcat( buf, " is DEAD!!" );              break;
    case POS_MORTAL:   strcat( buf, " is mortally wounded." );   break;
    case POS_INCAP:    strcat( buf, " is incapacitated." );      break;
    case POS_STUNNED:  strcat( buf, " is lying here stunned." ); break;
    case POS_SLEEPING:
	if (victim->on != NULL)
	{
	    if (IS_SET(victim->on->value[2],SLEEP_AT))
  	    {
		sprintf(message," is sleeping at %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	    else if (IS_SET(victim->on->value[2],SLEEP_ON))
	    {
		sprintf(message," is sleeping on %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	    else
	    {
		sprintf(message, " is sleeping in %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	}
	else
	    strcat(buf," is sleeping here.");
	break;
    case POS_RESTING:
        if (victim->on != NULL)
	{
            if (IS_SET(victim->on->value[2],REST_AT))
            {
                sprintf(message," is resting at %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
            else if (IS_SET(victim->on->value[2],REST_ON))
            {
                sprintf(message," is resting on %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
            else
            {
                sprintf(message, " is resting in %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
	}
        else
	    strcat( buf, " is resting here." );
	break;
    case POS_SITTING:
        if (victim->on != NULL)
        {
            if (IS_SET(victim->on->value[2],SIT_AT))
            {
                sprintf(message," is sitting at %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
            else if (IS_SET(victim->on->value[2],SIT_ON))
            {
                sprintf(message," is sitting on %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
            else
            {
                sprintf(message, " is sitting in %s.",
                    victim->on->short_descr);
                strcat(buf,message);
            }
        }
        else
	    strcat(buf, " is sitting here.");
	break;
    case POS_STANDING:
	if (victim->on != NULL)
	{
	    if (IS_SET(victim->on->value[2],STAND_AT))
	    {
		sprintf(message," is standing at %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	    else if (IS_SET(victim->on->value[2],STAND_ON))
	    {
		sprintf(message," is standing on %s.",
		   victim->on->short_descr);
		strcat(buf,message);
	    }
	    else
	    {
		sprintf(message," is standing in %s.",
		    victim->on->short_descr);
		strcat(buf,message);
	    }
	}
	else
	    strcat( buf, " is here." );
	break;
    case POS_FIGHTING:
	strcat( buf, " is here, fighting " );
	if ( victim->fighting == NULL )
	    strcat( buf, "thin air??" );
	else if ( victim->fighting == ch )
	    strcat( buf, "YOU!" );
	else if ( victim->in_room == victim->fighting->in_room )
	{
	    strcat( buf, PERS( victim->fighting, ch ) );
	    strcat( buf, "." );
	}
	else
	    strcat( buf, "someone who left??" );
	break;
    }

    strcat( buf, "\n\r" );
    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );
    return;
}



void show_char_to_char_1( CHAR_DATA *victim, CHAR_DATA *ch )
{
     if ( can_see( victim, ch ) )
    {
	if (ch == victim)
	    act( "$n looks at $mself.",ch,NULL,NULL,TO_ROOM);
	else
	{
	    act( "$n looks at you.", ch, NULL, victim, TO_VICT    );
	    act( "$n looks at $N.",  ch, NULL, victim, TO_NOTVICT );
	}
    }

    showDesc(victim, ch);

    showHealth(victim, ch);

    showEq(victim, ch);

    showInventory(victim, ch);

    return;
}

void show_char_to_char_2( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
	char obj_vnum[MSL];
    int iWear;
    int percent;
    bool found;

    if ( can_see( victim, ch ) )
    {
	if (ch == victim)
	    act( "$n glances at $mself.",ch,NULL,NULL,TO_ROOM);
	else
	{
	    act( "$n glances at you.", ch, NULL, victim, TO_VICT    );
	    act( "$n glances at $N.",  ch, NULL, victim, TO_NOTVICT );
	}
    }

    if ( victim->max_hit > 0 )
	percent = ( 100 * victim->hit ) / victim->max_hit;
    else
	percent = -1;

    strcpy( buf, victim->name );

    if (IS_SET(ch->act,PLR_EVALUATION))
    {
        if (percent >= 100)
          strcat(buf," is in perfect condition.\n\r");
        else if (percent >= 95)
            strcat(buf," has a few scratches.\n\r");
        else if (percent >= 90)
            strcat(buf," has a few bruises.\n\r");
        else if (percent >= 80)
              strcat(buf," has some small wounds.\n\r");
       else if (percent >= 70)
            strcat(buf," has quite a few wounds.\n\r");
       else if (percent >= 60)
		strcat(buf," is covered in bleeding wounds.\n\r");
        else if (percent >= 50)
            strcat(buf," is bleeding profusely.\n\r");
        else if (percent >= 40)
            strcat(buf," is gushing blood.\n\r");
        else if (percent >= 30)
            strcat(buf," is screaming in pain.\n\r");
        else if (percent >= 20)
            strcat(buf," is spasming in shock.\n\r");
        else if (percent >= 10)
            strcat(buf," is writhing in agony.\n\r");
        else if (percent >= 1)
            strcat(buf," is convulsing on the ground.\n\r");
        else
            strcat(buf," is nearly dead.\n\r");
    }
    else
    {
    if (percent >= 100)
        strcat( buf, " is in perfect condition.\n\r");
    else if (percent >= 90)
	strcat( buf, " has a few scratches.\n\r");
    else if (percent >= 75)
        strcat( buf, " has some small wounds.\n\r");
    else if (percent >=  50)
        strcat( buf, " has some nasty cuts.\n\r");
    else if (percent >= 30)
        strcat( buf, " is bleeding profusely.\n\r");
    else if (percent >= 15)
        strcat( buf, " is screaming in pain.\n\r");
    else if (percent >= 0 )
        strcat( buf, " is in pretty bad shape.\n\r");
    else
        strcat(buf, " is nearly dead.\n\r");
    }

    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );

    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	if ( ( obj = get_eq_char( victim, iWear ) ) != NULL
	&&   can_see_obj( ch, obj ) )
	{
	    if ( !found )
	    {
		send_to_char( "\n\r", ch );
		act( "$N is using:", ch, NULL, victim, TO_CHAR );
		found = TRUE;
	    }
	    send_to_char( where_name[iWear], ch );
	    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
		if (IS_IMMORTAL(ch) && IS_SET(ch->act,PLR_HOLYLIGHT))
		{
   			sprintf(obj_vnum,"{x  [{W%5d{x]",obj->pIndexData->vnum);
			send_to_char(obj_vnum,ch);
		}
	    send_to_char( "\n\r", ch );
	}
    }

    if ( victim != ch
    &&   !IS_NPC(ch)
    && (IS_IMMORTAL(ch) || !IS_IMMORTAL(victim))
    &&   number_percent( ) < get_skill(ch,gsn_peek))
    {
	send_to_char( "\n\rYou peek at the inventory:\n\r", ch );
	check_improve(ch,gsn_peek,TRUE,4);
	show_list_to_char( victim->carrying, ch, TRUE, TRUE );
    }

    return;
}

/*
void show_char_to_char( CHAR_DATA *list, CHAR_DATA *ch )
{
    CHAR_DATA *rch;

    for ( rch = list; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch == ch )
	    continue;

	if ( get_trust(ch) < rch->invis_level)
	    continue;

        if (can_see (ch, rch))
	{
	    show_char_to_char_0( rch, ch );
	}
	else if ( room_is_dark( ch->in_room )
	&&        IS_AFFECTED(rch, AFF_INFRARED ) )
	{
	    send_to_char( "You see glowing red eyes watching YOU!\n\r", ch );
	}
    }

    return;
}
*/

void show_char_to_char( CHAR_DATA *list, CHAR_DATA *ch )
{
    CHAR_DATA *rch;
    int light,i;
    OBJ_DATA *obj;
    bool cant_see = FALSE;

    if (!IS_NPC(ch)
    && IS_SET(ch->in_room->extra_room_flags,ROOM_GLOBE_DARKNESS)
    && str_cmp(pc_race_table[ch->race].name,"dark-elf") )
    {
	light = 0;
	for (i = 0; i < MAX_WEAR; i++)
	{
	obj = get_eq_char(ch,i);
	if (obj == NULL)
		continue;
	if (IS_SET(obj->extra_flags,ITEM_GLOW))
		light++;
	if (obj->item_type == ITEM_LIGHT)
		light += 3;
	}
	if (light < 8)
	{
	cant_see = TRUE;
    	}
    }

    for ( rch = list; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch == ch )
	    continue;

	if ( get_trust(ch) < rch->invis_level)
	    continue;

	if ((cant_see)
	&&        IS_AFFECTED(rch, AFF_INFRARED ) )
	{
	    send_to_char( "You see glowing red eyes watching YOU!\n\r", ch );
	}
	else if ( (cant_see))
		continue;
	else if ( can_see( ch, rch ))
	{
	    show_char_to_char_0( rch, ch );
	}
	else if ( room_is_dark( ch->in_room )
	&&        IS_AFFECTED(rch, AFF_INFRARED ) )
	{
	    send_to_char( "You see glowing red eyes watching YOU!\n\r", ch );
	}
    }

    return;
}

bool check_blind( CHAR_DATA *ch )
{

    if (!IS_NPC(ch) && IS_SET(ch->act,PLR_HOLYLIGHT))
	return TRUE;

    if ( IS_AFFECTED(ch, AFF_BLIND) )
    {
	send_to_char( "You can't see a thing!\n\r", ch );
	return FALSE;
    }

    return TRUE;
}

/* changes your scroll */
void do_scroll(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char buf[100];
    int lines;

    one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	if (ch->lines == 0)
	    send_to_char("You do not page long messages.\n\r",ch);
	else
	{
	    sprintf(buf,"You currently display %d lines per page.\n\r",
		    ch->lines + 2);
	    send_to_char(buf,ch);
	}
	return;
    }

    if (!is_number(arg))
    {
	send_to_char("You must provide a number.\n\r",ch);
	return;
    }

    lines = atoi(arg);

    if (lines == 0)
    {
        send_to_char("Paging disabled.\n\r",ch);
        ch->lines = 0;
        return;
    }

    if (lines < 10 || lines > 100)
    {
	send_to_char("You must provide a reasonable number.\n\r",ch);
	return;
    }

    sprintf(buf,"Scroll set to %d lines.\n\r",lines);
    send_to_char(buf,ch);
    ch->lines = lines - 2;
}

/* RT does socials */
void do_socials(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    int iSocial;
    int col;

    col = 0;

    for (iSocial = 0; social_table[iSocial].name[0] != '\0'; iSocial++)
    {
	sprintf(buf,"%-12s",social_table[iSocial].name);
	send_to_char(buf,ch);
	if (++col % 6 == 0)
	    send_to_char("\n\r",ch);
    }

    if ( col % 6 != 0)
	send_to_char("\n\r",ch);
    return;
}

/* RT Commands to replace news, motd, imotd, etc from ROM */

void do_motd(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"motd");
}

void do_imotd(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"imotd");
}

void do_rules(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"rules");
}

void do_story(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"story");
}

void do_wizlist(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"wizlist");
	return;
}

/* RT this following section holds all the auto commands from ROM, as well as
   replacements for config */

void do_autolist(CHAR_DATA *ch, char *argument)
{
    /* lists most player flags */
    if (IS_NPC(ch))
      return;

    send_to_char("   action     status\n\r",ch);
    send_to_char("---------------------\n\r",ch);

    send_to_char("autoassist     ",ch);
    if (IS_SET(ch->act,PLR_AUTOASSIST))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autoexit       ",ch);
    if (IS_SET(ch->act,PLR_AUTOEXIT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autogold       ",ch);
    if (IS_SET(ch->act,PLR_AUTOGOLD))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autoloot       ",ch);
    if (IS_SET(ch->act,PLR_AUTOLOOT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autosac        ",ch);
    if (IS_SET(ch->act,PLR_AUTOSAC))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autosplit      ",ch);
    if (IS_SET(ch->act,PLR_AUTOSPLIT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("compact mode   ",ch);
    if (IS_SET(ch->comm,COMM_COMPACT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("prompt         ",ch);
    if (IS_SET(ch->comm,COMM_PROMPT))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    send_to_char("color          ",ch);
    if (IS_SET(ch->act,PLR_COLOR))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    send_to_char("combine items  ",ch);
    if (IS_SET(ch->comm,COMM_COMBINE))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    if (IS_SET(ch->act,PLR_NOSUMMON))
	send_to_char("You cannot be summoned by anyone out of PK.\n\r",ch);
    else
	send_to_char("You can be summoned by anyone.\n\r",ch);

    if (IS_SET(ch->act,PLR_NOFOLLOW))
	send_to_char("You do not welcome followers.\n\r",ch);
    else
	send_to_char("You accept followers.\n\r",ch);

	if (IS_SET(ch->act,PLR_NO_TRANSFER))
		send_to_char("You do not accept transfered objects from transfer object spell.\n\r",ch);
	else
		send_to_char("You accept transfered objects from transfer object spell.\n\r",ch);

}

void do_color(CHAR_DATA *ch, char *argument)
{
    char     arg[ MAX_STRING_LENGTH ];

    argument = one_argument( argument, arg );

    if( !*arg )
    {
       if( !IS_SET( ch->act, PLR_COLOR ) )
       {
           SET_BIT( ch->act, PLR_COLOR );
           send_to_char( "{BYou now see color.{x\n\r",ch);
       }
       else
       {
           send_to_char_bw( "Color has been removed.\n\r",ch);
           REMOVE_BIT( ch->act, PLR_COLOR );
       }
       return;
    }
    else
    {
       send_to_char_bw( "Color Configuration is unavailable in this\n\r", ch );
       send_to_char_bw( "version of color, sorry!\n\r", ch );
    }

    return;
}

void do_autoassist(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTOASSIST))
    {
      send_to_char("Autoassist removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOASSIST);
    }
    else
    {
      send_to_char("You will now assist when needed.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOASSIST);
    }
}

void do_autoexit(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTOEXIT))
    {
      send_to_char("Exits will no longer be displayed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOEXIT);
    }
    else
    {
      send_to_char("Exits will now be displayed.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOEXIT);
    }
}

void do_autogold(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTOGOLD))
    {
      send_to_char("Autogold removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOGOLD);
    }
    else
    {
      send_to_char("Automatic gold looting set.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOGOLD);
    }
}

void do_autoloot(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTOLOOT))
    {
      send_to_char("Autolooting removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOLOOT);
    }
    else
    {
      send_to_char("Automatic corpse looting set.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOLOOT);
    }
}

void do_autosac(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTOSAC))
    {
      send_to_char("Autosacrificing removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOSAC);
    }
    else
    {
      send_to_char("Automatic corpse sacrificing set.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOSAC);
    }
}

void do_autosplit(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->act,PLR_AUTOSPLIT))
    {
      send_to_char("Autosplitting removed.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_AUTOSPLIT);
    }
    else
    {
      send_to_char("Automatic gold splitting set.\n\r",ch);
      SET_BIT(ch->act,PLR_AUTOSPLIT);
    }
}

void do_brief(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_BRIEF))
    {
      send_to_char("Full descriptions activated.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_BRIEF);
    }
    else
    {
      send_to_char("Short descriptions activated.\n\r",ch);
      SET_BIT(ch->comm,COMM_BRIEF);
    }
}

void do_compact(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_COMPACT))
    {
      send_to_char("Compact mode removed.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_COMPACT);
    }
    else
    {
      send_to_char("Compact mode set.\n\r",ch);
      SET_BIT(ch->comm,COMM_COMPACT);
    }
}

void do_show(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_SHOW_AFFECTS))
    {
      send_to_char("Affects will no longer be shown in score.\n\r",ch);

      REMOVE_BIT(ch->comm,COMM_SHOW_AFFECTS);
    }
    else
    {
      send_to_char("Affects will now be shown in score.\n\r",ch);
      SET_BIT(ch->comm,COMM_SHOW_AFFECTS);
    }
}

void do_prompt(CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];

   if ( argument[0] == '\0' )
   {
	if (IS_SET(ch->comm,COMM_PROMPT))
   	{
      	    send_to_char("You will no longer see prompts.\n\r",ch);
      	    REMOVE_BIT(ch->comm,COMM_PROMPT);
    	}
    	else
    	{
      	    send_to_char("You will now see prompts.\n\r",ch);
      	    SET_BIT(ch->comm,COMM_PROMPT);
    	}
       return;
   }
   //return;
   if( !strcmp( argument, "all" ) )
      strcpy( buf, "<%hhp %mm %vmv> ");
   else
   {
      if ( strlen(argument) > 50 )
         argument[50] = '\0';
      strcpy( buf, argument );
      smash_tilde( buf );
      if (str_suffix("%c",buf))
	strcat(buf," ");

   }

   free_string( ch->prompt );
   ch->prompt = str_dup( buf );
   sprintf(buf,"Prompt set to %s\n\r",ch->prompt );
   send_to_char(buf,ch);
   return;
}

void do_combine(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_COMBINE))
    {
      send_to_char("Long inventory selected.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_COMBINE);
    }
    else
    {
      send_to_char("Combined inventory selected.\n\r",ch);
      SET_BIT(ch->comm,COMM_COMBINE);
    }
}

void do_noloot(CHAR_DATA *ch, char *argument)
{
    return;

    if (IS_SET(ch->act,PLR_CANLOOT))
    {
      send_to_char("Your corpse is now safe from thieves.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_CANLOOT);
    }
    else
    {
      send_to_char("Your corpse may now be looted.\n\r",ch);
      SET_BIT(ch->act,PLR_CANLOOT);
    }
}

void do_nofollow(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *wch;
    if (IS_NPC(ch))
      return;

    if (IS_AFFECTED(ch,AFF_CHARM))
	{
	 send_to_char("Now why would you want to leave your master?\n\r",
ch);
	 return;
	}

    if (IS_SET(ch->act,PLR_NOFOLLOW))
    {
      send_to_char("You now accept followers.\n\r",ch);
      REMOVE_BIT(ch->act,PLR_NOFOLLOW);
    }
    else
    {
      send_to_char("You no longer accept followers.\n\r",ch);
      SET_BIT(ch->act,PLR_NOFOLLOW);
      die_follower( ch );
      if (IS_SET(ch->comm, COMM_MOUNTED))
      {
		  do_dismount(ch,"");
	  }
    for (wch = char_list; wch != NULL; wch = wch->next)
    {
    if (wch->life_lined != NULL)
	{
	if (wch->life_lined == ch)
	{
	act("Your lifeline to $N breaks.",ch,0,wch,TO_CHAR);
	wch->life_lined = NULL;
	}
	}
    }
    }
}

void do_nosummon(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
    {
      if (IS_SET(ch->imm_flags,IMM_SUMMON))
      {
	send_to_char("You may now be summoned by anyone.\n\r",ch);
	REMOVE_BIT(ch->imm_flags,IMM_SUMMON);
      }
      else
      {
	send_to_char("You may no longer be summoned.\n\r",ch);
	SET_BIT(ch->imm_flags,IMM_SUMMON);
      }
    }
    else
    {
      if (IS_SET(ch->act,PLR_NOSUMMON))
      {
        send_to_char("You may now be summoned by anyone.\n\r",ch);
        REMOVE_BIT(ch->act,PLR_NOSUMMON);
      }
      else
      {
        send_to_char("You may only be summoned by those in PK.\n\r",ch);
        SET_BIT(ch->act,PLR_NOSUMMON);
      }
    }
}

void do_glance( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if ( ch->desc == NULL )
	return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Glance at whom?\n\r", ch );
	return;
    }

    if ( ch->position < POS_SLEEPING )
    {
	send_to_char( "You can't see anything but stars!\n\r", ch );
	return;
    }

    if ( ch->position == POS_SLEEPING )
    {
	send_to_char( "You can't see anything, you're sleeping!\n\r", ch );
	return;
    }

    if ( !check_blind( ch ) )
	return;

    if ( (victim = get_char_room(ch, argument)) == NULL )
    {
	send_to_char( "That person isn't here.\n\r", ch );
	return;
    }
    if (is_affected(victim,gsn_cloak_form))
    {
        send_to_char("The figure is buried deep within a dark colored cloak.\n\r",ch);
        return;
    }

	show_char_to_char_2(victim,ch);
    return;
}

void do_examine( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    if ( ch->desc == NULL )
	return;
    one_argument(argument,arg);

    if ( arg[0] == '\0' )
    {
	send_to_char( "Examine what?\n\r", ch );
	return;
    }

    if ( ch->position < POS_SLEEPING )
    {
	send_to_char( "You can't see anything but stars!\n\r", ch );
	return;
    }

    if ( ch->position == POS_SLEEPING )
    {
	send_to_char( "You can't see anything, you're sleeping!\n\r", ch );
	return;
    }

    if ( !check_blind( ch ) )
	return;

    do_look(ch,arg);
    if ( (obj = get_obj_here(ch,arg)) != NULL)
    {
	switch(obj->item_type)
   	{
	default: 	break;
	case (ITEM_DRINK_CON):
	case (ITEM_CONTAINER):
	case (ITEM_CORPSE_NPC):
	case (ITEM_CORPSE_PC):
	sprintf(buf,"in %s",arg);
	do_look(ch,buf);
	}
    }
    return;
}

void do_look( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    EXIT_DATA *pexit;
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char *pdesc;
    int door;
    int number,count, light, i;
    OBJ_DATA *lobj;

    if ( ch->desc == NULL )
	return;

    if ( ch->position < POS_SLEEPING )
    {
	send_to_char( "You can't see anything but stars!\n\r", ch );
	return;
    }

    if ( ch->position == POS_SLEEPING )
    {
	send_to_char( "You can't see anything, you're sleeping!\n\r", ch );
	return;
    }

    if ( !check_blind( ch ) )
	return;

    if ( !IS_NPC(ch)
    &&   !IS_SET(ch->act, PLR_HOLYLIGHT)
    &&   room_is_dark( ch->in_room )
    &&   !IS_AFFECTED(ch, AFF_INFRARED) )
	{
	send_to_char( "It is pitch black ... \n\r", ch );
	show_char_to_char( ch->in_room->people, ch );
	return;
    	}
 if (!IS_NPC(ch)
    && IS_SET(ch->in_room->extra_room_flags,ROOM_GLOBE_DARKNESS)
    && str_cmp(pc_race_table[ch->race].name,"dark-elf") )
    {
    light = 0;
    for (i = 0; i < MAX_WEAR; i++)
    {
    lobj = get_eq_char(ch,i);
    if (lobj == NULL)
		continue;
	if (IS_SET(lobj->extra_flags,ITEM_GLOW))
		light++;
	if (lobj->item_type == ITEM_LIGHT)
		light += 3;
	}
	if (light < 8)
	{
	send_to_char( "It is pitch black ... \n\r", ch );
	show_char_to_char( ch->in_room->people, ch );
	return;
    	}
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    number = number_argument(arg1,arg3);
    count = 0;

    /*
	if ( !str_cmp( arg1, "liriel")
	&& (!IS_IMMORTAL(ch)))
	{
		AFFECT_DATA af;
		CHAR_DATA *victim;

		if ( ((victim = get_char_world(ch,arg1)) == NULL))
		{
			send_to_char("You do not see that here.\n\r",ch);
			return;
		}
		act("$n tries to gaze upon Liriel and is blinded by her beauty!",ch,0,0,TO_ROOM);
		send_to_char("You try to gaze upon Liriel and are blinded by her beauty!\n\r",ch);
		init_affect(&af);
		af.where     = TO_AFFECTS;
		af.type      = gsn_blindness;
		af.aftype    = AFT_SPELL;
		af.level     = 51;
		af.location  = APPLY_HITROLL;
		af.modifier  = -4;
		af.duration  = 2;
		af.bitvector = AFF_BLIND;
	   	affect_to_char( ch, &af );
		return;
	}*/

    if ( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) )
    {
	/* 'look' or 'look auto' */
	send_to_char( ch->in_room->name, ch );

	if ( (IS_IMMORTAL(ch)) &&
	   ( (IS_NPC(ch)) || (IS_SET(ch->act,PLR_HOLYLIGHT)) || (IS_BUILDER(ch, ch->in_room->area)) ) )
	{
	    sprintf(buf," {C[{WRoom %d{C]{x",ch->in_room->vnum);
	    send_to_char(buf,ch);
		sprintf(buf," {Y[{W%s{Y]{x",flag_string(room_flags,ch->in_room->room_flags));
		send_to_char(buf,ch);
	}

	send_to_char( "\n\r", ch );

	if ( arg1[0] == '\0'
	|| ( !IS_NPC(ch) && !IS_SET(ch->comm, COMM_BRIEF) ) )
	{
	    send_to_char( "  ",ch);
	    send_to_char( ch->in_room->description, ch );
	}

        if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOEXIT) )
	{
	    send_to_char("\n\r",ch);
            do_exits( ch, "auto" );
	}

	show_list_to_char( ch->in_room->contents, ch, FALSE, FALSE );
	show_char_to_char( ch->in_room->people,   ch );
	return;
    }

    if ( !str_cmp( arg1, "i" ) || !str_cmp(arg1, "in")  || !str_cmp(arg1,"on"))
    {
	/* 'look in' */
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Look in what?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_here( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You do not see that here.\n\r", ch );
	    return;
	}

	switch ( obj->item_type )
	{
	default:
	    send_to_char( "That is not a container.\n\r", ch );
	    break;

	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
		send_to_char( "It is empty.\n\r", ch );
		break;
	    }

	    sprintf( buf, "It's %sfilled with  a %s liquid.\n\r",
		obj->value[1] <     obj->value[0] / 4
		    ? "less than half-" :
		obj->value[1] < 3 * obj->value[0] / 4
		    ? "about half-"     : "more than half-",
		liq_table[obj->value[2]].liq_color
		);

	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    {
		send_to_char( "It is closed.\n\r", ch );
		break;
	    }

	    act( "$p holds:", ch, obj, NULL, TO_CHAR );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE );
	    break;
	}
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) != NULL )
    {
	show_char_to_char_1( victim, ch );
	return;
    }

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( can_see_obj( ch, obj ) )
	{  /* player can see object */
	    pdesc = get_extra_descr( arg3, obj->extra_descr );
	    if ( pdesc != NULL )
		{
	    	if (++count == number)
	    	{
		    send_to_char( pdesc, ch );
		    return;
	    	}
	    	else continue;
		}
 	    pdesc = get_extra_descr( arg3, obj->pIndexData->extra_descr );
 	    if ( pdesc != NULL )
		{
 	    	if (++count == number)
 	    	{
		    send_to_char( pdesc, ch );
		    return;
	     	}
		else continue;
		}
	    if ( is_name( arg3, obj->name ) )
	    	if (++count == number)
	    	{
	    	    send_to_char( obj->description, ch );
	    	    send_to_char( "\n\r",ch);
		    return;
		  }
	  }
    }

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
    {
	if ( can_see_obj( ch, obj ) )
	{
	    pdesc = get_extra_descr( arg3, obj->extra_descr );
	    if ( pdesc != NULL )
	    	if (++count == number)
	    	{
		    send_to_char( pdesc, ch );
		    return;
	    	}

	    pdesc = get_extra_descr( arg3, obj->pIndexData->extra_descr );
	    if ( pdesc != NULL )
	    	if (++count == number)
	    	{
		    send_to_char( pdesc, ch );
		    return;
	    	}

	    if ( is_name( arg3, obj->name ) )
		if (++count == number)
		{
		    send_to_char( obj->description, ch );
		    send_to_char("\n\r",ch);
		    return;
		}
	}
    }

    pdesc = get_extra_descr(arg3,ch->in_room->extra_descr);
    if (pdesc != NULL)
    {
	if (++count == number)
	{
	    send_to_char(pdesc,ch);
	    return;
	}
    }

    if (count > 0 && count != number)
    {
    	if (count == 1)
    	    sprintf(buf,"You only see one %s here.\n\r",arg3);
    	else
    	    sprintf(buf,"You only see %d of those here.\n\r",count);

    	send_to_char(buf,ch);
    	return;
    }

         if ( !str_cmp( arg1, "n" ) || !str_cmp( arg1, "north" ) ) door = 0;
    else if ( !str_cmp( arg1, "e" ) || !str_cmp( arg1, "east"  ) ) door = 1;
    else if ( !str_cmp( arg1, "s" ) || !str_cmp( arg1, "south" ) ) door = 2;
    else if ( !str_cmp( arg1, "w" ) || !str_cmp( arg1, "west"  ) ) door = 3;
    else if ( !str_cmp( arg1, "u" ) || !str_cmp( arg1, "up"    ) ) door = 4;
    else if ( !str_cmp( arg1, "d" ) || !str_cmp( arg1, "down"  ) ) door = 5;
    else
    {
	send_to_char( "You do not see that here.\n\r", ch );
	return;
    }

    /* 'look direction' */
    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
	send_to_char( "Nothing special there.\n\r", ch );
	return;
    }

    if ( pexit->description != NULL && pexit->description[0] != '\0' )
	send_to_char( pexit->description, ch );
    else
	send_to_char( "Nothing special there.\n\r", ch );

    if ( pexit->keyword    != NULL
    &&   pexit->keyword[0] != '\0'
    &&   pexit->keyword[0] != ' ' )
    {
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
	{
	    act( "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );
	}
	else if ( IS_SET(pexit->exit_info, EX_ISDOOR) )
	{
	    act( "The $d is open.",   ch, NULL, pexit->keyword, TO_CHAR );
	}
    }

    return;
}

/* RT added back for the hell of it */
void do_read (CHAR_DATA *ch, char *argument )
{
    do_look(ch,argument);
}


/*
 * Thanks to Zrin for auto-exit part.
 */
void do_exits( CHAR_DATA *ch, char *argument )
{
    extern char * const dir_name[];
    char buf[MAX_STRING_LENGTH];
    EXIT_DATA *pexit;
    bool found;
    bool fAuto;
    int door;

    fAuto  = !str_cmp( argument, "auto" );

    if ( !check_blind( ch ) )
	return;

	if (fAuto)
	{
	sprintf(buf,"[Exits:");
	}
    else if (IS_IMMORTAL(ch))
	{
		send_to_char("\n\r",ch);
		send_to_char("Exit identifiers: [closed] *pickproof* (nopass) >nobash< :locked:\n\r",ch);
		send_to_char("\n\r",ch);
		sprintf(buf,"Obvious exits from room %d:\n\r",ch->in_room->vnum);
	}
    else
	sprintf(buf,"Obvious exits:\n\r");

    found = FALSE;
    for ( door = 0; door <= 5; door++ )
    {
	if ( ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->u1.to_room != NULL
	&&   can_see_room(ch,pexit->u1.to_room)
	&&   !IS_SET(pexit->exit_info, EX_CLOSED) )
	{
	    found = TRUE;
	    if ( fAuto )
	    {
		strcat( buf, " " );
		strcat( buf, dir_name[door] );
	    }
	    else
	    {
		sprintf( buf + strlen(buf), "%-5s - %s",
		    capitalize( dir_name[door] ),
		    room_is_dark( pexit->u1.to_room )
			?  "Too dark to tell"
			: pexit->u1.to_room->name
		    );
		if (IS_IMMORTAL(ch))
		    sprintf(buf + strlen(buf),
			" (room %d)\n\r",pexit->u1.to_room->vnum);
		else
		    sprintf(buf + strlen(buf), "\n\r");
	    }
	}
	else if ( ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->u1.to_room != NULL
	&&   can_see_room(ch,pexit->u1.to_room)
	&&   IS_SET(pexit->exit_info, EX_CLOSED) )
	{
	    found = TRUE;
	    if ( fAuto && IS_SET(pexit->exit_info, EX_PICKPROOF) && IS_IMMORTAL(ch) )
	    {
		strcat( buf, " *" );
		strcat( buf, dir_name[door] );
		strcat( buf, "*" );
	    }
	    else if ( fAuto && IS_SET(pexit->exit_info, EX_NOPASS) && IS_IMMORTAL(ch) )
	    {
		strcat( buf, " (" );
		strcat( buf, dir_name[door] );
		strcat( buf, ")" );
	    }
	    else if ( fAuto && IS_SET(pexit->exit_info, EX_NOBASH) && IS_IMMORTAL(ch) )
	    {
		strcat( buf, " >" );
		strcat( buf, dir_name[door] );
		strcat( buf, "<" );
	    }
	    else if ( fAuto && IS_SET(pexit->exit_info, EX_LOCKED) && IS_IMMORTAL(ch) )
	    {
		strcat( buf, " :" );
		strcat( buf, dir_name[door] );
		strcat( buf, ":" );
	    }
	    else if ( fAuto )
	    {
		strcat( buf, " [" );
		strcat( buf, dir_name[door] );
		strcat( buf, "]" );
	    }
	    else if (IS_IMMORTAL(ch) && IS_SET(pexit->exit_info, EX_PICKPROOF))
	    {
		sprintf( buf + strlen(buf), "*%-5s* - %s",
		    capitalize( dir_name[door] ),
		    room_is_dark( pexit->u1.to_room )
			?  "Too dark to tell"
			: pexit->u1.to_room->name
		    );
		if (IS_IMMORTAL(ch))
		    sprintf(buf + strlen(buf),
			" (room %d)\n\r",pexit->u1.to_room->vnum);
		else
		    sprintf(buf + strlen(buf), "\n\r");
	    }
	    else if (IS_IMMORTAL(ch) && IS_SET(pexit->exit_info, EX_NOPASS))
	    {
		sprintf( buf + strlen(buf), "(%-5s) - %s",
		    capitalize( dir_name[door] ),
		    room_is_dark( pexit->u1.to_room )
			?  "Too dark to tell"
			: pexit->u1.to_room->name
		    );
		if (IS_IMMORTAL(ch))
		    sprintf(buf + strlen(buf),
			" (room %d)\n\r",pexit->u1.to_room->vnum);
		else
		    sprintf(buf + strlen(buf), "\n\r");
	    }
	    else if (IS_IMMORTAL(ch) && IS_SET(pexit->exit_info, EX_NOBASH))
	    {
		sprintf( buf + strlen(buf), ">%-5s< - %s",
		    capitalize( dir_name[door] ),
		    room_is_dark( pexit->u1.to_room )
			?  "Too dark to tell"
			: pexit->u1.to_room->name
		    );
		if (IS_IMMORTAL(ch))
		    sprintf(buf + strlen(buf),
			" (room %d)\n\r",pexit->u1.to_room->vnum);
		else
		    sprintf(buf + strlen(buf), "\n\r");
	    }
	    else if (IS_IMMORTAL(ch) && IS_SET(pexit->exit_info, EX_LOCKED))
	    {
		sprintf( buf + strlen(buf), ":%-5s: - %s",
		    capitalize( dir_name[door] ),
		    room_is_dark( pexit->u1.to_room )
			?  "Too dark to tell"
			: pexit->u1.to_room->name
		    );
		if (IS_IMMORTAL(ch))
		    sprintf(buf + strlen(buf),
			" (room %d)\n\r",pexit->u1.to_room->vnum);
		else
		    sprintf(buf + strlen(buf), "\n\r");
	    }
	    else if (IS_IMMORTAL(ch))
	    {
		sprintf( buf + strlen(buf), "[%-5s] - %s",
		    capitalize( dir_name[door] ),
		    room_is_dark( pexit->u1.to_room )
			?  "Too dark to tell"
			: pexit->u1.to_room->name
		    );
		if (IS_IMMORTAL(ch))
		    sprintf(buf + strlen(buf),
			" (room %d)\n\r",pexit->u1.to_room->vnum);
		else
		    sprintf(buf + strlen(buf), "\n\r");
	    }
	}
    }

    if ( !found )
	strcat( buf, fAuto ? " none" : "None.\n\r" );

    if ( fAuto )
	strcat( buf, "]\n\r" );

    send_to_char( buf, ch );
    return;
}

void do_worth( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
    {
	sprintf(buf,"You have %ld gold and %ld silver.\n\r",
	    ch->gold,ch->silver);
	send_to_char(buf,ch);
	return;
    }

    sprintf(buf,
    "You have %ld gold, %ld silver, and %d experience (%d exp to level).\n\r",
	ch->gold, ch->silver,ch->exp,
        ch->level * exp_per_level(ch) - ch->exp);

    send_to_char(buf,ch);

    return;
}


void do_score( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int i;

    if ( IS_NPC(ch) )
    {
	send_to_char( "Nope, not for NPC's. Try stat <mob>\n\r", ch );
	return;
    }

    sprintf( buf,
	"You are %s%s, level %d, %d years old (%d hours).\n\r",
	ch->original_name ? ch->original_name : ch->name,
	IS_NPC(ch) ? "" : ch->pcdata->title,
	ch->level, get_age(ch),
        ( ch->played + (int) (current_time - ch->logon) ) / 3600);
    send_to_char( buf, ch );
    sprintf(buf,"You are %s.\n\r",get_age_name(ch));
    send_to_char(buf,ch);

    if ( get_trust( ch ) != ch->level )
    {
	sprintf( buf, "You are trusted at level %d.\n\r",
	    get_trust( ch ) );
	send_to_char( buf, ch );
    }

    sprintf(buf, "Race: %s  Sex: %s  Class: %s\n\r",
	race_table[ch->race].name,
	ch->sex == 0 ? "sexless" : ch->sex == 1 ? "male" : "female",
 	IS_NPC(ch) ? "mobile" : class_table[ch->class].name);
    send_to_char(buf,ch);

    if (ch->class == 10)
    {

		if (ch->pcdata->element == 1)
		{
			send_to_char("Your specializing in the element of air.\n\r", ch );
		}
		else if (ch->pcdata->element == 2)
		{
			send_to_char("Your specializing in the element of earth.\n\r", ch );
		}
		else if (ch->pcdata->element == 3)
		{
			send_to_char("Your specializing in the element of ice.\n\r", ch );
		}
		else if (ch->pcdata->element == 4)
		{
			send_to_char("Your specializing in the element of fire.\n\r", ch );
		}
		else
		{
			send_to_char("Your not specializing in an element.\n\r", ch );
		}
	}

    if(ch->level >= 30)
    {
    sprintf( buf,
	"You have %d/%d hit, %d/%d mana, %d/%d movement.\n\r",
	ch->hit,  ch->max_hit,
	ch->mana, ch->max_mana,
	ch->move, ch->max_move);
    send_to_char( buf, ch );
    }
    else
    {
    int percenta, percentb, percentc;
    if (ch->max_hit == 0)
	percenta = 0;
    else
	percenta = (ch->hit * 100) / ch->max_hit;
    if (ch->max_mana == 0)
	percentb = 0;
    else
        percentb = (ch->mana * 100) / ch->max_mana;
    if (ch->max_move == 0)
 	percentc = 0;
    else
        percentc = (ch->move * 100) / ch->max_move;
    sprintf( buf,
        "You have %d%% hit, %d%% mana, %d%% movement.\n\r",
        percenta,
        percentb,
        percentc);
    send_to_char( buf, ch);
    }
     sprintf( buf,
	"You have %d practices and %d training sessions.\n\r",
	ch->practice, ch->train);
    send_to_char( buf, ch );
     sprintf( buf,
	"You are carrying %d/%d items with weight %ld/%d pounds.\n\r",
	ch->carry_number, can_carry_n(ch),
	get_carry_weight(ch) / 10, can_carry_w(ch) /10 );
    send_to_char( buf, ch );
    if(ch->level >= 20)
    {
    sprintf( buf,
	"Str: %d(%d) Int: %d(%d) Wis: %d(%d) Dex: %d(%d) Con: %d(%d) Chr: %d(%d)\n\r",
	ch->perm_stat[STAT_STR],
	get_curr_stat(ch,STAT_STR),
	ch->perm_stat[STAT_INT],
	get_curr_stat(ch,STAT_INT),
	ch->perm_stat[STAT_WIS],
	get_curr_stat(ch,STAT_WIS),
	ch->perm_stat[STAT_DEX],
	get_curr_stat(ch,STAT_DEX),
	ch->perm_stat[STAT_CON],
	get_curr_stat(ch,STAT_CON),
	ch->perm_stat[STAT_CHR],
	get_curr_stat(ch,STAT_CHR) );
    send_to_char( buf, ch );
    }
    else
    {
    sprintf( buf,
        "Str: %d(?\?)  Int: %d(?\?)  Wis: %d(?\?)  Dex: %d(?\?)  Con: %d(?\?)\n\r",
        ch->perm_stat[STAT_STR],
        ch->perm_stat[STAT_INT],
        ch->perm_stat[STAT_WIS],
        ch->perm_stat[STAT_DEX],
        ch->perm_stat[STAT_CON] );
    send_to_char( buf, ch );
    }
    sprintf( buf,
	"You have scored %d exp, and have %ld gold and %ld silver coins.\n\r",
	ch->exp,  ch->gold, ch->silver );
    send_to_char( buf, ch );

    if(isShifted(ch))
    {
	sprintf(buf,"You are shifted in the %s form.\n\r", form_table[ch->pcdata->shifted].name);
	send_to_char(buf,ch);
    }


    /* RT shows exp to level */
    if (!IS_NPC(ch) && ch->level < LEVEL_HERO)
    {
      sprintf (buf,
	"You need %d exp to level.\n\r",
        (ch->level * exp_per_level(ch) - ch->exp));
      send_to_char( buf, ch );
     }
    if(ch->level >= 30)
    {
    sprintf( buf, "Wimpy set to %d hit points.  Hometown is %s\n\r",ch->wimpy,
(ch->temple == 8839) ? "Elvenhame (good)" :
(ch->temple == 8844) ? "Elvenhame (neutral)" :
(ch->temple == 8848) ? "Elvenhame (evil)" :
(ch->temple == 9609) ? "New thalos" :
(ch->temple == 699 ) ? "Ofcol" :
(ch->temple == 768) ? "Arkham" :
"Midgaard");
    }
    else
    {
    sprintf( buf, "Wimpy set to %d%% hit points.  Hometown is %s\n\r",ch->wimpy,
(ch->temple == 8839) ? "Elvenhame (good)" :
(ch->temple == 8844) ? "Elvenhame (neutral)" :
(ch->temple == 8848) ? "Elvenhame (evil)" :
(ch->temple == 9609) ? "New thalos" :
(ch->temple == 699 ) ? "Ofcol" :
(ch->temple == 768) ? "Arkham" :
"Midgaard");
    }
    send_to_char( buf, ch );

//    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]   > 10 )
//	send_to_char( "You are drunk.\n\r",   ch );
//    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] ==  0 )
//	send_to_char( "You are thirsty.\n\r", ch );
//    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_HUNGER]   ==  0 )
//	send_to_char( "You are hungry.\n\r",  ch );

    switch ( ch->position )
    {
    case POS_DEAD:
	send_to_char( "You are DEAD!!\n\r",		ch );
	break;
    case POS_MORTAL:
	send_to_char( "You are mortally wounded.\n\r",	ch );
	break;
    case POS_INCAP:
	send_to_char( "You are incapacitated.\n\r",	ch );
	break;
    case POS_STUNNED:
	send_to_char( "You are stunned.\n\r",		ch );
	break;
    case POS_SLEEPING:
	send_to_char( "You are sleeping.\n\r",		ch );
	break;
    case POS_RESTING:
	send_to_char( "You are resting.\n\r",		ch );
	break;
    case POS_SITTING:
	send_to_char( "You are sitting.\n\r",		ch );
	break;
    case POS_STANDING:
	send_to_char( "You are standing.\n\r",		ch );
	break;
    case POS_FIGHTING:
	send_to_char( "You are fighting.\n\r",		ch );
	break;
    }


    /* print AC values */
    if (ch->level >= 25)
    {
	sprintf( buf,"Armor: pierce: %d  bash: %d  slash: %d  magic: %d\n\r",
		 GET_AC(ch,AC_PIERCE),
		 GET_AC(ch,AC_BASH),
		 GET_AC(ch,AC_SLASH),
		 GET_AC(ch,AC_EXOTIC));
    	send_to_char(buf,ch);
    }

    if (!IS_NPC (ch) && ch->level < LEVEL_HERO)
    {
    for (i = 0; i < 4; i++)
    {
	char * temp;

	switch(i)
	{
	    case(AC_PIERCE):	temp = "piercing";	break;
	    case(AC_BASH):	temp = "bashing";	break;
	    case(AC_SLASH):	temp = "slashing";	break;
	    case(AC_EXOTIC):	temp = "magic";		break;
	    default:		temp = "error";		break;
	}

	send_to_char("You are ", ch);

	if      (GET_AC(ch,i) >=  101 )
	    sprintf(buf,"hopelessly vulnerable to %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 80)
	    sprintf(buf,"defenseless against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 60)
	    sprintf(buf,"barely protected from %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 40)
	    sprintf(buf,"slightly armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 20)
	    sprintf(buf,"somewhat armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= 0)
	    sprintf(buf,"armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -20)
	    sprintf(buf,"well-armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -40)
	    sprintf(buf,"very well-armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -60)
	    sprintf(buf,"heavily armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -80)
	    sprintf(buf,"superbly armored against %s.\n\r",temp);
	else if (GET_AC(ch,i) >= -100)
	    sprintf(buf,"almost invulnerable to %s.\n\r",temp);
        else
	    sprintf(buf,"divinely armored against %s.\n\r",temp);

	send_to_char(buf,ch);
    }
    }


    /* RT wizinvis and holy light */
    if ( IS_IMMORTAL(ch))
    {
      send_to_char("Holy Light: ",ch);
      if (IS_SET(ch->act,PLR_HOLYLIGHT))
        send_to_char("on",ch);
      else
        send_to_char("off",ch);

      if (ch->invis_level)
      {
        sprintf( buf, "  Invisible: level %d",ch->invis_level);
        send_to_char(buf,ch);
      }

      if (ch->incog_level)
      {
	sprintf(buf,"  Incognito: level %d",ch->incog_level);
	send_to_char(buf,ch);
      }
      send_to_char("\n\r",ch);
    }

if ( ch->level >= 15 )
    {
	sprintf( buf, "Hitroll: %d  Damroll: %d. ",
	    GET_HITROLL(ch), GET_DAMROLL(ch) );
	send_to_char( buf, ch );
	if (ch->level < 39)
	send_to_char("\n\r",ch);
    }
    if (ch->level >= 39)
    {
	sprintf(buf,"Saves spell: %d.", ch->saving_throw);
	send_to_char(buf,ch);
    send_to_char("\n\r",ch);
    }

	if (!IS_IMMORTAL(ch)) {
	for (i = 0; i <= MAX_SPECS; i++) {
		if (ch->pcdata->warrior_specs[i] > 0 && ch->pcdata->warrior_specs[i] <= MAX_WEAPON) {
			strcpy(buf,weapon_name_lookup(ch->pcdata->warrior_specs[i]));
			if (!str_infix(buf,"whip")) {
				strcpy(buf,"whip/flail");
			} else if (!str_infix(buf,"flail")) {
				strcpy(buf,"whip/flail");
			} else if (!str_infix(buf,"staff")) {
				strcpy(buf,"staff/spear");
			} else if (!str_infix(buf,"spear")) {
				strcpy(buf,"staff/spear");
			}
			send_to_char("You are specialized in ",ch);
			send_to_char(buf,ch);
			send_to_char(".\n\r",ch);
		}
	}
	}

    send_to_char( "You are ", ch );
         if ( ch->alignment == 1000 ) send_to_char( "good,", ch );
    else if ( ch->alignment == 0 ) send_to_char( "neutral,", ch );
    else                             send_to_char( "evil,", ch );
    send_to_char( " and have a ", ch );
         if ( ch->pcdata->ethos == 1000 ) send_to_char( "lawful ethos.\n\r", ch );
    else if ( ch->pcdata->ethos == 0 ) send_to_char( "neutral ethos.\n\r", ch );
    else                               send_to_char( "chaotic ethos.\n\r", ch );

    if (IS_SET(ch->comm,COMM_SHOW_AFFECTS))
	do_affects(ch,"");
}

void do_affects(CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA *paf, *paf_last = NULL;
    char buf[MAX_STRING_LENGTH];
    if ( ch->affected != NULL && (ch->affected->aftype!=AFT_INVIS || ch->affected->next!=NULL))
    {
	send_to_char( "You are affected by:\n\r", ch );
	for ( paf = ch->affected; paf != NULL; paf = paf->next )
	{

	    if(paf->aftype==AFT_INVIS)
		continue;

	    if (paf_last != NULL && paf->type == paf_last->type)
		{
		if (ch->level >= 20)
		    sprintf( buf, "                      ");
		else
		    continue;
	    } else {
		if (paf->aftype == AFT_SKILL)
	    		sprintf( buf, "Skill: %-15s", paf->name ? paf->name : skill_table[paf->type].name );
		else if (paf->aftype == AFT_POWER)
	    		sprintf( buf, "Power: %-15s", paf->name ? paf->name : skill_table[paf->type].name );
		else if (paf->aftype == AFT_MALADY)
	    		sprintf( buf, "Malady: %-14s", paf->name ? paf->name : skill_table[paf->type].name );
		else if (paf->aftype == AFT_COMMUNE)
	    		sprintf( buf, "Commune: %-13s", paf->name ? paf->name : skill_table[paf->type].name );
		else if (paf->aftype == AFT_SONG)
			sprintf( buf, "Song: %-16s", paf->name ? paf->name : skill_table[paf->type].name );
		else if (paf->aftype == AFT_HIDDEN && (IS_IMMORTAL(ch)))
			sprintf( buf, "Hidden: %-15s", paf->name ? paf->name : skill_table[paf->type].name );
		else if (paf->aftype != AFT_INVIS)
			sprintf( buf, "Spell: %-15s", paf->name ? paf->name : skill_table[paf->type].name );
		}

	    	send_to_char( buf, ch );

	    if ( ch->level >= 20 )
	    {
		sprintf( buf,
		    ": modifies %s by %d ",
		    affect_loc_name( paf->location ),
		    paf->modifier);
		if(paf->aftype!=AFT_INVIS)
			send_to_char( buf, ch );
		if ( paf->duration == -1 )
		    sprintf( buf, "permanently" );
		else
		    sprintf( buf, "for %d hours", paf->duration );
		if(paf->aftype!=AFT_INVIS)
			send_to_char( buf, ch );
	    }
	    if(paf->aftype!=AFT_INVIS);
	    	send_to_char( "\n\r", ch );
	    paf_last = paf;
	}
    }
    else
	send_to_char("You are not affected by anything.\n\r",ch);

    return;
}

char *	const	day_name	[] =
{
    "the Moon", "the Bull", "Deception", "Thunder", "Freedom",
    "the Great Gods", "the Sun"
};

char *	const	month_name	[] =
{
    "Winter", "the Winter Wolf", "the Frost Giant", "the Old Forces",
    "the Grand Struggle", "the Spring", "Nature", "Futility", "the Dragon",
    "the Sun", "the Heat", "the Battle", "the Dark Shades", "the Shadows",
    "the Long Shadows", "the Ancient Darkness", "the Great Evil"
};

void do_time( CHAR_DATA *ch, char *argument )
{
    extern char str_boot_time[];
    char buf[MAX_STRING_LENGTH];
    char *suf;
    int day;

    day     = time_info.day + 1;

         if ( day > 4 && day <  20 ) suf = "th";
    else if ( day % 10 ==  1       ) suf = "st";
    else if ( day % 10 ==  2       ) suf = "nd";
    else if ( day % 10 ==  3       ) suf = "rd";
    else                             suf = "th";

    sprintf( buf,
	"It is %d o'clock %s, Day of %s, %d%s the Month of %s.\n\r",
	(time_info.hour % 12 == 0) ? 12 : time_info.hour %12,
	time_info.hour >= 12 ? "pm" : "am",
	day_name[day % 7],
	day, suf,
	month_name[time_info.month]);
    send_to_char(buf,ch);
    sprintf(buf,"Inahn started up at %s\n\rThe system time is %s\n\r",
	str_boot_time,
	(char *) ctime( &current_time )
	);

    send_to_char( buf, ch );
    return;
}



void do_weather( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    static char * const sky_look[4] =
    {
	"cloudless",
	"cloudy",
	"rainy",
	"lit by flashes of lightning"
    };

    if ( !IS_OUTSIDE(ch) )
    {
	send_to_char( "You can't see the weather indoors.\n\r", ch );
	return;
    }

    sprintf( buf, "The sky is %s and %s.\n\r",
	sky_look[weather_info.sky],
	weather_info.change >= 0
	? "a warm southerly breeze blows"
	: "a cold northern gust blows"
	);
    send_to_char( buf, ch );
    return;
}

void do_help( CHAR_DATA *ch, char *argument )
{
    HELP_DATA *pHelp;
    BUFFER *output;
    bool found = FALSE;
    char argall[MAX_INPUT_LENGTH],argone[MAX_INPUT_LENGTH];
    int level;

    output = new_buf();

    if ( argument[0] == '\0' )
	argument = "summary";

    /* this parts handles help a b so that it returns help 'a b' */
    argall[0] = '\0';
    while (argument[0] != '\0' )
    {
	argument = one_argument(argument,argone);
	if (argall[0] != '\0')
	    strcat(argall," ");
	strcat(argall,argone);
    }

    for ( pHelp = help_first; pHelp != NULL; pHelp = pHelp->next )
    {
    	level = (pHelp->level < 0) ? -1 * pHelp->level - 1 : pHelp->level;

	if (level > get_trust( ch ) )
	{
	if (pHelp->level < 61)
		continue;
        if ((pHelp->level == 61 && ch->cabal != CABAL_ACADIAN)
        || ( pHelp->level == 62 && ch->cabal != CABAL_COVEN)
        || ( pHelp->level == 63 && ch->cabal != CABAL_BRIAR)
        || ( pHelp->level == 64 && ch->cabal != CABAL_PROTECTOR)
        || ( pHelp->level == 65 && ch->cabal != CABAL_VIGILANTE)
        || ( pHelp->level == 66 && ch->cabal != CABAL_RAVAGER) )
	    continue;
    	}

	if ( is_name( argall, pHelp->keyword ) )
	{
	    /* add seperator if found */
	    if (found)
		add_buf(output,
    "\n\r============================================================\n\r\n\r");
	    if ( pHelp->level >= 0 && str_cmp( argall, "imotd" ) )
	    {
		add_buf(output,pHelp->keyword);
		add_buf(output,"\n\r");
	    }

	    /*
	     * Strip leading '.' to allow initial blanks.
	     */
	    if ( pHelp->text[0] == '.' )
		add_buf(output,pHelp->text+1);
	    else
		add_buf(output,pHelp->text);
	    found = TRUE;
	    /* small hack :) */
	    if (ch->desc != NULL && ch->desc->connected != CON_PLAYING
	    &&  		    ch->desc->connected != CON_GEN_GROUPS)
		break;
	}
    }

    if (!found)
    	send_to_char( "No help on that word.\n\r", ch );
    else
	page_to_char(buf_string(output),ch);
    free_buf(output);
}

void do_whois (CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    BUFFER *output;
    char buf[MAX_STRING_LENGTH];
    char rbuf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    bool found = FALSE;

    one_argument(argument,arg);

    if (arg[0] == '\0')
    {
        send_to_char("Whois who?\n\r",ch);
	return;
    }

    output = new_buf();

    for (d = descriptor_list; d != NULL; d = d->next)
    {
	CHAR_DATA *wch;
	char const *class;

 	if (d->connected != CON_PLAYING || !can_see(ch,d->character))
	    continue;

	wch = ( d->original != NULL ) ? d->original : d->character;

 	if (!can_see(ch,wch))
	    continue;

	if (str_cmp(wch->name, wch->original_name) ? IS_IMMORTAL(ch) ?
		!str_prefix(arg,wch->original_name) : 0 :
		!str_prefix(arg,wch->name)) {
	    found = TRUE;

	    /* work out the printing */
	    class = class_table[wch->class].who_name;
	    switch(wch->level)
	    {
		case MAX_LEVEL - 0 : class = "IMP"; 	break;
		case MAX_LEVEL - 1 : class = "CRE";	break;
		case MAX_LEVEL - 2 : class = "SUP";	break;
		case MAX_LEVEL - 3 : class = "DEI";	break;
		case MAX_LEVEL - 4 : class = "GOD";	break;
		case MAX_LEVEL - 5 : class = "IMM";	break;
		case MAX_LEVEL - 6 : class = "DEM";	break;
		case MAX_LEVEL - 7 : class = "ANG";	break;
		case MAX_LEVEL - 8 : class = "AVA";	break;
	    }
	    /* a little formatting */
            if (wch->level >= 52 || wch == ch || IS_IMMORTAL(ch) || IS_HEROIMM(wch))
            {
        if((get_trust(ch) >= 52)) {
                strcpy(rbuf,"  ");
        } else {
                strcpy(rbuf,"");
        }
			sprintf(buf, "[%2d %5s %s%s] %s%s%s%s%s%s%s%s%s%s%s%s\n\r",
			wch->level,
			pc_race_table[wch->race].who_name,
			class,
			(get_trust(ch) >= 52) && wch->pcdata->history_buffer != NULL ? " *" : rbuf,
			can_pk(ch, wch) ? "{W({rPK{W){x " : "",
			wch->incog_level >= LEVEL_HERO ? "(Incog) ": "",
 			wch->invis_level >= LEVEL_HERO ? "(Wizi) " : "",
			cabal_table[wch->cabal].who_name,
			IS_SET(wch->comm, COMM_AFK) ? "[AFK] " : "",
			IS_SET(wch->act,PLR_CRIMINAL) ? "(WANTED) " : "",
            ch->cabal==CABAL_ACADIAN && wch->pcdata->bounty != 0 ? "(BOUNTY) " : "",
			IS_SET(wch->act,PLR_THIEF) ? "(THIEF) " : "",
			isNewbie(wch) ? "(NEWBIE) " : "",
			wch->original_name,
 			IS_NPC(wch) ? "" : wch->pcdata->title,
			IS_NPC(wch) ? "" :
			(wch->pcdata->extitle) ? wch->pcdata->extitle : "");
			add_buf(output,buf);
            }
            else
            {
			sprintf(buf, "[%2d %5s %s] %s%s%s%s%s%s%s%s%s%s%s%s\n\r",
			wch->level,
			pc_race_table[wch->race].who_name,
			class,
			can_pk(ch, wch) ? "{W({rPK{W){x " : "",
			wch->incog_level >= LEVEL_HERO ? "(Incog) ": "",
 			wch->invis_level >= LEVEL_HERO ? "(Wizi) " : "",
            		wch->cabal == CABAL_VIGILANTE ? cabal_table[wch->cabal].who_name :(wch->cabal == ch->cabal) ? cabal_table[wch->cabal].who_name : "",
			IS_SET(wch->comm, COMM_AFK) ? "[AFK] " : "",
			IS_SET(wch->act,PLR_CRIMINAL) ? "(WANTED) " : "",
            ch->cabal==CABAL_ACADIAN && wch->pcdata->bounty != 0 ? "(BOUNTY) " : "",
			IS_SET(wch->act,PLR_THIEF) ? "(THIEF) " : "",
			isNewbie(wch) ? "(NEWBIE) " : "",
			wch->original_name,
 			IS_NPC(wch) ? "" : wch->pcdata->title,
			IS_NPC(wch) ? "" :
			(wch->pcdata->extitle) ? wch->pcdata->extitle : "");
			add_buf(output,buf);
            }
	}
    }

    if (!found)
    {
	send_to_char("No one of that name is playing.\n\r",ch);
	return;
    }

    page_to_char(buf_string(output),ch);
    free_buf(output);
}

/*
 * New 'who' command originally by Alander of Rivers of Mud.
 */
void do_who( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char rbuf[MAX_STRING_LENGTH];
    BUFFER *output;
    char SPECIAL[10];
    DESCRIPTOR_DATA *d;
    bool OVERRIDE=FALSE;
    int iClass;
    int iRace;
    int iCabal;
    int iLevelLower;
    int iLevelUpper;
    int nNumber;
    int nMatch;
    int anc=0;
    bool rgfClass[MAX_CLASS];
    bool rgfRace[MAX_PC_RACE];
    bool rgfCabal[MAX_CABAL];
    bool fCriminal = FALSE;
    bool fClassRestrict = FALSE;
    bool fCabalRestrict = FALSE;
    bool fCabal = FALSE;
    bool fRaceRestrict = FALSE;
    bool fImmortalOnly = FALSE;
    bool fPkOnly = FALSE;
    bool fBuilder = FALSE;
    bool fNewbie = FALSE;
    bool fAnathema = FALSE;
    bool fBloodoath = FALSE;
    bool fCitizen = FALSE;
    bool fLeader = FALSE;

    //char abuf[100];
    /*
     * Set default arguments.
     */
    iLevelLower    = 0;
    iLevelUpper    = MAX_LEVEL;
    for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
        rgfClass[iClass] = FALSE;
    for ( iRace = 0; iRace < MAX_PC_RACE; iRace++ )
        rgfRace[iRace] = FALSE;
    for (iCabal = 0; iCabal < MAX_CABAL; iCabal++)
        rgfCabal[iCabal] = FALSE;

    un_duo(ch,NULL);

    /*
     * Parse arguments.
     */
    nNumber = 0;
    for ( ;; )
    {
        char arg[MAX_STRING_LENGTH];

        argument = one_argument( argument, arg );
        if ( arg[0] == '\0' )
            break;

        if ( is_number( arg ) )
        {
                            send_to_char(
                            "That's not a valid race or cabal.\n\r",
                               ch);
                            return;
        }
        else
        {

            /*
             * Look for classes to turn on.
             */
            if (!str_prefix(arg,"immortals"))
            {
                fImmortalOnly = TRUE;
            }
            else if (!str_prefix(arg,"pk"))
            {
                fPkOnly = TRUE;
            }
	    else if (!str_prefix(arg,"wanted") || !str_prefix(arg,"criminal"))
	    {
		fCriminal = TRUE;
	    }
	    else if (!str_prefix(arg,"builder") && IS_IMMORTAL(ch))
	    {
		fBuilder = TRUE;
	    }
	    else if (!str_prefix(arg,"newbie"))
	    {
		fNewbie = TRUE;
	    }
	    else if (!str_prefix(arg,"anathema")
		&& ch->pcdata->empire > EMPIRE_PBLOOD)
	    {
		fAnathema = TRUE;
	    }
	    else if (!str_prefix(arg,"bloodoath")
		&& ch->pcdata->empire > EMPIRE_PBLOOD)
	    {
		fBloodoath = TRUE;
	    }

	    else if (!str_prefix(arg,"citizen")
		&& ch->pcdata->empire > EMPIRE_PBLOOD)
	    {
		fCitizen = TRUE;
	    }
	    else if (!str_prefix(arg,"leader")
		&& ch->pcdata->empire > EMPIRE_PBLOOD)
	    {
		fLeader = TRUE;
	    }

            else
            {
                iClass = class_lookup(arg);
                if (iClass == -1)
                {
                    iRace = race_lookup(arg);

                    if (iRace == 0 || iRace >= MAX_PC_RACE)
		    {
                        iCabal = cabal_lookup(arg);
                        if (iCabal)
                        {
                            if (iCabal == ch->cabal
				|| IS_IMMORTAL(ch) )
                            {
                                fCabalRestrict = TRUE;
                                rgfCabal[iCabal] = TRUE;
                            }
                            else
                            {
                                send_to_char(
                                "You're not a member of that cabal.\n\r",
                                   ch);
                                return;
                            }
                        }
                        else
                        {
                            send_to_char(
                            "That's not a valid race or cabal.\n\r",
                               ch);
                            return;
                        }
		    }
                    else
                    {
                        fRaceRestrict = TRUE;
                        rgfRace[iRace] = TRUE;
                    }
                }
                else
                {
		if (IS_IMMORTAL(ch))
		{
                    fClassRestrict = TRUE;
                    rgfClass[iClass] = TRUE;
		}
		else
		   {
                            send_to_char(
                            "That's not a valid race or cabal.\n\r",
                               ch);
                            return;
		   }
                }
            }
        }
    }

    /*
     * Now show matching chars.
     */
    nMatch = 0;
    buf[0] = '\0';
    output = new_buf();
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        CHAR_DATA *wch;
        char const *class;
	char name[MIL];

        /*
         * Check for match against restrictions.
         * Don't use trust as that exposes trusted mortals.
         */
        if ( d->connected != CON_PLAYING || !can_see( ch, d->character ) )
            continue;

        wch   = ( d->original != NULL ) ? d->original : d->character;

	if (!can_see(ch,wch)) continue;

	if (isShifted(wch) && !IS_IMMORTAL(ch))
		continue;

	if(!IS_IMMORTAL(ch)&&is_affected(wch,gsn_cloak_form)&&ch!=wch
           && (!((ch->cabal == CABAL_ACADIAN) && (ch->pcdata->induct == 5))))
	{
	    if(can_pk(ch,wch)) anc=1;
	    continue;
	}

        if ( wch->level < iLevelLower
        ||   wch->level > iLevelUpper
        || ( fImmortalOnly  && (wch->level < LEVEL_IMMORTAL && !(IS_HEROIMM(wch)) ))
        || ( fPkOnly && !can_pk(ch, wch) )
        || ( fClassRestrict && !rgfClass[wch->class] )
        || ( fRaceRestrict && !rgfRace[wch->race])
	|| ( fCriminal && !IS_SET(wch->act,PLR_CRIMINAL))
	|| ( fBuilder && !IS_SET(wch->comm,COMM_BUILDER))
        || ( fNewbie && !isNewbie(wch))
	|| ( fAnathema && wch->pcdata->empire != EMPIRE_ANATH)
	|| ( fBloodoath && wch->pcdata->empire != EMPIRE_BLOOD)
	|| ( fCitizen && wch->pcdata->empire != EMPIRE_CITIZEN)
	|| ( fLeader && wch->pcdata->empire != EMPIRE_SLEADER)
        || ( fCabal && !is_cabal(wch) && !IS_IMMORTAL(ch))
        || ( fCabalRestrict && !rgfCabal[wch->cabal]))
            continue;

        nMatch++;

        /*
         * Figure out what to print for class.
	 */
	class = class_table[wch->class].who_name;
	switch ( wch->level )
	{
	default: break;
            {
                case MAX_LEVEL - 0 : class = "IMP";     break;
                case MAX_LEVEL - 1 : class = "CRE";     break;
                case MAX_LEVEL - 2 : class = "SUP";     break;
                case MAX_LEVEL - 3 : class = "DEI";     break;
                case MAX_LEVEL - 4 : class = "GOD";     break;
                case MAX_LEVEL - 5 : class = "IMM";     break;
                case MAX_LEVEL - 6 : class = "DEM";     break;
                case MAX_LEVEL - 7 : class = "ANG";     break;
                case MAX_LEVEL - 8 : class = "AVA";     break;
            }
	}
	OVERRIDE=FALSE;
	if (!strcmp(wch->name,"Kraenesk"))
        {
                OVERRIDE = TRUE;
                strcpy(SPECIAL,"Dragon");
        }

	if (!strcmp(wch->name,"Erifdel"))
		{
				OVERRIDE = TRUE;
				strcpy(SPECIAL,"Bhldr");
		}

	if (!strcmp(wch->name,"Liriel"))
        {
                OVERRIDE = TRUE;
                strcpy(SPECIAL,"Nymph");
        }
	/*if (!strcmp(wch->name,"Ceial"))
	{
		OVERRIDE=TRUE;
		strcpy(SPECIAL,"Angel");
	}*/
        if (!strcmp(wch->name,"Slash"))
        {
                OVERRIDE=TRUE;
                strcpy(SPECIAL,"Bitch");
        }
        if (!strcmp(wch->name,"openname"))
        {
                OVERRIDE=TRUE;
                strcpy(SPECIAL,"{bDemon{x");
        }
        if (!strcmp(wch->name,"Ainur"))
        {
                OVERRIDE=TRUE;
                strcpy(SPECIAL,"Bitch");
        }
	/*
	 * Format it up.
	 */

	if (isShifted(wch) )
		sprintf(name, "[%s] %s", wch->pcdata->old->name,wch->name);
	else
		strcpy(name, wch->name);

	if (wch->level >= 52 || IS_IMMORTAL(ch) || wch == ch || IS_HEROIMM(wch))
	{
        if((get_trust(ch) >= 52))
		{
			strcpy(rbuf,"  ");
        }
		else
		{
			strcpy(rbuf,"");
        }
        sprintf( buf, "[%2d %5s %s%s] %s%s%s%s%s%s%s%s%s%s%s%s%s%s\n\r",
			wch->level,
			OVERRIDE?SPECIAL:pc_race_table[wch->race].who_name,
			class,
            (get_trust(ch) >= 52) && wch->pcdata->history_buffer != NULL ? " {W*{x" : rbuf,
			can_pk(ch, wch) ? "{W({rPK{W){x " : "",
			IS_SET(wch->comm, COMM_OLCEDIT) ? "{C[{WOLC{C]{x " : "",
			wch->incog_level >= LEVEL_HERO ? "(Incog) " : "",
			wch->invis_level >= LEVEL_HERO ? "(Wizi) " : "",
            cabal_table[wch->cabal].who_name,
			(wch->pcdata->empire != 0) ? empire_table[wch->pcdata->empire+1].who_name : "",
			IS_SET(wch->comm, COMM_AFK) ? "{W[{BAFK{W]{x " : "",
            IS_SET(wch->act, PLR_CRIMINAL) ? "(WANTED) " : "",
            ch->cabal == CABAL_ACADIAN && wch->pcdata->bounty != 0 ? "(BOUNTY) " : "",
            IS_SET(wch->act, PLR_THIEF)  ? "(THIEF) "  : "",
			isNewbie(wch) ? "(NEWBIE) " : "",
			(wch->original_name) ? wch->original_name : wch->name,
			IS_NPC(wch) ? "" : wch->pcdata->title ,
			IS_NPC(wch) ? "" : (wch->pcdata->extitle) ? wch->pcdata->extitle : "");
		add_buf(output,buf);
	}
	else
	{
        sprintf( buf, "[    %5s   ] %s%s%s%s%s%s%s%s%s%s%s%s%s%s\n\r",
            OVERRIDE?SPECIAL:pc_race_table[wch->race].who_name,
			can_pk(ch, wch) ? "{W({rPK{W){x " : "",
			IS_SET(wch->comm, COMM_OLCEDIT) ? "{C[{WOLC{C]{x " : "",
			wch->incog_level >= LEVEL_HERO ? "(Incog) " : "",
			wch->invis_level >= LEVEL_HERO ? "(Wizi) " : "",
            wch->cabal == CABAL_VIGILANTE ? cabal_table[wch->cabal].who_name : (wch->cabal == ch->cabal) ?  cabal_table[wch->cabal].who_name : "",
			(ch->pcdata->empire > 1 && wch->pcdata->empire != 0) ? empire_table[wch->pcdata->empire+1].who_name : "",
			IS_SET(wch->comm, COMM_AFK) ? "{W[{BAFK{W]{x " : "",
            IS_SET(wch->act, PLR_CRIMINAL) ? "(WANTED) " : "",
            ch->cabal==CABAL_ACADIAN && wch->pcdata->bounty != 0 ? "(BOUNTY) " : "",
            IS_SET(wch->act, PLR_THIEF)  ? "(THIEF) "  : "",
			isNewbie(wch) ? "(NEWBIE) " : "",
            ((ch->cabal == CABAL_ACADIAN) && (wch->original_name) && (ch->pcdata->induct==5)) ? wch->original_name : wch->name,
			IS_NPC(wch) ? "" : wch->pcdata->title,
			IS_NPC(wch) ? "" : (wch->pcdata->extitle) ? wch->pcdata->extitle : "");
		add_buf(output,buf);
	}

    }

    sprintf( buf2, "\n\rPlayers found: %d\n\r", nMatch );
    add_buf(output,buf2);
    if(anc==1) {
	sprintf(buf,"Ancient is on the prowl.\n\r");
	add_buf(output,buf);
    }
    page_to_char( buf_string(output), ch );
    free_buf(output);
    return;
}

void do_count ( CHAR_DATA *ch, char *argument )
{
    int count;
    DESCRIPTOR_DATA *d;
    int not_seen;
    char buf[MAX_STRING_LENGTH];

    not_seen = 0;
    count = 0;

    for ( d = descriptor_list; d != NULL; d = d->next )
        if ( d->connected == CON_PLAYING)
	{
	 if ( can_see( ch, d->character ) )
	    count++;
	 else
	    not_seen++;
    	}

    if ( (count + not_seen) >= count_data[0])
	count_data[0] = (count + not_seen);

    if (count >= count_data[0])
        sprintf(buf,"There are %d characters on, the most so far today.\n\r",
	    count);
    else
	sprintf(buf,"There are %d characters on, the most on today was %d.\n\r",
	    count,count_data[0]);
    send_to_char(buf,ch);

/*    if (not_seen != 0)
    {

	sprintf(buf,"There are %d characters on that you can not see.\n\r",not_seen);
	sprintf(buf,"There are 0 characters on that you can not see.\n\r");

    send_to_char(buf,ch);
} */
    return;

}

void do_inventory( CHAR_DATA *ch, char *argument )
{

    if( isShifted(ch)  && !IS_IMMORTAL(ch))
    {
      	send_to_char("You can only do this in your human form.\n\r",ch);
      	return;
    }

    send_to_char( "You are carrying:\n\r", ch );
    show_list_to_char( ch->carrying, ch, TRUE, TRUE );
    return;
}



void do_equipment( CHAR_DATA *ch, char *argument )
{
	char obj_vnum[MSL];
    OBJ_DATA *obj;
    int iWear;
    bool found;

    if( isShifted(ch)  && !IS_IMMORTAL(ch))
    {
	send_to_char("You can only do this in your human form.\n\r",ch);
        return;
    }

    send_to_char( "You are using:\n\r", ch );
    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	if ( ( obj = get_eq_char( ch, iWear ) ) == NULL )
	    continue;

	send_to_char( where_name[iWear], ch );
	if ( can_see_obj( ch, obj ) )
	{
	    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
		if (IS_IMMORTAL(ch) && IS_SET(ch->act,PLR_HOLYLIGHT))
		{
   			sprintf(obj_vnum,"{x  [{W%5d{x]",obj->pIndexData->vnum);
			send_to_char(obj_vnum,ch);
		}
	    send_to_char( "\n\r", ch );
	}
	else
	{
	    send_to_char( "something.\n\r", ch );
	}
	found = TRUE;
    }

    if ( !found )
	send_to_char( "Nothing.\n\r", ch );

    return;
}



void do_compare( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj1;
    OBJ_DATA *obj2;
    int value1;
    int value2;
    char *msg;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Compare what to what?\n\r", ch );
	return;
    }

    if ( ( obj1 = get_obj_carry( ch, arg1, ch ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if (arg2[0] == '\0')
    {
	for (obj2 = ch->carrying; obj2 != NULL; obj2 = obj2->next_content)
	{
	    if (obj2->wear_loc != WEAR_NONE
	    &&  can_see_obj(ch,obj2)
	    &&  obj1->item_type == obj2->item_type
	    &&  (obj1->wear_flags & obj2->wear_flags & ~ITEM_TAKE) != 0 )
		break;
	}

	if (obj2 == NULL)
	{
	    send_to_char("You aren't wearing anything comparable.\n\r",ch);
	    return;
	}
    }

    else if ( (obj2 = get_obj_carry(ch,arg2,ch) ) == NULL )
    {
	send_to_char("You do not have that item.\n\r",ch);
	return;
    }

    msg		= NULL;
    value1	= 0;
    value2	= 0;

    if ( obj1 == obj2 )
    {
	msg = "You compare $p to itself.  It looks about the same.";
    }
    else if ( obj1->item_type != obj2->item_type )
    {
	msg = "You can't compare $p and $P.";
    }
    else
    {
	switch ( obj1->item_type )
	{
	default:
	    msg = "You can't compare $p and $P.";
	    break;

	case ITEM_ARMOR:
	    value1 = obj1->value[0] + obj1->value[1] + obj1->value[2];
	    value2 = obj2->value[0] + obj2->value[1] + obj2->value[2];
	    break;

	case ITEM_WEAPON:
	    if (obj1->pIndexData->new_format)
		value1 = (1 + obj1->value[2]) * obj1->value[1];
	    else
	    	value1 = obj1->value[1] + obj1->value[2];

	    if (obj2->pIndexData->new_format)
		value2 = (1 + obj2->value[2]) * obj2->value[1];
	    else
	    	value2 = obj2->value[1] + obj2->value[2];
	    break;
	}
    }

    if ( msg == NULL )
    {
	     if ( value1 == value2 ) msg = "$p and $P look about the same.";
	else if ( value1  > value2 ) msg = "$p looks better than $P.";
	else                         msg = "$p looks worse than $P.";
    }

    act( msg, ch, obj1, obj2, TO_CHAR );
    return;
}



void do_credits( CHAR_DATA *ch, char *argument )
{
    do_help( ch, "diku" );
    return;
}


/*
void do_where( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    bool found;

    one_argument( argument, arg );

    un_duo(ch,NULL);

    if (IS_AFFECTED(ch,AFF_BLIND))
    {
	send_to_char("You can't see anything!",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Players near you:\n\r", ch );
	found = FALSE;
	for ( d = descriptor_list; d; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
	    && ( victim = d->character ) != NULL
	    &&   !IS_NPC(victim)
	    &&   victim->in_room != NULL
            &&   (!IS_SET(victim->in_room->room_flags,ROOM_NOWHERE) || IS_IMMORTAL(ch))
 	    &&   (is_room_owner(ch,victim->in_room)
	    ||    !room_is_private(victim->in_room))
	    &&   victim->in_room->area == ch->in_room->area
	    &&   can_see( ch, victim ))
	    //&& ( !isShifted(victim) || (isShifted(victim) && IS_IMMORTAL(ch))))
	    {
		found = TRUE;
		if ( !isShifted(victim))
		sprintf( buf, "%s%-28s %s\n\r",
		can_pk(ch,victim) ? "{W({rPK{W){x " : "", victim->name,victim->in_room->name );
		else
		sprintf( buf, "%s[%s] %-28s%s\n\r",
		    can_pk(ch,victim) ? "{W({rPK{W){x " : "", victim->pcdata->old->name,victim->name,victim->in_room->name );
		send_to_char( buf, ch );
	    }
	}

	if ( !found )
	    send_to_char( "None\n\r", ch );
    }
    else if (!str_prefix(arg,"pk"))
    {
	found = FALSE;
	for ( d = descriptor_list; d; d = d->next )
	{

	    if ( d->connected == CON_PLAYING
	    && ( victim = d->character ) != NULL
	    &&   !IS_NPC(victim)
	    &&   victim->in_room != NULL
            &&   (!IS_SET(victim->in_room->room_flags,ROOM_NOWHERE) || IS_IMMORTAL(ch))
 	    &&   (is_room_owner(ch,victim->in_room)
	    ||    !room_is_private(victim->in_room))
	    &&   victim->in_room->area == ch->in_room->area
	    &&   can_pk(ch,victim)
	    &&   can_see( ch, victim ))
	    //&& ( !isShifted(victim) || (isShifted(victim) && IS_IMMORTAL(ch))))
	    {
	   	found = TRUE;
		if (!isShifted(victim) )
		sprintf( buf, "%s%-28s %s\n\r",
		    can_pk(ch,victim) ? "{W({rPK{W){x " : "", victim->name,victim->in_room->name );
		else
		sprintf(buf, "%s[%s] %-18s%s\n\r",
		    can_pk(ch,victim) ? "{W({rPK{W){x " : "", victim->pcdata->old->name,victim->name,victim->in_room->name );
		send_to_char( buf, ch );
	    }
	}
	if ( !found )
	    act( "You didn't find anyone in PK.", ch, NULL, arg, TO_CHAR );
    }
    else
    {
	found = FALSE;
	for ( victim = char_list; victim != NULL; victim = victim->next )
	{
	    if ( victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    && !IS_AFFECTED (victim, AFF_HIDE)
            && !IS_AFFECTED (victim, AFF_CAMOUFLAGE)
	    &&   !IS_AFFECTED(victim, AFF_SNEAK)
	    &&   can_see( ch, victim )
	    && ( !isShifted(victim) || (isShifted(victim) && IS_IMMORTAL(ch)))
	    && ( is_name( arg, victim->name ) || (isShifted(victim) &&
		IS_IMMORTAL(ch) && is_name(arg, victim->pcdata->old->name))))
	    {
		found = TRUE;
		if (!isShifted(victim))
		{
			if (!IS_NPC(victim))
		sprintf( buf, "%s%-28s %s\n\r",
		can_pk(ch,victim) ? "{W({rPK{W){x " : "", victim->name,victim->in_room->name);
			else
		sprintf( buf, "%-28s %s\n\r", victim->short_descr,victim->in_room->name);
		} else {
			if (!IS_NPC(victim))
		sprintf(buf, "%s[%s] %-18s%s\n\r",
		    can_pk(ch,victim) ? "{W({rPK{W){x " : "", victim->pcdata->old->name,victim->name,victim->in_room->name );
		else
		sprintf(buf, "%-28s[%s] %s\n\r",victim->short_descr,victim->pcdata->old->name,victim->in_room->name);
		}
		send_to_char( buf, ch );
		break;
	    }
	}
	if ( !found )
	    act( "You didn't find any $T.", ch, NULL, arg, TO_CHAR );
    }

    return;
}
*/

void do_where( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char *name;
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;

    bool found;

    one_argument( argument, arg );

    un_duo(ch,NULL);

    if (IS_AFFECTED(ch,AFF_BLIND))
    {
	send_to_char("You can't see anything!",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Players near you:\n\r", ch );
	found = FALSE;
	for ( d = descriptor_list; d; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
	    && ( victim = d->character ) != NULL
	    &&   !IS_NPC(victim)
	    &&   victim->in_room != NULL
	    &&   !IS_SET(victim->in_room->room_flags,ROOM_NOWHERE)
 	    &&   (!IS_ROOM_AFFECTED(victim->in_room,AFF_ROOM_FOG))
 	    &&   (is_room_owner(ch,victim->in_room)
	    ||    !room_is_private(victim->in_room))
	    &&   victim->in_room->area == ch->in_room->area
	    &&   can_see( ch, victim ) )
	    {
		found = TRUE;
		name = victim->name;
		sprintf( buf, "%s%-28s %s\n\r",
		    can_pk(ch,victim) ? "{W({rPK{W){x " : "", name,victim->in_room->name );
		send_to_char( buf, ch );
	    }
	}
	if ( !found )
	    send_to_char( "None\n\r", ch );
    }
    else if (!str_prefix(arg,"pk"))
    {
	found = FALSE;
	for ( d = descriptor_list; d; d = d->next )
	{

	    if ( d->connected == CON_PLAYING
	    && ( victim = d->character ) != NULL
	    &&   !IS_NPC(victim)
	    &&   victim->in_room != NULL
	    &&   !IS_SET(victim->in_room->room_flags,ROOM_NOWHERE)
 	    &&   (!IS_ROOM_AFFECTED(victim->in_room,AFF_ROOM_FOG))
 	    &&   (is_room_owner(ch,victim->in_room)
	    ||    !room_is_private(victim->in_room))
	    &&   victim->in_room->area == ch->in_room->area
	    &&   can_pk(ch,victim)
	    &&   can_see( ch, victim ) )
	    {
	   name = victim->name;
		found = TRUE;
		sprintf( buf, "%s%-28s %s\n\r",
		    can_pk(ch,victim) ? "{W({rPK{W){x " : "", name,victim->in_room->name );
		send_to_char( buf, ch );
	    }
	}
	if ( !found )
	    act( "You didn't find anyone in PK.", ch, NULL, arg, TO_CHAR );
    }
    else
    {
	found = FALSE;
	for ( victim = char_list; victim != NULL; victim = victim->next )
	{
	    if ( victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   !IS_AFFECTED(victim, AFF_SNEAK)
	    &&  (!IS_ROOM_AFFECTED(victim->in_room,AFF_ROOM_FOG))
	    &&   can_see( ch, victim )
	    &&   is_name( arg, victim->name ) )
	    {
		found = TRUE;
		sprintf( buf, "%s%-28s %s\n\r",
		    can_pk(ch,victim) ? "{W({rPK{W){x " : "", PERS(victim, ch),victim->in_room->name );
		send_to_char( buf, ch );
		break;
	    }
	}
	if ( !found )
	    act( "You didn't find any $T.", ch, NULL, arg, TO_CHAR );
    }

    return;
}

void do_consider( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    char *msg;
    int diff;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Consider killing whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They're not here.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim))
    {
	send_to_char("Don't even think about it.\n\r",ch);
	return;
    }

    diff = victim->level - ch->level;

         if ( diff <= -10 )
             sprintf( buf, "You could kill %s naked and weaponless.\n\r", IS_NPC(victim) ?
		victim->short_descr : victim->name );
    else if ( diff <=  -5 )
             sprintf( buf, "%s is no match for you.\n\r",
		IS_NPC(victim) ? victim->short_descr : victim->name );
    else if ( diff <=  -2 )
	     sprintf( buf, "%s looks like an easy kill.\n\r", IS_NPC(victim) ?
		victim->short_descr : victim->name );
    else if ( diff <=   1 )
	     sprintf( buf, "The perfect match!\n\r" );
    else if ( diff <=   4 )
	     sprintf( buf, "%s looks just a little tough.\n\r", IS_NPC(victim)
		? victim->short_descr : victim->name );
    else if ( diff <=   9 )
             sprintf( buf, "%s laughs at you mercilessly.\n\r",
		IS_NPC(victim) ? victim->short_descr : victim->name );
    else
             sprintf( buf, "You must have a fascination with suicide.\n\r" );

send_to_char( buf, ch );

         if ( victim->alignment >=  1 ) msg = "$N smiles happily at you.";
    else if ( victim->alignment <= -1 ) msg = "$N grins evilly at you.";
    else			        msg = "$N seems indifferent towards you.";

    act( msg, ch, NULL, victim, TO_CHAR );
    return;
}



void set_title( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
	bug( "Set_title: NPC.", 0 );
	return;
    }

    if ( title[0] != '.' && title[0] != ',' && title[0] != '!' && title[0] != '?' )
    {
	buf[0] = ' ';
	strcpy( buf+1, title );
    }
    else
    {
	strcpy( buf, title );
    }

    free_string( ch->pcdata->title );
    ch->pcdata->title = str_dup( buf );
    return;
}



void do_title( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char *pArg;
    char cEnd;

    if ( IS_NPC(ch) || ch->level < 52 )
	return;

    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    if ( arg1[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Change who's title to what?\n\r", ch );
	return;
    }

    if ( (victim = get_char_world(ch, arg1)) == NULL)
    {
    	send_to_char( "Player isn't on.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Trying to change a mob's title?\n\r", ch );
	return;
    }

    if ( ch != victim && get_trust(victim) >= get_trust(ch) )
    {
  	send_to_char( "You can't do that to that person.\n\r", ch );
	return;
    }

    if ( strlen(argument) > 45 )
	argument[45] = '\0';

    smash_tilde( argument );
    set_title( victim, argument );
    send_to_char( "Ok.\n\r", ch );
}

void set_extitle(CHAR_DATA *ch, char *title) {
    char buf[MAX_STRING_LENGTH];

    if(IS_NPC(ch)) {
        bug("Set_extitle: NPC.", 0);
        return;
    }

    if(title[0]!='.'&&title[0]!=','&&title[0]!='!'&&title[0]!='?') {
        buf[0] = ' ';
        strcpy(buf+1, title);
    } else {
        strcpy(buf, title);
    }

    free_string(ch->pcdata->extitle);
    ch->pcdata->extitle = str_dup(buf);
    return;
}

void do_extitle( CHAR_DATA *ch, char *argument ) {
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char *pArg;
    char cEnd;

    if (IS_NPC(ch))
        return;

    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while ( isspace(*argument) )
        argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
        cEnd = *argument++;

    while ( *argument != '\0' )
    {
        if ( *argument == cEnd )
        {
            argument++;
            break;
        }
        *pArg++ = *argument++;
    }
    *pArg = '\0';

    if ( arg1[0] == '\0')
    {
        send_to_char( "Change who's extra title to what?\n\r", ch );
        return;
    }

    if ( (victim = get_char_world(ch, arg1)) == NULL)
    {
        send_to_char( "That player isn't on.\n\r", ch );
        return;
    }

    if ( IS_NPC(victim) )
    {
        send_to_char( "Trying to change a mob's extra title?\n\r", ch );
        return;
    }

    if ( ch != victim && get_trust(victim) >= get_trust(ch) )
    {
        send_to_char( "You can't do that to that person.\n\r", ch );
        return;
    }

    if(argument[0]=='\0') {
	free_string(ch->pcdata->extitle);
//	ch->pcdata->extitle=NULL;
	ch->pcdata->extitle = str_dup(" ");
	send_to_char("Extitle cleared.\n\r",ch);
	return;
    }

    if ( strlen(argument) > 45 )
        argument[45] = '\0';

    smash_tilde( argument );
    set_extitle( victim, argument );
    send_to_char( "Ok.\n\r", ch );
}

void do_description( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
	return;

    if ( argument[0] != '\0' )
    {
	buf[0] = '\0';
	smash_tilde( argument );

    	if (argument[0] == '-')
    	{
            int len;
            bool found = FALSE;

            if (ch->description == NULL || ch->description[0] == '\0')
            {
                send_to_char("No lines left to remove.\n\r",ch);
                return;
            }

  	    strcpy(buf,ch->description);

            for (len = strlen(buf); len > 0; len--)
            {
                if (buf[len] == '\r')
                {
                    if (!found)  /* back it up */
                    {
                        if (len > 0)
                            len--;
                        found = TRUE;
                    }
                    else /* found the second one */
                    {
                        buf[len + 1] = '\0';
			free_string(ch->description);
			ch->description = str_dup(buf);
			send_to_char( "Your description is:\n\r", ch );
			send_to_char( ch->description ? ch->description :
			    "(None).\n\r", ch );
                        return;
                    }
                }
            }
            buf[0] = '\0';
	    free_string(ch->description);
	    ch->description = str_dup(buf);
	    send_to_char("Description cleared.\n\r",ch);
	    return;
        }
	if ( argument[0] == '+' )
	{
	    if ( ch->description != NULL )
		strcat( buf, ch->description );
	    argument++;
	    while ( isspace(*argument) )
		argument++;
	}

        if (strlen (buf) >= 1024)
	{
	    send_to_char( "Description too long.\n\r", ch );
	    return;
	}

	strcat( buf, argument );
	strcat( buf, "\n\r" );
	free_string( ch->description );
	ch->description = str_dup( buf );
    }

    send_to_char( "Your description is:\n\r", ch );
    send_to_char( ch->description ? ch->description : "(None).\n\r", ch );
    return;
}

void do_report( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    int percenta, percentb, percentc;

    if (ch->max_hit == 0)
	percenta = 0;
    else
	percenta = (ch->hit * 100) / ch->max_hit;
    if (ch->max_mana == 0)
	percentb = 0;
    else
        percentb = (ch->mana * 100) / ch->max_mana;
    if (ch->max_move == 0)
 	percentc = 0;
    else
        percentc = (ch->move * 100) / ch->max_move;

    sprintf( buf,
	"You say 'I have %d%% hp %d%% mana %d%% mv %d xp.'\n\r",
	percenta, percentb,percentc, ch->exp);
    send_to_char( buf, ch );

    sprintf( buf, "$n says 'I have %d%% hp %d%% mana %d%% mv %d xp.'",
    	percenta, percentb, percentc, ch->exp);

    act( buf, ch, NULL, NULL, TO_ROOM );

    return;
}

void do_practice( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int sn;
    bool fAll;
    int pracs=0;

    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0')
    {
	int col;

	col    = 0;
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name == NULL )
		break;

	    if (skill_table[sn].skill_level[ch->class] > 52)
		continue;
	    if ( ch->level < skill_table[sn].skill_level[ch->class]
	      || ch->pcdata->learned[sn] < 1 /* skill is not known */)
		continue;

	    sprintf( buf, "%-18s %3d%%  ",
		skill_table[sn].name, ch->pcdata->learned[sn] );
	    send_to_char( buf, ch );
	    if ( ++col % 3 == 0 )
		send_to_char( "\n\r", ch );
	}

	if ( col % 3 != 0 )
	    send_to_char( "\n\r", ch );

	sprintf( buf, "You have %d practice sessions left.\n\r",
	    ch->practice );
	send_to_char( buf, ch );
	if (IS_IMMORTAL(ch) && get_trust(ch)>58)
	send_to_char("Use 'set' to change class and see a prac list for other classes.\n\r",ch);
    }
    else
    {
	CHAR_DATA *mob;
	int adept;

	if ( !IS_AWAKE(ch) )
	{
	    send_to_char( "In your dreams, or what?\n\r", ch );
	    return;
	}

	for ( mob = ch->in_room->people; mob != NULL; mob = mob->next_in_room )
	{
	    if ( IS_NPC(mob) && IS_SET(mob->act, ACT_PRACTICE) )
		break;
	}

	if ( mob == NULL )
	{
	    send_to_char( "You can't do that here.\n\r", ch );
	    return;
	}

	if ( ch->practice <= 0 )
	{
	    send_to_char( "You have no practice sessions left.\n\r", ch );
	    return;
	}
	fAll = !str_cmp( argument, "all" );
      if ( fAll )
      {
	if(get_curr_stat(ch,STAT_INT) < 17)
		pracs=3;
	if(get_curr_stat(ch,STAT_INT) < 24)
		pracs=2;
	if(get_curr_stat(ch,STAT_INT) > 23)
		pracs=1;
	for ( sn = 0; sn < MAX_SKILL; sn++ )
		{
	    		if ( !IS_NPC(ch) && skill_table[sn].name != NULL
			&&    ch->level >= skill_table[sn].skill_level[ch->class]
			&&	ch->pcdata->learned[sn]>=1
			&&	ch->pcdata->learned[sn]<75)
			{
			if(ch->practice<pracs)
			{
				send_to_char("You do not have enough practice sessions to finish practicing everything.",ch);
				return;
			}
			ch->pcdata->learned[sn]	= 75;
			ch->practice-=pracs;
			}
		}
	send_to_char("You are now learned at all your skills.\n\r",ch);
	return;
    	}

	if ( ( sn = find_spell( ch,argument ) ) < 0
	|| ( !IS_NPC(ch)
	&&   (ch->level < skill_table[sn].skill_level[ch->class]
 	||    ch->pcdata->learned[sn] < 1 /* skill is not known */
	||    skill_table[sn].rating[ch->class] == 0)))
	{
	    send_to_char( "You can't practice that.\n\r", ch );
	    return;
	}

	adept = IS_NPC(ch) ? 100 : class_table[ch->class].skill_adept;

	if ( ch->pcdata->learned[sn] >= adept )
	{
	    sprintf( buf, "You are already learned at %s.\n\r",
		skill_table[sn].name );
	    send_to_char( buf, ch );
	}
	else
	{
	    ch->practice--;
	    ch->pcdata->learned[sn] +=
		int_app[get_curr_stat(ch,STAT_INT)].learn /
	        skill_table[sn].rating[ch->class];
	    if ( ch->pcdata->learned[sn] < adept )
	    {
		act( "You practice $T.",
		    ch, NULL, skill_table[sn].name, TO_CHAR );
		act( "$n practices $T.",
		    ch, NULL, skill_table[sn].name, TO_ROOM );
	    }
	    else
	    {
		ch->pcdata->learned[sn] = adept;
		act( "You are now learned at $T.",
		    ch, NULL, skill_table[sn].name, TO_CHAR );
		act( "$n is now learned at $T.",
		    ch, NULL, skill_table[sn].name, TO_ROOM );
	    }
	}
    }
    return;
}


/*
 * 'Wimpy' originally by Dionysos.
 */
void do_wimpy( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int wimpy;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
        {
        if(ch->level >= 30)
	wimpy = ch->max_hit / 5;
        else
        wimpy = 20;
        }
    else
	wimpy = atoi( arg );

    if ( wimpy < 0 )
    {
	send_to_char( "Your courage exceeds your wisdom.\n\r", ch );
	return;
    }
    if (ch->level >= 30)
    {
    if ( wimpy > ch->max_hit/2 )
    {
	send_to_char( "Such cowardice ill becomes you.\n\r", ch );
	return;
    }

    ch->wimpy	= wimpy;
    sprintf( buf, "Wimpy set to %d hit points.\n\r", wimpy );
    send_to_char( buf, ch );
    }
    else
    {
    if ( wimpy > 50)
    {
        send_to_char( "Such cowardice ill becomes you.\n\r", ch );
        return;
    }
    ch->wimpy = wimpy;
    sprintf( buf, "Wimpy set to %d%% hit points.\n\r", wimpy );
    send_to_char( buf, ch );
    }
    return;
}



void do_password( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char *pArg;
    char *pwdnew;
    char *p;
    char cEnd;

    if ( IS_NPC(ch) )
	return;

    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    pArg = arg2;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: password <old> <new>.\n\r", ch );
	return;
    }

    if ( strcmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ) )
    {
	WAIT_STATE( ch, 40 );
	send_to_char( "Wrong password.  Wait 10 seconds.\n\r", ch );
	return;
    }

    if ( strlen(arg2) < 5 )
    {
	send_to_char(
	    "New password must be at least five characters long.\n\r", ch );
	return;
    }

    /*
     * No tilde allowed because of player file format.
     */
    pwdnew = crypt( arg2, ch->name );
    for ( p = pwdnew; *p != '\0'; p++ )
    {
	if ( *p == '~' )
	{
	    send_to_char(
		"New password not acceptable, try again.\n\r", ch );
	    return;
	}
    }

    free_string( ch->pcdata->pwd );
    ch->pcdata->pwd = str_dup( pwdnew );
    save_char_obj( ch );
    send_to_char( "Ok.\n\r", ch );
    return;
}

CHAR_DATA * get_char_room( CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *rch;
    int number;
    int count;

    number = number_argument(argument, arg);
    count = 0;
    if (!str_cmp(arg,"self") )
        return ch;
    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
        if (!can_see(ch,rch) )
            continue;
        if (
		arg[0] != '\0' &&
		!is_exact_name (arg, rch->name)
	   )
            continue;
        if ( ++count == number )
            return rch;
    }

    return NULL;
}

void do_eye_of_the_predator(CHAR_DATA *ch,char *argument)
{
    AFFECT_DATA af;

    if ( (get_skill(ch,gsn_eye_of_the_predator) == 0)
    || (ch->level < skill_table[gsn_eye_of_the_predator].skill_level[ch->class]) )
    {
	send_to_char("Huh?\n\r",ch);
 	return;
    }

    if (cabal_down(ch,CABAL_ACADIAN))
    	return;

    /*if(!is_affected(ch,gsn_cloak_form)) {
	send_to_char("You may only invoke that power while concealed.\n\r",ch);
	return;
    }*/

    if (is_affected(ch,gsn_eye_of_the_predator) )
    {
	send_to_char("You already watch things through preadatory eyes.\n\r",ch);
	return;
    }
if (ch->mana < 40)
    {
	send_to_char("You do not have the mana.\n\r",ch);
	return;
    }

    if (number_percent() > ch->pcdata->learned[gsn_eye_of_the_predator] )
    {
	send_to_char("Your vision sharpens but then fades.\n\r",ch);
	check_improve(ch,gsn_eye_of_the_predator,FALSE,1);
	ch->mana -= 20;
	return;
    }

init_affect(&af);
    af.where = TO_AFFECTS;
    af.type = gsn_eye_of_the_predator;
af.aftype = AFT_POWER;
    af.level = ch->level;
    af.modifier = 0;
    af.location = 0;
    af.duration = ch->level/2;
    af.bitvector = AFF_DETECT_INVIS;
    send_to_char("You begin watching things through the eyes of a hunter.\n\r",ch);
    send_to_char("The shadows are revealed to you.\n\r",ch);
    affect_to_char(ch, &af);
    af.bitvector = AFF_DETECT_HIDDEN;
    affect_to_char( ch, &af );
/*
    if(ch->level>50&&!is_affected(ch,gsn_acute_vision)) {
    	af.type = gsn_acute_vision;
    	af.bitvector = 0;
    	affect_to_char(ch,&af);
	send_to_char("You begin to see through the twigs and brush.\n\r",ch);
    }
*/
    ch->mana -= 40;
	check_improve(ch,gsn_eye_of_the_predator,TRUE,1);

    return;
}



void do_wanted(CHAR_DATA *ch, char *argument)
{
        CHAR_DATA *criminal;
        char arg[MAX_INPUT_LENGTH];
        char status[MAX_INPUT_LENGTH];
        char *next_arg;
        bool wanted;

        wanted = FALSE;
        next_arg = one_argument(argument,arg);
        one_argument(next_arg,status);
        if (IS_NPC(ch))
        return;
        if ( (get_skill(ch,gsn_wanted) == 0)
        || (ch->level < skill_table[gsn_wanted].skill_level[ch->class]) )
        {
        send_to_char("Huh?\n\r",ch);
        return;
        }


        if (!str_cmp(status,"yes"))
                wanted = TRUE;
        else if (!str_cmp(status,"no"))
        ;
        else
        {
        send_to_char("You must use WANTED PLAYER (YES/NO).\n\r",ch);
        return;
        }

        if (( criminal = get_char_world(ch,arg) ) == NULL )
        {
        send_to_char("That person isn't here.\n\r",ch);
        return;
        }
        if (IS_NPC(criminal))
        {
        send_to_char("You can only flag other players.\n\r",ch);
        return;
        }

        if (wanted)
        {
        if (IS_SET(criminal->act,PLR_CRIMINAL))
          {
          send_to_char("That person is already a criminal.\n\r",ch);
          return;
          }
        else
          {
          SET_BIT(criminal->act,PLR_CRIMINAL);
          send_to_char("You are now WANTED!!\n\r",criminal);
          act("$N is now a criminal.",ch,0,criminal,TO_CHAR);
	criminal->pcdata->wanteds += 1;
          return;
          }
        }

        if (!IS_SET(criminal->act,PLR_CRIMINAL) )
          {
          send_to_char("That person is not a criminal.\n\r",ch);
          return;
          }
        else
          {
          REMOVE_BIT(criminal->act,PLR_CRIMINAL);
          send_to_char("You are no longer wanted.\n\r",criminal);
          act("$N is no longer wanted.",ch,0,criminal,TO_CHAR);
	criminal->pcdata->wanteds -= 1;
          }
        return;
}



void do_balance(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *banker;
    char buf[100];

    for (banker = ch->in_room->people; banker != NULL; banker = banker->next_in_room)
    {
        if (IS_NPC(banker) && IS_SET(banker->act,ACT_BANKER))
		break;
    }
    if (banker == NULL)
    {
        send_to_char("You can't do that here.\n\r",ch);
        return;
    }

    sprintf(buf,"You have %ld gold coins and %ld silver in your account.\n\r",ch->gold_bank,ch->silver_bank);
    if (ch->gold_bank == 0 && ch->silver_bank == 0)
        sprintf(buf,"You have no account here!\n\r");

    send_to_char(buf, ch);
    return;
}

void do_withdraw(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *banker;
    char arg[MAX_STRING_LENGTH];
    int amount;
    char buf[MAX_STRING_LENGTH];
    int charges;

    for (banker = ch->in_room->people; banker != NULL; banker = banker->next_in_room)
    {
        if (IS_SET(banker->act,ACT_BANKER))
            break;
    }
    if (banker == NULL)
    {
        send_to_char("You can't do that here.\n\r",ch);
        return;
    }

    argument = one_argument(argument,arg);
    if (arg[0] == '\0')
    {
        send_to_char("Withdraw how much of which coin type?\n\r",ch);
        return;
    }
    if (!is_number(arg))
    {
        send_to_char("Withdraw how much of which type of coin?\n\r",ch);
        return;
    }

    amount = atoi(arg);
    argument = one_argument(argument,arg);
    if (amount <= 0 || (str_cmp(arg, "gold") && str_cmp(arg,"silver")))
    {
      sprintf(buf,"%s Sorry, withdraw how much of which coin type?",ch->name);
      do_tell(banker,buf);
      return;
    }
    charges = 5 * amount;
    charges /= 100;

    if (!str_cmp(arg,"gold"))
    {
    if (ch->gold_bank < amount)
    {
            act("$N tells you, 'Sorry you do not have we don't give loans.'",ch,0,banker,TO_CHAR);
        return;
    }
    ch->gold_bank -= amount;
    ch->gold += amount;
    ch->gold -= charges;
    }
    else if (!str_cmp(arg,"silver"))
    {
        if (ch->silver_bank < amount)
        {
            act("$N tells you, 'You don't have that much silver in the bank.'",ch,0,banker,TO_CHAR);
            return;
        }
        ch->silver_bank -= amount;
        ch->silver += amount;
	ch->silver -= charges;
    }

    sprintf(buf,"You withdraw %d %s.\n\r",amount,arg);
    send_to_char(buf,ch);
    sprintf(buf,"You are charged a small fee of %d %s.\n\r",charges, !str_cmp(arg,"gold") ? "gold" : "silver");

    send_to_char(buf,ch);
    return;
}

void do_deposit(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *banker;
    char arg[MAX_STRING_LENGTH];
    int amount;
    char buf[MAX_STRING_LENGTH];

    for (banker = ch->in_room->people; banker != NULL; banker = banker->next_in_room)
    {
        if (IS_NPC(banker) && IS_SET(banker->act,ACT_BANKER))
		break;
    }

    if (banker == NULL)
    {
        send_to_char("You can't do that here.\n\r",ch);
        return;
    }

     argument = one_argument(argument,arg);
    if (arg[0] == '\0')
    {
        send_to_char("Deposit how much of which coin type?\n\r",ch);
        return;
    }
    if (!is_number(arg))
    {
        send_to_char("Deposit how much of which type of coin?\n\r",ch);
        return;
    }

    amount = atoi(arg);
    argument = one_argument(argument,arg);
    if (amount <= 0 || (str_cmp(arg, "gold") && str_cmp(arg,"silver")))
    {
      act("$N tells you, 'Sorry, deposit how much of which coin type?'",ch,0,banker,TO_CHAR);
      return;
    }

    if (!str_cmp(arg,"gold"))
    {
        if (ch->gold < amount)
        {
            act("$N tells you, 'You don't have that much gold on you!'",ch,0,banker,TO_CHAR);
            return;
        }
        ch->gold_bank += amount;
        ch->gold -= amount;
    }
    else if (!str_cmp(arg,"silver"))
    {
        if (ch->silver < amount)
        {
            act("$N tells you, 'You don't have that much silver on you!'",ch,0,banker,TO_CHAR);
            return;
        }
        ch->silver_bank += amount;
        ch->silver -= amount;
    }

    sprintf(buf,"You deposit %d %s.\n\r",amount,arg);
    send_to_char(buf,ch);
    sprintf(buf,"Your new balance is %ld gold, %ld silver.\n\r",ch->gold_bank,ch->silver_bank);
    send_to_char(buf,ch);
    return;
}


void showDesc(CHAR_DATA *victim, CHAR_DATA *ch)
{
    if (victim->description[0] != '\0')
        send_to_char (victim->description, ch);
    else
        act ("You see nothing special about $M.", ch, NULL, victim, TO_CHAR);
}

void showHealth(CHAR_DATA *victim, CHAR_DATA *ch)
{
    char buf[MAX_STRING_LENGTH];
    int percent = 0;

    if (victim->max_hit > 0)
        percent = (100 * victim->hit) / victim->max_hit;
    else
        percent = -1;

    strcpy (buf, PERS (victim, ch));

    if (percent >= 100)
        strcat (buf, " is in excellent condition.\n\r");
    else if (percent >= 90)
        strcat (buf, " has a few scratches.\n\r");
    else if (percent >= 75)
        strcat (buf, " has some small wounds and bruises.\n\r");
    else if (percent >= 50)
        strcat (buf, " has quite a few wounds.\n\r");
    else if (percent >= 30)
        strcat (buf, " is bleeding profusely.\n\r");
    else if (percent >= 15)
        strcat (buf, " is writhing in agnoy.\n\r");
    else if (percent >= 0)
        strcat (buf, " is convulsing on the ground.\n\r");
    else
        strcat (buf, " is bleeding to death.\n\r");

    buf[0] = UPPER (buf[0]);
    send_to_char (buf, ch);
    }

void showEq(CHAR_DATA *victim, CHAR_DATA *ch)
    {
    OBJ_DATA *obj;
	char obj_vnum[MSL];
    bool found = FALSE;
    int iWear = 0;

    for (iWear = 0; iWear < MAX_WEAR; iWear++)
{
        if ((obj = get_eq_char (victim, iWear)) != NULL
            && can_see_obj (ch, obj))
	{
            if (!found)
    {
                send_to_char ("\n\r", ch);
                act ("$N is using:", ch, NULL, victim, TO_CHAR);
                found = TRUE;
    }
            send_to_char (where_name[iWear], ch);
            send_to_char (format_obj_to_char (obj, ch, TRUE), ch);
			if (IS_IMMORTAL(ch) && IS_SET(ch->act,PLR_HOLYLIGHT))
			{
    			sprintf(obj_vnum,"{x  [{W%5d{x]",obj->pIndexData->vnum);
				send_to_char(obj_vnum,ch);
			}
            send_to_char ("\n\r", ch);
        }
    }
}


void showInventory(CHAR_DATA *victim, CHAR_DATA *ch)
    {
    if (victim != ch && !IS_NPC (ch)
        && number_percent () < get_skill (ch, gsn_peek))
    {
        send_to_char ("\n\rYou peek at the inventory:\n\r", ch);
        check_improve (ch, gsn_peek, TRUE, 4);
        show_list_to_char (victim->carrying, ch, TRUE, TRUE);
    }
}

void do_records(CHAR_DATA *ch,char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char buf1[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
 	int count = 0;

    if (ch->cabal != CABAL_VIGILANTE && !IS_IMMORTAL(ch))
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}

 	buf1[0] = '\0';
	sprintf(buf,"Listing of current active players :\n\r");

	for (victim = char_list; victim != NULL; victim = victim->next)
	{
	if (IS_NPC(victim))
		continue;
	if (victim->pcdata->wanteds > 0)
	{
	count++;
	sprintf(buf1,"%s has %d flags.\n\r",victim->name,victim->pcdata->wanteds);
	strcat(buf,buf1);
	}
	}
	if (count == 0)
	strcat(buf,"None in the realms right now have had a flag before.\n\r");

	send_to_char(buf,ch);
	send_to_char("\n\r",ch);
	return;
}

void do_ratings(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	if (IS_NPC(ch))
	return;


	argument = one_argument( argument, buf);

	if (!(ch->level > LEVEL_HERO)) buf[0]='\0';

	if ( buf[0] == '\0' )
	{
	send_to_char("\n\r   Your PK record:\n\r",ch);
	send_to_char("----------------------\n\r",ch);
	send_to_char(" PK kills | PK deaths\n\r",ch);
	send_to_char("----------------------\n\r",ch);
	sprintf(buf,"    %d           %d",ch->pcdata->kills[PK_KILLS], ch->pcdata->killed[PK_KILLED]);
	send_to_char(buf,ch);
	send_to_char("\n\r",ch);
	} else {

	victim = get_char_world(ch,buf);
	if (victim == NULL) {
		return;
	}
	if (IS_NPC(victim)) {
		return;
	}
	send_to_char("\n\r",ch);
	act( " $N's PK record:", ch, NULL, victim, TO_CHAR );
	send_to_char("----------------------\n\r",ch);
	send_to_char(" PK kills | PK deaths\n\r",ch);
	send_to_char("----------------------\n\r",ch);
	sprintf(buf,"    %d           %d",victim->pcdata->kills[PK_KILLS], victim->pcdata->killed[PK_KILLED]);
	send_to_char(buf,ch);
	send_to_char("\n\r",ch);
	}
	return;
}

void do_lore( CHAR_DATA *ch, char *argument )
{
  char object_name[MAX_INPUT_LENGTH + 100];

  OBJ_DATA *obj;
  char buf[MAX_STRING_LENGTH];
  argument = one_argument(argument, object_name);

  if (get_skill(ch,gsn_lore) == 0)
	{
      send_to_char( "You don't know how to do that.\n\r", ch );
      return;
	}
  if ( ( obj = get_obj_carry( ch, object_name, ch ) ) == NULL )
    {
      send_to_char( "You are not carrying that.\n\r", ch );
      return;
    }

  send_to_char("You ponder the item.\n\r",ch);
  if (number_percent() < get_skill(ch,gsn_lore)) {
    sprintf(buf,
             "Object '%s' is type %s, extra flags %s.\n\rWeight is %d, value is %d, level is %d.\n\r",

             obj->name,
             item_name(obj->item_type),
             extra_bit_name( obj->extra_flags ),
             obj->weight,
             obj->cost,
             obj->level
             );
	send_to_char(buf,ch);

    switch ( obj->item_type )
      {
      case ITEM_SCROLL:
      case ITEM_POTION:
      case ITEM_PILL:
        sprintf(buf,  "Some level spells of:");

        if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )
          {
            send_to_char( " '", ch );
            send_to_char( skill_table[obj->value[1]].name, ch );
            send_to_char( "'", ch );
          }

        if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )
          {
            send_to_char( " '", ch );
            send_to_char( skill_table[obj->value[2]].name, ch );
            send_to_char( "'", ch );
          }

        if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
          {
            send_to_char( " '", ch );
            send_to_char( skill_table[obj->value[3]].name, ch );
            send_to_char( "'", ch );
          }

        if (obj->value[4] >= 0 && obj->value[4] < MAX_SKILL)
          {
            send_to_char(" '",ch);
            send_to_char(skill_table[obj->value[4]].name,ch);
            send_to_char("'",ch);
          }

        send_to_char( ".\n\r", ch );
        break;

      case ITEM_WAND:
      case ITEM_STAFF:
        sprintf(buf,  "Has some charges of some level" );

        if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
          {
            send_to_char( " '", ch );
            send_to_char( skill_table[obj->value[3]].name, ch );
            send_to_char( "'", ch );
          }

        send_to_char( ".\n\r", ch );
        break;

      case ITEM_DRINK_CON:
        sprintf(buf, "It holds %s-colored %s.\n\r",
                liq_table[obj->value[2]].liq_color,
                liq_table[obj->value[2]].liq_name);
        break;

      case ITEM_CONTAINER:
        sprintf(buf, "Capacity: %d#  Maximum weight: %d#  flags: %s\n\r",
                obj->value[0], obj->value[3], cont_bit_name(obj->value[1]));
        if (obj->value[4] != 100)
          {
            sprintf(buf, "Weight multiplier: %d%%\n\r",
                    obj->value[4]);
          }
        break;

      case ITEM_WEAPON:
        send_to_char("Weapon type is ",ch);
        switch (obj->value[0])
          {
          case(WEAPON_EXOTIC): send_to_char("exotic.\n\r",ch);        break;
          case(WEAPON_SWORD)  : send_to_char("sword.\n\r",ch);  break;
          case(WEAPON_DAGGER) : send_to_char("dagger.\n\r",ch); break;
          case(WEAPON_SPEAR)    : send_to_char("spear.\n\r",ch);  break;
          case(WEAPON_MACE)     : send_to_char("mace.\n\r",ch);    break;
          case(WEAPON_AXE)      : send_to_char("axe.\n\r",ch);          break;
          case(WEAPON_FLAIL)    : send_to_char("flail.\n\r",ch);        break;
          case(WEAPON_WHIP)     : send_to_char("whip.\n\r",ch);         break;
          case(WEAPON_POLEARM): send_to_char("polearm.\n\r",ch);        break;
          case(WEAPON_STAFF): send_to_char("staff.\n\r",ch);        break;
          default               : send_to_char("unknown.\n\r",ch);      break;
          }
        if (obj->pIndexData->new_format)
          sprintf(buf, "Damage is %dd%d (average %d).\n\r",
                  obj->value[1],obj->value[2],
                  (1 + obj->value[2]) * obj->value[1] / 2);
        else
          sprintf(buf,  "Damage is %d to %d (average %d).\n\r",
                   obj->value[1], obj->value[2],
                   ( obj->value[1] + obj->value[2] ) / 2 );
        if (obj->value[4])  /* weapon flags */
          {
            sprintf(buf, "Weapons flags: %s\n\r",weapon_bit_name(obj->value[4]));
          }
        break;

      case ITEM_ARMOR:
        sprintf(buf,
                 "Armor class is %d pierce, %d bash, %d slash, and %d vs. magic.\n\r",
                 obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
        break;
      }
	send_to_char(buf,ch);
	check_improve(ch,skill_lookup("lore"),TRUE,1);
  }
  else
  {
    check_improve(ch,skill_lookup("lore"),FALSE,1);
  }
  return;
}

bool isCabalItem( OBJ_DATA *obj )
{
    if (obj->pIndexData->vnum == OBJ_VNUM_RAVAGER
    || obj->pIndexData->vnum == OBJ_VNUM_COVEN
    || obj->pIndexData->vnum == OBJ_VNUM_VIGILANTE
    || obj->pIndexData->vnum == OBJ_VNUM_BRIAR
    || obj->pIndexData->vnum == OBJ_VNUM_PROTECTOR
    || obj->pIndexData->vnum == OBJ_VNUM_ACADIAN)
	{
		return TRUE;
	}
    else
	{
		return FALSE;
	}
}

bool isNewbie( CHAR_DATA *ch )
{
	if (IS_NPC(ch))
		return FALSE;

	if (ch->pcdata->newbie==TRUE)
		return TRUE;

	return FALSE;
}

bool isCabalGuard( CHAR_DATA *ch )
{
	if (!IS_NPC(ch))
		return FALSE;

    if (ch->pIndexData->vnum == MOB_VNUM_RAVAGER
    || ch->pIndexData->vnum == MOB_VNUM_COVEN
    || ch->pIndexData->vnum == MOB_VNUM_VIGILANTE
    || ch->pIndexData->vnum == MOB_VNUM_BRIAR
    || ch->pIndexData->vnum == MOB_VNUM_PROTECTOR
    || ch->pIndexData->vnum == MOB_VNUM_ACADIAN)
	{
		return TRUE;
	}
    else
	{
		return FALSE;
	}
}

void debug_string( const char *str )
{
	FILE *fp;

   if ( ( fp = fopen( DEBUG_LOG_FILE, "a" ) ) != NULL )
    {
    fprintf( fp, "DEBUG :: %s\n", str );
    fclose(fp);
    }
}

void do_xlook( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	int iWear, foundIR;
	char buf[MAX_STRING_LENGTH];
	char obj_vnum[MSL];
      CHAR_DATA *vch;
      CHAR_DATA *vch_next;

	if (argument[0] == '\0') {
		send_to_char("Syntax: xlook <char>\n\r", ch );
		return;
	}

	if ( ( victim = get_char_world( ch, argument ) ) == NULL )
	{
		send_to_char( "No such player found.\n\r", ch );
		return;
	}

	if ( !can_see( ch, victim ) )
	{
		send_to_char( "No such player found.\n\r", ch );
		return;
	}

	sprintf(buf,"%s is at '%s' (%d) of area '%s'",
			//victim->name,
                        IS_NPC(victim) ? victim->short_descr : victim->name,
			victim->in_room->name,
			victim->in_room->vnum,
			victim->in_room->area->name);

	send_to_char (buf, ch);
	send_to_char ("\n\r", ch);
        sprintf(buf, "%ihp/%ihp  %im/%im  %imv/%imv\n\r",ch->hit,ch->max_hit,ch->mana,ch->max_mana,ch->move,ch->max_move);

	foundIR = 0;
	send_to_char ("In room: ", ch);
	for (vch = victim->in_room->people; vch != NULL; vch = vch_next)
	{
		vch_next = vch->next_in_room;
		if (vch == victim) {
			continue;
		}
		if (IS_NPC(vch)) {
			continue;
		}
		if (!can_see(ch,vch))
			continue;
		if (is_same_group(vch,victim) )
		{
			send_to_char ("(GROUP)",ch);
		}
		send_to_char(vch->name,ch);
		send_to_char(" ",ch);
		foundIR++;
	}
	if (foundIR == 0) {
		send_to_char("none.",ch);
	}

	if (victim->fighting != NULL) {
		send_to_char ("\n\rFighting: ", ch);
		send_to_char ((IS_NPC(victim->fighting) ? victim->fighting->short_descr : victim->fighting->name), ch);
	} else {
		send_to_char ("\n\rFighting: none.", ch);
	}

	if(victim->description)
	{
		sprintf(buf,"\n\rDescription: \n\r%s",victim->description);
		send_to_char(buf,ch);
	}
	send_to_char ("\n\rEquipment:\n\r", ch);
	for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
	{
		if ( ( obj = get_eq_char( victim, iWear ) ) != NULL
		&&   can_see_obj( ch, obj ) )
		{
			send_to_char(where_name[iWear], ch);
			send_to_char(format_obj_to_char( obj, ch, TRUE ), ch);
			if (IS_IMMORTAL(ch) && IS_SET(ch->act,PLR_HOLYLIGHT))
			{
			sprintf(obj_vnum,"{x  [{W%d{x]",obj->pIndexData->vnum);
			send_to_char(obj_vnum,ch);
			}
			send_to_char("\n\r", ch);
		}
	}

	send_to_char ("\n\r", ch);
	send_to_char ("Inventory:\n\r", ch);
	show_list_to_char( victim->carrying, ch, TRUE, FALSE );
	return;
}

void do_ability( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int sn;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        send_to_char( "Check your ability in the skill or spell?\n\r", ch );
        return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
        for ( sn = 0; sn < MAX_SKILL; sn++ )
        {
        if ( skill_table[sn].name == NULL )
                break;
		if (ch->pcdata->learned[sn] <= 1)
		sprintf( buf, "You are not learned in the skill or spell '%s'.\n\r",
		skill_table[sn].name);
		else
            sprintf( buf, "Your ability in the skill or spell '%s' is %d%%.\n\r",
            skill_table[sn].name, ch->pcdata->learned[sn]);
            send_to_char( buf, ch );
        }
     }
     else
     {
        if ( ( sn = skill_lookup( arg ) ) < 0 )
        {
            send_to_char( "No such skill or spell.\n\r", ch );
            return;
        }
		if (ch->pcdata->learned[sn] <= 1)
		sprintf( buf, "You are not learned in the skill or spell '%s'.\n\r",
		skill_table[sn].name);
		else
            sprintf( buf, "Your ability in the skill or spell '%s' is %d%%.\n\r",
            skill_table[sn].name, ch->pcdata->learned[sn]);
            send_to_char( buf, ch );
        }
   return;
}

char * get_where_name(int iWear)
{
        return where_name[iWear];
}

/* moved from scan.c to here */
char *const distance[7]=
{
" **** 1 %s **** ", " **** 2 %s **** ", " **** 3 %s **** ",
" **** 4 %s **** ", " **** 5 %s **** ", " **** 6 %s **** ", " **** 7 %s **** "
};

void show_char_to_char_0 args((CHAR_DATA *victim, CHAR_DATA *ch));
void scan_list           args((ROOM_INDEX_DATA *scan_room, CHAR_DATA *ch,
                               sh_int depth, sh_int door));
void do_scan(CHAR_DATA *ch, char *argument)
{
   extern char *const dir_name[];
   char arg1[MAX_INPUT_LENGTH], buf[MAX_INPUT_LENGTH];
   ROOM_INDEX_DATA *scan_room;
   EXIT_DATA *pExit;
   sh_int door, depth;

   argument = one_argument(argument, arg1);

        if (!str_cmp(arg1, "n") || !str_cmp(arg1, "north")) door = 0;
   else if (!str_cmp(arg1, "e") || !str_cmp(arg1, "east"))  door = 1;
   else if (!str_cmp(arg1, "s") || !str_cmp(arg1, "south")) door = 2;
   else if (!str_cmp(arg1, "w") || !str_cmp(arg1, "west"))  door = 3;
   else if (!str_cmp(arg1, "u") || !str_cmp(arg1, "up" ))   door = 4;
   else if (!str_cmp(arg1, "d") || !str_cmp(arg1, "down"))  door = 5;
   else if (!str_cmp(arg1, "a") || !str_cmp(arg1, "all"))   door = -1;
   else { send_to_char("Which way do you want to scan?\n\r", ch); return; }
   if(door==-1)
   {
	do_scan(ch,"north");
	do_scan(ch,"east");
	do_scan(ch,"south");
	do_scan(ch,"west");
	do_scan(ch,"up");
	do_scan(ch,"down");
	return;
   }

   act("You scan $T.", ch, NULL, dir_name[door], TO_CHAR);
   act("$n scans $T.", ch, NULL, dir_name[door], TO_ROOM);
   sprintf(buf, "Looking %s you see:\n\r", dir_name[door]);

   scan_room = ch->in_room;

   for (depth = 1; depth <= (ch->level/10)+1; depth++)
   {
      if ((pExit = scan_room->exit[door]) != NULL)
      {
	if (!IS_SET(pExit->exit_info, EX_CLOSED))
	{
         scan_room = pExit->u1.to_room;
	 if (scan_room == NULL) return;
         scan_list(pExit->u1.to_room, ch, depth, door);
	 send_to_char("\n\r", ch);
	}
      }
   }
   return;
}

void scan_list(ROOM_INDEX_DATA *scan_room, CHAR_DATA *ch, sh_int depth,
               sh_int door)
{
   CHAR_DATA *rch;
   extern char *const dir_name[];
   extern char *const distance[];
   char buf[MAX_INPUT_LENGTH], buf2[MAX_INPUT_LENGTH];

   buf[0] = '\0';

   if (scan_room == NULL) return;
   sprintf(buf2, distance[(depth-1)], dir_name[door]);
   strcat(buf, buf2);
   send_to_char(buf, ch);
   send_to_char("\n\r", ch);
   for (rch=scan_room->people; rch != NULL; rch=rch->next_in_room)
   {
      if (rch == ch) continue;
      if (!IS_NPC(rch) && rch->invis_level > get_trust(ch)) continue;
      if (can_see(ch, rch)) show_char_to_char_0(rch, ch);
   }
   return;
}

