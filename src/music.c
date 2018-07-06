#include "include.h"

DECLARE_DO_FUN(do_stand);
DECLARE_DO_FUN(do_murder);
bool    check_songsmith args( (CHAR_DATA *ch, int sn) );
int     songsmith_dam  args( ( int num, int dice) );
char *target_name;

void spell_flight(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
     CHAR_DATA *vch;
     AFFECT_DATA af;

     init_affect(&af);
     af.where = TO_AFFECTS;
     af.type = skill_lookup("fly");
     af.aftype=AFT_SONG;
     af.level = level;
     af.duration = level/2;
     af.location = 0;
     af.modifier = 0;
     af.bitvector = AFF_FLYING;

act("{GOh! I have slipped the surly bonds of earth,{x",ch,NULL,NULL,TO_ALL);
act("{GAnd danced the skies on laughter-silvered wings;{x",ch,NULL,NULL,TO_ALL);
act("{GSunward I've climbed, and joined the tumbling mirth{x",ch,NULL,NULL,TO_ALL);
act("{GOf sun-split clouds -- and done a hundred things{x",ch,NULL,NULL,TO_ALL);
check_improve(ch,gsn_songsmith,TRUE,6);
     if (!is_affected( ch, af.type ))
     {
      act("$n sings to the sky and is lifted off the ground by the winds.",ch,NULL,NULL,TO_ROOM);
      send_to_char("You feel yourself standing on the wind.\n\r", ch);
      affect_to_char( ch,&af );
     }

     for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
     {

     if (ch == vch)
          continue;

	if (!is_same_group(vch,ch) && !is_same_cabal(vch,ch))
		continue;

     if (!IS_NPC(vch))
      {
          if (number_percent() <= 80 && !is_affected( vch, af.type ))
          {
           act("$n is lifted off the ground.",vch,NULL,NULL,TO_ROOM);
           send_to_char("The wind lifts you off the ground.\n\r", vch);
           affect_to_char( vch, &af );
          }

      }
     }

     return;
 }

void spell_dew(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *vch;
    AFFECT_DATA af;

 act( "{cAs dew leaves the cobweb lightly{x", ch, NULL, NULL, TO_ALL );
 act( "{cThreaded with stars,{x", ch, NULL, NULL, TO_ALL );
 act( "{cScattering jewels on the fence{x", ch, NULL, NULL, TO_ALL );
 act( "{cAnd the pasture bars;{x", ch, NULL, NULL, TO_ALL );
 act( "{cSet every common sight{x", ch, NULL, NULL, TO_ALL );
 act( "{cOf flesh or stone{x", ch, NULL, NULL, TO_ALL );
 act( "{cDelicately alight{x", ch, NULL, NULL, TO_ALL );
 act( "{cFor me alone.{x", ch, NULL, NULL, TO_ALL );
check_improve(ch,gsn_songsmith,TRUE,6);

    init_affect(&af);
    af.where = TO_AFFECTS;
    af.aftype = AFT_SONG;
    af.type = gsn_dew;
    af.level = level;
    af.duration = 4 + (level/8);
    af.bitvector = 0;
    af.modifier = 0;

    for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room )
    {
	if (vch->invis_level > 0)
	    continue;

	if (is_same_cabal(vch,ch))
               continue;

	if (is_same_group(vch,ch))
		continue;

	if (vch->invis_level > 51 )
	    continue;

	if (vch == ch)
	    continue;

	if (vch == ch || saves_spell( level, vch, DAM_OTHER))
		//act("The dew evaporates before affecting $N.",ch,NULL,vch,TO_CHAR);
                //act("The dew dries before revealing your location.",ch,NULL,vch, TO_VICT);
                //act("Dew forms within the air, but simply evaporates.",ch,NULL,vch,TO_NOTVICT);
		return;

	affect_to_char( vch,&af);

	affect_strip ( vch,gsn_camouflage		);
	affect_strip ( vch, gsn_invis			);
	affect_strip ( vch, gsn_mass_invis		);
	affect_strip ( vch, gsn_sneak			);
	un_earthfade ( vch, NULL );
	REMOVE_BIT   ( vch->affected_by, AFF_HIDE	);
	REMOVE_BIT   ( vch->affected_by, AFF_INVISIBLE	);
	REMOVE_BIT   ( vch->affected_by, AFF_SNEAK	);
	act( "$n is revealed!", vch, NULL, NULL, TO_ROOM );
	send_to_char( "You are revealed!\n\r", vch );
        //send_to_char( "Beads of water form on your body.\n\r",vch );
  	//act( "$n is revealed by drops of dew.", vch, NULL, NULL, TO_ROOM );
   	//affect_to_char(vch, &af);
	}
  return;
}

