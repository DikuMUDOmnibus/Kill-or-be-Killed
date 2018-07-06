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
 **************************************************************************/

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

/***************************************************************************
*       POSmud code is copyright (C) 1999-2000 and for exclusive use of    *
*       POSmud only. Hah!                                                  *
***************************************************************************/

#include "include.h"

/* command procedures needed */
DECLARE_DO_FUN(do_quit	);
DECLARE_DO_FUN(do_look);
DECLARE_DO_FUN(do_yell);
DECLARE_DO_FUN(do_illithid_yell);
DECLARE_DO_FUN(do_delete);
DECLARE_DO_FUN(do_revert);
DECLARE_DO_FUN(do_shadowform);
#define PDEATH_LOG_FILE	"permdeaths.txt"
#define DEATH_LOG_FILE	"tempdeaths.txt"
#define LOGIN_LOG_FILE	"logins.txt"

void obj_say_lordly args((CHAR_DATA *ch,OBJ_DATA *obj,char *argument));
void obj_say_heavenly_sceptre args((CHAR_DATA *ch,OBJ_DATA *obj,char *argument));
void mob_say_lady args((CHAR_DATA *ch, char *argument));
void say_prog_summon_demon args((CHAR_DATA *ch,char *argument));
void say_prog_raise_shadowlord args((CHAR_DATA *ch,char *argument));
void say_prog_karguska args((CHAR_DATA *ch,char *argument));
//static const char *garble(CHAR_DATA *ch, const char *i);
char *garble(CHAR_DATA *ch, char *i);
//static const char *upstring(CHAR_DATA *ch, const char *i);
const char *upstring(const char *i);
bool arena;
void perm_death_log(CHAR_DATA *ch, int deltype);
void report_cabal_items(CHAR_DATA *ch, char *argument);

/* RT code to delete yourself */

void do_delet( CHAR_DATA *ch, char *argument)
{
    send_to_char("You must type the full command to delete yourself.\n\r",ch);
}

void do_delete( CHAR_DATA *ch, char *argument)
{
   char strsave[MAX_INPUT_LENGTH];

   if (IS_NPC(ch))
	return;

if (isShifted(ch))
    {
	send_to_char("You can't delete while in another form.\n\r",ch);
	return;
    }

   if (is_affected(ch,gsn_cloak_form))
   {
	send_to_char("You must uncloak before you delete!\n\r",ch);
	return;
   }

   if (ch->pcdata->confirm_delete)
   {
	if (argument[0] != '\0')
	{
	    send_to_char("Delete status removed.\n\r",ch);
	    ch->pcdata->confirm_delete = FALSE;
	    return;
	}
	else
	{
		perm_death_log(ch,1);
    	    sprintf( strsave, "%s%s.plr", PLAYER_DIR, capitalize( ch->name ) );
	    wiznet("$N turns $Mself into line noise.",ch,NULL,0,0,0);
	    ch->pause = 0;
	    while (ch->affected) {
	         affect_remove(ch,ch->affected);
          }
	    stop_fighting(ch,TRUE);
	    do_quit(ch, "");
	    remove(strsave);
	    return;
 	}
    }

    if (argument[0] != '\0')
    {
	send_to_char("Just type delete. No argument.\n\r",ch);
	return;
    }

    send_to_char("Type delete again to confirm this command.\n\r",ch);
    send_to_char("WARNING: this command is irreversible.\n\r",ch);
    send_to_char("Typing delete with an argument will undo delete status.\n\r",
	ch);
    ch->pcdata->confirm_delete = TRUE;
    wiznet("$N is contemplating deletion.",ch,NULL,0,0,get_trust(ch));
    //WAIT_STATE(ch,PULSE_VIOLENCE*3);
}


/* RT code to display channel status */

void do_channels( CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];

    /* lists all channels and their status */
    send_to_char("   channel     status\n\r",ch);
    send_to_char("---------------------\n\r",ch);

    send_to_char("auction        ",ch);
    if (!IS_SET(ch->comm,COMM_NOAUCTION))
      send_to_char("ON\n\r",ch);
    else
      send_to_char("OFF\n\r",ch);

    send_to_char("Q/A            ",ch);
    if (!IS_SET(ch->comm,COMM_NOQUESTION))
      send_to_char("ON\n\r",ch);
    else
      send_to_char("OFF\n\r",ch);

    if (IS_IMMORTAL(ch))
    {
      send_to_char("god channel    ",ch);
      if(!IS_SET(ch->comm,COMM_NOWIZ))
        send_to_char("ON\n\r",ch);
      else
        send_to_char("OFF\n\r",ch);
    }

    send_to_char("tells          ",ch);
    if (!IS_SET(ch->comm,COMM_DEAF))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

     send_to_char("ooctell          ",ch);
    if (!IS_SET(ch->comm,COMM_NO_OOC_TELL))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("quiet mode     ",ch);
    if (IS_SET(ch->comm,COMM_QUIET))
      send_to_char("ON\n\r",ch);
    else
      send_to_char("OFF\n\r",ch);

    if (IS_SET(ch->comm,COMM_SNOOP_PROOF))
	send_to_char("You are immune to snooping.\n\r",ch);

    if (ch->lines != PAGELEN)
    {
	if (ch->lines)
	{
	    sprintf(buf,"You display %d lines of scroll.\n\r",ch->lines+2);
	    send_to_char(buf,ch);
        }
	else
	    send_to_char("Scroll buffering is off.\n\r",ch);
    }

    if (IS_SET(ch->comm,COMM_NOTELL))
      send_to_char("You cannot use tell.\n\r",ch);

    if (IS_SET(ch->comm,COMM_NOCHANNELS))
     send_to_char("You cannot use channels.\n\r",ch);

    if (IS_SET(ch->comm,COMM_NOEMOTE))
      send_to_char("You cannot show emotions.\n\r",ch);

}

/* RT deaf blocks out all shouts */
void do_ooctell( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int count, number;
    char arg2[MAX_STRING_LENGTH];

    if(isShifted(ch) && !IS_IMMORTAL(ch))
    {

	send_to_char("You can't seem to form the words.\n\r",ch);
	return;
    }

    if  (is_affected(ch,gsn_silence))
    {
	send_to_char("You cannot find the words within you.\n\r",ch);
	return;
    }

    if(is_affected(ch,gsn_tigerclaw))
    {
	send_to_char("Your throat constricts too painfully.\n\r",ch);
	return;
    }

    if (is_affected(ch, gsn_gag) )
    {
    	send_to_char("You find difficultly talking.\n\r",ch);
    	return;
    }

    if ( IS_SET(ch->comm, COMM_QUIET) )
    {
	send_to_char( "You must turn off quiet mode first.\n\r", ch);
	return;
    }

    if (IS_SET(ch->comm,COMM_DEAF))
    {
	send_to_char("You must turn off deaf mode first.\n\r",ch);
	return;
    }

    argument = one_argument( argument, arg );

    if ( argument[0] == '\0' )
	{
	  if (IS_SET(ch->comm,COMM_NO_OOC_TELL))
	{
	   send_to_char("OOC channel on!\n\r",ch);
	   REMOVE_BIT(ch->comm,COMM_NO_OOC_TELL);
	}
	  else
	{
	   send_to_char("OOC channel off!\n\r",ch);
	   SET_BIT(ch->comm,COMM_NO_OOC_TELL);
	}
	  return;
    }

    arg2[0] = '\0';
    sprintf(arg2,"%s",arg);

   number = number_argument(arg2, arg );
	count = 0;

    for (victim = char_list; victim != NULL; victim = victim->next)
    {
	if (is_name(victim->name,arg)
	&& (IS_NPC(victim) && victim->pIndexData->vnum == MOB_VNUM_DECOY))
		continue;
	if (victim->in_room == NULL || (IS_IMMORTAL(ch) ?
	!is_name(arg,(victim->original_name?victim->original_name:victim->name)):
	!is_name(arg,victim->name)) || !can_see( ch,victim ))
		continue;
        if ( ++count == number )
		break;
    }

    if (victim == NULL || ( IS_NPC(victim) && victim->in_room != ch->in_room ) )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL && !IS_NPC(victim))
    {
	act("$N seems to have lost consciousness...try again later.",ch,NULL,victim,TO_CHAR);
        sprintf(buf,"%s tells you '%s'\n\r",PERS(ch,victim),argument);
        buf[0] = UPPER(buf[0]);
        add_buf(victim->pcdata->buffer,buf);
	return;
    }

    if ( !(IS_IMMORTAL(ch) && ch->level > LEVEL_IMMORTAL) && !IS_AWAKE(victim) )
    {
	act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
	return;
    }
    if (is_affected(victim,gsn_shock_sphere))
    {
	act("$E can't hear you.",ch,0,victim,TO_CHAR);
	return;
    }

    if ((IS_SET(victim->comm,COMM_QUIET) || IS_SET(victim->comm,COMM_DEAF))
	&& !IS_IMMORTAL(ch) )
    {
	act( "$E is not receiving ooctells.", ch, 0, victim, TO_CHAR );
  	return;
    }

    argument = garble(ch, argument);
    if (is_affected(ch,gsn_deafen)) {
    argument = str_dup(upstring(argument));
    }

    if(IS_IMMORTAL(ch)&&IS_SET(ch->comm,COMM_ANSI))
	act("\x01B[1;37m[OOC] to $F '$t\x01B[0;37m'",ch,argument,victim,TO_CHAR);
    else if(IS_IMMORTAL(ch))
	act("[OOC] to $F '$t'",ch,argument,victim,TO_CHAR);
    else if (IS_SET(ch->comm,COMM_ANSI))
	act( "\x01B[1;37m[OOC] to $N '$t\x01B[0;37m'", ch, argument, victim, TO_CHAR );
    else
	act( "[OOC] to $N '$t'", ch, argument, victim, TO_CHAR );

    if(IS_IMMORTAL(victim)&&IS_SET(victim->comm,COMM_ANSI))
	act_new("\x01B[1;37m[OOC] from $f '$t\x01B[0;37m'",ch,argument,victim,TO_VICT,POS_DEAD);
    else if(IS_IMMORTAL(victim))
	act_new("[OOC] from $f '$t'",ch,argument,victim,TO_VICT,POS_DEAD);
    else if (IS_SET(victim->comm,COMM_ANSI))
	act_new("\x01B[1;37m[OOC] to $n '$t\x01B[0;37m'",ch,argument,victim,TO_VICT,POS_DEAD);
    else
	act_new("[OOC] from $n '$t'",ch,argument,victim,TO_VICT,POS_DEAD);

    return;
}

void do_deaf( CHAR_DATA *ch, char *argument)
{

   if (IS_SET(ch->comm,COMM_DEAF))
   {
     send_to_char("You can now hear tells again.\n\r",ch);
     REMOVE_BIT(ch->comm,COMM_DEAF);
   }
   else
   {
     send_to_char("From now on, you won't hear tells.\n\r",ch);
     SET_BIT(ch->comm,COMM_DEAF);
   }
}

/* RT quiet blocks out all communication */

void do_quiet ( CHAR_DATA *ch, char * argument)
{
    if (IS_SET(ch->comm,COMM_QUIET))
    {
      send_to_char("Quiet mode removed.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_QUIET);
    }
   else
   {
     send_to_char("From now on, you will only hear says and emotes.\n\r",ch);
     SET_BIT(ch->comm,COMM_QUIET);
   }
}


void do_replay (CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
    {
	send_to_char("You can't replay.\n\r",ch);
	return;
    }

    if (buf_string(ch->pcdata->buffer)[0] == '\0')
    {
	send_to_char("You have no tells to replay.\n\r",ch);
	return;
    }

    page_to_char(buf_string(ch->pcdata->buffer),ch);
    clear_buf(ch->pcdata->buffer);
}

/* RT auction rewritten in ROM style */
void do_auction( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    if(isShifted(ch) && !IS_IMMORTAL(ch))
    {

	send_to_char("You can't seem to form the words.\n\r",ch);
	return;
    }

    if (argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOAUCTION))
      {
	send_to_char("Auction channel is now ON.\n\r",ch);
	REMOVE_BIT(ch->comm,COMM_NOAUCTION);
      }
      else
      {
	send_to_char("Auction channel is now OFF.\n\r",ch);
	SET_BIT(ch->comm,COMM_NOAUCTION);
      }
    }
    else  /* auction message sent, turn auction on if it is off */
    {
	if (IS_SET(ch->comm,COMM_QUIET))
	{
	  send_to_char("You must turn off quiet mode first.\n\r",ch);
	  return;
	}

	if (IS_SET(ch->comm,COMM_NOCHANNELS))
	{
	  send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
	  return;
	}

    if(is_affected(ch,gsn_silence))
    {
	send_to_char("You cannot find the words within you.\n\r",ch);
	return;
    }

	if (is_affected(ch, gsn_gag) )
	{
    	send_to_char("You find difficultly talking.\n\r",ch);
    	return;
	}

    argument = garble(ch, argument);

      REMOVE_BIT(ch->comm,COMM_NOAUCTION);
      if (IS_SET(ch->act,PLR_COLOR))
      sprintf( buf, "You auction '\x01B[1;36m%s\x01B[0;37m'\n\r", argument );
      else
      sprintf( buf, "You auction '%s'\n\r", argument );
      send_to_char( buf, ch );
      for ( d = descriptor_list; d != NULL; d = d->next )
      {
	CHAR_DATA *victim;

	victim = d->original ? d->original : d->character;

	if ( d->connected == CON_PLAYING &&
	     d->character != ch &&
             ch->in_room->area == victim->in_room->area &&
	     !IS_SET(victim->comm,COMM_NOAUCTION) &&
	     !IS_SET(victim->comm,COMM_QUIET) )
	{
	    if (IS_SET(d->character->act,PLR_COLOR))
            act_new("$n auctions '\x01B[1;36m$t\x01B[0;37m'", ch,argument,d->character,TO_VICT,POS_DEAD);
	    else
            act_new("$n auctions '$t'", ch,argument,d->character,TO_VICT,POS_DEAD);
 	}
      }
    }
}

