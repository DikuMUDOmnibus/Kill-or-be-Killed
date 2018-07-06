#include "include.h"

bool check_dispel( int dis_level, CHAR_DATA *victim, int sn);

void do_invoke_jackass args((CHAR_DATA *ch));
void do_invoke_jerle args((CHAR_DATA *ch));
void do_invoke_vasilis args((CHAR_DATA *ch));
void do_invoke_merth args((CHAR_DATA *ch));
void do_invoke_deomanix args((CHAR_DATA *ch));
void do_invoke_orduno args((CHAR_DATA *ch));
void do_invoke_dhayton args((CHAR_DATA *ch));
void do_invoke_merthairu args((CHAR_DATA *ch));
void do_invoke_ceial args ((CHAR_DATA *ch));
void do_invoke_detlef args ((CHAR_DATA *ch));

DECLARE_DO_FUN(do_ghost);

void do_brands( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    OBJ_DATA *objj;

    if (ch->level < 53 || IS_NPC(ch))
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }
    argument = one_argument( argument, arg1 );

    if ( arg1[0] == '\0' )
    {
        send_to_char("Syntax: brands <char>\n\r", ch);
        return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
        send_to_char( "They aren't playing.\n\r", ch );
        return;
    }

    if (ch == victim)
    {
        send_to_char( "Trying to brand yourself?\n\r", ch );
        return;
    }
    if ( (obj = get_eq_char(victim, WEAR_BRAND) ) != NULL )
    {
        send_to_char("That person already has a brand.\n\r", ch);
        return;
    }

    if ( (obj = get_eq_char(ch, WEAR_BRAND) ) != NULL )
    {
	objj = create_object(obj->pIndexData, 0);
        clone_object(obj, objj);
        obj_to_char( objj, victim );
        equip_char( victim, objj, WEAR_BRAND );
        send_to_char("That person now has your brand.\n\r", ch);
        send_to_char("You wince in agony as the hot metal brands you!\n\r", victim);
        sprintf(buf,"AUTO: Branded by %s.\n\r", ch->name);
        if (!IS_IMMORTAL(victim))
            add_history(NULL,victim,buf);
        return;
    }
    send_to_char("You don't have a brand yourself.\n\r", ch);
    return;
}

void do_unbrands( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    if (ch->level < 53 || IS_NPC(ch))
    {
        send_to_char("Huh?\n\r", ch);
        return;
    }
    argument = one_argument( argument, arg1 );

    if ( arg1[0] == '\0' )
    {
        send_to_char("Syntax: unbrand <char>\n\r", ch);
        return;
    }
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
        send_to_char( "They aren't playing.\n\r", ch );
        return;
    }
    if (IS_NPC(victim))
    {
	send_to_char( "Trying to unbrand a mob?\n\r", ch );
	return;
    }
    if (ch == victim)
    {
        send_to_char( "Trying to unbrand yourself?\n\r", ch );
        return;
    }
    if ((obj = get_eq_char(victim, WEAR_BRAND)) == NULL)
    {
	send_to_char( "That person doesn't have a brand.\n\r", ch );
	return;
    }
    unequip_char(victim, obj);
    obj_from_char(obj);
    extract_obj(obj);
    send_to_char("You have unbranded that person.\n\r", ch);
    send_to_char("Your brand is painfully removed.\n\r", victim);
    sprintf(buf,"AUTO: Unbranded by %s.\n\r",ch->name);
    if (!IS_IMMORTAL(victim))
        add_history(NULL,victim,buf);
    return;
}