void spell_deafening_pitch(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
     CHAR_DATA *vch=0;
     CHAR_DATA *vch_next;
     AFFECT_DATA af;

	init_affect(&af);
     af.where     = TO_AFFECTS;
     af.type       = skill_lookup("deafen");
     af.level      = level;
     af.duration = 4;
	af.aftype=AFT_SONG;
     af.location  = APPLY_NONE;
     af.modifier  = 0;
     af.bitvector = 0;

act("{mNow, my people, my time is nigh,{x",ch,NULL,NULL,TO_ALL);
act("{mWhen to bed I must go, for eight hours to lie.{x",ch,NULL,NULL,TO_ALL);
act("{mI must twist like a lizard into my own dreams,{x",ch,NULL,NULL,TO_ALL);
act("{mAnd you'll know that I'm dreaming by my deafening screams;{x",ch,NULL,NULL,TO_ALL);
act("{mYou'll know that I'm sleeping by the sweat pouring down my face,{x",ch,NULL,NULL,TO_ALL);
act("{mAnd I'll dissolve in the night, disappearing without trace.{x",ch,NULL,NULL,TO_ALL);
WAIT_STATE(ch, 1 * PULSE_VIOLENCE);
check_improve(ch,gsn_songsmith,TRUE,6);

      for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
     {
         vch_next = vch->next_in_room;
         if (is_same_group(vch,ch))
              continue;
         if (is_safe(ch,vch))
             continue;
	 if (is_affected(vch,gsn_deafen))
		continue;
         if (is_same_cabal(vch,ch))
               continue;
	  if (ch->fighting != NULL)
          {
           send_to_char( "You cannot find the opportunity to sing this song in the midst of combat.\n\r", vch );
          }


       {

           if (!saves_spell(level,vch,DAM_OTHER))
               {
              act( "Hands to the sky, $n lets out the highest note they can possibly manage.\n\r", vch,NULL,NULL, TO_VICT );
              act( "$n screams in agony and clutches their ears.", vch, NULL, NULL, TO_ROOM );
              affect_to_char(vch, &af);
           }
           else
           {
           act("$n covers his ears just in time to avoid the effects of your song.\n\r",vch,NULL,NULL,TO_CHAR);
           act("You cover your ears and $n's song does not affect you.\n\r",ch,NULL,vch, TO_VICT);
           act("$N covers his ears just in time to avoid the effects of $n's song.\n\r",vch,NULL,ch,TO_NOTVICT);
           }
      }
     }
     return;
 }

void spell_healing(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *vch=0;

act("{yA barrier breached{x",ch,NULL,NULL,TO_ALL);
act("{yDestined harmony disrupted{x",ch,NULL,NULL,TO_ALL);
act("{yOnce companions{x",ch,NULL,NULL,TO_ALL);
act("{yTurn to that which made them one{x",ch,NULL,NULL,TO_ALL);
act("{yTo make the body whole again{x",ch,NULL,NULL,TO_ALL);
WAIT_STATE(ch,PULSE_VIOLENCE);
check_improve(ch,gsn_songsmith,TRUE,6);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {

        if (is_safe(ch,vch))
                continue;

	if (!is_same_group(vch,ch) && !is_same_cabal(vch,ch))
		continue;

        if (!IS_NPC(vch))
        {
          vch->hit = UMIN( vch->hit + 100, vch->max_hit );
            act("$n's cheeks flush as his wounds heal.",vch,NULL,NULL,TO_ROOM);
            send_to_char("Your face feels warm as your wounds heal.\n\r", vch);

        	}
        }
  return;
}

