// Should work fine. Just add it to const, interp.c and .h
// and declare the gsn's in db and merc. You'll need to make
// another object for the pc corpse. Just copy and paste the
// existing pc corpse, and define the obj in merc and your set.
// Just include this header in the code if you use it.


/*

			Made for Inahn's Doom by Ceial June 28, 2003
						(footlongbobby@hotmail.com)

*/

void do_siphon( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_STRING_LENGTH];
  char * name;
  char * last_name;
  int hit = 0, dam = 0;
  OBJ_DATA *corpse;
  OBJ_DATA *drained_corpse;
  OBJ_DATA *obj; 		// Shit in the corpse
  OBJ_DATA *obj_next;
  AFFECT_DATA af;
  AFFECT_DATA *paf;
  AFFECT_DATA *paf_next;

  one_argument(argument, arg);

  corpse = get_obj_list( ch, arg, ch->in_room->contents );

  if (IS_NPC(ch)
  ||  ch->level < skill_table[gsn_siphon].skill_level[ch->class])
  {
  	send_to_char("Huh?\n\r",ch);
  	return;
  }

  if (corpse == NULL)
  {
	  send_to_char("You don't see that here.\n\r", ch );
	  return;
  }

  if (corpse->pIndexData->vnum != OBJ_VNUM_CORPSE_PC) // So you can only siphon it once.
  {
	  send_to_char("You can't siphon energy from that.\n\r", ch );
	  return;
  }

  /*
  corpse->talked is just a pointer in obj_data that assigns the killer
  name to it in fight.c when creating the corpse. You can do that, or
  just take this out.

  Here's what it looks like in fight.c, just add this.

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
*/

  if (strcmp(corpse->talked,ch->name))
  {
	  send_to_char("You can only drain your own kills.\n\r", ch );
	  return;
  }

  // If they are already affected by it. They get the bonus added on top
  // of the existing affect.

  if (is_affected(ch, gsn_siphon))
  {
	  for ( paf = ch->affected; paf != NULL; paf = paf_next )
	  {
	  	paf_next = paf->next;
	  	if ( paf->type == gsn_siphon ) {
	  		switch (paf->location)
	  		{
	  		default: break;
	  		case APPLY_HITROLL: hit = paf->modifier; affect_remove( ch, paf ); break;
	  		case APPLY_DAMROLL: dam = paf->modifier; affect_remove( ch, paf ); break;
	  		}
	  	}
	  }

	  if (ch->level < 20)
	  {
		  hit += 1;
		  dam += 1;
	  }else if (ch->level < 30){
		  hit += 2;
		  dam += 2;
	  }else if (ch->level < 40){
		  hit += 3;
		  dam += 3;
	  }
	  else if (ch->level < 50){
		  hit += 4;
		  dam += 4;
	  }else{
		  hit += 5;
		  dam += 5;
	  }

	  init_affect(&af);
	  af.aftype 		= AFT_SKILL;
	  af.where     		= TO_AFFECTS;
	  af.type 	 		= gsn_siphon;
	  af.level	 		= ch->level;
	  af.duration       = -1;
	  af.modifier  		= hit;
	  af.bitvector 		= 0;
	  af.location  		= APPLY_HITROLL;
	  affect_to_char(ch,&af);
	  af.modifier		= dam;
	  af.location  		= APPLY_DAMROLL;
	  affect_to_char(ch,&af);

	  drained_corpse = create_object(get_obj_index(OBJ_VNUM_SIPHON_CORPSE),0);

	  for (obj = corpse->contains; obj != NULL; obj = obj_next)
	  {
	   	obj_next = obj->next_content;
	   	obj_from_obj(obj);
	   	obj_to_obj(obj,drained_corpse);
      }
	  name = corpse->short_descr;

	  last_name = name;
	  last_name = one_argument(last_name,name);
	  last_name = one_argument(last_name,name);
	  last_name = one_argument(last_name,name);
	  name = last_name;

	  sprintf(buf,"the drained and lifeless corpse of %s",name);
	  free_string(drained_corpse->short_descr);
	  drained_corpse->short_descr = str_dup(buf);
	  sprintf(buf,"The drained and lifeless corpse of %s is lying here.",name);
	  free_string(drained_corpse->description);
      drained_corpse->description = str_dup(buf);
	  extract_obj( corpse );
	  obj_to_room( drained_corpse, ch->in_room );

	  sprintf( buf, "$n magically siphons the life force from the corpse of %s.",
	  name );
	  act( buf, ch, NULL, NULL, TO_ROOM );
	  send_to_char("You feel more powerful as you siphon the life force from the corpse.\n\r",
	  ch );
	  check_improve(ch,gsn_siphon,TRUE,4);
	  WAIT_STATE(ch,skill_table[gsn_siphon].beats);
	  return;
  }
  // If not then we'll give them a small bonus to start off with.
  else
  {
	  init_affect(&af);
	  af.aftype 		= AFT_SKILL;
	  af.where     		= TO_AFFECTS;
	  af.type 	 		= gsn_siphon;
	  af.level	 		= ch->level;
	  af.duration       = -1;
	  af.modifier  		= 2;
	  af.bitvector 		= 0;
	  af.location  		= APPLY_HITROLL;
	  affect_to_char(ch,&af);
	  af.modifier		= 2;
	  af.location  		= APPLY_DAMROLL;
	  affect_to_char(ch,&af);

	  drained_corpse = create_object(get_obj_index(OBJ_VNUM_SIPHON_CORPSE),0);

	  for (obj = corpse->contains; obj != NULL; obj = obj_next)
	  {
	  	obj_next = obj->next_content;
	  	obj_from_obj(obj);
	  	obj_to_obj(obj,drained_corpse);
      }

	  name = corpse->short_descr;

	  last_name = name;
	  last_name = one_argument(last_name,name);
	  last_name = one_argument(last_name,name);
	  last_name = one_argument(last_name,name);
	  name = last_name;

	  sprintf(buf,"the drained and lifeless corpse of %s",name);
	  free_string(drained_corpse->short_descr);
	  drained_corpse->short_descr = str_dup(buf);
	  sprintf(buf,"The drained and lifeless corpse of %s is lying here.",name);
	  free_string(drained_corpse->description);
      drained_corpse->description = str_dup(buf);
	  extract_obj( corpse );
	  obj_to_room( drained_corpse, ch->in_room );

	  sprintf( buf, "$n magically siphons the life force from the corpse of %s.",
	  name );
	  act( buf, ch, NULL, NULL, TO_ROOM );
	  send_to_char("You feel more powerful as you siphon the life force from the corpse.\n\r",
	  ch );
	  check_improve(ch,gsn_siphon,TRUE,4);
	  WAIT_STATE(ch,skill_table[gsn_siphon].beats);
	  return;
  }
}