/* RT question channel */
void do_question( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    if(isShifted(ch) && !IS_IMMORTAL(ch))
    {

	send_to_char("You can't seem to form the words.\n\r",ch);
	return;
    }

    if (argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOQUESTION))
      {
        send_to_char("Q/A channel is now ON.\n\r",ch);
        REMOVE_BIT(ch->comm,COMM_NOQUESTION);
      }
      else
      {
        send_to_char("Q/A channel is now OFF.\n\r",ch);
        SET_BIT(ch->comm,COMM_NOQUESTION);
      }
    }
    else  /* question sent, turn Q/A on if it isn't already */
    {
        if (IS_SET(ch->comm,COMM_QUIET))
        {
          send_to_char("You must turn off quiet mode first.\n\r",ch);
          return;
        }

        if (IS_SET(ch->comm,COMM_NOCHANNELS))
        {
          send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
          return;
	}

    if(is_affected(ch,gsn_tigerclaw))
    {
	send_to_char("Your throat constricts too painfully.\n\r",ch);
	return;
    }

	if (is_affected(ch, gsn_gag) )
	{
    	send_to_char("You find difficultly talking.\n\r",ch);
    	return;
	}

    argument = garble(ch, argument);

        REMOVE_BIT(ch->comm,COMM_NOQUESTION);
      if (IS_SET(ch->act,PLR_COLOR))
      sprintf( buf, "You question '\x01B[1;36m%s\x01B[0;37m'\n\r", argument );
      else
      sprintf( buf, "You question '%s'\n\r", argument );
      send_to_char( buf, ch );
      for ( d = descriptor_list; d != NULL; d = d->next )
      {
        CHAR_DATA *victim;

        victim = d->original ? d->original : d->character;

        if ( d->connected == CON_PLAYING &&
             d->character != ch &&
	     d->character->in_room->area == ch->in_room->area &&
             !IS_SET(victim->comm,COMM_NOQUESTION) &&
             !IS_SET(victim->comm,COMM_QUIET) )
        {
	  if (IS_SET(d->character->act,PLR_COLOR))
          act_new("$n questions '\x01B[1;36m$t\x01B[0;37m'",
	 	  ch,argument,d->character,TO_VICT,POS_SLEEPING);
	  else
          act_new("$n questions '$t'",
	 	  ch,argument,d->character,TO_VICT,POS_SLEEPING);
        }
      }
    }
}

/* RT answer channel - uses same line as questions */
void do_answer( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    if(isShifted(ch) && !IS_IMMORTAL(ch))
    {

	send_to_char("You can't seem to form the words.\n\r",ch);
	return;
    }

    if (argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOQUESTION))
      {
        send_to_char("Q/A channel is now ON.\n\r",ch);
        REMOVE_BIT(ch->comm,COMM_NOQUESTION);
      }
      else
      {
        send_to_char("Q/A channel is now OFF.\n\r",ch);
        SET_BIT(ch->comm,COMM_NOQUESTION);
      }
    }
    else  /* answer sent, turn Q/A on if it isn't already */
    {
        if (IS_SET(ch->comm,COMM_QUIET))
        {
          send_to_char("You must turn off quiet mode first.\n\r",ch);
          return;
        }

        if (IS_SET(ch->comm,COMM_NOCHANNELS))
        {
          send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
          return;
	}

    if(is_affected(ch,gsn_tigerclaw))
    {
	send_to_char("Your throat constricts too painfully.\n\r",ch);
	return;
    }

	if (is_affected(ch, gsn_gag) )
	{
    	send_to_char("You find difficultly talking.\n\r",ch);
    	return;
	}

    argument = garble(ch, argument);

        REMOVE_BIT(ch->comm,COMM_NOQUESTION);
      if (IS_SET(ch->act,PLR_COLOR))
      sprintf( buf, "You answer '\x01B[1;36m%s\x01B[0;37m'\n\r", argument );
      else
      sprintf( buf, "You answer '%s'\n\r", argument );
      send_to_char( buf, ch );
      for ( d = descriptor_list; d != NULL; d = d->next )
      {
        CHAR_DATA *victim;

        victim = d->original ? d->original : d->character;

        if ( d->connected == CON_PLAYING &&
             d->character != ch &&
	     d->character->in_room->area == ch->in_room->area &&
             !IS_SET(victim->comm,COMM_NOQUESTION) &&
             !IS_SET(victim->comm,COMM_QUIET) )
        {
	  if (IS_SET(d->character->act,PLR_COLOR))
          act_new("$n answers '\x01B[1;36m$t\x01B[0;37m'",
		  ch,argument,d->character,TO_VICT,POS_SLEEPING);
	  else
          act_new("$n answers '$t'",
		  ch,argument,d->character,TO_VICT,POS_SLEEPING);
        }
      }
    }
}

void do_cb( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;

    if(isShifted(ch) && !IS_IMMORTAL(ch))
    {

	send_to_char("You can't seem to form the words.\n\r",ch);
	return;
    }

    if (!is_cabal(ch) || cabal_table[ch->cabal].independent)
    {
        send_to_char("You aren't in a cabal.\n\r",ch);
	return;
    }
    if ( argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOCABAL))
      {
        send_to_char("Cabal channel is now ON\n\r",ch);
        REMOVE_BIT(ch->comm,COMM_NOCABAL);
      }
      else
      {
        send_to_char("Cabal channel is now OFF\n\r",ch);
        SET_BIT(ch->comm,COMM_NOCABAL);
      }
      return;
    }

    if(is_affected(ch,gsn_silence))
    {
	send_to_char("You cannot find the words within you.\n\r",ch);
	return;
    }

    if(is_affected(ch,gsn_tigerclaw))
    {
	send_to_char("Your throat constricts too painfully.\n\r",ch);
	return;
    }

	if (is_affected(ch, gsn_gag) )
	{
    	send_to_char("You find difficultly talking.\n\r",ch);
    	return;
	}

        if (IS_SET(ch->comm,COMM_NOCHANNELS))
        {
         send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
          return;
        }

	argument = garble(ch, argument);

	if (is_affected(ch,gsn_deafen)) {
	//	argument = upstring(ch,argument);
		sprintf(argument,"%s", upstring(argument));
	}

        REMOVE_BIT(ch->comm,COMM_NOCABAL);
      if (IS_SET(ch->act,PLR_COLOR)) {
		sprintf( buf, "\x01B[1;37m%s%s: %s\x01B[0;37m\n\r",
		cabal_table[ch->cabal].who_name, IS_NPC(ch) ? ch->short_descr : ch->name,argument );
		if (is_affected(ch,gsn_cloak_form)) {
			sprintf( buf, "\x01B[1;37m%s%s (%s): %s\x01B[0;37m\n\r",
			cabal_table[ch->cabal].who_name, ch->name, ch->original_name, argument );
		}
      } else {
		sprintf( buf, "%s%s: %s\n\r", cabal_table[ch->cabal].who_name,
		IS_NPC(ch) ? ch->short_descr : ch->name, argument );
      }
      send_to_char( buf, ch );
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        if ( d->connected == CON_PLAYING &&
             d->character != ch &&
            ( ( is_same_cabal(ch,d->character) &&
             !IS_SET(d->character->comm,COMM_NOCABAL))
        || IS_SET(d->character->comm,COMM_ALL_CABALS) ) )
        {
	    if (IS_SET(d->character->act,PLR_COLOR)) {
            sprintf( buf, "\x01B[1;37m%s%s: %s\x01B[0;37m\n\r",
		cabal_table[ch->cabal].who_name, PERS(ch,d->character), argument );
		if (is_affected(ch,gsn_cloak_form)) {
			sprintf( buf, "\x01B[1;37m%s%s (%s): %s\x01B[0;37m\n\r",
			cabal_table[ch->cabal].who_name, PERS(ch,d->character), ch->original_name, argument );
		}
	    } else {
            sprintf( buf, "%s%s: %s\n\r", cabal_table[ch->cabal].who_name,PERS(ch,d->character), argument );
		if (is_affected(ch,gsn_cloak_form)) {
			sprintf( buf, "%s%s (%s): %s\n\r",
			cabal_table[ch->cabal].who_name, PERS(ch,d->character), ch->original_name, argument );
		}
	    }
	    send_to_char( buf, d->character );
        }
    }

    return;
}

void do_global(CHAR_DATA *ch,char *argument)
{
    if (IS_SET(ch->comm,COMM_ALL_CABALS))
    {
        send_to_char("You will no longer hear all Cabal channels.\n\r",ch);
        REMOVE_BIT(ch->comm,COMM_ALL_CABALS);
    }
    else
    {
        send_to_char("You will now hear all Cabal channels.\n\r",ch);
        SET_BIT(ch->comm,COMM_ALL_CABALS);
    }
    return;
}

void do_immtalk( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *wch;
    int trust;
    char arg1[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int level;

    trust = get_trust(ch);

    if (trust < 52 && (!IS_SET(ch->comm,COMM_IMMORTAL) && (!IS_HEROIMM(ch))))
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}

    if ( argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOWIZ))
      {
	send_to_char("Immortal channel is now ON\n\r",ch);
	REMOVE_BIT(ch->comm,COMM_NOWIZ);
      }
      else
      {
	send_to_char("Immortal channel is now OFF\n\r",ch);
	SET_BIT(ch->comm,COMM_NOWIZ);
      }
      return;
    }

	sprintf(buf2,"%s",argument);
        argument = one_argument(argument,arg1);

	if (arg1[0] == '\0' )
	{
                send_to_char("Syntax: immtalk <level?> <message>\n\r",ch);
                return;
	}
	if (!is_number(arg1)) {
		level = LEVEL_HERO;
	} else {
		level = atoi(arg1);
		if (level > trust || level < LEVEL_HERO) {
			level = LEVEL_HERO;
		} else {
			free_string(buf2);
			sprintf(buf2,"%s",argument);
		}
	}

	if (buf2[0] == '\0' )
	{
                send_to_char("Syntax: immtalk <level?> <message>\n\r",ch);
                return;
	}

	if (level > LEVEL_HERO)
	sprintf(buf," [%d]: %s",level,buf2);
	else
	sprintf(buf,": %s",buf2);

    REMOVE_BIT(ch->comm,COMM_NOWIZ);
    if (IS_SET(ch->comm,PLR_COLOR)) {
    sprintf(buf2,"{B%s[IMM] $n$t%s",get_char_color(ch,"immtalk"),END_COLOR(ch));
    act_new(buf2,ch,buf,0,TO_CHAR,POS_DEAD); }
    else
    act_new("{B[IMM] $n$t{x",ch,buf,0,TO_CHAR,POS_DEAD);

    for (wch = char_list; wch != NULL; wch = wch->next)
    {
	if (IS_NPC(wch) && wch->desc == NULL)	continue;
   	if ( ( IS_IMMORTAL(wch)) || IS_SET(wch->comm,COMM_IMMORTAL) || IS_HEROIMM(wch) ||
	(IS_NPC(wch) && (wch->desc != NULL) && IS_IMMORTAL(wch->desc->original)) )
	{
		if (level > get_trust(wch))
			continue;
	    if (IS_SET(wch->comm,PLR_COLOR))
	    {
            sprintf(buf2,"{B%s[IMM] $n$t%s",get_char_color(wch,"immtalk"),END_COLOR(wch));
	    act_new(buf2,ch,buf,wch,TO_VICT,POS_DEAD);
	    }
	    else
            act_new("{B[IMM] $n$t{x",ch,buf,wch,TO_VICT,POS_DEAD);
	}
    }

    return;
}