void spell_balmy_sleep(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *vch=0;
    CHAR_DATA *vch_next;
    AFFECT_DATA af;
/*
act("{bSleep, balmy sleep, has closed the eyes of all{x",ch,NULL,NULL,TO_ALL);
act("{bBut me! ah me! no respite can I gain;{x",ch,NULL,NULL,TO_ALL);
act("{bTho' darkness reigns o're the terrestrial ball,{x",ch,NULL,NULL,TO_ALL);
act("{bNot one soft slumber cheats this vital pain.{x",ch,NULL,NULL,TO_ALL);
*/
	init_affect(&af);
    af.where     = TO_AFFECTS;
    af.type      = skill_lookup("sleep");
    af.aftype    = AFT_SONG;
    af.level     = level;
    af.duration  = 5;
    af.aftype=AFT_SONG;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_SLEEP;

act("{bSleep, balmy sleep, has closed the eyes of all{x",ch,NULL,NULL,TO_ALL);
act("{bBut me! ah me! no respite can I gain;{x",ch,NULL,NULL,TO_ALL);
act("{bTho' darkness reigns o're the terrestrial ball,{x",ch,NULL,NULL,TO_ALL);
act("{bNot one soft slumber cheats this vital pain.{x",ch,NULL,NULL,TO_ALL);
WAIT_STATE(ch,PULSE_VIOLENCE*1);
check_improve(ch,gsn_songsmith,TRUE,6);

     for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next_in_room;
        if (is_same_group(vch,ch))
            continue;
        if (is_safe(ch,vch))
                continue;
        if (is_same_cabal(vch,ch))
		continue;
	if (is_affected(vch,gsn_sleep))
		continue;
	if (vch->invis_level > 51 )
                continue;

   	 if (ch->fighting != NULL)
    	{
		send_to_char( "You are too out of breath for that song.\n\r", vch );
    	}


    	if ( IS_AWAKE(vch))
 	{

		if (!saves_spell(level,vch,DAM_OTHER))
    		{
        	send_to_char( "You feel yourself falling asleep.\n\r", vch );
  		act( "$n goes to sleep.", vch, NULL, NULL, TO_ROOM );
       		vch->position = POS_SLEEPING;
   		affect_to_char(vch, &af);
		}
		else
		{
		act("$n resists your songs affects.\n\r",vch,NULL,NULL,TO_CHAR);
    		act("You resist the affects of $n's song.\n\r",ch,NULL,NULL, TO_VICT);
		act("$n resists the affects of $n's song.\n\r",vch,NULL,ch,TO_NOTVICT);
		}
	}
    }
    return;
}

void spell_storms(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam;
    int check;
    AFFECT_DATA af;

	init_affect(&af);
    af.where = TO_AFFECTS;
    af.location = APPLY_HITROLL;
    af.modifier = -3;
    af.duration = 3;
	af.aftype=AFT_SONG;
    af.level = level;
    af.type = skill_lookup ("windwall");
    af.bitvector = AFF_BLIND;

act("{CThe four winds of earth, the North, South, East, and West,{x",ch,NULL,NULL,TO_ALL);
act("{CShrieked and groaned, sobbed and wailed, like the soul of unrest.{x",ch,NULL,NULL,TO_ALL);
act("{CI stood in the dusk of the twilight alone,{x",ch,NULL,NULL,TO_ALL);
act("{CAnd heard them go by with a terrible moan.{x",ch,NULL,NULL,TO_ALL);
act("{C'What is it, O winds! that is grieving you so?{x",ch,NULL,NULL,TO_ALL);
act("{CCome tell me your sorrow, and tell me your woe!'{x",ch,NULL,NULL,TO_ALL);
act("{C'What is it?' I questioned. They shuddered, and said:{x",ch,NULL,NULL,TO_ALL);
act("{C'We mourn for the dead! Oh! we mourn for the dead-- {x",ch,NULL,NULL,TO_ALL);
    send_to_char("Your song raises strong winds to strike your foes!\n\r",ch);
    act("$n song raises terrible winds all around you!",ch,NULL,NULL,TO_ROOM);
    WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
    check_improve(ch,gsn_songsmith,TRUE,6);
    dam = dice(level, 6);
    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
	vch_next = vch->next_in_room;
        if (is_same_group(vch,ch))
            continue;

	if (vch->invis_level > 51 )
		continue;

	if (is_safe(ch,vch))
		continue;

	if (IS_NPC(vch))
	{
	if (ch->cabal == CABAL_EMPIRE && vch->pIndexData->vnum == MOB_VNUM_CENTURION)
		continue;
	}

	if (is_same_cabal(vch,ch))
               continue;

/*	if (check_songsmith(ch,sn))
	dam = songsmith_dam(level,13);
	else
	dam = dice(level/3,6);
*/

	if (check_songsmith(ch,sn))
        dam = songsmith_dam(level*3,4);
    else
        dam = dice(level*1.75,4);

        check = skill_lookup ("windwall"); // To stop windwall stacking.
        if ((number_range(0,1) == 0)
        && !saves_spell(level,vch,DAM_BASH)
        && !is_affected(vch, check))
        {
            act("$n appears blinded by the debris.",vch,NULL,NULL,TO_ROOM);
            affect_to_char(vch,&af);
        }
        damage_old(ch,vch,dam,skill_lookup("windwall"),DAM_BASH,TRUE);


	  if (number_percent()>50)
		{
            act("$n is thrown wildly to the ground by force of the winds!",vch,NULL,NULL,TO_ROOM);
            send_to_char("You are thrown down by the strong winds!\n\r",vch);
            affect_strip(vch,skill_lookup("fly"));
		WAIT_STATE(vch, 1 * PULSE_VIOLENCE);
		}
    }
    return;
}