void do_invoke( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;

    obj = get_eq_char(ch, WEAR_BRAND);

    if ((obj = get_eq_char(ch, WEAR_BRAND)) == NULL)
    {
	send_to_char( "You have no tattoo to invoke.\n\r", ch );
	return;
    }
    if(ch->position==POS_SLEEPING) {
        act("You can't do that while sleeping.",ch,0,0,TO_CHAR);
        return;
    }
    if (obj->pIndexData->vnum == OBJ_VNUM_TATTOO_ASS)
    {
	do_invoke_jackass(ch);
    }
    if (obj->pIndexData->vnum == OBJ_VNUM_TATTOO_JERLE)
    {
	do_invoke_jerle(ch);
    }
    if (obj->pIndexData->vnum == OBJ_VNUM_TATTOO_VAS)
    {
        do_invoke_vasilis(ch);
    }
    if (obj->pIndexData->vnum == OBJ_VNUM_TATTOO_DEO)
    {
	do_invoke_deomanix(ch);
    }
    if (obj->pIndexData->vnum == 66 /*Orduno*/)
    {
	do_invoke_orduno(ch);
    }
    if (obj->pIndexData->vnum == 67 /*dhayton*/)
    {
	do_invoke_dhayton(ch);
    }
    if (obj->pIndexData->vnum == 57 /*detlef*/)
    {
	do_invoke_detlef(ch);
    }

   // if (obj->pIndexData->vnum == 4452 /*merthairu*/)
   // {
   //     do_invoke_merthairu(ch);
   // }
	if (obj->pIndexData->vnum == 72 /*Ceial*/)
	{
		do_invoke_ceial(ch);
	}
}

void do_invoke_dhayton(CHAR_DATA *ch)
{
    AFFECT_DATA af;
    OBJ_DATA *obj;

    if (is_affected(ch,gsn_rage))
    {
        send_to_char("You are already filled with rage!\n\r",ch);
        return;
    }
    obj = get_eq_char(ch, WEAR_BRAND);
    act("$n's $p glows a dark crimson as $s  eyes fill with rage!",ch,obj,0,TO_ROOM);
    act("Your $p glows a dark crimson as you are filled with rage!",ch,obj,0,TO_CHAR);

    ch->move /= 3;

        init_affect(&af);
    af.type         = gsn_rage;
        af.aftype    = AFT_SKILL;
    af.level        = ch->level;
    af.duration     = 5;
    af.modifier     = 30;
    af.bitvector    = AFF_HASTE;
    af.location     = APPLY_HITROLL;
    affect_to_char(ch,&af);
    af.location     = APPLY_DAMROLL;
    affect_to_char(ch,&af);
    af.location     = APPLY_DEX;
    af.modifier     = 5;
    affect_to_char(ch,&af);
    af.location     = APPLY_STR;
    affect_to_char(ch,&af);
    return;
}

void do_invoke_orduno(CHAR_DATA *ch)
{
	OBJ_DATA *obj;
	AFFECT_DATA af;
	int num;
	obj = get_eq_char(ch, WEAR_BRAND);

	if (is_affected(ch,gsn_body_of_wood) || is_affected(ch,gsn_body_of_iron) || is_affected(ch,gsn_body_of_clay) || is_affected(ch,gsn_body_of_steel) || is_affected(ch,gsn_body_of_stone) || is_affected(ch,gsn_body_of_diamond))
	{
	    act("Your $p begins to feel warm, but nothing happens.",ch,obj,0,TO_CHAR);
	    return;
	}

	act("Your $p begins to glow and all of a sudden grows red-hot wracking your body with spasms of pain.",ch,obj,0,TO_CHAR);
	act("$n's $p begins to glow fiercly and all of a sudden $n begins to spasm in pain clutching their brand.",ch,obj,0,TO_ROOM);

        init_affect(&af);
	af.duration = ch->level / 4;
	af.level = ch->level;
	af.where = TO_AFFECTS;
        af.aftype    = AFT_SPELL;
	af.modifier = 0;
	af.bitvector = 0;
	af.location = APPLY_NONE;
	//for a random affect
	num = number_range(1,6);
	if (num == 1)
	{
	        init_affect(&af);
		af.type = gsn_body_of_wood;
	        af.aftype    = AFT_SPELL;
		send_to_char("As the pain fades you realize that your skin has become made of wood!\n\r",ch);
		act("$n's skin slowly comes to more resemble wood then skin",ch,NULL,NULL,TO_ROOM);
	}
	if (num == 2)
	{
	        init_affect(&af);
		af.type = gsn_body_of_clay;
	        af.aftype    = AFT_SPELL;
		send_to_char("As the pain fades you realize that your skin has become made of clay!",ch);
                act("$n's skin slowly comes to more resemble clay then skin",ch,NULL,NULL,TO_ROOM);
        }
	if (num == 3)
	{
	        init_affect(&af);
		 af.type = gsn_body_of_iron;
	        af.aftype    = AFT_SPELL;
		send_to_char("As the pain fades you realize that your skin has become made of iron!",ch);
                act("$n's skin slowly comes to more resemble iron then skin",ch,NULL,NULL,TO_ROOM);
         }
	if (num == 4)
	{
	        init_affect(&af);
		 af.type = gsn_body_of_steel;
	        af.aftype    = AFT_SPELL;
		send_to_char("As the pain fades you realize that your skin has become made of steel!",ch);
                act("$n's skin slowly comes to more resemble steel then skin",ch,NULL,NULL,TO_ROOM);
         }
	if (num == 5)
	{
	        init_affect(&af);
		 af.type = gsn_body_of_stone;
	        af.aftype    = AFT_SPELL;
		send_to_char("As the pain fades you realize that your skin has become made of stone!",ch);
                act("$n's skin slowly comes to more resemble stone then skin",ch,NULL,NULL,TO_ROOM);
        }
	if (num == 6)
	{
	        init_affect(&af);
		 af.type = gsn_body_of_diamond;
	        af.aftype    = AFT_SPELL;
		 send_to_char("As the pain fades you realize that your skin has become made of diamond!",ch);
                act("$n's skin slowly comes to more resemble diamond then skin",ch,NULL,NULL,TO_ROOM);
	}
	affect_to_char(ch,&af);
}
void do_invoke_jackass( CHAR_DATA *ch
) {
	OBJ_DATA *obj;

	obj = get_eq_char(ch, WEAR_BRAND);
      act("$n's $p doesn't glow much due to $s stupidity.",ch,obj,0,TO_ROOM);
      act("Your $p doesn't glow much, probably because you're a jackass.",ch,obj,0,TO_CHAR);
      send_to_char("You smite yourself! What a jackass!\n\r",ch);
      act("$n smites $mself! What a jackass!",ch,NULL,NULL,TO_ROOM);
	ch->hit /= 2;
	return;
}