/* This is a real messy hack for a builder channel where people making
areas, but who aren't immortals can talk and discuss the matter...(Ceran)
*/
void do_builder( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *wch;

	if (!IS_SET(ch->comm,COMM_BUILDER))
	{

	send_to_char("Huh?\n\r",ch);
	return;
	}
    if ( argument[0] == '\0' )
	return;

    if (IS_SET(ch->act,PLR_COLOR))
    act_new("\x01B[1;34m[BUILDER] $n: $t\x01B[0;37m",ch,argument,NULL,TO_CHAR,POS_DEAD);
    else
    act_new("[BUILDER] $n: $t",ch,argument,NULL,TO_CHAR,POS_DEAD);

    for (wch = char_list; wch != NULL; wch = wch->next)
    {
	if (IS_NPC(wch))	continue;
	if (IS_SET(wch->comm,COMM_BUILDER))
	{
	    if (IS_SET(wch->act,PLR_COLOR))
            act_new("\x01B[1;34m[BUILDER] $n: $t\x01B[0;37m",ch,argument,wch,TO_VICT,POS_DEAD);
	    else
            act_new("[BUILDER] $n: $t",ch,argument,wch,TO_VICT,POS_DEAD);
	}
    }

    return;
}

/* Used for an object...take this out if you don't want it */
void obj_say_heavenly_sceptre(CHAR_DATA *ch,OBJ_DATA *obj,char *argument)
{
int sn;
	sn = -1;

	if (strstr(argument,"i am the wrath of god")
	|| strstr(argument,"I am the wrath of god"))
		sn = skill_lookup("heavenly wrath");

	if (strstr(argument,"feel the force of god")
	|| strstr(argument,"Feel the force of god"))
		sn = skill_lookup("heavenly fire");


	if (sn == -1)
		return;

	(*skill_table[sn].spell_fun) (sn,obj->level,ch,NULL,TAR_IGNORE);
	return;
}

/*static const char *garble(CHAR_DATA *ch, const char *i)
{
	static char buf[MAX_STRING_LENGTH];
	char *o;

	if (!is_affected(ch, gsn_garble))
		return i;

	for (o = buf; *i && o-buf < sizeof(buf)-1; i++, o++) {
		if (*i >= 65 && *i <= 90)
		{
			*o = number_range(65, 90);
		}
		else if (*i >= 97 && *i <= 122)
		{
			*o = number_range(97,122);
		}
		else
		{
			*o = *i;
		}
	}
	*o = '\0';
	return buf;
}
*/

char *garble(CHAR_DATA *ch, char *i)
{
        static char buf[MAX_STRING_LENGTH];
        char *o;

        if (!is_affected(ch, gsn_garble))
                return i;

        for (o = buf; *i && o-buf < sizeof(buf)-1; i++, o++) {
                if (*i >= 65 && *i <= 90)
                {
                        *o = number_range(65, 90);
                }
                else if (*i >= 97 && *i <= 122)
                {
                        *o = number_range(97,122);
                }
                else
                {
                        *o = *i;
                }
        }
        *o = '\0';
        return buf;
}

/*static const char *upstring(CHAR_DATA *ch, const char *i)
{
	static char buf[MAX_STRING_LENGTH];
	char *o;

	for (o = buf; *i && o-buf < sizeof(buf)-1; i++, o++) {
		if (*i >= 97 && *i <= 122)
		{
			*o = UPPER(*i);
		}
		else
		{
			*o = *i;
		}
	}
	*o = '\0';
	return buf;
}
*/

const char *upstring(const char *i)
{
        static char buf[MAX_STRING_LENGTH];
        char *o;
        const char *or;
        or = i;

        for (o = buf; *i && o-buf < sizeof(buf)-1; i++, o++) {
                *o = UPPER(*i);
        }
        *o = '\0';
        return buf;
}

void do_say( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
if (!IS_NPC(ch))

    if(isShifted(ch) && !IS_IMMORTAL(ch))
    {

	send_to_char("You can't seem to form the words.\n\r",ch);
	return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "Say what?\n\r", ch );
	return;
    }

    if(is_affected(ch,gsn_silence))
    {
	send_to_char("You cannot find the words within you.\n\r",ch);
	return;
    }

    if(is_affected(ch,gsn_tigerclaw))
    {
	send_to_char("Your throat constricts too painfully.\n\r",ch);
	return;
    }

	if (is_affected(ch, gsn_gag) )
	{
    	send_to_char("You find difficultly talking.\n\r",ch);
    	return;
	}

        argument = garble(ch, argument);

	if (is_affected(ch,gsn_deafen)) {
		do_yell(ch,argument);
		return;
	}


    for ( victim = ch->in_room->people; victim != NULL; victim = victim->next_in_room )
    {

        if ( IS_AWAKE(victim) )
	{
		if (is_affected(victim,gsn_shock_sphere)) {
 	act("$n says something you can't quite hear.",ch,0,victim,TO_VICT);
		}
		else if (!(IS_IMMORTAL(ch)) && is_affected(victim,gsn_deafen)) {
			act("$n says something you can't quite hear.",ch,0,victim,TO_VICT);

		}
		else if(!(IS_IMMORTAL(ch)) && is_affected(victim,gsn_nullify_sound))
		{
			act("$n seems to be saying something but you hear nothing.",ch,NULL,victim,TO_VICT);

		}
		else if(!(IS_IMMORTAL(ch)) && is_affected(ch,gsn_nullify_sound))
		{
			act("$n's lips are moving but you hear nothing.",ch,NULL,victim,TO_VICT);
		}
	 	else if (IS_SET(victim->act,PLR_COLOR))
         {
   	if (ch->race==race_lookup("illithid"))
   		act("$n mentally projects '\x01B[1;33m$t\x01B[0;37m'",ch,argument,victim,TO_VICT);
   		else
   		act("$n says '\x01B[1;33m$t\x01B[0;37m'",ch,argument,victim,TO_VICT);
	 }
	    else
	{
     if (ch->race==race_lookup("illithid"))
     	  act("$n mentally projects '\x01B[1;33m$t\x01B[0;37m'",ch,argument,victim,TO_VICT);
     else
          act("$n says '$t'",ch,argument,victim,TO_VICT);
	}
	}
    }
    if (IS_SET(ch->act,PLR_COLOR))
	{
	if (ch->race==race_lookup("illithid"))
	   act( "You mentally project '\x01B[1;33m$T\x01B[0;37m'", ch, NULL, argument, TO_CHAR );
	else
	   act( "You say '\x01B[1;33m$T\x01B[0;37m'", ch, NULL, argument, TO_CHAR );
	} else{
	if (ch->race==race_lookup("illithid"))
	   act( "You mentally project '$T'", ch, NULL, argument, TO_CHAR );
	  else
	   act( "You say '$T'", ch, NULL, argument, TO_CHAR );
	}


    if ( !IS_NPC(ch) )
    {
        CHAR_DATA *mob, *mob_next;
        for ( mob = ch->in_room->people; mob != NULL; mob = mob_next )
        {
            mob_next = mob->next_in_room;
            if ( IS_NPC(mob) && HAS_TRIGGER( mob, TRIG_SPEECH )
            &&   mob->position == mob->pIndexData->default_pos )
                mp_act_trigger( argument, mob, ch, NULL, NULL, TRIG_SPEECH );
        }
    }
    mob_say_lady(ch,argument);

    for (obj = ch->carrying; obj != NULL; obj = obj->next_content)
    {
	if (obj->wear_loc != WEAR_NONE && obj->pIndexData->vnum == OBJ_VNUM_ROD_LORDLY)
		obj_say_lordly(ch,obj,argument);
	if (obj->wear_loc != WEAR_NONE && obj->pIndexData->vnum == OBJ_VNUM_HEAVENLY_SCEPTRE)
		obj_say_heavenly_sceptre(ch,obj,argument);
    }

report_cabal_items(ch, argument);
check_bloodoath(ch,argument);

/*
Ugh..ugly coded say_progs for arkham legion area. Take these out if you
don't want to use legion.are ... (Ceran)
*/
    say_prog_karguska(ch,argument);
    say_prog_summon_demon(ch, argument);
    say_prog_raise_shadowlord(ch,argument);

    return;
}

void do_whisper( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
//	OBJ_DATA *obj;

	if (!IS_NPC(ch))

        if(isShifted(ch) && !IS_IMMORTAL(ch))
        {

		send_to_char("You can't seem to form the words.\n\r",ch);
		return;
        }

	if ( argument[0] == '\0' )
	{
		send_to_char( "Whisper what?\n\r", ch );
		return;
	}

	if(is_affected(ch,gsn_silence))
	{
		send_to_char("You cannot find the words within you.\n\r",ch);
		return;
	}

    if(is_affected(ch,gsn_tigerclaw))
    {
	send_to_char("Your throat constricts too painfully.\n\r",ch);
	return;
    }

	if (is_affected(ch, gsn_gag) )
	{
    	send_to_char("You find difficultly talking.\n\r",ch);
    	return;
	}

	argument = garble(ch, argument);

	if (is_affected(ch,gsn_deafen)) {
		do_yell(ch,argument);
		return;
	}


	for ( victim = ch->in_room->people; victim != NULL; victim = victim->next_in_room )
	{
		if ( IS_AWAKE(victim) )
		{
			if (is_affected(victim,gsn_shock_sphere))
			{
	 			act("$n says something you can't quite hear.",ch,0,victim,TO_VICT);
			} else if (!(IS_IMMORTAL(ch)) && is_affected(victim,gsn_deafen)) {
				act("$n says something you can't quite hear.",ch,0,victim,TO_VICT);
			} else if (IS_SET(victim->act,PLR_COLOR)) {
				act("$n whispers '\x01B[1;31m$t\x01B[0;37m'",ch,argument,victim,TO_VICT);
			} else {
	            	act("$n whispers '$t'",ch,argument,victim,TO_VICT);
			}
		}
	}

	if (IS_SET(ch->act,PLR_COLOR))
	{
		act( "You whisper '\x01B[1;31m$T\x01B[0;37m'", ch, NULL, argument, TO_CHAR );
	} else {
		act( "You whisper '$T'", ch, NULL, argument, TO_CHAR );
	}

	if ( !IS_NPC(ch) )
	{
		CHAR_DATA *mob, *mob_next;
		for ( mob = ch->in_room->people; mob != NULL; mob = mob_next )
		{
			mob_next = mob->next_in_room;
			if ( IS_NPC(mob) && HAS_TRIGGER( mob, TRIG_SPEECH )
			&&   mob->position == mob->pIndexData->default_pos )
			mp_act_trigger( argument, mob, ch, NULL, NULL, TRIG_SPEECH );
		}
	}
	return;
}

void do_pray( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;

    char *check;

    if (argument[0] == '\0' )
    {
        send_to_char("Pray what?\n\r",ch);
      	return;
    }

    if ( IS_SET(ch->comm, COMM_NOCHANNELS) )
    {
        send_to_char( "You can't pray.\n\r", ch );
        return;
    }

    check = str_dup(argument);

    WAIT_STATE( ch, 12 );
    if (IS_SET(ch->act,PLR_COLOR))
    act( "\x01B[1;31mYou pray to the gods: $T.\x01B[0;37m", ch, NULL,argument, TO_CHAR );
    else
    act( "You pray to the gods: $T.", ch, NULL, argument, TO_CHAR );

/* Quick anti-swearing hack for two common pray morons who like to pray
stupid and insulting things...just add words you don't want to see prayed
to you here....(Ceran)
*/
    if (strstr(argument,"fuck") || strstr(argument,"fag")
	|| strstr(argument,"FUCK")
	|| strstr(argument,"FAG")
	|| strstr(argument,"Fuck")
	|| strstr(argument,"Fag")
	|| strstr(argument,"!!!"))
		WAIT_STATE(ch,360);


    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *victim;

	victim = d->original ? d->original : d->character;

	if ( d->connected == CON_PLAYING &&
	     d->character != ch &&
	     !IS_SET(victim->comm, COMM_SHOUTSOFF) &&
	     !IS_SET(victim->comm, COMM_QUIET) &&
	     victim->level >= 53 )
	{
	    if (IS_SET(d->character->act,PLR_COLOR))
            act("\x01B[1;31m$f PRAYS: $t\x01B[0;37m",ch,argument,d->character,TO_VICT);
	    else
            act("$f PRAYS: $t",ch,argument,d->character,TO_VICT);
	}
    }

    return;
}