void spell_titans(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *vch;
    AFFECT_DATA af1, af2;

	init_affect(&af1);
    af1.where = TO_AFFECTS;
    af1.type = skill_lookup("berserk");
    af1.level = level;
	af1.aftype=AFT_SONG;
    af1.duration = level/2;
    af1.location = APPLY_DAMROLL;
    af1.modifier = level/8;
    af1.bitvector = 0;

	init_affect(&af2);
    af2.where = TO_AFFECTS;
    af2.type = skill_lookup("warcry");
    af2.level = level;
	af2.aftype=AFT_SONG;
    af2.duration = level/2;
    af2.location = APPLY_SAVES;
    af2.modifier = -7;
    af2.bitvector = 0;

act("{RBlack, formless, without substance, motionless.{x",ch,NULL,NULL,TO_ALL);
act("{RThe flaming tumult of disastrous fight,{x",ch,NULL,NULL,TO_ALL);
act("{RTalking their outward fire, had left them bleak{x",ch,NULL,NULL,TO_ALL);
act("{RAs their own statues, who yet ached within.{x",ch,NULL,NULL,TO_ALL);
act("{RAnd battle-splendours gloomed with rust were strewn{x",ch,NULL,NULL,TO_ALL);
act("{RAround them, where the darkness heeded not{x",ch,NULL,NULL,TO_ALL);
	check_improve(ch,gsn_songsmith,TRUE,6);
    if (!is_affected( ch, af1.type ))
    {
	act("$n is filled with fury.",ch,NULL,NULL,TO_ROOM);
	send_to_char("You are filled with fury.\n\r", ch);
	affect_to_char(ch,&af1);
    }

    if (!is_affected( ch, af2.type ))
    {
	act("$n is suddenly overcome by the cry of battle!",ch,NULL,NULL,TO_ROOM);
	send_to_char("You are suddenly overcome by the cry of battle!\n\r", ch);
	affect_to_char(ch,&af2);
    }

    for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
	if (ch == vch)
	    continue;

	if (!is_same_group(vch,ch) && !is_same_cabal(vch,ch))
	continue;

	if (!IS_NPC(vch))
	{
	    if (number_percent() <= 80 && !is_affected( vch, af1.type ))
	    {
		act("$n is filled with fury.\n\r",vch,NULL,NULL,TO_ROOM);
		send_to_char("You are filled with fury.\n\r", vch);
		affect_to_char( vch, &af1 );
	    }
	    if (number_percent() <= 80 && !is_affected( vch, af2.type ))
	    {
		act("$n is overcome by the cry of battle.\n\r",vch,NULL,NULL,TO_ROOM);
		send_to_char("You are suddenly overcome by the cry of battle!\n\r", vch);
		affect_to_char( vch, &af2 );
	    }
	}
    }

    return;
}

void spell_guarding(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *vch;
    AFFECT_DATA af1, af2;

	init_affect(&af1);
    af1.where = TO_AFFECTS;
    af1.type = skill_lookup("stone skin");
    af1.level = level;
	af1.aftype=AFT_SONG;
    af1.duration = level/2;
    af1.location = APPLY_AC;
    af1.modifier = -40;
    af1.bitvector = 0;

	init_affect(&af2);
    af2.where = TO_AFFECTS;
    af2.type = skill_lookup("armor");
    af2.level = level;
	af2.aftype=AFT_SONG;
    af2.duration = level/2;
    af2.location = APPLY_AC;
    af2.modifier = -20;
    af2.bitvector = 0;

act("{WEntering the castle,{x",ch,NULL,NULL,TO_ALL);
act("{WAnd danced the skies on laughter-silvered wings;{x",ch,NULL,NULL,TO_ALL);
act("{Wice crystals safe from dawn,{x",ch,NULL,NULL,TO_ALL);
act("{WI am drawn to the cold,{x",ch,NULL,NULL,TO_ALL);
act("{WYour warmth is my knight,{x",ch,NULL,NULL,TO_ALL);
act("{Wflames on ice ablaze,{x",ch,NULL,NULL,TO_ALL);
act("{WOn to the valley, bright with kings,{x",ch,NULL,NULL,TO_ALL);
act("{WPast the copper statues,{x",ch,NULL,NULL,TO_ALL);
act("{Wruby angels, golden wings.{x",ch,NULL,NULL,TO_ALL);
	check_improve(ch,gsn_songsmith,TRUE,6);
    if (!is_affected( ch, af1.type ))
    {
	act("$n looks as solid as a rock.",ch,NULL,NULL,TO_ROOM);
	send_to_char("You feel your skin harden like stone.\n\r", ch);
	affect_to_char(ch,&af1);
    }

    if (!is_affected( ch, af2.type ))
    {
	act("$n is suddenly surrounded by a glowing suit of armor.",ch,NULL,NULL,TO_ROOM);
	send_to_char("You are suddenly surrounded by a glowing suit of armor.\n\r", ch);
	affect_to_char(ch,&af2);
    }

    for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
	if (ch == vch)
	    continue;

	if (!is_same_group(vch,ch) && !is_same_cabal(vch,ch))
		continue;

	if (!IS_NPC(vch))
	{
	    if (number_percent() <= 80 && !is_affected( vch, af1.type ))
	    {
		act("$n glows briefly.",vch,NULL,NULL,TO_ROOM);
		send_to_char("You glow briefly.\n\r", vch);
		affect_to_char( vch, &af1 );
	    }
	    if (number_percent() <= 80 && !is_affected( vch, af2.type ))
	    {
		act("$n is suddenly surrounded by a glowing suit of armor.",vch,NULL,NULL,TO_ROOM);
		send_to_char("You are suddenly surrounded by a glowing suit of armor.\n\r", vch);
		affect_to_char( vch, &af2 );
	    }
	}
    }

    return;
}

