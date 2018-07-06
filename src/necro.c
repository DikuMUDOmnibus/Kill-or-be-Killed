#include "include.h"

char *target_name;

void spell_animate_dead( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *zombie;
    OBJ_DATA *corpse;
    OBJ_DATA *obj_next;
    OBJ_DATA *obj;
    CHAR_DATA *search;
    AFFECT_DATA af;
    char *name;
    char *last_name;
    char *obj_name;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int chance;
    int z_level;
    int control;

    if (level == 72)	/* necromancy scroll */
	target_name = "corpse";

    if ((is_affected(ch,sn)
    || is_affected(ch,skill_lookup("mummify")))
	&& level < 70)
    {
	send_to_char("You have not yet regained your powers over the dead.\n\r",ch);
	return;
    }

    control = 0;

    for (search = char_list; search != NULL; search = search->next)
    {
	if (IS_NPC(search) && (search->master == ch)
	&& search->pIndexData->vnum == MOB_VNUM_ZOMBIE)
        control += 6;
    }
    control += 4;

    if ((ch->level < 30 && control > 12) || (ch->level < 35 && control > 18)
    || (ch->level < 40 && control > 24) || (ch->level < 52 && control > 30))
    //|| control >=36 && !(IS_IMMORTAL(ch)))
    {
    send_to_char("You already control as many undead as you can.\n\r",ch);
	return;
    }

    if (target_name[0] == '\0')
    {
	send_to_char("Animate which corpse?\n\r",ch);
	return;
    }
    obj_name = str_dup(target_name);

    corpse = get_obj_here(ch,obj_name);

    if (corpse == NULL)
    {
	send_to_char("You can't animate that.\n\r",ch);
  	return;
    }

    if (corpse->item_type != ITEM_CORPSE_NPC && corpse->contains!=NULL)
    {
	send_to_char("You can't animate a player corpse with gear inside!\n\r",ch);
	return;
    }

   if (IS_SET(corpse->extra_flags,CORPSE_NO_ANIMATE))
    {
	send_to_char("That corpse can not sustain further life beyond the grave.\n\r",ch);
	return;
    }

    name = corpse->short_descr;


    for (obj = corpse->contains; obj != NULL; obj = obj_next)
    {
	obj_next = obj->next_content;
	obj_from_obj(obj);
	obj_to_room(obj,ch->in_room);
    }

        init_affect(&af);
    af.where = TO_AFFECTS;
        af.aftype    = AFT_SPELL;
    af.type = sn;
    af.level = level;
    af.duration = 12;
    af.modifier = 0;
    af.location = 0;
    af.bitvector = 0;
    if (level < 70)
	    affect_to_char(ch,&af);

    chance = get_skill(ch,sn);

    if (ch->level < corpse->level)
    {
	chance += (3*ch->level);
	chance -= (3*ch->level);
    }

    chance = URANGE(20,chance,90);
    chance = chance + 10;
    if (number_percent() > chance)
    {
	act("You fail and destroy $p",ch,corpse,NULL,TO_CHAR);
	act("$n tries to animate a corpse but destroys it.",ch,NULL,NULL,TO_ROOM);
	extract_obj(corpse);
	return;
    }

    act("$n utters an incantation and a burning red glow flares into the eyes of $p.",ch,corpse,NULL,TO_ROOM);
    act("$p shudders and comes to life!",ch,corpse,NULL,TO_ROOM);
    act("You call upon the powers of the dark to give life to $p.",ch,corpse,NULL,TO_CHAR);


     zombie = create_mobile(get_mob_index(MOB_VNUM_ZOMBIE));
	    char_to_room(zombie,ch->in_room);

	    z_level = UMAX(1,corpse->level - number_range(3,6));
	    zombie->level = z_level;
	    zombie->max_hit = (dice(z_level, 20));
	    zombie->max_hit += (z_level * 30);
	    zombie->hit = zombie->max_hit;
	    zombie->damroll += (z_level*8/10);
	    zombie->alignment = -1000;
	    last_name = name;

	    last_name = one_argument(corpse->short_descr, name);
	    last_name = one_argument(last_name,name);
	    last_name = one_argument(last_name,name);
	    name = last_name;

	    extract_obj(corpse);

	    sprintf( buf1, "the zombie of %s", name);
	    sprintf( buf2, "A zombie of %s is standing here.\n\r", name);
	    free_string(zombie->short_descr);
	    free_string(zombie->long_descr);
	    zombie->short_descr = str_dup(buf1);
	    zombie->long_descr = str_dup(buf2);

	    add_follower(zombie,ch);
	    zombie->leader = ch;

	    af.duration = -1;
	    af.bitvector = AFF_CHARM;
	    affect_to_char(zombie,&af);


	    return;
}

