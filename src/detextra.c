/*******************/
/* Detlef's Extras */
/*******************/

#include "include.h"

DECLARE_DO_FUN(do_myell);
DECLARE_DO_FUN(do_yell);
#define LOGIN_LOG_FILE	"logins.txt"
#define TEMP_GREP_RESULTS_TWO	"../temp/tempgrepresults2.tmp"

int battlecry_multiplier   args((CHAR_DATA *ch, int dt));
char *target_name;

bool IS_IMP(CHAR_DATA *ch)
{
     if(get_trust(ch)==MAX_LEVEL)
             return TRUE;
     return FALSE;
}

void do_gouge( CHAR_DATA *ch, char *argument )
{
    bool attempt_dual;
    char arg[MAX_INPUT_LENGTH];
    AFFECT_DATA af;
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int chance;
    int dam;

    attempt_dual = FALSE;
    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_gouge)) == 0
    ||  IS_NPC(ch)
    ||  (!IS_NPC(ch) && ch->level < skill_table[gsn_gouge].skill_level[ch->class]) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	send_to_char("But you aren't fighting anyone.\n\r",ch);
	return;
	}
    }
    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
	return;
    }

    if (ch->fighting == NULL)
	{
	send_to_char("You can't gouge someone's eye like that.\n\r",ch);
	return;
	}

    if (is_affected(victim,gsn_gouge) )
      {
      send_to_char("Their eye is already gouged.\n\r",ch);
      return;
      }

    if (victim == ch)
        {
	send_to_char("Huh?\n\r", ch);
	return;
        }

   obj = get_eq_char(ch,WEAR_WIELD);
   if (obj == NULL || obj->value[0] != WEAPON_DAGGER)
	{
	attempt_dual = TRUE;
	obj = get_eq_char(ch,WEAR_DUAL_WIELD);
	}

   if (obj == NULL || obj->value[0] != WEAPON_DAGGER)
	{
	attempt_dual = TRUE;
	obj = get_eq_char(ch,WEAR_TERTIARY_WIELD);
	}

	if (obj == NULL)
	{
	send_to_char("You must wield a dagger to gouge someone.\n\r",ch);
	return;
	}

    if (obj->value[0] != 2)
    {
	send_to_char("You must wield a dagger to gouge someone.\n\r",ch);
	return;
    }

    chance += (ch->drain_level + ch->level - victim->level - victim->drain_level);

    WAIT_STATE(ch,skill_table[gsn_gouge].beats);

    if (number_percent() < chance - 40)
    {
        act("You attempt to gouge at $N's eye.",ch,NULL,victim,TO_CHAR);
	act("$n attempts to gouge at your eye.",ch,NULL,victim,TO_VICT);
	act("$n attempts to gouge at $N's eye.",ch,NULL,victim,TO_NOTVICT);
        act("$N appears to be blinded.",ch,NULL,victim,TO_CHAR);
	act("You are blinded.",ch,NULL,victim,TO_VICT);
	act("$N appears to be blinded.",ch,NULL,victim,TO_NOTVICT);
	init_affect(&af);
        af.where = TO_AFFECTS;
        af.aftype = AFT_SKILL;
        af.duration = ch->level/25;
	af.bitvector = AFF_BLIND;
        af.type=gsn_gouge;
        af.level = ch->level;
        af.location = APPLY_HITROLL;
        af.modifier = -5;
        affect_to_char(victim,&af);
	check_improve(ch,gsn_gouge,TRUE,1);
	dam = dice(obj->value[1],obj->value[2]);
	dam += 10;

	WAIT_STATE(ch,PULSE_VIOLENCE*1);

	if ((ch->drain_level + ch->level) <= 15)
		dam *= 2;
	else if ((ch->drain_level + ch->level) <= 20)
                dam *= 2.5;
	else if ((ch->drain_level + ch->level) < 25)
                dam *= 3;
	else if ((ch->drain_level + ch->level) < 30)
                dam *= 3.5;
	else if ((ch->drain_level + ch->level) < 40)
                dam *= 4;
	else if ((ch->drain_level + ch->level) <= 49)
                dam *= 4.5;
	else if ((ch->drain_level + ch->level) <= 55)
                dam *= 5;
        else dam *= 5;

	dam *= battlecry_multiplier(ch,gsn_gouge);

	damage_old(ch,victim,dam,gsn_gouge, attack_table[obj->value[3]].damage, TRUE);
    }
    else
    {
	check_improve(ch,gsn_gouge,FALSE,1);

	damage_old(ch,victim,0,gsn_gouge,DAM_NONE,TRUE);
    }

    return;
}

void do_stab( CHAR_DATA *ch, char *argument )
{
    bool attempt_dual;
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int chance;
    int dam;

    attempt_dual = FALSE;
    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_stab)) == 0
    ||  IS_NPC(ch)
    ||  (!IS_NPC(ch) && ch->level < skill_table[gsn_stab].skill_level[ch->class]) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	send_to_char("But you aren't fighting anyone.\n\r",ch);
	return;
	}
    }
    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
	return;
    }

    if (ch->fighting == NULL)
	{
	send_to_char("You can't stab someone like that.\n\r",ch);
	return;
	}

    if (victim == ch)
        {
	send_to_char("Huh?\n\r", ch);
	return;
        }

   obj = get_eq_char(ch,WEAR_WIELD);
   if (obj == NULL || obj->value[0] != WEAPON_DAGGER)
	{
	attempt_dual = TRUE;
	obj = get_eq_char(ch,WEAR_DUAL_WIELD);
	}

   if (obj == NULL || obj->value[0] != WEAPON_DAGGER)
        {
        attempt_dual = TRUE;
        obj = get_eq_char(ch,WEAR_TERTIARY_WIELD);
        }

	if (obj == NULL)
	{
	send_to_char("You must wield a dagger to stab someone.\n\r",ch);
	return;
	}

    if (obj->value[0] != 2)
    {
	send_to_char("You must wield a dagger to stab someone.\n\r",ch);
	return;
    }

    chance += (ch->drain_level + ch->level - victim->level - victim->drain_level);
    chance = URANGE(5, chance, 95);

    WAIT_STATE(ch,skill_table[gsn_stab].beats);

    if (number_percent() < chance)
    {
        act("You unleash with a viscious stab on $N.",ch,NULL,victim,TO_CHAR);
	act("$n unleashes with a viscious stab.",ch,NULL,victim,TO_VICT);
	act("$n unleashes with a viscious stab on $N.",ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_stab,TRUE,1);
	dam = dice(obj->value[1],obj->value[2]);
	dam += 40;

	if ((ch->drain_level + ch->level) <= 15)
		dam *= 1;
	else if ((ch->drain_level + ch->level) <= 20)
                dam *= 2;
	else if ((ch->drain_level + ch->level) < 25)
                dam *= 2.5;
	else if ((ch->drain_level + ch->level) < 30)
                dam *= 3;
	else if ((ch->drain_level + ch->level) < 40)
                dam *= 3.5;
	else if ((ch->drain_level + ch->level) <= 49)
                dam *= 4;
	else if ((ch->drain_level + ch->level) <= 55)
                dam *= 4.5;
        else dam *= 4.5;

	dam *= battlecry_multiplier(ch,gsn_stab);

	damage_old(ch,victim,dam,gsn_stab, attack_table[obj->value[3]].damage, TRUE);
    }
    else
    {
	check_improve(ch,gsn_stab,FALSE,1);

	damage_old(ch,victim,0,gsn_stab,DAM_NONE,TRUE);
    }

    return;
}