void spell_insanity(int sn, int level, CHAR_DATA *ch, void *vo, int
target)
{
     CHAR_DATA *vch;
     AFFECT_DATA af;

     if (IS_AFFECTED(ch,AFF_LOOKING_GLASS))
     	return;

     init_affect(&af);
     af.where = TO_AFFECTS;
     af.type = skill_lookup("looking glass");
     af.aftype=AFT_SONG;
     af.level = level;
     af.duration = 3;
     af.location = 0;
     af.modifier = 0;
     af.bitvector = AFF_LOOKING_GLASS;


act("{MIt clouds my mind{x",ch,NULL,NULL,TO_ALL);
act("{MAnd still provokes thought.{x",ch,NULL,NULL,TO_ALL);
act("{MTwisted images are smote and wrought.{x",ch,NULL,NULL,TO_ALL);
act("{MThe voice in my head leads me on.{x",ch,NULL,NULL,TO_ALL);
act("{MI can not find a pattern that.s smooth.{x",ch,NULL,NULL,TO_ALL);
act("{MI bump down a rugged hill.{x",ch,NULL,NULL,TO_ALL);
act("{MThey say you need help,{x",ch,NULL,NULL,TO_ALL);
act("{MIn all honesty you know your quite pleased.{x",ch,NULL,NULL,TO_ALL);
check_improve(ch,gsn_songsmith,TRUE,6);
WAIT_STATE(ch,PULSE_VIOLENCE*3);

     for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
     {

     if (ch == vch)
          continue;

	if (is_safe(ch,vch))
                continue;

	if (is_same_group(vch,ch))
		continue;

	if (is_same_cabal(vch,ch))
               continue;

	if (saves_spell(level,vch,DAM_OTHER))
        	continue;

     if (!IS_NPC(vch))
      {
          if (number_percent() <= 80 && !is_affected( vch, af.type ))
          {
           act("$n's eyes open wide in bewilderment.",vch,NULL,NULL,TO_ROOM);
           send_to_char("You feel yourself losing touch with reality.\n\r", vch);
           affect_to_char( vch, &af );
          }

      }
     }

     return;
 }

void spell_chilliness_of_death(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
     CHAR_DATA *vch;
     AFFECT_DATA af;

     init_affect(&af);
     af.where = TO_AFFECTS;
     af.type = skill_lookup("weaken");
     af.aftype=AFT_SONG;
     af.level = level;
     af.duration = level/2;
     af.location = APPLY_STR;
     af.modifier = -10;
     af.bitvector = 0;


act("{ythe chilyness of death{x",ch,NULL,NULL,TO_ALL);
act("{ytouched me playfull,{x",ch,NULL,NULL,TO_ALL);
act("{ythoughtless {x",ch,NULL,NULL,TO_ALL);
act("{yjust passing by {x",ch,NULL,NULL,TO_ALL);
act("{yjust then you feel{x",ch,NULL,NULL,TO_ALL);
act("{yhow weak a man is{x",ch,NULL,NULL,TO_ALL);
act("{ynot very firm not{x",ch,NULL,NULL,TO_ALL);
act("{yvery prepared{x",ch,NULL,NULL,TO_ALL);
check_improve(ch,gsn_songsmith,TRUE,6);

     for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
     {

     if (ch == vch)
          continue;

	if (is_safe(ch,vch))
		continue;

	if (is_same_group(vch,ch))
		continue;

	if (is_same_cabal(vch,ch))
               continue;

	if (vch->invis_level > 51 )
                continue;

     if (ch != vch)
      {
          if (number_percent() <= 70 && !is_affected( vch, af.type ))
          {
           act("$n's shoulders slump as the strength is drained from them.",vch,NULL,NULL,TO_ROOM);
           send_to_char("You feel your strength diminish.\n\r", vch);
           affect_to_char( vch, &af );
          }

      }
     }

     return;
 }

