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
*       ROM 2.4 is copyright 1993-1996 Russ Taylor                         *
*       ROM has been brought to you by the ROM consortium                  *
*           Russ Taylor (rtaylor@pacinfo.com)                              *
*           Gabrielle Taylor (gtaylor@pacinfo.com)                         *
*           Brian Moore (rom@rom.efn.org)                                  *
*       By using this code, you have agreed to follow the terms of the     *
*       ROM license, in the file Tartarus/doc/rom.license                  *
***************************************************************************/

/***************************************************************************
*       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
*	In using this code you agree to comply with the Tartarus license   *
*       found in the file /Tartarus/doc/tartarus.doc                       *
***************************************************************************/

/***************************************************************************
*       POSmud code is copyright (C) 1999-2000 and for exclusive use of    *
*	POSmud only. Hah!                                                  *
***************************************************************************/

#include "include.h"

#define HITS (dt==gsn_kick||dt==gsn_lunge||dt==gsn_downstrike||dt==gsn_throw||dt==gsn_crush||dt==gsn_bash)


/* command procedures needed */
DECLARE_DO_FUN(do_quit);
DECLARE_DO_FUN(do_say);
DECLARE_DO_FUN(do_backstab      );
DECLARE_DO_FUN(do_emote         );
DECLARE_DO_FUN(do_ambush	);
DECLARE_DO_FUN(do_berserk       );
DECLARE_DO_FUN(do_deathstyle    );
DECLARE_DO_FUN(do_bash          );
DECLARE_DO_FUN(do_trip          );
DECLARE_DO_FUN(do_dirt          );
DECLARE_DO_FUN(do_flee          );
DECLARE_DO_FUN(do_kick          );
DECLARE_DO_FUN(do_disarm        );
DECLARE_DO_FUN(do_get           );
DECLARE_DO_FUN(do_recall        );
DECLARE_DO_FUN(do_yell          );
DECLARE_DO_FUN(do_myell          );
DECLARE_DO_FUN(do_sacrifice     );
DECLARE_DO_FUN(do_circle_stab	);
DECLARE_DO_FUN(do_battlecry	);
DECLARE_DO_FUN(do_steel_nerves	);
DECLARE_DO_FUN(do_look);
DECLARE_DO_FUN(do_crush);
DECLARE_DO_FUN(do_tail	);
DECLARE_DO_FUN(do_undead_drain);
DECLARE_DO_FUN(do_executioners_grace);
DECLARE_DO_FUN(do_bind);
DECLARE_DO_FUN(do_unbind);
DECLARE_DO_FUN(do_blitz);
DECLARE_SPELL_FUN(spell_evil_eye);
DECLARE_SPELL_FUN(spell_power_word_kill);
DECLARE_DO_FUN(do_echo);
DECLARE_DO_FUN(do_help);
DECLARE_DO_FUN(do_cb);
DECLARE_DO_FUN(do_ccb);
//DECLARE_DO_FUN(do_arena_echo);
void temp_death_log(CHAR_DATA *killer, CHAR_DATA *dead);
void perm_death_log(CHAR_DATA *ch, int deltype);

/*
 * Local functions.
 */
void check_downstrike args( (CHAR_DATA *ch, CHAR_DATA *victim) );
void check_assist args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool check_dodge args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
bool check_questdodge args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
bool check_block args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void check_killer args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool check_parry args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
//bool check_flourintine args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
bool check_spin args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
bool check_displacement args( (CHAR_DATA *ch, CHAR_DATA *victim) );
bool check_defiant_absorb args((CHAR_DATA *ch, CHAR_DATA *victim, int dt ));
bool check_shield_block args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void dam_message args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam,int dt, bool immune ) );
void    death_cry       args( ( CHAR_DATA *ch ) );
void    group_gain      args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
int     xp_compute      args( ( CHAR_DATA *gch, CHAR_DATA *victim,int group_amount, int glevel ) );
bool    is_safe         args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    make_corpse     args( ( CHAR_DATA *killer, CHAR_DATA *ch ) );
void    one_hit         args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ));
void    mob_hit         args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void    raw_kill        args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    set_fighting    args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    disarm          args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
int 	battlecry_multiplier 	args( ( CHAR_DATA *ch, int dt) );
void do_counter_parry	args( (CHAR_DATA *ch,CHAR_DATA *victim, int dt,int dam) );
void 	check_ground_control args( (CHAR_DATA *ch,CHAR_DATA *victim,int chance,int dam) );
void	check_ranger_staff	args( (CHAR_DATA *ch,OBJ_DATA *wield) );
void	check_clown_mallet	args( (int level,CHAR_DATA *ch,CHAR_DATA *victim, OBJ_DATA *wield) );
void	golden_weapon	args( (CHAR_DATA *ch,OBJ_DATA *wield) );
void	check_ceial_tat args ( (CHAR_DATA *ch) );
void	check_outlaw_badge	args( (CHAR_DATA *ch) );
bool	check_counter	args( (CHAR_DATA *ch,CHAR_DATA *victim, int dam,int dt) );
bool 	check_palm	args( (CHAR_DATA *ch ) );
void check_follow_through	args( ( CHAR_DATA *ch, CHAR_DATA *victim,int dam) );
bool 	check_roll	args( (CHAR_DATA *ch,CHAR_DATA *victim,int dt) );
void	check_red_shield args(( CHAR_DATA *ch) );
void check_bracelets args((CHAR_DATA *ch));
void wing_beating args((CHAR_DATA *ch) );
void etched_pendant args((CHAR_DATA *ch) );
void hammer_pendant args((CHAR_DATA *ch) );
void kyr_tattoo args((CHAR_DATA *ch,CHAR_DATA *victim) );
void dev_tattoo args((CHAR_DATA *ch,CHAR_DATA *victim) );
void cei_tattoo(CHAR_DATA *ch);
void deo_tattoo	args( (CHAR_DATA *ch, CHAR_DATA *victim) );
void gra_tattoo	args( (CHAR_DATA *ch, CHAR_DATA *victim) );
void mer_tattoo(CHAR_DATA *ch);
void kra_tattoo args((CHAR_DATA *ch,CHAR_DATA *victim) );
void xanthr_tattoo args((CHAR_DATA *ch,CHAR_DATA *victim) );
bool check_crown_blood args((CHAR_DATA *ch,CHAR_DATA *victim,int dt, int dam, int dam_type));
void check_blue_talon args((CHAR_DATA *ch, CHAR_DATA *victim));
int 	check_cutter args((CHAR_DATA *ch,CHAR_DATA *victim));
bool 	eye_of_beholder_absorb args((CHAR_DATA *ch,CHAR_DATA *victim,
			int dt, int dam, int dam_type));
void do_strap args((CHAR_DATA *victim,OBJ_DATA *strap));
bool check_unholy_bless args((CHAR_DATA *ch, CHAR_DATA *victim));
void check_cheap_shot args((CHAR_DATA *ch, CHAR_DATA *victim));
void check_parting_blow args((CHAR_DATA *ch, CHAR_DATA *victim));
bool                 arena;
bool check_distance  args((CHAR_DATA *ch, CHAR_DATA *victim, int dt ));
bool check_ironhands args((CHAR_DATA *ch, CHAR_DATA *victim, int dt ));
bool check_unarmed_defense args((CHAR_DATA *ch, CHAR_DATA *victim, int dt ));
char *get_dam_message args((CHAR_DATA *ch, int dt));
char * get_attack_noun args((CHAR_DATA *ch, int dt));
int get_attack_number args((CHAR_DATA *ch, int dt));
bool check_dispel( int dis_level, CHAR_DATA *victim, int sn);
bool check_cutoff(CHAR_DATA *ch, CHAR_DATA *victim);
bool    check_chargeset args( ( CHAR_DATA *ch, CHAR_DATA *victim ));
void do_arena_echo(const char *txt);
int check_imperial_training(CHAR_DATA *ch);
bool check_wall_thorns args((CHAR_DATA *ch));
bool check_bloody args((CHAR_DATA *ch));

bool isShifted args ((CHAR_DATA *ch));
int get_shifted args ((CHAR_DATA *ch));
bool check_rangermob(CHAR_DATA *mob);

CHAR_DATA *jujitsu_victim;

 /*
 * Control the fights going on.
 * Called periodically by update_handler.
 */
void violence_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *victim;
    int regen, to_heal;
    OBJ_DATA *torque;
    OBJ_DATA *obj;

    //for ( ch = char_list; ch != NULL; ch = ch->next )
    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
        ch_next = ch->next;

	obj = get_eq_char(ch,WEAR_HEAD);
	if (obj != NULL && obj->pIndexData->vnum == OBJ_VNUM_TIARA)
	{
	ch->mana = UMIN(ch->mana + number_range(1,4),ch->max_mana);
    	}
	obj = get_eq_char(ch,WEAR_HOLD);
	if (obj != NULL && obj->pIndexData->vnum == 14739)
	{
	/* greater symbol of magic */
	ch->mana = UMIN(ch->mana + number_range(1,3),ch->max_mana);
	}

/* regen */
	if (!IS_NPC(ch))
	{
	regen = get_skill(ch,gsn_regeneration);
	to_heal = (ch->max_hit - ch->hit);

	if (number_percent() <=  ch->level*2)
	{
	if ((ch->hit < ch->max_hit)
	&& ch->noregen_dam < to_heal
	&& number_percent() < regen)
	{
	if (number_percent() < 3)
		check_improve(ch,gsn_regeneration,TRUE,4);
	ch->hit += number_range(1,3);
	}
	}
	}

	if (ch->regen_rate != 0)
	{
	if (ch->regen_rate > 0)
	regen = number_range(1,ch->regen_rate);
	else
	regen = number_range(1,-ch->regen_rate);
	if (ch->hit < ch->max_hit)
	{
	if (ch->regen_rate > 0)
	ch->hit = UMIN(ch->hit + regen,ch->max_hit);
	else
	ch->hit = UMIN(ch->hit - regen,ch->max_hit);
	}
	}

        torque = get_eq_char(ch,WEAR_NECK_1);
        if (torque == NULL)
                torque = get_eq_char(ch,WEAR_NECK_2);
        if (torque != NULL)
        {
        if (torque->pIndexData->vnum == OBJ_VNUM_TORQUE)
                if (ch->hit < ch->max_hit)
                        ch->hit = UMIN(ch->hit + dice(1,3),ch->max_hit);
        }

        if ( ( victim = ch->fighting ) == NULL || ch->in_room == NULL )
            continue;

	if (IS_NPC(ch) && (!IS_NPC(ch->fighting))
	&& !IS_SET(ch->act,ACT_NO_TRACK)
	&& !IS_AFFECTED(ch,AFF_CHARM)
	&& !IS_SET(ch->act,ACT_IS_HEALER)
	&& !IS_SET(ch->act,ACT_BANKER))
		ch->last_fought = ch->fighting;

	update_pc_last_fight(ch,ch->fighting);

        if ( IS_AWAKE(ch) && ch->in_room == victim->in_room )
            multi_hit( ch, victim, TYPE_UNDEFINED );
        else
            stop_fighting( ch, FALSE );

        if ( ( victim = ch->fighting ) == NULL )
            continue;

        if ( IS_NPC( ch ) )
        {
            if ( HAS_TRIGGER( ch, TRIG_FIGHT ) )
                mp_percent_trigger( ch, victim, NULL, NULL, TRIG_FIGHT );
            if ( HAS_TRIGGER( ch, TRIG_HPCNT ) )
                mp_hprct_trigger( ch, victim );
        }

        /*
         * Fun for the whole family!
         */
        check_assist(ch,victim);
    }

    return;
}

/* for auto assisting */
void check_assist(CHAR_DATA *ch,CHAR_DATA *victim)
{
    CHAR_DATA *rch, *rch_next;

    for (rch = ch->in_room->people; rch != NULL; rch = rch_next)
    {
        rch_next = rch->next_in_room;

	if (!IS_NPC(rch) && rch->ghost > 0)
		continue;

        if (IS_AWAKE(rch) && rch->fighting == NULL)
        {

/* NPC assisting group (for charm, zombies, elementals..added by Ceran */
	if (IS_NPC(rch) && (IS_AFFECTED(rch,AFF_CHARM)) && is_same_group(rch,ch) )
 	{
		multi_hit(rch,victim,TYPE_UNDEFINED);
		continue;
	}


            /* quick check for ASSIST_PLAYER */
            if (!IS_NPC(ch) && IS_NPC(rch)
            && IS_SET(rch->off_flags,ASSIST_PLAYERS)
            &&  rch->level + 6 > victim->level)
            {
                do_emote(rch,"screams and attacks!");
                multi_hit(rch,victim,TYPE_UNDEFINED);
                continue;
            }

            /* PCs next */
            if ( !IS_NPC(ch) || IS_AFFECTED(ch,AFF_CHARM))
            {
/* First check defending */
		if (rch->defending != NULL)
		{
		CHAR_DATA *fch;

		fch = ch->fighting;
		if (rch->defending != NULL
		&& rch->defending == ch
		&& fch != NULL
		&& fch->fighting == ch
		&& number_percent() < get_skill(rch,gsn_defend)
		&& number_percent() > 40
		&& number_percent() < get_skill(rch,gsn_rescue))
		{
	act("$n leaps to $N's rescue!",rch,0,ch,TO_NOTVICT);
	act("$n leaps to your rescue!",rch,0,ch,TO_VICT);
	act("You leap to $N's rescue!",rch,0,ch,TO_CHAR);

    if (is_safe(rch,fch))
	return;

    WAIT_STATE( rch, skill_table[gsn_rescue].beats );
    WAIT_STATE(ch,12);
    check_improve(rch,gsn_defend,TRUE,1);

    stop_fighting( fch, FALSE );
    stop_fighting( ch, FALSE );

    set_fighting( rch, fch );
    set_fighting( fch, rch );
	if (IS_NPC(fch)
	&& (fch->pIndexData->vnum == MOB_VNUM_COVEN
	|| fch->pIndexData->vnum == MOB_VNUM_ACADIAN
	|| fch->pIndexData->vnum == MOB_VNUM_PROTECTOR
	|| fch->pIndexData->vnum == MOB_VNUM_RAVAGER
    || fch->pIndexData->vnum == MOB_VNUM_BRIAR
	|| fch->pIndexData->vnum == MOB_VNUM_PROTECTOR) )
		set_fighting(ch,fch);

		continue;
		}
	}


                if ( ( (!IS_NPC(rch) && IS_SET(rch->act,PLR_AUTOASSIST))
                ||     IS_AFFECTED(rch,AFF_CHARM))
                &&   is_same_group(ch,rch)
                &&   !is_safe(rch, victim))
                    multi_hit (rch,victim,TYPE_UNDEFINED);

                continue;
            }

            /* now check the NPC cases */

            if (IS_NPC(ch) && !IS_AFFECTED(ch,AFF_CHARM))

            {
                if ( (IS_NPC(rch) && IS_SET(rch->off_flags,ASSIST_ALL))

                ||   (IS_NPC(rch) && rch->group && rch->group == ch->group)

                ||   (IS_NPC(rch) && rch->race == ch->race
                   && IS_SET(rch->off_flags,ASSIST_RACE))

                ||   (IS_NPC(rch) && IS_SET(rch->off_flags,ASSIST_ALIGN)
                   &&   ((IS_GOOD(rch)    && IS_GOOD(ch))
                     ||  (IS_EVIL(rch)    && IS_EVIL(ch))
                     ||  (IS_NEUTRAL(rch) && IS_NEUTRAL(ch))))

                ||   (rch->pIndexData == ch->pIndexData
                   && IS_SET(rch->off_flags,ASSIST_VNUM)))

                {
                    CHAR_DATA *vch;
                    CHAR_DATA *target;
                    int number;

                    if (number_bits(1) == 0)
                        continue;

                    target = NULL;
                    number = 0;
                    for (vch = ch->in_room->people; vch; vch = vch->next)
                    {
                        if (can_see(rch,vch)
                        &&  is_same_group(vch,victim)
                        &&  number_range(0,number) == 0)
                        {
                            target = vch;
                            number++;
                        }
                    }

                    if (target != NULL)
                    {
                        do_emote(rch,"screams and attacks!");
                        multi_hit(rch,target,TYPE_UNDEFINED);
                    }
                }
            }
        }
    }
}


/*
 * Do one group of attacks.
 */
void multi_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    int     chance;
    int	    dual_chance;
    int	trinal_chance;
    int attacks;
    int form;
    int	i;
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;

    OBJ_DATA *obj;
    OBJ_DATA *wield;
    if (is_affected(ch,gsn_timestop))
    {
	if (ch->fighting != NULL)
		ch->fighting = NULL;
	return;
    }
    /* decrement the wait */
    if (ch->desc == NULL)
        ch->wait = UMAX(0,ch->wait - PULSE_VIOLENCE);

	if (ch->level == MAX_LEVEL) {
		ch->wait = 0;
	}

    if (ch->desc == NULL)
        ch->daze = UMAX(0,ch->daze - PULSE_VIOLENCE);

	if (ch->level == MAX_LEVEL)
		ch->daze = 0;

	for ( paf = ch->affected; paf != NULL; paf = paf_next )
	{
		paf_next = paf->next;

		if (paf->wait > 0)
		{
			paf->wait -= 1;
		}

		/* jujitsu check - start */
		if ( paf->type == gsn_jujitsu && paf->wait == 0)
		{

			char jujitsubuf[MSL];
			int jujitsu_chance, dam;
			jujitsu_victim = paf->owner;

			jujitsu_chance = get_skill(ch,gsn_jujitsu);

			dam = (ch->drain_level + ch->level)/2;
			dam += number_range(0,(ch->drain_level + ch->level)/6);
			dam += number_range(0,(ch->drain_level + ch->level)/6);
			dam += number_range(0,(ch->drain_level + ch->level)/6);
			dam += number_range(0,(ch->drain_level + ch->level)/6);
			dam += number_range(0,(ch->drain_level + ch->level)/6);
			dam += number_range(0,(ch->drain_level + ch->level)/6);
			dam += number_range(ch->level/5,ch->level/4);

			if (jujitsu_victim->fighting != ch)
			{
				sprintf(jujitsubuf,"%s is not fighting you anymore, and you cannot complete your manuever.\n\r",jujitsu_victim->name);
				send_to_char(jujitsubuf,ch);
				affect_strip(ch,gsn_jujitsu);
			}
			else
			{
				if (number_percent() < jujitsu_chance)
				{
					send_to_char("{GATTACK 1!!!!{x\n\r",ch);
					damage(ch,jujitsu_victim,dam,gsn_jujitsu,DAM_BASH,TRUE);
					jujitsu_chance -= 10;
					dam -= number_range(0,(ch->drain_level + ch->level)/6);
					if (number_percent() < jujitsu_chance)
					{
						send_to_char("{GATTACK 2!!!!{x\n\r",ch);
						damage(ch,jujitsu_victim,dam,gsn_jujitsu,DAM_BASH,TRUE);
						jujitsu_chance -= 10;
						dam -= number_range(0,(ch->drain_level + ch->level)/6);
						if (number_percent() < jujitsu_chance)
						{
							send_to_char("{GATTACK 3!!!!{x\n\r",ch);
							damage(ch,jujitsu_victim,dam,gsn_jujitsu,DAM_BASH,TRUE);
							jujitsu_chance -= 10;
							dam -= number_range(0,(ch->drain_level + ch->level)/6);
							if (number_percent() < jujitsu_chance)
							{
								send_to_char("{GATTACK 4!!!!{x\n\r",ch);
								damage(ch,jujitsu_victim,dam,gsn_jujitsu,DAM_BASH,TRUE);
								jujitsu_chance -= 10;
								dam -= number_range(0,(ch->drain_level + ch->level)/6);
								if (number_percent() < jujitsu_chance)
								{
									send_to_char("{GATTACK 5!!!!{x\n\r",ch);
									damage(ch,jujitsu_victim,dam,gsn_jujitsu,DAM_BASH,TRUE);
								}
							}
						}
					}
				}
				affect_strip(ch,gsn_jujitsu);
			}
		}
		/* jujitsu check - end */
	}

if(is_affected(victim,gsn_snare) && number_percent()<11 && victim->fighting!=NULL)
	{
		act("In the chaos of combat, the snare holding $n comes loose.",victim,0,0,TO_ROOM);
		act("In the chaos of combat, the snare holding you comes loose.",victim,0,0,TO_CHAR);
		affect_strip(victim,gsn_snare);
	}

    /* no attacks for stunnies -- just a check */
    if (ch->position < POS_RESTING)
        return;
    if (IS_NPC(ch)) {
	    if (ch->pIndexData->vnum  == MOB_VNUM_WATER_ELEMENTAL) {
		   if (number_percent() < 20) {
			do_bash(ch,"");
	         }
	    }
	    if (ch->pIndexData->vnum  == MOB_VNUM_AIR_ELEMENTAL) {
		   if (number_percent() < 20) {
			do_trip(ch,"");
	         }
	    }
      }
    if (IS_NPC(ch))
    {
        mob_hit(ch,victim,dt);
        return;
    }

if( isShifted(ch) )
    {
	form = get_shifted(ch);
	attacks = number_range(1, form_table[form].attacks);

	for(i = 0; i < attacks; i++)
	{
		one_hit(ch, victim, dt);
	}
	return;
    }

    one_hit( ch, victim, dt );
    if (ch->fighting != victim)
        return;
    check_bracelets(ch);
    wing_beating(ch);
    hammer_pendant(ch);
    etched_pendant(ch);
    kyr_tattoo(ch,victim);
    dev_tattoo(ch,victim);
    mer_tattoo(ch);
    cei_tattoo(ch);
	deo_tattoo(ch,victim);
    gra_tattoo(ch,victim);
    kra_tattoo(ch,victim);
    xanthr_tattoo(ch,victim);
    obj = get_eq_char(ch,WEAR_BRAND);
    if (ch->fighting == victim)

   if ( (wield = get_eq_char(ch,WEAR_WIELD)) != NULL)
    {
        if (ch->fighting == victim && wield->pIndexData->vnum == 14726)
                check_blue_talon(ch,ch->fighting);
    }

    if ( ch->fighting != victim || dt == gsn_backstab || (dt ==  gsn_ambush) )
        return;

    if (IS_AFFECTED(ch,AFF_HASTE))
        one_hit(ch,victim,dt);


    chance = get_skill(ch,gsn_second_attack)/2;
    dual_chance = get_skill(ch,gsn_dual_wield)/2;
    trinal_chance = get_skill(ch,gsn_tertiary_wield)/2;

    if (is_affected(ch,gsn_maehslin) && number_percent() < 65)
    {
	act("The amber aura around $n intenses and their movements become more fluid!",ch,0,0,TO_VICT);
    act("$n's amber aura intenses and their movements become more fluid!" ,ch,0,0,TO_NOTVICT);
    act("Your amber aura intenses and your movements become more fluid!",ch,0,0,TO_CHAR);
	one_hit(ch,victim,dt);
    chance*=1.25;
	dual_chance*=1.25;
	chance*=1.25;
	}
    if (IS_AFFECTED(ch,AFF_HASTE))
    {
	chance*=1.25;
	dual_chance*=1.25;
	chance*=1.25;
    }
    if (IS_AFFECTED(ch,AFF_SLOW))
	{
      chance /= 2;
	dual_chance /= 2;
	trinal_chance /= 2;
	}
	if (check_imperial_training(ch) == IMPERIAL_DEFENSE)
	{
	chance /= 2.5;
	dual_chance /= 2.5;
	}

    if ( number_percent( ) < chance )
    {
        one_hit( ch, victim, dt );
        check_improve(ch,gsn_second_attack,TRUE,5);
        if ( ch->fighting != victim )
            return;
    }
    check_bracelets(ch);
    wing_beating(ch);

    if ( (wield = get_eq_char(ch,WEAR_WIELD)) != NULL)
    {
        if (ch->fighting == victim && wield->pIndexData->vnum == 14726)
                check_blue_talon(ch,ch->fighting);
    }

    if (get_eq_char(ch,WEAR_DUAL_WIELD) != NULL
	&& ch->pcdata->learned[gsn_second_attack] > 10)
    {
	if (number_percent( ) < dual_chance)
	{
	one_hit(ch,victim,gsn_dual_wield);
	check_improve(ch,gsn_dual_wield,TRUE,3);
	if (ch->fighting != victim)
		return;
	}
	else
	check_improve(ch,gsn_dual_wield,FALSE,3);
    }

    if (get_eq_char(ch,WEAR_TERTIARY_WIELD) != NULL
	&& ch->pcdata->learned[gsn_second_attack] > 10)
    {
	if (number_percent( ) < trinal_chance)
	{
	one_hit(ch,victim,gsn_tertiary_wield);
	check_improve(ch,gsn_tertiary_wield,TRUE,3);
	if (ch->fighting != victim)
		return;
	}
	else
	check_improve(ch,gsn_tertiary_wield,FALSE,3);
    }

	if (!IS_NPC(ch) && !str_cmp(class_table[ch->class].name,"monk")
	&&  (get_eq_char(ch,WEAR_WIELD) != NULL) )
	return; 	/* Monks can't get 3/4th attack with a weapon */

    chance = get_skill(ch,gsn_third_attack)/4;

    if (IS_AFFECTED(ch,AFF_SLOW))
        chance = 0;
    if (is_affected(ch,skill_lookup("haste")))
        chance*=1.3;
    if ( number_percent( ) < chance )
    {
        one_hit( ch, victim, dt );
        check_improve(ch,gsn_third_attack,TRUE,6);
        if ( ch->fighting != victim )
            return;
    }

    dual_chance = get_skill(ch,gsn_dual_wield)/4;
    if (is_affected(ch,skill_lookup("haste")))
        dual_chance*=1.2;
    if (get_eq_char(ch,WEAR_DUAL_WIELD) != NULL
	&& ch->pcdata->learned[gsn_third_attack] > 10)
    {
	if (number_percent( ) < dual_chance)
	{
	one_hit(ch,victim,gsn_dual_wield);
	check_improve(ch,gsn_dual_wield,TRUE,3);
	if (ch->fighting != victim)
		return;
	}
	else
	check_improve(ch,gsn_dual_wield,FALSE,3);
    }

    trinal_chance = get_skill(ch,gsn_tertiary_wield)/3;
    if (is_affected(ch,skill_lookup("haste")))
        trinal_chance*=1.2;
    if (get_eq_char(ch,WEAR_TERTIARY_WIELD) != NULL
	&& ch->pcdata->learned[gsn_third_attack] > 10)
    {
	if (number_percent( ) < trinal_chance)
	{
	one_hit(ch,victim,gsn_tertiary_wield);
	check_improve(ch,gsn_tertiary_wield,TRUE,3);
	if (ch->fighting != victim)
		return;
	}
	else
	check_improve(ch,gsn_tertiary_wield,FALSE,3);
    }

    chance = get_skill(ch,gsn_fourth_attack)/5;

    if (IS_AFFECTED(ch,AFF_SLOW))
	chance /= 2;
    if (is_affected(ch,skill_lookup("haste")))
        chance*=1.3;
    if ( number_percent() < chance )
    {
	one_hit( ch, victim, dt );
	check_improve(ch,gsn_fourth_attack,TRUE,6);
	if (ch->fighting != victim )
	    return;
    }

    dual_chance = get_skill(ch,gsn_dual_wield)/4;
    if (is_affected(ch,skill_lookup("haste")))
        dual_chance*=1.2;
    if (get_eq_char(ch,WEAR_DUAL_WIELD) != NULL
	&& ch->pcdata->learned[gsn_fourth_attack] > 10)
    {
	if (number_percent( ) < dual_chance)
	{
	one_hit(ch,victim,gsn_dual_wield);
	check_improve(ch,gsn_dual_wield,TRUE,6);
	if (ch->fighting != victim)
		return;
	}
	else
	check_improve(ch,gsn_dual_wield,FALSE,5);
    }

    trinal_chance = get_skill(ch,gsn_tertiary_wield)/6;
    if (is_affected(ch,skill_lookup("haste")))
        trinal_chance*=1.1;
    if (get_eq_char(ch,WEAR_TERTIARY_WIELD) != NULL
	&& ch->pcdata->learned[gsn_fourth_attack] > 85)
    {
	if (number_percent( ) < trinal_chance)
	{
	one_hit(ch,victim,gsn_tertiary_wield);
	check_improve(ch,gsn_tertiary_wield,TRUE,3);
	if (ch->fighting != victim)
		return;
	}
	else
	check_improve(ch,gsn_tertiary_wield,FALSE,3);
    }

/* zealotism */
	chance = get_skill(ch,gsn_zealotism)/4;
	if (chance != 0) {
	if (IS_AFFECTED(ch,AFF_SLOW))
		chance /= 2;
	if (IS_AFFECTED(ch,AFF_HASTE))
		chance*=1.25;
	if (is_affected(ch,gsn_bloodthirst))
		chance*=1.25;
	if (is_affected(ch,gsn_rage_of_dioxide))
		chance*=1.35;
	if (IS_AFFECTED(ch,AFF_BERSERK))
		chance*=1.25;

	for (i=0; i<(chance/20); i++) {
		act("Delirious with zeal, you strike $N again!",ch,0,victim,TO_CHAR);
		one_hit(ch, victim, dt);
	}
	check_improve(ch,gsn_zealotism,TRUE,1);
	if (ch->fighting != victim )
	return;
	}

/* imperial training */

	if (check_imperial_training(ch) == IMPERIAL_OFFENSE && get_skill(ch,gsn_third_attack)>80)
	{
		chance = number_range(1,2);
		for (i=0; i<chance; i++)
			one_hit(ch, victim, dt);
	}
	return;
}

/* procedure for all mobile attacks */
void mob_hit (CHAR_DATA *ch, CHAR_DATA *victim, int dt)
{
    int chance,number;
    int dual_chance;
    CHAR_DATA *vch, *vch_next;

    one_hit(ch,victim,dt);

    if (ch->fighting != victim)
        return;

    /* Area attack -- BALLS nasty! */
    if (IS_SET(ch->off_flags,OFF_AREA_ATTACK))
    {
        for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
        {
            vch_next = vch->next;
            if ((vch != victim && vch->fighting == ch))
                one_hit(ch,vch,dt);
        }
    }

    if (is_affected(ch,skill_lookup("haste"))
    ||  (IS_SET(ch->off_flags,OFF_FAST) && !IS_AFFECTED(ch,AFF_SLOW)))
        one_hit(ch,victim,dt);

    if (ch->fighting != victim || dt == gsn_backstab)
        return;

    chance = get_skill(ch,gsn_second_attack)/2;
    dual_chance = ch->level*2/3;

    if (IS_AFFECTED(ch,AFF_SLOW) && !IS_SET(ch->off_flags,OFF_FAST))
    {
	dual_chance /= 2;
        chance /= 2;
    }

    if (number_percent() < chance)
    {
        one_hit(ch,victim,dt);
        if (ch->fighting != victim)
            return;
    }

    if (IS_AFFECTED(ch,AFF_HASTE))
	dual_chance *= 6/4;

    if (number_percent() < dual_chance && get_eq_char(ch,WEAR_DUAL_WIELD) != NULL)
    {
	one_hit(ch,victim,gsn_dual_wield);
	if (ch->fighting != victim)
		return;
    }

    chance = get_skill(ch,gsn_third_attack)/4;
     dual_chance *= 2/3;

    if (IS_AFFECTED(ch,AFF_SLOW) && !IS_SET(ch->off_flags,OFF_FAST))
    {
	dual_chance = 0;
        chance = 0;
    }

    if (IS_AFFECTED(ch,AFF_HASTE))
	dual_chance *= 3/2;

    if (number_percent() < chance)
    {
        one_hit(ch,victim,dt);
        if (ch->fighting != victim)
            return;
    }

    if (number_percent() < dual_chance && get_eq_char(ch,WEAR_DUAL_WIELD) != NULL)
    {
	one_hit(ch,victim,gsn_dual_wield);
	if (ch->fighting != victim)
		return;
    }

    /* oh boy!  Fun stuff! */

    if (ch->wait > 0)
        return;

    number = number_range(0,2);

    if (number == 1 && IS_SET(ch->act,ACT_MAGE))
    {
        /*  { mob_cast_mage(ch,victim); return; } */ ;
    }

    if (number == 2 && IS_SET(ch->act,ACT_CLERIC))
    {
        /* { mob_cast_cleric(ch,victim); return; } */ ;
    }

    /* now for the skills */

    number = number_range(0,9);

    switch(number)
    {
    case (0) :
        if (IS_SET(ch->off_flags,OFF_BASH))
            do_bash(ch,"");
	else if (IS_SET(ch->off_flags,OFF_TAIL))
	    do_tail(ch,"");
        break;

    case (1) :
        if (IS_SET(ch->off_flags,OFF_BERSERK) && !IS_AFFECTED(ch,AFF_BERSERK))
            do_berserk(ch,"");
        break;


    case (2) :
        if (IS_SET(ch->off_flags,OFF_DISARM)
        || (get_weapon_sn(ch) != gsn_hand_to_hand
        && (IS_SET(ch->act,ACT_WARRIOR)
        ||  IS_SET(ch->act,ACT_THIEF))))
            do_disarm(ch,"");
        break;

    case (3) :
        if (IS_SET(ch->off_flags,OFF_KICK))
            do_kick(ch,"");
        break;

    case (4) :
        if (IS_SET(ch->off_flags,OFF_KICK_DIRT))
            do_dirt(ch,"");
        break;

    case (5) :
        if (IS_SET(ch->off_flags,OFF_TAIL))
        {
               do_tail(ch,"");
        }
        break;

    case (6) :
        if (IS_SET(ch->off_flags,OFF_TRIP))
            do_trip(ch,"");
        break;

    case (7) :
        if (IS_SET(ch->off_flags,OFF_CRUSH))
        {
            do_crush(ch,"") ;
        }
        break;
    case (8) :
        if (IS_SET(ch->off_flags,OFF_BACKSTAB))
        {
            do_backstab(ch,"");
        }
	else if (IS_SET(ch->off_flags,OFF_UNDEAD_DRAIN))
		do_undead_drain(ch,"");
	break;
    case (9) :
	if ( IS_SET(ch->off_flags,OFF_UNDEAD_DRAIN))
	{
		do_undead_drain(ch,"");
	}
	break;
    }
}


/*
 * Hit one guy once.
 */
void one_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt)
{
	OBJ_DATA *wield;
	OBJ_DATA *backfist_wield;
	OBJ_DATA *backfist_dual;
	OBJ_DATA *backfist_shield;
	OBJ_DATA *backfist_hold;
    int victim_ac;
    int thac0;
    OBJ_DATA *obj;
    int bcry;
    int thac0_00;
    int thac0_32;
    int dam, mdam, bladam, tchance;
    int diceroll, diceroll2 = 0;
    int sn,skill;
    int dam_type;
    int tmp_dt;
	int handcounter=0;
    bool result;
    bool exeresult;
    int exedam;
    int form;
    /*sn = -1;*/
    const char *attack;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char buf3[MAX_STRING_LENGTH];
    int thesn;

    /* just in case */
    if (victim == ch || ch == NULL || victim == NULL)
        return;

    /*
     * Can't beat a dead char!
     * Guard against weird room-leavings.
     */
    if ( victim->position == POS_DEAD || ch->in_room != victim->in_room )
        return;

    /*
     * Figure out the type of damage message.
     */

    if(isShifted(ch))
    {
	form = get_shifted(ch);
	dt = TYPE_HIT + attack_lookup(form_table[form].attack_type);
    }

    wield = get_eq_char( ch, WEAR_WIELD );
	backfist_wield = get_eq_char( victim, WEAR_WIELD );
	backfist_dual = get_eq_char( victim, WEAR_DUAL_WIELD );
	backfist_shield = get_eq_char( victim, WEAR_SHIELD );
	backfist_hold = get_eq_char( victim, WEAR_HOLD );

    if ( dt == TYPE_UNDEFINED )
    {
        dt = TYPE_HIT;
        if ( wield != NULL && wield->item_type == ITEM_WEAPON )
            dt += wield->value[3];
        else
            dt += ch->dam_type;
    }
    if(is_affected(ch,gsn_executioner) && number_percent()<5)
    {
      act("$n suddenly darts forward with impossible agility and gracefully strikes you!",ch,0,victim,TO_VICT);
      act("$n suddenly darts forward with impossible agility and gracefully strikes $N!",ch,0,victim,TO_NOTVICT);
      act("Imbued with the grace of an executioner, you suddenly dart forward with impossible agility and gracefully strike $N!",ch,0,victim,TO_CHAR);
	exedam=GET_DAMROLL(ch)+140;
	exeresult = damage( ch, victim, exedam, gsn_executioner, DAM_HOLY, TRUE );
    }
    if(is_affected(victim,gsn_executioner) && number_percent()<7 && dt!=gsn_flurry && dt!=gsn_drum)
    {
        act("$n smoothly redirects $N's attack, and with lithe grace unleashes a deadly riposte!",victim,0,ch,TO_NOTVICT);
        act("You smoothly redirect $N's attack and with lithe grace unleash a deadly riposte!",victim,0,ch,TO_CHAR);
        act("$n smoothly redirects your attack and with lithe grace unleashes a deadly riposte!",victim,0,ch,TO_VICT);
	  exedam=GET_DAMROLL(ch)+100;
	  exeresult = damage( victim, ch, exedam, gsn_executioner, DAM_HOLY, TRUE );
    }


    if(get_skill(victim,gsn_backfist) !=0)
    {
		if (backfist_wield != NULL)
			handcounter += 1;
		if (backfist_dual != NULL)
			handcounter += 1;
		if (backfist_shield != NULL)
			handcounter += 1;
		if (backfist_hold != NULL)
			handcounter += 1;

		if (number_percent() < (get_skill(victim,gsn_backfist)/5) && handcounter < 2)
		{
			act("$n steps in quickly to strike $N with a quick backfist!",victim,0,ch,TO_NOTVICT);
			act("You step in quickly to strike $N with a quick backfist!",victim,0,ch,TO_CHAR);
			act("$n steps in quickly to strike you with a quick backfist!!",victim,0,ch,TO_VICT);
			exeresult = damage( victim, ch, GET_DAMROLL(victim)+75, gsn_backfist, DAM_BASH, TRUE );
			check_improve(victim,gsn_backfist,TRUE,1);
		}
		else
		{
			check_improve(victim,gsn_backfist,FALSE,1);
		}
    }

    if (dt < TYPE_HIT)
        if (wield != NULL)
            dam_type = attack_table[wield->value[3]].damage;
        else
            dam_type = attack_table[ch->dam_type].damage;
    else
        dam_type = attack_table[dt - TYPE_HIT].damage;

    if (dam_type == -1)
        dam_type = DAM_BASH;

    /* get the weapon skill */
    sn = get_weapon_sn(ch);
    skill = 20 + get_weapon_skill(ch,sn);

/* DUAL WIELD..kinda messy here */
    if (dt == gsn_dual_wield || dt == gsn_tertiary_wield)
    {
if (dt == gsn_dual_wield)
    wield = get_eq_char( ch, WEAR_DUAL_WIELD );
else
    wield = get_eq_char(ch,WEAR_TERTIARY_WIELD);
        tmp_dt = TYPE_HIT;
        if ( wield != NULL && wield->item_type == ITEM_WEAPON )
            tmp_dt += wield->value[3];
        else
            tmp_dt += ch->dam_type;

    if (tmp_dt < TYPE_HIT)
        if (wield != NULL)
            dam_type = attack_table[wield->value[3]].damage;
        else
            dam_type = attack_table[ch->dam_type].damage;
    else
        dam_type = attack_table[dt - TYPE_HIT].damage;

    if (dam_type == -1)
        dam_type = DAM_BASH;

    if (wield == NULL)		sn = -1;
    else switch (wield->value[0])
    {
	default:	sn = -1;	break;
	case (WEAPON_SWORD):	sn = gsn_sword;		break;
	case (WEAPON_DAGGER):	sn = gsn_dagger;	break;
	case (WEAPON_SPEAR):	sn = gsn_spear;		break;
	case (WEAPON_MACE):	sn = gsn_mace;		break;
	case (WEAPON_AXE):	sn = gsn_axe;		break;
	case (WEAPON_FLAIL):	sn = gsn_flail;		break;
	case (WEAPON_WHIP):	sn = gsn_whip;		break;
	case (WEAPON_POLEARM):	sn = gsn_polearm;	break;
    }

    skill = 10 + get_weapon_skill(ch,sn);

    }
    if(get_skill(victim,skill_lookup("blademaster"))>0 && !(cabal_down(ch,CABAL_PROTECTOR)))
    {
	tchance=3+((get_skill(victim,skill_lookup("blademaster"))-75)/3);
	if(is_affected(victim,AFF_HASTE))
		tchance+=2;
	if(is_affected(victim,AFF_SLOW))
		tchance-=5;
	if(number_percent()<tchance)
	{
      tmp_dt = TYPE_HIT;
      if (dt  == gsn_dual_wield)
    	  wield = get_eq_char(ch,WEAR_DUAL_WIELD);
      else if (dt == gsn_tertiary_wield)
	  wield = get_eq_char(ch,WEAR_TERTIARY_WIELD);
      else
	  wield = get_eq_char(ch,WEAR_WIELD);
	if (wield != NULL)
	  tmp_dt += wield->value[3];
      else
	  tmp_dt += ch->dam_type;
	if (wield != NULL && dt!=gsn_tertiary_wield)
	{
      if (tmp_dt > TYPE_HIT && tmp_dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
	  attack = attack_table[tmp_dt - TYPE_HIT].noun;
      else
      {
	  attack = attack_table[0].name;
      }
	  sprintf(buf1,"$n parries $N's %s, and skillfully unleashes an attack of $s own!",attack);
        act(buf1,victim,0,ch,TO_NOTVICT);
	  sprintf(buf2,"You parry $N's %s and skillfully unleash an attack of your own!",attack);
	  act(buf2,victim,0,ch,TO_CHAR);
        sprintf(buf3,"$n parries your %s and skillfully unleashes an attack of $s own!",attack);
	  act(buf3,victim,0,ch,TO_VICT);
	  thesn=skill_lookup("blademaster");
        bladam = number_range( wield->value[1], wield->value[2]);
	  bladam += GET_DAMROLL(ch)+(number_percent()/2);
	  exeresult = damage( victim, ch, bladam, thesn, DAM_SLASH, TRUE );
	  check_improve(victim,skill_lookup("blademaster"),TRUE,2);
          return;
	}
	}
    }

    /*
     * Calculate to-hit-armor-class-0 versus armor.
     */
    if ( IS_NPC(ch) )
    {
        thac0_00 = 20;
        thac0_32 = -4;   /* as good as a thief */
        if (IS_SET(ch->act,ACT_WARRIOR))
            thac0_32 = -10;
        else if (IS_SET(ch->act,ACT_THIEF))
            thac0_32 = -4;
        else if (IS_SET(ch->act,ACT_CLERIC))
            thac0_32 = 2;
        else if (IS_SET(ch->act,ACT_MAGE))
            thac0_32 = 6;
    }
    else
    {
        thac0_00 = class_table[ch->class].thac0_00;
        thac0_32 = class_table[ch->class].thac0_32;
    }
    thac0  = interpolate( ch->level + ch->drain_level, thac0_00, thac0_32 );

    if (thac0 < 0)
        thac0 = thac0/2;

    if (thac0 < -5)
        thac0 = -5 + (thac0 + 5) / 2;

    thac0 -= GET_HITROLL(ch) * skill/100;
    thac0 += 5 * (100 - skill) / 100;

    if (dt == gsn_backstab)
        thac0 -= 10 * (100 - get_skill(ch,gsn_backstab));

    if (dt == gsn_ambush)
	thac0 -= 15 * (100 - get_skill(ch,gsn_ambush));
    if (dt == gsn_impale)
	thac0 -= 15 * (100 - get_skill(ch,gsn_ambush));

    switch(dam_type)
    {
        case(DAM_PIERCE):victim_ac = GET_AC(victim,AC_PIERCE)/10;       break;
        case(DAM_BASH):  victim_ac = GET_AC(victim,AC_BASH)/10;         break;
        case(DAM_SLASH): victim_ac = GET_AC(victim,AC_SLASH)/10;        break;
        default:         victim_ac = GET_AC(victim,AC_EXOTIC)/10;       break;
    };

    if (victim_ac < -15)
        victim_ac = (victim_ac + 15) / 5 - 15;

    if ( !can_see( ch, victim ) )
        victim_ac -= 4;

    if ( victim->position < POS_FIGHTING)
        victim_ac += 4;

    if (victim->position < POS_RESTING)
        victim_ac += 6;

    /*
     * The moment of excitement!
     */
    while ( ( diceroll = number_bits( 5 ) ) >= 20 )
        ;

    if ( diceroll == 0
    || ( diceroll != 19 && diceroll < thac0 - victim_ac ) )
    {
        /* Miss. */
        damage( ch, victim, 0, dt, dam_type, TRUE );
        tail_chain( );
        return;
    }

    /*
     * Hit.
     * Calc damage.
     */
    if ( IS_NPC(ch) && (!ch->pIndexData->new_format || wield == NULL))
        if (!ch->pIndexData->new_format)
        {
            dam = number_range( ( ch->level + ch->drain_level) / 2, (ch->drain_level + ch->level) * 3 / 2 );
            if ( wield != NULL )
                dam += dam / 2;
        }
        else
            dam = dice(ch->damage[DICE_NUMBER],ch->damage[DICE_TYPE]);

    else
    {
        if (sn != -1)
            check_improve(ch,sn,TRUE,5);
        if ( wield != NULL )
        {
            if (wield->pIndexData->new_format)
                dam = dice(wield->value[1],wield->value[2]) * skill/100;
            else
                dam = number_range( wield->value[1] * skill/100,
                                wield->value[2] * skill/100);

            if (get_eq_char(ch,WEAR_SHIELD) == NULL)  /* no shield = more */
                dam = dam * 11/10;

	    if (wield->level - (ch->drain_level + ch->level) >= 35)
		dam = (dam*6)/10;
	    else if (wield->level - (ch->drain_level + ch->level) >= 25)
		dam = (dam*7)/10;
	    else if (wield->level - (ch->drain_level + ch->level) >= 15)
		dam = (dam*8)/10;
	    else if (wield->level - (ch->drain_level + ch->level) >= 5)
		dam = (dam*9)/10;

            /* sharpness! */
            if (IS_WEAPON_STAT(wield,WEAPON_SHARP))
            {
                int percent;

                if ((percent = number_percent()) <= (skill / 8))
                    dam = 2 * dam + (dam * 2 * percent / 100);
            }
        }
        else
            dam = number_range( 1 + 4 * skill/100, 2 * ch->level/3 * skill/100);
    }

    /*
     * Bonuses.
     */

	dam += (ch->drain_level + ch->level)/8;

    if ( get_skill(ch,gsn_enhanced_damage) > 0 )
    {
        diceroll = number_percent();
	if (get_skill(ch,gsn_enhanced_damage_two) > 0)
	{
	diceroll2 = number_percent();
	if (diceroll2 < get_skill(ch,gsn_enhanced_damage_two)
	&& diceroll2 < get_skill(ch,gsn_enhanced_damage) )
	{
	check_improve(ch,gsn_enhanced_damage_two,TRUE,5);
		diceroll2 = diceroll2 + 5;
		diceroll = diceroll2;
	}
	}
	if (get_skill(ch,gsn_enhanced_damage_three) > 0)
	{
            check_improve(ch,gsn_enhanced_damage_three,TRUE,3);
		diceroll += get_skill(ch,gsn_enhanced_damage_three)/3;
	}
        if (diceroll <= get_skill(ch,gsn_enhanced_damage))
        {
            check_improve(ch,gsn_enhanced_damage,TRUE,6);
            dam += (dam * diceroll/125);
        }
    }
    else
	{
	dam *= 7;
	dam /= 10;
	}

    if ( !IS_AWAKE(victim) )
        dam *= 2;
     else if (victim->position < POS_FIGHTING)
        dam = dam * 3 / 2;

    if ( dt == gsn_backstab && wield != NULL)
	{
        if ( wield->value[0] != 2 )
 	    dam *= 2 + ((ch->level + ch->drain_level) / 10) ;
        else
            dam *= 2 + ((ch->drain_level + ch->level) / 8);
        }

    if ( dt == gsn_ambush )
	dam *= 5/2 + ((ch->drain_level + ch->level) / 6 );


    mdam = GET_DAMROLL(ch);

/* Let's cut back on mage's damage a bit */

    dam += mdam * UMIN(100,skill) /100;

    if (!IS_NPC(ch))
    if (!str_cmp(class_table[ch->class].name,"monk"))
    {
	if (wield == NULL
	&& get_eq_char(ch,WEAR_DUAL_WIELD) == NULL)
	dam += (2*(ch->drain_level + ch->level)/5);
    }

    if (IS_NPC(victim) && victim->pIndexData->vnum == MOB_VNUM_HYDRA)
    {
	if (wield != NULL && wield->pIndexData->vnum == OBJ_VNUM_HYDRA_SLAYER)
		dam = dice(17,50);
    }

    if (IS_NPC(victim) && victim->pIndexData->vnum == MOB_VNUM_STALKER)
    {
	dam *= .6;
    }

    bcry = battlecry_multiplier(ch,dt);
    if (wield != NULL && wield->pIndexData->vnum == 23265)
	    if (check_cutter(ch,victim) > 1)
			dam += dice(10,3);

    if (bcry == 1 && wield == NULL)
    {
     if (check_palm(ch))
     {
	act("$n strikes a powerful open handed blow!",ch,0,0,TO_ROOM);
	act("You strike a powerful open handed blow!",ch,0,0,TO_CHAR);
	if ((ch->drain_level + ch->level) <= 39)
	{
	dam *= 3;
	dam /= 2;
	}
	else
	dam *= 2;
     }
    }

    if (bcry == 1 && ch->cabal != CABAL_RAVAGER)
	{
	if ((obj = get_eq_char(ch,WEAR_HANDS)) != NULL
	&& obj->pIndexData->vnum == 29711
	&& number_percent() < 10)
	{
	act("$n delivers a blow of deadly force!",ch,0,0,TO_ROOM);
	act("You deliver a blow of deadly force!",ch,0,0,TO_CHAR);
	bcry = 2;
	}
	}
    dam *= bcry;

    if ( (get_skill(ch,gsn_cblow) > 0) || (ch->level < skill_table[gsn_cblow].skill_level[ch->class]) )
	{
	  	if  ((wield != NULL)
			&& ((wield->value[0] == WEAPON_STAFF) || (wield->value[0] == WEAPON_AXE))
			&& (number_percent() < get_skill(ch,gsn_cblow)/3) )
		{
		    act("$n lands a crushing blow!",ch,0,0,TO_ROOM);
			act("You land a crushing blow!",ch,0,0,TO_CHAR);
			dam *=1.5;
			check_improve(ch,gsn_cblow,TRUE,2);
		}
	}

	if (wield != NULL && IS_WEAPON_STAT(wield,WEAPON_VORPAL) && number_percent() < 20)
	{
		act("$p emits a fierce red glow as it lashes violently at $N!",ch,wield,victim,TO_CHAR);
		act("$p emits a fierce red glow as it lashes violently at You!",ch,wield,victim,TO_VICT);
		act("$p emits a fierce red glow as it lashes violently at $N!",ch,wield,victim,TO_NOTVICT);
		dam *= 1.5;
	}

    if (wield != NULL && wield->pIndexData->vnum == OBJ_VNUM_AVENGER)
    {
	if (IS_WEAPON_STAT(wield,WEAPON_SHADOWBANE)
	&& number_percent() < 7)
	{
	act("$p flares and sears $N!",ch,wield,victim,TO_CHAR);
	act("$p flares and sears you!",ch,wield,victim,TO_VICT);
	act("$p flares and sears $N!",ch,wield,victim,TO_NOTVICT);
	dam *= 2;
	}
	if (IS_WEAPON_STAT(wield,WEAPON_LIGHTBRINGER)
	&& number_percent() < 10)
	{
	act("$p flares and sears $N!",ch,wield,victim,TO_CHAR);
	act("$p flares and sears you!",ch,wield,victim,TO_VICT);
	act("$p flares and sears $N!",ch,wield,victim,TO_NOTVICT);
	dam *= 3;
	dam /=2;
	}

	}

if (bcry == 1)
{
    if (wield != NULL && wield->pIndexData->vnum == 24309)
    {
	if (number_percent() < 6)
	{
	act("$n's $p lands a crushing blow!",ch,wield,0,TO_ROOM);
	act("Your $p lands a crushing blow!",ch,wield,0,TO_CHAR);
	dam *= 2;
	}
    }

/* sword of planes  and mace disruption .. go right before damage report*/
        if (wield != NULL && wield->pIndexData->vnum == OBJ_VNUM_SWORD_PLANES)
        {
        if (IS_NPC(victim)
        && ( IS_SET(victim->act,ACT_UNDEAD)
        || victim->pIndexData->vnum == MOB_VNUM_FIRE_ELEMENTAL
        || victim->pIndexData->vnum == MOB_VNUM_AIR_ELEMENTAL
        || victim->pIndexData->vnum == MOB_VNUM_WATER_ELEMENTAL
        || victim->pIndexData->vnum == MOB_VNUM_EARTH_ELEMENTAL
        || victim->pIndexData->vnum == MOB_VNUM_DRAGON
        || victim->pIndexData->vnum == MOB_VNUM_UNICORN
	|| is_name(victim->name,"demon")
	|| is_name(victim->name,"familiar")
        || is_name(victim->name,"golem") ) )
        {
        if (!(saves_spell(ch->level,victim,DAM_ENERGY))
        && (!saves_spell(ch->level + 3,victim,DAM_ENERGY))
        && (!saves_spell(wield->level,victim,DAM_OTHER))
	&& number_percent() < 50)
                {
act("$n's body suddenly crumples as $p draws $s lifeforce back to the outer planes!",
victim,wield,0,TO_ROOM);
raw_kill(ch,victim);
        return;
                }
        else if (IS_SET(victim->affected_by, AFF_SANCTUARY) )
                {
                dam += (dam/2);  /* partiallyNegates sanc */
                }
        }
        }
        else if (wield != NULL && wield->pIndexData->vnum == OBJ_VNUM_MACE_DISRUPTION)
        {
        if (IS_NPC(victim)
        && ( IS_SET(victim->act,ACT_UNDEAD)) )
        {
        if (!saves_spell(ch->level,victim,DAM_ENERGY)
        && !saves_spell(ch->level,victim,DAM_ENERGY)
        && !saves_spell(ch->level,victim,DAM_OTHER))
        {
        act("$N's body explodes under $n's mace of disruption!",ch,0,victim,TO_NOTVICT);
        act("$N's body explodes under your mace of disruption!",ch,0,victim,TO_CHAR);
        raw_kill(ch,victim);
        return;
        }
        else if (IS_SET(victim->affected_by,AFF_SANCTUARY))
                {
                        dam += (3* dam/4);
                }
        }
    }

    if (IS_NPC(ch)
	&& ch->pIndexData->vnum == 19000
	&& number_percent() < 4)
	{
act("$n bellows a cry and unleashes a mighty blow upon you!",ch,0,victim,TO_VICT);
act("$n bellows a cry and unleashes a mighty blow upon $N",ch,0,victim,TO_NOTVICT);
dam += dice(ch->level,3);
}
}

    if ( dam <= 0 )
        dam = 1;
    if((get_skill(ch,skill_lookup("truestrike")))>0 && (number_percent()<((get_skill(ch,skill_lookup("truestrike"))-40)/5)-3) && !(cabal_down(ch,CABAL_PROTECTOR)))
    {
      act("$n unleashes a blow of true accuracy, striking through your defenses!",ch,0,victim,TO_VICT);
      act("$n unleashes a blow of true accuracy, striking through $N's defenses!",ch,0,victim,TO_NOTVICT);
      act("You unleash a blow of true accuracy, striking through $N's defenses!",ch,0,victim,TO_CHAR);
	dam*=1;
	result = damage( ch, victim, dam, dt, DAM_TRUESTRIKE, TRUE );
      check_improve(ch,skill_lookup("truestrike"),TRUE,1);
    }
    else
    {
    result = damage( ch, victim, dam, dt, dam_type, TRUE );
    }
    check_outlaw_badge(ch);
    check_red_shield(ch);
    check_ceial_tat(ch);

    if (wield != NULL && wield->pIndexData->vnum == OBJ_VNUM_CLOWN_MALLET)
        check_clown_mallet(ch->level,ch,ch->fighting, wield);
    if (wield != NULL && wield->pIndexData->vnum == OBJ_VNUM_RANGER_STAFF)
	check_ranger_staff(ch,wield);
    if (wield != NULL && wield->pIndexData->vnum == OBJ_VNUM_DRAGON_WEAPON && dt!=gsn_tertiary_wield)
	golden_weapon(ch,wield);
    if (wield != NULL && wield->pIndexData->vnum == 8003)
	check_ranger_staff(ch,wield);	/* Staff of magi call */

if(isShifted(ch))
	{
		form = get_shifted(ch);
		dam = dice(form_table[form].dice_num, form_table[form].dice_type);
		dam += form_table[form].dice_add;

	}
    /* but do we have a funky weapon? */
    if (result && wield != NULL)
    {
        int dam;

        if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_POISON))
        {
            int level;
            AFFECT_DATA *poison, af;

            if ((poison = affect_find(wield->affected,gsn_poison)) == NULL)
                level = wield->level;
            else
                level = poison->level;

            if (!saves_spell(level / 2,victim,DAM_POISON))
            {
                send_to_char("You feel poison coursing through your veins.",
                    victim);
                act("$n is poisoned by the venom on $p.",
                    victim,wield,NULL,TO_ROOM);

		init_affect(&af);
                af.where     = TO_AFFECTS;
		af.aftype = AFT_SPELL;
                af.type      = gsn_poison;
                af.level     = level * 3/4;
                af.duration  = level / 2;
                af.location  = APPLY_STR;
                af.modifier  = -1;
                af.bitvector = AFF_POISON;
                affect_join( victim, &af );
            }

            /* weaken the poison if it's temporary */
            if (poison != NULL)
            {
                poison->level = UMAX(0,poison->level - 2);
                poison->duration = UMAX(0,poison->duration - 1);

                if (poison->level == 0 || poison->duration == 0)
                    act("The poison on $p has worn off.",ch,wield,NULL,TO_CHAR);
            }
        }


        if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_VAMPIRIC)
	&& number_percent() < 8)
        {
            dam = number_range(wield->level/2, wield->level);
            act("$p draws life from $n.",victim,wield,NULL,TO_ROOM);
            act("You feel $p drawing your life away.",
                victim,wield,NULL,TO_CHAR);
            damage_old(ch,victim,dam,0,DAM_NEGATIVE,FALSE);
            ch->hit = UMIN(ch->hit + dam, ch->max_hit);
        }

        if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_FLAMING)
	&& number_percent() < 15)
        {
            dam = number_range(1,wield->level / 4 + 1);
            act("$n is burned by $p.",victim,wield,NULL,TO_ROOM);
            act("$p sears your flesh.",victim,wield,NULL,TO_CHAR);
            fire_effect( (void *) victim,wield->level/2,dam,TARGET_CHAR);
            damage(ch,victim,dam,0,DAM_FIRE,FALSE);
        }

        if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_FROST)
	&& number_percent() < 15)
        {
            dam = number_range(1,wield->level / 6 + 2);
            act("$p freezes $n.",victim,wield,NULL,TO_ROOM);
            act("The cold touch of $p surrounds you with ice.",
                victim,wield,NULL,TO_CHAR);
            cold_effect(victim,wield->level/2,dam,TARGET_CHAR);
            damage(ch,victim,dam,0,DAM_COLD,FALSE);
        }

        if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_SHOCKING)
	&& number_percent() < 15)
        {
            dam = number_range(1,wield->level/5 + 2);
            act("$n is struck by lightning from $p.",victim,wield,NULL,TO_ROOM);
            act("You are shocked by $p.",victim,wield,NULL,TO_CHAR);
            shock_effect(victim,wield->level/2,dam,TARGET_CHAR);
            damage(ch,victim,dam,0,DAM_LIGHTNING,FALSE);
        }

    }
    tail_chain( );
    return;
}


void check_red_shield(CHAR_DATA *ch)
{
	OBJ_DATA *shield;
	shield = get_eq_char(ch,WEAR_SHIELD);
	if (shield == NULL)
	return;
	if (shield->pIndexData->vnum != 23728)
		return;
	if (number_percent() < 6)
	{
	act("The cross on $n's shield glows a soft red.",ch,shield,0,TO_ROOM);
	act("The cross on your shield glows a soft red.",ch,shield,0,TO_CHAR);
	obj_cast_spell(skill_lookup("cure critical"), 45, ch, ch, shield);
	}
	return;
}

void check_outlaw_badge(CHAR_DATA *ch)
{
	OBJ_DATA *badge;

    	badge = get_eq_char(ch,WEAR_NECK_1);
	if (badge == NULL)
		badge = get_eq_char(ch,WEAR_NECK_2);

    if (badge == NULL)
	return;
    if (badge->pIndexData->vnum == OBJ_VNUM_BADGE
	&& number_percent() < 8)
	{
	act("$n's $p glows bright blue!",ch,badge,0,TO_ROOM);
	act("Your $p glows bright blue!",ch,badge,0,TO_CHAR);
	obj_cast_spell(skill_lookup("cure critical"), badge->level, ch, ch, badge);
	}
    return;
}

void check_ceial_tat(CHAR_DATA *ch)
{
	OBJ_DATA *tattoo;

    tattoo = get_eq_char(ch,WEAR_BRAND);
    if (tattoo == NULL)
	return;
    if (tattoo->pIndexData->vnum == 72
	&& number_percent() < 3)
	{
	act("$n's $p begins to suck life from his opponent!",ch,tattoo,0,TO_ROOM);
	act("Your $p begins to suck the life out of your opponent!",ch,tattoo,0,TO_CHAR);
	obj_cast_spell(skill_lookup("cure critical"), tattoo->level, ch, ch, tattoo);
	}
    return;
}

void check_clown_mallet(int level, CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *wield)
{
   AFFECT_DATA af;

	if (number_percent() < 2)
        {
	if (wield->pIndexData->vnum == OBJ_VNUM_CLOWN_MALLET)
        {
        act("$p begins to eminate a haunting tune.",ch,wield,0,TO_ROOM);
        act("$p puts your foe to sleep!",ch,wield,0,TO_CHAR);
        }
        	init_affect(&af);
	        af.where     = TO_AFFECTS;
                af.type      = gsn_clown_mallet;
                af.aftype    = AFT_SKILL;
                af.level     = ch->level;
                af.duration  = 2;
                af.location  = 0;
                af.modifier  = 0;
                af.bitvector = AFF_SLEEP;
                affect_to_char( victim, &af );
		stop_fighting( victim, TRUE);
		stop_fighting( ch, TRUE);
		victim->position = POS_SLEEPING;
		ch->position = POS_STANDING;
	 }
    return;
}
/*
void check_clown_mallet(int level,CHAR_DATA *ch,CHAR_DATA *victim, OBJ_DATA *wield)
{
  AFFECT_DATA af;

	if (wield->pIndexData->vnum == OBJ_VNUM_CLOWN_MALLET)
  	{
	act("$n lands a devasating blow!",ch,wield,0,TO_ROOM);
	act("You land a devastating blow!",ch,wield,0,TO_CHAR);

		af.where     = TO_AFFECTS;
                af.type      = gsn_sleep;
                af.level     = ch->level;
                af.duration  = level / 2;
                af.location  = 0;
                af.modifier  = 0;
                af.bitvector = AFF_SLEEP;
                affect_to_char( victim, &af );
         }
    return;
}
*/
void check_ranger_staff(CHAR_DATA *ch,OBJ_DATA *wield)
{
	if (number_percent() < 7)
	{
	if (wield->pIndexData->vnum == OBJ_VNUM_RANGER_MAGIC)
  	{
	act("$p glows softly.",ch,wield,0,TO_ROOM);
	act("Your body warms as your staff restores you!",ch,wield,0,TO_CHAR);
	}
	else
	{
	act("$p glows a burning green!",ch,wield,0,TO_ROOM);
	act("Your staff of the magi glows burning green!",ch,wield,0,TO_CHAR);
	}

      ch->hit = UMIN( ch->hit + 75, ch->max_hit );
        }
    return;
}

void golden_weapon(CHAR_DATA *ch,OBJ_DATA *wield)
{
	if (number_percent() < 7  && ch->in_room->vnum!=ROOM_FORCE_DUEL)
	{
	act("$n's weapon glows brilliant gold.",ch,wield,0,TO_ROOM);
	act("Your body warms as your weapon heals you!",ch,wield,0,TO_CHAR);
        ch->hit = UMIN( ch->hit + 75, ch->max_hit );
	}

    return;
}

int avenger_multiplier(CHAR_DATA *ch,OBJ_DATA *wield)
{
    int avenge;

    avenge = 1;
    if (IS_EVIL(ch->fighting) && wield != NULL)
	{
	if (number_percent() < 7 && IS_WEAPON_STAT(wield,WEAPON_SHADOWBANE))
		avenge = 2;
	if (number_percent() < 7 && IS_WEAPON_STAT(wield,WEAPON_LIGHTBRINGER))
	avenge = 3/2;
	}

    return avenge;
}

int battlecry_multiplier(CHAR_DATA *ch, int dt)
{
    OBJ_DATA *obj;
    int battlecry;
    int chance;
    int mod;
    battlecry = 1;
    if (IS_NPC(ch))
	return 1;
    if (!is_affected(ch,gsn_battlecry) && !IS_NPC(ch))
	return 1;
    if (cabal_down(ch,CABAL_RAVAGER))
	return 1;
    if (IS_NPC(ch) && ch->pIndexData->vnum != MOB_VNUM_RAVAGER
	&& !is_affected(ch,gsn_battlecry))
	return battlecry;
    if (dt==gsn_flurry || dt==gsn_drum)
	return 1;
    if ((ch->drain_level + ch->level) < skill_table[gsn_battlecry].skill_level[ch->class]
	&& !IS_NPC(ch))
	return 1;

    if  ((obj = get_eq_char(ch,WEAR_HANDS)) != NULL
	&& obj->pIndexData->vnum == 29711)
	mod = 3;
	else mod = 0;
    if  (number_percent( ) > (mod + 14))
	return 1;

    chance = get_skill(ch,gsn_battlecry);
    if (IS_NPC(ch) )
	chance = 100;

    if (chance > 2)
    {
    if (number_percent( ) > (chance - 10) )
    	{
	send_to_char("You fail to deliver a blow of deadly force.\n\r",ch);
	if (ch->pcdata->learned[gsn_battlecry] < 100)
	{
send_to_char("You learn from your mistakes and become better at battlecry.\n\r",ch);
	ch->pcdata->learned[gsn_battlecry]++;
	}
    	}
    else
    	{
	send_to_char("You deliver a blow of deadly force!\n\r",ch);
	act("$n delivers a blow of deadly force!",ch,NULL,NULL,TO_ROOM);
	check_improve(ch,gsn_battlecry,TRUE,1);
    	if ((ch->drain_level + ch->level) <= 29) 	battlecry = 2;
   else if ((ch->drain_level + ch->level) <= 39) 	battlecry = 3;
   else if ((ch->drain_level + ch->level) <= 49)	battlecry = 4;
   else if ((ch->drain_level + ch->level) <= 59) 	battlecry = 5;
   else battlecry = 5;
	}
    }

    if (dt == gsn_circle)
	battlecry = UMIN(battlecry,3);
    if (dt == gsn_lunge)
	battlecry = UMIN(battlecry,3);
    if (dt == gsn_backstab)
	battlecry = UMIN(battlecry,2);
    if (dt == gsn_dual_backstab)
	battlecry = UMIN(battlecry,2);
    if (dt == gsn_ambush)
	battlecry = UMIN(battlecry,2);
    if (dt == gsn_flurry)
	battlecry = 3;
    if (dt == gsn_drum)
	battlecry = 3;

    if (cabal_down(ch,CABAL_RAVAGER))
	battlecry = 1;

    return battlecry;
}



/*
 * Inflict damage from a hit.
 */
bool damage(CHAR_DATA *ch,CHAR_DATA *victim,int dam,int dt,int dam_type,
            bool show)
{
    OBJ_DATA *corpse;
    bool immune;
    OBJ_DATA *obj;
    OBJ_DATA *wield;
    bool kineticresult;
    int kineticdam;
    int originaldam;

    if ( victim->position == POS_DEAD )
        return FALSE;

    if (IS_NPC(ch) && spellbaned(ch,victim,dt))
	return FALSE;

    if (is_safe(ch,victim))
	return FALSE;

    originaldam=dam;
    /*
     * Stop up any residual loopholes.
     */
    if ( dam > 1500 && dt >= TYPE_HIT)
    {
        dam = 1500;

    }
    if ( victim != ch )
    {
        /*
         * Certain attacks are forbidden.
         * Most other attacks are returned.
         */
        if ( is_safe( ch, victim ) )
            return FALSE;
        check_killer( ch, victim );


        if ( victim->position > POS_STUNNED )
        {
            if ( victim->fighting == NULL )
            {
                set_fighting( victim, ch );
                if ( IS_NPC( victim ) && HAS_TRIGGER( victim, TRIG_KILL ) )
                    mp_percent_trigger( victim, ch, NULL, NULL, TRIG_KILL );
            }
            if (victim->timer <= 4)
                victim->position = POS_FIGHTING;
        }

        if ( victim->position > POS_STUNNED )
        {
            if ( ch->fighting == NULL )
                set_fighting( ch, victim );
        }

        /*
         * More charm stuff.
         */
        if ( victim->master == ch )
            stop_follower( victim );
    }

    if ( (dt >= TYPE_HIT || dt == gsn_dual_wield || dt == gsn_tertiary_wield)
	&& ch != victim)
    {


	if ( check_spin(ch,victim, dt))
            return FALSE;
        if ( check_parry( ch, victim, dt ) )
        {
		do_counter_parry(ch,victim,dt, dam);
		return FALSE;
	}
        if ( check_distance( ch, victim, dt ) )
            return FALSE;
	if ( check_block( ch, victim, dt) )
		return FALSE;
        if ( check_dodge( ch, victim, dt ) )
            return FALSE;
        if ( check_questdodge( ch, victim, dt ) )
            return FALSE;
        if ( check_shield_block(ch,victim, dt))
            return FALSE;
        if ( check_defiant_absorb(ch,victim,dt))
        	return FALSE;
        if (check_displacement( ch, victim))
	    return FALSE;
        if ( check_ironhands( ch, victim, dt ) )
			return FALSE;
		if ( check_unarmed_defense( ch, victim, dt ) )
			return FALSE;
	}
   //stuff for elementalist
   if (is_affected(victim,gsn_static_charge))
   {
	affect_strip(victim,gsn_static_charge);
	damage_old(victim,ch,victim->level * number_range(6,8),gsn_static_charge,DAM_LIGHTNING,TRUE);
   }
/* damage reduction */
    if (dam > 85)
    {
	dam = (dam - 85);
	dam *= 7;
	dam /= 10;
	dam += 85;
    }
    if (is_affected(victim,gsn_fireshield) && dt!=gsn_kinetic_shield && dam_type!=DAM_TRUESTRIKE)
	dam *= .65;
    if (is_affected(victim,gsn_wraithform) && dt!=gsn_kinetic_shield && dam_type!=DAM_TRUESTRIKE)
	dam *= .65;
    if (is_affected(victim,gsn_stoneskin) && dt!=gsn_kinetic_shield && dam_type!=DAM_TRUESTRIKE)
	dam *= .8;
    if (is_affected(victim,gsn_shield) && dt!=gsn_kinetic_shield && dam_type!=DAM_TRUESTRIKE)
	dam *= .8;
    if (is_affected(victim,gsn_kinetic_shield) && dt!=gsn_kinetic_shield && dam_type!=DAM_TRUESTRIKE)
	dam *= .75;
    if (is_affected(victim,gsn_barrier) && dt!=gsn_barrier && dam_type!=DAM_TRUESTRIKE)
	dam *= .50;

    if (is_affected(victim,gsn_shackles))
	dam *= 1.5;
    if (is_affected(victim, gsn_soften))
	dam *= 1.3;

    if (is_affected(ch,gsn_rage))
        dam *= 1.25;

    un_camouflage( ch, NULL);
    un_earthfade(ch,NULL);
    un_hide(ch, NULL);
    un_invis(ch, NULL);
    un_sneak(ch, NULL);
    un_duo(ch, NULL);
    /*
     * Damage modifiers.
     */
    //elemental shields
/*
    if (is_affected(victim, gsn_shield_of_flames))
	dam *= .9;
    if (is_affected(victim, gsn_shield_of_frost))
	dam *= .88;
    if (is_affected(victim, gsn_shield_of_bubbles))
	dam *= .86;
    if (is_affected(victim, gsn_shield_of_dust))
	dam *= .83;  //slightly more defensive
    if (is_affected(victim, gsn_shield_of_lightning))
	dam *= .86;
    if (is_affected(victim, gsn_shield_of_wind))
	dam *= .86;
	*/
    //orduno brand stuff
    if (is_affected(victim, gsn_body_of_wood))
    {
	dam /= 10;
	dam *= 9;
    }
    if (is_affected(victim, gsn_body_of_clay))
    {
	dam /= 10;
	dam *= 8;
    }
    if (is_affected(victim, gsn_body_of_iron))
    {
	dam /= 10;
	dam *= 7;
    }
    if (is_affected(victim, gsn_body_of_steel))
    {
	dam /= 10;
	dam *= 6;
    }
    if ((is_affected(victim, gsn_body_of_stone)) && (!is_affected(victim, gsn_ressist))
    && (!IS_AFFECTED(victim, AFF_SANCTUARY)))
    {
	dam /= 10;
	dam *= 5;
    }
    if (is_affected(victim, gsn_body_of_diamond))
    {
	dam /= 10;
	dam *= 4;
    }

    if ( dam > 1 && !IS_NPC(victim)
    &&   victim->pcdata->condition[COND_DRUNK]  > 10 )
        dam = 9 * dam / 10;

    if ( dam > 1 && IS_AFFECTED(victim, AFF_SANCTUARY) && dt!=gsn_kinetic_shield && dam_type!=DAM_TRUESTRIKE)
    {
    if ( (dt >= TYPE_HIT || dt == gsn_dual_wield || dt == gsn_tertiary_wield))
	{
	if (dt == gsn_dual_wield)
		wield = get_eq_char(ch,WEAR_DUAL_WIELD);
	else if (dt == gsn_tertiary_wield)
	wield = get_eq_char(ch,WEAR_TERTIARY_WIELD);
  	else
	wield = get_eq_char(ch,WEAR_WIELD);
	if (wield == NULL )
		dam /= 2;
	else if (wield->pIndexData->vnum != 29724)
        dam /= 2;
	}
    else
	dam /= 2;
    }

    if (is_affected(victim,gsn_shroud))
	dam *= 1;

    if (is_affected(victim,gsn_steel_nerves))
	dam *= .9;

    obj = get_eq_char(ch,WEAR_HEAD);
    if (obj != NULL
    && obj->pIndexData->vnum == OBJ_VNUM_TIARA
    && (dam_type == DAM_COLD || dam_type == DAM_FIRE
	|| dam_type == DAM_HOLY || dam_type == DAM_NEGATIVE
    || dam_type == DAM_ACID) )
	{
    if (dam > 125)
    {
	dam = (dam - 125);
	dam *= 6;
	dam /= 10;
	dam += 125;
    }
	}

    if (check_crown_blood(ch,victim,dt,dam,dam_type))
	return FALSE;

    if ((dt >= TYPE_HIT || dt==gsn_backstab || HITS) && (ch != victim) && (is_affected(victim,gsn_steel_nerves)) && dam > 1  && dt!=gsn_kinetic_shield && dam_type!=DAM_TRUESTRIKE)
    {
	dam *= .8;
    }

    if ((dt >= TYPE_HIT || dt==gsn_backstab || HITS) && (ch != victim)
	&& dam > 1 && dam_type!=DAM_TRUESTRIKE)
    {
	dam -= dam*3/10;
    }

    if ( dam > 1 && ((IS_AFFECTED(victim, AFF_PROTECT_EVIL) && IS_EVIL(ch) )
    ||               (IS_AFFECTED(victim, AFF_PROTECT_GOOD) && IS_GOOD(ch) )) && dt!=gsn_kinetic_shield && dam_type!=DAM_TRUESTRIKE)
        dam -= dam / 4;

    if (is_affected(victim,gsn_spellbane) && dt!=gsn_kinetic_shield && dam_type!=DAM_TRUESTRIKE)
    {
	if (dam_type > 3)
		{
		dam *= 7;
		dam /= 10;
		}
    }

if (isShifted(victim))
	dam *= (victim->dam_reduc * 0.01);

    immune = FALSE;

    /* Check for absorb */
    if (check_absorb(ch,victim,dt))
	return FALSE;
    if (eye_of_beholder_absorb(ch,victim,dt,dam,dam_type))
	return FALSE;

    /*
     * Check for parry, and dodge.
     */

    switch(check_immune(victim,dam_type))
    {
        case(IS_IMMUNE):
            immune = TRUE;
            dam = 0;
            break;
        case(IS_RESISTANT):
	    if (IS_NPC(victim))
        	dam -= dam/3;
	    else
		dam -= dam/3;
            break;
        case(IS_VULNERABLE):
            dam += dam/3;
            break;
    }

    if (dt == gsn_dual_wield)
	wield = get_eq_char(ch,WEAR_DUAL_WIELD);
    else if ( dt == gsn_tertiary_wield )
	wield = get_eq_char(ch,WEAR_TERTIARY_WIELD);
    else wield = get_eq_char(ch,WEAR_WIELD);

    if (show)
        dam_message( ch, victim, dam, dt, immune );
   if ( (dt >= TYPE_HIT || dt == gsn_dual_wield || dt == gsn_tertiary_wield || dt == gsn_drum || dt==gsn_flurry)
	&& ch != victim)
    {
     if (is_affected(victim,gsn_kinetic_shield) && dam>0)
	{
	  if(number_percent()<67 || dam>150) {
        act("$n's kinetic shield reacts to $N's attack!",victim,0,ch,TO_NOTVICT);
        act("Your kinetic shield reacts to $N's attack!",victim,0,ch,TO_CHAR);
        act("$n's kinetic shield reacts to your attack!",victim,0,ch,TO_VICT);
	  kineticdam=originaldam*.15;
	  kineticresult = damage_old( victim, ch, kineticdam, gsn_kinetic_shield, DAM_ENERGY, TRUE );
	  }}
     if (is_affected(victim,gsn_self_immolation) && dam>0)
     {
	act("$N is burned by the flames surrounding $n!",victim,NULL,ch,TO_NOTVICT);
	act("$N is burned by the flames surrounding you!",victim,NULL,ch,TO_CHAR);
	act("You are burned by the flames engulfing $n!",victim,NULL,ch,TO_VICT);
	damage_old(victim,ch,number_range(30,100),gsn_flames,DAM_FIRE,
TRUE);
     }

    }
    if (dam == 0)
        return FALSE;

    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */
    victim->hit -= dam;
    if (dam_type == DAM_FIRE || dam_type == DAM_ACID)
	victim->noregen_dam += dam;
    if (victim->noregen_dam > victim->max_hit)
	victim->noregen_dam = victim->max_hit;

    if ( !IS_NPC(victim)
    &&   victim->level >= LEVEL_IMMORTAL
    &&   victim->hit < 1 )
        victim->hit = 1;
    update_pos( victim );

    switch( victim->position )
    {
    case POS_MORTAL:
        act( "$n is mortally wounded, and will die soon, if not aided.",
            victim, NULL, NULL, TO_ROOM );
        send_to_char(
            "You are mortally wounded, and will die soon, if not aided.\n\r",
            victim );
        break;

    case POS_INCAP:
        act( "$n is incapacitated and will slowly die, if not aided.",
            victim, NULL, NULL, TO_ROOM );
        send_to_char(
            "You are incapacitated and will slowly die, if not aided.\n\r",
            victim );
        break;

    case POS_STUNNED:
        act( "$n is stunned, but will probably recover.",
            victim, NULL, NULL, TO_ROOM );
        send_to_char("You are stunned, but will probably recover.\n\r",
            victim );
        break;

    case POS_DEAD:
        act( "$n is DEAD!!", victim, 0, 0, TO_ROOM );
        send_to_char( "You have been KILLED!!\n\r\n\r", victim );
        break;

    default:
        if ( dam > victim->max_hit / 4 )
            send_to_char( "That really did HURT!\n\r", victim );
        if ( victim->hit < victim->max_hit / 4 )
            send_to_char( "You sure are BLEEDING!\n\r", victim );
        break;
    }

    /*
     * Sleep spells and extremely wounded folks.
     */
        if ( !IS_AWAKE(victim) && ch != victim)
        stop_fighting( victim, FALSE );


    /*
     * Payoff for killing things.
     */
    if ( victim->position == POS_DEAD )
    {
        group_gain( ch, victim );


        if ( !IS_NPC(victim) )
        {
            sprintf( log_buf, "%s killed by %s at %d",
                victim->name,
                (IS_NPC(ch) ? ch->short_descr : ch->name),
                ch->in_room->vnum );
            log_string( log_buf );
	  }

            /*
             * Dying penalty:
             */

        raw_kill(ch, victim );


        /*
         * Death trigger
         */
        if ( IS_NPC( victim ) && HAS_TRIGGER( victim, TRIG_DEATH) )
        {
            victim->position = POS_STANDING;
            mp_percent_trigger( victim, ch, NULL, NULL, TRIG_DEATH );
        }
      if (!IS_NPC(ch)
        &&  (corpse = get_obj_list(ch,"corpse",ch->in_room->contents)) != NULL
        &&  corpse->item_type == ITEM_CORPSE_NPC && can_see_obj(ch,corpse))
        {
            OBJ_DATA *coins;

            corpse = get_obj_list( ch, "corpse", ch->in_room->contents );
            if ( IS_SET(ch->act, PLR_AUTOLOOT) &&
                 corpse && corpse->contains) /* exists and not empty */
                do_get( ch, "all corpse" );

            if (IS_SET(ch->act,PLR_AUTOGOLD) &&
                corpse && corpse->contains  && /* exists and not empty */
                !IS_SET(ch->act,PLR_AUTOLOOT))
                if ((coins = get_obj_list(ch,"gcash",corpse->contains)) != NULL)
                    do_get(ch, "all.gcash corpse");

            if ( IS_SET(ch->act, PLR_AUTOSAC) )
	{
              if ( IS_SET(ch->act,PLR_AUTOLOOT) && corpse && corpse->contains)
                return TRUE;  /* leave if corpse has treasure */
              else
                do_sacrifice( ch, "corpse" );
        }
	}
        return TRUE;
    }

    if ( victim == ch )
        return TRUE;

    /*
     * Take care of link dead people.
     */
    if ( !IS_NPC(victim) && victim->desc == NULL )
    {
        if ( number_range( 0, victim->wait ) == 0 )
        {
            do_recall( victim, "" );
            return TRUE;
        }
    }

    /*
     * Wimp out?
     */
    /*if ( IS_NPC(victim) && dam > 0 && victim->wait < PULSE_VIOLENCE / 2)
    {
        if ( ( IS_SET(victim->act, ACT_WIMPY) && number_bits( 2 ) == 0
        &&   victim->hit < victim->max_hit / 5)
        ||   ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL
        &&     victim->master->in_room != victim->in_room)
        && !is_centurion(victim) )
            do_flee( victim, "" );
    }*/

    if(ch->in_room != victim->in_room || dt == gsn_parting_blow)
    return TRUE;

    if ( IS_NPC(victim) && dam > 0 && victim->wait < PULSE_VIOLENCE / 2)
    {
        if ( (( IS_SET(victim->act, ACT_WIMPY) && number_bits( 2 ) == 0
        &&   victim->hit < victim->max_hit / 5))
        ||   (( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL
        &&     victim->master->in_room != victim->in_room
 	      && !is_centurion(victim) )
        && dt != gsn_parting_blow))
            do_flee( victim, "" );
    }


    if (victim->level >= 30)
    {
    if ( !IS_NPC(victim)
    &&   victim->hit > 0
    &&   victim->hit <= victim->wimpy
    &&   victim->wait < PULSE_VIOLENCE / 2
    && dt != gsn_parting_blow)
         do_flee( victim, "" );
    }
    else
    {
    if ( !IS_NPC(victim)
    &&   victim->hit > 0
    &&   (victim->hit*100)/victim->max_hit <= victim->wimpy
    &&   victim->wait < PULSE_VIOLENCE / 2
    && dt != gsn_parting_blow)
        do_flee( victim, "" );
    }

    tail_chain( );
    return TRUE;
}



/*
 * Inflict damage from a hit.
 */
bool damage_old( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type, bool show )
{

    OBJ_DATA *corpse;
    bool immune;
    OBJ_DATA *obj;
    OBJ_DATA *wield;
    bool kineticresult;
    int kineticdam;
    int originaldam;
    CHAR_DATA *owner = NULL;
    bool nosafe = FALSE;

    if ((dt == gsn_poison || dt == gsn_plague) && (ch != victim))
	{
	owner = ch;
	ch = victim;
	nosafe = TRUE;
	}

    if (!nosafe && is_safe(ch,victim))
	return FALSE;

    if ( victim->position == POS_DEAD )
        return FALSE;

    if (IS_NPC(ch) && spellbaned(ch,victim,dt))
		return FALSE;
    if (IS_NPC(victim) && victim->pIndexData->vnum == MOB_VNUM_RAVAGER)
    {
	if (spellbaned(ch,victim,dt))
		return FALSE;
    }

    if (is_safe(ch,victim))
	return FALSE;
    /*
     * Stop up any residual loopholes.
     */
    originaldam=dam;
    if ( dam > 1400 && dt >= TYPE_HIT)
    {
        bug( "Damage: %d: more than 1400 points!", dam );
        dam = 1400;
    }
   if ( victim != ch )
    {
        /*
         * Certain attacks are forbidden.
         * Most other attacks are returned.
         */
        if ( is_safe( ch, victim ) )
            return FALSE;
        check_killer( ch, victim );

    if ( (dt >= TYPE_HIT
	|| dt == gsn_backstab
	|| dt == gsn_ambush
	|| dt == gsn_cleave
	|| dt == gsn_deathstrike)
	&& ch != victim && dam_type!=DAM_TRUESTRIKE)
    {
	if (check_counter(ch,victim,dam,dt))
		return FALSE;
	if(is_affected(victim,gsn_steel_nerves) && dt!=gsn_kinetic_shield)
		dam*=.8;
	}
    if(dt==gsn_dual_backstab || HITS) {
	if(is_affected(victim,gsn_steel_nerves) && dam_type!=DAM_TRUESTRIKE)
	    dam=dam*6/10;
	}
        if ( victim->position > POS_STUNNED )
        {
            if ( victim->fighting == NULL )
                set_fighting( victim, ch );
            if (victim->timer <= 4)
                victim->position = POS_FIGHTING;
        }

        if ( victim->position > POS_STUNNED )
        {
            if ( ch->fighting == NULL )
                set_fighting( ch, victim );

            /*
             * If victim is charmed, ch might attack victim's master.
             */
            if ( IS_NPC(ch)
            &&   IS_NPC(victim)
            &&   IS_AFFECTED(victim, AFF_CHARM)
            &&   victim->master != NULL
            &&   victim->master->in_room == ch->in_room
            &&   number_bits( 3 ) == 0 )
            {
                stop_fighting( ch, FALSE );
                multi_hit( ch, victim->master, TYPE_UNDEFINED );
                return FALSE;
            }
        }

        /*
         * More charm stuff.
         */
        if ( victim->master == ch )
            stop_follower( victim );
    }

    /*
     * Inviso attacks ... not.
     */

    un_camouflage(ch,NULL);
    un_hide(ch, NULL);
    un_invis(ch, NULL);
    un_sneak(ch, NULL);

    if (( (dt >= TYPE_HIT || dt == gsn_dual_wield) || (dt == gsn_tertiary_wield) )
	&& ch != victim)
    {

        if ( check_parry( ch, victim, dt ) )
        {
		do_counter_parry(ch,victim,dt, dam);
		return FALSE;
	}
        //if ( check_flourintine( ch, victim, dt ) )
        //    return FALSE;
	if ( check_block( ch, victim, dt) )
		return FALSE;
        if ( check_dodge( ch, victim, dt ) )
            return FALSE;
        if ( check_questdodge( ch, victim, dt ) )
            return FALSE;
        if ( check_shield_block(ch,victim, dt))
            return FALSE;
        //if ( check_phase( ch, victim) )
        //    return FALSE;
	if (check_displacement( ch, victim))
            return FALSE;
        if ( check_spin(ch,victim, dt))
            return FALSE;
    }


    /* damage reduction */
    if (dam > 85)
    {
	dam = (dam - 85);
	dam *= 7;
	dam /= 10;
	dam += 85;
    }




    /*
     * Damage modifiers.
     */
    if (is_affected(ch, gsn_rage))
    {
	dam *= 5;
	dam /= 4;
    }
//elemental shields
    if (is_affected(victim, gsn_shield_of_flames))
        dam *= .9;
    if (is_affected(victim, gsn_shield_of_frost))
        dam *= .88;
    if (is_affected(victim, gsn_shield_of_bubbles))
        dam *= .86;
    if (is_affected(victim, gsn_shield_of_dust))
        dam *= .83;  //slightly more defensive
    if (is_affected(victim, gsn_shield_of_lightning))
        dam *= .86;
    if (is_affected(victim, gsn_shield_of_wind))
	dam *= .86;
 //orduno brand stuff
    if (is_affected(victim, gsn_body_of_wood))
    {
        dam /= 10;
        dam *= 9;
    }
    if (is_affected(victim, gsn_body_of_clay))
    {
        dam /= 10;
        dam *= 8;
    }
    if (is_affected(victim, gsn_body_of_iron))
    {
        dam /= 10;
        dam *= 7;
    }
    if (is_affected(victim, gsn_body_of_steel))
    {
        dam /= 10;
        dam *= 6;
    }
    if ((is_affected(victim, gsn_body_of_stone)) && (!is_affected(victim, gsn_steel_nerves))
    && (!IS_AFFECTED(victim, AFF_SANCTUARY)))
    {
        dam /= 10;
        dam *= 5;
    }
    if (is_affected(victim, gsn_body_of_diamond))
    {
        dam /= 10;
        dam *= 4;
    }

    if ( dam > 1 && !IS_NPC(victim)
    &&   victim->pcdata->condition[COND_DRUNK]  > 10 )
        dam = 9 * dam / 10;

    if ( dam > 1 && IS_AFFECTED(victim, AFF_SANCTUARY) && dt!=gsn_kinetic_shield && dam_type!=DAM_TRUESTRIKE)
    {
    if ( (dt >= TYPE_HIT || dt == gsn_dual_wield || dt == gsn_tertiary_wield))
	{
	if (dt == gsn_dual_wield)
		wield = get_eq_char(ch,WEAR_DUAL_WIELD);
	else if (dt == gsn_tertiary_wield)
	wield = get_eq_char(ch,WEAR_TERTIARY_WIELD);
  	else
	wield = get_eq_char(ch,WEAR_WIELD);
	if (wield == NULL )
		dam /= 2;
	else if (wield->pIndexData->vnum != 29724)
        dam /= 2;
	}
    else
	dam /=2;
    }

    obj = get_eq_char(ch,WEAR_HEAD);

    if (obj != NULL
    && obj->pIndexData->vnum == OBJ_VNUM_TIARA
    && (dam_type == DAM_COLD || dam_type == DAM_FIRE
	|| dam_type == DAM_HOLY || dam_type == DAM_NEGATIVE
    || dam_type == DAM_ACID) )
	{
    if (dam > 125)
    {
	dam = (dam - 125);
	dam *= 6;
	dam /= 10;
	dam += 125;
    }
	}
    if (is_affected(victim,gsn_fireshield) && dt!=gsn_kinetic_shield && dam_type!=DAM_TRUESTRIKE)
	dam *= .5;
    if (is_affected(victim,gsn_wraithform) && dt!=gsn_kinetic_shield && dam_type!=DAM_TRUESTRIKE)
	dam *= .6;
    if (is_affected(victim,gsn_stoneskin) && dt!=gsn_kinetic_shield && dam_type!=DAM_TRUESTRIKE)
	dam *= .7;
    if (is_affected(victim,gsn_shield) && dt!=gsn_kinetic_shield && dam_type!=DAM_TRUESTRIKE)
	dam *= .7;
    if (is_affected(victim,gsn_kinetic_shield) && dt!=gsn_kinetic_shield && dam_type!=DAM_TRUESTRIKE)
	dam *= .7;

    if (is_affected(victim,gsn_shackles))
	dam *= 1.5;

    if (is_affected(ch,gsn_rage_of_dioxide))
	dam *= 1.15;

     if (check_crown_blood(ch,victim,dt,dam,dam_type))
	return FALSE;

    if ( dam > 1 && ((IS_AFFECTED(victim, AFF_PROTECT_EVIL) && IS_EVIL(ch) )
    ||               (IS_AFFECTED(victim, AFF_PROTECT_GOOD) && IS_GOOD(ch) )) && dam_type!=DAM_TRUESTRIKE)
        dam -= dam / 4;

    immune = FALSE;

    if (check_absorb(ch,victim,dt))
	return FALSE;
    if (eye_of_beholder_absorb(ch,victim,dt,dam,dam_type))
	return FALSE;


    /*
     * Check for parry, and dodge.
     */

    switch(check_immune(victim,dam_type))
    {
        case(IS_IMMUNE):
            immune = TRUE;
            dam = 0;
            break;
        case(IS_RESISTANT):
	    if (IS_NPC(victim))
        	dam -= dam/3;
	    else
		dam -= dam/5;
            break;
        case(IS_VULNERABLE):
            dam += dam/3;
            break;
    }

    if (show)
        dam_message( ch, victim, dam, dt, immune );

   if ( (dt >= TYPE_HIT || dt == gsn_dual_wield || dt == gsn_tertiary_wield || dt == gsn_drum || dt==gsn_flurry)
	&& ch != victim)
    {
     if (is_affected(victim,gsn_kinetic_shield) && dam>0)
	{
	  if(number_percent()<67 || originaldam>175) {
        act("$n's kinetic shield reacts to $N's attack!",victim,0,ch,TO_NOTVICT);
        act("Your kinetic shield reacts to $N's attack!",victim,0,ch,TO_CHAR);
        act("$n's kinetic shield reacts to your attack!",victim,0,ch,TO_VICT);
	  kineticdam=originaldam*.15;
	  kineticresult = damage_old( victim, ch, kineticdam, gsn_kinetic_shield, DAM_ENERGY, TRUE );
	  }
      if (is_affected(victim,gsn_self_immolation) && dam>0)
     {
        act("$N is burned by the flames engulfing $n!",victim,NULL,ch,TO_NOTVICT);
        act("$N is burned by the flames engulfing you!",victim,NULL,ch,TO_CHAR);
        act("You are burned by the flames engulfing $n!",victim,NULL,ch,TO_VICT);
        damage_old(victim,ch,number_range(30,100),gsn_flames,DAM_FIRE, TRUE);
     }
      }
    }

    if (dam == 0)
        return FALSE;

    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */
    victim->hit -= dam;
    if (dam_type == DAM_FIRE || dam_type == DAM_ACID)
	victim->noregen_dam += dam;
    if (victim->noregen_dam > victim->max_hit)
	victim->noregen_dam = victim->max_hit;

    if ( !IS_NPC(victim)
    &&   victim->level >= LEVEL_IMMORTAL
    &&   victim->hit < 1 )
        victim->hit = 1;
    update_pos( victim );

    switch( victim->position )
    {
    case POS_MORTAL:
        act( "$n is mortally wounded, and will die soon, if not aided.",
            victim, NULL, NULL, TO_ROOM );
        send_to_char(
            "You are mortally wounded, and will die soon, if not aided.\n\r",
            victim );
        break;

    case POS_INCAP:
        act( "$n is incapacitated and will slowly die, if not aided.",
            victim, NULL, NULL, TO_ROOM );
        send_to_char(
            "You are incapacitated and will slowly die, if not aided.\n\r",
            victim );
        break;

    case POS_STUNNED:
        act( "$n is stunned, but will probably recover.",
            victim, NULL, NULL, TO_ROOM );
        send_to_char("You are stunned, but will probably recover.\n\r",
            victim );
        break;

    case POS_DEAD:
        act( "$n is DEAD!!", victim, 0, 0, TO_ROOM );
        send_to_char( "You have been KILLED!!\n\r\n\r", victim );
        break;

    default:
        if ( dam > victim->max_hit / 4 )
            send_to_char( "That really did HURT!\n\r", victim );
        if ( victim->hit < victim->max_hit / 4 )
            send_to_char( "You sure are BLEEDING!\n\r", victim );
        break;
    }

    /*
     * Sleep spells and extremely wounded folks.
     */
    if ( !IS_AWAKE(victim) && ch != victim)
        stop_fighting( victim, FALSE );

    /*
     * Payoff for killing things.
     */
    if ( victim->position == POS_DEAD )
    {
        group_gain( ch, victim );

        if ( !IS_NPC(victim) )
        {
            sprintf( log_buf, "%s killed by %s at %d",
                victim->name,
                (IS_NPC(ch) ? ch->short_descr : ch->name),
                ch->in_room->vnum );
            log_string( log_buf );
	}
            /*
             * Dying penalty:
             */

        raw_kill(ch, victim );
        /* RT new auto commands */
        if ( !IS_NPC(ch) && IS_NPC(victim) )
        {
            corpse = get_obj_list( ch, "corpse", ch->in_room->contents );

            if ( IS_SET(ch->act, PLR_AUTOLOOT) &&
                 corpse && corpse->contains) /* exists and not empty */
                do_get( ch, "all corpse" );

            if (IS_SET(ch->act,PLR_AUTOGOLD) &&
                corpse && corpse->contains  && /* exists and not empty */
                !IS_SET(ch->act,PLR_AUTOLOOT))
              do_get(ch, "gold corpse");

            if ( IS_SET(ch->act, PLR_AUTOSAC) )
	{
              if ( IS_SET(ch->act,PLR_AUTOLOOT) && corpse && corpse->contains)
                return TRUE;  /* leave if corpse has treasure */
              else
                do_sacrifice( ch, "corpse" );
        }
	}

        return TRUE;
    }

    if ( victim == ch )
        return TRUE;

    /*
     * Take care of link dead people.
     */
    if ( !IS_NPC(victim) && victim->desc == NULL )
    {
        if ( number_range( 0, victim->wait ) == 0 )
        {
            do_recall( victim, "" );
            return TRUE;
        }
    }

    /*
     * Wimp out?
     */
    if ( IS_NPC(victim) && dam > 0 && victim->wait < PULSE_VIOLENCE / 2)
    {
        if ( ( IS_SET(victim->act, ACT_WIMPY) && number_bits( 2 ) == 0
        &&   victim->hit < victim->max_hit / 5)
        ||   ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL
        &&     victim->master->in_room != victim->in_room ) )
            do_flee( victim, "" );
    }

    if ( !IS_NPC(victim)
    &&   victim->hit > 0
    &&   victim->hit <= victim->wimpy
    &&   victim->wait < PULSE_VIOLENCE / 2 )
        do_flee( victim, "" );

    tail_chain( );
    return TRUE;
}

bool is_area_safe(CHAR_DATA *ch, CHAR_DATA *victim)
{
    if (victim->in_room == NULL || ch->in_room == NULL)
	return TRUE;
    if (victim == ch )
	return TRUE;
    if (victim->fighting == ch)
	return FALSE;
    if (IS_NPC(ch) && IS_GOOD(ch) && IS_GOOD(victim))
	return FALSE;
    if (IS_IMMORTAL(victim) && victim->invis_level > 2)
	return TRUE;
    if (IS_IMMORTAL(ch) && IS_IMMORTAL(victim) && victim->level > LEVEL_IMMORTAL && !is_same_group(ch,victim))
	return FALSE;
    if (IS_IMMORTAL(ch) && victim->level < ch->level && !is_same_group(ch,victim))
	return FALSE;

   /* killing mobiles */
    if (IS_NPC(victim))
    {
	if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))
        {
	    send_to_char("Not in this room.\n\r",ch);
	    return TRUE;
	}
	if (is_same_group(ch,victim))
            return TRUE;
    }
    /* killing players */
    else
    {
        if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))
        {
            send_to_char("Not in this room.\n\r",ch);
            return TRUE;
        }
        if (IS_NPC(ch) && ch->master != NULL && !IS_NPC(ch->master))
            return TRUE;
        if (IS_NPC(ch))
            return FALSE;
	if (is_same_group(ch,victim))
            return TRUE;

    }
    if (!IS_NPC(ch))
        ch->ghost = (time_t)NULL;
    return FALSE;
}

bool is_safe(CHAR_DATA *ch, CHAR_DATA *victim)
{
    if (victim->in_room == NULL || ch->in_room == NULL)
        return TRUE;

    if (IS_NPC(ch) && victim->invis_level >= LEVEL_HERO)
	return TRUE;

    if (!IS_NPC(ch))
	{
	if (ch->pcdata->death_status == HAS_DIED)
		return TRUE;
	}
    if (!IS_NPC(victim))
	{
	if (victim->pcdata->death_status == HAS_DIED)
		return TRUE;
	}

/* Handle Link dead players , only saves from PKS, not mobs -Ceran */
    if (!IS_NPC(victim) && victim->desc == NULL && !IS_NPC(ch)
	&& victim->fighting != ch && ch->fighting != victim)
	{
	act("The gods protect $N from $n.",ch,0,victim,TO_NOTVICT);
	act("The gods protect $N from you.",ch,0,victim,TO_CHAR);
	return TRUE;
 	}

/* Protect newbies. 1 hour max - Dioxide*/
    if (!IS_NPC(victim) && ((victim->played + current_time - victim->logon) < 3)
	&& !IS_NPC(ch) && ((ch->played + current_time - ch->logon) < 3))
	{
	act("The gods protect $N from $n.",ch,0,victim,TO_NOTVICT);
	act("The gods protect $N from you.",ch,0,victim,TO_CHAR);
	return TRUE;
 	}

    if (is_affected(victim,gsn_timestop))
    {
	act("$n is stuck in time and the gods protect $m.",victim,0,0,TO_ROOM);
	act("$N tries to do something but you are stuck in time and unaffected.",victim,0,ch,TO_CHAR);
	if (victim->fighting != NULL)
		victim->fighting = NULL;
	return TRUE;
    }

    if (victim->fighting == ch || victim == ch)
        return FALSE;

    if (IS_NPC(ch) && ch->last_fought == victim)
	return FALSE;


    if (victim->ghost > 0)
    {
	act("The gods protect $N from $n.",ch,0,victim,TO_NOTVICT);
	act("The gods protect you from $n.",ch,0,victim,TO_VICT);
	act("The gods protect $N from you.",ch,0,victim,TO_CHAR);
	return TRUE;
    }

    if (ch->ghost > 0)
    {
	if (!IS_NPC(victim))
	{
	act("The gods protect $N from $n.",ch,0,victim,TO_NOTVICT);
	act("The gods protect you from $n.",ch,0,victim,TO_VICT);
	act("The gods protect $N from you.",ch,0,victim,TO_CHAR);
		return TRUE;
	}
            if (IS_AFFECTED(victim,AFF_CHARM) && victim->master != NULL
		&& !IS_NPC(victim->master))
            {
  	act("The gods protect $N from $n.",ch,0,victim,TO_NOTVICT);
	act("The gods protect you from $n.",ch,0,victim,TO_VICT);
	act("The gods protect $N from you.",ch,0,victim,TO_CHAR);
                return TRUE;
            }

    }

    if (IS_IMMORTAL(ch) && is_affected(ch,gsn_riot) && !IS_NPC(victim))
    {
	send_to_char("You can't riot on mortals.\n\r",ch);
	return TRUE;
    }

    if (IS_IMMORTAL(ch) && ch->level > LEVEL_IMMORTAL)
        return FALSE;


    if (IS_SET(victim->act,PLR_CRIMINAL))
    {
	if (IS_NPC(ch) && ch->pIndexData->vnum == MOB_VNUM_VIGILANTE)
		return FALSE;
    }
    if(IS_NPC(victim)&&victim->pIndexData->vnum==MOB_VNUM_VIGILANTE)
	return FALSE;

    /* killing mobiles */
    if (IS_NPC(victim))
    {
        if (!IS_NPC(ch))
        {
            /* no charmed creatures unless in pk of master */
            if (IS_AFFECTED(victim,AFF_CHARM) && !can_pk(ch,victim->master) && !is_centurion(victim))
            {
  	act("The gods protect $N from $n.",ch,0,victim,TO_NOTVICT);
	act("The gods protect you from $n.",ch,0,victim,TO_VICT);
	act("The gods protect $N from you.",ch,0,victim,TO_CHAR);
                return TRUE;
            }
        }
	else
	{
	if (IS_AFFECTED(ch,AFF_CHARM) && IS_AFFECTED(victim,AFF_CHARM)
	&& !can_pk(ch->master,victim->master))
                return TRUE;
	}
    }
    /* killing players */
    else
    {
        /* NPC doing the killing */
        if (IS_NPC(ch))
        {
            /* charmed mobs and pets cannot attack players while owned */
            if (IS_AFFECTED(ch,AFF_CHARM) && ch->master != NULL
            &&  ch->master->fighting != victim && !is_centurion(ch)
            && !check_rangermob(ch))
            {
                send_to_char("Players are your friends!\n\r",ch);
                return TRUE;
            }
        }
        /* player doing the killing */
        else
        {
	    if (!can_pk(ch, victim))
    	    {
	if (!IS_IMMORTAL(victim)) {
  	act("The gods protect $N from $n.",ch,0,victim,TO_NOTVICT);
	act("The gods protect you from $n.",ch,0,victim,TO_VICT);
	act("The gods protect $N from you.",ch,0,victim,TO_CHAR);
		}
        	return TRUE;
    	    }
        }
    }
    return FALSE;
}

bool is_safe_spell(CHAR_DATA *ch, CHAR_DATA *victim, bool area )
{
    if (victim->in_room == NULL || ch->in_room == NULL)
        return TRUE;

    if (victim == ch && area)
        return TRUE;

    if (victim->fighting == ch || victim == ch)
        return FALSE;

    if (IS_IMMORTAL(ch) && ch->level > LEVEL_IMMORTAL && !area)
        return FALSE;

    /* killing mobiles */
    if (IS_NPC(victim))
    {
        /* safe room? */
        if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))
            return TRUE;

        if (victim->pIndexData->pShop != NULL)
            return TRUE;

        /* no killing healers, trainers, etc */
        if (IS_SET(victim->act,ACT_TRAIN)
        ||  IS_SET(victim->act,ACT_PRACTICE)
        ||  IS_SET(victim->act,ACT_IS_HEALER)
        ||  IS_SET(victim->act,ACT_IS_CHANGER))
            return TRUE;

        if (!IS_NPC(ch))
        {
            /* no pets */
            if (IS_SET(victim->act,ACT_PET))
                return TRUE;

            /* no charmed creatures unless owner */
            if (IS_AFFECTED(victim,AFF_CHARM) && (area || ch != victim->master))
                return TRUE;

            /* legal kill? -- cannot hit mob fighting non-group member */
            if (victim->fighting != NULL && !is_same_group(ch,victim->fighting))
                return TRUE;
        }
        else
        {
            /* area effect spells do not hit other mobs */
            if (area && !is_same_group(victim,ch->fighting))
                return TRUE;
        }
    }
    /* killing players */
    else
    {
        if (area && IS_IMMORTAL(victim) && victim->level > LEVEL_IMMORTAL)
            return TRUE;

        /* NPC doing the killing */
        if (IS_NPC(ch))
        {
            /* charmed mobs and pets cannot attack players while owned */
            if (IS_AFFECTED(ch,AFF_CHARM) && ch->master != NULL
            &&  ch->master->fighting != victim)
                return TRUE;

            /* safe room? */
            if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))
                return TRUE;

            /* legal kill? -- mobs only hit players grouped with opponent*/
            if (ch->fighting != NULL && !is_same_group(ch->fighting,victim))
                return TRUE;
        }

        /* player doing the killing */
        else
        {
            if (!is_cabal(ch))
                return TRUE;

            if (IS_SET(victim->act,PLR_KILLER) || IS_SET(victim->act,PLR_THIEF))
                return FALSE;

            if (!is_cabal(victim))
                return TRUE;

            if (ch->level > victim->level + 8)
                return TRUE;
        }

    }
    return FALSE;
}
/*
 * See if an attack justifies a KILLER flag.
 */
void check_killer( CHAR_DATA *ch, CHAR_DATA *victim )
{
    /*
     * Follow charm thread to responsible character.
     * Attacking someone's charmed char is hostile!
     */
    while ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL )
        victim = victim->master;

    /*
     * NPC's are fair game.
     * So are killers and thieves.
     */
    if ( IS_NPC(victim)
    ||   IS_SET(victim->act, PLR_KILLER)
    ||   IS_SET(victim->act, PLR_THIEF))
        return;

    /*
     * NPC's are cool of course (as long as not charmed).
     * Hitting yourself is cool too (bleeding).
     * So is being immortal (Alander's idea).
     * And current killers stay as they are.
     */
    if ( IS_NPC(ch)
    ||   ch == victim
    ||   ch->level >= LEVEL_IMMORTAL
    ||   !is_cabal(ch)
    ||   IS_SET(ch->act, PLR_KILLER)
    ||   ch->fighting  == victim)
        return;

    save_char_obj( ch );
    return;
}

/* Check a counter-parry */

void do_counter_parry(CHAR_DATA *ch,CHAR_DATA *victim,int dt, int dam)
{
	int chance;
	int tmp_dt;
	char *attack;
	OBJ_DATA *wield;
	char buf1[MAX_STRING_LENGTH];
	char buf2[MAX_STRING_LENGTH];

	if ((chance = get_skill(victim,gsn_counter_parry)) == 0
	|| IS_NPC(victim)
	|| victim->level < skill_table[gsn_counter_parry].skill_level[victim->class])
		return;

	chance /= 5;
	if (chance > 20)
		chance = 20;

	if (number_percent() < chance)
	{
    tmp_dt = TYPE_HIT;
    if (dt  == gsn_dual_wield)
    	wield = get_eq_char(ch,WEAR_DUAL_WIELD);
    else if (dt == gsn_tertiary_wield)
	wield = get_eq_char(ch,WEAR_TERTIARY_WIELD);
    else
	wield = get_eq_char(ch,WEAR_WIELD);


    if (wield != NULL)
	tmp_dt += wield->value[3];
    else
	tmp_dt += ch->dam_type;


    if (tmp_dt > TYPE_HIT && tmp_dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
	attack = attack_table[tmp_dt - TYPE_HIT].noun;
    else
	attack = attack_table[0].name;

    sprintf(buf1,"You counter parry $n's %s!",attack);
    sprintf(buf2,"$N counter parries your %s!",attack);
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
        if (cabal_down(victim,CABAL_RAVAGER))
		return;

	damage_old(victim,ch,dam,gsn_counter_parry,TYPE_HIT,TRUE);
	check_improve(victim,gsn_counter_parry,TRUE,3);
	return;
	}

	check_improve(victim,gsn_counter_parry,FALSE,2);
	return;
}
bool check_displacement(CHAR_DATA *ch, CHAR_DATA *victim)
{
    int chance;

    if (!(is_affected(victim, gsn_displacement)))
	return FALSE;
    chance = get_skill(victim,gsn_displacement);
    chance /= 3;
    chance *= 2;

    if (!can_see(victim,ch))
	chance /=2;

    if (IS_AFFECTED(ch,AFF_DETECT_ILLUSION))
	chance /= 4;

    if (number_percent() > chance + victim->level - ch->level)
	return FALSE;

    act("$n's attack passes harmlessly through $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n's attack misses you completely.",ch,NULL,victim,TO_VICT);
    act("Your attack passes harmlessly through $N.",ch,NULL,victim,TO_CHAR);
    return TRUE;
}

bool check_counter(CHAR_DATA *ch,CHAR_DATA *victim,int dam,int dt)
{
    int chance;

    if (dt == gsn_counter )
	return FALSE;
    if (dt == gsn_counter_parry)
	return FALSE;

    if (ch->fighting != NULL)
		return FALSE;

    if ( (chance = get_skill(victim,gsn_counter)) == 0
    || victim->level < skill_table[gsn_counter].skill_level[ch->class])
	return FALSE;

    if (!can_see(victim,ch))
	chance -= 30;
    if (get_eq_char(victim,WEAR_WIELD) == NULL)
	chance -= 50;
    chance += (victim->level - ch->level);

    if (victim->position != POS_FIGHTING && IS_AWAKE(victim))
    {
	if (number_percent() < chance)
	{
	act("You counter $n's attack!",ch,0,victim,TO_VICT);
	act("$N counters your attack!",ch,0,victim,TO_CHAR);
	act("$N counters $n's attack!",ch,0,victim,TO_NOTVICT);
	check_improve(victim,gsn_counter,TRUE,1);
	damage_old(victim,ch,dam,gsn_counter,TYPE_UNDEFINED,TRUE);
	return TRUE;
	}
    }

    check_improve(victim,gsn_counter,FALSE,2);
    return FALSE;
}



/*
 * Check for parry.
 */
bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    int flourintine = 0;
    int chance;
    int tmp_dt;
    OBJ_DATA *wield;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *attack;
    OBJ_DATA *victimwield;

    if ( !IS_AWAKE(victim) )
        return FALSE;

    chance = get_skill(victim,gsn_parry) / 2;

    if ( get_eq_char( victim, WEAR_WIELD ) == NULL )
    {
        if (IS_NPC (victim) || isShifted(victim))
            chance /= 2;
        else
            return FALSE;
    }

    if (IS_AFFECTED(victim,AFF_BLIND))
        chance *= number_range(6/10,3/4);
    if (is_affected(victim,gsn_executioner))
	  chance+=20;
    if (is_affected(ch,gsn_rage_of_dioxide))
	  chance-=10;
    if (is_affected(victim,skill_lookup("haste")))
	  chance+=20;
    if (is_affected(ch,skill_lookup("haste")))
	  chance-=15;
    if (is_affected(victim,gsn_rage))
	chance *= .75;
/* The rest is all new dual wield stuff, with dam types displayed for
clarity when dual wielding - Ceran */
    tmp_dt = TYPE_HIT;
    if (dt  == gsn_dual_wield)
    	wield = get_eq_char(ch,WEAR_DUAL_WIELD);
    else if (dt == gsn_tertiary_wield)
	wield = get_eq_char(ch,WEAR_TERTIARY_WIELD);
    else
	wield = get_eq_char(ch,WEAR_WIELD);
    if (wield != NULL)
    {
    /* Weapons affect parrying. */
    switch (wield->value[0])
    {
	default:			chance+=15;		break;
	case (WEAPON_SWORD): 	chance+=5;		break;
	case (WEAPON_DAGGER): 	chance+=5;		break;
	case (WEAPON_SPEAR):	chance-=5;		break;
	case (WEAPON_STAFF):	chance-=5;		break;
	case (WEAPON_MACE):	chance-=5;		break;
	case (WEAPON_AXE):	chance-=5;		break;
	case (WEAPON_FLAIL):	chance+=10;		break;
	case (WEAPON_WHIP):	chance+=10;		break;
	case (WEAPON_POLEARM):	chance-=5;		break;
    }
    }
    if((victimwield=get_eq_char(victim,WEAR_WIELD)) != NULL)
    {
    switch (victimwield->value[0])
    {
	default:						break;
	case (WEAPON_SWORD): 	chance+=10; flourintine++; 		break;
	case (WEAPON_DAGGER): 	chance-=20;		break;
	case (WEAPON_SPEAR):	chance+=20;		break;
	case (WEAPON_STAFF):	chance+=10;		break;
	case (WEAPON_MACE):	chance-=20;		break;
	case (WEAPON_AXE):	chance-=25;		break;
	case (WEAPON_FLAIL):	chance-=10;		break;
	case (WEAPON_WHIP):	chance-=10;		break;
	case (WEAPON_POLEARM):	chance+=10;		break;
    }
    }

	if ((get_skill(victim,gsn_flourintine) > 0) && flourintine == 1)
	{
		check_improve(victim,gsn_flourintine,TRUE,1);
		chance += get_skill(victim,gsn_flourintine) / 7;
	}
if (isShifted(victim))
	chance += form_table[victim->pcdata->shifted].parry_mod;

    if ( number_percent( ) >= chance)
    {
	check_improve(victim,gsn_parry,FALSE,2);
        return FALSE;
    }


	/* imperial training */
	if (!IS_NPC(ch))
	{
	if (check_imperial_training(ch) == IMPERIAL_OFFENSE)
		chance -= 20;
	if (check_imperial_training(ch) == IMPERIAL_DEFENSE)
		chance += 20;
	}
	if (!IS_NPC(victim))
	{
	if (check_imperial_training(victim) == IMPERIAL_OFFENSE)
		chance -= 20;
	if (check_imperial_training(victim) == IMPERIAL_DEFENSE)
		chance += 20;
	}

    if (wield != NULL)
	tmp_dt += wield->value[3];
    else
	tmp_dt += ch->dam_type;


    if (tmp_dt > TYPE_HIT && tmp_dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
	attack = attack_table[tmp_dt - TYPE_HIT].noun;
    else
    {
	bug("In parry, bad dam message: dt %d.",tmp_dt);
	sprintf(buf1,"Player: %s Target: %s.",ch->name,victim->name);
	bug(buf1, 0);
	attack = attack_table[0].name;
    }

    sprintf(buf1,"You parry $n's %s.",attack);
    sprintf(buf2,"$N parries your %s.",attack);
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
    check_improve(victim,gsn_parry,TRUE,2);
    return TRUE;
}

/* The rest is all new dual wield stuff, with dam types displayed for
clarity when dual wielding - Ceran */
/*    tmp_dt = TYPE_HIT;
    if (dt  == gsn_dual_wield)
    	wield = get_eq_char(ch,WEAR_DUAL_WIELD);
    else if (dt == gsn_tertiary_wield)
	wield = get_eq_char(ch,WEAR_TERTIARY_WIELD);
    else
	wield = get_eq_char(ch,WEAR_WIELD);


    if (wield != NULL)
	tmp_dt += wield->value[3];
    else
	tmp_dt += ch->dam_type;


    if (tmp_dt > TYPE_HIT && tmp_dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
	attack = attack_table[tmp_dt - TYPE_HIT].noun;
    else
    {
	bug("In parry, bad dam message: dt %d.",tmp_dt);
	sprintf(buf1,"Player: %s Target: %s.",ch->name,victim->name);
	bug(buf1, 0);
	attack = attack_table[0].name;
    }

    sprintf(buf1,"You block $n's %s with an agile cross parry.",attack);
    sprintf(buf2,"$N blocks your %s with an agile cross parry.",attack);
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
   // check_improve(victim,tsn,TRUE,1);
    check_improve(victim,gsn_flourintine,TRUE,2);
    return TRUE;
}
*/
/* Block for monks */
bool check_block( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    int chance;
    int tmp_dt;
    OBJ_DATA *wield;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *attack;

    if ( !IS_AWAKE(victim) )
        return FALSE;
    if (IS_NPC(victim))
	return FALSE;

    chance = get_skill(victim,gsn_block);

    if ( victim->level <skill_table[gsn_block].skill_level[victim->class])
  	return FALSE;

    chance /= 2.25;

    if (get_eq_char(victim,WEAR_WIELD) != NULL)
	return FALSE;

    if (!can_see(ch,victim))
        chance /= 2;
    if (is_affected(victim,gsn_rage))
        chance *= .75;
    if ( number_percent( ) >= chance + (victim->drain_level +  victim->level) - (ch->drain_level + ch->level) )
    {
	check_improve(victim,gsn_block,FALSE,5);
        return FALSE;
    }

/* The rest is all new dual wield stuff, with dam types displayed for
clarity when dual wielding - Ceran */
    tmp_dt = TYPE_HIT;
    if (dt  == gsn_dual_wield)
    	wield = get_eq_char(ch,WEAR_DUAL_WIELD);
     else if (dt == gsn_tertiary_wield)
	wield = get_eq_char(ch,WEAR_TERTIARY_WIELD);
    else
	wield = get_eq_char(ch,WEAR_WIELD);


    if (wield != NULL)
	tmp_dt += wield->value[3];
    else
	tmp_dt += ch->dam_type;


    if (tmp_dt > TYPE_HIT && tmp_dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
	attack = attack_table[tmp_dt - TYPE_HIT].noun;
    else
    {
	bug("In check_block, bad dam message: dt %d.",tmp_dt);
	attack = attack_table[0].name;
    }

    sprintf(buf1,"You hand block $n's %s.",attack);
    sprintf(buf2,"$N hand blocks your %s.",attack);
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
    check_improve(victim,gsn_block,TRUE,6);
    return TRUE;
}

/*
 * Check for shield block.
 */
bool check_shield_block( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    int chance;
    int tmp_dt;
    OBJ_DATA *wield;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *attack;

    if ( !IS_AWAKE(victim) )
        return FALSE;

    if(victim->level < skill_table[gsn_shield_block].skill_level[victim->class])
	return FALSE;

    chance = (( get_skill(victim,gsn_shield_block) / 4) + 15);

/* imperial training */
	if (!IS_NPC(ch))
	{
	if (check_imperial_training(ch) == IMPERIAL_OFFENSE)
		chance -= 10;
	if (check_imperial_training(ch) == IMPERIAL_DEFENSE)
		chance += 20;
	}
	if (!IS_NPC(victim))
	{
	if (check_imperial_training(victim) == IMPERIAL_OFFENSE)
		chance -= 20;
	if (check_imperial_training(victim) == IMPERIAL_DEFENSE)
		chance += 10;
	}

    if ( get_eq_char( victim, WEAR_SHIELD ) == NULL )
        return FALSE;

    chance += (victim->drain_level + victim->level - (ch->drain_level + ch->level));
    if (is_affected(victim,gsn_rage))
        chance *= .75;
    if ( number_percent( ) >= chance  )
    {
	check_improve(victim,gsn_shield_block,FALSE,4);
        return FALSE;
    }

    tmp_dt = TYPE_HIT;
    if (dt  == gsn_dual_wield)
    	wield = get_eq_char(ch,WEAR_DUAL_WIELD);
    else if (dt == gsn_tertiary_wield)
	wield = get_eq_char(ch,WEAR_TERTIARY_WIELD);
    else
	wield = get_eq_char(ch,WEAR_WIELD);


    if (wield != NULL)
	tmp_dt += wield->value[3];
    else
	tmp_dt += ch->dam_type;


    if (tmp_dt > TYPE_HIT && tmp_dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
	attack = attack_table[tmp_dt - TYPE_HIT].noun;
    else
    {
	bug("In shield block, bad dam message: dt %d.",tmp_dt);
	attack = attack_table[0].name;
    }

    sprintf(buf1,"You shield block $n's %s.",attack);
    sprintf(buf2,"$N shield blocks your %s.",attack);
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
    check_improve(victim,gsn_shield_block,TRUE,4);
    return TRUE;
}


/*
 * Check for dodge.
 */
bool check_dodge( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    int chance;
    int tmp_dt;
    OBJ_DATA *wield;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *attack;
    int dex, dexa;

    if ( !IS_AWAKE(victim) )
        return FALSE;

chance = ( 3 * get_skill(victim,gsn_dodge) / 10);

    dex = get_curr_stat(victim,STAT_DEX);
    dexa= get_curr_stat(ch,STAT_DEX);
    if (dex <= 5)
	chance += 0;
    else if (dex <= 10)
	chance += dex/2;
    else if (dex <=15)
	chance += (2*dex/3);
    else if (dex <= 20)
	chance += (8*dex/10);
    else
	chance += dex;
    chance+=dex-dexa;
    chance+=(ch->size-victim->size)*5;
    /*if ((get_skill(ch,gsn_ironhands)) >=95)
        chance+=35;
    else if ((get_skill(ch,gsn_ironhands)) >=85)
        chance+=20;
    else if ((get_skill(ch,gsn_ironhands)) >=75)
        chance+=15;
    else if ((get_skill(ch,gsn_ironhands)) >=65)
        chance+=10;
    else if ((get_skill(ch,gsn_ironhands)) >=55)
        chance+=10;
    else if ((get_skill(ch,gsn_ironhands)) >=45)
        chance+=5;*/
    if (is_affected(ch,gsn_executioner))
	  chance+=30;

	/* imperial training */
	if (!IS_NPC(ch))
	{
	if (check_imperial_training(ch) == IMPERIAL_OFFENSE)
		chance -= 20;
	if (check_imperial_training(ch) == IMPERIAL_DEFENSE)
		chance += 20;
	}
	if (!IS_NPC(victim))
	{
	if (check_imperial_training(victim) == IMPERIAL_OFFENSE)
		chance -= 20;
	if (check_imperial_training(victim) == IMPERIAL_DEFENSE)
		chance += 20;
	}

    if (is_affected(ch,gsn_rage_of_dioxide))
	  chance-=20;
    if (!can_see(victim,ch))
        chance *= number_range(6/10,3/4);
    if (is_affected(victim,gsn_rage))
        chance *= .6;
    if ( number_percent( ) >= chance + (victim->drain_level +  victim->level) - (ch->drain_level + ch->level) )
    {
	check_improve(victim,gsn_dodge,FALSE,4);
        return FALSE;
    }

/* The rest is all new dual wield stuff, with dam types displayed for
clarity when dual wielding - Ceran */
    tmp_dt = TYPE_HIT;
    if (dt  == gsn_dual_wield)
    	wield = get_eq_char(ch,WEAR_DUAL_WIELD);
    else if (dt == gsn_tertiary_wield)
	wield = get_eq_char(ch,WEAR_TERTIARY_WIELD);
    else
	wield = get_eq_char(ch,WEAR_WIELD);

    if (wield != NULL)
	tmp_dt += wield->value[3];
    else
	tmp_dt += ch->dam_type;

    if (tmp_dt > TYPE_HIT && tmp_dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
	attack = attack_table[tmp_dt - TYPE_HIT].noun;
    else
    {
	bug("In dodge, bad dam message: dt %d.",tmp_dt);
	attack = attack_table[0].name;
    }

		sprintf(buf1,"You dodge $n's %s.",attack);
		sprintf(buf2,"$N dodges your %s.",attack);

    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
    check_improve(victim,gsn_dodge,TRUE,5);
    return TRUE;
}

/*
 * Check for quest dodge.
 */
bool check_questdodge( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    int chance;
    int tmp_dt;
    OBJ_DATA *wield;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *attack;
    int dex, dexa;

    if ( !IS_AWAKE(victim) )
        return FALSE;

	if (get_skill(victim,gsn_questdodge) == 0) {
		return FALSE;
	}

	chance = ( 3 * get_skill(victim,gsn_questdodge) / 7);
    dex = get_curr_stat(victim,STAT_DEX);
    dexa= get_curr_stat(ch,STAT_DEX);
    if (dex <= 5)
	chance += 0;
    else if (dex <= 10)
	chance += dex/2;
    else if (dex <=15)
	chance += (2*dex/3);
    else if (dex <= 20)
	chance += (8*dex/10);
    else
	chance += dex;
    chance+=dex-dexa;
    chance+=(ch->size-victim->size)*5;
    if (is_affected(ch,gsn_executioner))
	  chance+=30;
    if (!can_see(victim,ch))
        chance /= 2;

    if ( number_percent( ) >= chance + (victim->drain_level +  victim->level) - (ch->drain_level + ch->level) )
    {
		check_improve(victim,gsn_questdodge,FALSE,4);
       return FALSE;
    }

    tmp_dt = TYPE_HIT;
    if (dt  == gsn_dual_wield)
    	wield = get_eq_char(ch,WEAR_DUAL_WIELD);
    else if (dt == gsn_tertiary_wield)
	wield = get_eq_char(ch,WEAR_TERTIARY_WIELD);
    else
	wield = get_eq_char(ch,WEAR_WIELD);

    if (wield != NULL)
	tmp_dt += wield->value[3];
    else
	tmp_dt += ch->dam_type;

    if (tmp_dt > TYPE_HIT && tmp_dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
	attack = attack_table[tmp_dt - TYPE_HIT].noun;
    else
    {
	bug("In quest dodge, bad dam message: dt %d.",tmp_dt);
	attack = attack_table[0].name;
    }

		sprintf(buf1,"You evade $n's %s.",attack);
		sprintf(buf2,"$N evades your %s.",attack);

    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);

    check_improve(victim,gsn_questdodge,TRUE,5);

    return TRUE;
}

/*
 * Check for defensive spin.
 */
bool check_spin( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
	int chance;
	int tmp_dt;
//	int weaponskill;
	OBJ_DATA *wield;
	char buf1[MAX_STRING_LENGTH];
	char buf2[MAX_STRING_LENGTH];
	char *attack;

	if ((chance = get_skill(victim,gsn_spin) / 2) == 0) {
		return FALSE;
	}

	wield = get_eq_char(victim,WEAR_WIELD);

	if (wield == NULL) {
		return FALSE;
	}

	if (wield != NULL)
	{
		switch (wield->value[0])
		{
			case (WEAPON_STAFF):
			 	chance+= get_skill(victim,gsn_staff) / 10;
				break;
			case (WEAPON_SPEAR):
				chance+= get_skill(victim,gsn_spear) / 10;
				break;
			case (WEAPON_POLEARM):
				chance+= get_skill(victim,gsn_polearm) / 10;
				break;
			default:
				return FALSE;
				break;
		}
	}

	if ( !IS_AWAKE(victim) )
	return FALSE;

	if ( get_eq_char( victim, WEAR_WIELD ) == NULL )
	{
		if (IS_NPC(victim))
			chance /= 2;
		else
			return FALSE;
	}

	if (IS_AFFECTED(victim,AFF_BLIND))
		chance *= .6;
	if (is_affected(victim,skill_lookup("haste")))
		chance+=20;
	if (is_affected(ch,skill_lookup("haste")))
		chance-=15;
	if (is_affected(victim,gsn_rage))
	        chance *= .75;

	tmp_dt = TYPE_HIT;

	if ( number_percent() >= chance)
	{
		check_improve(victim,gsn_spin,FALSE,2);
		return FALSE;
	}


    if (wield != NULL)
	tmp_dt += wield->value[3];
    else
	tmp_dt += ch->dam_type;


    if (tmp_dt > TYPE_HIT && tmp_dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
	attack = attack_table[tmp_dt - TYPE_HIT].noun;
    else
    {
	bug("In spin, bad dam message: dt %d.",tmp_dt);
	sprintf(buf1,"Player: %s Target: %s.",ch->name,victim->name);
	bug(buf1, 0);
	attack = attack_table[0].name;
    }

    sprintf(buf1,"Your defensive spin blocks $n's %s.",attack);
    sprintf(buf2,"$N's defensive spin blocks your %s.",attack);
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
    check_improve(victim,gsn_spin,TRUE,2);
    return TRUE;
}

bool check_distance( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    int chance, sn = skill_lookup("distance");
    OBJ_DATA *wield;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *attack;

    if ( !IS_AWAKE(victim) )
        return FALSE;

    if(get_skill(victim,sn)<5)
        return FALSE;

    chance = (( get_skill(victim,sn) / 4) + 15);

	/* imperial training */
	if (check_imperial_training(ch) == IMPERIAL_OFFENSE)
		chance -= 10;
	if (check_imperial_training(victim) == IMPERIAL_DEFENSE)
		chance += 10;
	if (check_imperial_training(victim) == IMPERIAL_OFFENSE)
		chance -= 20;
	if (check_imperial_training(ch) == IMPERIAL_DEFENSE)
		chance += 20;

    chance+=20*(victim->size-ch->size);
    chance+=10*(get_curr_stat(victim,STAT_STR)-get_curr_stat(ch,STAT_STR));
    if(IS_AFFECTED(victim,AFF_HASTE))
             chance+=15;
    if(IS_AFFECTED(ch,AFF_HASTE))
             chance-=15;
    if( ((wield=get_eq_char(victim,WEAR_WIELD)) == NULL || wield->value[0]!=WEAPON_POLEARM))
             return FALSE;
    chance += victim->level - ch->level;
    chance*=(1-((float)GET_HITROLL(ch) / (float) 250));
    if (is_affected(victim,gsn_rage))
        chance *= .75;
    chance=URANGE(5,chance,65);
    if ( number_percent( ) >= chance )
        return FALSE;
    attack=get_dam_message(ch,dt);
    if(number_percent()<(chance/3))
    {
         sprintf(buf1,"You keep $n at bay with your $p, knocking $m off balance with a counter strike.");
         sprintf(buf2,"$N keeps you at bay with $S $p, knocking you off balance with a counter strike.");
         act(buf1,ch,wield,victim,TO_VICT);
         act(buf2,ch,wield,victim,TO_CHAR);
         sprintf(buf1,"$N keeps $n at bay with $S $p, knocking $m off balance with a counter strike.");
         act(buf1,ch,wield,victim,TO_NOTVICT);
         one_hit(victim,ch,sn);
         WAIT_STATE(ch,PULSE_VIOLENCE*1.5);
         return TRUE;
    }
    sprintf(buf1,"You keep $n's %s at bay with your polearm.",attack);
    sprintf(buf2,"$N keeps your %s at bay with $s polearm.",attack);
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
    check_improve(victim,sn,TRUE,2);
    return TRUE;
}

bool check_defiant_absorb( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    char buf1[MAX_STRING_LENGTH];
	char buf2[MAX_STRING_LENGTH];
	char *attack;


	if ( !IS_AWAKE(victim) )
	     return FALSE;
	if (!is_affected(victim,gsn_defiance))
	     return FALSE;

	attack=get_dam_message(ch,dt);

	if (number_percent () < 35)
	{
		sprintf(buf1,"You defiantly absorb $n's %s.",attack);
		sprintf(buf2,"$N defiantly absorbs your %s.",attack);
		act(buf1,ch,0,victim,TO_VICT);
    	act(buf2,ch,0,victim,TO_CHAR);
    	return TRUE;
	}
	return FALSE;

}

/*
 * Set position of a victim.
 */
void update_pos( CHAR_DATA *victim )
{
    if ( victim->hit > 0 )
    {
        if ( victim->position <= POS_STUNNED )
            victim->position = POS_STANDING;
        return;
    }

    if ( IS_NPC(victim) && victim->hit < 1 )
    {
        victim->position = POS_DEAD;
        return;
    }

    if ( victim->hit <= -11 )
    {
        victim->position = POS_DEAD;
        return;
    }

         if ( victim->hit <= -6 ) victim->position = POS_MORTAL;
    else if ( victim->hit <= -3 ) victim->position = POS_INCAP;
    else                          victim->position = POS_STUNNED;

    return;
}



/*
 * Start fights.
 */
void set_fighting( CHAR_DATA *ch, CHAR_DATA *victim )
{

    char buf[MAX_STRING_LENGTH];

    if ( ch->fighting != NULL )
    {
        bug( "Set_fighting: already fighting", 0 );
        return;
    }

    if ( IS_AFFECTED(ch, AFF_SLEEP) )
        affect_strip( ch, gsn_sleep );

    if (is_affected(victim,gsn_trip_wire))
    {
    	send_to_char("You lost your trip wire in the frey.\n\r",victim);
	affect_strip(victim,gsn_trip_wire);
    }
    if (is_affected(ch,gsn_trip_wire))
    {
    	send_to_char("You lost your trip wire in the frey.\n\r",ch);
    	affect_strip(ch,gsn_trip_wire);
    }

    ch->fighting = victim;
    ch->position = POS_FIGHTING;

    if (is_centurion(victim))
    {
	sprintf(buf,"empire Help! %s is attacking me!",PERS(ch,victim));
	do_ccb(victim,buf);
    }

    return;
}

/*
 * Stop fights.
 */
void stop_fighting( CHAR_DATA *ch, bool fBoth )
{
    CHAR_DATA *fch;

    for ( fch = char_list; fch != NULL; fch = fch->next )
    {
        if ( fch == ch || ( fBoth && fch->fighting == ch ) )
        {
            fch->fighting       = NULL;
            fch->position       = IS_NPC(fch) ? fch->default_pos : POS_STANDING;
            update_pos( fch );
        }
    }

    return;
}



/*
 * Make a corpse out of a character.
 */
void make_corpse( CHAR_DATA *killer,CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *corpse;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    CHAR_DATA *itemguy;
    char *name;

    if (IS_NPC(ch) && (ch->pIndexData->vnum == MOB_VNUM_PROTECTOR
	|| ch->pIndexData->vnum == MOB_VNUM_ACADIAN
	|| ch->pIndexData->vnum == MOB_VNUM_COVEN
	|| ch->pIndexData->vnum == MOB_VNUM_BRIAR
	|| ch->pIndexData->vnum == MOB_VNUM_RAVAGER) )
    {
    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
        obj_next = obj->next_content;
        if ((obj->pIndexData->vnum!=0) && isCabalItem(obj) )
	  {
		itemguy=killer;
		sprintf(buf,"As %s falls to the ground, the %s appears in the hands of %s!",ch->short_descr,obj->short_descr,itemguy->name);
		if(IS_NPC(killer))
		{
			if (killer->leader!=0) itemguy=killer->leader;
			sprintf(buf,"As %s falls to %s's minions, the %s appears in the hands of %s!",ch->short_descr,itemguy->name,obj->short_descr,itemguy->name);
		}
            act(buf,itemguy,NULL,NULL,TO_ROOM);
		sprintf(buf,"As %s falls to the ground, the %s appears in your hands!",ch->short_descr,obj->short_descr);
            act(buf,itemguy,NULL,NULL,TO_CHAR);
            obj_from_char( obj );
		obj_to_char(obj,itemguy);
	  }
    }
    return;
    }

    if ( IS_NPC(ch) )
    {
        name            = ch->short_descr;
        corpse          = create_object(get_obj_index(OBJ_VNUM_CORPSE_NPC), 0);
        corpse->timer   = number_range( 5, 8 );

	free_string(corpse->talked);
	if (IS_NPC(killer))
		corpse->talked = str_dup(killer->short_descr);
	else
		corpse->talked = str_dup(killer->name);
        if ( ch->gold > 0 )
        {
            obj_to_obj( create_money( ch->gold, ch->silver ), corpse );
            ch->gold = 0;
            ch->silver = 0;
        }
        corpse->cost = 0;
    }
    else
    {
        name            = ch->name;
        corpse          = create_object(get_obj_index(OBJ_VNUM_CORPSE_PC), 0);
        corpse->timer   = number_range( 20, 35 );
	corpse->value[4] = ch->temple;
        REMOVE_BIT(ch->act,PLR_CANLOOT);
	free_string(corpse->talked);
	if (IS_NPC(killer))
		corpse->talked = str_dup(killer->short_descr);
	else
		corpse->talked = str_dup(killer->name);
        if (!is_cabal(ch))
            corpse->owner = str_dup(ch->name);
        else
        {

            if (ch->gold > 1 || ch->silver > 1)
            {
                obj_to_obj(create_money(ch->gold, ch->silver), corpse);
		ch->gold = 0;
		ch->silver = 0;
            }
        }

        corpse->cost = 0;
    }

    corpse->level = ch->level;
     if (IS_SET(ch->act,ACT_UNDEAD) || IS_SET(ch->form,FORM_UNDEAD))
	SET_BIT(corpse->extra_flags,CORPSE_NO_ANIMATE);


    sprintf( buf, corpse->short_descr, name );
    free_string( corpse->short_descr );
    corpse->short_descr = str_dup( buf );

    sprintf( buf, corpse->description, name );
    free_string( corpse->description );
    corpse->description = str_dup( buf );

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
        bool floating = FALSE;

        obj_next = obj->next_content;
        if (obj->wear_loc != WEAR_BRAND)
	{
	obj_from_char( obj );
	}

        if (obj->item_type == ITEM_POTION)
            obj->timer = number_range(500,1000);
        if (obj->item_type == ITEM_SCROLL)
            obj->timer = number_range(1000,2500);
/* Area specific objects, remove these if you don't use the area */
	if (obj->pIndexData->vnum == OBJ_VNUM_HYDRA_SLAYER)
		obj->timer = number_range(500,1500);
	else if (obj->pIndexData->vnum == OBJ_VNUM_HYDRA_HEAD)
		obj->timer = number_range(300,900);

        if (IS_SET(obj->extra_flags,ITEM_ROT_DEATH) && !floating)
        {
            obj->timer = 1;
            REMOVE_BIT(obj->extra_flags,ITEM_ROT_DEATH);
        }
        REMOVE_BIT(obj->extra_flags,ITEM_VIS_DEATH);

	if (obj->item_type == ITEM_WEAPON)
	{
		if (IS_WEAPON_STAT(obj,WEAPON_LEADER))
		obj_to_char(obj,ch);
	}

	if (obj->pIndexData->limcount > obj->pIndexData->limtotal
		 && obj->pIndexData->limtotal > 0)
	{
	    act("$p flashes brightly and vaporizes.",ch,obj,NULL,TO_ROOM);
            extract_obj( obj );
	}
/*	if (IS_SET(obj->extra_flags,ITEM_FIXED) && !(IS_NPC(ch))) {
		act("$p flashes brightly and returns to its rightful owner!",ch,obj,NULL,TO_ROOM);
	}
*/
        else if (floating)
        {
            if (IS_OBJ_STAT(obj,ITEM_ROT_DEATH)) /* get rid of it! */
            {
                if (obj->contains != NULL)
                {
                    OBJ_DATA *in, *in_next;

                    act("$p evaporates,scattering its contents.",
                        ch,obj,NULL,TO_ROOM);
                    for (in = obj->contains; in != NULL; in = in_next)
                    {
                        in_next = in->next_content;
                        obj_from_obj(in);
                        obj_to_room(in,ch->in_room);
                    }
                 }
                 else
                    act("$p evaporates.",
                        ch,obj,NULL,TO_ROOM);
                 extract_obj(obj);
            }
            else
            {
                act("$p falls to the floor.",ch,obj,NULL,TO_ROOM);
                obj_to_room(obj,ch->in_room);
            }
        }
        else if (obj->wear_loc != WEAR_BRAND)
	{
	obj_to_obj( obj, corpse );
	}
	else if (obj->wear_loc != WEAR_BOUND)
	{
		obj_to_obj( obj, corpse );
	}
/*
        else if (!( (obj->wear_loc == WEAR_BRAND) ||
	((IS_SET(obj->extra_flags,ITEM_FIXED)) )  ) )
	{
            obj_to_obj( obj, corpse );
	}
*/
    }

    obj_to_room( corpse, ch->in_room );

    return;
}



/*
 * Improved Death_cry contributed by Diavolo.
 */
void death_cry( CHAR_DATA *ch )
{
    ROOM_INDEX_DATA *was_in_room;
    char *msg;
    int door;
    int vnum;

    vnum = 0;
    msg = "You hear $n's death cry.";

    switch ( number_bits(4))
    {
    case  0: msg  = "$n hits the ground ... DEAD.";                     break;
    case  1:
        if (ch->material == 0)
        {
            msg  = "$n splatters blood on your armor.";
            break;
        }
    case  2:
        if (IS_SET(ch->parts,PART_GUTS))
        {
            msg = "$n spills $s guts all over the floor.";
            vnum = OBJ_VNUM_GUTS;
        }
        break;
    case  3:
        if (IS_SET(ch->parts,PART_HEAD))
        {
            msg  = "$n's severed head plops on the ground.";
            vnum = OBJ_VNUM_SEVERED_HEAD;
        }
        break;
    case  4:
        if (IS_SET(ch->parts,PART_HEART))
        {
            msg  = "$n's heart is torn from $s chest.";
            vnum = OBJ_VNUM_TORN_HEART;
        }
        break;
    case  5:
        if (IS_SET(ch->parts,PART_ARMS))
        {
            msg  = "$n's arm is sliced from $s dead body.";
            vnum = OBJ_VNUM_SLICED_ARM;
        }
        break;
    case  6:
        if (IS_SET(ch->parts,PART_LEGS))
        {
            msg  = "$n's leg is sliced from $s dead body.";
            vnum = OBJ_VNUM_SLICED_LEG;
        }
        break;
    case 7:
        if (IS_SET(ch->parts,PART_BRAINS))
        {
            msg = "$n's head is shattered, and $s brains splash all over you.";
            vnum = OBJ_VNUM_BRAINS;
        }
    }

    if (IS_SET(ch->parts,PART_HEAD) && is_affected(ch,gsn_decapitate))
    {
       msg  = "$n's severed head plops on the ground.";
       vnum = OBJ_VNUM_SEVERED_HEAD;
    }

    act( msg, ch, NULL, NULL, TO_ROOM );

    if ( vnum != 0 )
    {
        char buf[MAX_STRING_LENGTH];
        OBJ_DATA *obj;
        char *name;

        name            = IS_NPC(ch) ? ch->short_descr : ch->name;
        obj             = create_object( get_obj_index( vnum ), 0 );
        obj->timer      = number_range( 4, 7 );

        sprintf( buf, obj->short_descr, name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( buf );

        sprintf( buf, obj->description, name );
        free_string( obj->description );
        obj->description = str_dup( buf );

        if (obj->item_type == ITEM_FOOD)
        {
            if (IS_SET(ch->form,FORM_POISON))
                obj->value[3] = 1;
            else if (!IS_SET(ch->form,FORM_EDIBLE))
                obj->item_type = ITEM_TRASH;
        }

        obj_to_room( obj, ch->in_room );
    }

    if ( IS_NPC(ch) )
        msg = "You hear something's death cry.";
    else
        msg = "You hear someone's death cry.";

    was_in_room = ch->in_room;
    for ( door = 0; door <= 5; door++ )
    {
        EXIT_DATA *pexit;

        if ( ( pexit = was_in_room->exit[door] ) != NULL
        &&   pexit->u1.to_room != NULL
        &&   pexit->u1.to_room != was_in_room )
        {
            ch->in_room = pexit->u1.to_room;
            act( msg, ch, NULL, NULL, TO_ROOM );
        }
    }
    ch->in_room = was_in_room;

    return;
}



void raw_kill( CHAR_DATA *ch,CHAR_DATA *victim )
{
    int i, cloak = 0;
//    char strsave[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    AFFECT_DATA af;
    CHAR_DATA *gch;
    CHAR_DATA *gch_next;
    CHAR_DATA *shadow;
    CHAR_DATA *zombie;
//    OBJ_DATA *etched;
    OBJ_DATA *trophy;
    OBJ_DATA *duelcorpse;
    ROOM_INDEX_DATA *arena_room;
    ROOM_INDEX_DATA *old_room;
    char buf[MAX_STRING_LENGTH];
    char wizbuf[MAX_STRING_LENGTH];
    bool unholy=0;

    stop_fighting( victim, TRUE );

    if (IS_NPC(victim) && (victim->pIndexData->vnum == MOB_VNUM_BLACK_STEED
    || victim->pIndexData->vnum == MOB_VNUM_RED_STEED
    || victim->pIndexData->vnum == MOB_VNUM_BLUE_STEED
    || victim->pIndexData->vnum == MOB_VNUM_GREEN_STEED))
    {
		do_dismount(victim->master,"");
		act("$n vanishes into the abyss!",victim,0,0,TO_ROOM);
	}

    if(IS_NPC(victim) && (victim->pIndexData->vnum == MOB_VNUM_PROTECTOR
		|| victim->pIndexData->vnum == MOB_VNUM_ACADIAN
		|| victim->pIndexData->vnum == MOB_VNUM_COVEN
        || victim->pIndexData->vnum == MOB_VNUM_BRIAR
		|| victim->pIndexData->vnum == MOB_VNUM_RAVAGER))
    do_cb(victim,"Alas, my life has come to an end...");

    if (IS_NPC(victim) && victim->pIndexData->vnum == MOB_VNUM_DECOY)
    {
death_cry(victim);
act("$n vanishes in a poof of smoke!",victim,0,0,TO_ROOM);
    }
    else
    {
    death_cry( victim );
/* Imm deaths */

    if (!IS_NPC(victim) && IS_IMMORTAL(victim))
    {
        if (victim->pcdata->imm_death)
        {
            act("$t.",victim,NULL,NULL,TO_ROOM);
            act("$t.",victim,NULL,NULL,TO_CHAR);
        }
        else
        {
            act("$n's skull glows brightly and $n rises from $s ashes!",victim,0,0,TO_ROOM);
            send_to_char("Your skull glows brightly and you rise from your ashes!\n\r",victim);
        }
        return;
    }

/* ARENA DEATH  -- Dioxide */
if (!IS_NPC(victim)) {
	if (arena) {

		/* MAIN STUFF */
		sprintf(buf,"%s is killed by %s!",victim->name,ch->name);
		do_arena_echo(buf);
		char_from_room(victim);
		location = get_room_index(26000);
		char_to_room(victim,location);
		victim->ghost = 1;

		affect_strip(victim,gsn_plague);
            affect_strip(victim,gsn_poison);
            affect_strip(victim,gsn_blindness);
            affect_strip(victim,gsn_sleep);
            affect_strip(victim,gsn_curse);
            affect_strip(victim,gsn_decapitate);
            affect_strip(victim,gsn_shackles);
            affect_strip(victim,gsn_boneshatter);

            victim->hit 	= victim->max_hit;
            victim->mana   = victim->max_mana;
            victim->move	= victim->max_move;
            update_pos( victim);
		send_to_char("You have died and turned into an invincible ghost a few seconds.\n\r",victim);
		act("$n suddenly appears as a ghost.",victim,0,0,TO_ROOM);

		/* TROPHY CODE */
		trophy = create_object(get_obj_index(OBJ_VNUM_ARENA_TROPHY),0);
		sprintf( buf, trophy->short_descr, victim->name);
		free_string( trophy->short_descr );
		trophy->short_descr = str_dup( buf );
		sprintf( buf, trophy->description, victim->name);
		free_string( trophy->description );
		trophy->description = str_dup( buf );
		obj_to_char(trophy,ch);
		act("You receive $p.",ch,trophy,0,TO_CHAR);
		act("$n receives $p.",ch,trophy,0,TO_ROOM);

		return;
	}
}

temp_death_log(ch,victim);

//Bounty payoff for Ancient.
if (!IS_NPC(victim)
&& (ch->pcdata->target == victim->name)
&& victim->pcdata->bounty > 0)
{
	trophy = create_object(get_obj_index(OBJ_VNUM_ANCIENT_SCALP),0);
	sprintf( buf, trophy->short_descr, victim->name);
	free_string( trophy->short_descr );
	trophy->short_descr = str_dup( buf);
	sprintf( buf, trophy->description, victim->name);
	free_string( trophy->description );
	trophy->description = str_dup(buf);
	obj_to_char(trophy,ch);
	act("You violently carve off their scalp.",ch,0,0,TO_CHAR);
	act("$n violently carves off their scalp.",ch,0,0,TO_ROOM);
	sprintf(buf,"You recive a %d gold bounty, for killing %s.\n\r",
	victim->pcdata->bounty, victim->name);
	send_to_char("Assignment completed.\n\r", ch );
	send_to_char(buf, ch);
	ch->gold += victim->pcdata->bounty;
	victim->pcdata->bounty =0;
	affect_strip(ch, gsn_target);
	ch->pcdata->target = NULL;
}

/* AP unholy bless check --Dioxide */

if (check_unholy_bless(ch,victim)) {
	unholy = TRUE;
} else {
	unholy = FALSE;
}

if (is_affected(victim,gsn_unholy_timer)) {
	unholy = TRUE;
}

//strap = get_eq_char(victim, WEAR_STRAPPED);

/*if (strap != NULL) {
	do_strap(victim,strap);
}*/
    make_corpse( ch,victim );
    //victim->noregen_dam = 0;
    }

        sprintf( wizbuf, "%s got toasted by %s at %s [room %d]",
            (IS_NPC(victim) ? victim->short_descr : (victim->original_name ? victim->original_name : victim->name)),
            (IS_NPC(ch)     ? ch->short_descr     : (ch->original_name     ?     ch->original_name : ch->name)),
            ch->in_room->name, ch->in_room->vnum);

        if (IS_NPC(victim))
            wiznet(wizbuf,NULL,NULL,WIZ_MOBDEATHS,0,0);
        else
            wiznet(wizbuf,NULL,NULL,WIZ_DEATHS,0,0);

    for (gch = char_list; gch != NULL; gch = gch_next)
    {
	gch_next = gch->next;
  	if (IS_NPC(gch) && gch->master == victim)
		extract_char(gch,TRUE);
    }
	if (IS_NPC(victim) && victim->pIndexData->vnum == MOB_VNUM_STALKER) {
		if (victim->last_fought != NULL) {
		sprintf(buf,"enforcer My life has come to an end; %s remains at large.",victim->last_fought->name);
		do_ccb(victim,buf);
		}
	}

    for (gch = char_list; gch != NULL; gch = gch_next)
    {
	gch_next = gch->next;
  	if (IS_NPC(gch)
	&& (gch->pIndexData->vnum == MOB_VNUM_STALKER)
	&& (gch->last_fought == victim)) {
		sprintf(buf,"enforcer %s has been terminated.",victim->name);
		do_ccb(gch,buf);
		extract_char(gch,TRUE);
		act("$N slowly fades away.",victim,NULL,gch,TO_NOTVICT);
	}
   }


/* This next bit handles the Nightfall spell */
    if (victim->nightfall != NULL
	&& victim->nightfall->in_room == victim->in_room)
	{
act("$n's soul is torn from it's body and formed into a shadow!",victim,0,0,TO_ROOM);
act("$n rips your soul away and forms a shadow!",victim->nightfall,0,victim,TO_VICT);
shadow = create_mobile(get_mob_index(MOB_VNUM_NIGHTFALL));
char_to_room(shadow,victim->in_room);
free_string(shadow->short_descr);
free_string(shadow->long_descr);
sprintf(buf,"the shadow of %s",
IS_NPC(victim) ? victim->short_descr : victim->name);
shadow->short_descr = str_dup(buf);
sprintf(buf,"The shadow of %s is hovering here in silence.\n\r",
IS_NPC(victim) ? victim->short_descr : victim->name);
shadow->long_descr = str_dup(buf);

shadow->max_hit = victim->nightfall->max_hit;
shadow->hit = victim->max_hit;
shadow->damroll = victim->level/2;
shadow->hitroll = victim->level/3;
shadow->level = victim->level;
shadow->armor[0] = -victim->level*4;
shadow->armor[1] = -victim->level*4;
shadow->armor[2] = -victim->level*4;
shadow->armor[3] = -victim->level*3;
SET_BIT(shadow->comm,COMM_QUIET);
SET_BIT(shadow->affected_by,AFF_CHARM);
shadow->leader = victim->nightfall;
shadow->master = victim->nightfall;
	}

    if (victim->nightfall != NULL)
	{
if (victim->nightfall->in_room == victim->in_room)
act("You grasp onto $n's soul for you own uses!",victim,0,victim->nightfall,TO_VICT);
else
act("Your grasp on $n's soul fades away.",victim,0,victim->nightfall,TO_VICT);
victim->nightfall = NULL;
	}

/* Quick score time before we dump any NPCs..for 'ratings' command */
if (!IS_NPC(ch))
	{
		if (!IS_NPC(victim))
		{
			ch->pcdata->kills[PK_KILLS]++;
			if (victim->alignment > 0) {
				ch->pcdata->kills[PK_GOOD]++;
			} else if (victim->alignment == 0) {
				ch->pcdata->kills[PK_NEUTRAL]++;
			} else if (victim->alignment < 0) {
				ch->pcdata->kills[PK_EVIL]++;
			}
			victim->pcdata->killed[PK_KILLED]++;
		}
	} else {
		if (!IS_NPC(victim)) {
			victim->pcdata->killed[MOB_KILLED]++;
		}
	}

    if ( IS_NPC(victim) )
    {
        victim->pIndexData->killed++;
        kill_table[URANGE(0, victim->level, MAX_LEVEL-1)].killed++;
        extract_char( victim, TRUE );
        return;
    }


    while(victim->affected)
       	affect_remove( victim, victim->affected );
    extract_char( victim, FALSE );
    if(is_affected(victim,gsn_cloak_form)) cloak=1;
    while(victim->affected)
       	affect_remove( victim, victim->affected );
    victim->affected_by = race_table[victim->race].aff;
    for (i = 0; i < 4; i++)
        victim->armor[i]= 100;
    victim->position    = POS_RESTING;
    victim->hit         = victim->pcdata->perm_hit/2;
    victim->mana        = victim->pcdata->perm_mana/2;
    victim->move        = victim->pcdata->perm_move/2;
    victim->pcdata->condition[COND_HUNGER] = 48;
    victim->pcdata->condition[COND_THIRST] = 48;
    victim->pcdata->condition[COND_DRUNK] = 0;
    victim->pcdata->condition[COND_FULL] = 0;
    victim->pcdata->condition[COND_DEHYDRATED] = 0;
    victim->pcdata->condition[COND_STARVING] = 0;
    REMOVE_BIT(victim->act,PLR_CRIMINAL);
    victim->ghost = 20;
    send_to_char("You have died and turned into an invincible ghost for a few minutes.\n\r",victim);

   if(cloak==1)
	location=get_room_index(3808);
    else if (victim->temple != 0)
	    location = get_room_index(victim->temple);
    else
	location = get_room_index(ROOM_VNUM_TEMPLE);

    if (location == NULL)
	location = get_room_index(3014);
    if (ch->in_room != NULL && ch->in_room->vnum == 1212)
	location = get_room_index(3014);

    char_from_room(victim);
    char_to_room(victim,location);
	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
    af.type = skill_lookup("infravision");
    af.duration = 20;
    af.location = 0;
    af.bitvector = AFF_INFRARED;
    af.modifier = 0;
    af.level=victim->level;
    affect_to_char(victim,&af);

    if (unholy) {
	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
    af.type = skill_lookup("unholy guard");
    af.duration = 36;
    af.location = 0;
    af.modifier = 0;
    af.level=victim->level;
    affect_to_char(victim,&af);
    }

	victim->last_fight_name = NULL;
	(int)victim->last_fight_time = NULL;

/* Con loss per 4 deaths. If you don't want this, and particularly perm
death by going below 4 con, then remove this code...(Ceran)
*/
	if (!IS_NPC(victim))
	{
		victim->pcdata->death_count++;
		if ( (victim->pcdata->death_count >= 10)
		&& !IS_IMMORTAL(victim)
		&& !(victim->perm_stat[STAT_CON] < 3))
		{
			victim->perm_stat[STAT_CON] -= 1;
			victim->pcdata->death_count = 0;
			send_to_char("With this death you feel your vitality draining away from you.\n\r",victim);
		}
		if (victim->perm_stat[STAT_CON] < 5) {
			send_to_char("The banshee wails as you feel closer to a permanent death.\n\r",victim);
		}
		if (victim->perm_stat[STAT_CON] < 3) {
			SET_BIT(victim->act, PLR_DENY);
			perm_death_log(victim,2);
			send_to_char("You gasp your final breaths. Your vitality has faded.\n\r",victim);
			wiznet("$N dies of constitution loss.",victim,NULL,0,0,get_trust(victim));
			stop_fighting(victim,TRUE);
			victim->pause = 0;
			save_char_obj(victim);
			do_quit(victim, "");
		}
	}

    if(!IS_NPC(ch) && ch->in_room->vnum==ROOM_FORCE_DUEL)
	{
		init_affect(&af);
	      af.where = TO_AFFECTS;
		af.aftype = AFT_POWER;
            af.type = gsn_honorable_combat;
	      af.duration = 50;
	      af.location = 0;
	      af.modifier = 0;
		af.bitvector= 0;
	      af.level=ch->level;
	      affect_to_char(victim,&af);
		char_from_room(ch);
		old_room=get_room_index(ch->pcdata->old_room);
		arena_room=get_room_index(ROOM_FORCE_DUEL);
		char_to_room(ch,old_room);
		act("$n arrives in a blaze of white light, having triumphed honorably over $N!\n\r",ch,0,victim,TO_ROOM);
		for (duelcorpse = arena_room->contents; duelcorpse != NULL; duelcorpse = duelcorpse->next)
    		{
			if(duelcorpse->in_room==arena_room)
			{
	      		obj_from_room(duelcorpse);
				obj_to_room(duelcorpse,old_room);
			}
    		}
            for (zombie = arena_room->people; zombie != NULL; zombie = zombie->next_in_room)
		{
		      if (zombie!=NULL && IS_NPC(zombie) && IS_AFFECTED(zombie,AFF_CHARM) && zombie->pIndexData->vnum==MOB_VNUM_ZOMBIE)
			{
				char_from_room(zombie);
				char_to_room(zombie,old_room);
			}
    		}
	}
    do_look(victim,"auto");
    return;
}



void group_gain( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *gch;
    CHAR_DATA *lch;
    int xp;
    int members;
    int group_levels;

    /*
     * Monsters don't get kill xp's or alignment changes.
     * P-killing doesn't help either.
     * Dying of mortal wounds or poison doesn't give xp to anyone!
     */
    if ( victim == ch )
        return;
    if (!IS_NPC(victim)) /* No xp for PKs */
	return;

    members = 0;
    group_levels = 0;
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
        if ( is_same_group( gch, ch ) )
        {
            if(!IS_NPC(gch)) members++;
            group_levels+=gch->level;
            if(IS_NPC(gch)) group_levels+=gch->level;
        }
    }

    if ( members == 0 )
    {
        members = 1;
    }

    lch = (ch->leader != NULL) ? ch->leader : ch;

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
        OBJ_DATA *obj;
        OBJ_DATA *obj_next;
	bool owner = FALSE;

        if ( !is_same_group( gch, ch ) || IS_NPC(gch))
            continue;

        if ( gch->level - lch->level > 8 )
        {
            send_to_char( "You are too high for this group.\n\r", gch );
            continue;
        }

        if ( gch->level - lch->level < -8 )
        {
            send_to_char( "You are too low for this group.\n\r", gch );
            continue;
        }


        xp = xp_compute( gch, victim, members, group_levels );
        sprintf( buf, "You receive %d experience points.\n\r", xp );
        send_to_char( buf, gch );
        gain_exp( gch, xp );

        for ( obj = ch->carrying; obj != NULL; obj = obj_next )
        {
            obj_next = obj->next_content;
            if ( obj->wear_loc == WEAR_NONE )
                continue;

	if (is_affected(ch,gsn_cloak_form))
	{
	ch->unholy_name = str_dup(ch->original_name);
	}
	else
	{
	ch->unholy_name = str_dup(ch->name);
	}

	if (str_cmp(obj->owner,ch->unholy_name) )
	{
	owner = TRUE;
	}

            if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
            ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
            ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) )
	    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_LAWFUL)  &&
		 (ch->pcdata->ethos > 0))
	    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUT)    &&
	         (ch->pcdata->ethos == 0))
	    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_CHAOTIC) &&
		 (ch->pcdata->ethos < 0))
	    ||   ( IS_WEAPON_STAT(obj,WEAPON_UNHOLY)  && (owner)))
            {
                act( "You are zapped by $p.", ch, obj, NULL, TO_CHAR );
                act( "$n is zapped by $p.",   ch, obj, NULL, TO_ROOM );
                obj_from_char( obj );
                obj_to_room( obj, ch->in_room );
            }
        }
    }

    return;
}



/*
 * Compute xp for a kill.
 * Also adjust alignment of killer.
 * Edit this function to change xp computations.
 */
int xp_compute( CHAR_DATA *gch, CHAR_DATA *victim, int group_amount, int glevel )
{
    int xp,base_exp;
    int level_range;
    double mult;

    mult=((double)(gch->level)/glevel)*group_amount;
    if(mult>=1) { mult=(1+mult)/2;
    } else {
       mult=mult*mult;
    }
    mult = URANGE(.25,mult,1.1);

    level_range = victim->level - gch->level;

    /* compute the base exp */
    switch (level_range)
    {
        default :       base_exp =   0;         break;
        case -9 :       base_exp =   2;         break;
        case -8 :       base_exp =   4;         break;
        case -7 :       base_exp =   7;         break;
        case -6 :       base_exp =   12;         break;
        case -5 :       base_exp =  14;         break;
        case -4 :       base_exp =  25;         break;
        case -3 :       base_exp =  36;         break;
        case -2 :       base_exp =  55;         break;
        case -1 :       base_exp =  70;         break;
        case  0 :       base_exp =  88;         break;
        case  1 :       base_exp =  110;         break;
        case  2 :       base_exp = 131;         break;
        case  3 :       base_exp = 153;         break;
        case  4 :       base_exp = 165;         break;
    }

    if (level_range > 4)
        base_exp = 165 + 15 * (level_range - 4);
    if(mult<1&&level_range>4)
        base_exp = (2*base_exp+165)/3;

    if (IS_SET(victim->act,ACT_NOALIGN))
        xp = base_exp;

    else if (gch->alignment > 0)
    {
        if (victim->alignment < 0)
            xp = (base_exp *4)/3;

        else if (victim->alignment > 0)
            xp = -30;

        else
            xp = base_exp;
    }

    else if (gch->alignment < 0) /* for baddies */
    {
        if (victim->alignment > 0)
            xp = (base_exp * 4)/3;

        else if (victim->alignment < 0)
            xp = base_exp/2;

        else
            xp = base_exp;
    }


    else /* neutral */
    {
            xp = base_exp;
    }

    xp = (xp*2)/3;

    xp=xp*mult;
    xp = number_range (xp, xp * 5/4);

    /* adjust for grouping */
    if (group_amount == 2)
        xp = (xp * 5)/3;
    if (group_amount == 3)
        xp = (xp * 7)/3;
    if (group_amount > 3)
	xp /= (group_amount - 2);

    return xp;
}


void dam_message( CHAR_DATA *ch, CHAR_DATA *victim,int dam,int dt,bool immune )
{
    char buf1[256], buf2[256], buf3[256];
    OBJ_DATA *wield;
    const char *vs;
    const char *vp;
    const char *attack;
    char punct;


    if (ch == NULL || victim == NULL)
        return;

	if ( dam ==   0 )
        {
		vs = "{Rmiss{x";
		vp = "{Rmisses{x";
	}
    	else if ( dam <=   2 )
    	{
    		vs = "{Rscratch{x";
    		vp = "{Rscratches{x";
    	}
	else if ( dam <=   4 )
	{
		vs = "{Rgraze{x";
		vp = "{Rgrazes{x";
	}
	else if ( dam <=   7 )
	{
		vs = "{Rhit{x";
		vp = "{Rhits{x";
	}
	else if ( dam <=  11 )
	{
		vs = "{Rinjure{x";
		vp = "{Rinjures{x";
	}
	else if ( dam <=  15 )
	{
		vs = "{Rwound{x";
		vp = "{Rwounds{x";
	}
	else if ( dam <=  20 )
	{
		 vs = "{Rmaul{x";
		 vp = "{Rmauls{x";
	}
	else if ( dam <=  25 )
	{
		vs = "{Rdecimate{x";
		vp = "{Rdecimates{x";
	}
	else if ( dam <=  30 )
	{
		vs = "{Rdevastate{x";
		vp = "{Rdevastates{x";
	}
	else if ( dam <=  37 )
	{
		vs = "{Rmaim{x";
		vp = "{Rmaims{x";
	}
	else if ( dam <=  45 )
	{
		vs = "{RMUTILATE{x";
		vp = "{RMUTILATES{x";
	}
	else if ( dam <=  55 )
	{
		vs = "{REVISCERATE{x";
		vp = "{REVISCERATES{x";
	}
	else if ( dam <=  65 )
	{
		vs = "{RDISMEMBER{x";
		vp = "{RDISMEMBERS{x";
	}
	else if ( dam <=  85 )
	{
		vs = "{RMASSACRE{x";
		vp = "{RMASSACRES{x";
	}
	else if ( dam <=  100 )
	{
		vs = "{RMANGLE{x";
		vp = "{RMANGLES{x";
	}
	else if ( dam <= 135 )
	{
		vs = "{R*** DEMOLISH ***{x";
                vp = "{R*** DEMOLISHES ***{x";
        }
	else if ( dam <= 160 )
	{
		vs = "{R*** DEVASTATE ***{x";
                vp = "{R*** DEVASTATES ***{x";
        }
	else if ( dam <= 250)
	{
		vs = "{R=== OBLITERATE ==={x";
                vp = "{R=== OBLITERATES ==={x";
        }
	else if ( dam <= 330)
	{
		vs = "{R>>> ANNIHILATE <<<{x";
                vp = "{R>>> ANNIHILATES <<<{x";
        }
	else if ( dam <= 380)
	{
		vs = "{R<<< ERADICATE >>>{x";
                vp = "{R<<< ERADICATES >>>{x";
        }
	else
	{
		vs = "{Rdo UNSPEAKABLE things to{x";
                vp = "{Rdoes UNSPEAKABLE things to{x";
        }

    punct   = (dam <= 33) ? '.' : '!';


    if (dt  == gsn_dual_wield || dt == gsn_tertiary_wield)
	{
if (dt == gsn_dual_wield)
    	wield = get_eq_char(ch,WEAR_DUAL_WIELD);
else
	wield = get_eq_char(ch,WEAR_TERTIARY_WIELD);
	dt = TYPE_HIT;
	if (wield != NULL)
		dt += wield->value[3];
	else
		dt += ch->dam_type;
	}

    if ( dt == TYPE_HIT )
    {
        if (ch == victim)
        {
            sprintf( buf1, "$n %s $melf%c",vp,punct);
            sprintf( buf2, "You %s yourself%c",vs,punct);
        }
        else
        {
            sprintf (buf1, "$n %s $N%c", vp, punct);
            sprintf( buf2, "You %s $N%c", vs, punct );
            sprintf( buf3, "$n %s you%c", vp, punct );
        }
    }
    else
    {
        if ( dt >= 0 && dt < MAX_SKILL )
            attack      = skill_table[dt].noun_damage;
        else if ( dt >= TYPE_HIT
        && dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
            attack      = attack_table[dt - TYPE_HIT].noun;
        else
        {
            bug( "Dam_message: bad dt %d.", dt );
            dt  = TYPE_HIT;
            attack  = attack_table[0].name;
        }

        if (immune)
        {
            if (ch == victim)
            {
                sprintf(buf1,"$n is unaffected by $s own %s.",attack);
                sprintf(buf2,"Luckily, you are immune to that.");
            }
            else
            {
                sprintf(buf1,"$N is unaffected by $n's %s!",attack);
                sprintf(buf2,"$N is unaffected by your %s!",attack);
                sprintf(buf3,"$n's %s is powerless against you.",attack);
            }
        }
	    else
        {
            if (ch == victim)
            {
		        sprintf( buf1, "$n's %s %s $m%c",  attack, vp, punct );
                sprintf( buf2, "Your %s %s you%c",attack,vp,punct);
            }
            else
            {
			  	sprintf( buf1, "$n's %s %s $N%c",  attack, vp, punct );
			  	sprintf( buf2, "Your %s %s $N%c",  attack, vp, punct );
			   	sprintf( buf3, "$n's %s %s you%c", attack, vp, punct );

			}

        }
    }

    if (ch == victim)
    {
        act (buf1, ch, NULL, NULL, TO_ROOM);
        act(buf2,ch,NULL,NULL,TO_CHAR);
    }
    else
    {
		act( buf1, ch, NULL, victim, TO_NOTVICT);
		act( buf2, ch, NULL, victim, TO_CHAR );
		act( buf3, ch, NULL, victim, TO_VICT );
	}

/* Don't stop players entering guilds or quiting for
   starvation/poison/plague damage
*/
    if (dt != gsn_poison && dt != gsn_plague
	&& dt != gsn_starvation
	&& dt != gsn_dehydrated)
    {
    if (!IS_IMMORTAL(ch))
	{
	if (!IS_NPC(victim))
	{
		ch->pause = 10;
	}
	else
	{
		ch->pause = 1;
	}
	}
    if (!IS_IMMORTAL(victim))
	{
	if (!IS_NPC(ch))
	{
		victim->pause = 10;
	}
	else
	{
		ch->pause = 1;
	}
	}
    }
    return;
}



/*
 * Disarm a creature.
 * Caller must check for successful attack.
 */
void disarm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    OBJ_DATA *obj;
    OBJ_DATA *secondary;


    if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
        return;

    if (IS_AFFECTED(ch,AFF_BLIND))

    {
        act("You can't see the person to disarm them!",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (IS_NPC(ch) && (IS_AFFECTED(ch,AFF_BLIND)))
       return;

    if ( IS_OBJ_STAT(obj,ITEM_NOREMOVE) || IS_OBJ_STAT(obj,ITEM_NODISARM))
    {
        act("$S weapon won't budge!",ch,NULL,victim,TO_CHAR);
        act("$n tries to disarm you, but your weapon won't budge!",
            ch,NULL,victim,TO_VICT);
        act("$n tries to disarm $N, but fails.",ch,NULL,victim,TO_NOTVICT);
        return;
    }

    act( "$n DISARMS you and sends your weapon flying!",
         ch, NULL, victim, TO_VICT    );
    act( "You disarm $N!",  ch, NULL, victim, TO_CHAR    );
    act( "$n disarms $N!",  ch, NULL, victim, TO_NOTVICT );

    obj_from_char( obj );
    if ( IS_OBJ_STAT(obj,ITEM_NODROP) || IS_OBJ_STAT(obj,ITEM_INVENTORY) )
        obj_to_char( obj, victim );
    else
    {
        obj_to_room( obj, victim->in_room );
        if (IS_NPC(victim) && victim->wait == 0 && can_see_obj(victim,obj))
            get_obj(victim,obj,NULL);
    }

reslot_weapon(victim);

    if ((secondary = get_eq_char(victim,WEAR_DUAL_WIELD)) != NULL)
    {
	unequip_char(victim,secondary);
	equip_char(victim,secondary,WEAR_WIELD);
    }
    return;
}

void do_berserk( CHAR_DATA *ch, char *argument)
{
    int chance, hp_percent;

    if ((chance = get_skill(ch,gsn_berserk)) == 0
    ||  (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BERSERK))
    ||  (!IS_NPC(ch)
    &&   ch->level < skill_table[gsn_berserk].skill_level[ch->class]))
    {
        send_to_char("You turn red in the face, but nothing happens.\n\r",ch);
        return;
    }

    if (IS_AFFECTED(ch,AFF_BERSERK) || is_affected(ch,gsn_berserk)
    ||  is_affected(ch,skill_lookup("frenzy")))
    {
        send_to_char("You get a little madder.\n\r",ch);
        return;
    }

    if (IS_AFFECTED(ch,AFF_CALM))
    {
        send_to_char("You're feeling to mellow to berserk.\n\r",ch);
        return;
    }

    if (ch->mana < 50)
    {
        send_to_char("You can't get up enough energy.\n\r",ch);
        return;
    }

    /* modifiers */

    /* fighting */
    if (ch->position == POS_FIGHTING)
        chance += 10;

    /* damage -- below 50% of hp helps, above hurts */
    if (ch->max_hit == 0)
	hp_percent = 0;
     else
    hp_percent = 100 * ch->hit/ch->max_hit;
    chance += 25 - hp_percent/2;

    if (number_percent() < chance)
    {
        AFFECT_DATA af;

        WAIT_STATE(ch,PULSE_VIOLENCE);
        ch->mana -= 50;
        ch->move /= 1.2
;

        /* heal a little damage */
        ch->hit += (ch->drain_level + ch->level) * 2;
        ch->hit = UMIN(ch->hit,ch->max_hit);

        send_to_char("Your pulse races as you are consumed by rage!\n\r",ch);
        act("$n gets a wild look in $s eyes.",ch,NULL,NULL,TO_ROOM);
        check_improve(ch,gsn_berserk,TRUE,2);

	init_affect(&af);
        af.where        = TO_AFFECTS;
        af.type         = gsn_berserk;
        af.level        = ch->level + ch->drain_level;
	af.duration 	= (ch->drain_level + ch->level /2);
        af.modifier     = UMAX(1,(ch->drain_level + ch->level)/5);
        af.bitvector    = AFF_BERSERK;
	af.aftype = AFT_SKILL;

        af.location     = APPLY_HITROLL;
        affect_to_char(ch,&af);

        af.location     = APPLY_DAMROLL;
        affect_to_char(ch,&af);

	af.modifier = (ch->drain_level + ch->level);
        af.location     = APPLY_AC;
        affect_to_char(ch,&af);
    }

    else
    {
        WAIT_STATE(ch,PULSE_VIOLENCE);
        ch->mana -= 25;

        send_to_char("Your pulse speeds up, but nothing happens.\n\r",ch);
        check_improve(ch,gsn_berserk,FALSE,2);
    }
}

void do_bash( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int dam;
    int chance;
    char buf[MAX_STRING_LENGTH];

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_bash)) == 0
    ||   (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BASH) && !check_rangermob(ch))
    ||   (!IS_NPC(ch)
    &&    ch->level < skill_table[gsn_bash].skill_level[ch->class]))
    {
        send_to_char("Bashing? What's that?\n\r",ch);
        return;
    }

    if (IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM)
    && !check_rangermob(ch))
	return;

    if (arg[0] == '\0')
    {
        victim = ch->fighting;
        if (victim == NULL)
        {
            send_to_char("But you aren't fighting anyone!\n\r",ch);
            return;
        }
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    if (victim->position < POS_FIGHTING)
    {
        act("You'll have to let $M get back up first.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (victim == ch)
    {
        send_to_char("You try to bash your brains out, but fail.\n\r",ch);
        return;
    }

    if (is_safe(ch,victim))
        return;

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
        act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (is_affected(victim,gsn_protective_shield))
    {
	act("$N's bash seems to slide around you.",victim,0,ch,TO_CHAR);
	act("$N's bash seems to slide around $n.",victim,0,ch,TO_NOTVICT);
	act("Your bash seems to slide around $n.",victim,0,ch,TO_VICT);
	WAIT_STATE(ch,PULSE_VIOLENCE*.5);
	return;
    }

    if (is_affected(victim,gsn_asscounter))
    {
	act("You deftly sidestep $N's bash, throwing them to the ground.",victim,0,ch,TO_CHAR);
        act("$n quickly dodges $N's bash, throwing $s down in the process.",victim,0,ch,TO_NOTVICT);
        act("$n deftly dodges your bash, throwing you to the ground in the process.",victim,0,ch,TO_VICT);
        WAIT_STATE(ch,2 * PULSE_VIOLENCE);
        dam = victim->level*2 + number_percent()*2 - 50;
        damage_old(victim,ch,dam,gsn_throw,DAM_BASH,TRUE);
        affect_strip( victim, gsn_asscounter );
	return;
    }

    if (is_affected(victim,gsn_wraithform))
    {
	act("$N's tries to bash you but stumbles through thin air.",victim,0,ch,TO_CHAR);
	act("$N's tries to bash $n but stumbles through thin air.",victim,0,ch,TO_NOTVICT);
	act("You stumble as you try to bash $n but stumble through thin air.",victim,0,ch,TO_VICT);
	WAIT_STATE(ch,PULSE_VIOLENCE*1);
	return;
    }

    if(is_affected(ch,gsn_shadowstrike)) {
	send_to_char("The shadowy arms restrict you.\n\r",ch);
	act("$N's tries to bash, but is restrained by shadowy arms!",ch,NULL,NULL,TO_ROOM);
	WAIT_STATE(ch,20);
	return;
    }

    /* modifiers */

    /* size  and weight */
    chance += ch->carry_weight / 250;
    chance -= victim->carry_weight / 200;

    if (ch->size < victim->size)
        chance += (ch->size - victim->size) * 20;
    else
        chance += (ch->size - victim->size) * 10;


    /* stats */
    chance += get_curr_stat(ch,STAT_STR);
    chance -= (get_curr_stat(victim,STAT_DEX) * 4)/3;
    chance -= GET_AC(victim,AC_BASH) /25;
    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || is_affected(ch,skill_lookup("haste")))
        chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || is_affected(victim,skill_lookup("haste")))
        chance -= 40;

    /* level */
    chance += (ch->drain_level + ch->level - victim->level - victim->drain_level);
	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (victim->fighting == NULL || ch->fighting == NULL))
	{
	sprintf(buf,"Help! %s is bashing me!",PERS(ch,victim));
	do_myell(victim,buf);
	}


    if (!IS_NPC(victim)
        && chance < get_skill(victim,gsn_dodge) )
    {   /*
        act("$n tries to bash you, but you dodge it.",ch,NULL,victim,TO_VICT);
        act("$N dodges your bash, you fall flat on your face.",ch,NULL,victim,TO_CHAR);
        WAIT_STATE(ch,PULSE_VIOLENCE*1.5);
        return;*/
        chance -= 3 * (get_skill(victim,gsn_dodge) - chance);
    }

    /* now the attack */
    if (number_percent() < chance )
    {

        act("$n sends you sprawling with a powerful bash!",
                ch,NULL,victim,TO_VICT);
        act("You slam into $N, and send $M flying!",ch,NULL,victim,TO_CHAR);
        act("$n sends $N sprawling with a powerful bash.",
                ch,NULL,victim,TO_NOTVICT);
        check_improve(ch,gsn_bash,TRUE,1);
        WAIT_STATE(victim, 2 * PULSE_VIOLENCE);
        damage_old(ch,victim,number_range(2,2 + 2 * ch->size + chance/20),gsn_bash,
       DAM_BASH,TRUE);

        WAIT_STATE(ch,skill_table[gsn_bash].beats);
        victim->position = POS_RESTING;
	if (!IS_NPC(ch) && (ch->class == CLASS_WARRIOR)
	&& (number_percent() < ch->level))
		ch->pcdata->sp++;
    }
    else
    {
        damage_old(ch,victim,0,gsn_bash,DAM_BASH,FALSE);
        act("You fall flat on your face!",
            ch,NULL,victim,TO_CHAR);
        act("$n falls flat on $s face.",
            ch,NULL,victim,TO_NOTVICT);
        act("You evade $n's bash, causing $m to fall flat on $s face.",
            ch,NULL,victim,TO_VICT);
	  check_cheap_shot(victim,ch);
        check_improve(ch,gsn_bash,FALSE,1);

        ch->position = POS_RESTING;
        WAIT_STATE(ch,skill_table[gsn_bash].beats);
    }
        check_killer(ch,victim);
}

void do_dirt( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;
    char buf[MAX_STRING_LENGTH];

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_dirt)) == 0
    ||   (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_KICK_DIRT) && ch->pIndexData->vnum!=MOB_VNUM_TEMPLAR)
    ||   (!IS_NPC(ch)
    &&    ch->level < skill_table[gsn_dirt].skill_level[ch->class]))
    {
        send_to_char("You get your feet dirty.\n\r",ch);
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
	send_to_char("Their eyes are closed.\n\r",ch);
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
    if (is_affected(victim, gsn_shield_of_dust))
    {
	act("Your dirt is blocked by $N's shield of dust!",ch,NULL,victim,TO_CHAR);
	act("$n kicks dirt at you but it is stoped by your shield of dust!", ch,NULL,victim,TO_VICT);
	act("$n's dirt is deflected by $N's shield of dust!",ch,NULL,victim,TO_NOTVICT);
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

    /* terrain */

    switch(ch->in_room->sector_type)
    {
        case(SECT_INSIDE):              chance -= 20;   break;
        case(SECT_CITY):                chance -= 10;   break;
        case(SECT_FIELD):               chance +=  5;   break;
        case(SECT_FOREST):                              break;
        case(SECT_HILLS):                               break;
        case(SECT_MOUNTAIN):            chance -= 10;   break;
        case(SECT_WATER_SWIM):          chance  =  0;   break;
        case(SECT_WATER_NOSWIM):        chance  =  0;   break;
        case(SECT_AIR):                 chance  =  0;   break;
        case(SECT_DESERT):              chance += 10;   break;
    }

    if (chance == 0)
    {
        send_to_char("There isn't any dirt to kick.\n\r",ch);
        return;
    }

    /* now the attack */
    if (number_percent() < chance)
    {
        AFFECT_DATA af;
        act("$n is blinded by the dirt in $s eyes!",victim,NULL,NULL,TO_ROOM);
        act("$n kicks dirt in your eyes!",ch,NULL,victim,TO_VICT);
	if (!IS_NPC(ch) && !IS_NPC(victim) &&
	( (victim->fighting == NULL) || (ch->fighting == NULL)))
		do_myell(victim,"Help! Someone just kicked dirt in my eyes!");

        damage(ch,victim,number_range(2,5),gsn_dirt,DAM_NONE,TRUE);
        send_to_char("You can't see a thing!\n\r",victim);
        check_improve(ch,gsn_dirt,TRUE,2);
        WAIT_STATE(ch,skill_table[gsn_dirt].beats);

	init_affect(&af);
        af.where        = TO_AFFECTS;
        af.type         = gsn_dirt;
	af.aftype = AFT_MALADY;
        af.level        = ch->level + ch->drain_level;
        af.duration     = 0;
        af.location     = APPLY_HITROLL;
        af.modifier     = -4;
        af.bitvector    = AFF_BLIND;

        affect_to_char(victim,&af);
    }
    else
    {
/* PK yells....add these to most attacks that can initiate PKs.
-Ceran
*/
	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (victim->fighting == NULL || ch->fighting == NULL))
	{
	switch(number_range(0,1))
		{
		case (0):
		sprintf(buf,"Help! %s just tried to kick dirt in my eyes!",PERS(ch,victim));
		break;
		case (1):
		sprintf(buf,"Die, %s you dirty fool!",PERS(ch,victim));
		break;
		}
	do_myell(victim,buf);
	}
        damage(ch,victim,0,gsn_dirt,DAM_NONE,TRUE);

        check_improve(ch,gsn_dirt,FALSE,2);
        WAIT_STATE(ch,skill_table[gsn_dirt].beats);
    }
        check_killer(ch,victim);
}

void do_trip( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;
    int dam;
    AFFECT_DATA af;
    char buf[MAX_STRING_LENGTH];

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_trip)) == 0
    ||   (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_TRIP) && ch->pIndexData->vnum!=MOB_VNUM_TEMPLAR)
    ||   (!IS_NPC(ch)
          && ch->level < skill_table[gsn_trip].skill_level[ch->class]))
    {
        send_to_char("Tripping?  What's that?\n\r",ch);
        return;
    }


    if (arg[0] == '\0')
    {
        victim = ch->fighting;
        if (victim == NULL)
        {
            send_to_char("But you aren't fighting anyone!\n\r",ch);
            return;
        }
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    if (is_safe(ch,victim))
        return;

    if (IS_AFFECTED(victim,AFF_FLYING)
	&& !is_affected(victim,gsn_earthbind))
    {
        act("$S feet aren't on the ground.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (victim->position < POS_FIGHTING)
    {
        act("$N is already down.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (victim == ch)
    {
        send_to_char("You fall flat on your face!\n\r",ch);
        WAIT_STATE(ch,2 * skill_table[gsn_trip].beats);
        act("$n trips over $s own feet!",ch,NULL,NULL,TO_ROOM);
        return;
    }

    if (is_affected(victim,gsn_asscounter))
    {
        act("You dart to the side kicking the inside of $N's knee!",victim,0,ch,TO_CHAR);
        act("$n darts to the side kicking the inside of $N's knee!",victim,0,ch,TO_NOTVICT);
        act("$n dodges your trip, then kicks the inside of your knee!",victim,0,ch,TO_VICT);
        WAIT_STATE(victim,1 * PULSE_VIOLENCE);
        dam = victim->level*2 + number_percent()*2 - 50;
        damage_old(victim,ch,dam,gsn_legbreak,DAM_BASH,TRUE);

         if (!is_affected(ch,gsn_legbreak))
           {
		init_affect(&af);
                af.where     = TO_AFFECTS;
		af.aftype = AFT_MALADY;
                af.type      = gsn_legbreak;
                af.level     = victim->level;
                af.duration  = victim->level/5;
                af.bitvector = 0;
                af.location  = APPLY_DEX;
                af.modifier  = 1-(victim->level/5);
                affect_to_char(ch,&af);
            }
        affect_strip( victim, gsn_asscounter );
        return;
 }

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
        act("$N is your beloved master.",ch,NULL,victim,TO_CHAR);
        return;
    }

    /* modifiers */

    /* size */
    if (ch->size < victim->size)
        chance += (ch->size - victim->size) * 10;  /* bigger = harder to trip */

    /* dex */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= get_curr_stat(victim,STAT_DEX) * 3 / 2;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || is_affected(ch,skill_lookup("haste")))
        chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || is_affected(victim,skill_lookup("haste")))
        chance -= 40;
    if(is_affected(victim,gsn_cloak_form))
	chance+=10;

    /* level */
    chance += (ch->drain_level + ch->level - victim->level - victim->drain_level) * 2;


    /* now the attack */
    if (number_percent() < chance)
    {
        act("$n trips you and you go down!",ch,NULL,victim,TO_VICT);
        act("You trip $N and $N goes down!",ch,NULL,victim,TO_CHAR);
        act("$n trips $N, sending $M to the ground.",ch,NULL,victim,TO_NOTVICT);
        check_improve(ch,gsn_trip,TRUE,1);

	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (ch->fighting == NULL || victim->fighting == NULL))
	{
	sprintf(buf,"Help! %s just tripped me!",PERS(ch,victim));
	do_myell(victim,buf);
	}
        WAIT_STATE(victim,2 * PULSE_VIOLENCE);
        WAIT_STATE(ch,skill_table[gsn_trip].beats);
        victim->position = POS_RESTING;
        damage(ch,victim,number_range(2, 2 +  2 * victim->size),gsn_trip,
            DAM_BASH,TRUE);
	check_downstrike(ch,victim);
	check_cheap_shot(ch,victim);
    }
    else
    {
	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (ch->fighting == NULL || victim->fighting == NULL))
	{
	sprintf(buf,"Help! %s just tried to trip me!",PERS(ch,victim));
	do_myell(victim,buf);
	}
        damage(ch,victim,0,gsn_trip,DAM_BASH,TRUE);
        WAIT_STATE(ch,skill_table[gsn_trip].beats*2/3);
        check_improve(ch,gsn_trip,FALSE,1);
    }
        check_killer(ch,victim);
}


void do_steel_nerves(CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA af;
    int chance;

    if ( (chance = get_skill(ch,gsn_steel_nerves)) == 0
    ||   (ch->level < skill_table[gsn_steel_nerves].skill_level[ch->class]))
    {
	send_to_char("Huh?\n\r",ch);
        return;
    }

    if (is_affected(ch,gsn_steel_nerves))
    {
	send_to_char("You feel as invincible as you will ever feel.\n\r",ch);
	return;
    }

    if (ch->mana < 100)
    {
	send_to_char("You don't have the mana.\n\r",ch);
	return;
    }

    if (number_percent() > ch->pcdata->learned[gsn_steel_nerves])
    {
	ch->mana -= 50;
	send_to_char("You try to make yourself feel invincible, but fail.\n\r",ch);
	return;
    }

	init_affect(&af);
    af.where = TO_AFFECTS;
    af.duration = 3 + (ch->level)/10;
    af.location = 0;
    af.modifier = 0;
    af.level = ch->level + ch->drain_level;
    af.type = gsn_steel_nerves;
	af.aftype = AFT_POWER;
        af.bitvector = AFF_DETECT_INVIS;
        affect_to_char(ch,&af);
        af.bitvector = AFF_DETECT_MAGIC;
        affect_to_char(ch,&af);

    send_to_char("You build up an intense mental resolve and prepare for coming battles.\n\r",ch);
    act("$n seems to glower and look deadly.",ch,NULL,NULL,TO_ROOM);
        send_to_char("Your eyes tingle.\n\r",ch);
        send_to_char("You are more sensitive to the prescence of magical auras.\n\r",ch);

    ch->mana -= 100;
    return;
}


void do_battlecry( CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA af;
    int chance;

    if ( (chance = get_skill(ch,gsn_battlecry)) == 0
    ||   (ch->level < skill_table[gsn_battlecry].skill_level[ch->class]))
    {
	send_to_char("Huh?\n\r",ch);
        return;
    }

    if (cabal_down(ch,CABAL_RAVAGER))
		return;


    if (is_affected(ch,gsn_battlecry))
    {
	send_to_char("You are already as angry as you can be.\n\r",ch);
	return;
    }
    if (ch->mana < 40)
    {
	send_to_char("You don't have the mana.\n\r",ch);
	return;
    }
    if (number_percent() > ch->pcdata->learned[gsn_battlecry])
    {
	send_to_char("You build up your rage but it dies down too fast.\n\r",ch);
	ch->mana -= 20;
	return;
    }

    ch->mana -= 40;
	init_affect(&af);
    af.where = TO_AFFECTS;
    af.level = ch->level + ch->drain_level;
    af.duration = (ch->drain_level + ch->level)/2;
    af.type = gsn_battlecry;
    af.modifier = 3;
    af.location = APPLY_HITROLL;
    af.bitvector = 0;
	af.aftype = AFT_POWER;
    affect_to_char(ch,&af);

    send_to_char("You are filled with total fury as you let out a battlecry!\n\r",ch);
    act("$n lets out a chilling battlecry and begins frothing at the mouth.",ch,NULL,NULL,TO_ROOM);
    return;
}

void do_hit( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( ch->position != POS_FIGHTING )
    {
	send_to_char( "You're not fighting anyone.\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Switch targets to whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( victim == ch )
    {
        send_to_char( "Are you crazy?\n\r", ch );
        return;
    }

    if ( victim->fighting != ch )
    {
	send_to_char( "That person isn't fighting you.\n\r", ch );
	return;
    }

    ch->fighting = victim;
    return;
}

void do_kill( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Kill whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
/*  Allow player killing   */
    if ( !IS_NPC(victim) )
    {
        send_to_char( "You must MURDER a player.\n\r", ch );
        return;
    }
    if ( victim == ch )
    {
        send_to_char( "You hit yourself.  Ouch!\n\r", ch );
        multi_hit( ch, ch, TYPE_UNDEFINED );
        return;
    }

    if ( is_safe( ch, victim ) )
        return;

    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
    {
        act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
        return;
    }

    if ( ch->position == POS_FIGHTING )
    {
        send_to_char( "You do the best you can!\n\r", ch );
        return;
    }

    WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return;
}



void do_murde( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to MURDER, spell it out.\n\r", ch );
    return;
}



void do_murder( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Murder whom?\n\r", ch );
        return;
    }

    if ((IS_AFFECTED(ch,AFF_CHARM) || (IS_NPC(ch) && IS_SET(ch->act,ACT_PET)))
    && !is_centurion(ch) && !check_rangermob(ch))
        return;

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( victim == ch )
    {
        send_to_char( "Suicide is a mortal sin.\n\r", ch );
        return;
    }

    if ( is_safe( ch, victim ) )
        return;

	if (check_chargeset(ch,victim)) {return;}

    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
    {
        act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
        return;
    }

    if ( ch->position == POS_FIGHTING )
    {
        send_to_char( "You do the best you can!\n\r", ch );
        return;
    }

    WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
    if (IS_NPC(ch))
        sprintf(buf, "Help! I am being attacked by %s!",ch->short_descr);
    else
    {
        switch(number_range(1,6))
        {
	default:
            sprintf(buf, "Help!  I am being attacked by %s!",PERS(ch,victim));
            break;
        case (1):
            sprintf(buf, "Help!  I am being attacked by %s!",PERS(ch,victim));
            break;
        case (2):
            sprintf(buf, "Die %s, you dirty fool!", PERS(ch,victim));
            break;
        case (3): case (4):
            sprintf(buf, "Help!  %s is attacking me!", PERS(ch,victim));
            break;
        case (5):
            sprintf(buf, "How dare you attack me %s!", PERS(ch,victim));
            break;
        case (6):
            sprintf(buf, "Help!  %s is hurting me!", PERS(ch,victim));
            break;
        }
    }

/* Added a myell hack of yell so Enforcer cabal members can see if a PK
yell is faked or not (see myell in act_comm.c for this).
-Ceran
*/
    do_myell( victim, buf );
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return;
}


void do_ambush( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    int chance;


    one_argument( argument, arg);


    if (arg[0] == '\0')
    {
	send_to_char("Ambush whom?\n\r",ch);
	return;
    }

    if (ch->fighting != NULL)
    {
	send_to_char("But you're still fighting!\n\r",ch);
	return;
    }
    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
	return;
    }
    else if (!IS_AFFECTED(ch, AFF_CAMOUFLAGE) || can_see(victim, ch) )
    {
	send_to_char("But they can still see you.\n\r",ch);
	return;
    }

    if ( victim == ch )
    {
	send_to_char("That's just not possible.\n\r",ch);
	return;
    }
    if ( is_safe( ch, victim ) )
      return;

	if (check_chargeset(ch,victim)) {return;}

    chance = get_skill(ch,gsn_moving_ambush);
    if (victim->fighting != NULL
	&& ( chance < 3
	|| ch->level < skill_table[gsn_moving_ambush].skill_level[ch->class]))
    {
	send_to_char("They are moving around too much to ambush.\n\r",ch);
	return;
    }

    if (victim->fighting != NULL
	&& number_percent() >= chance)
	{
	send_to_char("You can't quite pin them down for your ambush.\n\r",ch);
	return;
	}

	if (!IS_NPC(ch) && !IS_NPC(victim) && victim->fighting == NULL)
	{
	sprintf(buf,"Help! I've been ambushed by %s!",PERS(ch,victim));
	do_myell(victim,buf);
	}

   WAIT_STATE( ch, skill_table[gsn_ambush].beats );
   if ( (number_percent( ) < get_skill(ch, gsn_ambush) )
   || ( get_skill(ch,gsn_ambush) >= 2 && !IS_AWAKE(victim) ) )
    {
	check_improve(ch,gsn_ambush, TRUE,1);
	multi_hit( ch, victim, gsn_ambush );
    }
    else
    {
	check_improve(ch,gsn_ambush, FALSE,1);
	damage( ch, victim, 0, gsn_ambush, DAM_NONE, TRUE);
    }

    return;
}


void do_backstab( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int dam;
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );


    if ( (get_skill(ch,gsn_backstab) == 0 )
	|| ch->level < skill_table[gsn_backstab].skill_level[ch->class])
    {
	if (!IS_NPC(ch))
	{
	send_to_char("You don't know how to backstab.\n\r",ch);
	return;
	}
    }

    if (arg[0] == '\0')
    {
        send_to_char("Backstab whom?\n\r",ch);
        return;
    }

    if (ch->fighting != NULL)
    {
        send_to_char("You're facing the wrong end.\n\r",ch);
        return;
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    if ( victim == ch )
    {
        send_to_char( "How can you sneak up on yourself?\n\r", ch );
        return;
    }

        obj= get_eq_char(ch,WEAR_WIELD);

        if (obj == NULL)
        {
        send_to_char("You must be wielding a weapon to backstab.\n\r",ch);
        return;
        }

        if (obj->value[0] != WEAPON_DAGGER)
        {
        obj = get_eq_char(ch,WEAR_DUAL_WIELD);
        if (obj == NULL)
        {
        send_to_char("You must be wielding a weapon to backstab.\n\r",ch);
        return;
        }
        }

        if (obj->value[0] != WEAPON_DAGGER)
        {
        send_to_char("You need to be wielding a dagger to backstab.\n\r",ch);
        return;
        }

    if ( is_safe(ch, victim ) )
      return;

	if (check_chargeset(ch,victim)) {return;}

    if ( victim->fighting != NULL )
    {
	send_to_char( "That person is moving around too much to backstab.\n\r", ch );
	return;
    }

    if (victim->hit < victim->max_hit*5/10)
    {
        act( "$N is hurt and suspicious ... you can't sneak up.",
            ch, NULL, victim, TO_CHAR );
        return;
    }

    WAIT_STATE( ch, skill_table[gsn_backstab].beats );
    if ( number_percent( ) < get_skill(ch,gsn_backstab)
    || ( get_skill(ch,gsn_backstab) >= 2 && !IS_AWAKE(victim) ) )
    {
        check_improve(ch,gsn_backstab,TRUE,1);
	if (!IS_NPC(ch) && !IS_NPC(victim) && victim->fighting == NULL)
	{
	switch(number_range(0,1))
	{
	case (0):
	sprintf(buf,"Die, %s you backstabbing fool!",PERS(ch,victim));
	break;
	case (1):
	sprintf(buf,"Help! I'm being backstabbed by %s!",PERS(ch,victim));
	break;
	}
	do_myell(victim,buf);
	}


        one_hit( ch, victim, gsn_backstab );
    }
    else
    {
        check_improve(ch,gsn_backstab,FALSE,1);
	if (!IS_NPC(ch) && !IS_NPC(victim) && victim->fighting == NULL)
	{
	switch(number_range(0,1))
	{
	case (0):
	sprintf(buf,"Die, %s you backstabbing fool!",PERS(ch,victim));
	break;
	case (1):
	sprintf(buf,"Help! I'm being backstabbed by %s!",PERS(ch,victim));
	break;
	}
	do_myell(victim,buf);
	}


        damage( ch, victim, 0, gsn_backstab,DAM_NONE,TRUE);
    }


     if (ch->level < skill_table[gsn_dual_backstab].skill_level[ch->class])
	return;

    if (number_range(0,3) == 0)
    {
	if (get_skill(ch,gsn_dual_backstab) != 100)
		check_improve(ch,gsn_dual_backstab,FALSE,1);
	return;
    }

    if (number_percent() > (get_skill(ch,gsn_dual_backstab) + ch->level - victim->level) )
    {
	check_improve(ch,gsn_dual_backstab,FALSE,1);
	dam  = 0;
    }
    else
    {
	dam = dice(obj->value[1], obj->value[2]) * get_skill(ch,gsn_dual_backstab)/100;
	if ((ch->drain_level + ch->level) < 35) 	dam *= number_range(2,4);
 	else if ((ch->drain_level + ch->level) < 40) 	dam *= number_range(2,5);
	else if ((ch->drain_level + ch->level) < 45)	dam *= number_range(3,5);
	else if ((ch->drain_level + ch->level) < 50)	dam *= number_range(10,25); /* 4,6 */
	else dam *= number_range(15,30); /* 4,7 */
        dam += (ch->damroll * get_skill(ch,gsn_dual_backstab)/100);
    }
    dam *= battlecry_multiplier(ch,gsn_dual_backstab);
    damage_old(ch,victim,dam,gsn_dual_backstab,DAM_NONE,TRUE);

    dam *= battlecry_multiplier(ch,gsn_dual_backstab);
    damage_old(ch,victim,dam,gsn_dual_backstab,DAM_NONE,TRUE);

    return;
}



void do_rescue( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *fch;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        send_to_char( "Rescue whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( victim == ch )
    {
        send_to_char( "What about fleeing instead?\n\r", ch );
        return;
    }

    if ( !IS_NPC(ch) && IS_NPC(victim) )
    {
        send_to_char( "Doesn't need your help!\n\r", ch );
        return;
    }

    if ( ( fch = victim->fighting ) == NULL )
    {
        send_to_char( "That person is not fighting right now.\n\r", ch );
        return;
    }


    if (is_safe(ch,victim->fighting))
	return;

    WAIT_STATE( ch, skill_table[gsn_rescue].beats );
    if ( number_percent( ) > get_skill(ch,gsn_rescue))
    {
        send_to_char( "You fail the rescue.\n\r", ch );
        check_improve(ch,gsn_rescue,FALSE,1);
        return;
    }

    WAIT_STATE(victim,12);
    act( "You rescue $N!",  ch, NULL, victim, TO_CHAR    );
    act( "$n rescues you!", ch, NULL, victim, TO_VICT    );
    act( "$n rescues $N!",  ch, NULL, victim, TO_NOTVICT );
    check_improve(ch,gsn_rescue,TRUE,1);

    stop_fighting( fch, FALSE );
    stop_fighting( victim, FALSE );

    set_fighting( ch, fch );
    set_fighting( fch, ch );
    return;
}



void do_kick( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    int dam;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_kick].skill_level[ch->class] )
    {
        send_to_char(
            "You better leave the martial arts to fighters.\n\r", ch );
        return;
    }

    if (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_KICK))
        return;

    if ( ( victim = ch->fighting ) == NULL )
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    WAIT_STATE( ch, skill_table[gsn_kick].beats );
    if ( get_skill(ch,gsn_kick) > number_percent())
    {
	dam = (ch->drain_level + ch->level)/2;
	dam += number_range(0,(ch->drain_level + ch->level)/6);
	dam += number_range(0,(ch->drain_level + ch->level)/6);
	dam += number_range(0,(ch->drain_level + ch->level)/6);
	dam += number_range(0,(ch->drain_level + ch->level)/6);
	dam += number_range(0,(ch->drain_level + ch->level)/6);
	dam += number_range(0,(ch->drain_level + ch->level)/6);
	dam += number_range(ch->level/5,ch->level/4);

	if (!IS_NPC(ch))
	{
	if (!str_cmp(class_table[ch->class].name,"transmuter"))
	if (!str_cmp(class_table[ch->class].name,"necromancer"))
	if (!str_cmp(class_table[ch->class].name,"elementalist"))
		{
		dam *= 3;
		dam /= 4;
		}
	}
        damage(ch,victim,dam, gsn_kick,DAM_BASH,TRUE);
        check_improve(ch,gsn_kick,TRUE,1);
	check_follow_through(ch,victim,dam);
    }
    else
    {
        damage( ch, victim, 0, gsn_kick,DAM_BASH,TRUE);
        check_improve(ch,gsn_kick,FALSE,1);
    }
        check_killer(ch,victim);
    return;
}




void do_disarm( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim=0;
    OBJ_DATA *obj;
    int dam;
    AFFECT_DATA af;
    int chance,hth,ch_weapon,vict_weapon,ch_vict_weapon;

    hth = 0;

    if ((chance = get_skill(ch,gsn_disarm)) == 0)
    {
        send_to_char( "You don't know how to disarm opponents.\n\r", ch );
        return;
    }

    if ( get_eq_char( ch, WEAR_WIELD ) == NULL
    &&   ((hth = get_skill(ch,gsn_hand_to_hand)) == 0
    ||    (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_DISARM) && !check_rangermob(ch))))
    {
        send_to_char( "You must wield a weapon to disarm.\n\r", ch );
        return;
    }

    if (IS_AFFECTED(ch,AFF_BLIND))

    {
        act("You can't see the person to disarm them!",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (IS_NPC(ch) && (IS_AFFECTED(ch,AFF_BLIND)))
       return;

    if ( ( victim = ch->fighting ) == NULL )
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    if (is_affected(victim,gsn_asscounter))
    {
        act("With blinding speed you hit $N's arm, breaking it!",victim,0,ch,TO_CHAR);
        act("With blinding speed $n hits $N's arm, breaking it!",victim,0,ch,TO_NOTVICT);
        act("With blinding speed $n hits your arm, breaking it instantly!",victim,0,ch,TO_VICT);
        WAIT_STATE(victim,2 * PULSE_VIOLENCE);
        dam = victim->level*2 + number_percent()*2 - 50;
        damage_old(victim,ch,dam,gsn_armbreak,DAM_BASH,TRUE);

        if (!is_affected(ch,gsn_armbreak))
              {
		init_affect(&af);
                af.where     = TO_AFFECTS;
                af.type      = gsn_armbreak;
		af.aftype = AFT_MALADY;
                af.level     = victim->level;
                af.duration  = victim->level/5;
                af.bitvector = 0;
                af.location  = APPLY_DEX;
                af.modifier  = 1-(victim->level/5);
                affect_to_char(ch,&af);
                af.location  = APPLY_STR;
                af.modifier  = 1-(victim->level/5);
                af.bitvector = 0;
                affect_to_char(ch,&af);
          	}
        affect_strip( victim, gsn_asscounter );
        return;
    }

    if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
    {
        send_to_char( "Your opponent is not wielding a weapon.\n\r", ch );
        return;
    }

	if (is_affected(victim,gsn_spiderhands) )
	{
		act("You try to disarm $N, but $s hands stick to his weapon.",
			ch,NULL,victim,TO_CHAR);
		act("$n tries to disarm you, but your hands stick to your weapon.",
			ch,NULL,victim,TO_VICT);
		act("$n tries to disarm $N, but their hands stick to their weapon.",
			ch,NULL,victim,TO_NOTVICT);
		return;
	}

    /* find weapon skills */
    ch_weapon = get_weapon_skill(ch,get_weapon_sn(ch));
    vict_weapon = get_weapon_skill(victim,get_weapon_sn(victim));
    ch_vict_weapon = get_weapon_skill(ch,get_weapon_sn(victim));

    /* modifiers */
    chance *= 6;
    chance /=10;

    /* skill */
    if ( get_eq_char(ch,WEAR_WIELD) == NULL)
        chance = chance * hth/150;
    else
        chance = chance * ch_weapon/100;

    chance += (ch_vict_weapon/2 - vict_weapon) / 2;

    /* dex vs. strength */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= 2 * get_curr_stat(victim,STAT_STR);

    /* level */
    chance += (ch->drain_level + ch->level - victim->drain_level - victim->level) * 2;

    /* and now the attack */
    if (number_percent() < chance)
    {
        WAIT_STATE( ch, skill_table[gsn_disarm].beats );
        disarm( ch, victim );
        check_improve(ch,gsn_disarm,TRUE,1);
    }
    else
    {
        WAIT_STATE(ch,skill_table[gsn_disarm].beats);
        act("You fail to disarm $N.",ch,NULL,victim,TO_CHAR);
        act("$n tries to disarm you, but fails.",ch,NULL,victim,TO_VICT);
        act("$n tries to disarm $N, but fails.",ch,NULL,victim,TO_NOTVICT);
        check_improve(ch,gsn_disarm,FALSE,1);
    }
    check_killer(ch,victim);
    return;
}


void do_circle_stab( CHAR_DATA *ch, char *argument )
{
    bool attempt_dual;
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *v_check;
    CHAR_DATA *v_next;
    OBJ_DATA *obj;
    int chance;
    int dam;

    attempt_dual = FALSE;
    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_circle)) == 0
    ||  IS_NPC(ch)
    ||  (!IS_NPC(ch) && ch->level < skill_table[gsn_circle].skill_level[ch->class]) )
    {
	send_to_char("Circling? What's that?\n\r",ch);
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
	send_to_char("You can't circle someone like that.\n\r",ch);
	return;
	}


    for (v_check = ch->in_room->people; v_check != NULL; v_check = v_next)
    {
	v_next = v_check->next_in_room;

	if (v_check->fighting == ch)
	{
	send_to_char("Not while you're defending yourself!\n\r",ch);
	return;
	}
    }

    if (victim == ch)
    {
	send_to_char("huh?\n\r", ch);
	return;
    }

   obj = get_eq_char(ch,WEAR_WIELD);
   if (obj == NULL || obj->value[0] != WEAPON_DAGGER)
	{
	attempt_dual = TRUE;
	obj = get_eq_char(ch,WEAR_DUAL_WIELD);
	}

	if (obj == NULL)
	{
	send_to_char("You must wield a dagger to circle stab.\n\r",ch);
	return;
	}

    if (obj->value[0] != 2)
    {
	send_to_char("You must wield a dagger to circle stab.\n\r",ch);
	return;
    }

    chance += (ch->drain_level + ch->level - victim->level - victim->drain_level);
    chance = URANGE(5, chance, 95);

    WAIT_STATE(ch,skill_table[gsn_circle].beats);

    if (number_percent() < chance)
    {
   	act("You circle around $N to land a critical strike.",ch,NULL,victim,TO_CHAR);
	act("$n cirlces around you to land a critical strike.",ch,NULL,victim,TO_VICT);
	act("$n circles $N to land a critical strike.",ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_circle,TRUE,1);
	dam = dice(obj->value[1],obj->value[2]);
	dam += 40;

	if ((ch->drain_level + ch->level) <= 15)
		dam *= 1;
	else if ((ch->drain_level + ch->level) <= 20)
		dam *= 3/2;
	else if ((ch->drain_level + ch->level) < 25)
		dam *= 2;
	else if ((ch->drain_level + ch->level) < 30)
		dam *= 7/3;
	else if ((ch->drain_level + ch->level) < 40)
		dam *= 5/2;
	else if ((ch->drain_level + ch->level) <= 49)
		dam *= 7/2;
	else if ((ch->drain_level + ch->level) <= 55)
		dam *= 10/3;
	else dam *= 10/3;

	dam *= battlecry_multiplier(ch,gsn_circle);

	damage_old(ch,victim,dam,gsn_circle, attack_table[obj->value[3]].damage, TRUE);
    }
    else
    {
	check_improve(ch,gsn_circle,FALSE,1);

	damage_old(ch,victim,0,gsn_circle,DAM_NONE,TRUE);
    }

    return;
}


void do_surrender( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *mob;
    if ( (mob = ch->fighting) == NULL )
    {
        send_to_char( "But you're not fighting!\n\r", ch );
        return;
    }
    act( "You surrender to $N!", ch, NULL, mob, TO_CHAR );
    act( "$n surrenders to you!", ch, NULL, mob, TO_VICT );
    act( "$n tries to surrender to $N!", ch, NULL, mob, TO_NOTVICT );
    stop_fighting( ch, TRUE );

    if ( !IS_NPC( ch ) && IS_NPC( mob )
    &&   ( !HAS_TRIGGER( mob, TRIG_SURR )
        || !mp_percent_trigger( mob, ch, NULL, NULL, TRIG_SURR ) ) )
    {
        act( "$N seems to ignore your cowardly act!", ch, NULL, mob, TO_CHAR );
        multi_hit( mob, ch, TYPE_UNDEFINED );
    }
}


void do_sla( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to SLAY, spell it out.\n\r", ch );
    return;
}



void do_slay( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        send_to_char( "Slay whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( ch == victim )
    {
        send_to_char( "Suicide is a mortal sin.\n\r", ch );
        return;
    }

    if ( !IS_NPC(victim) && victim->level >= get_trust(ch))
    {
        send_to_char( "You failed.\n\r", ch );
        return;
	if (IS_NPC(ch) && (ch->pIndexData->vnum != MOB_VNUM_PROTECTOR)
	&& (ch->pIndexData->vnum != MOB_VNUM_ACADIAN)
	&& (ch->pIndexData->vnum != MOB_VNUM_COVEN)
        && (ch->pIndexData->vnum != MOB_VNUM_BRIAR)
	&& (ch->pIndexData->vnum != MOB_VNUM_RAVAGER)
	&& (ch->pIndexData->vnum != MOB_VNUM_VIGILANTE) )
	{
        send_to_char( "You failed.\n\r", ch );
        return;
    	}
    }

    act( "You point at $M and $S heart stops!",  ch, NULL, victim, TO_CHAR    );
    act( "$n points at you and your heart suddenly stops!", ch, NULL, victim, TO_VICT    );
    act( "$n points at $N and $E falls over clenching his chest!",  ch, NULL, victim, TO_NOTVICT );

    raw_kill(ch, victim );
    return;
}


void spell_evil_eye(int sn,int level, CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam, modify, saves;

    if (victim == ch)
    {
	send_to_char("You can't do such a thing.\n\r",ch);
	return;
    }

    act("$n's eyes turn pitch-black and a surging darkness erupts at $N.",ch,NULL,victim,TO_NOTVICT);
    act("Your eyes turn pitch-black and a stream of pure negative energy streams out at $N.",ch,NULL,victim,TO_CHAR);
    send_to_char("You feel a sudden intense agony burning into your skull.\n\r",victim);

    if (IS_SET(victim->affected_by,AFF_BLIND) )
    {
	dam = dice(level,8);
	if (saves_spell(level,victim, DAM_NEGATIVE) )
	dam /= 2;
	damage_old(ch,victim,dam,sn,DAM_NEGATIVE, TRUE);
	return;
    }

   saves = 1;
   for (modify = 0; modify < 4; modify ++)
    {
	if (saves_spell(level, victim, DAM_NEGATIVE))
		saves++;
    }

    if (saves == 0)
    {
	act("$n's surging darkness devestates $N's skull, killing $M instantly!",ch,NULL,victim,TO_NOTVICT);
	act("Your darkness slays $N!",ch,NULL,victim,TO_CHAR);
	send_to_char("With a violent burning sensation your mind vaporises.\n\r",victim);
	raw_kill(ch,victim);
    	return;
    }

    dam = dice(level, 16);
    dam /= (saves + 1);

    damage_old(ch,victim,dam,sn,DAM_NEGATIVE,TRUE);
    return;

}


void spell_power_word_kill(int sn,int level,CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam,saves,modify;

    saves = 0;

    act("$n points a finger at $N and utters the word, 'Die'.",ch,NULL,victim,TO_NOTVICT);
    act("$n points a finger at you and utters the word, 'Die'.",ch,NULL,victim,TO_VICT);
    send_to_char("You intone a word of unholy power.\n\r",ch);

    for ( modify = 0; modify < 5; modify++)
    {
	if (saves_spell(level-1,victim,DAM_NEGATIVE) )
		saves++;
    }

    if(IS_IMMORTAL(ch) && (victim->level<ch->level)) { saves=0; }
    if (saves == 0)
    {
	act("$N shudders in shock as $S heart explodes!", ch, NULL,victim,TO_NOTVICT);
	send_to_char("You feel your heart rupture in a vilent explosion of pain!\n\r",victim);
	act("Your word of power vaporises $N's heart, killing $M instantly!",ch,NULL,victim,TO_CHAR);
	raw_kill(ch,victim);
    	return;
    }
    dam = dice(level,14);
    dam /= saves;
    damage_old(ch,victim,dam,sn,DAM_NEGATIVE,TRUE);

	return;
}

void do_lunge( CHAR_DATA *ch, char *argument)
{
        CHAR_DATA *victim;
        int chance, dam;
        char arg[MAX_INPUT_LENGTH];
        OBJ_DATA *wield;
        int multiplier;

	dam = 0;
        one_argument(argument,arg);
        if ((chance = get_skill(ch,gsn_lunge) ) == 0
        || (ch->level < skill_table[gsn_lunge].skill_level[ch->class]) )
        {
        send_to_char("Lunging? What's that?\n\r",ch);
        return;
        }
        if (arg[0] == '\0')
        {
        victim = ch->fighting;
        if (victim == NULL)
                {
                send_to_char("But you aren't fighting anyone!\n\r",ch);
                return;
                }
        }
        else if ( (victim = get_char_room(ch,arg)) == NULL )
        {
                send_to_char("They aren't here.\n\r",ch);
                return;
        }
        if (victim == ch)
        {
        send_to_char("You can't lunge at yourself!\n\r",ch);
        return;
        }

        wield = get_eq_char(ch,WEAR_WIELD);
        if ((wield == NULL)
        || (( wield->value[0] != WEAPON_SWORD)
        && ( wield->value[0] != WEAPON_SPEAR)
        && ( wield->value[0] != WEAPON_POLEARM) ))
        {
        send_to_char("You must be wielding a sword, spear or polearm to lunge.\n\r",ch);
        return;
        }

        if (is_safe(ch,victim) )
        return;

	if (check_chargeset(ch,victim)) {return;}

        chance += ch->carry_weight/25;
        chance -= victim->carry_weight/20;
        chance += (ch->size - victim->size)*20;
        chance -= get_curr_stat(victim,STAT_DEX);
        chance += get_curr_stat(ch,STAT_STR)/3;
        chance += get_curr_stat(ch,STAT_DEX)/2;
        if (is_affected(ch,skill_lookup("haste")) )
                chance += 10;
        if (is_affected(victim,skill_lookup("haste")) )
                chance = 20;
        chance += (ch->drain_level + ch->level - victim->level - victim->drain_level);

        act("$n attempts to impale $N with a quick lunge!",ch,0,victim,TO_NOTVICT);
        act("You attempt to impale $N with a quick lunge!",ch,0,victim,TO_CHAR);
        act("$n attempts to impale you with a quick lunge!",ch,0,victim,TO_VICT);

        if (number_percent() < chance)
        {
        check_improve(ch,gsn_lunge,TRUE,1);
        WAIT_STATE(ch,skill_table[gsn_lunge].beats);
        if (wield->pIndexData->new_format)
                dam = dice(wield->value[1],wield->value[2]);
        else
                dam = number_range(wield->value[1],wield->value[2]);

        if (get_skill(ch,gsn_enhanced_damage) > 0 )
        {
        if (number_percent() <= get_skill(ch,gsn_enhanced_damage) )
                {
                check_improve(ch,gsn_enhanced_damage,TRUE,1);
dam += dam*(number_range(60,100)/100) * ch->pcdata->learned[gsn_enhanced_damage]/100;
                }
        }

        dam += GET_DAMROLL(ch);
        dam *= ch->pcdata->learned[gsn_lunge];
	dam /= 100;
        multiplier = number_range((ch->drain_level + ch->level)/8,(ch->drain_level + ch->level)/4);
        multiplier /= 10;
	multiplier += 6/4;
        dam *= multiplier;
	dam *= battlecry_multiplier(ch, gsn_lunge);

        if (dam <= 0)
                dam = 1;
        damage_old(ch,victim,dam,gsn_lunge,DAM_PIERCE,TRUE);
        }
        else
        {
        damage_old(ch,victim,dam,gsn_lunge,DAM_PIERCE,TRUE);
        check_improve(ch,gsn_lunge,FALSE,1);
        WAIT_STATE(ch,skill_table[gsn_lunge].beats);
        }
        return;
}

void do_blackjack( CHAR_DATA *ch,char *argument)
{
        char arg[MAX_INPUT_LENGTH];
	AFFECT_DATA af;
        CHAR_DATA *victim;
	int chance,size;
        char buf[MAX_STRING_LENGTH];

        if (IS_NPC(ch) )
                return;

        one_argument(argument,arg);
        if ((chance = get_skill(ch,gsn_blackjack)) == 0
        || ( ch->level < skill_table[gsn_blackjack].skill_level[ch->class]) )
        {
        send_to_char("You don't know how to blackjack.\n\r",ch);
        return;
        }
        if (arg[0] == '\0')
        {
        send_to_char("Blackjack who?\n\r",ch);
        return;
        }
        if ((victim = get_char_room(ch,arg)) == NULL)
        {
        send_to_char("They aren't here.\n\r",ch);
        return;
        }
        if (victim == ch)
        {
        send_to_char("Amusing, but not possible.\n\r",ch);
        return;
        }
    if (is_safe(ch,victim))
	return;

	if (check_chargeset(ch,victim)) {return;}

	update_pc_last_fight(ch,victim);

	size=victim->size-ch->size;
	if(size>2) {
	    send_to_char("That person is too large to blackjack.\n\r",ch);
	    return;
	}
    if (!(str_cmp(race_table[victim->race].name,"lich")))
    {
        act("You whack $N over the head with a heavy looking sack, but with no affect.",ch,0,victim,TO_CHAR);
        act("$n whacks $N over the head with a heavy looking sack, but with no affect.",ch,0,victim,TO_NOTVICT);
        send_to_char("You feel a sudden pain erupt through the back of your skull. It doesn't affect you.\n\r",victim);
        return;
    }
        if (victim->position == POS_SLEEPING)
        {
        send_to_char("But that person is already asleep!\n\r",ch);
        return;
        }
        if (is_affected(victim,gsn_blackjack_timer) || is_affected(victim,gsn_blackjack) )
        {
        send_to_char("They are guarding their head too well right now.\n\r",ch);
        return;
        }
	chance *= 75;
	chance /= 90;
        chance += (ch->drain_level + ch->level - victim->level - victim->drain_level);
	if(size>0)
                chance -= 15*size;
        if (is_affected(victim,skill_lookup("haste")))
                chance -= 50;
        if (is_affected(ch,skill_lookup("haste")))
                chance += 10;
        if (get_eq_char(victim,WEAR_HEAD) != NULL)
                chance -= 10;
	if(is_affected(victim,gsn_cloak_form))
		chance+=10;
        if (victim->position == POS_FIGHTING)
        {
		chance -= 30;
        }

	init_affect(&af);
        af.where = TO_AFFECTS;
        af.level = ch->level + victim->drain_level;
        af.location = 0;
        af.type = gsn_blackjack;
        af.modifier = 0;
        af.bitvector = 0;
	af.aftype = AFT_SKILL;

	if (ch->level == MAX_LEVEL) {
		chance = 100;
	}
        if (number_percent() < chance)
        {
        act("You whack $N over the head with a heavy looking sack. Ouch.",ch,0,victim,TO_CHAR);
        act("$n whacks $N over the head with a heavy looking sack. Ouch.",ch,0,victim,TO_NOTVICT);
        send_to_char("You feel a sudden pain erupt through the back of your skull.\n\r",victim);
        af.duration = 2;
        affect_to_char(victim,&af);
        af.type=gsn_blackjack_timer;
	af.duration=4;
	affect_to_char(victim,&af);
	stop_fighting( victim, TRUE);
	victim->position = POS_SLEEPING;
        check_improve(ch,gsn_blackjack,TRUE,1);
        WAIT_STATE(ch,2*PULSE_VIOLENCE);
        return;
        }
        else
        {
        act("You attempt to blackjack $N but fail.",ch,0,victim,TO_CHAR);
        act("$n attempts to blackjack $N but misses.",ch,0,victim,TO_NOTVICT);
        act("$n hits you over the head with a heavy sack.",ch,0,victim,TO_VICT);
        af.duration = 2;
        affect_to_char(victim,&af);
        check_improve(ch,gsn_blackjack,FALSE,1);
        damage_old(ch,victim,2 + 2*number_range(2,5), gsn_blackjack,DAM_BASH,TRUE);
        WAIT_STATE(ch, 2*PULSE_VIOLENCE);
        if (!IS_NPC(victim))
           {
                sprintf(buf, "Help! %s tried to blackjack me!",PERS(ch,victim));
                do_myell(victim,buf);
                multi_hit(ch,victim,TYPE_UNDEFINED);
           }
        }
        return;
}


void do_bandage(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;
        char arg[MAX_INPUT_LENGTH];
        AFFECT_DATA af;

        one_argument(argument,arg);

        if ( (get_skill(ch,gsn_bandage) == 0)
        || (ch->level < skill_table[gsn_bandage].skill_level[ch->class]) )
        {
        send_to_char("Huh?\n\r",ch);
        return;
        }

        if (is_affected(ch,gsn_bandage))
        {
        send_to_char("You can't apply more aid yet.\n\r",ch);
        return;
        }
        if (ch->mana < 15)
        {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
        }

    if (arg[0] == '\0')
		victim = ch;
        else if ((victim = get_char_room(ch,arg)) == NULL)
        {
        send_to_char("They aren't here.\n\r",ch);
        return;
        }
        if (number_percent() > ch->pcdata->learned[gsn_bandage])
        {
        act("You fail to apply battle dressing to $N's wounds.",ch,0,victim,TO_CHAR);
        act("$n fumbles with $s bandages but fails to use them effectively.",ch,0,0,TO_ROOM);
        ch->mana -= 7;
        check_improve(ch,gsn_bandage,FALSE,3);
        return;
        }
        ch->mana -= 15;

        if (victim != ch)
        {
        act("$n applies bandages to $N's battle wounds.",ch,0,victim,TO_NOTVICT);
        act("You apply bandages to $N's battle wounds.",ch,0,victim,TO_CHAR);
        act("$n applies bandages to your battle wounds.",ch,0,victim,TO_VICT);
        }
        else
        {
        act("$n applies bandages to $mself.",ch,0,0,TO_ROOM);
        send_to_char("You apply battle dressing to yourself.\n\r",ch);
        }
        send_to_char("You feel better.\n\r",victim);

        victim->hit = UMIN(victim->hit + (9*ch->level), victim->max_hit);
	if (number_percent() < 60)
	{
	if (IS_AFFECTED(victim,AFF_PLAGUE))
	{
	affect_strip(victim,gsn_plague);
	act("The sores on $n's body vanish.\n\r",victim,0,0,TO_ROOM);
	send_to_char("The sores on your body vanish.\n\r",victim);
	}
	}
	if (number_percent() < 60)
	{
	 if (is_affected(victim, gsn_blindness))
	 {
	  affect_strip(victim, gsn_blindness);
	  send_to_char("Your vision returns!\n\r",victim);
	 }
	}
	if (number_percent() < 60)
	{
	if (is_affected(victim,gsn_poison))
	{
	affect_strip(victim,gsn_poison);
	send_to_char("A warm feeling goes through your body.\n\r",victim);
	act("$n looks better.",victim,0,0,TO_ROOM);
	}
	if (is_affected(victim,gsn_poison_dust))
	{
	affect_strip(victim,gsn_poison_dust);
	send_to_char("The dust in your eyes is cleaned out.\n\r",victim);
	act("The dust in $n's eyes looks cleaned out.",victim,0,0,TO_ROOM);
	}
	}

        check_improve(ch,gsn_bandage,TRUE,3);
	init_affect(&af);
        af.where = TO_AFFECTS;
        af.type = gsn_bandage;
        af.location = 0;
        af.duration = 2;
        af.modifier = 0;
        af.bitvector = 0;
        af.level = ch->level;
	af.aftype = AFT_POWER;
        affect_to_char(ch,&af);
        return;
}

void do_herb(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;
        char arg[MAX_INPUT_LENGTH];
        AFFECT_DATA af;

        one_argument(argument,arg);

        if ( (get_skill(ch,gsn_herb) == 0)
        || (ch->level < skill_table[gsn_herb].skill_level[ch->class]) )
        {
        send_to_char("Huh?\n\r",ch);
        return;
        }
        if (is_affected(ch,gsn_herb))
        {
        send_to_char("You can't find any more herbs.\n\r",ch);
        return;
        }
        if ((ch->in_room->sector_type != SECT_FOREST)
        && (ch->in_room->sector_type != SECT_HILLS)
        && (ch->in_room->sector_type != SECT_MOUNTAIN) )
        {
        send_to_char("You can't find any herbs here.\n\r",ch);
        return;
        }

	if (arg[0] == '\0')
		victim = ch;
       else   if ((victim = get_char_room(ch,arg)) == NULL)
        {
        send_to_char("They aren't here.\n\r",ch);
        return;
        }

        if (number_percent() > ch->pcdata->learned[gsn_herb])
        {
        send_to_char("You search for herbs but fail to find any.\n\r",ch);
        act("$n looks about in the bushes but finds nothing.",ch,0,0,TO_ROOM);
        check_improve(ch,gsn_herb,FALSE,4);
        return;
        }

        if (victim != ch)
        {
        act("$n applies herbs to $N.",ch,0,victim,TO_NOTVICT);
        act("You apply herbs to $N.",ch,0,victim,TO_CHAR);
        act("$n applies herbs to you.",ch,0,victim,TO_VICT);
        }

	if (victim == ch)
	{
        act("$n applies herbs to $mself.",ch,0,0,TO_ROOM);
        send_to_char("You find herbs and apply them to yourself.\n\r",ch);
        }

	send_to_char("You feel better.\n\r",victim);

	if (IS_AFFECTED(victim,AFF_PLAGUE) && number_percent() > 30)
	{
	affect_strip(victim,gsn_plague);
	act("The sores on $n's body vanish.\n\r",victim,0,0,TO_ROOM);
	send_to_char("The sores on your body vanish.\n\r",victim);
	}

        check_improve(ch,gsn_herb,TRUE,4);

        victim->hit = UMIN(victim->hit + (4*(ch->drain_level + ch->level)), victim->max_hit);
	init_affect(&af);
        af.where = TO_AFFECTS;
        af.type = gsn_herb;
        af.location = 0;
        af.duration = 4;
        af.modifier = 0;
        af.bitvector = 0;
	af.aftype = AFT_SKILL;
        af.level = ch->level + ch->drain_level;
        affect_to_char(ch,&af);
        return;
}

void do_cleave(CHAR_DATA *ch,char *argument)
{
	OBJ_DATA *weapon;
        CHAR_DATA *victim;
        char arg[MAX_INPUT_LENGTH];
        int dam, chance, dam_type;
        char buf[MAX_STRING_LENGTH];
        int sn;
	int skill;

        if ( ( ( chance = get_skill(ch,gsn_cleave)) == 0)
        || (ch->level < skill_table[gsn_cleave].skill_level[ch->class]) )
        {
        send_to_char("You don't know how to cleave.\n\r",ch);
        return;
        }

        one_argument(argument,arg);
        weapon = get_eq_char(ch,WEAR_WIELD);

    	if (weapon == NULL)
	{
        send_to_char("You need to wield a sword or axe to cleave.\n\r",ch);
        return;
        }

        if ((weapon->value[0] != WEAPON_SWORD)
        && (weapon->value[0] != WEAPON_AXE) )
        {
        send_to_char("You need to wield a sword or axe to cleave.\n\r",ch);
        return;
        }
        if ( (victim = get_char_room(ch,arg)) == NULL )
        {
        send_to_char("They aren't here.\n\r",ch);
        return;
        }
        if (victim == ch)
        {
        send_to_char("You can't do such a thing.\n\r",ch);
        return;
        }
        if (victim->fighting != NULL)
        {
        send_to_char("They are moving too much to cleave.\n\r",ch);
        return;
        }
        if (victim->hit < victim->max_hit*9/10)
        {
        send_to_char("They are too hurt and watchful to cleave right now...\n\r",ch);
        return;
        }

        if (is_safe(ch,victim))
                return;

	if (check_chargeset(ch,victim)) {return;}

        chance /= 5;
        if ((ch->drain_level + ch->level - victim->drain_level + victim->level) < 0)
                chance -= (ch->drain_level + ch->level - victim->drain_level - victim->level)*3;
        else
                chance += (ch->drain_level + ch->level - victim->level - victim->drain_level);
        chance += get_curr_stat(ch,STAT_STR)/2;
        chance -= get_curr_stat(victim,STAT_DEX)/3;  /* Improve evasion */
        chance -= get_curr_stat(victim,STAT_STR)/4;  /* Improve repelling */
        chance -= get_curr_stat(victim,STAT_CON)/4;  /* Shock survival */
        dam_type = attack_table[weapon->value[3]].damage;


        if (!can_see(victim,ch))
                chance += 5;
        chance -= dice(2,6);
        chance = URANGE(2,chance,10);
        if (number_percent() < 50)
                sprintf(buf,"Die, %s you butchering fool!",PERS(ch,victim));
        else
                sprintf(buf,"Help! %s just tried to cleave me in half!",PERS(ch,victim));

        sn = get_weapon_sn(ch);
        skill = get_weapon_skill(ch,sn) + ch->pcdata->learned[gsn_cleave] + 10;
        skill = URANGE(0,skill/2,100);

	if (get_skill(ch,gsn_zealotism) == 100) {
		chance += 100;
	}

        if (IS_IMP(ch)) {
                chance += 100;
        }

        act("You make a brutal swing at $N in an attempt to cleave them in half.",ch,0,victim,TO_CHAR);
        act("$n attempts to cleave you in half with a brutal slice.",ch,0,victim,TO_VICT);
        act("$n makes an attempt to cleave $N in half.",ch,0,victim,TO_NOTVICT);

	if (IS_IMP(victim)) {
		act("You make a brutal swing at $N in an attempt to cleave them in half, but miss and chop your own head off.",ch,0,victim,TO_CHAR);
		act("$n attempts to cleave you in half with a brutal slice, but misses and chops thier own head off.",ch,0,victim,TO_VICT);
		act("$n makes an attempt to cleave $N in half but misses and chops their own head off!",ch,0,victim,TO_NOTVICT);
		raw_kill(victim,ch);
		return;
	}

    if (IS_NPC(victim))
	victim->last_fought = ch;

        if (number_percent() > chance)
        {
        check_improve(ch,gsn_cleave,FALSE,5);
        if (weapon->pIndexData->new_format)
                dam = dice(weapon->value[1],weapon->value[2])*skill/100;
        else
                dam = number_range(weapon->value[1]*skill/100,weapon->value[2]*skill/100);
        dam *= number_range((ch->drain_level + ch->level)/10,(ch->drain_level + ch->level)/7);
        damage_old(ch,victim,dam,gsn_cleave,dam_type,TRUE);
        if (!IS_NPC(victim) && !IS_NPC(ch) && victim->hit > 1)
                do_myell(victim,buf);
	WAIT_STATE(ch,PULSE_VIOLENCE*3);
	WAIT_STATE(victim,PULSE_VIOLENCE*2);
        return;
        }

        act("Your cleave slices $S body in half with a clean cut!",ch,0,victim,TO_CHAR);
        act("$n cleaves you in half, tearing your body into two bloody bits!",ch,0,victim,TO_VICT);
        act("$n cleaves $N into to bits of bloody flesh!",ch,0,victim,TO_NOTVICT);
        check_improve(ch,gsn_cleave,TRUE,5);
        raw_kill(ch,victim);
        return;
}


void do_deathstrike(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;
        int chance, dam;
        char arg[MAX_INPUT_LENGTH];
        char buf[MAX_INPUT_LENGTH];
/*	AFFECT_DATA af;	*/

        one_argument(argument,arg);
        if (IS_NPC(ch))
                return;

	if ((get_skill(ch,gsn_deathstrike) == 0)
	|| ch->level < skill_table[gsn_deathstrike].skill_level[ch->class])
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}

	if (is_affected(ch,gsn_deathstrike))
	{
	send_to_char("You can't build up the intensity to deathstrike yet.\n\r",ch);
	return;
	}

        if (arg[0] == '\0')
        {
        send_to_char("Attempt to destroy who's skull?\n\r",ch);
        return;
        }
        if ( (victim = get_char_room(ch,arg)) == NULL)
        {
        send_to_char("They aren't here.\n\r",ch);
        return;
        }
        if (victim == ch)
        {
        send_to_char("You can't do that.\n\r",ch);
        return;
        }
        if (victim->fighting != NULL || victim->position == POS_FIGHTING)
        {
        send_to_char("They are moving around too much to get in close for the kill.\n\r",ch);
        return;
        }

        if (is_safe(ch,victim))
                return;

        if (victim->hit < victim->max_hit)
        {
        send_to_char("They have too much blood on them right now to locate their strike point.\n\r",ch);
        return;
        }
        chance = get_skill(ch,gsn_deathstrike)/2;
        chance += ch->level;
        chance -= victim->level * 3/2;
        chance -= number_range(0,15);
        if (!can_see(victim,ch))
                chance += 10;
        if (victim->position == POS_FIGHTING)
                chance -= 25;
        else if (victim->position == POS_SLEEPING)
                chance += 10;
        else chance -= 10;

        chance /= 2;
        chance = URANGE(2,chance,90);

        act("$n strikes out at $N with deadly intensity.",ch,0,victim,TO_NOTVICT);
        act("You strike out at $N with deadly intensity.",ch,0,victim,TO_CHAR);
        act("$n strikes at you with deadly intensity.",ch,0,victim,TO_VICT);

        if (number_percent() < chance)
        {
        send_to_char("With agonising pain your skull is smashed by the blow!\n\r",victim);
        act("Your blow shatters $N's skull into bloody fragments!",ch,0,victim,TO_CHAR);
        act("$N's skull is shattered into bits of mangled flesh and bone by $n's strike!",ch,0,victim,TO_NOTVICT);
        raw_kill(ch,victim);
        check_improve(ch,gsn_deathstrike,TRUE,3);
        return;
        }
        else
        {
        send_to_char("You feel a sharp pain searing your skull!\n\r",victim);
        act("Your deathstrike smashes $N's skull but fails to kill.",ch,0,victim,TO_CHAR);
        dam = ch->level * 2;
        dam += dice(ch->level, 4);
        damage_old(ch,victim,dam,gsn_deathstrike,DAM_PIERCE,TRUE);
        check_improve(ch,gsn_deathstrike,FALSE,3);
        if (!IS_NPC(victim) && !IS_NPC(ch) && (victim->hit > 1))
               {
                sprintf(buf,"Help! %s just tried to kill me!",PERS(ch,victim));
                do_myell(victim,buf);
                }
        multi_hit(victim,ch,TYPE_UNDEFINED);
        }

        return;
}

void do_crush(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;
        int chance, dam;

    if (IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM))
	return;

        if ( (chance = get_skill(ch,gsn_crush)) == 0
        || ch->level < skill_table[gsn_crush].skill_level[ch->class])
        if (!IS_NPC(ch))
	{
                send_to_char("Crushing, how do you do that again?\n\r",ch);
                return;
        }
	if ((victim = ch->fighting) == NULL)
	{
	send_to_char("You aren't fighting anyone.\n\r",ch);
	return;
	}
    if (is_affected(victim,gsn_protective_shield))
    {
	act("$N's crush attempt seems to slide around you.",victim,0,ch,TO_CHAR);
	act("$N's crush attempt seems to slide around $n.",victim,0,ch,TO_NOTVICT);
	act("Your crush attempt seems to slide around $n.",victim,0,ch,TO_VICT);
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
    }

    if (is_affected(victim,gsn_wraithform))
    {
        act("$N's crush attempt seems to slide around you.",victim,0,ch,TO_CHAR);
        act("$N's crush attempt seems to slide around $n.",victim,0,ch,TO_NOTVICT);
        act("Your crush attempt seems to slide around $n.",victim,0,ch,TO_VICT);
        WAIT_STATE(ch,PULSE_VIOLENCE*2);
        return;
    }

	chance -= chance/4;
        chance += (ch->drain_level + ch->level - victim->drain_level - victim->level) * 2;
        chance += get_curr_stat(ch,STAT_STR);
        chance -= get_curr_stat(victim,STAT_DEX)/3;
        chance -= get_curr_stat(victim,STAT_STR)/2;
        if (ch->size < victim->size)
                chance += (ch->size - victim->size)*25;
        else
                chance += (ch->size - victim->size)*10;
        if (number_percent() < chance)
	{
	if (check_roll(ch,victim, gsn_crush))
	{
	check_improve(ch,gsn_crush,TRUE,2);
	act("$N rolls free of $n's crush.",ch,0,victim,TO_NOTVICT);
	act("You roll out of $n's crush attempt.",ch,0,victim,TO_VICT);
	act("$N rolls free of your crush attempt.",ch,0,victim,TO_CHAR);
	return;
	}
        act("$n grabs you and slams you to the ground with bone crushing force!",ch,0,victim,TO_VICT);
        act("You grab $N and slam $M to the ground with bone crushing force!",ch,0,victim,TO_CHAR);
        act("$n slams $N to the ground with bone crushing force!",ch,0,victim,TO_NOTVICT);
        check_improve(ch,gsn_crush,TRUE,4);
        if ((ch->drain_level + ch->level) < 20)
                dam = 20;
        else if ((ch->drain_level + ch->level) < 25)
                dam = 30;
        else if ((ch->drain_level + ch->level) < 30)
                dam = 40;
        else if ((ch->drain_level + ch->level) < 35)
                dam = 50;
        else if ((ch->drain_level + ch->level) < 40)
                dam = 60;
        else if ((ch->drain_level + ch->level) < 52)
                dam = 70;
        else
                dam = 70;
        dam += str_app[get_curr_stat(ch,STAT_STR)].todam;
        WAIT_STATE(victim,2*PULSE_VIOLENCE);
        WAIT_STATE(ch,2*PULSE_VIOLENCE);
        damage_old(ch,victim,dam,gsn_crush,DAM_BASH,TRUE);
        victim->position = POS_RESTING;

        if (ch->fighting == victim)
                check_ground_control(ch,victim,chance,dam);
        return;

	}
        act("Your crush attempt misses $N.",ch,0,victim,TO_CHAR);
        act("$n lashes out wildly with $s arms but misses.",ch,0,0,TO_ROOM);
        WAIT_STATE(ch,2*PULSE_VIOLENCE);
        check_improve(ch,gsn_crush,FALSE,3);
        return;

}

void check_ground_control(CHAR_DATA *ch,CHAR_DATA *victim,int chance,int
dam)

{
	if (chance < 10)
		return;
	if (dam == 0)
		return;

        chance += get_skill(ch,gsn_ground_control)/2;
        chance /= 3;

  	chance -= number_range(5,15);
	chance = URANGE(5,chance,60);

        if (ch->level < skill_table[gsn_ground_control].skill_level[ch->class])
                return;
        if (number_percent() > chance)
	{
                check_improve(ch,gsn_ground_control,FALSE,3);
                return;

	}
        act("With brutal skill you grind $N against the ground with your weight.",ch,0,victim,TO_CHAR);
        act("$n takes hold of $N and grinds $M against the ground.",ch,0,victim,TO_NOTVICT);
        act("$n grinds you against the ground with brutal skill.",ch,0,victim,TO_VICT);
        dam -= 10;
        check_improve(ch,gsn_ground_control,TRUE,2);
        damage_old(ch,victim,dam,gsn_ground_control,DAM_BASH,TRUE);
        WAIT_STATE(victim,2*PULSE_VIOLENCE);
        return;

}


void do_riot(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *gch;
    int bonus;
     AFFECT_DATA af;

    if ( (get_skill(ch,gsn_riot) == 0 )
	|| ch->level < skill_table[gsn_riot].skill_level[ch->class])
	{
	send_to_char("Huh?\n\r",ch);
	return;
 	}

    if (ch->mana < 40)
    {
	send_to_char("You don't have the mana.\n\r",ch);
	return;
    }

    if (is_affected(ch,gsn_riot))
    {
	send_to_char("You can't stir up another riot yet.\n\r",ch);
	return;
    }
    if (number_percent() > ch->pcdata->learned[gsn_riot])
    {
	send_to_char("You try to incite a riot amongst your group but fail.\n\r",ch);
	act("$n gestures around angrily, trying to build up anger and discord but fails.",ch,0,0,TO_ROOM);
	ch->mana -= 20;
	check_improve(ch,gsn_riot,FALSE,2);
	return;
    }

    send_to_char("You stir up a riot with your actions!\n\r",ch);
	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype = AFT_POWER;
    af.type = gsn_riot;
    af.level = ch->level + ch->drain_level;
    af.bitvector = 0;
    af.duration = ( 2 + (ch->drain_level + ch->level)/8);
    af.location = APPLY_HITROLL;

    if ((ch->drain_level + ch->level) < 20)
	bonus = 6;
    else if ((ch->drain_level + ch->level) < 25)
	bonus = 7;
    else if ((ch->drain_level + ch->level) < 30)
	bonus = 8;
    else if ((ch->drain_level + ch->level) < 35)
	bonus = 9;
    else if ((ch->drain_level + ch->level) < 38)
	bonus = 10;
    else
	bonus = 12;

    af.modifier = bonus;
    affect_to_char(ch,&af);
    af.location = APPLY_DAMROLL;
    affect_to_char(ch,&af);
    send_to_char("You are filled with terrible fury!\n\r",ch);

    for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
    {
	if (!is_same_group(ch,gch) || !IS_AWAKE(gch))
		continue;
	if (is_affected(gch,gsn_riot))
		continue;
	if (gch->trust > 51)
	        continue;
	if (gch == ch)
		continue;
	af.location = APPLY_DAMROLL;
	affect_to_char(gch,&af);
	af.location = APPLY_HITROLL;
	affect_to_char(gch,&af);
	send_to_char("You are filled with a terrible fury!\n\r",gch);
    	act("$n gets a terrible look of anger in $s eyes.",gch,0,0,TO_ROOM);
    }

    check_improve(ch,gsn_riot,TRUE,2);
    ch->mana -= 40;
    return;
}



void do_tail(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance, dam;
    int knock;

    one_argument(argument,arg);

    if ((IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM)
    && (!check_rangermob(ch))) )
	return;

    if ( (chance = get_skill(ch,gsn_tail)) == 0
    ||   (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_TAIL))
    ||   (!IS_NPC(ch)
    &&    ch->level < skill_table[gsn_tail].skill_level[ch->class]))
    {
	send_to_char("You don't have a tail to strike with like that.\n\r",ch);
        return;
    }

    if (arg[0] == '\0')
    {
        victim = ch->fighting;
        if (victim == NULL)
        {
            send_to_char("But you aren't fighting anyone!\n\r",ch);
            return;
        }
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    chance = URANGE(5,chance,90);

    chance += (ch->level - victim->level);
    chance -= get_curr_stat(victim,STAT_DEX);
    chance += get_curr_stat(ch,STAT_DEX)/3;
    chance += get_curr_stat(ch,STAT_STR)/3;

	act("$n violently lashes out with $s tail.",ch,0,0,TO_ROOM);
	send_to_char("You violently lash out with your tail!\n\r",ch);

    if (number_percent() > chance)
    {
	damage_old(ch,victim,0,gsn_tail,DAM_BASH,TRUE);
	return;
    }

    dam = dice(ch->level, 3);
    knock = (ch->level + 15);

    damage_old(ch,victim,dam,gsn_tail,DAM_BASH,TRUE);
    if (number_percent() < knock)
    {
	act("$n is sent crashing to the ground by the force of the blow!",victim,0,0,TO_ROOM);
	send_to_char("The tail strike sends you crashing to the ground!\n\r",victim);
	WAIT_STATE(victim,( 3 * PULSE_VIOLENCE/2));
    }
    WAIT_STATE(ch,2*PULSE_VIOLENCE);
    return;
}


void do_spellbane(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;

	if (get_skill(ch,gsn_spellbane) < 10
	|| ch->level < skill_table[gsn_spellbane].skill_level[ch->class])
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}
	if (is_affected(ch,gsn_spellbane))
	{
	send_to_char("You are already turning spells.\n\r",ch);
	return;
	}

	if (ch->mana < 50)
	{
send_to_char("You don't have the mana.\n\r",ch);
	return;
	}

	if (number_percent() > get_skill(ch,gsn_spellbane))
	{
	send_to_char("You think of your hatred of magic but can not sustain it.\n\r",ch);
act("$n makes some angry gestures and goes red in the face, but nothing happens.",ch,0,0,TO_ROOM);
	ch->mana -= 25;
	check_improve(ch,gsn_spellbane,FALSE,1);
	return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = gsn_spellbane;
	af.level = ch->level + ch->drain_level;
	af.duration = (5 + (ch->drain_level + ch->level)/5);
	af.location = APPLY_SAVES;
	af.modifier = (-5 -ch->level/4);
	af.bitvector = 0;
	af.aftype = AFT_POWER;

	affect_to_char(ch,&af);

	act("$n fills the air with $s hatred of magic.",ch,0,0,TO_ROOM);
	send_to_char("You surround yourself with your hatred of magic.\n\r",ch);
    if (is_affected(ch,gsn_undead_drain))
    {
	if (saves_spell(50,ch,DAM_OTHER))
	{
	send_to_char("Your spellbane burns away the negative draining magic of the undead.\n\r",ch);

	affect_strip(ch,gsn_undead_drain);
	}
    }

check_improve(ch,gsn_spellbane,TRUE,1);
	ch->mana -= 50;
	return;

}

bool spellbaned(CHAR_DATA *caster, CHAR_DATA *victim, int sn)
{
	bool status = FALSE;
  	int chance;

    if (!is_affected(victim,gsn_spellbane))
	return FALSE;

    if(skill_table[sn].ctype == CMD_NONE)
    return FALSE;

    if ( sn >= TYPE_HIT && sn < (TYPE_HIT + MAX_DAMAGE_MESSAGE) )
	return FALSE;

    if ( skill_table[sn].target == TAR_CHAR_DEFENSIVE)
	status = TRUE;
    else
	{
	chance = 25 + (victim->drain_level + victim->level - caster->level - caster->drain_level) * 2;
	chance -= victim->saving_throw;
	if (IS_NPC(victim) &&
	victim->pIndexData->vnum == MOB_VNUM_RAVAGER)
		chance += 25;

	chance = URANGE(10,chance,25);
	if (number_percent() < chance)
		status = TRUE;
	}

   if (status)
	{
	act("$n's spellbane reflects $N's spell!",victim,0,caster,TO_NOTVICT);
	act("Your spellbane reflects $N's spell!",victim,0,caster,TO_CHAR);
	act("$n's spellbane reflects your spell!",victim,0,caster,TO_VICT);

	damage_old(victim,caster,dice(caster->level,2),gsn_spellbane,DAM_ENERGY,TRUE);
	}

    return status;
}

void do_throw(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *victim;
    int dam, chance;
    char arg[MAX_INPUT_LENGTH];

    if ( (chance = get_skill(ch,gsn_throw)) == 0
    || ch->level < skill_table[gsn_throw].skill_level[ch->class])
    {
        send_to_char("Throwing? What's that?\n\r",ch);
        return;
    }

    one_argument(argument,arg);
    if (arg[0] == '\0')
        victim = ch->fighting;
    else
        victim = get_char_room(ch,arg);

    if (victim == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }
    if (victim == ch)
    {
        send_to_char("Now that would be a bit stupid.\n\r",ch);
        return;
    }
    if ((victim->fighting != ch) && (ch->fighting != victim))
    {
        send_to_char("But you aren't engaged in combat with them.\n\r",ch);
        return;
    }

    if (hands_full(ch))
    {
        send_to_char("You can't throw, your hands are filled with items.\n\r",ch);
        return;
    }
/*
    if (is_affected(ch,gsn_palm))
    {
        send_to_char("You can't throw, you're fighting with open hand palms.\n\r",ch);
        return;
    }
*/
    if (is_affected(victim,gsn_protective_shield))
    {
	act("$N's throw seems to slide around you.",victim,0,ch,TO_CHAR);
	act("$N's throw seems to slide around $n.",victim,0,ch,TO_NOTVICT);
	act("Your throw seems to slide around $n.",victim,0,ch,TO_VICT);
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
    }
    chance += (ch->drain_level + ch->level - victim->level - victim->drain_level);
    chance -= get_curr_stat(victim,STAT_DEX);
    chance += get_curr_stat(ch,STAT_DEX)/2;
    chance *= 9;
    chance /= 10;
    chance = URANGE(5, chance, 90);

    if (number_percent() > chance)
    {
        dam = 0;
	check_improve(ch,gsn_throw,FALSE,3);
    }
    else
    {
	if (check_roll(ch,victim,gsn_throw))
	{
	check_improve(ch,gsn_throw,TRUE,2);
	act("$N rolls free of $n's throw.",ch,0,victim,TO_NOTVICT);
	act("You roll out of $n's throw attempt.",ch,0,victim,TO_VICT);
	act("$N rolls free of your throw attempt.",ch,0,victim,TO_CHAR);
	return;
	}

        if ((ch->drain_level + ch->level) <= 18)    dam = 30;
        else if ((ch->drain_level + ch->level) <= 22)   dam = 35;
        else if ((ch->drain_level + ch->level) <= 25)   dam = 40;
        else if ((ch->drain_level + ch->level) <= 28)   dam = 45;
        else if ((ch->drain_level + ch->level) <= 32)   dam = 48;
        else if ((ch->drain_level + ch->level) <= 35)   dam = 52;
        else if ((ch->drain_level + ch->level) <= 38)   dam = 56;
        else if ((ch->drain_level + ch->level) <= 40)   dam = 60;
        else if ((ch->drain_level + ch->level) <= 43)   dam = 64;
        else if ((ch->drain_level + ch->level) <= 45)   dam = 68;
        else if ((ch->drain_level + ch->level) <= 48)   dam = 70;
        else                        dam = 72;

	act("$n grabs $N and throws $M to the ground with stunning force!",ch,0,victim,TO_NOTVICT);
	act("You grab $N and throw $M to the ground with stunning force!",ch,0,victim,TO_CHAR);
	act("$n grabs you and throws you to the ground with stunning force!",ch,0,victim,TO_VICT);

        dam += str_app[get_curr_stat(ch,STAT_STR)].todam;
        check_improve(ch,gsn_throw,TRUE,3);
        WAIT_STATE(victim,2*PULSE_VIOLENCE);
    }

    damage_old(ch,victim,dam,gsn_throw,DAM_BASH,TRUE);
    WAIT_STATE(ch,2*PULSE_VIOLENCE);
    if (ch->fighting == victim)
        check_ground_control(ch,victim,chance,dam);

    return;
}

void do_palm(CHAR_DATA *ch,char *argument)
{
    AFFECT_DATA af;
    int chance;

    if ( (chance = get_skill(ch,gsn_palm)) == 0
    || ch->level < skill_table[gsn_palm].skill_level[ch->class])
    {
        send_to_char("You don't have the ability to use palm striking martial arts.\n\r",ch);
        return;
    }

    if (is_affected(ch,gsn_palm))
    {
        send_to_char("You're already fighting with concentrated open hand martial arts.\n\r",ch);
        return;
    }

    if (ch->mana <= 60)
    {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
    }

    if (get_eq_char(ch,WEAR_WIELD) != NULL
	|| get_eq_char(ch,WEAR_DUAL_WIELD) != NULL)
	{
	send_to_char("You can't start using open palm striking while using weapons.\n\r",ch);
	return;
	}

    if (number_percent() >= chance)
    {
        send_to_char("You flex your hands but cannot grasp the concentration required.\n\r",ch);
        check_improve(ch,gsn_palm,FALSE,2);
        ch->mana -= 30;
        return;
    }

    act("$n slowly spreads $s hands and a look of deadly concentration clouds $s eyes.",ch,0,0,TO_ROOM);
    send_to_char("You feel your mind relax and pass power to your opened hands.\n\r",ch);
	init_affect(&af);
    af.where = TO_AFFECTS;
    af.type = gsn_palm;
    af.location = 0;
    af.modifier = 0;
    af.duration = ((ch->drain_level + ch->level)/3);
    af.level = ch->drain_level + ch->level;
    af.bitvector = 0;
	af.aftype = AFT_SKILL;
    affect_to_char(ch,&af);
    check_improve(ch,gsn_palm,TRUE,2);

    return;
}

bool check_palm(CHAR_DATA *ch)
{
    if (!is_affected(ch,gsn_palm))
        return FALSE;
    if (number_percent() > 15)
        return FALSE;
    return TRUE;
}

void do_nerve(CHAR_DATA *ch,char *argument)
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    AFFECT_DATA af;
    int chance;

    if ( (chance = get_skill(ch,gsn_nerve)) == 0
    || ch->level < skill_table[gsn_nerve].skill_level[ch->class])
    {
        send_to_char("You don't know how to use nerve pressure tactics.\n\r",ch);
        return;
    }

    one_argument(argument,arg);
    if (arg[0] == '\0')
        victim = ch->fighting;
    else
        victim = get_char_room(ch,arg);

    if (victim == NULL)
    {
        send_to_char("Attempt to put pressure on who's nerves?\n\r",ch);
        return;
    }
    if (victim == ch)
    {
        send_to_char("You can't do that.\n\r",ch);
        return;
    }
    if (is_safe(ch,victim))
	return;

    if (is_affected(victim,gsn_nerve))
    {
	send_to_char("They have already been weakened using nerve pressure.\n\r",ch);
	return;
    }


    chance += (ch->drain_level + ch->level - victim->level  - victim->drain_level)*3;
    chance -= get_curr_stat(victim,STAT_DEX)/3;
    chance += get_curr_stat(ch,STAT_DEX)/2;
    chance -= get_curr_stat(victim,STAT_CON)/3;

    if (number_percent() > chance)
    {
        act("$n grasps $N's arm but fails to apply the right pressure point.",ch,0,victim,TO_NOTVICT);
        act("You grasp $N's arm but fail to apply the right pressure point.",ch,0,victim,TO_CHAR);
        act("$n grasps your arm but fails to apply the right pressure point.",ch,0,victim,TO_VICT);
        check_improve(ch,gsn_nerve,FALSE,3);
        WAIT_STATE(ch,PULSE_VIOLENCE);
        return;
    }
    else
    {
    act("$n grasps $N's arm and weakens $m with pressure points.",ch,0,victim,TO_NOTVICT);
    act("You grasp $N's arm and weaken $m with pressure points.",ch,0,victim,TO_CHAR);
    act("$n grasps your arm and weakens you with pressure point.",ch,0,victim,TO_VICT);
    check_improve(ch,gsn_nerve,TRUE,3);
	init_affect(&af);
    af.where = TO_AFFECTS;
    af.type = gsn_nerve;
    af.location = APPLY_STR;
    af.duration = ((ch->drain_level + ch->level)/5);
    af.bitvector = 0;
    af.modifier = -3;
	af.aftype = AFT_SKILL;
    af.level = ch->drain_level + ch->level;
    affect_to_char(victim,&af);
    WAIT_STATE(ch,PULSE_VIOLENCE);
    }

	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (ch->fighting == NULL
	|| victim->fighting == NULL) )
	{
sprintf(buf,"Help, %s is attacking me!",PERS(ch,victim));
do_myell(victim,buf);
	}
    if (victim->fighting == NULL)
    {
	multi_hit(victim,ch,TYPE_UNDEFINED);
    }

    return;
}

void check_follow_through(CHAR_DATA *ch, CHAR_DATA *victim, int dam)
{
    int chance;

    chance = get_skill(ch,gsn_follow_through);

    if (ch->level <skill_table[gsn_follow_through].skill_level[ch->class])
        return;

    chance /= 2;
    chance -= number_range(0,15);

    if (chance <= 0)
	return;

    if (number_percent() < chance)
    {
        act("$n's leg sweeps around and strikes with a follow through!",ch,0,0,TO_ROOM);
        act("Your leg sweeps around to land a follow through kick!",ch,0,0,TO_CHAR);
        damage_old(ch,victim, 4*dam/5, gsn_follow_through, DAM_BASH,TRUE);
        check_improve(ch,gsn_follow_through,TRUE,2);
    }
    else
        check_improve(ch,gsn_follow_through,FALSE,3);

    return;
}



void do_blindness_dust(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
        AFFECT_DATA af;
        int chance;
        bool fighting = FALSE;
        char buf[MAX_STRING_LENGTH];

        if ( (chance = get_skill(ch,gsn_blindness_dust)) == 0
        || ch->level < skill_table[gsn_blindness_dust].skill_level[ch->class])
        {
        send_to_char("You don't know how to make blindness dust to throw.\n\r",ch);
        return;
        }
        if (ch->mana < 18)
        {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
        }
        if (number_percent() > chance)
        {
        act("$n hurls some dust into the air but it is blown away.",ch,0,0,TO_ROOM);
        send_to_char("You throw out some dust but it is blown away.\n\r",ch);
        ch->mana -= 9;
        check_improve(ch,gsn_blindness_dust,FALSE,2);
        WAIT_STATE(ch,12);
        return;
        }
        act("$n hurls a handful of dust into the room!",ch,0,0,TO_ROOM);
        send_to_char("You throw a handful of blindness dust into the room!\n\r",ch);
        check_improve(ch,gsn_blindness_dust,TRUE,2);
        if (ch->fighting != NULL)
                fighting = TRUE;

	init_affect(&af);
        af.where = TO_AFFECTS;
        af.type = gsn_blindness_dust;
        af.level = ch->drain_level + ch->level;
        af.duration = 1;
        af.bitvector = AFF_BLIND;
        af.location = APPLY_HIT;
        af.modifier = -3;
	af.aftype = AFT_SKILL;
        ch->mana -= 18;
        WAIT_STATE(ch,12);
        for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
        {
        vch_next = vch->next_in_room;
        if (is_safe(ch,vch))
		 continue;
        if (is_same_group(ch,vch))
		continue;
        if (!IS_AFFECTED(vch,AFF_BLIND)
        && !saves_spell(ch->drain_level + ch->level,vch,DAM_OTHER))
	{
	act("$n appears blinded.",vch,0,0,TO_ROOM);
	send_to_char("You get dust in your eyes.\n\r",vch);
                affect_to_char(vch,&af);
	}
        if (!IS_NPC(vch) && !IS_NPC(ch)
        && (vch->fighting == NULL || (!fighting)))
                {
        sprintf(buf,"Help! %s just threw dust in my eyes!",PERS(ch,vch));
                do_myell(vch,buf);
                }

                if (vch->fighting == NULL)
                        multi_hit(vch,ch,TYPE_UNDEFINED);

        }

        return;
}

void do_poison_dust(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
        AFFECT_DATA af;
        int chance;
        bool fighting = FALSE;
        char buf[MAX_STRING_LENGTH];

        if ( (chance = get_skill(ch,gsn_poison_dust)) == 0
        || ch->level < skill_table[gsn_poison_dust].skill_level[ch->class])
        {
        send_to_char("You don't know how to make poison dust to throw.\n\r",ch);
        return;
        }
        if (ch->mana < 20)
        {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
        }
        if (number_percent() > chance)
        {
        act("$n hurls some dust into the air but it is blown away.",ch,0,0,TO_ROOM);
        send_to_char("You throw out some dust but it is blown away.\n\r",ch);
        ch->mana -= 10;
        WAIT_STATE(ch,12);
        check_improve(ch,gsn_poison_dust,FALSE,2);
        return;
        }
        act("$n hurls a handful of dust into the room!",ch,0,0,TO_ROOM);
        send_to_char("You throw a handful of poison dust into the room!\n\r",ch);
        check_improve(ch,gsn_poison_dust,TRUE,2);
          if (ch->fighting != NULL)
                fighting = TRUE;

	init_affect(&af);
        af.where = TO_AFFECTS;
        af.type = gsn_poison_dust;
        af.level = ch->drain_level + ch->level;
        af.duration = (ch->drain_level + ch->level)/5;
        af.bitvector = AFF_POISON;
        af.location = APPLY_STR;
        af.modifier = -3;
	af.aftype = AFT_SKILL;
        ch->mana -= 20;
        WAIT_STATE(ch,12);
        for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
        {
        vch_next = vch->next_in_room;
        if (is_safe(ch,vch))    continue;
        if (is_same_group(ch,vch))      continue;
        if (!IS_AFFECTED(vch,AFF_POISON)
        && !saves_spell(ch->drain_level + ch->level,vch,DAM_OTHER))
	{
	act("$n turns green and looks sick.",vch,0,0,TO_ROOM);
	send_to_char("You suddenly feel very sick.\n\r",vch);
                affect_to_char(vch,&af);
	}

        if (!IS_NPC(vch) && !IS_NPC(ch)
        && (vch->fighting == NULL || (!fighting)))
                {
        sprintf(buf,"Help! %s just threw dust in my eyes!",PERS(ch,vch));
                do_myell(vch,buf);
                }
                if (vch->fighting == NULL)
                        multi_hit(vch,ch,TYPE_UNDEFINED);

        }
        return;
}

bool check_roll(CHAR_DATA *ch, CHAR_DATA *victim, int dt)
{
        int chance;

        if ( (chance = get_skill(victim,gsn_roll)) == 0
        || victim->level < skill_table[gsn_roll].skill_level[victim->class])
                return FALSE;

        chance *= 4;
        chance /= 5;
        chance += get_curr_stat(victim,STAT_DEX);
        chance -= victim->carry_weight/100;
        if (number_percent() > chance)
                {
                check_improve(victim,gsn_roll,FALSE,2);
                return FALSE;
                }

        check_improve(victim,gsn_roll,TRUE,2);
        return TRUE;
}

void do_warcry(CHAR_DATA *ch,char *argument)
{
        AFFECT_DATA af;
        int chance;
        if ( (chance = get_skill(ch,gsn_warcry)) == 0
        || ch->level < skill_table[gsn_warcry].skill_level[ch->class])
        {
        send_to_char("You don't know how to warcry properly.\n\r",ch);
        return;
        }

        if (is_affected(ch,gsn_warcry))
        {
        send_to_char("You are already affected by a warcry.\n\r",ch);
        return;
        }
        if (ch->mana < 20)
        {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
        }

        if (number_percent() > chance)
        {
        act("$n makes some soft grunting noises.",ch,0,0,TO_ROOM);
        send_to_char("You make soft grunting sounds but nothing happens.\n\r",ch);
        check_improve(ch,gsn_warcry,FALSE,2);
        ch->mana -= 10;
        WAIT_STATE(ch,12);
        return;
        }

        act("$n lets out a blood freezing warcry!",ch,0,0,TO_ROOM);
        send_to_char("You let out a fierce warcry!\n\r",ch);
        check_improve(ch,gsn_warcry,TRUE,2);
	init_affect(&af);
        af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
        af.type = gsn_warcry;
        af.modifier = (ch->drain_level + ch->level)/7;
        af.location = APPLY_HITROLL;
        af.duration = ch->drain_level + ch->level;
        af.bitvector = 0;
        af.level = ch->drain_level + ch->level;
        affect_to_char(ch,&af);
	af.location = APPLY_SAVES;
        af.modifier = -(ch->level/10);
        affect_to_char(ch,&af);
        ch->mana -= 20;
        WAIT_STATE(ch,12);
        return;
}

void do_strangle(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;
        char arg[MAX_INPUT_LENGTH];
        char buf[MAX_STRING_LENGTH];
        int chance;
        AFFECT_DATA af;

        one_argument(argument,arg);

        if ( (chance = get_skill(ch,gsn_strangle)) == 0
        || ch->level < skill_table[gsn_strangle].skill_level[ch->class])
        {
        send_to_char("You don't know how to strangle properly.\n\r",ch);
        return;
	}

        if ((victim = get_char_room(ch,arg)) == NULL)
        {
        send_to_char("They aren't here.\n\r",ch);
        return;
        }
        if (victim == ch)
        {
        send_to_char("Try using a piece of rope and a high tree branch maybe?\n\r",ch);
        return;
        }
        if (is_affected(victim,gsn_strangle) && !IS_AWAKE(victim))
        {
        send_to_char("They are already lying on the ground unconcious.\n\r",ch);
        return;
        }
        if (victim->position == POS_FIGHTING)
        {
        send_to_char("They are moving around too much to strangle.\n\r",ch);
        return;
        }
    if (is_safe(ch,victim))
	return;

	if (check_chargeset(ch,victim)) {return;}

    if (!(str_cmp(race_table[victim->race].name,"lich")))
    {
        act("$n grabs hold of $N's neck, but with no affect.",ch,0,victim,TO_NOTVICT);
        act("You grab hold of $N's neck, but with no affect",ch,0,victim,TO_CHAR);
        send_to_char("Someone grabs hold of your neck. It doesn't affect you.\n\r",victim);
        return;
    }

	update_pc_last_fight(ch,victim);

        if (is_affected(victim,gsn_strangle) || is_affected(victim,gsn_strangle_timer) )
        {
        send_to_char("They are guarding their necks too well right now.\n\r",ch);
        return;
        }

	init_affect(&af);
        af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
        af.type = gsn_strangle;
        af.level = ch->drain_level + ch->level;
        af.duration = 2;
        af.modifier = 0;
        af.location = 0;
        af.bitvector = AFF_SLEEP;
        affect_to_char(victim,&af);

	chance /= 2;
        chance += ( 2*(ch->drain_level + ch->level) - 2*(victim->drain_level + victim->level));
        chance -= get_curr_stat(victim,STAT_DEX)/2;
	chance += 10;
	if(is_affected(victim,gsn_cloak_form))
		chance+=20;
	if (chance > 85)
		chance = 85;

        if (number_percent() < 50)
                sprintf(buf,"Help! %s is trying to strangle me!",PERS(ch,victim));
        else
                sprintf(buf,"Help! %s just tried to strangle me!",PERS(ch,victim));

        if (number_percent() > chance)
        {
	af.bitvector = 0;
	af.duration = 2;
	affect_to_char(victim,&af);
        damage_old(ch,victim,0,gsn_strangle,DAM_BASH,TRUE);
        check_improve(ch,gsn_strangle,FALSE,2);
        WAIT_STATE(ch,12);

        if (!IS_NPC(ch) && !IS_NPC(victim))
                do_myell(victim,buf);
        multi_hit(victim,ch,TYPE_UNDEFINED);
	return;
        }
        act("$n grabs hold of $N's neck and puts them to sleep.",ch,0,victim,TO_NOTVICT);
        act("You grab hold of $N's neck and put them to sleep.",ch,0,victim,TO_CHAR);
        send_to_char("Someone grabs hold of your neck and puts you to sleep.\n\r",victim);
        victim->position = POS_SLEEPING;
        check_improve(ch,gsn_strangle,TRUE,2);
        WAIT_STATE(ch,12);
        return;
}

void check_downstrike(CHAR_DATA *ch,CHAR_DATA *victim)
{
    OBJ_DATA *wield;
    int dam;
    int chance;

    chance = get_skill(ch,gsn_downstrike);
    if (chance <= 10)	return;
    if (ch->level < skill_table[gsn_downstrike].skill_level[ch->class])
	return;

    chance /= 4.5;
    chance += (ch->drain_level + ch->level - victim->level - victim->drain_level);
    chance -= get_curr_stat(victim,STAT_DEX)/3;
    chance += get_curr_stat(ch,STAT_DEX)/3;

    chance = URANGE(2, chance, 35);

    if (number_percent() > chance)
    {
    check_improve(victim,gsn_downstrike,FALSE,2);
	return;
    }


    wield = get_eq_char(ch,WEAR_WIELD);
    if (wield == NULL || wield->value[0]!=2)
	wield = get_eq_char(ch,WEAR_DUAL_WIELD);
    if (wield == NULL || wield->value[0]!=2)
	return;

    dam = dice(wield->value[1],wield->value[2]);
    dam += ch->damroll;
    act("You strike at $N with a downward thrust of your dagger!",ch,0,victim,TO_CHAR);
    act("$n strikes at you with a downward thrust of $s dagger!",ch,0,victim,TO_VICT);
    act("$n strikes at $N with a downward thrust of $s dagger!",ch,0,victim,TO_NOTVICT);
    check_improve(victim,gsn_downstrike,TRUE,2);
    damage_old(ch,victim,dam,gsn_downstrike,attack_table[wield->value[3]].damage, TRUE);
    return;
}




void do_enlist(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    int level;
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA af;
    int chance;
    CHAR_DATA *check;

    chance = get_skill(ch,gsn_enlist);
    if (chance <= 10
    || ch->level < skill_table[gsn_enlist].skill_level[ch->class])
    {
        send_to_char("You do not have the skills required to enlist mercenary aid.\n\r",ch);
        return;
    }
    one_argument(argument,arg);
    if (is_affected(ch,gsn_enlist))
    {
	send_to_char("You haven't built up the nerve to recruit anyone else yet.\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
        send_to_char("Who do you wish to enlist as a follower?\n\r",ch);
        return;
    }
    if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }
    if (victim == ch)
    {
        send_to_char("You can't recruit yourself.\n\r",ch);
        return;
    }
    if (!IS_NPC(victim))
    {
        send_to_char("Why not just ask them to join you?\n\r",ch);
        return;
    }
    for (check = char_list; check != NULL; check = check->next)
    {
	if (is_affected(check,gsn_enlist) && check->master == ch)
	{
	send_to_char("You already have a devoted recruit following you.\n\r",ch);
	return;
	}
    }

    if (!IS_SET(victim->act,ACT_WARRIOR))
    {
	send_to_char("You may only enlist the aid of those trained as warriors.\n\r",ch);
        return;
    }

    level = ch->drain_level + ch->level;

    if ((IS_GOOD(ch) && IS_EVIL(victim))
       || (IS_EVIL(ch) && IS_GOOD(victim)))
       level = 0;

    if (IS_NEUTRAL(victim) || IS_NEUTRAL(ch))
        level -= 4;

    if (IS_AFFECTED(victim,AFF_CHARM))
        level = 0;
    if ( (victim->level + 8) > level)
	level = 0;

    if (saves_spell(level,victim,DAM_OTHER) || level == 0)
    {
        if (number_percent() < 50)
            do_say(victim,"I don't follow scum such as you!");
        else
            do_say(victim,"You couldn't recruit a mangy dog to your company!");
        sprintf(buf,"Help! I'm being attacked by %s!",PERS(victim,ch));
        do_myell(ch,buf);
        multi_hit(victim,ch,TYPE_UNDEFINED);
        return;
    }

    victim->leader = ch;
    victim->master = ch;
	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
    af.type = gsn_enlist;
    af.modifier = 0;
    af.level = ch->drain_level + ch->level;
    af.bitvector = AFF_CHARM;
    af.duration = ch->drain_level + ch->level;
    af.location = 0;
    affect_to_char(victim,&af);
    af.bitvector = 0;
    af.duration = 15;
    affect_to_char(ch,&af);
    act("$N salutes you and prepares to follow into combat.",ch,0,victim,TO_CHAR);
    act("$N salutes $n and falls into line with $s.",ch,0,victim,TO_NOTVICT);
    return;
}



void do_tame(CHAR_DATA *ch, char *argument)
{
        CHAR_DATA *victim;
        char arg[MAX_INPUT_LENGTH];
        int chance;

        chance = get_skill(ch,gsn_tame);
        one_argument(argument,arg);
        if (arg[0] == '\0')
                victim = ch->fighting;
        else
                victim = get_char_room(ch,arg);
        if (victim == NULL)
        {
        send_to_char("They aren't here to tame though.\n\r",ch);
        return;
        }
        if (!IS_NPC(victim))
        {
        send_to_char("Why not just talk to them about your problems?\n\r",ch);
        return;
        }
        else if (!IS_SET(victim->act,ACT_AGGRESSIVE)
        && !IS_SET(victim->off_flags,SPAM_MURDER))
        {
        send_to_char("They are as tame as they can be.\n\r",ch);
        return;
        }

        chance += (ch->drain_level + ch->level - victim->level - victim->drain_level)*3;
        if (chance > 90)        chance = 90;
        WAIT_STATE(ch,12);
        if (number_percent() > chance)
        {
        act("$n tries to calm down $N but fails.",ch,0,victim,TO_NOTVICT);
        act("$n tries to calm you down but fails.",ch,0,victim,TO_VICT);
        act("You try to calm $N down but fail.",ch,0,victim,TO_CHAR);
        check_improve(ch,gsn_tame,FALSE,2);
        return;
        }
        act("$n calms $N down.",ch,0,victim,TO_NOTVICT);
        act("You calm $N down.",ch,0,victim,TO_CHAR);
        act("$n calms you down.",ch,0,victim,TO_VICT);
        check_improve(ch,gsn_tame,TRUE,2);
        stop_fighting(victim,TRUE);
     victim->last_fought = NULL;
        REMOVE_BIT(victim->act,ACT_AGGRESSIVE);
        REMOVE_BIT(victim->off_flags,SPAM_MURDER);
        return;
}

void do_find_water(CHAR_DATA *ch,char *argument)
{
        OBJ_DATA *spring;
        int chance;
        chance = get_skill(ch,gsn_find_water);
        if (chance > 95)        chance = 95;
        if (chance == 0
        || ch->level < skill_table[gsn_find_water].skill_level[ch->class])
        {
        send_to_char("You poke the ground with a stick but find no water that way.\n\r",ch);
        return;
        }
        if (ch->mana < 15)
        {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
        }
        if (ch->in_room->sector_type == SECT_WATER_SWIM
        || ch->in_room->sector_type == SECT_WATER_NOSWIM)
        {
        send_to_char("Water water all around but not a drop to drink..\n\r",ch);
        return;
        }
        if (number_percent() > chance)
        {
        act("$n pokes the ground with a stick then scratches $s head.",ch,0,0,TO_ROOM);
        send_to_char("You poke about on the ground but fail to find any water.\n\r",ch);
        check_improve(ch,gsn_find_water,FALSE,1);
        ch->mana -= 7;
        WAIT_STATE(ch,18);
        return;
        }
        act("$n pokes at the ground and digs up a spring of natural water!",ch,0,0,TO_ROOM);
        send_to_char("You poke about for a bit and eventually dig up a spring of water.\n\r",ch);
        WAIT_STATE(ch,18);
        ch->mana -= 15;
        spring = create_object(get_obj_index(OBJ_VNUM_SPRING),0);
        check_improve(ch,gsn_find_water,TRUE,1);
        obj_to_room(spring,ch->in_room);
        return;
}

void do_track(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;
        int chance, dir;
        char arg[MAX_INPUT_LENGTH];
        char buf[MAX_STRING_LENGTH];

        dir = 0;
	chance = get_skill(ch,gsn_track);

        if (chance == 0
        || ch->level < skill_table[gsn_track].skill_level[ch->class])
        {
        send_to_char("You peer intently at the ground but discern nothing.\n\r",ch);
        return;
        }
        one_argument(argument,arg);
        victim = get_char_world(ch,arg);
        chance *= 2;
        chance /= 3;
        if (ch->in_room->sector_type == SECT_INSIDE)
                chance -= 40;
        else if (ch->in_room->sector_type == SECT_CITY)
                chance -= 35;
        else if (ch->in_room->sector_type >= SECT_WATER_SWIM
        && ch->in_room->sector_type < SECT_DESERT)
                chance = 0;
        else if (ch->in_room->sector_type == SECT_FOREST)
                chance += 25;

        if (victim != NULL && !IS_NPC(victim))
                dir = victim->pcdata->track_dir[ch->in_room->vnum];

        if (number_percent() > chance
        || victim == NULL
        || !IS_NPC(victim)
        || dir == 0)
        {
        act("$n peers intently at the ground but doesn't seem to find anything.",ch,0,0,TO_ROOM);
        send_to_char("You peer intently at the ground but uncover nothing new.\n\r",ch);
        check_improve(ch,gsn_track,FALSE,2);
        WAIT_STATE(ch,18);
        return;
        }
        act("$n peers at the ground and seems more enlightened.",ch,0,0,TO_ROOM);

sprintf(buf,"You peer at the surroundings and find that %s left %s last time they were here.\n\r",
victim->name,
(dir == 1) ? "north" : (dir == 2) ? "east" : (dir == 3) ? "south" :
(dir == 4) ? "west" : (dir == 5) ? "up" : "down");
        send_to_char(buf,ch);
        WAIT_STATE(ch,18);
        check_improve(ch,gsn_track,TRUE,2);
        return;
}

void do_shield_cleave(CHAR_DATA *ch,char *argument)
{
	char buf[MAX_STRING_LENGTH];
        char arg[MAX_INPUT_LENGTH];
        int chance;
        OBJ_DATA *weapon;
        OBJ_DATA *shield;
        bool using_primary = TRUE;
  	CHAR_DATA *victim;

        one_argument(argument,arg);
        if (arg[0] == '\0')
                victim = ch->fighting;
        else
                victim = get_char_room(ch,arg);

        chance = get_skill(ch,gsn_shield_cleave);
        if (chance == 0
        || ch->level < skill_table[gsn_shield_cleave].skill_level[ch->class])
        {
        send_to_char("You don't know the methods to cleave a shield in two.\n\r",ch);
        return;
        }
        weapon = get_eq_char(ch,WEAR_WIELD);
        if (weapon == NULL)
        {
        weapon = get_eq_char(ch,WEAR_DUAL_WIELD);
        using_primary = FALSE;
        }
        if (weapon == NULL
        || (weapon->value[0] != WEAPON_SWORD
        && weapon->value[0] != WEAPON_AXE) )
        {
        send_to_char("You must be wielding an axe of sword to shield cleave.\n\r",ch);
        return;
        }
        if (victim == NULL)
        {
        send_to_char("But they aren't here.\n\r",ch);
        return;
        }
    if (victim == ch)
    {
send_to_char("That isn't possible.\n\r",ch);
return;
}
        if ((shield = get_eq_char(victim,WEAR_SHIELD)) == NULL)
        {
        send_to_char("But they aren't using a shield.\n\r",ch);
        return;
        }
        chance *= 9;
        chance /= 10;
        chance += (ch->level - victim->level)*3;
        chance -= shield->level;
        if (!using_primary)
                chance -= 15;
    if (!IS_NPC(victim) && ch->fighting != victim)
    {
sprintf(buf,"Help! %s just shield cleaved me!",PERS(ch,victim));
	do_yell(victim,buf);
    }
        if (number_percent() > chance)
        {
        act("$n makes a might blow at $N's shield but fails to cleave it.",ch,0,victim,TO_NOTVICT);
        act("$n lands a mighty blow to your shield but fails to cleave it.",ch,0,victim,TO_VICT);
        act("You strike a mighty blow to $N's shield but fail to cleave it.",ch,0,victim,TO_CHAR);
        check_improve(ch,gsn_shield_cleave,FALSE,1);
        WAIT_STATE(ch,12);

multi_hit(victim,ch,-1);
        return;
        }
        act("$n's mighty blow cleaves $N's shield in half!",ch,0,victim,TO_NOTVICT);
        act("Your might blow cleaves $N's shield in half!",ch,0,victim,TO_CHAR);
        act("$n strikes your shield with powerful force, cleaving it in two!",ch,0,victim,TO_VICT);
        extract_obj(shield);
        WAIT_STATE(ch,12);
	check_improve(ch,gsn_shield_cleave,TRUE,1);
    multi_hit(victim,ch,-1);

        return;
}

/* more race skills...Ceran */

void do_breath_fire(CHAR_DATA *ch,char *argument)
{
    AFFECT_DATA af;
    CHAR_DATA *victim;
    OBJ_DATA *obj_lose;
    OBJ_DATA *obj_next;
    char arg[MAX_INPUT_LENGTH];
    int dam, chance;
    int hpch, level;

    level = ch->drain_level + ch->level;
    chance = get_skill(ch,gsn_breath_fire);
    if (chance == 0
    || ch->level < skill_table[gsn_breath_fire].skill_level[ch->class])
    {
    send_to_char("You aren't able to breath fire.\n\r",ch);
    return;
    }

    if (is_affected(ch,gsn_breath_fire))
    {
    send_to_char("You haven't yet regained the heat in your body to create more flames.\n\r",ch);
    return;
    }

    one_argument(argument,arg);
    if (arg[0] == '\0')
        victim = ch->fighting;
    else
        victim = get_char_room(ch,arg);

    if (victim == NULL)
    {
    send_to_char("They aren't here.\n\r",ch);
    return;
    }
    if (ch->mana < 20)
    {
    send_to_char("You don't have the mana.\n\r",ch);
    return;
    }
    if (number_percent() > chance)
    {
    send_to_char("You try to breath forth fire but only smokes comes out.\n\r",ch);
    act("$n opens $s mouth but only a thick plume of black smoke comes forth.",ch,0,0,TO_ROOM);
    ch->mana -= 10;
    check_improve(ch,gsn_breath_fire,FALSE,1);
    return;
    }
	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
    af.type = gsn_breath_fire;
    af.location = 0;
    af.modifier = 0;
    af.duration = 4;
    af.bitvector = 0;
    af.level = ch->drain_level + ch->level;
    affect_to_char(ch,&af);

    act("$n breaths forth a cone of searing flames!",ch,0,0,TO_ROOM);
    send_to_char("You breath forth a cone of searing flames!\n\r",ch);

    if ( number_percent( ) < level && !saves_spell( level - 2, victim,DAM_FIRE ) )
    {
	for ( obj_lose = victim->carrying; obj_lose != NULL;
	obj_lose = obj_next )
	{
	    char *msg;

	    obj_next = obj_lose->next_content;
	    if ( number_bits( 2 ) != 0 )
		continue;

	    switch ( obj_lose->item_type )
	    {
	    default:             continue;
	    case ITEM_CONTAINER: msg = "$p ignites and burns!";   break;
	    case ITEM_POTION:    msg = "$p bubbles and boils!";   break;
	    case ITEM_SCROLL:    msg = "$p crackles and burns!";  break;
	    case ITEM_STAFF:     msg = "$p smokes and chars!";    break;
	    case ITEM_WAND:      msg = "$p sparks and sputters!"; break;
	    case ITEM_FOOD:      msg = "$p blackens and crisps!"; break;
	    case ITEM_PILL:      msg = "$p melts and drips!";     break;
	    }

	    act( msg, victim, obj_lose, NULL, TO_CHAR );
	    extract_obj( obj_lose );
	}
    }

    hpch = UMAX( 10, ch->hit );
    dam  = number_range( (hpch/7) + 1, hpch/3 );
    if ( saves_spell( level - 2, victim, DAM_FIRE ) )
	dam /= 2;
    damage_old( ch, victim, dam, gsn_breath_fire, DAM_FIRE,TRUE);
    check_improve(ch,gsn_breath_fire,TRUE,1);
    return;
}


void do_rear_kick(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *victim;
    int dam;
/*
    AFFECT_DATA af;
*/
    if (str_cmp(race_table[ch->race].name,"centaur"))
    {
    send_to_char("You don't have a centaur's rear legs to kick like that.\n\r",ch);
    return;
    }

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_rear_kick].skill_level[ch->class] )
    {
	send_to_char("You do not have the ability to rear kick yet.\n\r",ch );
	return;
    }

    if (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_KICK))
	return;

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You are not fighting anyone.\n\r", ch );
	return;
    }

    WAIT_STATE( ch, 24 );
    act("$n whips about and kicks out with $s hind legs!",ch,0,0,TO_ROOM);
    act("You whip about and kick out with your hind legs!",ch,0,0,TO_CHAR);

    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_rear_kick] )
    {
        dam = number_range(ch->level/2,ch->level);
        dam += get_curr_stat(ch,STAT_STR);
        dam += number_range(1,ch->drain_level + ch->level);
        dam += number_range((ch->drain_level + ch->level)/4, (ch->drain_level + ch->level)/2);

        if (dam < 1)
                dam = 1;
        if (dam > 140)
                dam = 140;

        damage_old( ch, victim, dam, gsn_rear_kick,DAM_BASH, TRUE );
        check_improve(ch,gsn_rear_kick,TRUE,1);
    }
    else
    {
        damage_old( ch, victim, 0, gsn_rear_kick,DAM_BASH,TRUE );
        check_improve(ch,gsn_rear_kick,FALSE,1);
    }

    return;
}


/* Wings of fallen angel..put in multi_hit, right after 1st and 2nd attacks
*/
void wing_beating(CHAR_DATA *ch)
{
        OBJ_DATA *wings;
        CHAR_DATA *vch;
        int sn_windwall;
        int dam;

        if (IS_NPC(ch))
                return;

        if ( (wings = get_eq_char(ch,WEAR_ABOUT) ) == NULL)
		return;
	if (wings->pIndexData->vnum != OBJ_VNUM_WINGS)
        return;

        if (number_percent() > 6)
                return;
act("The shredded wings on $n's back unfold and beat with savage force!",ch,0,0,TO_ROOM);
send_to_char("The shredded wings on your back unfold and beat with savage force!\n\r",ch);
sn_windwall = skill_lookup("windwall");
if (sn_windwall == -1)
        return;

for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
{
        if (vch->fighting == ch && number_percent() < 40)
                break;
}

if (vch == NULL)
        vch = ch->fighting;
if (vch == NULL)
        return;
        dam = dice(45,4);
        if (saves_spell(45,vch, DAM_BASH))
                dam /= 2;
        damage_old(ch,vch,dam, sn_windwall, DAM_BASH,TRUE);
        return;
}

void hammer_pendant(CHAR_DATA *ch)
{
	  OBJ_DATA *hammer;
	  OBJ_DATA *hammer2;

        if (IS_NPC(ch))
                return;
	  if (number_percent() > 40)
                return;
         hammer  = get_eq_char(ch,WEAR_NECK_1);
         hammer2 = get_eq_char(ch,WEAR_NECK_2);
        if (hammer != NULL)
		{
			if (hammer->pIndexData->vnum == 11454) {
			        ch->hit = UMIN( ch->hit + 20, ch->max_hit );
				  act("$n's hammer pendant glows brightly and he looks healthier.",ch,0,0,TO_ROOM);
			        send_to_char( "Your hammer pendant glows brightly and you feel a rush of health!\n\r", ch);
			}
		}
        if (hammer2 != NULL)
		{
			if (hammer2->pIndexData->vnum == 11454) {
			        ch->hit = UMIN( ch->hit + 20, ch->max_hit );
				  act("$n's hammer pendant glows brightly and he looks healthier.",ch,0,0,TO_ROOM);
			        send_to_char( "Your hammer pendant glows brightly and you feel a rush of health!\n\r", ch);
			}
		}


    return;

}

void etched_pendant(CHAR_DATA *ch)
{
	  OBJ_DATA *hammer;
	  OBJ_DATA *hammer2;

        if (IS_NPC(ch))
                return;

        if (number_percent() > 25)
                return;
         hammer  = get_eq_char(ch,WEAR_NECK_1);
         hammer2 = get_eq_char(ch,WEAR_NECK_2);
        if (hammer != NULL)
		{
			if (hammer->pIndexData->vnum == 11453) {
		        ch->hit = UMIN( ch->hit + 40, ch->max_hit );
			  act("$n's etched pendant writhes upon his chest.",ch,0,0,TO_ROOM);
		        send_to_char( "Your etched pendant writhes upon your chest!\n\r", ch);
			}
		}
        if (hammer2 != NULL)
		{
			if (hammer2->pIndexData->vnum == 11453) {
		        ch->hit = UMIN( ch->hit + 40, ch->max_hit );
	  act("$n's etched pendant writhes upon his chest.",ch,0,0,TO_ROOM);
        send_to_char( "Your etched pendant writhes upon your chest!\n\r", ch);
			}
		}
    return;

}

void mer_tattoo(CHAR_DATA *ch) //Merthairu's tattoo, Ceial
{
	OBJ_DATA *brand;
	CHAR_DATA *vch;
	int sn_mert_brand;
	brand = get_eq_char(ch,WEAR_BRAND);
	sn_mert_brand = skill_lookup("ball lightning");
	vch=ch->fighting;

	if (brand == NULL)
	return;
	if (brand->pIndexData->vnum == 68 && number_percent() < 12)
	        {
	        act("A demon enshrouded in flames sends forth a ball of lightning!",ch,0,0,TO_ROOM);
	        act("A demon enshrouded in flames sends forth a ball of lightning at your foes!",ch,0,0,TO_CHAR);
	(*skill_table[sn_mert_brand].spell_fun) (sn_mert_brand, 60,ch,vch,TAR_CHAR_OFFENSIVE);
	        }
	        if (vch->in_room != ch->in_room
	        || vch->ghost > 0)
                return;
}

void deo_tattoo(CHAR_DATA *ch, CHAR_DATA *victim)
{
	OBJ_DATA *brand;

	   	brand = get_eq_char(ch,WEAR_BRAND);

    if (brand != NULL)
	{
    if (brand->pIndexData->vnum == 58)
	{
	if ((number_percent() < .5) && (!IS_NPC(victim)))
	{
		act("{R$n's $p flares violently.{x",ch,brand,NULL,TO_NOTVICT);
		act("{R$n's $p flares violently.{x",ch,brand,NULL,TO_VICT);
		act("{RYour $p flares violently.{x",ch,brand,NULL,TO_CHAR);
		send_to_char("Perhaps you should run away?\n\r",victim);
	}
	if (number_percent() < 1)
	{
		act("{r$n's $p flares brightly, stealing $N's last living breath. {x",ch,brand,victim,TO_NOTVICT);
		act("{r$n's $p flares brightly, stealing your last living breath.{x",ch,brand,victim,TO_VICT);
		act("{rYour $p flares brightly, stealing $N's last living breath. {x",ch,brand,victim,TO_CHAR);
		raw_kill(ch,victim);
	}
	if ((number_percent() < 2.5) && (IS_NPC(victim)))
	{
		act("{R$n's $p emits a fierce glow.{x",ch,brand,NULL,TO_NOTVICT);
		act("{R$n's $p emits a fierce glow.{x",ch,brand,NULL,TO_VICT);
		act("{RYour $p emits a fierce glow.{x",ch,brand,NULL,TO_CHAR);
		send_to_char("Perhaps you should run away?\n\r",victim);
	}
	if (number_percent() < 7)
	{
	act("{R$n's $p pulses with a red luminance!{x",ch,brand,0,TO_ROOM);
	act("{RYour $p pulses with a red luminance!{x",ch,brand,0,TO_CHAR);
	obj_cast_spell(skill_lookup("acid needles"), ch->level, ch, victim, brand);
 	}
	if (number_percent() < 18)
	{
	act("{G$n's $p emits a soft glow!{x",ch,brand,0,TO_ROOM);
	act("{GYour $p emits a soft glow!{x",ch,brand,0,TO_CHAR);
	obj_cast_spell(skill_lookup("rejuvenate"), ch->level, ch, ch, brand);
	}
	}
	}
	return;
}

void gra_tattoo(CHAR_DATA *ch, CHAR_DATA *victim)
{
	OBJ_DATA *brand;

   	brand = get_eq_char(ch,WEAR_BRAND);

    if (brand != NULL)
	{
    if (brand->pIndexData->vnum == 77)
	{
    if (ch->last_fought == victim || (IS_NPC(victim)))
	{
		brand->timer = 0;
	}
	if ((number_percent() < 5) && (brand->timer == 0))
	{
		brand->timer = 10;
		act("{r$n's $p seems to come to life as it glares at $N.{x",ch,brand,victim,TO_NOTVICT);
		act("{r$n's $p seems to come to life as it glares at you.{x",ch,brand,victim,TO_VICT);
		act("{rYour $p seems to come to life as it glares at $N.{x",ch,brand,victim,TO_CHAR);
		send_to_char("A strange feeling of fear suddenly overcomes you...perhaps you should consider running away?\n\r",victim);
	}
	if ((brand->timer == 9))
	{
		brand->timer = 8;
		act("{r$n's $p begins to flare, as an uneasy look comes over $N's face.{x",ch,brand,victim,TO_NOTVICT);
		act("{r$n's $p begins to flare, as you begin to feel uneasy.{x",ch,brand,victim,TO_VICT);
		act("{rYour $p begins to flare, as an uneasy look comes over $N's face.{x",ch,brand,victim,TO_CHAR);
	}
	if ((brand->timer == 7))
	{
		brand->timer = 6;
		act("{r$N gasps for air as blood begins to flow from $S pores.{x",ch,brand,victim,TO_NOTVICT);
		act("{rYou gasp for air as blood begins to flow from your pores.{x",ch,brand,victim,TO_VICT);
		act("{r$N gasps for air as blood begins to flow from $S pores.{x",ch,brand,victim,TO_CHAR);
	}
	if ((brand->timer == 5))
	{
		brand->timer = 4;
		act("{r$N's eyes begin to glaze over as the blood trickles down $s face.{x",ch,brand,victim,TO_NOTVICT);
		act("{rYour eyes begin to glaze over as the blood trickles down your face.{x",ch,brand,victim,TO_VICT);
		act("{r$N's eyes begin to glaze over as the blood trickles down $S face.{x",ch,brand,victim,TO_CHAR);
	}
	if ((brand->timer == 3))
	{
		brand->timer = 0;
		act("{rWith a final groping of $S flailing arms, $N collapses to the floor...{RDEAD!!{x",ch,brand,victim,TO_NOTVICT);
		act("{rWith a final groping of your flailing arms, you collapses to the floor...{RDEAD!!{x",ch,brand,victim,TO_VICT);
		act("{rWith a final groping of $S flailing arms, $N collapses to the floor...{RDEAD!!{x",ch,brand,victim,TO_CHAR);
		raw_kill(ch,victim);
	}

	if ((number_percent() < 9) && (brand->timer == 0))
	{
		act("{G$n's $p glows a pale green as your skin begins to burn.{x",ch,brand,victim,TO_VICT);
		act("{G$n's $p glows a pale green.{x",ch,brand,0,TO_ROOM);
		act("{GYour $p glows a pale green as $N's skin begins to burn.{x",ch,brand,victim,TO_CHAR);
		obj_cast_spell(skill_lookup("acid needles"), ch->level, ch, victim, brand);
 	}
	if ((number_percent() < 18) && (brand->timer == 0))
	{
		act("{Y$n's $p glows ever so softly as a look of relief overcomes $s face.{x",ch,brand,0,TO_ROOM);
		act("{YYour $p glows ever so softly.{x",ch,brand,0,TO_CHAR);
		obj_cast_spell(skill_lookup("rejuvenate"), ch->level, ch, ch, brand);
	}
	}
	}
	return;
}

void cei_tattoo(CHAR_DATA *ch) //My tattoo, Ceial
{
	OBJ_DATA *brand;
	int sn_cei_brand;
	brand = get_eq_char(ch,WEAR_BRAND);
	sn_cei_brand = skill_lookup("heal");

	if (brand == NULL)
	return;
	if (brand->pIndexData->vnum == 70 && number_percent() < 15)
		{
			act("$n's brand glows brightly flowing energy into him and healing his wounds.",ch,0,0,TO_ROOM);
			act("$n seems to be filled with a divine energy and his wounds heal.",ch,0,0,TO_CHAR);
			obj_cast_spell(sn_cei_brand,60,ch,ch,brand);
		}
		return;
}

void kra_tattoo(CHAR_DATA *ch,CHAR_DATA *victim )
{
        OBJ_DATA *kratattoo;

        if (number_percent() > 25)
                return;

    	kratattoo  = get_eq_char(ch,WEAR_BRAND);

        if (kratattoo != NULL)
                {
                        if (kratattoo->pIndexData->vnum == 59) {
                          ch->hit = UMIN( ch->hit + 100, ch->max_hit);
 			  act("$n's mark of the dark dragon seems to drain the light around $n momentarily.",ch,0,0,TO_ROOM);
                          send_to_char( "You mark of the dark dragon seems to drain the light around you momentarily!\n\r", ch);
                        }
                }
    return;

}

void kyr_tattoo(CHAR_DATA *ch,CHAR_DATA *victim)
{
	  OBJ_DATA *kyrtattoo;

        if (number_percent() > 4)
                return;

         kyrtattoo = get_eq_char(ch, WEAR_BRAND);

        if (kyrtattoo != NULL)
		{
			if (kyrtattoo->pIndexData->vnum == 60) {
    			  act("$n's crimson caduceus glows dark red.",ch,0,0,TO_VICT);
    			  act("$n's crimson caduceus glows dark red.",ch,0,0,TO_NOTVICT);
   			  act("Your crimson caduceus glows dark red.",ch,0,0,TO_CHAR);
		        ch->hit = UMIN( ch->hit + 75, ch->max_hit );
			}
		}
    return;
}

void xanthr_tattoo(CHAR_DATA *ch, CHAR_DATA *victim)
{

	OBJ_DATA *brand;
	brand = get_eq_char(ch,WEAR_BRAND);

    if (brand != NULL)
        {
        if (brand->pIndexData->vnum == 65)
		{
         if ((number_percent() < 14)) //&& (!IS_NPC(victim)))
         {
	   switch ((number_percent()-1)/20)
	   {
	    case 4:
            {
                act("The small diamond on $n's $p flashes with a brilliant light.",ch,brand,NULL,TO_NOTVICT);
		act("The small diamond on $n's $p burns you with a brilliant light.",ch,brand,NULL,TO_VICT);
                act("The small diamond on your $p flashes with a brilliant light.",ch,brand,NULL,TO_CHAR);
                obj_cast_spell(skill_lookup("wrath"), ch->level, ch, victim, brand);
		break;
	    }
            case 3:
	    {
	        act("The ruby over the thumb on $n's $p flares suddenly.",ch,brand,NULL,TO_NOTVICT);
		act("The ruby over the thumb on $n's $p flares at you suddenly.",ch,brand,NULL,TO_VICT);
                act("The ruby over the thumb on your $p flares suddenly.",ch,brand,NULL,TO_CHAR);
                obj_cast_spell(skill_lookup("nova"), ch->level, ch, victim, brand);
                break;
            }
            case 2:
	    {
                act("The small pearl on $n's $p vibrates slightly for a moment.",ch,brand,NULL,TO_NOTVICT);
		act("The small pearl on $n's $p vibrates slightly for a moment.",ch,brand,NULL,TO_VICT);
		act("The small pearl on your $p vibrates slightly for a moment.",ch,brand,NULL,TO_CHAR);
                obj_cast_spell(skill_lookup("sanctuary"), ch->level, ch, ch, brand);
		break;
            }
	   case 1:
	   {
                act("The turquoise stone on $n's $p glows faintly.",ch,brand,NULL,TO_NOTVICT);
		act("The turquoise stone on $n's $p glows faintly.",ch,brand,NULL,TO_VICT);
		act("The turquoise stone on your $p glows faintly.",ch,brand,NULL,TO_CHAR);
                obj_cast_spell(skill_lookup("weaken"), ch->level, ch, victim, brand);
		break;
           }
  	   case 0:
	   {
	        act("The sapphire gem on $n's $p burns with a blue light.",ch,brand,NULL,TO_NOTVICT);
	 	act("The sapphire gem on $n's $p burns with a blue light.",ch,brand,NULL,TO_VICT);
		act("The sapphire gem on your $p burns with a blue light.",ch,brand,NULL,TO_CHAR);
                obj_cast_spell(skill_lookup("heal"), ch->level, ch, ch, brand);
		break;
 	  }
          default: break;
	 }
	}
       }
      }
}

void do_strap(CHAR_DATA *victim,OBJ_DATA *strap) {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	if (strap->pIndexData->vnum == 5750) {
	act("\x01B[1;31m$n's $p explodes in a blinding flash of light!\x01B[0;37m",victim,strap,0,TO_ROOM);
	act("\x01B[1;31mYour $p explodes in a blinding flash of light!\x01B[0;37m",victim,strap,0,TO_CHAR);
	for (vch = victim->in_room->people; vch != NULL; vch = vch_next)
	{
	vch_next = vch->next_in_room;
	if (vch == victim) {
		continue;
	}
	if (IS_NPC(vch)) {
		continue;
	}
	if (vch->level > LEVEL_HERO) {
		continue;
	}
	act("$n detonates in an eruption of blood and fluids!",vch,strap,0,TO_ROOM);
	act("You detonate in an eruption of blood and fluids!",vch,strap,0,TO_CHAR);
	act( "$n is DEAD!!", vch, 0, 0, TO_ROOM );
	send_to_char( "You have been KILLED!!\n\r\n\r", vch );
	raw_kill(victim,vch);
	}
	unequip_char(victim,strap);
	obj_from_char(strap);
	extract_obj(strap);
	}
	if (strap->pIndexData->vnum == 5751) {
	unequip_char(victim,strap);
	obj_from_char(strap);
	extract_obj(strap);
	}
	return;
}

void dev_tattoo(CHAR_DATA *ch, CHAR_DATA *victim)
{
//	OBJ_DATA *devtattoo;
//	CHAR_DATA *templar;
//	AFFECT_DATA af;
//	CHAR_DATA *vch;
	if (number_percent() > 5)
		return;
/*	devtattoo = get_eq_char(ch, WEAR_BRAND);
	if(victim!=NULL && IS_NPC(victim) && IS_EVIL(victim) && devtattoo!=NULL && devtattoo->pIndexData->vnum == 54) {
	      for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
        	{
            	if (IS_NPC(vch) && vch->pIndexData->vnum == MOB_VNUM_TEMPLAR)
                  	  break;
	      }
      	if (vch != NULL)
	           	return FALSE;
    		act("The tattoo of a valiant knight on $n blazes with a white light and a golden figure steps forth.",ch,0,0,TO_ROOM);
   		act("Your tattoo of a valiant knight blazes with a white light and a golden figure steps forth.",ch,0,0,TO_CHAR);
	      templar = create_mobile( get_mob_index(MOB_VNUM_TEMPLAR) );
		char_to_room(templar,ch->in_room);
		SET_BIT(templar->affected_by, AFF_CHARM);
		templar->master = templar->leader = ch;
	      multi_hit( templar, victim, TYPE_UNDEFINED );
	}*/
}
bool check_crown_blood(CHAR_DATA *ch, CHAR_DATA *victim, int dt, int dam, int dam_type)
{
        OBJ_DATA *obj;
        char buf1[MAX_STRING_LENGTH];
        char buf2[MAX_STRING_LENGTH];
        char buf3[MAX_STRING_LENGTH];
	int mod;
	char *attack;
	mod = 0;
 	if (dam_type != DAM_COLD && dam_type != DAM_NEGATIVE)
	return FALSE;
        obj = get_eq_char(victim,WEAR_HEAD);
        if (obj == NULL)
                return FALSE;
        if (obj->pIndexData->vnum != OBJ_VNUM_CROWN_BLOOD)
                return FALSE;

if (!saves_spell(ch->level,victim,DAM_COLD))
        return FALSE;

        if ( dt >= 0 && dt < MAX_SKILL )
            attack      = skill_table[dt].noun_damage;
        else if ( dt >= TYPE_HIT
        && dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
            attack      = attack_table[dt - TYPE_HIT].noun;
        else
        {
            bug( "Dam_message: bad dt %d.", dt );
            dt  = TYPE_HIT;
            attack  = attack_table[0].name;
        }

sprintf(buf1,"$N's crown glows and %s is unaffected by $n's %s!",
(victim->sex == 1) ? "he" : (ch->sex == 2) ? "she" : "it",
attack);
sprintf(buf2,"Your crown glows and you are unaffected by $n's %s!",
attack);
sprintf(buf3,"$N's crown glows blue and %s is unaffected by your %s!",
(victim->sex == 1) ? "he" : (ch->sex == 2) ? "she" : "it",
attack);

act(buf1,ch,0,victim,TO_NOTVICT);
act(buf2,ch,0,victim,TO_VICT);
act(buf3,ch,0,victim,TO_CHAR);

if (IS_NPC(victim))
        return TRUE;

if (dam < 30)
        mod = -5;
else if (dam < 50)
        mod = -4;
else if (dam < 80)
        mod = -2;
else if (dam < 120)
        mod = -1;
else if (dam < 180)
        mod = 0;
else if (dam < 350)
        mod = 1;
else if (dam < 450)
        mod = 2;
else if (dam < 600)
        mod = 3;
else if (dam < 750)
        mod = 4;
else
        mod += 6;

if (saves_spell(51 + mod, victim, DAM_OTHER))
        return TRUE;
if (saves_spell(51 + mod,victim,DAM_OTHER))
        return TRUE;

act("$n's crown of frozen blood shudders and explodes!",ch,0,0,TO_ROOM);
send_to_char("Your crown of frozen blood shudders and explodes!\n\r",ch);
extract_obj(obj);

return TRUE;
}


/* ioun stone..blue, green and red
blue == for any magic dam
green == for any holy||negative
red == for any weapon
Add stone timers in so they decay after 999 ticks anyway. (~8 hrs)
All limit 2

bool check_blue_ioun(CHAR_DATA *ch,CHAR_DATA *victim,int dt, int dam, int dam_type)
{
        OBJ_DATA *obj;
        char bu1f[MAX_STRING_LENGTH];
        char bu2f[MAX_STRING_LENGTH];
        char bu3f[MAX_STRING_LENGTH];
        char *dam;

        if (dam_type != DAM_HOLY
        && dam_type != DAM_FIRE
        && dam_type != DAM_NEGATIVE
        && dam_type != DAM_COLD
        && dam_type != DAM_ACID
        && dam_type != DAM_ENERGY
        && DAM_TYPE != DAM_LIGHT)
                return FALSE;

        obj = get_eq_char(victim,WEAR_HOLD);
        if (obj == NULL)
                return FALSE;
        if (obj->pIndexData->vnum != OBJ_VNUM_BLUE_IOUN)
                return FALSE;

if (!saves_spell(ch->level,victim,DAM_OTHER))
        return FALSE;
if (dt >= TYPE_HIT && dt < (TYPE_HIT + MAX_DAMAGE_MESSAGE) )
        dam = dam_table[dt].noun;
else
        dam = skill_table[dt].dam_message;

sprintf(buf1,"$N's ioun stone glows and %s is unaffected by $n's %s!",
(victim->sex == 1) ? "he" : (ch->sex == 2) ? "she" : "it",dam);
sprintf(buf2,"Your ioun stone glows and you are unaffected by $n's %s!", dam);
sprintf(buf3,"$N's ioun stone glows blue and %s is unaffected by your %s!",
(victim->sex == 1) ? "he" : (ch->sex == 2) ? "she" : "it", dam);

act(buf1,ch,0,victim,TO_NOTVICT);
act(buf2,ch,0,victim,TO_VICT);
act(buf3,ch,0,victim,TO_CHAR);

if (IS_NPC(victim))
        return TRUE;

if (dam < 20)
        mod = -3;
else if (dam < 40)
        mod = -2;
else if (dam < 60)
        mod = -1;
else if (dam < 80)
        mod = 0;
else if (dam < 100)
        mod = 1;
else if (dam < 120)
        mod = 2;
else if (dam < 150)
        mod = 3;
else if (dam < 200)
        mod = 4;
else if (dam < 400)
        mod = 6;
else
        mod = 8;

chance = 50 + (51 + mod - victim->level) * 5;
chance -= victim->saving_spell;

if (number_percent() > chance)
        return TRUE;

act("$n's blue ioun stone shudders and explodes!",ch,0,0,TO_ROOM);
send_to_char("Your blue ioun stone shudders and explodes!",ch,0,0,TO_CHAR);
extract_obj(obj);

return TRUE;
}

bool check_green_ioun(CHAR_DATA *ch,CHAR_DATA *victim,int dt, int dam, int dam_type)
{
        OBJ_DATA *obj;
        char bu1f[MAX_STRING_LENGTH];
        char bu2f[MAX_STRING_LENGTH];
        char bu3f[MAX_STRING_LENGTH];
        char *dam;

        if (dam_type != DAM_HOLY
        && dam_type != DAM_NEGATIVE
                return FALSE;

        obj = get_eq_char(victim,WEAR_HOLD);
        if (obj == NULL)
                return FALSE;
        if (obj->pIndexData->vnum != OBJ_VNUM_GREEN_IOUN)
                return FALSE;

if (!saves_spell(ch->level,victim,DAM_OTHER))
        return FALSE;

if (dt >= TYPE_HIT && dt < (TYPE_HIT + MAX_DAMAGE_MESSAGE) )
        dam = dam_table[dt].noun;
else
        dam = skill_table[dt].dam_message;

sprintf(buf1,"$N's ioun stone glows and %s is unaffected by $n's %s!",
(victim->sex == 1) ? "he" : (ch->sex == 2) ? "she" : "it",dam);
sprintf(buf2,"Your ioun stone glows and you are unaffected by $n's %s!",dam);
sprintf(buf3,"$N's ioun stone glows blue and %s is unaffected by your %s!",
(victim->sex == 1) ? "he" : (ch->sex == 2) ? "she" : "it",dam);

act(buf1,ch,0,victim,TO_NOTVICT);
act(buf2,ch,0,victim,TO_VICT);
act(buf3,ch,0,victim,TO_CHAR);

if (IS_NPC(victim))
        return TRUE;

if (dam < 20)
        mod = -3;
else if (dam < 40)
        mod = -2;
else if (dam < 60)
        mod = -1;
else if (dam < 80)
        mod = 0;
else if (dam < 100)
        mod = 1;
else if (dam < 120)
        mod = 2;
else if (dam < 150)
        mod = 3;
else if (dam < 200)
        mod = 4;
else if (dam < 400)
        mod = 6;
else
        mod += 8;


chance = 50 + (45 + mod - victim->level) * 5;
chance -= victim->saving_spell;

if (number_percent() > chance)
        return TRUE;
if (number_percent() > chance)
        return TRUE;

act("$n's green ioun stone shudders and explodes!",ch,0,0,TO_ROOM);
send_to_char("Your green ioun stone shudders and explodes!",ch,0,0,TO_CHAR);
extract_obj(obj);

return TRUE;
}

bool check_red_ioun(CHAR_DATA *ch,CHAR_DATA *victim,int dt, int dam, int dam_type)
{
        OBJ_DATA *obj;
        char bu1f[MAX_STRING_LENGTH];
        char bu2f[MAX_STRING_LENGTH];
        char bu3f[MAX_STRING_LENGTH];
        char *dam;

        if (dam_type != DAM_BASH
        && dam_type != DAM_SLASH
        && dam_type != DAM_PIERCE)
                return FALSE;

        obj = get_eq_char(victim,WEAR_HOLD);
        if (obj == NULL)
                return FALSE;
        if (obj->pIndexData->vnum != OBJ_VNUM_RED_IOUN)
                return FALSE;

if (!saves_spell(ch->level,victim,DAM_OTHER))
        return FALSE;

if (dt >= TYPE_HIT && dt < (TYPE_HIT + MAX_DAMAGE_MESSAGE) )
        dam = dam_table[dt].noun;
else
        dam = skill_table[dt].dam_message;

sprintf(buf1,"$N's ioun stone glows and %s is unaffected by $n's %s!",
(victim->sex == 1) ? "he" : (ch->sex == 2) ? "she" : "it",dam);
sprintf(buf2,"Your ioun stone glows and you are unaffected by $n's %s!",dam);
sprintf(buf3,"$N's ioun stone glows blue and %s is unaffected by your %s!",
(victim->sex == 1) ? "he" : (ch->sex == 2) ? "she" : "it",dam);

act(buf1,ch,0,victim,TO_NOTVICT);
act(buf2,ch,0,victim,TO_VICT);
act(buf3,ch,0,victim,TO_CHAR);

if (IS_NPC(victim))
        return TRUE;

if (dam < 10)
        mod = -3;
else if (dam < 20)
        mod = -2;
else if (dam < 30)
        mod = -1;
else if (dam < 40)
        mod = 0;
else if (dam < 50)
        mod = 1;
else if (dam < 65)
        mod = 2;
else if (dam < 75)
        mod = 3;
else if (dam < 85)
        mod = 4;
else if (dam < 100)
        mod = 6;
else
        mod += 8;

chance = 50 + (47 + mod - victim->level) * 5;
chance -= victim->saving_spell;

if (number_percent() > chance)
        return TRUE;
if (number_percent() > chance)
        return TRUE;

act("$n's red ioun stone shudders and explodes!",ch,0,0,TO_ROOM);
send_to_char("Your red ioun stone shudders and explodes!",ch,0,0,TO_CHAR);
extract_obj(obj);

return TRUE;
}

*/

void check_bracelets(CHAR_DATA *ch)
{
        OBJ_DATA *obj1;
        OBJ_DATA *obj2;
        CHAR_DATA *vch;
        int sn_dispel;
        bool hair_1 = FALSE, hair_2 = FALSE, blood_1 = FALSE, blood_2 = FALSE;

        if (IS_NPC(ch))
                return;

        obj1 = get_eq_char(ch,WEAR_WRIST_L);
        if (obj1 != NULL)
        {
        if (obj1->pIndexData->vnum == OBJ_VNUM_HAIR_BRACELET)
                hair_1 = TRUE;
        else if (obj1->pIndexData->vnum == OBJ_VNUM_BLOOD_BRACELET)
                blood_1 = TRUE;
        }

        obj2 = get_eq_char(ch,WEAR_WRIST_R);
        if (obj2 != NULL)
        {
        if (obj2->pIndexData->vnum == OBJ_VNUM_HAIR_BRACELET)
                hair_2 = TRUE;
        else if (obj2->pIndexData->vnum == OBJ_VNUM_BLOOD_BRACELET)
                blood_2 = TRUE;
        }
        if (!hair_1 && !hair_2 && !blood_1 && !blood_2)
                return;


        if ( (vch = ch->fighting) == NULL)
                return;
        if (!IS_EVIL(vch) && ((hair_1) || (hair_2)) )
                return;
        if (!IS_GOOD(vch) && ((blood_1) || (blood_2)) )
                return;

        if ((blood_1) || (blood_2))
                sn_dispel = skill_lookup("dispel good");
        else
                sn_dispel = skill_lookup("dispel evil");

        if (blood_1 && number_percent() < 12)
        {
        act("The blood on $n's bracelet hisses and steams.",ch,0,0,TO_ROOM);
        act("The blood on your bracelet hisses and steams.",ch,0,0,TO_CHAR);
(*skill_table[sn_dispel].spell_fun) (sn_dispel, 55,ch,vch,TAR_CHAR_OFFENSIVE);
        }
        if (vch->in_room != ch->in_room
        || vch->ghost > 0)
                return;

        if (blood_2 && number_percent() < 10)
        {
        act("The blood on $n's bracelet hisses and steams.",ch,0,0,TO_ROOM);
        act("The blood on your bracelet hisses and steams.",ch,0,0,TO_CHAR);
(*skill_table[sn_dispel].spell_fun) (sn_dispel, 55,ch,vch,TAR_CHAR_OFFENSIVE);
        return;
        }
        if (hair_1 && number_percent() < 12)
        {
        act("The hair on $n's bracelet shimmers softly.",ch,0,0,TO_ROOM);
        act("The hair on your bracelet shimmers softly.",ch,0,0,TO_CHAR);
(*skill_table[sn_dispel].spell_fun) (sn_dispel, 55, ch,vch,TAR_CHAR_OFFENSIVE);
        }

        if (vch->in_room != ch->in_room
        || vch->ghost > 0)
                return;

        if (hair_2 && number_percent() < 10)
        {
        act("The hair on $n's bracelet shimmers softly.",ch,0,0,TO_ROOM);
        act("The hair on your bracelet shimmers softly.",ch,0,0,TO_CHAR);
(*skill_table[sn_dispel].spell_fun) (sn_dispel, 55,ch,vch,TAR_CHAR_OFFENSIVE);
        }
        return;
}



void check_blue_talon(CHAR_DATA *ch, CHAR_DATA *victim)
{
  int sn;
	if (number_percent() < 10)
	{
	 send_to_char("Electricity from the blue talon jolts you violently!\n\r",victim);
	 act("Electricity from the blue talon jolts $n!",victim,NULL,NULL,TO_ROOM);
 	sn = skill_lookup("shocking grasp");
(*skill_table[sn].spell_fun) (sn,ch->level,ch,victim,TAR_CHAR_OFFENSIVE);
	 return;
	}
	return;
}


/* Flesh-cutter code */
/* In check_palm strike:
        if ((is_affected(ch,gsn_mutilated_left_hand)
        || is_affected(ch,gsn_mutilated_right_hand)
        && number_percent() > ch->level )
        {
        send_to_char("The nerves in your hand spasm and you fail the palm strike.\n\r",ch);
        }
*/


int check_cutter(CHAR_DATA *ch,CHAR_DATA *victim)
{
        int cut;
        int part;
        AFFECT_DATA af;
        char buf1[MAX_STRING_LENGTH];
        char buf2[MAX_STRING_LENGTH];
        char buf3[MAX_STRING_LENGTH];
        char *part_name;

        if (number_percent() > 4
        || saves_spell(ch->level - 5,victim,DAM_SLASH))
                return 1;

        if (IS_SET(ch->form,FORM_INTANGIBLE)
        || IS_SET(ch->form,FORM_MIST)
        || IS_SET(ch->form,FORM_SNAKE)
        || IS_SET(ch->form,FORM_INSECT)
        || IS_SET(ch->form,FORM_WORM)
        || IS_SET(ch->form,FORM_BLOB)
        || IS_SET(ch->form,FORM_FISH)
        || IS_SET(ch->form,FORM_BIRD))
                return 1;

        part = number_range(0,11);
    cut = 1;
    part_name = "left hand";
	init_affect(&af);
        af.where = TO_AFFECTS;
	af.aftype = AFT_MALADY;
        af.duration = 3;
        af.bitvector = 0;
        af.level = ch->level;

        switch(part)
        {
        case (0): case (1): case (2):       /* left arm */
        if (is_affected(victim,gsn_mutilated_left_arm)
        || !IS_SET(victim->parts,PART_ARMS))
                        return 1;
        part_name = "left arm";
        af.type = gsn_mutilated_left_arm;
        af.modifier = -3;
        af.location = APPLY_HITROLL;
        affect_to_char(victim,&af);
        af.modifier = -3;
        af.location = APPLY_DAMROLL;
        affect_to_char(victim,&af);
        cut = 2;
        break;
        case (3): case (4): case (5): /* right arm */
        if (is_affected(victim,gsn_mutilated_right_arm)
        || !IS_SET(victim->parts,PART_ARMS))
                        return 1;
        part_name = "right arm";
        af.type = gsn_mutilated_right_arm;
        af.modifier = -3;
        af.location = APPLY_HITROLL;
        affect_to_char(victim,&af);
        af.modifier = -3;
        af.location = APPLY_DAMROLL;
        affect_to_char(victim,&af);
        cut = 2;
        break;
        case (6): case (7):       /* left leg */
        if (is_affected(victim,gsn_mutilated_left_leg)
        || !IS_SET(victim->parts,PART_LEGS))
                        return 1;
        part_name = "left leg";
        af.type = gsn_mutilated_left_leg;
        af.modifier = -2;
        af.location = APPLY_DEX;
        affect_to_char(victim,&af);
        af.modifier = -(number_range(20,50));
        af.location = APPLY_MOVE;
        affect_to_char(victim,&af);
        cut = 2;
        break;
        case (8):  case (9):      /* right leg */
        if (is_affected(victim,gsn_mutilated_right_leg)
        || !IS_SET(victim->parts,PART_LEGS))
                        return 1;
        part_name = "right leg";
        af.type = gsn_mutilated_right_leg;
        af.modifier = -2;
        af.location = APPLY_DEX;
        affect_to_char(victim,&af);
        af.modifier = -(number_range(20,50));
        af.location = APPLY_MOVE;
        affect_to_char(victim,&af);
        cut = 2;
        break;
        case (10):       /* left hand */
if (is_affected(victim,gsn_mutilated_left_hand)
        || !IS_SET(victim->parts,PART_HANDS))
                        return 1;
        part_name = "left hand";
        af.type = gsn_mutilated_left_hand;
        af.modifier = -2;
        af.location = APPLY_HITROLL;
        affect_to_char(victim,&af);
        af.modifier = -2;
        af.location = APPLY_DAMROLL;
        affect_to_char(victim,&af);
        cut = 2;
        break;
        case (11):       /* right hand */
if (is_affected(victim,gsn_mutilated_right_hand)
        || !IS_SET(victim->parts,PART_HANDS))
                        return 1;
        part_name = "right hand";
        af.type = gsn_mutilated_right_hand;
        af.modifier = -2;
        af.location = APPLY_HITROLL;
        affect_to_char(victim,&af);
        af.modifier = -2;
        af.location = APPLY_DAMROLL;
        affect_to_char(victim,&af);
        cut = 2;
        break;
        default:       /* right hand */
if (is_affected(victim,gsn_mutilated_right_hand)
        || !IS_SET(victim->parts,PART_HANDS))
                        return 1;
        part_name = "right hand";
        af.type = gsn_mutilated_right_hand;
        af.modifier = -2;
        af.location = APPLY_HITROLL;
        affect_to_char(victim,&af);
        af.modifier = -2;
        af.location = APPLY_DAMROLL;
        affect_to_char(victim,&af);
        cut = 2;
        break;
        }

        sprintf(buf1,"$n's flesh-cutter cruelly mangles $N's %s!",part_name);
        sprintf(buf2,"Your flesh-cutter cruelly mangles $N's %s!",part_name);
        sprintf(buf3,"$n's flesh-cutter cruelly mangles your %s!",part_name);
        act(buf1,ch,0,victim,TO_NOTVICT);
        act(buf2,ch,0,victim,TO_CHAR);
        act(buf3,ch,0,victim,TO_VICT);
        return cut;
}

bool eye_of_beholder_absorb(CHAR_DATA *ch,CHAR_DATA *victim,int dt,int dam,int dam_type)
{
    int chance;
    OBJ_DATA *eye;

    eye = get_eq_char(victim,WEAR_LIGHT);
    if (eye == NULL)
	return FALSE;
    if (eye->pIndexData->vnum != 14003)
	return FALSE;

    if (dt > TYPE_HIT && dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
        return FALSE;
    if (dam_type == DAM_SLASH  || dam_type == DAM_BASH || dam_type == DAM_PIERCE)
        return FALSE;

    chance = 50 + (victim->level - ch->level) * 5;
    if (dam <= 20)
        chance += 15;
    else if (dam <= 40)
        chance += 10;
    else if (dam <= 60)
        chance += 0;
    else if (dam <= 80)
        chance -= 5;
    else if (dam <= 100)
        chance -= 10;
    else if (dam <= 120)
        chance -= 15;
    else if (dam <= 140)
        chance -= 20;
    else if (dam <= 160)
        chance -= 25;
    else if (dam <= 180)
        chance -= 30;
    else if ( dam <= 200)
        chance -= 35;
    else
        chance -= 40;

    chance += victim->saving_throw/4;

    if (number_percent() < chance)
    {
        act("$n's eye of a beholder nullifies the magic around $m.",victim,0,0,TO_ROOM);
        act("Your eye of a beholder nullifies the magic around you.",victim,0,0,TO_CHAR);
        return TRUE;
    }

    return FALSE;
}


/* dying of old age */

void age_death(CHAR_DATA *ch)
{
    ROOM_INDEX_DATA *pRoomIndex;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    if (IS_NPC(ch))
        return;
    if (IS_GOOD(ch))
        pRoomIndex = get_room_index(ROOM_VNUM_DEATH_GOOD);
    else if (IS_EVIL(ch))
        pRoomIndex = get_room_index(ROOM_VNUM_DEATH_EVIL);
    else
        pRoomIndex = get_room_index(ROOM_VNUM_DEATH_NEUTRAL);
    if (pRoomIndex == NULL)
        pRoomIndex = get_room_index(1212);
    if (pRoomIndex == NULL)
        pRoomIndex = get_room_index(ch->temple);
    if (pRoomIndex == NULL)
        return;

/* For now default to the players recall, or midgaard */
        pRoomIndex = get_room_index(ch->temple);
	if (pRoomIndex == NULL)
		pRoomIndex = get_room_index(3001);

    for (obj = ch->carrying; obj != NULL; obj = obj_next)
    {
        obj_next = obj->next_content;
        extract_obj(obj);
    }
/* All items are poofed at age death */
    stop_fighting(ch,TRUE);
    make_corpse(ch,ch);
    char_from_room(ch);
    char_to_room(ch,pRoomIndex);
    do_look(ch,"auto");
    send_to_char("You have died and become a permenant ghost, awaiting your final departure.\n\r",ch);
    return;
}



void do_forage(CHAR_DATA *ch,char *argument)
{
        OBJ_DATA *berry_1;
        OBJ_DATA *berry_2;
        int chance, found;

    if ( (chance = get_skill(ch,gsn_forage)) == 0
    || (ch->level < skill_table[gsn_forage].skill_level[ch->class]) )
    {
        send_to_char("You aren't able to decide on which plants are edible.\n\r",ch);
	return;
    }
    if (ch->in_room->sector_type != SECT_FOREST)
    {
    send_to_char("You aren't in a suitable forest region where you can apply your plant lore.\n\r",ch);
    return;
    }

    if (chance > 90)
        chance = 90;

    if (number_percent() > chance)
    {
    act("$n messes about in the undergrowth but comes up looking perplexed.",ch,0,0,TO_ROOM);
    send_to_char("You search around but find nothing you can recognise as edible.\n\r",ch);
    check_improve(ch,gsn_forage,FALSE,2);
    WAIT_STATE(ch,12);
    return;
    }

    act("$n messes about in the nearby bushes and comes out with some berries.",ch,0,0,TO_ROOM);
    send_to_char("You search around and find some edible berries in the bushes.\n\r",ch);
    check_improve(ch,gsn_forage,TRUE,2);

    found = number_range(1,3);

    berry_1 = create_object(get_obj_index(OBJ_VNUM_BERRY),1);
    if (berry_1 == NULL)
        return;
    obj_to_char(berry_1,ch);

    if (found >= 2)
    {
        berry_2 = create_object(get_obj_index(OBJ_VNUM_BERRY),1);
        obj_to_char(berry_2,ch);
    }
/*
    if (found >= 3)
    {
        berry_3 = create_object(get_obj_index(OBJ_VNUM_BERRY),1);
        obj_to_char(berry_3,ch);
    }
    if (found >= 4)
    {
        berry_4 = create_object(get_obj_index(OBJ_VNUM_BERRY),1);
        obj_to_char(berry_4,ch);
    }
*/

    return;
}

void do_defend(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];

    one_argument(argument,arg);
        if ( (get_skill(ch,gsn_defend) == 0)
        || (ch->level < skill_table[gsn_defend].skill_level[ch->class]) )
    {
        send_to_char("You aren't able to defend other people.\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
    if (ch->defending == NULL)
    sprintf(buf,"You aren't defending anyone right now.\n\r");
    else
    sprintf(buf,"You are defending %s.\n\r",ch->defending->name);
    send_to_char(buf,ch);
    return;
    }
    victim = get_char_room(ch,arg);
    if (victim == NULL)
    {
    send_to_char("They aren't here.\n\r",ch);
    return;
    }
    if (victim == ch)
    {
    send_to_char("You no longer defend anyone.\n\r",ch);
    ch->defending = NULL;
    return;
    }
    if (IS_NPC(victim))
    {
    send_to_char("They don't need to be defended.\n\r",ch);
    return;
    }
    if (ch->defending != NULL)
    {
        act("You stop defending $N.",ch,0,ch->defending,TO_CHAR);
        act("$n stops defending you.",ch,0,ch->defending,TO_VICT);
    }
    sprintf(buf,"You start defending %s.\n\r",victim->name);

    ch->defending = victim;
    act("$n is now defending you.",ch,0,victim,TO_VICT);
    return;
}

void do_intimidate(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;
        char arg[MAX_INPUT_LENGTH];
        int chance;

        chance = get_skill(ch,gsn_intimidate);
        chance *= 8;
        chance /= 10;

        one_argument(argument,arg);
        if (arg[0] == '\0')
                victim = ch->fighting;
        else
                victim = get_char_room(ch,arg);
        if (victim == NULL)
        {
        send_to_char("Trying to intimidate those imaginary friends again?\n\r",ch);
        return;
        }
        if (!IS_NPC(victim))
        {
        send_to_char("Why not just threaten them yourself?\n\r",ch);
        return;
        }
        else if (!IS_SET(victim->act,ACT_AGGRESSIVE)
        && !IS_SET(victim->off_flags,SPAM_MURDER))
        {
        send_to_char("They are pretty passive as it is, why anger them further?\n\r",ch);
        return;
        }
        else if (IS_SET(victim->off_flags,OFF_INTIMIDATED))
        {
        send_to_char("They won't be intimidated any further by anyone.\n\r",ch);
        return;
        }

        chance += (ch->level*3 - victim->level*3);
        if (IS_GOOD(victim))
            chance -= 20;

        if (chance > 70)        chance = 70;
        WAIT_STATE(ch,12);
        if (number_percent() > chance)
        {
        act("$n tries to intimidate $N into submission but just makes a fool of $mself.",ch,0,victim,TO_NOTVICT);
        act("You try to intimidate $N into submission but just make a fool of yourself.",ch,0,victim,TO_CHAR);
        check_improve(ch,gsn_intimidate,FALSE,1);
        return;
        }
        act("$n stares down $N with $s overwhelming prescence.",ch,0,victim,TO_NOTVICT);
        act("You stare down $N with your overwhelming prescence.",ch,0,victim,TO_CHAR);

        check_improve(ch,gsn_intimidate,TRUE,1);
        stop_fighting(victim,TRUE);
        victim->last_fought = NULL;
        SET_BIT(victim->off_flags,OFF_INTIMIDATED);

        return;
}


/* New flee...this allows 'flee <direction>' for the thief skill escape.
-Ceran
*/
void do_flee( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    int attempt, chance, dir;

	if ( ( victim = ch->fighting ) == NULL )
    {
        if ( ch->position == POS_FIGHTING )
            ch->position = POS_STANDING;
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    if (is_affected(ch,gsn_bloody_shackles))
	{
	check_bloody(ch);
	}

    if (IS_ROOM_AFFECTED(ch->in_room,AFF_ROOM_WALL_THORNS))
	{
	check_wall_thorns(ch);
	}

    if (is_affected(ch,gsn_aristaeia))
    {
	send_to_char("You are too filled with righteous wrath to flee!\n\r",ch);
	check_parting_blow(ch,victim);
	return;
    }

    if (is_affected(ch,gsn_shadowstrike) && number_percent()<20)
    {
		act("$n tries to flee but the shadowy arms stop his movements.",ch,0,0,TO_ROOM);
		act("You try to flee but the shadow arms block you.",ch,0,0,TO_CHAR);
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
		return;
	}

    if (is_affected(ch,gsn_shackles) && number_percent()<20)
    {
		act("$n tries to flee but trips over his shackles.",ch,0,0,TO_ROOM);
		act("You try to flee but trip over your shackles.",ch,0,0,TO_CHAR);
		WAIT_STATE(ch,PULSE_VIOLENCE);
		check_cheap_shot(ch->fighting,ch);
		return;
	}
    if (is_affected(ch,gsn_bloodthirst) && number_percent()<20 )
    {
      send_to_char("You gasp for breath as you attempt to snap out of your insanity!\n\r",ch);
      act("$n gasps for breath, attempting to snap out of $s insanity!",ch,NULL,NULL,TO_ROOM);
      WAIT_STATE(ch,PULSE_VIOLENCE);
    }
    if (is_affected(ch,gsn_bind) )
    {
	act("$n attempts to flee but his bindings cause him to trip up and fall!",ch,0,0,TO_ROOM);
	act("You attempt to flee but your bindings cause you to trip up and fall!",ch,0,0,TO_CHAR);
	WAIT_STATE(ch,PULSE_VIOLENCE);
	check_cheap_shot(ch->fighting,ch);
	check_parting_blow(ch,victim);
	return;
    }

	if (check_cutoff(ch,victim)) {
		return;
	}

    one_argument(argument,arg);
    chance = get_skill(ch,gsn_escape);
    if (chance > 90) chance=90;

    dir = 10;

    if (arg[0] == '\0')             dir=10;
    else if (!str_cmp(arg,"north")) dir=0;
    else if (!str_cmp(arg,"east"))  dir=1;
    else if (!str_cmp(arg,"south")) dir=2;
    else if (!str_cmp(arg,"west"))  dir=3;
    else if (!str_cmp(arg,"up"))    dir=4;
    else if (!str_cmp(arg,"down"))  dir=5;
    else                            dir=10;

    was_in = ch->in_room;

    if (was_in==NULL)
    {
      bug("In do_flee: ch->in_room is NULL.",0);
      return;
    }

    if (chance>0 && number_percent()>chance && dir!=10)
    {
      send_to_char("Trying to escape...\n\r",ch);
    }

    for ( attempt = 0; attempt < 6; attempt++ )
    {
        EXIT_DATA *pexit;
        int door;

        if (dir>5 || dir<0) door=number_door()%6;
        else door = dir;

        pexit = was_in->exit[door];

        if ( pexit == 0) continue;

        if (pexit->u1.to_room == NULL
            ||   ( IS_SET(pexit->exit_info, EX_CLOSED)
	        && (!IS_AFFECTED(ch,AFF_PASS_DOOR) ))
            ||   number_range(0,ch->daze) != 0
            || ( IS_NPC(ch)
            &&   IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB) ) )
          continue;

	check_parting_blow(ch,victim);

        move_char( ch, door, FALSE );

        if ( ( now_in = ch->in_room ) == was_in )
            continue;

		stop_fighting(ch,TRUE);
		ch->last_fought = NULL;

        ch->in_room = was_in;
        act( "$n has fled!", ch, NULL, NULL, TO_ROOM );

        ch->in_room = now_in;

        if ( !IS_NPC(ch) )
        {
            send_to_char( "You flee from combat!\n\r", ch );

	if (dir != 10 && get_skill(ch,gsn_escape) > 2)
	check_improve(ch,gsn_escape,2,TRUE);
        }

        return;
    }

    send_to_char( "PANIC! You couldn't escape!\n\r", ch );
    return;
}

/* Some more new skills..Ceran */

void do_lash(CHAR_DATA *ch,char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *weapon;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_lash)) == 0
    &&    ch->level < skill_table[gsn_lash].skill_level[ch->class])
    {
        send_to_char("You don't have the skill to lash people's legs.\n\r",ch);
        return;
    }

    if (IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM))
	return;

    weapon = get_eq_char(ch,WEAR_WIELD);
    if (weapon == NULL || (weapon->value[0] != WEAPON_WHIP
    && weapon->value[0] != WEAPON_FLAIL) )
    {
        chance -= 15;
        weapon = get_eq_char(ch,WEAR_DUAL_WIELD);
    }
    if (weapon == NULL)
    {
    send_to_char("You aren't wielding any weapon to lash with.\n\r",ch);
    return;
    }
    if (weapon->value[0] != WEAPON_WHIP && weapon->value[0] != WEAPON_FLAIL)
    {
    send_to_char("You need to be wielding a whip or flail to lash.\n\r",ch);
    return;
    }



    if (arg[0] == '\0')
    {
        victim = ch->fighting;
        if (victim == NULL)
        {
            send_to_char("But you aren't fighting anyone!\n\r",ch);
            return;
        }
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    if (victim->position == POS_SLEEPING
	|| victim->position == POS_RESTING)
    {
        act("$N isn't on $S feet.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (victim == ch)
    {
        send_to_char("You try to lash your feet and look clumsy doing it.\n\r",ch);
        return;
    }

    if (is_safe(ch,victim))
        return;

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
        act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
        return;
    }
/*
    if (IS_AFFECTED(victim,AFF_FLYING)
	&& !is_affected(victim,gsn_earthbind))
    {
        act("$S feet aren't on the ground.",ch,NULL,victim,TO_CHAR);
        return;
    }
*/
    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || is_affected(ch,skill_lookup("haste")))
        chance += 5;
    if (IS_SET(victim->off_flags,OFF_FAST) || is_affected(victim,skill_lookup("haste")))
        chance -= 35;
    chance += get_curr_stat(ch,STAT_DEX)/2;
    chance -= get_curr_stat(victim,STAT_DEX)/2;
    if (IS_AFFECTED(victim,AFF_FLYING))
        chance -= dice(2,5);
    if (!IS_NPC(ch)
	&& (ch->class == CLASS_TRANSMUTER
	|| ch->class == CLASS_NECROMANCER
	|| ch->class == CLASS_ELEMENTALIST) )
		chance -= 10;
    /* CLS - not sure if you are going to make a number_chance() function, but until you do
             i stuck in rnadom */
    if (is_affected(victim,gsn_fireshield) && number_percent()>35)
    {
	   act("$S's shield of flame singes your whip as you attempt to lash him!",ch,NULL,victim,TO_CHAR);
         act("Your shield of flame singes $n's whip as he tries to lash you!",ch,0,victim,TO_VICT);
	   WAIT_STATE(ch,PULSE_VIOLENCE*2);
	   return;
    }
    /* level */
    chance += (ch->level - victim->level)*3;
    chance -= 25;
        if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (victim->fighting == NULL || ch->fighting == NULL))
	{
        sprintf(buf,"Help! %s is lashing me!",PERS(ch,victim));
	do_myell(victim,buf);
	}
        if (number_percent() > chance)
        {
        act("$n lashes at $N's legs but misses.",ch,0,victim,TO_NOTVICT);
        act("$n lashes at your legs but misses.",ch,0,victim,TO_VICT);
        act("You lash at $N's legs but miss.",ch,0,victim,TO_CHAR);
        check_improve(ch,gsn_lash,FALSE,1);
        WAIT_STATE(ch,skill_table[gsn_lash].beats);
        return;
        }
        act("$n lashes $N's legs, sending $M crashing down.",ch,0,victim,TO_NOTVICT);
        act("$n lashes your legs, sending you crashing to the ground.",ch,0,victim,TO_VICT);
        act("You lash $N's legs, sending $M crashing to the ground.",ch,0,victim,TO_CHAR);
        check_improve(ch,gsn_lash,TRUE,1);
        WAIT_STATE(victim,PULSE_VIOLENCE*3);
        WAIT_STATE(ch,PULSE_VIOLENCE*3);
        damage_old(ch,victim,dice(2,7),gsn_lash,DAM_BASH,TRUE);
        victim->position = POS_RESTING;
        return;
}

void do_pugil(CHAR_DATA *ch,char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int chance;
    int dam;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_pugil)) == 0
    ||  IS_NPC(ch)
    ||  (!IS_NPC(ch) && ch->level < skill_table[gsn_pugil].skill_level[ch->class]) )
    {
        send_to_char("You're not trained in the art of pugiling.\n\r",ch);
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
        send_to_char("You can't pugil someone like that.\n\r",ch);
	return;
	}

    if (victim == ch)
    {
        send_to_char("That would be a bit stupid.\n\r", ch);
	return;
    }

   obj = get_eq_char(ch,WEAR_WIELD);
   if (obj == NULL || obj->value[0] != WEAPON_STAFF)
    {
    send_to_char("You must be wielding a staff to pugil.\n\r",ch);
    return;
    }

    chance += (ch->level - victim->level);
    chance = URANGE(5, chance, 90);

    WAIT_STATE(ch,skill_table[gsn_pugil].beats);

    if (number_percent() < chance)
    {
        act("You smash $N with a bone crushing pugil!",ch,NULL,victim,TO_CHAR);
        act("$n smashes you with a bone crushing pugil!",ch,NULL,victim,TO_VICT);
        act("$n pugils $N with a bone crushing pugil!",ch,NULL,victim,TO_NOTVICT);
        check_improve(ch,gsn_pugil,TRUE,1);
	dam = dice(obj->value[1],obj->value[2]);
        dam += (get_skill(ch,gsn_enhanced_damage) * dam/100);
        if (ch->level <= 20)
            {
            dam *= number_range(10,13);
            dam /= 10;
            }
        else if (ch->level <= 25)
            {
            dam *= number_range(11,14);
            dam /= 10;
            }
        else if (ch->level <= 30)
            {
            dam *= number_range(12,15);
            dam /= 10;
            }
        else if (ch->level <= 35)
            {
            dam *= number_range(12,17);
            dam /= 10;
            }
        else if (ch->level <= 40)
            {
            dam *= number_range(13,18);
            dam /= 10;
            }
        else
            {
            dam *= number_range(14,20);
            dam /= 10;
            }

        damage_old(ch,victim,dam,gsn_pugil, attack_table[obj->value[3]].damage, TRUE);
    }
    else
    {
        check_improve(ch,gsn_pugil,FALSE,1);

        damage_old(ch,victim,0,gsn_pugil,DAM_NONE,TRUE);
    }

    return;
}


void do_protection_heat_cold(CHAR_DATA *ch,char *argument)
{
    AFFECT_DATA af;
    int chance;
    chance = get_skill(ch,gsn_protection_heat_cold);

    if (chance == 0
	|| ch->level <skill_table[gsn_protection_heat_cold].skill_level[ch->class])
    {
send_to_char("You don't know how to protect yourself from the elements.\n\r",ch);
	return;
    }

    if (is_affected(ch,gsn_protection_heat_cold))
    {
    send_to_char("You are already protected from the elements.\n\r",ch);
    return;
    }

    if (number_percent() > chance)
    {
	send_to_char("You try to protect yourself from the elements but fail.\n\r",ch);
	check_improve(ch,gsn_protection_heat_cold,FALSE,1);
	return;
    }

	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
    af.type = gsn_protection_heat_cold;
    af.level = ch->level;
    af.modifier = 0;
    af.duration = ch->level;
    af.bitvector = 0;
    af.location = 0;
    affect_to_char(ch,&af);
    send_to_char("You are protected from the elements.\n\r",ch);
	check_improve(ch,gsn_protection_heat_cold,TRUE,1);
    return;
}

void do_undead_drain(CHAR_DATA *ch,char *argument)
{
	CHAR_DATA *vch;
	int sn_drain;
	if (ch->level < 40) return;

	for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
	{
	if (vch->fighting == ch
	&& number_percent() > 70)
			break;
	}

if (vch == NULL)
		vch = ch->fighting;
	sn_drain = skill_lookup("undead drain");
	if (sn_drain == -1)
	return;

	(*skill_table[sn_drain].spell_fun) (sn_drain,ch->level,ch,vch,TAR_CHAR_OFFENSIVE);
	return;
}

char * check_evaluation_fight(CHAR_DATA *ch,CHAR_DATA *victim)
{
    int percent;
    char wound[100];
    char buf[MAX_STRING_LENGTH];

    if ((victim = ch->fighting) != NULL && can_see(ch,victim))
    {
        if (victim->max_hit > 0)
            percent = victim->hit * 100 / victim->max_hit;
        else
            percent = -1;

        if (percent >= 100)
            sprintf(wound,"is in perfect condition.");
        else if (percent >= 95)
            sprintf(wound,"has a few scratches.");
        else if (percent >= 90)
            sprintf(wound,"has a few bruises.");
        else if (percent >= 80)
             sprintf(wound,"has some small wounds.");
        else if (percent >= 70)
            sprintf(wound,"has some big nasty cuts.");
        else if (percent >= 60)
            sprintf(wound,"has quite a few wounds.");
        else if (percent >= 50)
            sprintf(wound,"is bleeding profusely.");
        else if (percent >= 40)
            sprintf(wound,"is gushing blood.");
        else if (percent >= 30)
            sprintf(wound,"is screaming in pain.");
        else if (percent >= 20)
            sprintf(wound,"is spasming in shock.");
        else if (percent >= 10)
            sprintf(wound,"is writhing on the ground.");
        else if (percent >= 1)
            sprintf(wound,"is convulsing near death.");
        else
            sprintf(wound,"is nearly dead.");
	}
	return str_dup(buf);
}

void do_iron_resolve(CHAR_DATA *ch,char *argument)
{
    AFFECT_DATA af;

    if (is_affected(ch,gsn_iron_resolve))
    {
        send_to_char("Your mind is already firmly set on your inner being.\n\r",ch);
        return;
    }
    if (ch->mana < 100)
    {
        send_to_char("You don't have the mental concentration right now.\n\r",ch);
        return;
    }

    if (number_percent() > get_skill(ch,gsn_iron_resolve))
    {
        send_to_char("You try to concentrate on your inner being but fail to maintain it.\n\r",ch);
        ch->mana -= 50;
        WAIT_STATE(ch,12);
        return;
    }

    send_to_char("You feel yourself come together as mind and body.\n\r",ch);

	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
    af.type = gsn_iron_resolve;
    af.level = ch->level;
    af.location = 0;
    af.bitvector = 0;
    af.modifier = 0;
    af.duration = ch->level/3;
    ch->mana -= 100;
    WAIT_STATE(ch,12);
}

void do_quiet_movement(CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA af;
    int chance;

    chance = get_skill(ch,gsn_quiet_movement);
    if (chance == 0
    || ch->level < skill_table[gsn_quiet_movement].skill_level[ch->class])
    {
        send_to_char("You don't know how to move with silent stealth through the wilderness.\n\r",ch);
        return;
    }

    if (is_affected(ch,gsn_quiet_movement))
    {
        send_to_char("You are already attempting to move silently through the wilderness.\n\r",ch);
        return;
    }
    if (ch->in_room->sector_type == SECT_FOREST)
    {
        send_to_char("You attempt to move quietly through the forest.\n\r",ch);
    }
    else if (ch->in_room->sector_type == SECT_MOUNTAIN)
    {
        send_to_char("You attempt to move quietly through the mountains.\n\r",ch);
    }
    else if (ch->in_room->sector_type == SECT_HILLS)
    {
        send_to_char("You attempt to move quietly through the hills.\n\r",ch);
    }
    else
    {
        send_to_char("You aren't in the right kind of wilderness to mvoe quietly.\n\r",ch);
        return;
    }

    if (number_percent() > chance)
    {
        check_improve(ch,gsn_quiet_movement,FALSE,1);
        return;
    }
    check_improve(ch,gsn_quiet_movement,TRUE,1);

	init_affect(&af);
    af.where = TO_AFFECTS;
    af.type = gsn_quiet_movement;
	af.aftype = AFT_SKILL;
    af.location = 0;
    af.modifier = 0;
    af.duration = ch->level;
    af.level = ch->level;
    af.bitvector = 0;
    affect_to_char(ch,&af);
    return;
}

void do_executioners_grace(CHAR_DATA *ch,char *argument)
{
	AFFECT_DATA af;
	int chance;
	chance = get_skill(ch,gsn_executioner);
	if(chance==0)
	{
		send_to_char("You don't know how to move with the deadly grace of an executioner.\n\r",ch);
		return;
	}
	if(number_percent() > chance)
	{
		send_to_char("You fail to move with the deadly grace of an executioner.\n\r",ch);
		WAIT_STATE(ch,PULSE_VIOLENCE*4);
		return;
	}
	if(is_affected(ch,gsn_executioner))
	{
		send_to_char("You are already moving with the deadly grace of an executioner.\n\r",ch);
		WAIT_STATE(ch,PULSE_VIOLENCE*4);
		return;
	}
	send_to_char("You begin to flow with the deadly grace of an executioner.\n\r",ch);
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = gsn_executioner;
	af.location = APPLY_DAMROLL;
	af.modifier = 20;
	af.level = 60;
	af.duration = 50;
	af.bitvector = 0;
	af.aftype = AFT_SKILL;
	ch->mana-=300;
	affect_to_char(ch,&af);
	return;
}
void do_spike(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;
        int chance, dam;
        char arg[MAX_INPUT_LENGTH];

        one_argument(argument,arg);
        if (IS_NPC(ch))
                return;
	if (ch->position == POS_FIGHTING)
	{
        send_to_char("You can't spike them in the heat of battle.\n\r",ch);
        return;
        }


	if ((get_skill(ch,gsn_spike) == 0)
	|| ch->level < skill_table[gsn_spike].skill_level[ch->class])
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}

        if (arg[0] == '\0')
        {
        send_to_char("Attempt to spike which person?\n\r",ch);
        return;
        }
        if ( (victim = get_char_room(ch,arg)) == NULL)
        {
        send_to_char("They aren't here.\n\r",ch);
        return;
        }
        if (victim == ch)
        {
        send_to_char("You can't do that.\n\r",ch);
        return;
        }
        if (victim->fighting != NULL || victim->position == POS_FIGHTING)
        {
        send_to_char("They are moving around too much to get in close for the kill.\n\r",ch);
        return;
        }

        if (is_safe(ch,victim))
                return;

        chance = get_skill(ch,gsn_spike)/2;
        chance += ch->level;
        chance -= victim->level * 3/2;
        chance -= number_range(0,15);
        if (!can_see(victim,ch))
                chance += 10;
        if (victim->position == POS_FIGHTING)
                chance -= 25;
        else if (victim->position == POS_SLEEPING)
                chance += 10;
        else chance -= 10;

        chance /= 2;
        chance = URANGE(2,chance,90);

        act("$n strikes out at $N with deadly intensity.",ch,0,victim,TO_NOTVICT);
        act("You strike out at $N with deadly intensity.",ch,0,victim,TO_CHAR);
        act("$n strikes at you with deadly intensity.",ch,0,victim,TO_VICT);

        if (number_percent() < chance)
        {
        send_to_char("With agonising pain your skull is smashed by the blow!\n\r",victim);
        act("Your blow shatters $N's skull into bloody fragments!",ch,0,victim,TO_CHAR);
        act("$N's skull is shattered into bits of mangled flesh and bone by $n's strike!",ch,0,victim,TO_NOTVICT);
        raw_kill(ch,victim);
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
        check_improve(ch,gsn_spike,TRUE,2);
        return;
        }
        else
        {
        send_to_char("You feel a sharp pain searing your skull!\n\r",victim);
        act("Your deathstrike smashes $N's skull but fails to kill.",ch,0,victim,TO_CHAR);
        dam = ch->level * 2;
        dam += dice(ch->level, 4);
        damage_old(ch,victim,dam,gsn_spike,DAM_PIERCE,TRUE);
        WAIT_STATE(ch,PULSE_VIOLENCE);
	check_improve(ch,gsn_spike,FALSE,1);
	}

	return;
}


bool check_unholy_bless(CHAR_DATA *ch, CHAR_DATA *victim) {

	AFFECT_DATA af;
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;
	// Some reasons it is not working well.
        //int sn, hitmod, manamod, hrollmod, drollmod, vdrollmod, i;
        // Now this should be working.
        int sn, hitmod = 0, manamod = 0, hrollmod = 0, drollmod = 0, vdrollmod = 0, i;
	if (IS_NPC(victim) && (victim->pIndexData->vnum != MOB_VNUM_PLRMOB) ) {
		return FALSE;
	}
	if (!is_affected(ch,gsn_unholy_bless)) {
		return FALSE;
	}
	if (is_affected(victim,gsn_unholy_timer)) {
		return FALSE;
	}
	/*if (!IS_NPC(victim) && ((victim->played + current_time - victim->logon) / 3600) < 3) {
		return FALSE;
	}*/

	if (isNewbie(victim)) {
		return FALSE;
	}

	sn = skill_lookup("unholy bless");

	/* check if victim is AP and take his charges */
	//if (is_affected(victim,gsn_unholy_bless)) {
        if (is_affected(victim,gsn_unholy_bless) && victim != ch) {
		for ( paf = victim->affected; paf != NULL; paf = paf_next )
		{
			paf_next = paf->next;
			if ( paf->type == sn ) {
				switch (paf->location)
				{
				default: break;
				case APPLY_DAMROLL: vdrollmod = paf->modifier; break;
				}
			}
		}
		vdrollmod = (vdrollmod - 1) / 6;
		if (vdrollmod < 1) {
			vdrollmod = 0;
		}
	} else {
		vdrollmod = 0;
	}

	for (i=0; i < vdrollmod+1; i++) {
	for ( paf = ch->affected; paf != NULL; paf = paf_next )
	{
		paf_next = paf->next;
		if ( paf->type == sn ) {
			switch (paf->location)
			{
			default: break;
			case APPLY_HIT: hitmod = paf->modifier; affect_remove( ch, paf ); break;
			case APPLY_MANA: manamod = paf->modifier; affect_remove( ch, paf ); break;
			case APPLY_HITROLL: hrollmod = paf->modifier; affect_remove( ch, paf ); break;
			case APPLY_DAMROLL: drollmod = paf->modifier; affect_remove( ch, paf ); break;
			}
		}
	}

	hitmod += 10;
	manamod += 10;
	hrollmod += 3;
	drollmod += 3;

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = ch->level;
	af.duration = -1;
	af.bitvector = 0;
	af.modifier = hitmod;
	af.location = APPLY_HIT;
	affect_to_char(ch,&af);
	af.modifier = manamod;
	af.location = APPLY_MANA;
	affect_to_char(ch,&af);
	af.modifier = hrollmod;
	af.location = APPLY_HITROLL;
	affect_to_char(ch,&af);
	af.modifier = drollmod;
	af.location = APPLY_DAMROLL;
	affect_to_char(ch,&af);
	act("\x01B[1;31m$n's body pulses as power drains from $N's corpse!\x01B[0;37m",ch,0,victim,TO_NOTVICT);
	act("\x01B[1;31m$n's body pulses as power drains from your corpse!\x01B[0;37m",ch,0,victim,TO_VICT);
	act("\x01B[1;31mYour body pulses as you drain power from $N's corpse!\x01B[0;37m",ch,0,victim,TO_CHAR);
	}
	return TRUE;
}
void do_bind(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;
        int chance;
	  AFFECT_DATA af;
        char arg[MAX_INPUT_LENGTH];
        one_argument(argument,arg);
        if (IS_NPC(ch))
                return;
	if (get_skill(ch,gsn_bind) == 0)
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}

        if (arg[0] == '\0')
        {
        send_to_char("Attempt to bind who?\n\r",ch);
        return;
        }
        if ( (victim = get_char_room(ch,arg)) == NULL)
        {
        send_to_char("They aren't here.\n\r",ch);
        return;
        }
        if (victim == ch)
        {
        send_to_char("You can't do that.\n\r",ch);
        return;
        }
        if (victim->position != POS_SLEEPING)
        {
        send_to_char("They must be sleeping for you to bind them.\n\r",ch);
        return;
        }

	  if (is_affected(victim,gsn_bind))
	  {
	  send_to_char("They are already bound.\n\r",ch);
	  return;
	  }
	chance = get_skill(ch,gsn_bind)-20;
	if(is_affected(victim,AFF_HASTE))
		chance-=30;
	if(is_affected(ch,AFF_HASTE))
		chance+=15;
	if(get_curr_stat(victim,STAT_DEX)>get_curr_stat(ch,STAT_DEX))
		chance-=20;
	if(number_percent() < chance)
	{
		af.aftype = AFT_SKILL;
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.type = gsn_bind;
		af.level = ch->level;
		af.duration = -1;
		af.bitvector = AFF_BLIND;
		af.modifier = -5;
		af.location = APPLY_DEX;
		affect_to_char(victim,&af);
		act("$n puts a sack over $N's head and ties a rope around his legs tightly.",ch,0,victim,TO_NOTVICT);
		act("You feel someone putting something over your head and legs.",ch,0,victim,TO_VICT);
		act("You put a sack over $N's head and tie a rope around his legs tightly.",ch,0,victim,TO_CHAR);
		WAIT_STATE(ch,PULSE_VIOLENCE);
		check_improve(ch,gsn_bind,TRUE,1);
	}
	else
	{

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_INVIS;
		af.type = gsn_bind;
		af.level = ch->level;
		af.duration = 3;
		af.bitvector = 0;
		af.modifier = 0;
		af.location = APPLY_HITROLL;

		affect_to_char(victim,&af);
		act("$n tries to put a sack over $N's head but it rips.",ch,0,victim,TO_NOTVICT);
		send_to_char("You feel someone trying to put something over your head and legs.",victim);
		act("You try to put a sack over $N's head but it rips.",ch,0,victim,TO_CHAR);
		WAIT_STATE(ch,PULSE_VIOLENCE*3);
		check_improve(ch,gsn_bind,FALSE,3);
	}
	return;
}
void check_cheap_shot(CHAR_DATA *ch, CHAR_DATA *victim)
{
	int chance;
	int dam;
	chance=get_skill(ch,gsn_cheap_shot);
	if(chance<2)
		return;
	if(is_affected(victim,AFF_HASTE))
		chance-=30;
	if(is_affected(ch,AFF_HASTE))
		chance+=15;
	if(is_affected(victim,AFF_FLYING))
		chance-=20;
	chance*=.75;
	if(number_percent() < chance)
	{
		act("Seizing upon $N's moment of weakness, you brutally kick him while he's down!",ch,0,victim,TO_CHAR);
		act("Seizing upon your moment of weakness, $n brutally kicks you while you're down!",ch,0,victim,TO_VICT);
		act("Seizing upon $N's moment of weakness, $n brutally kicks him while he's down!",ch,0,victim,TO_NOTVICT);
		dam=40;
		if(number_percent() < 26)
		{
			act("$N grunts in pain as you land a particularly vicious kick!",ch,0,victim,TO_CHAR);
			act("You grunt in pain as $n lands a particularly vicious kick!",ch,0,victim,TO_VICT);
			act("$N grunts in pain as $n lands a particularly vicious kick!",ch,0,victim,TO_NOTVICT);
			dam=125;
			WAIT_STATE(victim,PULSE_VIOLENCE);
		}
		damage_old(ch,victim,dam,gsn_cheap_shot,DAM_PIERCE,TRUE);
		check_improve(ch,gsn_cheap_shot,TRUE,1);
		WAIT_STATE(victim,PULSE_VIOLENCE);
		return;
	}
	else
	{
		send_to_char("You were unable to get a cheap shot in.\n\r",ch);
		check_improve(ch,gsn_cheap_shot,FALSE,1);
		return;
	}
}
void do_unbind(CHAR_DATA *ch,char *argument)
{
	if(!is_affected(ch,gsn_bind))
	{
		send_to_char("You are not bound.\n\r",ch);
		return;
	}
	if(number_percent()>30)
	{
		act("You break free of the bindings on your head and legs.",ch,0,0,TO_CHAR);
		act("$n breaks free of the bindings on $s head and legs.",ch,0,0,TO_ROOM);
		affect_strip( ch, gsn_bind );
		WAIT_STATE(ch,PULSE_VIOLENCE);
	}
	else
	{
		act("You fail to break free of the bindings on your head and legs.",ch,0,0,TO_CHAR);
		act("$n fails to break free of the bindings on $s head and legs.",ch,0,0,TO_ROOM);
		WAIT_STATE(ch,PULSE_VIOLENCE);
	}
}

void do_blitz( CHAR_DATA *ch, char *argument ) // Do not disable this! -Det.
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	int chance, imod;
	char buf[MAX_STRING_LENGTH];
	bool isDS;

	if (cabal_down(ch,CABAL_RAVAGER))
		return;

	one_argument(argument,arg);

	if ((chance = get_skill(ch,gsn_blitz)) == 0)
	{
		send_to_char("Blitzing? What's that?\n\r",ch);
		return;
	}

	if (IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM))
	return;

	if (arg[0] == '\0')
	{
		victim = ch->fighting;
		if (victim == NULL)
		{
			send_to_char("But you aren't fighting anyone!\n\r",ch);
			return;
		}
	}
	else if ((victim = get_char_room(ch,arg)) == NULL)
	{
		send_to_char("They aren't here.\n\r",ch);
		return;
	}

	if (victim->position < POS_FIGHTING)
	{
		act("You'll have to let $M get back up first.",ch,NULL,victim,TO_CHAR);
		return;
	}

	if (victim == ch)
	{
		send_to_char("You try to blitz your brains out, but fail.\n\r",ch);
		return;
	}
	if ((victim->fighting!=ch) && (ch->fighting != NULL))
	{
		send_to_char("You must be directly fighting that person to blitz them.\n\r",ch);
		return;
	}
	if (is_safe(ch,victim))
	return;

	if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
	{
		act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
		return;
	}

	/* modify chance */
	chance += ch->carry_weight / 200;
	chance -= victim->carry_weight / 200;
	if (ch->size < victim->size)
	{
		chance += (ch->size - victim->size) * 5;
	} else {
		chance += (ch->size - victim->size) * 5;
	}
	if (is_affected(victim,gsn_protective_shield))
	{
		chance -= 5;
	}
	if (is_affected(victim,gsn_wraithform))
	{
		chance -= 5;
	}
	if (is_affected(victim,gsn_kinetic_shield))
	{
		chance -= 5;
	}
	if (is_affected(victim,gsn_fireshield))
	{
		chance -= 5;
	}
	chance += get_curr_stat(ch,STAT_STR) + 5;
	chance -= (get_curr_stat(victim,STAT_DEX));
	chance -= GET_AC(victim,AC_BASH) /20;
    	if (IS_SET(ch->off_flags,OFF_FAST) || is_affected(ch,skill_lookup("haste")))
	{
		chance += 10;
	}
    	if (is_affected(ch,skill_lookup("bloodthirst")))
	{
		chance += 10;
	}
    	if (is_affected(ch,skill_lookup("berserk")))
	{
		chance += 10;
	}
	if (IS_SET(victim->off_flags,OFF_FAST) || is_affected(victim,skill_lookup("haste")))
	{
		chance -= 5;
	}
	chance += (ch->level - victim->level);
	if (!IS_NPC(ch) && !IS_NPC(victim) && (victim->fighting == NULL || ch->fighting == NULL))
	{
		sprintf(buf,"Help! %s is blitzing me!",PERS(ch,victim));
		do_myell(victim,buf);
	}
	if (!IS_NPC(victim) && chance < get_skill(victim,gsn_dodge) )
	{
		chance -= 3 * (get_skill(victim,gsn_dodge) - chance);
	}
	imod = 15;

	isDS = FALSE;

	/* now the attack */
	if (number_percent() < chance - 20)
	{
		act("$n pummels you to the ground with excruciating force!", ch,NULL,victim,TO_VICT);
		act("You slam into $N, pummeling $M into the ground!",ch,NULL,victim,TO_CHAR);
		act("$n pummels $N into the ground with a powerful blitz.",ch,NULL,victim,TO_NOTVICT);
		damage_old(ch,victim,(3 * ch->size + chance),gsn_blitz,DAM_BASH,TRUE);
		check_improve(ch,gsn_blitz,TRUE,3);
		WAIT_STATE(victim,PULSE_VIOLENCE*2);
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
		if (number_percent() < chance - imod)
		{
			if (is_affected(victim,gsn_sanctuary) && (number_percent() < chance-imod))
			{
				if (check_dispel(ch->level,victim,skill_lookup("sanctuary")))
				{
					act("$n looks stunned as $s mind wanders from $s spells.",victim,0,0,TO_ROOM);
					act("Your head throbs with pain as you lose control over your spells.",victim,0,0,TO_CHAR);
					act("The white aura about $n vanishes!",victim,NULL,NULL,TO_ROOM);
					isDS = TRUE;
				}
			}
			if ((isDS == FALSE) && is_affected(victim,gsn_fireshield) && (number_percent() < chance-imod))
			{
				if (check_dispel(ch->level,victim,skill_lookup("fireshield")))
				{
					act("$n looks stunned as $s mind wanders from $s spells.",victim,0,0,TO_ROOM);
					act("Your head throbs with pain as you lose control over your spells.",victim,0,0,TO_CHAR);
					act("The fire around $n vanishes!",victim,NULL,NULL,TO_ROOM);
					isDS = TRUE;

				}
			}

			if ((isDS == FALSE) && is_affected(victim,gsn_stoneskin) && (number_percent() < chance-imod))
			{
				if (check_dispel(ch->level,victim,skill_lookup("stone skin")))
				{
					act("$n looks stunned as $s mind wanders from $s spells.",victim,0,0,TO_ROOM);
					act("Your head throbs with pain as you lose control over your spells.",victim,0,0,TO_CHAR);
					act("The skin of $n softens into flesh!",victim,NULL,NULL,TO_ROOM);
					isDS = TRUE;

				}
			}
			if ((isDS == FALSE) && is_affected(victim,gsn_wraithform) && (number_percent() < chance-imod))
			{
				if (check_dispel(ch->level,victim,skill_lookup("wraithform")))
				{
					act("$n looks stunned as $s mind wanders from $s spells.",victim,0,0,TO_ROOM);
					act("Your head throbs with pain as you lose control over your spells.",victim,0,0,TO_CHAR);
					act("The body of $n fades back into the Prime Material Plane!",victim,NULL,NULL,TO_ROOM);
					isDS = TRUE;

				}
			}
			if ((isDS == FALSE) && is_affected(victim,gsn_shroud) && (number_percent() < chance-imod))
			{
				if (check_dispel(ch->level,victim,skill_lookup("shroud")))
				{
					act("$n looks stunned as $s mind wanders from $s spells.",victim,0,0,TO_ROOM);
					act("Your head throbs with pain as you lose control over your spells.",victim,0,0,TO_CHAR);
					act("The black aura about $n vanishes!",victim,NULL,NULL,TO_ROOM);
					isDS = TRUE;

				}
			}
		}
	} else {
		damage_old(ch,victim,0,gsn_blitz,DAM_BASH,FALSE);
		act("You fall over and bite the dust!",ch,NULL,victim,TO_CHAR);
		act("$n falls flat on $s face, eating a mouthful of dirt.",ch,NULL,victim,TO_NOTVICT);
		act("You evade $n's blitz, and $e falls flat on $s face.",ch,NULL,victim,TO_VICT);
		check_cheap_shot(victim,ch);
		check_improve(ch,gsn_blitz,FALSE,3);
		ch->position = POS_RESTING;
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
	}
	return;
}

void check_parting_blow( CHAR_DATA *ch, CHAR_DATA *victim )
{
	if (get_skill(victim,gsn_parting_blow) > 1)
     	{
		if (number_percent() < get_skill(victim,gsn_parting_blow))
         	{
			act("You get in one more shot as $N flees.",victim,0,ch,TO_CHAR);
			act("$n gets in one more shot as $N flees.",victim,0,ch,TO_NOTVICT);
			act("$n gets in one more shot as you flee.",victim,0,ch,TO_VICT);
			one_hit(victim,ch,gsn_parting_blow);
			check_improve(victim,gsn_parting_blow,TRUE,1);
		} else {
			check_improve(victim,gsn_parting_blow,TRUE,1);
		}
	}
	return;
}

bool check_ironhands( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    int chance;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *attack;

    if ( !IS_AWAKE(victim) )
        return FALSE;
    if (IS_NPC(victim))
        return FALSE;

    if ((chance = get_skill(victim,gsn_ironhands)) == 0)
        return FALSE;

    if (get_eq_char(victim,WEAR_WIELD) != NULL)
        return FALSE;

    if (!can_see(ch,victim))
        chance *= number_range(6/10,3/4);

    if ( number_percent( ) >= ((chance + (victim->level +ch->level) - 10)/2))
    {
        check_improve(victim,gsn_ironhands,FALSE,5);
        return FALSE;
    }

/* The rest is all new dual wield stuff, with dam types displayed for clarity when dual wielding - Ceran */
    attack=get_dam_message(ch,dt);

    sprintf(buf1,"You parry $n's %s.",attack);
    sprintf(buf2,"$N parries your %s.",attack);
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
    check_improve(victim,gsn_ironhands,TRUE,6);
    return TRUE;
}

bool check_unarmed_defense( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    int chance;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *attack;

    if ( !IS_AWAKE(victim) )
        return FALSE;
    if (IS_NPC(victim))
        return FALSE;

    if ((chance = get_skill(victim,gsn_unarmed_defense)) == 0)
        return FALSE;

    if (get_eq_char(victim,WEAR_WIELD) != NULL)
        return FALSE;

    if (!can_see(ch,victim))
	{
        chance *= number_range(6/10,3/4);
	}

	chance -= (get_curr_stat(victim,STAT_DEX) - get_curr_stat(ch,STAT_DEX))*4;
	chance -= (victim->level - ch->level)*2;

    if (number_percent() >= (chance))
    {
        check_improve(victim,gsn_unarmed_defense,FALSE,5);
        return FALSE;
    }

    attack=get_dam_message(ch,dt);

    sprintf(buf1,"You deftly avoid $n's %s.",attack);
    sprintf(buf2,"$N deftly avoids %s.",attack);
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
    check_improve(victim,gsn_unarmed_defense,TRUE,6);
    return TRUE;
}

char *get_dam_message(CHAR_DATA *ch, int dt)
{
        return get_attack_noun(ch,dt);
}

char * get_attack_noun(CHAR_DATA *ch, int dt)
{
      OBJ_DATA *wield;
      int tmp_dt;
      char *attack;

      tmp_dt = TYPE_HIT;
      if (dt == gsn_dual_wield)
          wield = get_eq_char(ch,WEAR_DUAL_WIELD);
      else
      wield = get_eq_char(ch,WEAR_WIELD);
        if (wield != NULL)
          tmp_dt += wield->value[3];
      else
          tmp_dt += ch->dam_type;
      if (tmp_dt > TYPE_HIT && tmp_dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
          attack = attack_table[tmp_dt - TYPE_HIT].noun;
      else
      {
          attack = attack_table[17].noun;
      }
      return attack;
}

int get_attack_number(CHAR_DATA *ch, int dt)
{
      OBJ_DATA *wield;
      int tmp_dt;

      tmp_dt = TYPE_HIT;
      if (dt  == gsn_dual_wield)
          wield = get_eq_char(ch,WEAR_DUAL_WIELD);
      else
          wield = get_eq_char(ch,WEAR_WIELD);
        if (wield != NULL)
          tmp_dt += wield->value[3];
      else
          tmp_dt += ch->dam_type;
      if (tmp_dt > TYPE_HIT && tmp_dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
        return (tmp_dt - TYPE_HIT);
      return 1;
}

void do_gslay (CHAR_DATA * ch, char *argument)
  {
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];

  one_argument (argument, arg);
  if (arg[0] == '\0')
  {
  send_to_char ("Slay whom?\n\r", ch);
  return;
  }

  if ((victim = get_char_world (ch, arg)) == NULL)
  {
  send_to_char ("They aren't here.\n\r", ch);
  return;
  }

  if (ch == victim)
  {
  send_to_char ("Suicide is a mortal sin.\n\r", ch);
  return;
  }

  if (!IS_NPC (victim) && victim->level >= get_trust (ch))
  {
  send_to_char ("You failed.\n\r", ch);
  return;
  }

  act ("You slay $M in accordance with your will!", ch,
  NULL,
  victim, TO_CHAR);
  act ("$n slays you according to his will!", ch, NULL, victim,
TO_VICT);
  act ("$n slays $N in accordance with his will!", ch,
  NULL,
  victim, TO_NOTVICT);

  sprintf(buf, "{WAn Immortal {Gglares at {Y%s {Gand he explodes into a million pieces!{x", victim->name);
  do_echo(ch, buf);

  raw_kill (ch, victim );

  do_help(ch, "rules" );

  return;
  }