void spell_dream_blossom(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
  ROOM_AFFECT_DATA af;
  AFFECT_DATA af2;

    if ((ch->in_room->sector_type == SECT_CITY) || (ch->in_room->sector_type == SECT_INSIDE))
    {
        send_to_char("You must be in the wilderness to call the Dream Blossoms.\n\r",ch);
        return;
    }

    if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char("This room is protected by gods.\n\r",  ch);
      return;
    }

    if (cabal_down(ch,CABAL_BRIAR))
	{
	send_to_char("You cannot find the power within you.\n\r",ch);
	return;
	}
    if ( is_affected(ch,gsn_dream_blossom))
	{
	send_to_char("You cannot call the Dream Blossoms again so soon.\n\r",ch);
	return;
	}

    if ( is_affected_room( ch->in_room, sn ))
    {
     send_to_char("The dream blossoms are already growing here.\n\r",ch);
     return;
    }
        init_affect(&af2);
   	af2.where        = TO_AFFECTS;
        af2.type         = gsn_dream_blossom;
        af2.aftype	 = AFT_SKILL;
        af2.level        = ch->level;
        af2.duration     = 24;
        af2.location     = 0;
        af2.modifier     = 0;
        af2.bitvector    = 0;
        affect_to_char(ch,&af2);

    init_affect_room(&af);
    af.where     = TO_ROOM_AFFECTS;
    af.aftype	 = AFT_SPELL;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = 8;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_SLEEP;
    affect_to_room( ch->in_room, &af );
    check_improve(ch,gsn_dream_blossom,TRUE,1);
    WAIT_STATE(ch,12);

    send_to_char("Tiny sprouts shoot from the ground as you command the Dream Blossom to grow in this area.\n\r",ch);
    act("The seductive smell of flowers pervades the area.\n\r",ch,NULL,NULL,TO_ROOM);

}

void spell_randomizer(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    AFFECT_DATA af;
    ROOM_AFFECT_DATA af2;

    if ( is_affected( ch, sn ) )
    {
      send_to_char
        ("Your power of randomness has been exhausted for now.\n\r",
         ch);
      return;
    }

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char(
            "This room is far too orderly for your powers to work on it.\n\r",
                   ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
        send_to_char("This room has already been randomized.\n\r",ch);
        return;
    }

  if (number_bits(1) == 0)
    {
      send_to_char("Despite your efforts, the universe resisted chaos.\n\r",ch);
      af2.where     = TO_AFFECTS;
      af2.type      = sn;
      af2.level     = ch->level;
      af2.duration  = level / 10;
      af2.modifier  = 0;
      af2.location  = APPLY_NONE;
      af2.bitvector = 0;
      affect_to_char( ch, &af );
      return;
    }

    init_affect(&af);
    af.where     = TO_ROOM_AFFECTS;
    af.type      = sn;
    af.aftype    = AFT_POWER;
    af.level     = ch->level;
    af.duration  = level / 15;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_RANDOMIZER;
    affect_to_room( ch->in_room, &af2 );

    init_affect_room(&af2);
    af2.where     = TO_AFFECTS;
    af2.type      = sn;
    af2.aftype	  = AFT_POWER;
    af2.level     = ch->level;
    af2.duration  = level / 5;
    af2.modifier  = 0;
    af2.location  = APPLY_NONE;
    af2.bitvector = 0;
    affect_to_char( ch, &af );
    send_to_char("The room was successfully randomized!\n\r", ch);
    send_to_char("You feel very drained from the effort.\n\r", ch);
    ch->hit -= UMIN(200, ch->hit/2);
    act("The room starts to randomize exits.",ch,NULL,NULL,TO_ROOM);
    return;
}

void do_newbie( CHAR_DATA *ch, char *argument )
{
    if(ch->pcdata->newbie==FALSE)
    {
        send_to_char("You are not a newbie.\n\r",ch);
        return;
    }
    if(!strcmp(argument,"off"))
    {
        ch->pcdata->newbie=FALSE;
        send_to_char("You are no longer a protected newbie.\n\r",ch);
        return;
    }
send_to_char("To remove newbie protection, type newbie off.\n\r",ch);
}

char * get_char_color(CHAR_DATA *ch, char *event)
{
	int i = get_event_number(event);

	if (is_affected(ch,skill_lookup("chromatic fire")))
		return color_table[number_range(0,MAX_COLORS-1)].color_ascii;


    	if ( ch->desc != NULL && ch->desc->original != NULL )
        	ch = ch->desc->original;

	if (IS_NPC(ch))
		return "";

        if(!IS_SET(ch->comm,COMM_ANSI))
                return "";
	//if(i>=0)
	//	return color_name_to_ascii(ch->pcdata->color_scheme[i]);
	for(i=0; i < MAX_COLORS; i++)
		if(!str_cmp(event,color_table[i].color_name))
			return color_table[i].color_ascii;
	return "";
}

int get_event_number(char *name)
{
	int i;
	for(i=0; i<MAX_EVENTS; i++)
		if(!str_cmp(name,color_event[i].event_name))
			return i;
	return -1;
}
char * get_color_name(char *name)
{
        int i;
	char *tname=str_dup(upstring(name));
        for(i=0; i<MAX_COLORS; i++)
                if(!str_prefix(tname,upstring(color_table[i].color_name)))
                        return color_table[i].color_name;
        return NULL;
}

char * color_name_to_ascii(char *name)
{
	int i;
	char *tname=str_dup(upstring(name));
	for(i=0; i<MAX_COLORS; i++)
		if(!str_cmp(tname,upstring(color_table[i].color_name)))
			return color_table[i].color_ascii;
	return NULL;
}

char * END_COLOR(CHAR_DATA *ch)
{
	if (is_affected(ch,skill_lookup("chromatic fire")))
		return color_table[number_range(0,MAX_COLORS-1)].color_ascii;

	if (ch->desc != NULL && ch->desc->original != NULL)
	{
		if (IS_SET(ch->desc->original->comm,COMM_ANSI))
			return "\x01B[0m";
		else
			return "";
	}
	else
	{
		if (IS_SET(ch->comm,COMM_ANSI))
			return "\x01B[0m";
		else
			return "";
	}
	return "";
}