void do_tell( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH],buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int count, number;
    char arg2[MAX_STRING_LENGTH];

    if(isShifted(ch) && !IS_IMMORTAL(ch))
    {

	send_to_char("You can't seem to form the words.\n\r",ch);
	return;
    }

    if  (is_affected(ch,gsn_silence))
    {
	send_to_char("You cannot find the words within you.\n\r",ch);
	return;
    }

	if(is_affected(ch,gsn_tigerclaw))
    {
	send_to_char("Your throat constricts too painfully.\n\r",ch);
	return;
    }

	if (is_affected(ch, gsn_gag) )
	{
    	send_to_char("You find difficultly talking.\n\r",ch);
    	return;
	}

    if ( IS_SET(ch->comm, COMM_NOTELL) || IS_SET(ch->comm,COMM_DEAF))
    {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }

    if ( IS_SET(ch->comm, COMM_QUIET) )
    {
	send_to_char( "You must turn off quiet mode first.\n\r", ch);
	return;
    }

    if (IS_SET(ch->comm,COMM_DEAF))
    {
	send_to_char("You must turn off deaf mode first.\n\r",ch);
	return;
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Tell whom what?\n\r", ch );
	return;
    }

/* This added for decoy tells..without this you have to locate the number
of the person you are trying to talk to when their decoys occupy 1.<name>,
and that's mega annoying...(Ceran)
*/
    arg2[0] = '\0';
    sprintf(arg2,"%s",arg);

    /*
     * Can tell to PC's anywhere, but NPC's only in same room.
     * -- Furey
     */

   number = number_argument(arg2, arg );
	count = 0;

    for (victim = char_list; victim != NULL; victim = victim->next)
    {
	if (is_name(victim->name,arg)
	&& (IS_NPC(victim) && victim->pIndexData->vnum == MOB_VNUM_DECOY))
		continue;
	if (victim->in_room == NULL || (IS_IMMORTAL(ch) ? !is_name(arg,(victim->original_name?victim->original_name:victim->name)): !is_name(arg,victim->name)) || !can_see( ch,victim ))
		continue;
        if ( ++count == number )
		break;
    }

    if (victim == NULL
    || ( IS_NPC(victim) && victim->in_room != ch->in_room ) )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL && !IS_NPC(victim))
    {
	act("$N seems to have lost consciousness...try again later.",
	    ch,NULL,victim,TO_CHAR);
        sprintf(buf,"%s tells you '%s'\n\r",PERS(ch,victim),argument);
        buf[0] = UPPER(buf[0]);
        add_buf(victim->pcdata->buffer,buf);
	return;
    }

	if (!(IS_IMMORTAL(ch)) && is_affected(victim,gsn_deafen)) {
		act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
		return;
	}

    if ( !(IS_IMMORTAL(ch) && ch->level > LEVEL_IMMORTAL) && !IS_AWAKE(victim) )
    {
	act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
	return;
    }
    if (is_affected(victim,gsn_shock_sphere))
    {
	act("$E can't hear you.",ch,0,victim,TO_CHAR);
	return;
    }


    if ((IS_SET(victim->comm,COMM_QUIET) || IS_SET(victim->comm,COMM_DEAF))
	&& !IS_IMMORTAL(ch) ) /* Let Imms send tells to deaf players */
    {
	act( "$E is not receiving tells.", ch, 0, victim, TO_CHAR );
  	return;
    }

	argument = garble(ch, argument);
	if (is_affected(ch,gsn_deafen)) {
	argument = str_dup(upstring(argument));
	}

    if (ch->race==race_lookup("illithid"))
	{
		act("You mentally project to $F '\x01b[1;32m$t\x01B[0;37m'",ch,argument,victim,TO_CHAR);
		act_new("$n mentally projects to you '\x01B[1;32m$t\x01B[0;37m'",ch,argument,victim,TO_VICT,POS_DEAD);
		victim->reply = ch;
		return;
	}

    if(IS_IMMORTAL(ch)&&IS_SET(ch->act,PLR_COLOR))
	act("You tell $F '\x01b[1;32m$t\x01B[0;37m'",ch,argument,victim,TO_CHAR);
    else if(IS_IMMORTAL(ch))
	act("You tell $F '$t'",ch,argument,victim,TO_CHAR);
    else if (IS_SET(ch->act,PLR_COLOR))
	act( "You tell $N '\x01B[1;32m$t\x01B[0;37m'", ch, argument, victim, TO_CHAR );
    else
	act( "You tell $N '$t'", ch, argument, victim, TO_CHAR );

    if(IS_IMMORTAL(victim)&&IS_SET(victim->act,PLR_COLOR))
	act_new("$f tells you '\x01B[1;32m$t\x01B[0;37m'",ch,argument,victim,TO_VICT,POS_DEAD);
    else if(IS_IMMORTAL(victim))
	act_new("$f tells you '$t'",ch,argument,victim,TO_VICT,POS_DEAD);
    else if (IS_SET(victim->act,PLR_COLOR))
	act_new("$n tells you '\x01B[1;32m$t\x01B[0;37m'",ch,argument,victim,TO_VICT,POS_DEAD);
    else
	act_new("$n tells you '$t'",ch,argument,victim,TO_VICT,POS_DEAD);

    victim->reply	= ch;

    if ( !IS_NPC(ch) && IS_NPC(victim) && HAS_TRIGGER(victim,TRIG_SPEECH) )
        mp_act_trigger( argument, victim, ch, NULL, NULL, TRIG_SPEECH );

    return;
}



void do_reply( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];

    if(isShifted(ch) && !IS_IMMORTAL(ch))
    {

	send_to_char("You can't seem to form the words.\n\r",ch);
	return;
    }

    if ( IS_SET(ch->comm, COMM_NOTELL) )
    {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }

    if(is_affected(ch,gsn_tigerclaw))
    {
	send_to_char("Your throat constricts too painfully.\n\r",ch);
	return;
    }

	if (is_affected(ch, gsn_gag) )
	{
    	send_to_char("You find difficultly talking.\n\r",ch);
    	return;
	}

    if ( ( victim = ch->reply ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL && !IS_NPC(victim))
    {
        act("$N seems to have misplaced $S link...try again later.",
            ch,NULL,victim,TO_CHAR);
        sprintf(buf,"%s tells you '%s'\n\r",PERS(ch,victim),argument);
        buf[0] = UPPER(buf[0]);
        add_buf(victim->pcdata->buffer,buf);
        return;
    }

    if ( !IS_IMMORTAL(ch) && !IS_AWAKE(victim) )
    {
	act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
	return;
    }

	if (!(IS_IMMORTAL(ch)) && is_affected(victim,gsn_deafen)) {
		act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
		return;
	}

    if ((IS_SET(victim->comm,COMM_QUIET) || IS_SET(victim->comm,COMM_DEAF))
    &&  !IS_IMMORTAL(ch) && !IS_IMMORTAL(victim))
    {
        act_new( "$E is not receiving tells.", ch, 0, victim, TO_CHAR,POS_DEAD);
        return;
    }

    if (!IS_IMMORTAL(victim) && !IS_AWAKE(ch))
    {
	send_to_char( "In your dreams, or what?\n\r", ch );
	return;
    }

    if (IS_SET(victim->comm,COMM_AFK))
    {
        if (IS_NPC(victim))
        {
            act_new("$E is AFK, and not receiving tells.",
		ch,NULL,victim,TO_CHAR,POS_DEAD);
            return;
        }

        act_new("$E is AFK, but your tell will go through when $E returns.",
            ch,NULL,victim,TO_CHAR,POS_DEAD);
        sprintf(buf,"%s tells you '%s'\n\r",PERS(ch,victim),argument);
	buf[0] = UPPER(buf[0]);
        add_buf(victim->pcdata->buffer,buf);
        return;
    }

   argument = garble(ch, argument);

	if (is_affected(ch,gsn_deafen)) {
	//	argument = upstring(ch,argument);
argument = str_dup(upstring(argument));
	}

    if (ch->race==race_lookup("illithid"))
	{
		act("You mentally project to $F '\x01b[1;32m$t\x01B[0;37m'",ch,argument,victim,TO_CHAR);
		act_new("$n mentally projects to you '\x01B[1;32m$t\x01B[0;37m'",ch,argument,victim,TO_VICT,POS_DEAD);
		return;
	}

    if(IS_IMMORTAL(ch)&&IS_SET(ch->act,PLR_COLOR))
        act("You tell $F '\x01b[1;32m$t\x01B[0;37m'",ch,argument,victim,TO_CHAR);
    else if(IS_IMMORTAL(ch))
        act("You tell $F '$t'",ch,argument,victim,TO_CHAR);
    else if (IS_SET(ch->act,PLR_COLOR))
	act( "You tell $N '\x01B[1;32m$t\x01B[0;37m'", ch,argument,victim, TO_CHAR );
    else
	act( "You tell $N '$t'", ch, argument, victim, TO_CHAR );

    if(IS_IMMORTAL(victim)&&IS_SET(victim->act,PLR_COLOR))
        act_new("$f tells you '\x01B[1;32m$t\x01B[0;37m'",ch,argument,victim,TO_VICT,POS_DEAD);
    else if(IS_IMMORTAL(victim))
        act_new("$f tells you '$t'",ch,argument,victim,TO_VICT,POS_DEAD);
    else if (IS_SET(victim->act,PLR_COLOR))
        act_new("$n tells you '\x01B[1;32m$t\x01B[0;37m'",ch,argument,victim,TO_VICT,POS_DEAD);
    else
	act_new("$n tells you '$t'",ch,argument,victim,TO_VICT,POS_DEAD);

    victim->reply	= ch;

    return;
}




void do_yell( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;

    if(isShifted(ch) && !IS_IMMORTAL(ch))
    {

	send_to_char("You can't seem to form the words.\n\r",ch);
	return;
    }

    if(is_affected(ch,gsn_tigerclaw))
    {
	send_to_char("Your throat constricts too painfully.\n\r",ch);
	return;
    }

    if(is_affected(ch,gsn_silence))
    {
	send_to_char("You cannot find the words within you.\n\r",ch);
	return;
    }

	if (is_affected(ch, gsn_gag) )
	{
    	send_to_char("You find difficultly talking.\n\r",ch);
    	return;
	}

    if ( IS_SET(ch->comm, COMM_NOSHOUT) )
    {
        send_to_char( "You can't yell.\n\r", ch );
        return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "Yell what?\n\r", ch );
	return;
    }

    argument = garble(ch, argument);

	if (is_affected(ch,gsn_deafen)) {
	argument = str_dup(upstring(argument));
	}

    if (ch->race==race_lookup("illithid"))
    {
    if (IS_SET(ch->act,PLR_COLOR))
			act("You mentally blast '\x01B[1;36m$t\x01B[0;37m'",ch,argument,NULL,TO_CHAR);
		  else
			act("You mentally blast '$t'",ch,argument,NULL,TO_CHAR);

		for ( d = descriptor_list; d != NULL; d = d->next )
	{
			if ( d->connected == CON_PLAYING
			&&   d->character != ch
			&&   d->character->in_room != NULL
			&&   d->character->in_room->area == ch->in_room->area
		        &&   !IS_SET(d->character->comm,COMM_QUIET) )
			{
			if (is_affected(d->character,gsn_shock_sphere))
				continue;
//			}
			    if (IS_SET(d->character->act,PLR_COLOR))
		            act("$n mentally blasts '\x01B[1;36m$t\x01B[0;37m'",ch,argument,d->character,TO_VICT);
	else
		            act("$n mentally blasts '$t'",ch,argument,d->character,TO_VICT);
			}

	    }
	return;
}

    if (IS_SET(ch->act,PLR_COLOR))
	act("You yell '\x01B[1;36m$t\x01B[0;37m'",ch,argument,NULL,TO_CHAR);
	else
	act("You yell '$t'",ch,argument,NULL,TO_CHAR);


    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING
	&&   d->character != ch
	&&   d->character->in_room != NULL
	&&   d->character->in_room->area == ch->in_room->area
        &&   !IS_SET(d->character->comm,COMM_QUIET) )
	{
	if (is_affected(d->character,gsn_shock_sphere))
		continue;
	if (IS_SET(d->character->act,PLR_COLOR))

    act("$n yells '\x01B[1;36m$t\x01B[0;37m'",ch,argument,d->character,TO_VICT);
     else
         act("$n yells '$t'",ch,argument,d->character,TO_VICT);
	}

     }
return;
}