void spell_resting(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *vch=0;

act("{YArise, awake, the soaring flight of a bird over green meadows of fragrant flowers bloomin{x",ch,NULL,NULL,TO_ALL);
act("{Ynodding in the gentle mist of a cool shower, a beginning, an awakening, oh how glorious{x",ch,NULL,NULL,TO_ALL);
act("{Yagain, bitten and again reborn, painting the birth of rejuvination.{x",ch,NULL,NULL,TO_ALL);
check_improve(ch,gsn_songsmith,TRUE,6);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {

        if (is_safe(ch,vch))
                continue;

	if (!is_same_group(vch,ch) && !is_same_cabal(vch,ch))
		continue;

        if (!IS_NPC(vch))
        {
          vch->mana = UMIN( vch->mana + 75, vch->max_mana );
            act("$n breaths deeply as his mind is rejuvinated.",vch,NULL,NULL,TO_ROOM);
            send_to_char("You breath deeply as your mind is rejuvinated.\n\r", vch);

        	}
        }
  return;
}

void spell_armageddon(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *vch=0;
    int dam, sn_frost, sn_fire, d_type, sn_type;

    sn_frost = skill_lookup("firestream");
    sn_fire = skill_lookup("iceball");

act("{BWithout sun's warmth will come the cold,{x",ch,NULL,NULL,TO_ALL);
act("{BSo frigid will be arctic blast;{x",ch,NULL,NULL,TO_ALL);
act("{BAll plants will die before they're old,{x",ch,NULL,NULL,TO_ALL);
act("{BEach species dies with man the last.{x",ch,NULL,NULL,TO_ALL);
act("{RA ball of fire may hit our lands,{x",ch,NULL,NULL,TO_ALL);
act("{ROr might in fact destroy our sun;{x",ch,NULL,NULL,TO_ALL);
act("{RAll is out of control, out of our hands{x",ch,NULL,NULL,TO_ALL);
act("{RNo shelter man can find to run.{x",ch,NULL,NULL,TO_ALL);
check_improve(ch,gsn_songsmith,TRUE,6);
WAIT_STATE(ch,PULSE_VIOLENCE*3);

for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {

        if (is_safe(ch,vch))
                continue;

	if (is_same_group(vch,ch))
		continue;

	if (is_same_cabal(vch,ch))
               continue;

	if(IS_NPC(vch))
	{
	 if (ch->cabal == CABAL_EMPIRE && vch->pIndexData->vnum == MOB_VNUM_CENTURION)
                continue;
	}

	if (ch == vch)
	    continue;

	if (vch->invis_level > 51 )
                continue;

    if (number_percent() > 50)
    {
        sn_type = sn_frost;
        d_type = DAM_COLD;
    }
    else
    {
        sn_type = sn_fire;
        d_type = DAM_FIRE;
    }
    if (check_songsmith(ch,sn))
	dam = songsmith_dam(level*3,4);
    else
	dam = dice(level*1.75,4);

    if(!IS_NPC(ch))
	dam/=1.5;

    damage_old(ch,vch,dam,sn_type,d_type,TRUE);

    if (check_songsmith(ch,sn))
        dam = songsmith_dam(level*3,4);
    else
        dam = dice(level*1.75, 4);

    if (d_type == DAM_COLD)
    {
        d_type = DAM_FIRE;
        sn_type = sn_fire;
    }
    else
    {
        d_type = DAM_COLD;
        sn_type = sn_frost;
    }

    if(!IS_NPC(ch))
	dam/=3;

    damage_old(ch,vch,dam,sn_type,d_type,TRUE);
}
    return;

}