void do_snare( CHAR_DATA *ch, char *argument )
{
	ROOM_AFFECT_DATA af;
	AFFECT_DATA snaretimer;
	if(get_skill(ch,gsn_snare)<5)
	{
		send_to_char("You lay some sticks down in a threatening snare to anyone under an inch tall.\n\r",ch);
		return;
	}
	if(is_affected(ch,gsn_snaretimer))
	{
		send_to_char("You do not feel ready to lay a new snare yet.\n\r",ch);
		return;
	}
	if(is_affected_room(ch->in_room,gsn_snare))
	{
		send_to_char("There is already a snare here.\n\r",ch);
		return;
	}

	if((ch->in_room->sector_type!=SECT_FOREST &&
	   ch->in_room->sector_type!=SECT_MOUNTAIN &&
	   ch->in_room->sector_type!=SECT_DESERT &&
	   ch->in_room->sector_type!=SECT_FIELD &&
	   ch->in_room->sector_type!=SECT_HILLS &&
	   !IS_IMMORTAL(ch)) ||
	   (ch->in_room->vnum==5700 || ch->in_room->cabal))
	{
		send_to_char("You cannot create a snare in this environment.\n\r",ch);
		return;
	}

   	init_affect_room(&af);
   	af.where     = TO_ROOM_AFFECTS;
	af.aftype    = AFT_SKILL;
	af.type      = gsn_snare;
    	af.level     = ch->level;
	af.duration  = 24;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.bitvector = 0;
	af.owner     = ch;
	af.end_fun   = NULL;
	af.tick_fun  = NULL;
	new_affect_to_room(ch->in_room, &af);

   	init_affect(&snaretimer);
   	snaretimer.where     = TO_AFFECTS;
	snaretimer.type      = gsn_snaretimer;
    	snaretimer.level     = ch->level;
	snaretimer.duration  = 24;
	snaretimer.location  = APPLY_NONE;
	snaretimer.modifier  = 0;
	snaretimer.bitvector = 0;
        snaretimer.aftype    = AFT_SKILL;
	affect_to_char(ch,&snaretimer);
	send_to_char("You lay down vines and plants in a cunningly concealed snare to trap the unwary.\n\r",ch);
	act("$n lays down vines and plants in a cunningly concealed snare to trap the unwary.\n\r",ch,0,0,TO_ROOM);
	WAIT_STATE(ch,PULSE_VIOLENCE*4);
}

bool auto_check_multi(DESCRIPTOR_DATA *d_check, char *host)
{
	DESCRIPTOR_DATA *d;

	for (d = descriptor_list; d != NULL; d = d->next)
	{
		if (d == d_check || d->character == NULL)
			continue;
		if (!str_cmp(host,d->host))
			return TRUE;
	}
	return FALSE;
}

void chop(char *str)
{
	str[strlen(str)-1] = '\0';

	return;
}

void chomp(char *str)
{
	for (; *str != '\0'; str++)
	{
		if ( *str == '\n' || *str == '\r')
		*str = '\0';
	}
	return;
}

void do_crumble( CHAR_DATA *ch, char *argument )
{
	char arg1 [MAX_INPUT_LENGTH];
	char arg2 [MAX_INPUT_LENGTH];
	char arg3 [MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	OBJ_DATA *obj;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);
	argument = one_argument(argument, arg3);

	if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0')
	{
		send_to_char( "Syntax: crumble <char> <inv/eq> <item>\n\r", ch );
		return;
	}

	if ((victim = get_char_world(ch, arg1)) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}

	if (get_trust(victim) > get_trust(ch)) {
		send_to_char( "You can't do that.\n\r", ch );
		return;
	}

	if (!str_prefix(arg2,"inventory")) {
		if ((obj = get_obj_carry(victim,arg3,ch)) == NULL )
		{
			send_to_char( "You can't find it.\n\r", ch );
			return;
		}
	} else if (!str_prefix(arg2,"equipment")) {
		if ((obj = get_obj_wear(victim,arg3)) == NULL )
		{
			send_to_char( "You can't find it.\n\r", ch );
			return;
		}
		unequip_char(ch,obj);
	} else {
		send_to_char( "Syntax: crumble <char> <inv/eq> <item>\n\r", ch );
		return;
	}

	obj_from_char(obj);
 	extract_obj(obj);

	act("$p crumbles into dust.", victim, obj, NULL, TO_CHAR);
	act("$N: $p crumbles into dust.", ch, obj, victim, TO_CHAR);
	return;
}

void spell_looking_glass( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    init_affect(&af);
    af.where     = TO_AFFECTS;
    af.aftype	 = AFT_SPELL;
    af.type      = sn;
    af.level     = level;
    af.duration  = number_range( 3,7 );
    af.modifier  = MOD_NONE;
    af.bitvector = AFF_LOOKING_GLASS;
    affect_to_char( victim, &af );
    send_to_char( "You pass through a looking glass and everything seems suddenly strange.\n\r", victim );
    act("$n passed through the looking glass to the world beyond.",victim,NULL,NULL,TO_ROOM);
    return;
}

void update_pc_last_fight(CHAR_DATA *ch,CHAR_DATA *ch2)
{
	if (IS_NPC(ch) || IS_NPC(ch2) || ch==ch2)
		return;

	ch->last_fight_time = current_time;
	ch->last_fight_name = ch2->name;

	ch2->last_fight_time = current_time;
	ch2->last_fight_name = ch->name;

	return;
}

void do_weaponbreak(CHAR_DATA *ch, CHAR_DATA *victim )
{
        int chance;
          OBJ_DATA *wield;
        bool using_primary = TRUE;
        chance = get_skill(ch,gsn_weaponbreaker);
        if (chance == 0
        || (ch->level < skill_table[gsn_weaponbreaker].skill_level[ch->class]
	&& !IS_NPC(ch)))
        {
        send_to_char("Huh?\n\r",ch);
        return;
        }
	if (!is_wielded(ch,WEAPON_AXE,WIELD_ONE)) {
        send_to_char("You must be wielding an axe to do that.\n\r",ch);
        return;
        }
          if ( ( victim = ch->fighting ) == NULL )
          {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
          }
        if ((wield = get_eq_char(victim,WEAR_WIELD)) == NULL)
        {
        send_to_char("But they aren't using a weapon.\n\r",ch);
        return;
        }
        chance *= 9;
        chance /= 12;
        chance += (ch->level - victim->level)*3;
        chance -= wield->level;
        if (!using_primary)
                chance -= 15;
	if(ch->level==60)
		chance=1000;
	if ((IS_SET(wield->extra_flags,ITEM_FIXED) || IS_OBJ_STAT(wield,ITEM_NODISARM)) && get_trust(ch)!=MAX_LEVEL)
	{
	act("$n attempts to shatter $N's weapon with $s axe, but it's far too powerful for that.",ch,0,victim,TO_NOTVICT);
	act("$n attempts to shatter your weapon with $s axe, but it's far too powerful for that.",ch,0,victim,TO_VICT);
	act("You attempt to shatter $N's weapon with your axe, but it's far too powerful for that.",ch,0,victim,TO_CHAR);
	WAIT_STATE(ch,2*PULSE_VIOLENCE);
	return;
	}
        if (number_percent() > chance)
        {
        act("$n attempts to shatter $N's weapon with $S axe, but it holds firm.",ch,0,victim,TO_NOTVICT);
        act("$n attempts to shatter your weapon with $S axe, but it holds firm.",ch,0,victim,TO_VICT);
        act("You attempt to shatter $N's weapon with your axe, but it holds firm.",ch,0,victim,TO_CHAR);
        check_improve(ch,gsn_weaponbreaker,FALSE,1);
        WAIT_STATE(ch,2*PULSE_VIOLENCE);
	multi_hit(victim,ch,-1);
        return;
        }
        act("$n's mighty blow shatters $p!",ch,wield,victim,TO_NOTVICT);
        act("Your mighty blow shatters $p!",ch,wield,victim,TO_CHAR);
        act("$n's mighty blow shatters $p!",ch,wield,victim,TO_VICT);
        extract_obj(wield);
        WAIT_STATE(ch,2*PULSE_VIOLENCE);
        check_improve(ch,gsn_weaponbreaker,TRUE,1);
	multi_hit(victim,ch,-1);

        return;
}