void do_myell( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;

if(isShifted(ch))
    {
        for (d = descriptor_list; d; d = d->next)
        {
                if (d->connected == CON_PLAYING
                &&  d->character->in_room != NULL && ch->in_room != NULL
                &&  d->character->in_room->area == ch->in_room->area
		&&  d->character != ch
		&&  !IS_SET(d->character->in_room->room_flags, COMM_QUIET))
                {
                        send_to_char(form_table[ch->pcdata->shifted].yell,d->character);
                        send_to_char("\n\r",d->character);
                }
        }
	send_to_char("You make some noise as you are attacked.\n\r",ch);
	return;
    }

    if ( IS_SET(ch->comm, COMM_NOSHOUT) )
    {
        send_to_char( "You can't yell.\n\r", ch );
        return;
    }

    if(is_affected(ch,gsn_tigerclaw))
    {
	send_to_char("Your throat constricts too painfully.\n\r",ch);
	return;
    }

	if (is_affected(ch, gsn_gag) )
	{
    	send_to_char("You find difficultly talking.\n\r",ch);
    	return;
	}

    if ( argument[0] == '\0' )
    {
	send_to_char( "Yell what?\n\r", ch );
	return;
    }
   argument = garble(ch, argument);

	if (is_affected(ch,gsn_deafen)) {
	argument = str_dup(upstring(argument));
	}

    if (ch->race==race_lookup("illithid"))
	    {
			if (IS_SET(ch->act,PLR_COLOR))
				act("You mentally blast '\x01B[1;36m$t\x01B[0;37m'",ch,argument,NULL,TO_CHAR);
			  else
				act("You mentally blast '$t'",ch,argument,NULL,TO_CHAR);

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING
	&&   d->character != ch
	&&   d->character->in_room != NULL
	&&   d->character->in_room->area == ch->in_room->area
        &&   !IS_SET(d->character->comm,COMM_QUIET) )
	{
				if (is_affected(d->character,gsn_shock_sphere))
					continue;
	    if (IS_SET(d->character->act,PLR_COLOR))
	    act("$n mentally blasts '\x01B[1;36m$t\x01B[0;37m'",ch,argument,d->character,TO_VICT);
		else
	    act("$n mentally blasts '$t'",ch,argument,d->character,TO_VICT);
           }
	}
    return;
}
    if (IS_SET(ch->act,PLR_COLOR))
		   		act("You yell '\x01B[1;36m$t\x01B[0;37m'",ch,argument,NULL,TO_CHAR);
		   			   	else
		   		act("You yell '$t'",ch,argument,NULL,TO_CHAR);


    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected == CON_PLAYING
	&&   d->character != ch
	&&   d->character->in_room != NULL
	&&   d->character->in_room->area == ch->in_room->area
        &&   !IS_SET(d->character->comm,COMM_QUIET) )
	{
	if (IS_SET(d->character->act,PLR_COLOR))
        act("$n yells '\x01B[1;36m$t\x01B[0;37m'",ch,argument,d->character,TO_VICT);
		else
        act("$n yells '$t'",ch,argument,d->character,TO_VICT);
	    }
	}


    return;
}

void new_yell( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    if (argument[0] == '\0' )
	return;
    if (!(IS_NPC(ch) && ch->master != NULL && IS_AFFECTED(ch,AFF_CHARM)))

    if(is_affected(ch,gsn_tigerclaw))
    {
	send_to_char("Your throat constricts too painfully.\n\r",ch);
	return;
    }

    if (is_affected(ch, gsn_gag) )
    {
        send_to_char("You find difficultly talking.\n\r",ch);
        return;
    }

    act( "{wYou yell '{C$T{x'", ch, NULL, argument, TO_CHAR);
    for ( d = descriptor_list; d != NULL; d = d->next )
        if ( d->connected == CON_PLAYING
        && d->character != ch
        && d->character->in_room != NULL
	&& (d->character->in_room->area == ch->in_room->area)
        && !IS_SET(d->character->comm, COMM_QUIET))
	{
    	    if (d->character->cabal != CABAL_VIGILANTE)
		act_new("$n yells '{C$t{x'",ch,argument,d->character,TO_VICT,POS_RESTING);
    	    else
		act_new("$n yells wildly '{C$t{x'",ch,argument,d->character,TO_VICT,POS_RESTING);
	}
}

void do_emote( CHAR_DATA *ch, char *argument )
{
	char buffer[MAX_STRING_LENGTH*2];

    if(isShifted(ch) && !IS_IMMORTAL(ch))
    {

	send_to_char("You can't seem to form the words.\n\r",ch);
	return;
    }

    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
    {
        send_to_char( "You can't show your emotions.\n\r", ch );
        return;
    }

	if (is_affected(ch, gsn_gag) )
	{
    	send_to_char("You find difficultly talking.\n\r",ch);
    	return;
	}

    if ( argument[0] == '\0' )
    {
        send_to_char( "Emote what?\n\r", ch );
        return;
    }

   argument = garble(ch, argument);
	if (IS_IMMORTAL(ch)) {
	colorconv(buffer, argument, ch);
	} else {
	strcpy(buffer, argument);
	}

    MOBtrigger = FALSE;
    act( "$n $T", ch, NULL, buffer, TO_ROOM );
    act( "$n $T", ch, NULL, buffer, TO_CHAR );
    MOBtrigger = TRUE;
    return;
}

void do_pmote( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    char *letter,*name;
    char last[MAX_INPUT_LENGTH], temp[MAX_STRING_LENGTH];
    int matches = 0;

    if(isShifted(ch) && !IS_IMMORTAL(ch))
    {

	send_to_char("You can't seem to form the words.\n\r",ch);
	return;
    }

    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
    {
        send_to_char( "You can't show your emotions.\n\r", ch );
        return;
    }

	if (is_affected(ch, gsn_gag) )
	{
    	send_to_char("You find difficultly talking.\n\r",ch);
    	return;
	}

    if ( argument[0] == '\0' )
    {
        send_to_char( "Emote what?\n\r", ch );
        return;
    }

   argument = garble(ch, argument);
    act( "$n $t", ch, argument, NULL, TO_CHAR );

    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
	if (vch->desc == NULL || vch == ch)
	    continue;

   argument = garble(ch, argument);
	if ((letter = strstr(argument,vch->name)) == NULL)
	{
            MOBtrigger = FALSE;
	    act("$N $t",vch,argument,ch,TO_CHAR);
            MOBtrigger = TRUE;
	    continue;
	}

	strcpy(temp,argument);
	temp[strlen(argument) - strlen(letter)] = '\0';
   	last[0] = '\0';
 	name = vch->name;

	for (; *letter != '\0'; letter++)
	{
	    if (*letter == '\'' && matches == strlen(vch->name))
	    {
		strcat(temp,"r");
		continue;
	    }

	    if (*letter == 's' && matches == strlen(vch->name))
	    {
		matches = 0;
		continue;
	    }

 	    if (matches == strlen(vch->name))
	    {
		matches = 0;
	    }

	    if (*letter == *name)
	    {
		matches++;
		name++;
		if (matches == strlen(vch->name))
		{
		    strcat(temp,"you");
		    last[0] = '\0';
		    name = vch->name;
		    continue;
		}
		strncat(last,letter,1);
		continue;
	    }

	    matches = 0;
	    strcat(temp,last);
	    strncat(temp,letter,1);
	    last[0] = '\0';
	    name = vch->name;
	}

   argument = garble(ch, argument);
        MOBtrigger = FALSE;
	act("$N $t",vch,temp,ch,TO_CHAR);
        MOBtrigger = TRUE;
    }

    return;
}


/*
 * All the posing stuff.
 */
struct	pose_table_type
{
    char *	message[2*MAX_CLASS];
};

const	struct	pose_table_type	pose_table	[]	=
{
    {
	{
	    "You sizzle with energy.",
	    "$n sizzles with energy.",
	    "You feel very holy.",
	    "$n looks very holy.",
	    "You perform a small card trick.",
	    "$n performs a small card trick.",
	    "You show your bulging muscles.",
	    "$n shows $s bulging muscles."
	}
    },

    {
	{
	    "You turn into a butterfly, then return to your normal shape.",
	    "$n turns into a butterfly, then returns to $s normal shape.",
	    "You nonchalantly turn wine into water.",
	    "$n nonchalantly turns wine into water.",
	    "You wiggle your ears alternately.",
	    "$n wiggles $s ears alternately.",
	    "You crack nuts between your fingers.",
	    "$n cracks nuts between $s fingers."
	}
    },

    {
	{
	    "Blue sparks fly from your fingers.",
	    "Blue sparks fly from $n's fingers.",
	    "A halo appears over your head.",
	    "A halo appears over $n's head.",
	    "You nimbly tie yourself into a knot.",
	    "$n nimbly ties $mself into a knot.",
	    "You grizzle your teeth and look mean.",
	    "$n grizzles $s teeth and looks mean."
	}
    },

    {
	{
	    "Little red lights dance in your eyes.",
	    "Little red lights dance in $n's eyes.",
	    "You recite words of wisdom.",
	    "$n recites words of wisdom.",
	    "You juggle with daggers, apples, and eyeballs.",
	    "$n juggles with daggers, apples, and eyeballs.",
	    "You hit your head, and your eyes roll.",
	    "$n hits $s head, and $s eyes roll."
	}
    },

    {
	{
	    "A slimy green monster appears before you and bows.",
	    "A slimy green monster appears before $n and bows.",
	    "Deep in prayer, you levitate.",
	    "Deep in prayer, $n levitates.",
	    "You steal the underwear off every person in the room.",
	    "Your underwear is gone!  $n stole it!",
	    "Crunch, crunch -- you munch a bottle.",
	    "Crunch, crunch -- $n munches a bottle."
	}
    },

    {
	{
	    "You turn everybody into a little pink elephant.",
	    "You are turned into a little pink elephant by $n.",
	    "An angel consults you.",
	    "An angel consults $n.",
	    "The dice roll ... and you win again.",
	    "The dice roll ... and $n wins again.",
	    "... 98, 99, 100 ... you do pushups.",
	    "... 98, 99, 100 ... $n does pushups."
	}
    },

    {
	{
	    "A small ball of light dances on your fingertips.",
	    "A small ball of light dances on $n's fingertips.",
	    "Your body glows with an unearthly light.",
	    "$n's body glows with an unearthly light.",
	    "You count the money in everyone's pockets.",
	    "Check your money, $n is counting it.",
            "The gods admire your physique.",
            "The gods admire $n's physique."
	}
    },

    {
	{
	    "Smoke and fumes leak from your nostrils.",
	    "Smoke and fumes leak from $n's nostrils.",
	    "A spot light hits you.",
	    "A spot light hits $n.",
	    "You balance a pocket knife on your tongue.",
	    "$n balances a pocket knife on your tongue.",
	    "Watch your feet, you are juggling granite boulders.",
	    "Watch your feet, $n is juggling granite boulders."
	}
    },

    {
	{
	    "The light flickers as you rap in magical languages.",
	    "The light flickers as $n raps in magical languages.",
	    "Everyone levitates as you pray.",
	    "You levitate as $n prays.",
	    "You produce a coin from everyone's ear.",
	    "$n produces a coin from your ear.",
	    "Oomph!  You squeeze water out of a granite boulder.",
	    "Oomph!  $n squeezes water out of a granite boulder."
	}
    },

    {
	{
	    "Your head disappears.",
	    "$n's head disappears.",
	    "A cool breeze refreshes you.",
	    "A cool breeze refreshes $n.",
	    "You step behind your shadow.",
	    "$n steps behind $s shadow.",
	    "You pick your teeth with a spear.",
	    "$n picks $s teeth with a spear."
	}
    },

    {
	{
	    "A fire elemental singes your hair.",
	    "A fire elemental singes $n's hair.",
	    "The sun pierces through the clouds to illuminate you.",
	    "The sun pierces through the clouds to illuminate $n.",
	    "Your eyes dance with greed.",
	    "$n's eyes dance with greed.",
	    "Everyone is swept off their foot by your hug.",
	    "You are swept off your feet by $n's hug."
	}
    },

    {
	{
	    "The sky changes color to match your eyes.",
	    "The sky changes color to match $n's eyes.",
	    "The ocean parts before you.",
	    "The ocean parts before $n.",
	    "You deftly steal everyone's weapon.",
	    "$n deftly steals your weapon.",
	    "Your karate chop splits a tree.",
	    "$n's karate chop splits a tree."
	}
    },

    {
	{
	    "The stones dance to your command.",
	    "The stones dance to $n's command.",
	    "A thunder cloud kneels to you.",
	    "A thunder cloud kneels to $n.",
	    "The Grey Mouser buys you a beer.",
	    "The Grey Mouser buys $n a beer.",
	    "A strap of your armor breaks over your mighty thews.",
	    "A strap of $n's armor breaks over $s mighty thews."
	}
    },

    {
	{
	    "The heavens and grass change colour as you smile.",
	    "The heavens and grass change colour as $n smiles.",
	    "The Burning Man speaks to you.",
	    "The Burning Man speaks to $n.",
	    "Everyone's pocket explodes with your fireworks.",
	    "Your pocket explodes with $n's fireworks.",
	    "A boulder cracks at your frown.",
	    "A boulder cracks at $n's frown."
	}
    },

    {
	{
	    "Everyone's clothes are transparent, and you are laughing.",
	    "Your clothes are transparent, and $n is laughing.",
	    "An eye in a pyramid winks at you.",
	    "An eye in a pyramid winks at $n.",
	    "Everyone discovers your dagger a centimeter from their eye.",
	    "You discover $n's dagger a centimeter from your eye.",
	    "Mercenaries arrive to do your bidding.",
	    "Mercenaries arrive to do $n's bidding."
	}
    },

    {
	{
	    "A black hole swallows you.",
	    "A black hole swallows $n.",
	    "Valentine Michael Smith offers you a glass of water.",
	    "Valentine Michael Smith offers $n a glass of water.",
	    "Where did you go?",
	    "Where did $n go?",
	    "Four matched Percherons bring in your chariot.",
	    "Four matched Percherons bring in $n's chariot."
	}
    },

    {
	{
	    "The world shimmers in time with your whistling.",
	    "The world shimmers in time with $n's whistling.",
	    "The great god Mota gives you a staff.",
	    "The great god Mota gives $n a staff.",
	    "Click.",
	    "Click.",
	    "Atlas asks you to relieve him.",
	    "Atlas asks $n to relieve him."
	}
    }
};