void do_invoke_jerle( CHAR_DATA *ch )
{




	send_to_char("Insert fancy tatto affect here.\n\r",ch);
	return;


	/*OBJ_DATA *obj;
	CHAR_DATA *victim;
	int x;
	int sn;

	if(ch->position!=POS_FIGHTING) {
		act("Nothing seemed to happen.",ch,0,0,TO_CHAR);
		return;
	}
	x=number_percent();
	obj = get_eq_char(ch, WEAR_BRAND);

	act("Comes to life to aid in the fight!",ch,obj,0,TO_ROOM);
        act("Comes to life to aid in the fight!",ch,obj,0,TO_CHAR);
	victim = ch->fighting;
	if(x<50) {
		sn = skill_lookup("holy word");
		spell_acid_blast(sn, 52, ch, victim, (int)NULL);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
		return;
	}*/
}

void do_invoke_vasilis( CHAR_DATA *ch )
{
	OBJ_DATA *obj;
	int sn;

	obj = get_eq_char(ch, WEAR_BRAND);

	act("$n's mark of $p begins to glow, bathing you in an aura of serenity.",ch,obj,0,TO_ROOM);
        act("Your mark of $p begins to glow, bathing you in an aura of serenity.",ch,obj,0,TO_CHAR);

	if (ch->position != POS_FIGHTING)
        {
		sn = skill_lookup("rejuvenate");
		spell_fortitude(sn, ch->level, ch, ch, (int)NULL);
		return;
        }
}