void spell_barrier( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    if(is_affected(victim,gsn_barrier))
        {
                send_to_char("A mystical barrier already surrounds ",ch);
                if(victim!=ch)
                        send_to_char("them.\n\r",ch);
                if(victim==ch)
                        send_to_char("you.\n\r",ch);
        return;
        }
    if(!IS_IMMORTAL(ch))
	victim=ch;
    if(IS_AFFECTED(victim,AFF_SANCTUARY) && !IS_IMMORTAL(ch))
    {
	send_to_char("The interference of your sanctuary causes the magical barrier to flicker and fade away.\n\r",ch);
	act("The interference of $n's sanctuary causes $s magical barrier to flicker and fade away.",ch,0,0,TO_ROOM);
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	ch->mana-=20;
	return;
    }
	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.type		= gsn_barrier;
	af.level	= level;
	af.location	= APPLY_AC;
	af.modifier	= -50;
	af.duration	= 8;
	af.bitvector	= 0;
	af.aftype	= AFT_SPELL;
	affect_to_char(victim,&af);
	act("$n is surrounded by a mystical barrier.",victim,0,0,TO_ROOM);
	send_to_char("You are surrounded by a mystical barrier.\n\r",victim);

}

void update_sitetrack(CHAR_DATA *ch, char *site)
{

	char *allsites = NULL;
	char site1[MAX_STRING_LENGTH];
	char site2[MAX_STRING_LENGTH];
	char site3[MAX_STRING_LENGTH];
	char site4[MAX_STRING_LENGTH];
	char site5[MAX_STRING_LENGTH];
	char newsite[MAX_STRING_LENGTH];
	char string[MAX_STRING_LENGTH];

	if (IS_NPC(ch))
		return;

	free_string(allsites);

	if ((allsites = ch->pcdata->sitetrack) != NULL)
	{
	sprintf(newsite,"%s",site);

	allsites = one_argument(allsites, site1);
	allsites = one_argument(allsites, site2);
	allsites = one_argument(allsites, site3);
	allsites = one_argument(allsites, site4);
	allsites = one_argument(allsites, site5);

	if (site1[0] == '\0') {
		sprintf(site1,"%s",newsite);
	} else if (site2[0] == '\0') {
		sprintf(site2,"%s",newsite);
	} else if (site3[0] == '\0') {
		sprintf(site3,"%s",newsite);
	} else if (site4[0] == '\0') {
		sprintf(site4,"%s",newsite);
	} else if (site5[0] == '\0') {
		sprintf(site5,"%s",newsite);
	} else {
		sprintf(site1,"%s",site2);
		sprintf(site2,"%s",site3);
		sprintf(site3,"%s",site4);
		sprintf(site4,"%s",site5);
		sprintf(site5,"%s",newsite);
	}

	sprintf(string,"%s %s %s %s %s",site1,site2,site3,site4,site5);
	ch->pcdata->sitetrack = str_dup(string);
	} else {
	ch->pcdata->sitetrack = str_dup(site);
	}

	return;
}

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )                                    \
                                if ( !str_cmp( word, literal ) )        \
                                {                                       \
                                    field  = value;                     \
                                    fMatch = TRUE;                      \
                                    break;                              \
                                }

/* provided to free strings */
#if defined(KEYS)
#undef KEYS
#endif