void do_pose( CHAR_DATA *ch, char *argument )
{
    int level;
    int pose;

    if ( IS_NPC(ch) )
	return;

    level = UMIN( ch->level, sizeof(pose_table) / sizeof(pose_table[0]) - 1 );
    pose  = number_range(0, level);

    act( pose_table[pose].message[2*ch->class+0], ch, NULL, NULL, TO_CHAR );
    act( pose_table[pose].message[2*ch->class+1], ch, NULL, NULL, TO_ROOM );

    return;
}



void do_bug( CHAR_DATA *ch, char *argument )
{
    append_file( ch, BUG_FILE, argument );
    send_to_char( "Bug logged.\n\r", ch );
    return;
}

void do_typo( CHAR_DATA *ch, char *argument )
{
    append_file( ch, TYPO_FILE, argument );
    send_to_char( "Typo logged.\n\r", ch );
    return;
}

void do_rent( CHAR_DATA *ch, char *argument )
{
    send_to_char( "There is no rent here.  Just save and quit.\n\r", ch );
    return;
}


void do_qui( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to QUIT, you have to spell it out.\n\r", ch );
    return;
}



void do_quit( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d,*d_next;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    int id, sn;
    CHAR_DATA *wch;
    CHAR_DATA *wch_next;

    if ( IS_NPC(ch) )
	return;

    if (IS_AFFECTED(ch,AFF_CHARM))
	{
	 send_to_char("Now why would you want to leave your master?\n\r",ch);
	 return;
	}

	if (arena) {
	send_to_char("You can't quit now, the Arena is running!\n\r",ch);
	}

    if ( ch->position == POS_FIGHTING )
    {
	send_to_char( "No way! You are fighting.\n\r", ch );
	return;
    }

    if ( ch->position  < POS_STUNNED  )
    {
	send_to_char( "You're not DEAD yet.\n\r", ch );
	return;
    }

    if ( ch->pause > 0 )
    {
	send_to_char( "Your adrenaline is gushing too much to quit!\n\r", ch );
	return;
    }
    if (is_affected(ch,gsn_blackjack))
    {
	send_to_char("You're blackjacked, you can't quit!\n\r",ch);
	return;
    }
    if (is_affected(ch,gsn_sleep))
    {
	send_to_char("You can't quit!\n\r",ch);
	return;
    }

sn = skill_lookup("channel");

    if (is_affected(ch,sn) )
	{
	 affect_strip(ch, sn);
	}

    sn = skill_lookup("fortitude");
    if (is_affected(ch,sn) )
	{
	affect_strip(ch, sn);
	}

    if (is_affected(ch,gsn_cloak_form))
    {
		do_shadowform(ch,"auto");
	}

   if (IS_AFFECTED(ch,AFF_CHARM))
    {
	send_to_char("You don't want to leave your master yet!\n\r",ch);
	return;
    }

    if (ch->in_room->cabal != 0)
    {
        if (ch->in_room->cabal != ch->cabal)
    {
        send_to_char("You can't quit out in an enemy Cabal!\n\r",ch);
	return;
    }
    }

    for (obj = object_list; obj != NULL; obj = obj_next)
    {
	obj_next = obj->next;
	if (isCabalItem(obj))
 	{
	if (obj->carried_by != NULL && obj->carried_by == ch) {
	act("A bolt from the heavens comes down and smites $n!",ch,obj,0,TO_ROOM);
	act("A bolt from the heavens comes down and smites your bitch ass!",ch,obj,0,TO_CHAR);
	send_to_char("You can't quit with cabal items in inventory.", ch);
       //	raw_kill(ch,ch);
		return;
	}
	}

    }

	if (IS_SET(ch->comm, COMM_MOUNTED))
	{
		do_dismount(ch,"");
	}
//  if (is_affected(ch,gsn_cloak_form))
//  affect_strip(ch, gsn_cloak_form);
    if (ch->cabal != 0)
	    announce_logout(ch);
    send_to_char(
	"\n\r{BWelcome back to the Real World!{x\n\r\0",ch);
    act( "$n has left the realm.", ch, NULL, NULL, TO_ROOM );
    if (isShifted(ch))
    do_revert(ch,"");
    sprintf( log_buf, "%s has quit.", ch->name);
    login_log( log_buf );
    log_string( log_buf );
    wiznet("$N rejoins the real world.",ch,NULL,WIZ_LOGINS,0,get_trust(ch));

    /*
     * After extract_char the ch is no longer valid!
     */

/* Remove any Cabal items first , also owners
    and any limiteds on levels 10 and below
*/
    for (obj = object_list; obj != NULL; obj = obj_next)
    {
	obj_next = obj->next;
   	if (isCabalItem(obj))
 	{
	if (obj->carried_by != NULL && obj->carried_by == ch)
		extract_obj(obj);
	}
	else if (obj->pIndexData->limtotal != 0 && ch->level < 10)
	{
	if (obj->carried_by == ch)
		extract_obj(obj);
	}

    }

/* extract all decoys */
    for (wch = char_list; wch != NULL; wch = wch_next)
    {
	wch_next = wch->next;
	if (wch->defending!= NULL
	&&  wch->defending == ch)
		wch->defending  =  NULL;	/* Clear defend */

	if (wch->nightfall != NULL		/* Clear nightfall targets */
	&& wch->nightfall == ch)
		wch->nightfall = NULL;

 	if (!IS_NPC(wch))
		continue;
	if ((wch->pIndexData->vnum == MOB_VNUM_DECOY
	|| wch->pIndexData->vnum == MOB_VNUM_SHADOW)
	&& is_name(ch->name,wch->name))
	{
	act("$n crumbles to dust.",wch,0,0,TO_ROOM);
		extract_char(wch,TRUE);
	}
    }


    save_char_obj( ch );
    id = ch->id;
    d = ch->desc;
    extract_char( ch, TRUE );
    if ( d != NULL )
	close_socket( d );

    /* toast evil cheating bastards */
    for (d = descriptor_list; d != NULL; d = d_next)
    {
	CHAR_DATA *tch;

	d_next = d->next;
	tch = d->original ? d->original : d->character;
	if (tch && tch->id == id)
	{
	    extract_char(tch,TRUE);
	    close_socket(d);
	}
    }

    return;
}



void do_save( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    save_char_obj( ch );
    send_to_char("Saving. Remember that Inahn has automatic saving.\n\r", ch);
    return;
}



void do_follow( CHAR_DATA *ch, char *argument )
{
/* RT changed to allow unlimited following and follow the NOFOLLOW rules */
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Follow whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master != NULL )
    {
	act( "But you'd rather follow $N!", ch, NULL, ch->master, TO_CHAR );
	return;
    }

    if ( victim == ch )
    {
	if ( ch->master == NULL )
	{
	    send_to_char( "You already follow yourself.\n\r", ch );
	    return;
	}
	stop_follower(ch);
	return;
    }

    if (!IS_NPC(victim) && IS_SET(victim->act,PLR_NOFOLLOW) && !IS_IMMORTAL(ch))
    {
	act("$N doesn't seem to want any followers.\n\r",
             ch,NULL,victim, TO_CHAR);
        return;
    }

    REMOVE_BIT(ch->act,PLR_NOFOLLOW);

    if ( ch->master != NULL )
	stop_follower( ch );

    add_follower( ch, victim );
    return;
}


void add_follower( CHAR_DATA *ch, CHAR_DATA *master )
{
    if ( ch->master != NULL )
    {
	bug( "Add_follower: non-null master.", 0 );
	return;
    }

    ch->master        = master;
    ch->leader        = NULL;

    if ( can_see( master, ch ) )
	act( "$n now follows you.", ch, NULL, master, TO_VICT );

    act( "You now follow $N.",  ch, NULL, master, TO_CHAR );

    return;
}



void stop_follower( CHAR_DATA *ch )
{
    if ( ch->master == NULL )
    {
	bug( "Stop_follower: null master.", 0 );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CHARM) )
    {
	REMOVE_BIT( ch->affected_by, AFF_CHARM );
	affect_strip( ch, gsn_charm_person );
    }

    if ( can_see( ch->master, ch ) && ch->in_room != NULL)
    {
	act( "$n stops following you.",     ch, NULL, ch->master, TO_VICT    );
    	act( "You stop following $N.",      ch, NULL, ch->master, TO_CHAR    );
    }
    if (ch->master->pet == ch)
	ch->master->pet = NULL;

    ch->master = NULL;
    ch->leader = NULL;
    return;
}

/* nukes charmed monsters and pets */
void nuke_pets( CHAR_DATA *ch )
{
    CHAR_DATA *pet;

    if ((pet = ch->pet) != NULL)
    {
    	stop_follower(pet);
    	if (pet->in_room != NULL)
    	    act("$N slowly fades away.",ch,NULL,pet,TO_NOTVICT);
    	extract_char(pet,TRUE);
    }
    ch->pet = NULL;

    return;
}



void die_follower( CHAR_DATA *ch )
{
    CHAR_DATA *fch;
    CHAR_DATA *fch_next;

    if ( ch->master != NULL )
    {
    	if (ch->master->pet == ch)
    	    ch->master->pet = NULL;
	stop_follower( ch );
    }

    ch->leader = NULL;

    for ( fch = char_list; fch != NULL; fch = fch_next )
    {
/* NPC fading added by Ceran...you'll have to keep this in cos tracking
crashes if you don't clear all trackers and followers.
*/
	fch_next = fch->next;
	if ( (IS_NPC(fch)) && ( (is_affected(fch,gsn_animate_dead)) || (IS_AFFECTED(fch,AFF_CHARM) ) ) )
 	{
	if  (fch->master == ch)
	{
	REMOVE_BIT(fch->affected_by,AFF_CHARM);
	affect_strip(fch,gsn_animate_dead);
	act("$n slowly fades away.",fch,NULL,NULL,TO_ROOM);
	extract_char(fch,TRUE);
	}
	}
	else
	{
	if ( fch->master == ch )
	    stop_follower( fch );
	if ( fch->leader == ch )
	    fch->leader = fch;
	}
    }

    return;
}



void do_order( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *och;
    CHAR_DATA *och_next;
    bool found;
    bool fAll;

    argument = one_argument( argument, arg );
    one_argument(argument,arg2);

    if (!str_cmp(arg2,"remove") || !str_cmp(arg,"rem"))
    {
		send_to_char("Don't do that!\n\r",ch);
		return;
	}

    if(is_affected(ch,gsn_tigerclaw))
    {
	send_to_char("Your throat constricts too painfully.\n\r",ch);
	return;
    }

	if (is_affected(ch, gsn_gag) )
	{
    	send_to_char("You find difficultly talking.\n\r",ch);
    	return;
	}

    if (!str_cmp(arg2,"delete") || !str_cmp(arg2,"mob"))
    {
        send_to_char("That will NOT be done.\n\r",ch);
        return;
    }

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Order whom to do what?\n\r", ch );
	return;
    }

    if ( IS_AFFECTED( ch, AFF_CHARM ) )
    {
	send_to_char( "You feel like taking, not giving, orders.\n\r", ch );
	return;
    }
    if (!str_cmp(arg2,"trip")
	|| !str_cmp(arg2,"bash")
	|| !str_cmp(arg2,"palm")
	|| !str_cmp(arg2,"throw"))
	return;

    if ( !str_cmp( arg, "all" ) )
    {
	fAll   = TRUE;
	victim = NULL;
    }
    else
    {
	fAll   = FALSE;
	if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	if ( victim == ch )
	{
	    send_to_char( "Aye aye, right away!\n\r", ch );
	    return;
	}

	if (!IS_AFFECTED(victim, AFF_CHARM) || victim->master != ch
	||  (IS_IMMORTAL(victim) && victim->trust >= ch->trust))
	{
	    send_to_char( "Do it yourself!\n\r", ch );
	    return;
	}
    }

    found = FALSE;

    for ( och = ch->in_room->people; och != NULL; och = och_next )
    {
	och_next = och->next_in_room;

	if ( IS_AFFECTED(och, AFF_CHARM)
	&&   och->master == ch
	&& ( fAll || och == victim ) )
	{
	    found = TRUE;
	    sprintf( buf, "$n orders you to '%s'.", argument );
	    act( buf, ch, NULL, och, TO_VICT );
	    interpret( och, argument );
	}
    }

    if ( found )
    {
	WAIT_STATE(ch,PULSE_VIOLENCE);
	send_to_char( "Ok.\n\r", ch );
    }
    else
	send_to_char( "You have no followers here.\n\r", ch );
    return;
}