void do_invoke_merth( CHAR_DATA *ch )
{
	OBJ_DATA *obj;
	AFFECT_DATA af;

	if (is_affected(ch,gsn_artic_fury))
	{
        send_to_char("{BYou are already in a state of inner strength.{x\n\r",ch);
        return;
	}

	obj = get_eq_char(ch, WEAR_BRAND);
      act("{B$n's $p shimmers a faint blue.{x",ch,obj,0,TO_ROOM);
      act("{bYour $p flickers a bright blue!{x",ch,obj,0,TO_CHAR);

	ch->hit += (ch->level*2)* 10;
      ch->hit = UMIN(ch->hit,ch->max_hit);
      send_to_char("{bYou concentrate and focus your mind on your inner courage.{x\n\r",ch);
      act("{B$n flickers a mystical blue as they flare with artic fury!{x",ch,NULL,NULL,TO_ROOM);

	if (check_dispel(ch->level+3,ch,skill_lookup("blindness")))
	{
		act("$n is no longer blinded.",ch,NULL,NULL,TO_ROOM);
	}
	if (check_dispel(ch->level+3,ch,skill_lookup("curse")))
	{
		act("$n is no longer cursed.",ch,NULL,NULL,TO_ROOM);
	}
	if (check_dispel(ch->level+3,ch,skill_lookup("poison")))
	{
		act("$n looks less ill.",ch,NULL,NULL,TO_ROOM);
	}
	if (check_dispel(ch->level+3,ch,skill_lookup("plague")))
	{
		act("$n's sores vanish.",ch,NULL,NULL,TO_ROOM);
	}

        init_affect(&af);
      af.where        = TO_AFFECTS;
        af.aftype    = AFT_SPELL;
      af.type         = gsn_artic_fury;
      af.level        = ch->level;
      af.duration     = 24;
      af.modifier     = -200;
      af.bitvector    = AFF_HASTE;
      af.location     = APPLY_AC;
      affect_to_char(ch,&af);
      return;
}
void do_invoke_deomanix( CHAR_DATA *ch )
{
	int sn;

	if(ch->position!=POS_FIGHTING) {
		 sn = skill_lookup("kinetic shield");
                 spell_kinetic_shield(sn, ch->level, ch, ch, (int)NULL);
                 return;
	}
}
void do_invoke_ceial(CHAR_DATA *ch)
{
    AFFECT_DATA af;
    OBJ_DATA *obj;

    if (is_affected(ch,gsn_dark_blessing))
    {
        send_to_char("The dark lord has already blessed you!\n\r",ch);
        return;
    }
    obj = get_eq_char(ch, WEAR_BRAND);
    act("$n's $p hisses violently!",ch,obj,0,TO_ROOM);
    act("Your $p hisses violently!",ch,obj,0,TO_CHAR);
	send_to_char("The dark lord has granted you a blessing.\n\r",ch);

    init_affect(&af);
    af.type         = gsn_dark_blessing;
    af.aftype       = AFT_SPELL;
    af.level        = ch->level;
    af.duration     = 20;
    af.modifier     = -25;
    af.bitvector    = 0;
    af.location		= APPLY_SAVES;
    affect_to_char(ch,&af);
    af.location     = APPLY_HITROLL;
    af.modifier     = 25;
    affect_to_char(ch,&af);
    return;
}

void do_invoke_detlef( CHAR_DATA *ch )
{
	OBJ_DATA *obj;
	AFFECT_DATA af;

	if (is_affected(ch,gsn_phat_blunt))
	{
        send_to_char("You cannot smoke phat blunt too much!\n\r",ch);
        return;
	}

	obj = get_eq_char(ch, WEAR_BRAND);
      act("$n's $p glows green.",ch,obj,0,TO_ROOM);
      act("Your $p glows green.",ch,obj,0,TO_CHAR);

	ch->hit += (ch->level*2)* 10;
      ch->hit = UMIN(ch->hit,ch->max_hit);
      send_to_char("You smoke a phat blunt and feel MUCH better.\n\r",ch);
      act("$n smokes $s phat blunt and feels much better.",ch,NULL,NULL,TO_ROOM);

	if (check_dispel(ch->level+3,ch,skill_lookup("blindness")))
	{
		act("$n is no longer blinded.",ch,NULL,NULL,TO_ROOM);
	}
	if (check_dispel(ch->level+3,ch,skill_lookup("curse")))
	{
		act("$n is no longer cursed.",ch,NULL,NULL,TO_ROOM);
	}
	if (check_dispel(ch->level+3,ch,skill_lookup("poison")))
	{
		act("$n looks less ill.",ch,NULL,NULL,TO_ROOM);
	}
	if (check_dispel(ch->level+3,ch,skill_lookup("plague")))
	{
		act("$n's sores vanish.",ch,NULL,NULL,TO_ROOM);
	}

        init_affect(&af);
      af.where        = TO_AFFECTS;
        af.aftype    = AFT_SKILL;
      af.type         = gsn_phat_blunt;
      af.level        = ch->level;
      af.duration     = 24;
      af.modifier     = 10;
      af.bitvector    = 0;
      af.location     = APPLY_HITROLL;
      affect_to_char(ch,&af);
      af.location     = APPLY_DAMROLL;
      affect_to_char(ch,&af);
      af.location     = APPLY_DEX;
      af.modifier     = 4;
      affect_to_char(ch,&af);
      return;
}