void spell_hour_of_rising(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *vch=0;

act("{gSomnolent waves of{x",ch,NULL,NULL,TO_ALL);
act("{gshifting consciousness,{x",ch,NULL,NULL,TO_ALL);
act("{gintangible images{x",ch,NULL,NULL,TO_ALL);
act("{gintertwine with substance.{x",ch,NULL,NULL,TO_ALL);
act("{gTime creeps in,{x",ch,NULL,NULL,TO_ALL);
act("{gtenacious reminder{x",ch,NULL,NULL,TO_ALL);
act("{gas seductive sleep{x",ch,NULL,NULL,TO_ALL);
act("{ggradually dissolves.{x",ch,NULL,NULL,TO_ALL);
act("{gThe day begins.{x",ch,NULL,NULL,TO_ALL);
check_improve(ch,gsn_songsmith,TRUE,6);

	for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
	if (is_safe(ch,vch))
                continue;

	if (vch == ch)
		continue;

	if (IS_AWAKE(vch))
	{
		act("$N is already awake.", ch, NULL, vch, TO_CHAR);
		continue;
	}

	if (is_affected(vch,skill_lookup("sleep")))
	{
		if (check_dispel(level,vch,skill_lookup("sleep")))
		{
			send_to_char( "You suddenly awaken.\n\r", vch );
			act("$n suddenly awakens.",vch,NULL,NULL,TO_ROOM);
			do_stand(vch,"");
		} else {
			send_to_char("You failed to awaken them.\n\r",ch);
		}
		return;
	} else if (is_affected(vch,gsn_strangle)) {
		if (check_dispel(level,vch,gsn_strangle))
		{
			send_to_char( "You suddenly awaken.\n\r", vch );
			act("$n suddenly awakens.",vch,NULL,NULL,TO_ROOM);
			do_stand(vch,"");
		} else {
			send_to_char("You failed to awaken them.\n\r",ch);
		}
		return;
	} else if (is_affected(vch,gsn_blackjack)) {
		if (check_dispel(level,vch,gsn_blackjack))
		{
			send_to_char( "You suddenly awaken.\n\r", vch );
			act("$n suddenly awakens.",vch,NULL,NULL,TO_ROOM);
			do_stand(vch,"");
		} else {
			send_to_char("You failed to awaken them.\n\r",ch);
		}
		return;
	} else if (is_affected(vch,gsn_choke)) {
		if (check_dispel(level,vch,gsn_choke))
		{
			send_to_char( "You suddenly awaken.\n\r", vch );
			act("$n suddenly awakens.",vch,NULL,NULL,TO_ROOM);
			do_stand(vch,"");
		} else {
			send_to_char("You failed to awaken them.\n\r",ch);
		}
		return;
	} else if (is_affected(vch,gsn_choke)) {
		if (check_dispel(level,vch,gsn_choke))
		{
			send_to_char( "You suddenly awaken.\n\r", vch );
			act("$n suddenly awakens.",vch,NULL,NULL,TO_ROOM);
			do_stand(vch,"");
		} else {
			send_to_char("You failed to awaken them.\n\r",ch);
		}
		return;
	} else if (is_affected(vch,gsn_stun)) {
		if (check_dispel(level,vch,gsn_stun))
		{
			send_to_char( "You suddenly awaken.\n\r", vch );
			act("$n suddenly awakens.",vch,NULL,NULL,TO_ROOM);
			do_stand(vch,"");
		} else {
			send_to_char("You failed to awaken them.\n\r",ch);
		}
		return;
	} else {
		send_to_char("They don't seem to be in deep sleep.\n\r",ch);
		return;
	}
	}
	return;
}


void spell_stonelike_resolve(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *vch;
    AFFECT_DATA af;

 act( "{cNow standing stiffly,{x", ch, NULL, NULL, TO_ALL );
 act( "{cRigidly,{x", ch, NULL, NULL, TO_ALL );
 act( "{cSnow capped head{x", ch, NULL, NULL, TO_ALL );
 act( "{cHonoring cruel combat.s dead{x", ch, NULL, NULL, TO_ALL );
 act( "{cRising hoary{x", ch, NULL, NULL, TO_ALL );
 act( "{cTall above silvery cloud,{x", ch, NULL, NULL, TO_ALL );
 act( "{cHeaven's bright shroud.{x", ch, NULL, NULL, TO_ALL );
 check_improve(ch,gsn_songsmith,TRUE,6);

    init_affect(&af);
    af.where = TO_AFFECTS;
    af.aftype = AFT_SONG;
    af.type = gsn_body_of_stone;
    af.level = level;
    af.duration = 4 + (level/8);
    af.bitvector = 0;
    af.modifier = 0;

    for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room
)
    {
	if (vch->invis_level > 51 )
	    continue;

	if (is_affected(vch,gsn_body_of_stone))
		continue;

	if (!is_same_group(vch,ch) && !is_same_cabal(vch,ch))
		continue;

       	act("$n's jaw tightens with new resolve.",vch,NULL,NULL,TO_ROOM);
           send_to_char("Your jaw tightens with new resolve.\n\r", vch);
           affect_to_char( vch, &af );
	}
  return;
}