void do_group( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	CHAR_DATA *gch;
	CHAR_DATA *leader;

	leader = (ch->leader != NULL) ? ch->leader : ch;
	sprintf( buf, "%s's group:\n\r", PERS(leader, ch) );
	send_to_char( buf, ch );

	for ( gch = char_list; gch != NULL; gch = gch->next )
	{
	    if ( is_same_group( gch, ch ) )
	    {
                sprintf( buf,
                "[%2d %s] %-16s %3d%%hp %3d%%mana %3d%%mv %5d xp\n\r",
                    gch->level,
                    IS_NPC(gch) ? "Mob" : class_table[gch->class].who_name,
                    capitalize( PERS(gch, ch) ),
	(gch->max_hit == 0) ? 0 : (gch->hit*100)/gch->max_hit,
	(gch->max_mana == 0) ? 0 :
                    (gch->mana*100)/gch->max_mana,
	(gch->max_move == 0) ? 0 :
                    (gch->move*100)/gch->max_move,
                    gch->exp );
                send_to_char( buf, ch );
            }
	}
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( ch->master != NULL || ( ch->leader != NULL && ch->leader != ch ) )
    {
	send_to_char( "But you are following someone else!\n\r", ch );
	return;
    }

    if ( victim->master != ch && ch != victim )
    {
	act_new("$N isn't following you.",ch,NULL,victim,TO_CHAR,POS_SLEEPING);
	return;
    }

    if (IS_AFFECTED(victim,AFF_CHARM))
    {
        send_to_char("You can't remove charmed mobs from your group.\n\r",ch);
        return;
    }

    if (IS_AFFECTED(ch,AFF_CHARM))
    {
    	act_new("You like your master too much to leave $m!",
	    ch,NULL,victim,TO_VICT,POS_SLEEPING);
    	return;
    }

    if ( is_same_group( victim, ch ) && ch != victim )
    {
	victim->leader = NULL;
	act_new("$n removes $N from $s group.",
	    ch,NULL,victim,TO_NOTVICT,POS_RESTING);
	act_new("$n removes you from $s group.",
	    ch,NULL,victim,TO_VICT,POS_SLEEPING);
	act_new("You remove $N from your group.",
	    ch,NULL,victim,TO_CHAR,POS_SLEEPING);
	return;
    }

    if (IS_GOOD(victim) && IS_EVIL(ch))
    {
	send_to_char( "That person is too pure to group with you.\n\r", ch);
	return;
    }

    if (IS_EVIL(victim) && IS_GOOD(ch))
    {
	send_to_char( "That person is too evil to group with you.\n\r", ch );
	return;
    }

    victim->leader = ch;
    act_new("$N joins $n's group.",ch,NULL,victim,TO_NOTVICT,POS_RESTING);
    act_new("You join $n's group.",ch,NULL,victim,TO_VICT,POS_SLEEPING);
    act_new("$N joins your group.",ch,NULL,victim,TO_CHAR,POS_SLEEPING);
    return;
}



/*
 * 'Split' originally by Gnort, God of Chaos.
 */
void do_split( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    int members;
    int amount_gold = 0, amount_silver = 0;
    int share_gold, share_silver;
    int extra_gold, extra_silver;

    argument = one_argument( argument, arg1 );
	       one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Split how much?\n\r", ch );
	return;
    }

    amount_silver = atoi( arg1 );

    if (arg2[0] != '\0')
	amount_gold = atoi(arg2);

    if ( amount_gold < 0 || amount_silver < 0)
    {
	send_to_char( "Your group wouldn't like that.\n\r", ch );
	return;
    }

    if ( amount_gold == 0 && amount_silver == 0 )
    {
	send_to_char( "You hand out zero coins, but no one notices.\n\r", ch );
	return;
    }

    if ( ch->gold <  amount_gold || ch->silver < amount_silver)
    {
	send_to_char( "You don't have that much to split.\n\r", ch );
	return;
    }

    members = 0;
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( is_same_group( gch, ch ) && !IS_AFFECTED(gch,AFF_CHARM))
	    members++;
    }

    if ( members < 2 )
    {
	send_to_char( "Just keep it all.\n\r", ch );
	return;
    }

    share_silver = amount_silver / members;
    extra_silver = amount_silver % members;

    share_gold   = amount_gold / members;
    extra_gold   = amount_gold % members;

    if ( share_gold == 0 && share_silver == 0 )
    {
	send_to_char( "Don't even bother, cheapskate.\n\r", ch );
	return;
    }

    ch->silver	-= amount_silver;
    ch->silver	+= share_silver + extra_silver;
    ch->gold 	-= amount_gold;
    ch->gold 	+= share_gold + extra_gold;

    if (share_silver > 0)
    {
	sprintf(buf,
	    "You split %d silver coins. Your share is %d silver.\n\r",
 	    amount_silver,share_silver + extra_silver);
	send_to_char(buf,ch);
    }

    if (share_gold > 0)
    {
	sprintf(buf,
	    "You split %d gold coins. Your share is %d gold.\n\r",
	     amount_gold,share_gold + extra_gold);
	send_to_char(buf,ch);
    }

    if (share_gold == 0)
    {
	sprintf(buf,"$n splits %d silver coins. Your share is %d silver.",
		amount_silver,share_silver);
    }
    else if (share_silver == 0)
    {
	sprintf(buf,"$n splits %d gold coins. Your share is %d gold.",
	amount_gold,share_gold);
    }
    else
    {
	sprintf(buf,
"$n splits %d silver and %d gold coins, giving you %d silver and %d gold.\n\r",
	 amount_silver,amount_gold,share_silver,share_gold);
    }

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( gch != ch && is_same_group(gch,ch) && !IS_AFFECTED(gch,AFF_CHARM))
	{
	    act( buf, ch, NULL, gch, TO_VICT );
	    gch->gold += share_gold;
	    gch->silver += share_silver;
	}
    }

    return;
}



void do_gtell( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *gch;

    if(isShifted(ch) && !IS_IMMORTAL(ch))
    {

	send_to_char("You can't seem to form the words.\n\r",ch);
	return;
    }

    if(is_affected(ch,gsn_silence))
    {
	send_to_char("You cannot find the words within you.\n\r",ch);
	return;
    }

	if (is_affected(ch, gsn_gag) )
	{
    	send_to_char("You find difficultly talking.\n\r",ch);
    	return;
	}

    if ( argument[0] == '\0' )
    {
	send_to_char( "Tell your group what?\n\r", ch );
	return;
    }

    if ( IS_SET( ch->comm, COMM_NOTELL ) )
    {
	send_to_char( "Your message didn't get through!\n\r", ch );
	return;
    }

   argument = garble(ch, argument);

	if (is_affected(ch,gsn_deafen)) {
//		argument = upstring(ch,argument);
argument = str_dup(upstring(argument));
	}

    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( is_same_group( gch, ch ) )
	{

	    if (IS_SET(gch->act,PLR_COLOR))
            act_new("$n tells the group '\x01B[1;35m$t\x01B[0;37m'",
                ch,argument,gch,TO_VICT,POS_SLEEPING);
	    else
            act_new("$n tells the group '$t'",
                ch,argument,gch,TO_VICT,POS_SLEEPING);
	}
    }

    if (IS_SET(ch->act,PLR_COLOR))
    act_new("You tell the group '\x01B[1;35m$t\x01B[0;37m'",ch,argument,NULL,TO_CHAR,POS_SLEEPING);
    else
    act_new("You tell the group '$t'",ch,argument,NULL,TO_CHAR,POS_SLEEPING);
    return;
}



/*
 * It is very important that this be an equivalence relation:
 * (1) A ~ A
 * (2) if A ~ B then B ~ A
 * (3) if A ~ B  and B ~ C, then A ~ C
 */
bool is_same_group( CHAR_DATA *ach, CHAR_DATA *bch )
{
    if ( ach == NULL || bch == NULL)
	return FALSE;
/*    if ( ( ach->level - bch->level > 8 || ach->level - bch->level < -8 ) && !IS_NPC(ach) )
	return FALSE;*/
    if ( ach->leader != NULL ) ach = ach->leader;
    if ( bch->leader != NULL ) bch = bch->leader;
    return ach == bch;
}

void do_release(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument(argument,arg);
    if (arg[0] == '\0')
    {
	send_to_char("Release which follower?\n\r",ch);
	return;
    }

    victim = get_char_room(ch,arg);
    if (victim == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
    return;
    }

    if (!IS_AFFECTED(victim,AFF_CHARM)
    || victim->master != ch)
    {
	send_to_char("They aren't under your control.\n\r",ch);
	return;
    }

    if (IS_NPC(victim))
    {
    act("$n slowly fades away.",victim,0,0,TO_ROOM);
    extract_char(victim,TRUE);
    }
    else
    {
    send_to_char("You can't release a player into the void..maybe nofollow them?\n\r",ch);
    return;
    }
    return;
}



/* Rod of lordly might for use in drannor..take out if you don't want to
use drannor areas...(Ceran)
*/
void obj_say_lordly(CHAR_DATA *ch,OBJ_DATA *obj,char *argument)
{
        if (strstr(argument,"rulers are just figureheads"))
        {
if (obj->item_type == ITEM_TRASH)
        {
        send_to_char("Your rod warms a little but nothing happens.\n\r",ch);
        WAIT_STATE(ch,12);
        return;
        }
        obj->value[0] = 0;
        obj->value[1] = 0;
        obj->value[2] = 0;
        obj->item_type = ITEM_TRASH;
        REMOVE_BIT(obj->wear_flags,ITEM_WIELD);
        SET_BIT(obj->wear_flags,ITEM_HOLD);
        obj->value[3] = 0;
        free_string(obj->short_descr);
        obj->short_descr = "the rod of lordly might";
        act("$n's rod of lordly might clicks and changes form into a nodesript staff.",ch,0,0,TO_ROOM);
        send_to_char("Your rod of lordly might clicks and changes into a nondescript staff.\n\r",ch);
        WAIT_STATE(ch,12);
        return;
        }


        if (strstr(argument,"rule with an iron fist"))
        {
if (obj->value[0] == WEAPON_MACE)
        {
        send_to_char("Your rod warms a little but nothing happens.\n\r",ch);
        WAIT_STATE(ch,12);
        return;
        }
        obj->value[0] = WEAPON_MACE;
        obj->value[1] = 7;
        obj->value[2] = 7;
        obj->item_type = ITEM_WEAPON;
        REMOVE_BIT(obj->wear_flags,ITEM_HOLD);
        SET_BIT(obj->wear_flags,ITEM_WIELD);
        obj->value[3] = attack_lookup("crush");
        free_string(obj->short_descr);
        obj->short_descr = "A rod with a large heavy mace head";
        act("$n's rod of lordly might clicks and changes form into a mace!",ch,0,0,TO_ROOM);
        send_to_char("Your rod of lordly might clicks and changes into a mace!\n\r",ch);
        WAIT_STATE(ch,12);
        return;
        }

        if (strstr(argument,"the strong rule the weak"))
        {
if (obj->value[0] == WEAPON_SWORD)
        {
        send_to_char("Your rod warms a little but nothing happens.\n\r",ch);
        WAIT_STATE(ch,12);
        return;
        }
        obj->value[0] = WEAPON_SWORD;
        obj->value[1] = 7;
        obj->value[2] = 7;
        obj->item_type = ITEM_WEAPON;
        REMOVE_BIT(obj->wear_flags,ITEM_HOLD);
        SET_BIT(obj->wear_flags,ITEM_WIELD);
        obj->value[3] = attack_lookup("slash");
        free_string(obj->short_descr);
        obj->short_descr = "A rod with long slim blade";
        act("$n's rod of lordly might clicks and changes form into a sword!",ch,0,0,TO_ROOM);
        send_to_char("Your rod of lordly might clicks and changes into a sword!\n\r",ch);
        WAIT_STATE(ch,12);
        return;
        }

        if (strstr(argument,"rulership is for the divine"))
        {
if (obj->value[0] == WEAPON_EXOTIC)
        {
        send_to_char("Your rod warms a little but nothing happens.\n\r",ch);
        WAIT_STATE(ch,12);
        return;
        }
        obj->value[0] = WEAPON_EXOTIC;
        obj->value[1] = 7;
        obj->value[2] = 7;
        obj->item_type = ITEM_WEAPON;
        REMOVE_BIT(obj->wear_flags,ITEM_HOLD);
        SET_BIT(obj->wear_flags,ITEM_WIELD);
        obj->value[3] = attack_lookup("divine");
        free_string(obj->short_descr);
        obj->short_descr = "A rod wreathed in blue flames";
        act("$n's rod of lordly might elongates and suddenly becomes wreathed in blue flames!",ch,0,0,TO_ROOM);
        send_to_char("Your rod of lordly might elongates and suddenly becomes wreathed in blue flames!\n\r",ch);
        WAIT_STATE(ch,12);
        return;
        }

        return;
}