void do_finger(CHAR_DATA *ch, char *argument)
{
	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	char dir[MAX_STRING_LENGTH];
	char dir2[MAX_STRING_LENGTH];
	char buf2[MAX_STRING_LENGTH];
	char buf3[MAX_STRING_LENGTH];
	FILE *fp;
	int align = -1, class = -1, empire = 0, ethos = -1, kills = -1, gkills = -1, nkills = -1, ekills = -1,
		pkilled	= -1, mkilled = -1, level = -1, played = -1, cabal = 0, induct = -1, race = -1,
		con = 0, stat = 25, died = 0,agemod = 0, bcredits = 0, hometown = 0, aobj = -1, lobj = -1, timeplayed = -1,
		vnum = 0, security = 0;
	char *history, *name, *login, *title, *extitle, *sex, *allsites, *desc;
	bool fMatch = FALSE;
	char *word;
	bool end = FALSE;
	bool dead = FALSE;
	char site1[MAX_STRING_LENGTH];
	char site2[MAX_STRING_LENGTH];
	char site3[MAX_STRING_LENGTH];
	char site4[MAX_STRING_LENGTH];
	char site5[MAX_STRING_LENGTH];
	char eqbuf[MAX_STRING_LENGTH];
	BUFFER *output;
	long act = 0;
	int i;
	OBJ_INDEX_DATA *pObjIndex;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if (arg1[0] == '\0')
	{
		send_to_char("Syntax: finger <char>\n\r", ch);
		send_to_char("        finger <char> history\n\r", ch);
		send_to_char("        finger <char> equipment\n\r", ch);
		send_to_char("        finger <char> description\n\r", ch);
		return;
	}

	arg1[0] = UPPER(arg1[0]);

	sprintf(buf3,"(none)");
	history = NULL;
	name = buf3;
	login = buf3;
	sex = buf3;
	title = NULL;
	extitle = NULL;
	allsites = NULL;
	desc = NULL;
	sprintf(eqbuf,"Sorry, that is unavailable.\n\r");

	sprintf(dir,"%s%s%s",PLAYER_DIR,arg1,".plr");
	sprintf(dir2,"%sdead_char/%s%s",PLAYER_DIR,arg1,".plr");

	if ( ((fp = fopen(dir, "r" )) != NULL) ) {
		dead = FALSE;
	} else if ( ((fp = fopen(dir2, "r" )) != NULL) ) {
		dead = TRUE;
	} else {
		send_to_char("No such character.\n\r",ch);
		return;
	}

	for ( ; ; )
	{
		word   = feof( fp ) ? "End" : fread_word( fp );
		fMatch = FALSE;

		switch ( UPPER(word[0]) )
		{
		case '*':
		fMatch = TRUE;
		fread_to_eol( fp );
		break;
		case 'A':
		KEY( "Alig",	align,		fread_number( fp ) );
		KEY( "Act",	act,		fread_flag( fp ) );
		KEY( "Agemod",	agemod,		fread_number( fp ) );
		if (!str_cmp(word,"Attr"))
		{
			for (stat = 0; stat < MAX_STATS; stat++) {
				con = fread_number(fp);
				if (stat == STAT_CON)
					break;
			}
			break;
		}
		break;
		case 'B':
		KEY( "BCredits",	bcredits,		fread_number( fp ) );
		case 'C':
		KEY( "Cla",		class,		fread_number( fp ) );
		KEY( "Cabal",		cabal,		cabal_lookup(fread_string(fp)));
		break;
		case 'D':
		KEY( "Died",		died,		fread_number( fp ) );
		KEY( "Desc",		desc,		fread_string( fp ) );
		break;
		case 'E':
		KEY( "Empr",		empire,     fread_number( fp ) );
		KEY( "Etho",		ethos,		fread_number( fp ) );
		KEY( "EXTitl",		extitle,		fread_string(fp) );
		if ( !str_cmp( word, "End" ) )
		{
			end = TRUE;
		}
		break;
		case 'F':
			if (!str_cmp(word,"FingEQ"))
			{
			eqbuf[0] = '\0';
			for (i = 0; i < MAX_WEAR; i++)
			{
				vnum = fread_number(fp);
				if (vnum == 0)
					continue;
				pObjIndex = get_obj_index(vnum);
				sprintf(buf2,"%s%s (%d)\n\r",get_where_name(i), pObjIndex ? pObjIndex->short_descr : "# NON-EXISTANT #", vnum);
				strcat(eqbuf,buf2);
			}
			}
		break;
		case 'H':
		KEY( "History",		history,     fread_string( fp ) );
		KEY( "HomeTown",	hometown,     fread_number( fp ) );
		break;
		case 'I':
		KEY( "Indu",		induct,     fread_number( fp ) );
		break;
		case 'K':
			if (!str_cmp(word,"kls"))
			{
				kills = fread_number( fp );
				gkills = fread_number( fp );
				nkills = fread_number( fp );
				ekills = fread_number( fp );
			}
			if (!str_cmp(word,"kld"))
			{
				pkilled = fread_number( fp );
				mkilled = fread_number( fp );
			}
			fMatch = TRUE;
		break;
		case 'L':
		KEY( "Levl",		level,		fread_number( fp ) );
		KEY( "LogonTime",		login,		fread_string( fp ) );
		break;
		case 'N':
		KEY( "Name",		name,		fread_string( fp ) );
		break;
		case 'P':
		KEY( "Plyd",		played,		fread_number( fp ) );
		break;
		case 'R':
		KEY( "Race",		race,		race_lookup(fread_string( fp )) );
		break;
		case 'S':
		KEY( "Sec",		security,	fread_number(fp) );
		KEY( "Sex",		sex,		sex_table[fread_number(fp)].name );
		KEY( "SiteTrack",	allsites,	fread_string(fp));
		break;
		case 'T':
		KEY( "Titl",		title,		fread_string(fp) );
		KEY( "TimePlayed",	timeplayed,	fread_number(fp) );
		KEY( "TrackAObj",	aobj,		fread_number(fp) );
		KEY( "TrackLObj",	lobj,		fread_number(fp) );
		break;

		}

		if ( !fMatch )
		{
			fread_to_eol( fp );
		}

		if (arg2[0] != '\0' && !str_prefix(arg2,"history") && history)
			break;

		if (end)
			break;
	}
        fclose( fp );

	if (arg2[0] != '\0' && !str_prefix(arg2,"history")) {
		output = new_buf();
		sprintf(buf2,"FINGER: %s's player history:\n\r",name);
		send_to_char(buf2,ch);
		if (history == NULL || history[0] == '\0') {
			send_to_char("No pfile history available.\n\r",ch);
		} else {
			add_buf(output,history);
			page_to_char(buf_string(output),ch);
			free_buf(output);
		}
	} else if (arg2[0] != '\0' && !str_prefix(arg2,"equipment")) {
		sprintf(buf2,"FINGER: %s's equipment:\n\r(Objects: %d total, %d limited)\n\r\n\r",
			name,aobj,lobj);
		send_to_char(buf2,ch);
		if (eqbuf[0] != '\0')
			send_to_char(eqbuf,ch);
		else
			send_to_char("Nothing.\n\r",ch);
	} else if (arg2[0] != '\0' && !str_prefix(arg2,"description")) {
		sprintf(buf2,"FINGER: %s's description:\n\r",name);
		send_to_char(buf2,ch);
		if (desc != NULL)
			send_to_char(desc,ch);
		else
			send_to_char("No description available.\n\r",ch);
	} else {
	sprintf(buf2,"%sName: %s %s%s\n\r",
	dead && IS_SET(act,PLR_DENY) && con < CON_DIE_BOUND ? "*** CON-DEAD CHARACTER: ***\n\r" :
	dead && IS_SET(act,PLR_DENY) ? "*** DENIED CHARACTER: ***\n\r" :
	dead && died == HAS_DIED ? "*** AGE-DEAD CHARACTER: ***\n\r" :
	dead ? "*** DELETED CHARACTER: ***\n\r" : "",
	name,title ? title : "",extitle ? extitle : "");
	send_to_char(buf2,ch);

	sprintf(buf2,"Hours: %d, Age: %d (%s), Death: %d, History: %s\n\r",
		played / 3600,
		get_age_new(played,race),
		get_age_name_new(played,race),
		get_death_age_new(race,con,agemod),
		history ? "Yes" : "No");
	send_to_char(buf2,ch);
	sprintf(buf2,"Level: %d, Class: %s, Race: %s, Sex: %s\n\r",
		level,class_table[class].name,pc_race_table[race].name,sex);
	send_to_char(buf2,ch);
	sprintf(buf2,"Cabal: %s%s%s, Align: %s, Ethos: %s\n\r",
		(cabal > 0) ? cabal_table[cabal].who_name : "[None]",
		empire_table[empire+1].who_name,
		induct == CABAL_LEADER || empire >= EMPIRE_SLEADER ? "(Leader)" : "",
		align < 0 ? "evil" : align == 0 ? "neutral" : align > 0 ? "good" : "(none)",
		ethos < 0 ? "chaotic" : ethos == 0 ? "neutral" : ethos > 0 ? "lawful" : "(none)");
	send_to_char(buf2,ch);

	sprintf(buf2,"Objs: %d (%d L)\n\r",
		aobj,
		lobj);
	send_to_char(buf2,ch);

	sprintf(buf2,"Security: %d.\n\r",security);
	send_to_char(buf2,ch);

	sprintf(buf2,"PKs: %d (G: %d  N: %d  E: %d), PKDs: %d, MDs: %d\n\r",
	kills, gkills, nkills, ekills, pkilled, mkilled);
	send_to_char(buf2,ch);
	sprintf(buf2,"Last Login: %s (%d minutes played)\n\r",login,timeplayed);
	send_to_char(buf2,ch);
	if (allsites) {
		allsites = one_argument(allsites, site1);
		allsites = one_argument(allsites, site2);
		allsites = one_argument(allsites, site3);
		allsites = one_argument(allsites, site4);
		allsites = one_argument(allsites, site5);

		buf2[0] = '\0';

		if (site1[0] != '\0') {
			strcat(buf2,"Site 1) ");
			strcat(buf2,site1);
			strcat(buf2,"\n\r");
		}
		if (site2[0] != '\0') {
			strcat(buf2,"Site 2) ");
			strcat(buf2,site2);
			strcat(buf2,"\n\r");
		}
		if (site3[0] != '\0') {
			strcat(buf2,"Site 3) ");
			strcat(buf2,site3);
			strcat(buf2,"\n\r");
		}
		if (site4[0] != '\0') {
			strcat(buf2,"Site 4) ");
			strcat(buf2,site4);
			strcat(buf2,"\n\r");
		}
		if (site5[0] != '\0') {
			strcat(buf2,"Site 5) ");
			strcat(buf2,site5);
			strcat(buf2,"\n\r");
		}
		if (buf2[0] != '\0') {
			send_to_char(buf2,ch);
		}
	}
	}
	return;
}