void spell_hex(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA hex;
	int severity = 0;
	if(is_affected(victim,sn))
		return send_to_char("They are already hexed.\n\r",ch);
	act("$n grins maniacally and spits out an archaic word, unleashing a hex upon $N!",ch,0,victim,TO_NOTVICT);
	act("$n grins maniacally and spits out an archaic word, unleashing a hex upon you!",ch,0,victim,TO_VICT);
	act("You grin maniacally and spit out an archaic word, unleashing a hex upon $N!",ch,0,victim,TO_CHAR);
	if(saves_spell(level+4,victim,DAM_NEGATIVE) && number_percent()>35)
	{
		act("$n staggers for a moment, but resists the hex.",victim,0,0,TO_ROOM);
		act("You stagger as the power of the hex strikes you, but manage to resist it.",victim,0,0,TO_CHAR);
		return;
	}
	init_affect(&hex);
	hex.where  	= TO_AFFECTS;
	hex.type   	= sn;
	hex.level	= level;
	hex.owner	= ch;
	hex.location	= APPLY_SAVING_SPELL;
	hex.modifier	= 10;
	hex.duration	= level/4;
	hex.bitvector	= AFF_CURSE;
	severity++;
	if(number_percent()>75)
	{
		hex.bitvector += AFF_BLIND;
		severity++;
	}
	affect_to_char(victim, &hex);
	if(number_percent()>70)
	{
		//plague
   		hex.location  = APPLY_STR;
    		hex.modifier  = -5;
    		hex.bitvector = AFF_PLAGUE;
		affect_to_char(victim, &hex);
		severity++;
	}
	if(number_percent()>66)
	{
		//poison
		hex.location  = APPLY_STR;
		hex.modifier  = -6;
		hex.bitvector = AFF_POISON;
		affect_to_char(victim, &hex);
		severity++;
	}
	if(severity>1 && severity<4)
	{
		act("$n sags as the hex strikes him powerfully!",victim,0,0,TO_ROOM);
		act("You slump as the hex strikes you powerfully.",victim,0,0,TO_CHAR);
	}
	if(severity==4)
	{
		act("$n screams in utter agony as the full power of the hex strangles $s soul!",victim,0,0,TO_ROOM);
		act("You scream in utter agony as the full power of the hex strangles your soul!",victim,0,0,TO_CHAR);
	}
	act("You feel drained from the power of your hex.",ch,0,0,TO_CHAR);
	ch->hit *= (98 - (severity * 3.0)) / 100.1;
}

void spell_black_circle(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *pet;
	AFFECT_DATA af;
	bool found = FALSE;
	if(is_affected(ch,sn))
		return send_to_char("You cannot summon your minions again yet.\n\r",ch);
	act("$n draws a black circle on the ground and falls into deep concentration.",ch,0,0,TO_ROOM);
	act("You draw a black circle on the ground and fall into deep concentration.",ch,0,0,TO_CHAR);
	for(pet = char_list; pet!= NULL; pet=pet->next)
		if(IS_NPC(pet) && IS_AFFECTED(pet,AFF_CHARM) && pet->master &&
			pet->master == ch)
			{
				stop_fighting(pet,TRUE);
				act("$n disappears suddenly.",pet,0,0,TO_ROOM);
				char_from_room(pet);
				char_to_room(pet, ch->in_room);
				act("$n arrives suddenly, kneeling before $N outside the circle.",pet,0,ch,TO_ROOM);
				found = TRUE;
			}
	if(!found)
		return send_to_char("Your summonings went unanswered.\n\r",ch);
	init_affect(&af);
	af.where	= TO_AFFECTS;
        af.aftype    = AFT_SPELL;
	af.type		= sn;
	af.modifier	= 0;
	af.location	= 0;
	af.duration	= 60;
	af.owner	= NULL;
	af.level	= ch->level;
	af.bitvector	= 0;
	af.end_fun	= NULL;
	affect_to_char(ch,&af);
}