/* a mob_say_trigger */

void mob_say_lady(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *lady;

    if (IS_NPC(ch))
        return;

    for (lady = ch->in_room->people; lady != NULL; lady = lady->next_in_room)
    {
        if (!IS_NPC(lady))    continue;
        if (lady->pIndexData->vnum == MOB_VNUM_DRANNOR_LADY)
            break;
    }
    if (lady == NULL)
	return;
    if (lady->position == POS_FIGHTING || lady->position == POS_SLEEPING)
        return;


    if (!str_cmp(argument,"yes"))
    {
        do_say(lady,"Bring me the head of the beast that slew my people as they fled to the seas.");
        do_say(lady,"It lives in the bloody waters, feeding off the bones of the elves it killed as they tried to flee.");
        return;
    }
    if (strstr(argument,"beast"))
    {
        do_say(lady,"The beast is dangerous beyond mortal comprehension.");
        do_say(lady,"Find the fallen knight in the towers and retrieve the slayer to kill the beast.");
        act("$n flickers for a moment then resumes her tranquil position of waiting.",lady,0,0,TO_ROOM);
        return;
    }
    return;
}


/* This is to introduce a basic random value in the ageing process. The
amount varies for each race since they live different lifespans .
*/
void get_age_mod(CHAR_DATA *ch)
{
	int mod;
	char *race;

	if (IS_NPC(ch))
	return;

	race = pc_race_table[ch->race].name;
	if (!str_cmp(race,"human"))
		mod = dice(2,6);
	else if (!str_cmp(race,"elf") || !str_cmp(race,"grey-elf")
	|| !str_cmp(race,"dark-elf"))
	{
		mod = dice(6,8);
		mod += dice(6,10);
	}
	else if (!str_cmp(race,"cloud") || !str_cmp(race,"centaur")
	|| !str_cmp(race,"troll"))
		mod = dice(4,5);
	else if (!str_cmp(race,"gnome"))
	{
		mod = dice(6,8);
		mod += dice(10,11);
	}
	else if (!str_cmp(race,"dwarf"))
		mod = dice(5,6);
	else
		mod = dice(4,5);

	ch->pcdata->age_mod = mod;
	return;
}

/* A new auto setting so players can indicate whether they want to accept
objects sent via the transfer item spell.
*/
void do_notransfer(CHAR_DATA *ch,char *argument)
{
	if (IS_NPC(ch))
		return;

	if (IS_SET(ch->act,PLR_NO_TRANSFER))
	{
	REMOVE_BIT(ch->act,PLR_NO_TRANSFER);
	send_to_char("You now accept transfered objects.\n\r",ch);
	return;
	}

	SET_BIT(ch->act,PLR_NO_TRANSFER);
	send_to_char("You no longer accept transfered objects.\n\r",ch);
	return;
}

void do_evaluation(CHAR_DATA *ch,char *argument)
{
	if (IS_NPC(ch))
		return;

	if (IS_SET(ch->act,PLR_EVALUATION))
	{
	send_to_char("You no longer attempt to evaluate your opponent's wounds.\n\r",ch);
	REMOVE_BIT(ch->act,PLR_EVALUATION);
	return;
	}
	send_to_char("You now attempt to evaluate your opponent's wounds.\n\r",ch);
	SET_BIT(ch->act,PLR_EVALUATION);
	return;
}


/* Say progs for legion.are...remove these if you don't want to use
legion.are
*/
void say_prog_summon_demon(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *mob;
    char buf[MAX_STRING_LENGTH];
    if (ch->in_room == NULL
    || ch->in_room->vnum != 29824)
        return;

    if (!strstr(argument,"Yal Dagon Nar Sothoth")
    && !strstr(argument,"Yal dagon nar sothoth")
    && !strstr(argument,"yal dagon nar sothoth"))
        return;

    for (mob = char_list;mob != NULL; mob = mob->next)
        if (IS_NPC(mob) && mob->pIndexData->vnum == 29736
	&& mob->in_room != NULL
	&& mob->in_room->vnum != 29599)
            break;

    if (mob != NULL)
        return;

    for (mob = char_list;mob != NULL; mob = mob->next)
        if (IS_NPC(mob) && mob->pIndexData->vnum == 29736
	&& mob->in_room != NULL
	&& mob->in_room->vnum == 29599)
            break;

    if (mob == NULL)
        return;
	char_from_room(mob);
act("The pentegram sizzles and steams and a huge demon steps out from a gate!",ch,0,0,TO_ROOM);
act("The pentegram sizzles and steams and a huge demon steps out from a gate!",ch,0,0,TO_CHAR);

    char_to_room(mob,ch->in_room);

    do_yell(mob,"You dare to summon me mortal!");
    sprintf(buf,"Help! I'm being attacked by %s!",mob->short_descr);
    do_yell(ch,buf);
    multi_hit(mob,ch,-1);

    return;


}

void say_prog_raise_shadowlord(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *mob;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    char buf[MAX_STRING_LENGTH];

    if (ch->in_room == NULL
    || ch->in_room->vnum != 29842)
        return;

    if (!strstr(argument,"yal nosferatu vampyri sothoth"))
        return;

    for (mob = mob = char_list;mob != NULL; mob = mob->next)
        if (IS_NPC(mob) && mob->pIndexData->vnum == 29735
	&& mob->in_room != NULL
	&& mob->in_room->vnum != 29599)
                break;

    if (mob != NULL)
        return;

    for (mob = char_list;mob != NULL; mob = mob->next)
        if (IS_NPC(mob) && mob->pIndexData->vnum == 29735
	&& mob->in_room != NULL
	&& mob->in_room->vnum == 29599)
            break;

    if (mob == NULL)
        return;

        for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	    {
        obj_next = obj->next_content;
        if (obj->pIndexData->vnum == 29757)
            break;
        }
        if (obj == NULL)
            return;
act("The corpses explode into a bloody mist and something dark forms out of it!",ch,0,0,TO_ROOM);
act("The corpses explode into a bloody mist and something dark forms out of it!",ch,0,0,TO_CHAR);

	extract_obj(obj);
	char_from_room(mob);

    char_to_room(mob,ch->in_room);
    do_say(mob,"Feeeed me mortal!");
    sprintf(buf,"Help! I'm being attacked by %s!",mob->short_descr);
    do_yell(ch,buf);

    multi_hit(mob,ch,-1);
    return;
}

/* This mostly rips off the venueport code..for use in legion.are */
void say_prog_karguska(CHAR_DATA *ch, char *argument)
{
    int vnum;
    ROOM_INDEX_DATA *to_room;
    CHAR_DATA *group;
    CHAR_DATA *arrival;
    CHAR_DATA *g_next;
    ROOM_INDEX_DATA *VENUEPORT;
    CHAR_DATA *last_to_venue;
    int numb;

    if (ch->in_room == NULL)
        return;

    if (!strstr(argument,"karguska"))
	return;

    vnum = 0;

    if (ch->in_room->vnum == 29706)
        vnum = 29520;
    else if (ch->in_room->vnum == 29707)
        vnum = 29521;
    else if (ch->in_room->vnum == 29708)
        vnum = 29522;
    else if (ch->in_room->vnum == 29709)
        vnum = 29523;
    else if (ch->in_room->vnum == 29520)
        vnum = 29706;
    else if (ch->in_room->vnum == 29521)
        vnum = 29707;
    else if (ch->in_room->vnum == 29522)
        vnum = 29708;
    else if (ch->in_room->vnum == 29523)
        vnum = 29709;
    else
        vnum = 0;
    if (vnum == 0)
        return;

    VENUEPORT = get_room_index(ROOM_VNUM_VENUEPORT);
    to_room = get_room_index(vnum);
    if (to_room == NULL || VENUEPORT == NULL)
        return;

    last_to_venue = ch;

    numb = 1;
    for (group = ch->in_room->people; group != NULL; group = g_next)
    {
	g_next = group->next_in_room;
	if (!is_same_group(group,ch) || (group->fighting != NULL) )
		continue;
	if (group == ch )
		continue;
	numb++;
send_to_char("The penegram glows red and with a blinding flash of light you are somewhere else!\n\r",group);

    char_from_room(group);
    char_to_room(group,to_room);
    act("The pentegram on the floor glows red and $n fades into existence.",group,0,0,TO_ROOM);

    char_from_room(group);
	char_to_room(group, VENUEPORT);
	last_to_venue = group;
   }

act("The pentegram on the floor glows red and there is a blinding flash of light!",ch,0,0,TO_ROOM);

send_to_char("The penegram glows red and with a blinding flash of light you are somewhere else!\n\r",ch);
   char_from_room(ch);
   char_to_room(ch,to_room);
   act("The pentegram on the floor glows red and $n fades into existence.",ch,0,0,TO_ROOM);

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
    if (--numb <= 0)
	break;
    }
return;
}

void perm_death_log( CHAR_DATA *ch, int deltype )
{
	FILE *fp;

	if ((fp = fopen( PDEATH_LOG_FILE, "a" )) != NULL)
	{
		if(deltype==1) {
			fprintf(fp,"DELETE||[%2d %5s %s]||%s%s%s%s||Kills: %d||Age: %d||Hours: %d||%s",
			ch->level,
			pc_race_table[ch->race].who_name,
			class_table[ch->class].who_name,
			cabal_table[ch->cabal].who_name,
			(ch->original_name) ? ch->original_name : ch->name,
			IS_NPC(ch) ? "" : ch->pcdata->title,
			IS_NPC(ch) ? "" : (ch->pcdata->extitle) ? ch->pcdata->extitle : "",
			ch->pcdata->kills[PK_KILLS],
			get_age(ch),
			(int) (ch->played + current_time - ch->logon) / 3600,
			(char *) ctime( &current_time )
			);
		}
		if(deltype==2) {
			fprintf(fp,"CON-DIE||[%2d %5s %s]||%s%s%s%s||Kills: %d||Age: %d||Hours: %d||%s",
			ch->level,
			pc_race_table[ch->race].who_name,
			class_table[ch->class].who_name,
			cabal_table[ch->cabal].who_name,
			(ch->original_name) ? ch->original_name : ch->name,
			IS_NPC(ch) ? "" : ch->pcdata->title,
			IS_NPC(ch) ? "" : (ch->pcdata->extitle) ? ch->pcdata->extitle : "",
			ch->pcdata->kills[PK_KILLS],
			get_age(ch),
			(int) (ch->played + current_time - ch->logon) / 3600,
			(char *) ctime( &current_time )
			);
		}
	}
	fclose(fp);
}

void temp_death_log( CHAR_DATA *killer, CHAR_DATA *dead)
{
	FILE *fp;
	char deadname[MAX_STRING_LENGTH];
	char killname[MAX_STRING_LENGTH];

	if (IS_IMMORTAL(killer)) {
		return;
	}

	if ( IS_NPC(killer) && (killer->master != NULL)) {

		killer = killer->master;
	}

	if (IS_NPC(killer) || (IS_NPC(dead))) {
		return;
	}

	if ( !IS_NPC(killer) && is_affected(killer,gsn_cloak_form)) {
		sprintf(killname,killer->original_name);
	} else {
		sprintf(killname,killer->name);
	}

	if ( !IS_NPC(dead) && is_affected(dead,gsn_cloak_form)) {
		sprintf(deadname,dead->original_name);
	} else {
		sprintf(deadname,dead->name);
	}

	if ((fp = fopen( DEATH_LOG_FILE, "a" )) != NULL)
	{
		fprintf(fp,"%s | %s | %s | %s",
		deadname,
		(IS_NPC(killer) ? killer->short_descr : killname),
		killer->in_room->name,
		(char *) ctime( &current_time )
		);
	}
	fclose(fp);
}

void login_log(const char *str)
{
        FILE *fp;
        char *strtime;

        strtime = ctime(&current_time);
        strtime[strlen(strtime)-1] = '\0';


        if ((fp = fopen( LOGIN_LOG_FILE, "a" )) != NULL)
        {
                fprintf( fp, "%s :: %s\n", strtime, str );
                  fclose(fp);
        }
        return;
}