void spell_phantom(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
        CHAR_DATA *phantom, *victim;
        AFFECT_DATA af;
        int i, dam;

	if (is_affected(ch,sn))
        {
          send_to_char("You cannot create an illusion again so soon.\n\r", ch);
          return;
        }
 act( "{WBone white mask {x", ch, NULL, NULL, TO_ALL );
        act( "{WMy black cloak {x", ch, NULL, NULL, TO_ALL );
        act( "{WI'll wait around forever {x", ch, NULL, NULL, TO_ALL );
        act( "{WBut until then,{x", ch, NULL, NULL, TO_ALL );
        act( "{WI am merely a dream {x", ch, NULL, NULL, TO_ALL );
        act( "{WA show {x", ch, NULL, NULL, TO_ALL );
        act( "{WA Phantom, {x", ch, NULL, NULL, TO_ALL );
        act( "{WFading away into the night.{x", ch, NULL, NULL, TO_ALL );
	WAIT_STATE(ch,PULSE_VIOLENCE*3);
	send_to_char("You attempt to summon a phantom.\n\r",ch);
        act("$n attempts to summon a phantom.",ch,NULL,NULL,TO_ROOM);
	check_improve(ch,gsn_songsmith,TRUE,6);

	for ( victim = ch->in_room->people; victim != NULL; victim = victim->next_in_room)
	{
	   if (victim == ch
           || victim->in_room == NULL
           || IS_NPC(victim))
		continue;

	if (saves_spell(level,victim,DAM_OTHER))
		continue;

	if (is_safe(ch,victim))
                continue;

	if (is_same_group(victim,ch))
		continue;

	if (is_same_cabal(victim,ch))
               continue;

	if (victim->invis_level > 51 )
		continue;

        phantom = create_mobile(get_mob_index(MOB_VNUM_PHANTOM));

	init_affect(&af);
        af.aftype = AFT_SONG;
        af.where              = TO_AFFECTS;
        af.type               = sn;
        af.level              = level;
        af.duration           = 15;
        af.bitvector          = 0;
        af.modifier           = 0;
        af.location           = APPLY_NONE;
        affect_to_char(ch, &af);

        for (i=0;i < MAX_STATS; i++)
        {
          phantom->perm_stat[i] = victim->perm_stat[i];
        }

	dam *= .3;

        phantom->max_hit = UMIN(20000,victim->max_hit);
        phantom->hit = victim->max_hit;
        phantom->max_mana = victim->max_mana;
        phantom->mana = victim->max_mana;
        phantom->level = victim->level;
	phantom->damroll = victim->level*1.5;
	phantom->hitroll = victim->level*1.5;
	phantom->invis_level = 52;
        phantom->damage[DICE_NUMBER] =
                number_range(victim->level/10, victim->level/10);
        phantom->damage[DICE_TYPE] =
                number_range(victim->level/4, victim->level/3);
        phantom->damage[DICE_BONUS] =
                number_range(victim->level/10, victim->level/9);

        for (i=0; i < 3; i++)
        phantom->armor[i] = interpolate(phantom->level,100,-100);
        phantom->armor[3] = interpolate(phantom->level,100,0);
        phantom->gold = 0;
        phantom->level = 52;

	SET_BIT(phantom->act,PLR_HOLYLIGHT);
        char_to_room(phantom,victim->in_room);
        phantom->last_fought = victim;

        send_to_char("An invisible phantom arrives to torment you!\n\r",victim);
        act("An invisible phantom arrives to torment $n!\n\r",victim,NULL,NULL,TO_ROOM);
        send_to_char("You have created a phantom in the mind of your enemy!\n\r", ch);
	do_murder(phantom,victim->name);
	}
	return;
}

void spell_divine_blessing(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    AFFECT_DATA af;

sn = skill_lookup("bless");

 if (is_affected(ch, sn))
 {
	 send_to_char("You already have a divine blessing.\n\r",ch);
	 return;
 }

 act( "{cOh lords above,{x", ch, NULL, NULL, TO_ALL );
 act( "{cHear my beckon,{x", ch, NULL, NULL, TO_ALL );
 act( "{cI request aid,{x", ch, NULL, NULL, TO_ALL );
 act( "{cFor the crusade,{x", ch, NULL, NULL, TO_ALL );
 act( "{cThat life has given,{x", ch, NULL, NULL, TO_ALL );
 act( "{cI call to ye, Lords!{x", ch, NULL, NULL, TO_ALL );
 act( "{cGrant me a divine blessing!{x", ch, NULL, NULL, TO_ALL );
 check_improve(ch,gsn_songsmith,TRUE,6);

 	init_affect(&af);
    af.aftype = AFT_SONG;
    af.where     = TO_AFFECTS;
    af.type      = skill_lookup("bless");
    af.level	 = level;
    af.duration  = 6+level;
    af.location  = APPLY_HITROLL;
	af.modifier = 10;
	af.bitvector = 0;
	affect_to_char( ch, &af );
	af.location  = APPLY_SAVING_SPELL;
	af.modifier = -10;
	affect_to_char( ch, &af );
   	act("$n smiles as the divine answers the beckon.\n\r",ch,NULL,NULL,TO_ROOM);
    send_to_char("The lords have answered your beckon.\n\r", ch);
	return;
}