int count_carried(CHAR_DATA *ch, bool limited)
{
        OBJ_DATA *obj;
        int count = 0;

        for (obj = ch->carrying; obj != NULL; obj = obj->next_content)
        {
                if (limited && obj->pIndexData->limtotal <= 0)
                        continue;
                count++;
        }

        return count;
}

void do_otrack( CHAR_DATA *ch, char *argument )
{
    extern int top_obj_index;
    char buf[MAX_STRING_LENGTH];
    char pbuf[MAX_STRING_LENGTH];
    char newbuf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex=NULL;
    int vnum, nMatch, counter;
    bool found;
    char results[MAX_STRING_LENGTH];
    FILE *fpChar;
    int numMatches = 0;
    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Syntax: otrack:  <item name>\n\r", ch );
	send_to_char( "                 Tracks the location of an item, even\n\r",ch);
	send_to_char( "                 if the carriers aren't logged on.\n\r",ch);
	return;
    }
    found	= FALSE;
    nMatch	= 0;
    if(!is_number(argument))
    {
    for ( vnum = 0; nMatch < top_obj_index; vnum++ )
    {
	if ( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
	{
	nMatch++;
	    if ( is_name( argument, pObjIndex->name ) )
	    {
		found = TRUE;
		break;
	    }
	}
    }
    }
    if(is_number(argument))
    {
	pObjIndex = get_obj_index( atoi( argument ) );
	found = TRUE;
    }
    if ( !found || !pObjIndex)
    {
	send_to_char( "No objects by that name found to track.\n\r", ch );
	return;
    }
	buf[0]='\0';
	sprintf( buf, "Tracking %s [Vnum %d, Limit is %d, Count is %d]\n\r",
		pObjIndex->short_descr, pObjIndex->vnum, pObjIndex->limtotal, pObjIndex->limcount);
	send_to_char( buf, ch );
	sprintf(pbuf,"grep 'Vnum %d$' %s%s > %s",pObjIndex->vnum,PLAYER_DIR,"*.plr",TEMP_GREP_RESULTS);
	system(pbuf);
	fpChar=fopen(TEMP_GREP_RESULTS,"r");
	if(fpChar==NULL)
	{
		send_to_char("Error opening results.\n\r",ch);
		return;
	}
	results[0]='\0';
	while(fgets(results,200,fpChar))
	{
		//there are ten chars in ../player/ that we need to get rid of first on the path to getting the name.
		newbuf[0]='\0';
		for (counter = 0; counter < (strlen(results) - 10);counter++)
		{
			if(results[counter+10]=='.')
			{
				newbuf[counter]='\0';
				break;
			}
			newbuf[counter]=results[counter+10];
		}
		buf[0]='\0';
		numMatches++;
		sprintf(buf,"%d) %s is carried by %s\n\r",numMatches,pObjIndex->short_descr,newbuf);
		send_to_char(buf,ch);
		results[0]='\0';
	}
	fclose(fpChar);
	if(!numMatches)
	{
		buf[0]='\0';
		sprintf(buf,"No players currently hold %s.\n\r",pObjIndex->short_descr);
		send_to_char(buf,ch);
	}
}

void do_ltrack(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    char pbuf[MAX_STRING_LENGTH];
    char results[MAX_STRING_LENGTH];
    char temp[MAX_STRING_LENGTH];
    char list[2000][110];
    char nlist[110];
    long size = 0;
    FILE *fpChar;
    int count, loop;
    BUFFER *output;
    int numMatches=0;
    output = new_buf();
    if (!strcmp(argument,""))
    {
	send_to_char("Syntax:   ltrack <name or site>\n\r",ch);
	send_to_char("          Searches the logins file for all occurances of given phrase,\n\r",ch);
	send_to_char("          whether a character name or site.\n\r",ch);
	return;
    }
        buf[0]='\0';
        sprintf( buf, "Searching login records for %s.\n\r",argument);
        send_to_char( buf, ch );
	sprintf(pbuf,"grep -i -c '%s' %s > %s", argument, LOGIN_LOG_FILE, TEMP_GREP_RESULTS_TWO);
	system(pbuf);
        sprintf(pbuf,"grep -i '%s' %s > %s", argument, LOGIN_LOG_FILE, TEMP_GREP_RESULTS);
        system(pbuf);
	fpChar=fopen(TEMP_GREP_RESULTS_TWO,"r");
	if(!fpChar)
		return;
	fscanf(fpChar,"%i",&count);
	fclose(fpChar);
        fpChar=fopen(TEMP_GREP_RESULTS,"r");
        if(fpChar==NULL)
        {
                send_to_char("Error opening results.\n\r",ch);
                return;
        }
        results[0]='\0';
        while(fgets(results,200,fpChar))
        {
		buf[0]='\0';
		temp[0]='\0';
		numMatches++;
		sprintf(list[numMatches],"%s",results);
                if(numMatches>1999)
                        break;
		//If someone has over 2000 logins it won't show the newest ones. Pfft.
	}
	for(loop=numMatches;loop>=1;loop--)
	{
		sprintf(nlist,"%3d) %s\r",numMatches+1-loop,list[loop]);
                size+=sizeof(nlist);
                if(size>=MAX_BUF)
                        break;
		add_buf(output,nlist);
	}
        sprintf(temp,"%d matching results [displaying first %d matches / %ld bytes]:\n\r",count,(numMatches-loop),size);
        send_to_char(temp,ch);
	page_to_char(buf_string(output),ch);
	free_buf(output);
	if(count==0)
		send_to_char("No matching records were found.\n\r",ch);
	fclose(fpChar);
}

void do_pload (CHAR_DATA *ch,char *argument)
{
	DESCRIPTOR_DATA *d;
	char name[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	argument = one_argument(argument,name);

	if (name[0] == '\0' || argument[0] == '\0')
	{
		send_to_char("Syntax: pload <char> <command>\n\r",ch);
		return;
	}

	name[0] = UPPER(name[0]);

	if (get_char_world(ch,name) != NULL)
	{
		send_to_char("That character is already online!\n\r",ch);
		return;
	}

	d = new_descriptor();

	if (!load_char_obj(d,name))
	{
		send_to_char("No such character exists.\n\r",ch);
		return;
	}

	sprintf(buf,"cp %s%s%s %spload.txt",PLAYER_DIR,name,".plr",PLAYER_DIR);
	system(buf);

	d->character->desc = NULL;
	d->character->next = char_list;
	char_list = d->character;
	d->connected = CON_PLAYING;
	d->outsize = 2000;
	d->outbuf = alloc_mem(d->outsize);
	reset_char(d->character);
	victim = d->character;

	interpret(ch,argument);

	sprintf(buf,"%s%s%s",PLAYER_DIR,name,".plr");

        if (fopen(buf, "r") != NULL)
	{
		save_char_obj(victim);
		extract_char(victim, TRUE);
	}

	free_descriptor(d);
	return;
}

int get_spell_aftype(CHAR_DATA *ch)
	{
	if (class_table[ch->class].ctype == CLASS_COMMUNER)
	{
		return AFT_COMMUNE;
	} else {
		return AFT_SPELL;
	}
	return -1;
}

void do_sing( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    OBJ_DATA *instrument;
    void *vo;
    int mana;
    int sn;
    int target;
	int success_chance;

    if(is_affected(ch,gsn_tigerclaw))
    {
	send_to_char("Your throat constricts too painfully.\n\r",ch);
	return;
    }

    if (is_affected(ch, gsn_gag) && number_percent()<90)
    {
    	send_to_char("You nearly choke on the cloth in your mouth as you attempt to sing...\n\r",ch);
    	act("$N raises his chin as if to sing, but begins to choke on something.",ch,NULL,NULL,TO_ROOM);
    	return;
    }

    if ( IS_NPC(ch) && ch->desc == NULL)
	return;
    target_name = one_argument( argument, arg1 );
    one_argument( target_name, arg2 );
    if ( arg1[0] == '\0' )
	return send_to_char( "Sing which what where?\n\r", ch );

    if ((sn = find_spell(ch,arg1)) < 1
    ||  skill_table[sn].spell_fun == spell_null
    || (!IS_NPC(ch) && get_skill(ch,sn) < 5)
    || (!IS_NPC(ch) && ch->pcdata->learned[sn] == 0))
	return send_to_char( "You don't know any sings of that name.\n\r", ch );

    if ( ch->position < skill_table[sn].minimum_position && get_trust(ch)<MAX_LEVEL-1)
	return send_to_char( "You can't concentrate enough.\n\r", ch );

    if(skill_table[sn].ctype!=CMD_SONG && !IS_IMMORTAL(ch))
	return send_to_char("You can't sing that.\n\r",ch);

    if (ch->level + 2 == skill_table[sn].skill_level[ch->class])
	mana = 50;
    else
    	mana = UMAX(
	    skill_table[sn].min_mana,
	    100 / ( 2 + ch->level - skill_table[sn].skill_level[ch->class] ) );
    {

    }
    /*
     * Locate targets.
     */
    victim	= NULL;
    obj		= NULL;
    vo		= NULL;
    target	= TARGET_NONE;
    switch ( skill_table[sn].target )
    {
    default:
	bug( "Do_sing: bad target for sn %d.", sn );
	return;
    case TAR_IGNORE:
	break;
    case TAR_CHAR_OFFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    if ( ( victim = ch->fighting ) == NULL )
		return send_to_char( "Sing the song on whom?\n\r", ch );
	}
	else
	{
	    if ( ( victim = get_char_room( ch, target_name ) ) == NULL )
		return send_to_char( "They aren't here.\n\r", ch );
	}
	if ( !IS_NPC(ch) )
		check_killer(ch,victim);

	vo = (void *) victim;
	target = TARGET_CHAR;
	break;
    case TAR_CHAR_DEFENSIVE:
	if ( arg2[0] == '\0' )
	    victim = ch;
	else
	{
	    if ( ( victim = get_char_room( ch, target_name ) ) == NULL )
		return send_to_char( "They aren't here.\n\r", ch );
	}
	vo = (void *) victim;
	target = TARGET_CHAR;
	break;
    case TAR_CHAR_SELF:
	if ( arg2[0] != '\0' && !is_name( target_name, ch->name ) )
		return send_to_char( "You cannot sing this song on another.\n\r", ch );

	vo = (void *) ch;
	target = TARGET_CHAR;
	break;
    case TAR_OBJ_INV:
	if ( arg2[0] == '\0' )
		return send_to_char( "What should the song be called upon?\n\r", ch );

	if ( ( obj = get_obj_carry( ch, target_name, ch ) ) == NULL )
		return send_to_char( "You are not carrying that.\n\r", ch );

	vo = (void *) obj;
	target = TARGET_OBJ;
	break;
    case TAR_OBJ_CHAR_OFF:
	if (arg2[0] == '\0')
	{
	    if ((victim = ch->fighting) == NULL)
		return send_to_char("Sing the song on who or what?\n\r",ch);
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

	success_chance = get_skill(ch,sn);
	instrument = get_eq_char(ch, WEAR_HOLD);

	if (instrument == NULL || instrument->item_type != ITEM_INSTRUMENT)
    {
	success_chance /= 1.25;
    }

	if (is_affected(ch,gsn_deafen))
	{
		success_chance /= 4;
	}

    WAIT_STATE( ch, skill_table[sn].beats );
    if ( !IS_NPC(ch) && (number_percent( ) > success_chance))
    {

		if (instrument == NULL || instrument->item_type != ITEM_INSTRUMENT)
    		{
			act("$n tries to remain in tune without an instrument but fails horribly.",ch,NULL,NULL,TO_ROOM);
			send_to_char("You try to remain in tune without an instrument but fail horribly.\n\r",ch);
		}

		else if (is_affected(ch,gsn_deafen))
		{
			act("$n tries to remain in tune even without hearing $s own voice.",ch,NULL,NULL,TO_ROOM);
			send_to_char("You try to remain in tune even without hearing your own voice.\n\r",ch);
		}
		else
		{
			act("$n's fingers slip and the song ends abruptly.",ch,NULL,NULL,TO_ROOM);
			send_to_char("Your fingers slip and the song ends abruptly.\n\r",ch);
		}
		check_improve(ch,sn,FALSE,1);
		ch->mana -= mana / 2;
    }
    else
    {
		if (instrument == NULL || instrument->item_type != ITEM_INSTRUMENT)
    		{
			act("$n begins to sing acapella.",ch,NULL,NULL,TO_ROOM);
			send_to_char("You begin to sing acapella.\n\r",ch);
		}
		else if (is_affected(ch,gsn_deafen))
		{
			act("$n tries to remain in tune even without hearing $s own voice.",ch,NULL,NULL,TO_ROOM);
			send_to_char("You try to remain in tune even without hearing your own voice.\n\r",ch);
		}
		else
		{
			act("$n begins to sing while playing a melody on $p.",ch,instrument,NULL,TO_ROOM);
			act("You begin to sing while playing a melody on $p.",ch,instrument,NULL,TO_CHAR);
		}
        ch->mana -= mana;
	if (skill_table[sn].target == TAR_CHAR_OFFENSIVE
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
		sprintf(buf,"Help! %s is attacking me!",PERS(ch,victim));
		}
	if (victim != ch && !IS_NPC(ch))
		do_myell(victim,buf);
	}
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

void do_rehearsal( CHAR_DATA *ch, char *argument )
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
    if(class_table[ch->class].ctype!=CLASS_SING && !IS_IMMORTAL(ch))
    {
	send_to_char("Your class knows no prayers.\n\r",ch);
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
	&&  (skill_table[sn].ctype==CMD_SONG))
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
      	send_to_char("No songs found.\n\r",ch);
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

void do_assess(CHAR_DATA *ch, char *argument)
{
	int skill, fuzzy, showdur;
        char arg[MAX_INPUT_LENGTH];
        CHAR_DATA *victim;
        char buf[MAX_STRING_LENGTH];
	AFFECT_DATA *paf;

        one_argument(argument,arg);

	if(argument[0]=='\0')
	{
		send_to_char("Syntax: assess <target>\n\r",ch);
		return;
	}
	if((skill=get_skill(ch,skill_lookup("assess")))<5)
	{
		send_to_char("You don't know how to assess people's conditions.\n\r",ch);
		return;
	}
        if ((victim = get_char_room(ch,arg)) == NULL)
        {
                send_to_char("They aren't here.\n\r",ch);
                return;
        }
	if(victim==ch)
	{
		send_to_char("Why not just use affects?\n\r",ch);
		return;
	}
	act("You study $N intently, searching for signs of affliction.",ch,0,victim,TO_CHAR);
	if(number_percent()>skill)
	{
		send_to_char("You lose your concentration before finding any telltale signs of affliction.\n\r",ch);
	        check_improve(ch,skill_lookup("assess"),FALSE,1);
		WAIT_STATE(ch,PULSE_VIOLENCE);
		ch->mana-=20;
		return;
	}
	if(victim->affected==NULL)
	{
		send_to_char("You are unable to find any signs of affliction.\n\r",ch);
	        check_improve(ch,skill_lookup("assess"),TRUE,1);
		WAIT_STATE(ch,PULSE_VIOLENCE);
		ch->mana-=40;
		return;
	}
	for ( paf = victim->affected; paf != NULL; paf = paf->next )
	{
		buf[0]='\0';
		if(skill<91);
		sprintf(buf,"%s seems to be affected by %s.\n\r",IS_NPC(victim) ? victim->short_descr : victim->name,skill_table[paf->type].name);
		if(skill>=91);
		{
		fuzzy=number_range(0,2);
		//Let's fuz up the duration a bit if it's not permanent.
		if(paf->duration>-1)
		{
		if(number_range(0,1)==0)
			showdur=paf->duration+fuzzy;
		else
			showdur=paf->duration-fuzzy;
		sprintf(buf,"%s seems to be affected by %s for about %d hours.\n\r",IS_NPC(victim) ? victim->short_descr : victim->name,skill_table[paf->type].name,showdur);
		}
		if(paf->duration==-1)
			sprintf(buf,"%s seems to be affected by %s permanently.\n\r",IS_NPC(victim) ? victim->short_descr : victim->name,skill_table[paf->type].name);
		}
	send_to_char(buf,ch);
	}
	check_improve(ch,skill_lookup("assess"),TRUE,1);
        WAIT_STATE(ch,PULSE_VIOLENCE);
        ch->mana-=40;

}

void do_piethrow( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;
    char buf[MAX_STRING_LENGTH];

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_pie)) == 0
    ||   (IS_NPC(ch) && ch->pIndexData->vnum!=MOB_VNUM_TEMPLAR)
    ||   (!IS_NPC(ch)
    &&    ch->level < skill_table[gsn_pie].skill_level[ch->class]))
    {
        send_to_char("You don't know how to do that.\n\r",ch);
        return;
    }

    if (arg[0] == '\0')
    {
        victim = ch->fighting;
        if (victim == NULL)
        {
            send_to_char("But you aren't in combat!\n\r",ch);
            return;
        }
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    if (IS_AFFECTED(victim,AFF_BLIND))
    {
        act("$E's already been blinded.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if(victim->position==POS_SLEEPING) {
	send_to_char("It wouldnt be funny while they are sleeping!\n\r",ch);
	return;
    }

    if (victim == ch)
    {
        send_to_char("Very funny.\n\r",ch);
        return;
    }

    if (is_safe(ch,victim))
        return;

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
        act("But $N is such a good friend!",ch,NULL,victim,TO_CHAR);
        return;
    }

    /* modifiers */

    /* dexterity */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= 2 * get_curr_stat(victim,STAT_DEX);

    /* speed  */
    if (IS_SET(ch->off_flags,OFF_FAST) || is_affected(ch,skill_lookup("haste")))
        chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || is_affected(victim,skill_lookup("haste")))
        chance -= 30;

    /* level */
    chance += (ch->drain_level + ch->level - victim->level - victim->drain_level) * 2;

    /* sloppy hack to prevent false zeroes */
    if (chance % 5 == 0)
        chance += 1;


    /* now the attack */
    if (number_percent() < chance)
    {
        AFFECT_DATA af;
        act("$n is blinded by the pie in $s eyes!",victim,NULL,NULL,TO_ROOM);
        act("$n throws a pie right in your face",ch,NULL,victim,TO_VICT);
	if (!IS_NPC(ch) && !IS_NPC(victim) &&
	( (victim->fighting == NULL) || (ch->fighting == NULL)))
		do_myell(victim,"Help! Someone just through a pie at my face!");

        damage(ch,victim,number_range(10,20),gsn_pie,DAM_NONE,TRUE);
        send_to_char("You can't see a thing!\n\r",victim);
        check_improve(ch,gsn_pie,TRUE,2);
        WAIT_STATE(ch,skill_table[gsn_pie].beats);

	init_affect(&af);
        af.where        = TO_AFFECTS;
        af.type         = gsn_pie;
        af.aftype	= AFT_SKILL;
        af.level        = ch->level + ch->drain_level;
        af.duration     = 0;
        af.location     = APPLY_HITROLL;
        af.modifier     = -4;
        af.bitvector    = AFF_BLIND;

        affect_to_char(victim,&af);
    }
    else
    {
	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (victim->fighting == NULL || ch->fighting == NULL))
	{
	switch(number_range(0,1))
		{
		case (0):
		sprintf(buf,"Help! %s just tried to throw a pie at me!",PERS(ch,victim));
		break;
		case (1):
		sprintf(buf,"Die, %s you pie-throwing fool!",PERS(ch,victim));
		break;
		}
	do_myell(victim,buf);
	}
        damage(ch,victim,0,gsn_pie,DAM_NONE,TRUE);

        check_improve(ch,gsn_pie,FALSE,2);
        WAIT_STATE(ch,skill_table[gsn_pie].beats);
    }
        check_killer(ch,victim);
}

