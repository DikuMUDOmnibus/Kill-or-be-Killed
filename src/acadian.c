#include "include.h"

// Track.c

/*
 * Modified by Baxter for Rom24bx (20-Feb-97)
 * deadland.ada.com.tr 9000 (195.142.130.3)
 * Track (Hunt) skill for warriors and thieves.
 */

/*
 * SillyMUD Distribution V1.1b             (c) 1993 SillyMUD Developement
 * See license.doc for distribution terms.   SillyMUD is based on DIKUMUD
 *
 * Modifications by Rip in attempt to port to merc 2.1
 */

/*
 * Modified by Turtle for Merc22 (07-Nov-94)
 *
 * I got this one from ftp.atinc.com:/pub/mud/outgoing/track.merc21.tar.gz.
 * It cointained 5 files: README, hash.c, hash.h, skills.c, and skills.h.
 * I combined the *.c and *.h files in this hunt.c, which should compile
 * without any warnings or errors.
 */

/*
 * Some systems don't have bcopy and bzero functions in their linked libraries.
 * If compilation fails due to missing of either of these functions,
 * define NO_BCOPY or NO_BZERO accordingly.                 -- Turtle 31-Jan-95
 */

//void bcopy(register char *s1,register char *s2,int len);
//void bzero(register char *sp,int len);

struct hash_link
{
  int			key;
  struct hash_link	*next;
  void			*data;
};

struct hash_header
{
  int			rec_size;
  int			table_size;
  int			*keylist, klistsize, klistlen; /* this is really lame,
							  AMAZINGLY lame */
  struct hash_link	**buckets;
};

#define WORLD_SIZE	30000
#define	HASH_KEY(ht,key)((((unsigned int)(key))*17)%(ht)->table_size)



struct hunting_data
{
  char			*name;
  struct char_data	**victim;
};

struct room_q
{
  int		room_nr;
  struct room_q	*next_q;
};

struct nodes
{
  int	visited;
  int	ancestor;
};

#define IS_DIR		(get_room_index(q_head->room_nr)->exit[i])
#define GO_OK		(!IS_SET( IS_DIR->exit_info, EX_CLOSED ))
#define GO_OK_SMARTER	1



#if defined( NO_BCOPY )
void bcopy(register char *s1,register char *s2,int len)
{
  while( len-- ) *(s2++) = *(s1++);
}
#endif

#if defined( NO_BZERO )
void bzero(register char *sp,int len)
{
  while( len-- ) *(sp++) = '\0';
}
#endif



void init_hash_table(struct hash_header	*ht,int rec_size,int table_size)
{
  ht->rec_size	= rec_size;
  ht->table_size= table_size;
  ht->buckets	= (void*)calloc(sizeof(struct hash_link**),table_size);
  ht->keylist	= (void*)malloc(sizeof(ht->keylist)*(ht->klistsize=128));
  ht->klistlen	= 0;
}

void init_world(ROOM_INDEX_DATA *room_db[])
{
  /* zero out the world */
  bzero((char *)room_db,sizeof(ROOM_INDEX_DATA *)*WORLD_SIZE);
}

void destroy_hash_table(struct hash_header *ht,void (*gman)())
{
  int			i;
  struct hash_link	*scan,*temp;

  for(i=0;i<ht->table_size;i++)
    for(scan=ht->buckets[i];scan;)
      {
	temp = scan->next;
	(*gman)(scan->data);
	free(scan);
	scan = temp;
      }
  free(ht->buckets);
  free(ht->keylist);
}

void _hash_enter(struct hash_header *ht,int key,void *data)
{
  /* precondition: there is no entry for <key> yet */
  struct hash_link	*temp;
  int			i;

  temp		= (struct hash_link *)malloc(sizeof(struct hash_link));
  temp->key	= key;
  temp->next	= ht->buckets[HASH_KEY(ht,key)];
  temp->data	= data;
  ht->buckets[HASH_KEY(ht,key)] = temp;
  if(ht->klistlen>=ht->klistsize)
    {
      ht->keylist = (void*)realloc(ht->keylist,sizeof(*ht->keylist)*
				   (ht->klistsize*=2));
    }
  for(i=ht->klistlen;i>=0;i--)
    {
      if(ht->keylist[i-1]<key)
	{
	  ht->keylist[i] = key;
	  break;
	}
      ht->keylist[i] = ht->keylist[i-1];
    }
  ht->klistlen++;
}

ROOM_INDEX_DATA *room_find(ROOM_INDEX_DATA *room_db[],int key)
{
  return((key<WORLD_SIZE&&key>-1)?room_db[key]:0);
}

void *hash_find(struct hash_header *ht,int key)
{
  struct hash_link *scan;

  scan = ht->buckets[HASH_KEY(ht,key)];

  while(scan && scan->key!=key)
    scan = scan->next;

  return scan ? scan->data : NULL;
}

int room_enter(ROOM_INDEX_DATA *rb[],int key,ROOM_INDEX_DATA *rm)
{
  ROOM_INDEX_DATA *temp;

  temp = room_find(rb,key);
  if(temp) return(0);

  rb[key] = rm;
  return(1);
}

int hash_enter(struct hash_header *ht,int key,void *data)
{
  void *temp;

  temp = hash_find(ht,key);
  if(temp) return 0;

  _hash_enter(ht,key,data);
  return 1;
}

ROOM_INDEX_DATA *room_find_or_create(ROOM_INDEX_DATA *rb[],int key)
{
  ROOM_INDEX_DATA *rv;

  rv = room_find(rb,key);
  if(rv) return rv;

  rv = (ROOM_INDEX_DATA *)malloc(sizeof(ROOM_INDEX_DATA));
  rb[key] = rv;

  return rv;
}

void *hash_find_or_create(struct hash_header *ht,int key)
{
  void *rval;

  rval = hash_find(ht, key);
  if(rval) return rval;

  rval = (void*)malloc(ht->rec_size);
  _hash_enter(ht,key,rval);

  return rval;
}

int room_remove(ROOM_INDEX_DATA *rb[],int key)
{
  ROOM_INDEX_DATA *tmp;

  tmp = room_find(rb,key);
  if(tmp)
    {
      rb[key] = 0;
      free(tmp);
    }
  return(0);
}

void *hash_remove(struct hash_header *ht,int key)
{
  struct hash_link **scan;

  scan = ht->buckets+HASH_KEY(ht,key);

  while(*scan && (*scan)->key!=key)
    scan = &(*scan)->next;

  if(*scan)
    {
      int		i;
      struct hash_link	*temp, *aux;

      temp	= (*scan)->data;
      aux	= *scan;
      *scan	= aux->next;
      free(aux);

      for(i=0;i<ht->klistlen;i++)
	if(ht->keylist[i]==key)
	  break;

      if(i<ht->klistlen)
	{
	  bcopy((char *)ht->keylist+i+1,(char *)ht->keylist+i,(ht->klistlen-i)
		*sizeof(*ht->keylist));
	  ht->klistlen--;
	}

      return temp;
    }

  return NULL;
}

void room_iterate(ROOM_INDEX_DATA *rb[],void (*func)(),void *cdata)
{
  register int i;

  for(i=0;i<WORLD_SIZE;i++)
    {
      ROOM_INDEX_DATA *temp;

      temp = room_find(rb,i);
      if(temp) (*func)(i,temp,cdata);
    }
}

void hash_iterate(struct hash_header *ht,void (*func)(),void *cdata)
{
  int i;

  for(i=0;i<ht->klistlen;i++)
    {
      void		*temp;
      register int	key;

      key = ht->keylist[i];
      temp = hash_find(ht,key);
      (*func)(key,temp,cdata);
      if(ht->keylist[i]!=key) /* They must have deleted this room */
	i--;		      /* Hit this slot again. */
    }
}



int exit_ok( EXIT_DATA *pexit )
{
  ROOM_INDEX_DATA *to_room;

  if ( ( pexit == NULL )
  ||   ( to_room = pexit->u1.to_room ) == NULL )
    return 0;

  return 1;
}

void donothing()
{
  return;
}

int find_path( int in_room_vnum, int out_room_vnum, CHAR_DATA *ch,
	       int depth, int in_zone )
{
  struct room_q		*tmp_q, *q_head, *q_tail;
  struct hash_header	x_room;
  int			i, tmp_room, count=0, thru_doors;
  ROOM_INDEX_DATA	*herep;
  ROOM_INDEX_DATA	*startp;
  EXIT_DATA		*exitp;

  if ( depth <0 )
    {
      thru_doors = TRUE;
      depth = -depth;
    }
  else
    {
      thru_doors = FALSE;
    }

  startp = get_room_index( in_room_vnum );

  init_hash_table( &x_room, sizeof(int), 2048 );
  hash_enter( &x_room, in_room_vnum, (void *) - 1 );

  /* initialize queue */
  q_head = (struct room_q *) malloc(sizeof(struct room_q));
  q_tail = q_head;
  q_tail->room_nr = in_room_vnum;
  q_tail->next_q = 0;

  while(q_head)
    {
      herep = get_room_index( q_head->room_nr );
      /* for each room test all directions */
      if( herep->area == startp->area || !in_zone )
	{
	  /* only look in this zone...
	     saves cpu time and  makes world safer for players  */
	  for( i = 0; i <= 5; i++ )
	    {
	      exitp = herep->exit[i];
	      if( exit_ok(exitp) && ( thru_doors ? GO_OK_SMARTER : GO_OK ) )
		{
		  /* next room */
		  tmp_room = herep->exit[i]->u1.to_room->vnum;
		  if( tmp_room != out_room_vnum )
		    {
		      /* shall we add room to queue ?
			 count determines total breadth and depth */
		      if( !hash_find( &x_room, tmp_room )
			 && ( count < depth ) )
			/* && !IS_SET( RM_FLAGS(tmp_room), DEATH ) ) */
			{
			  count++;
			  /* mark room as visted and put on queue */

			  tmp_q = (struct room_q *)
			    malloc(sizeof(struct room_q));
			  tmp_q->room_nr = tmp_room;
			  tmp_q->next_q = 0;
			  q_tail->next_q = tmp_q;
			  q_tail = tmp_q;

			  /* ancestor for first layer is the direction */
			  hash_enter( &x_room, tmp_room,
				     ((int)hash_find(&x_room,q_head->room_nr)
				      == -1) ? (void*)(i+1)
				     : hash_find(&x_room,q_head->room_nr));
			}
		    }
		  else
		    {
		      /* have reached our goal so free queue */
		      tmp_room = q_head->room_nr;
		      for(;q_head;q_head = tmp_q)
			{
			  tmp_q = q_head->next_q;
			  free(q_head);
			}
		      /* return direction if first layer */
		      if ((int)hash_find(&x_room,tmp_room)==-1)
			{
			  if (x_room.buckets)
			    {
			      /* junk left over from a previous track */
			      destroy_hash_table(&x_room, donothing);
			    }
			  return(i);
			}
		      else
			{
			  /* else return the ancestor */
			  int i;

			  i = (int)hash_find(&x_room,tmp_room);
			  if (x_room.buckets)
			    {
			      /* junk left over from a previous track */
			      destroy_hash_table(&x_room, donothing);
			    }
			  return( -1+i);
			}
		    }
		}
	    }
	}

      /* free queue head and point to next entry */
      tmp_q = q_head->next_q;
      free(q_head);
      q_head = tmp_q;
    }

  /* couldn't find path */
  if( x_room.buckets )
    {
      /* junk left over from a previous track */
      destroy_hash_table( &x_room, donothing );
    }
  return -1;
}



void do_acadian_track( CHAR_DATA *ch, char *argument )
{
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_STRING_LENGTH];
  CHAR_DATA *victim;
  int direction;
  bool fArea;

  one_argument( argument, arg );

  if (IS_NPC(ch)
  ||  ch->level < skill_table[gsn_track].skill_level[ch->class])
  {
	send_to_char("Huh?\n\r",ch);
	return;
  }

  if( arg[0] == '\0' )
    {
      send_to_char( "Whom are you trying to track?\n\r", ch );
      return;
    }


  victim = get_char_world( ch, arg );
  fArea = ( get_trust(ch) < MAX_LEVEL );


  if( victim == NULL )
    {
      send_to_char("They aren't in the land.\n\r", ch );
      return;
    }

  if( ch->in_room == victim->in_room )
    {
      act( "$N is here!", ch, NULL, victim, TO_CHAR );
      return;
    }


  act( "$n carefully sniffs the air.", ch, NULL, NULL, TO_ROOM );
  //WAIT_STATE( ch, skill_table[gsn_track].beats );
  direction = find_path( ch->in_room->vnum, victim->in_room->vnum,
			ch, -40000, fArea );

  if( direction == -1 )
    {
      act( "You couldn't find a path to $N from here.",
	  ch, NULL, victim, TO_CHAR );
      return;
    }

  if( direction < 0 || direction > 5 )
    {
      send_to_char( "Hmm... Something seems to be wrong.\n\r", ch );
      return;
    }

  /*
   * Give a random direction if the player misses the die roll.
   */
  /*if( ( IS_NPC (ch) && number_percent () > 75)
     || (!IS_NPC (ch) && number_percent () >
	 ch->pcdata->learned[gsn_track] ) )
    {
      do
	{
	  direction = number_door();
	}
      while( ( ch->in_room->exit[direction] == NULL )
	    || ( ch->in_room->exit[direction]->u1.to_room == NULL) );
    }*/

  /*
   * Display the results of the search.
   */
  sprintf( buf, "$N is %s from here.", dir_name[direction] );
  act( buf, ch, NULL, victim, TO_CHAR );
  //check_improve(ch,gsn_track,TRUE,1);
  return;
}

DECLARE_DO_FUN(do_look);

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

  if (corpse->pIndexData->vnum != OBJ_VNUM_CORPSE_PC)
  {
	  send_to_char("You can't siphon energy from that.\n\r", ch );
	  return;
  }

  if (strcmp(corpse->talked,ch->name))
  {
	  send_to_char("You can only drain your own kills.\n\r", ch );
	  return;
  }

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

void do_shadowform( CHAR_DATA *ch, char *argument )
{
	AFFECT_DATA af;
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_STRING_LENGTH];
	int chance;

	argument = one_argument(argument,arg);

	if ((chance = get_skill(ch,gsn_cloak_form)) == 0
	|| ch->level < skill_table[gsn_cloak_form].skill_level[ch->class])
	{
		send_to_char("Huh?\n\r",ch);
	    return;
	}

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
		sprintf(buf, "a cloaked rider");
		ch->name=str_dup(buf);
	  }
	  af.location = APPLY_HIT;
	  af.modifier = ch->level*2;
	  affect_to_char(ch,&af);
	  af.location = APPLY_AC;
	  af.modifier = -ch->level;
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

void do_steed( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *steed;
	CHAR_DATA *mob;
	AFFECT_DATA af;
	char arg[MAX_STRING_LENGTH];
	int chance, i;

	one_argument(argument,arg);

	if ((chance = get_skill(ch,gsn_steed)) == 0
	|| ch->level < skill_table[gsn_steed].skill_level[ch->class])
	{
		send_to_char("Huh?\n\r",ch);
	    return;
	}

	if (is_affected(ch, gsn_steed))
	{
		send_to_char("You can't summon the power to call forth another mount.\n\r", ch);
		return;
	}

	for (mob = char_list; mob != NULL; mob = mob->next)
	{
		if (IS_NPC(mob) && IS_AFFECTED(mob,AFF_CHARM)
	    && (mob->master == ch)
	    && ( (mob->pIndexData->vnum == MOB_VNUM_BLACK_STEED)
	    || (mob->pIndexData->vnum == MOB_VNUM_RED_STEED)
	    || (mob->pIndexData->vnum == MOB_VNUM_BLUE_STEED)
	    || (mob->pIndexData->vnum == MOB_VNUM_GREEN_STEED) ) )
	    break;
    }

    if (mob != NULL)
    {
		send_to_char("You've already got a mount.\n\r", ch);
		return;
	}

	if (!strcmp(arg, "black" ))
	{
		steed = create_mobile(get_mob_index(MOB_VNUM_BLACK_STEED) );

		for (i = 0; i < 4; i++)
		{
			steed->armor[i] -= (3*ch->level);
	   	}

	   	steed->max_hit = ch->max_hit*2;
		steed->hit 	   = ch->max_hit*2;
		steed->damroll = (ch->level/3);
		steed->hitroll = (ch->level/3);

		char_to_room(steed,ch->in_room);
		steed->level = ch->level;
		act("A thunderous roar is heard as a massive dragon steed lands beside $n!"
		,ch,0,steed,TO_ROOM);
		act("A massive dragon lands beside you!",ch,0,steed,TO_CHAR);
		ch->mana -= 50;
		SET_BIT(steed->affected_by,AFF_CHARM);
        steed->leader = ch;
	    steed->master = ch;

		init_affect(&af);
		af.where 	 = TO_AFFECTS;
		af.type 	 = gsn_steed;
		af.aftype    = AFT_SKILL;
		af.level 	 = ch->level;
		af.duration  = 24;
		af.location  = 0;
		af.modifier  = 0;
		af.bitvector = 0;
		affect_to_char(ch,&af);
		do_mount(ch,arg);
    	return;
	}
	else if (!strcmp(arg, "red" ))
	{
		steed = create_mobile(get_mob_index(MOB_VNUM_RED_STEED) );

		for (i = 0; i < 4; i++)
		{
			steed->armor[i] -= (3*ch->level);
	   	}

	   	steed->max_hit = ch->max_hit;
		steed->hit 	   = ch->max_hit;
		steed->damroll = (ch->level/1.5);
		steed->hitroll = (ch->level/1.5);

		char_to_room(steed,ch->in_room);
		steed->level = ch->level;
		act("A thunderous roar is heard as a massive dragon steed lands beside $n!"
		,ch,0,steed,TO_ROOM);
		act("A massive dragon lands beside you!",ch,0,steed,TO_CHAR);
		ch->mana -= 50;
		SET_BIT(steed->affected_by,AFF_CHARM);
        steed->leader = ch;
	    steed->master = ch;

		init_affect(&af);
		af.where 	 = TO_AFFECTS;
		af.type 	 = gsn_steed;
		af.aftype    = AFT_SKILL;
		af.level 	 = ch->level;
		af.duration  = 24;
		af.location  = 0;
		af.modifier  = 0;
		af.bitvector = 0;
		affect_to_char(ch,&af);
		do_mount(ch,arg);
    	return;
	}
	else if (!strcmp(arg, "blue" ))
	{
		steed = create_mobile(get_mob_index(MOB_VNUM_BLUE_STEED) );

		for (i = 0; i < 4; i++)
		{
			steed->armor[i] -= (4*ch->level);
	   	}

	   	steed->max_hit = ch->max_hit*1.5;
		steed->hit 	   = ch->max_hit*1.5;
		steed->damroll = (20 + ch->level/4);
		steed->hitroll = (20 + ch->level/4);

		char_to_room(steed,ch->in_room);
		steed->level = ch->level;
		act("A thunderous roar is heard as a massive dragon steed lands beside $n!"
		,ch,0,steed,TO_ROOM);
		act("A massive dragon lands beside you!",ch,0,steed,TO_CHAR);
		ch->mana -= 50;
		SET_BIT(steed->affected_by,AFF_CHARM);
        steed->leader = ch;
	    steed->master = ch;

		init_affect(&af);
		af.where 	 = TO_AFFECTS;
		af.type 	 = gsn_steed;
		af.aftype    = AFT_SKILL;
		af.level 	 = ch->level;
		af.duration  = 24;
		af.location  = 0;
		af.modifier  = 0;
		af.bitvector = 0;
		affect_to_char(ch,&af);
		do_mount(ch,arg);
    	return;
	}
	else if (!strcmp(arg, "green" ))
	{
		steed = create_mobile(get_mob_index(MOB_VNUM_GREEN_STEED) );

		for (i = 0; i < 4; i++)
		{
			steed->armor[i] -= (2*ch->level);
	   	}

	   	steed->max_hit = ch->max_hit*1.5;
		steed->hit 	   = ch->max_hit*1.5;
		steed->damroll = (ch->level/2);
		steed->hitroll = (ch->level/2);

		char_to_room(steed,ch->in_room);
		steed->level = ch->level;
		act("A thunderous roar is heard as a massive dragon steed lands beside $n!"
		,ch,0,steed,TO_ROOM);
		act("A massive dragon lands beside you!",ch,0,steed,TO_CHAR);
		ch->mana -= 50;
		SET_BIT(steed->affected_by,AFF_CHARM);
        steed->leader = ch;
	    steed->master = ch;

		init_affect(&af);
		af.where 	 = TO_AFFECTS;
		af.type 	 = gsn_steed;
		af.aftype    = AFT_SKILL;
		af.level 	 = ch->level;
		af.duration  = 24;
		af.location  = 0;
		af.modifier  = 0;
		af.bitvector = 0;
		affect_to_char(ch,&af);
		do_mount(ch,arg);
    	return;
	}else{
		send_to_char("You must choose either: [Blue, Black, Red or Green]\n\r", ch);
		return;
	}
}

void do_mount( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *mob;
	char arg[MAX_STRING_LENGTH];

	one_argument(argument, arg);

	if (arg == '\0')
	{
		send_to_char("Mount what?\n\r", ch );
		return;
	}

	if (IS_SET(ch->comm, COMM_MOUNTED))
	{
		send_to_char("Your already riding a mount.\n\r", ch );
		return;
	}

	if ( ( mob = get_char_room( ch, arg) ) == NULL )
	{
		send_to_char("You don't see that here.\n\r", ch );
		return;
	}

	if ( ( mob = get_char_room( ch, arg ) ) != NULL )
	{
		if (IS_NPC(mob) && IS_AFFECTED(mob,AFF_CHARM)
		&& (mob->master == ch)
		&& ( (mob->pIndexData->vnum == MOB_VNUM_BLACK_STEED)
		|| (mob->pIndexData->vnum == MOB_VNUM_RED_STEED)
		|| (mob->pIndexData->vnum == MOB_VNUM_BLUE_STEED)
		|| (mob->pIndexData->vnum == MOB_VNUM_GREEN_STEED) ) )
		{
			send_to_char("You gracefully sweep up onto your dragonmount.\n\r", ch);
			act("$n gracefully sweeps up onto $N's back and settles into the saddle."
			,ch,0,mob,TO_ROOM);
			SET_BIT(ch->comm, COMM_MOUNTED);
			return;
		}
		else
		{
			send_to_char("You can't mount that.\n\r", ch );
			return;
		}
	}
}

void do_dismount( CHAR_DATA *ch, char *argument )
{

	if (IS_SET(ch->comm, COMM_MOUNTED))
	{
		send_to_char("You effortlessly sweep down off your mount.\n\r", ch);
		act("$n effortlessly sweeps down off $s mount.",ch,0,0,TO_ROOM);
		REMOVE_BIT(ch->comm, COMM_MOUNTED);
		return;
	}
	else
	{
		send_to_char("You aren't mounted.\n\r", ch);
		return;
	}
}

void do_target( CHAR_DATA *ch, char *argument )
{
  	CHAR_DATA *victim;
  	AFFECT_DATA af;
  	char arg[MAX_STRING_LENGTH];


  	one_argument(argument, arg);

  	if (ch->cabal != CABAL_ACADIAN)
  	{
		send_to_char("Huh?\n\r", ch );
		return;
  	}

  	if (!strcmp(arg, "none"))
	{
		if (ch->pcdata->target == '\0')
		{
			send_to_char("You have no target to relase.\n\r", ch );
			return;
		}

		ch->pcdata->target = NULL;
		affect_strip(ch, gsn_target);
		send_to_char("You release your target.\n\r", ch );
		return;
	}



  	if ( (victim = get_char_world( ch, arg ) ) == NULL)
  	{
  		send_to_char("They aren't in the realm.\n\r",ch);
        return;
  	}

  	if (IS_NPC(victim))
  	{
		send_to_char("You can't target that!\n\r", ch );
		return;
	}

	if (IS_NPC(ch))
	{
		send_to_char("You can't do that!\n\r", ch );
		return;
	}

	if (victim == ch)
	{
		send_to_char("You can't target yourself.\n\r", ch );
		return;
	}

	if (victim->pcdata->bounty == 0)
	{
		send_to_char("They aren't marked.\n\r", ch );
		return;
	}


  	if (ch->pcdata->target == '\0')
  	{
		ch->pcdata->target = str_dup(victim->name);
  		send_to_char("You are now focused on your target.\n\r", ch );

  		init_affect(&af);
		af.where 	 = TO_AFFECTS;
		af.type 	 = gsn_target;
		af.aftype    = AFT_SKILL;
		af.level 	 = ch->level;
		af.duration  = 10;
		af.location  = 0;
		af.modifier  = 0;
		af.bitvector = 0;
		affect_to_char(ch,&af);
  		return;
	}
	else
	{
		ch->pcdata->target = str_dup(victim->name);
		send_to_char("You have refocused your target.\n\r", ch );
		affect_strip(ch, gsn_target);

		init_affect(&af);
		af.where 	 = TO_AFFECTS;
		af.type 	 = gsn_target;
		af.aftype    = AFT_SKILL;
		af.level 	 = ch->level;
		af.duration  = 10;
		af.location  = 0;
		af.modifier  = 0;
		af.bitvector = 0;
		affect_to_char(ch,&af);
  		return;
	}

}


void do_swoop( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	CHAR_DATA *mob;
	char arg[MAX_STRING_LENGTH];
	int direction;
	int i;
	bool fArea;

	one_argument(argument, arg);




	if ( (victim = get_char_world( ch,arg )) == NULL)
	{
		send_to_char("They aren't here.\n\r",ch);
		return;
    }

    if (victim == ch)
	{
		send_to_char("That would be foolish, wouldn't it.\n\r",ch);
	    return;
	}

	if (is_safe(ch,victim))
    return;


    if (!IS_SET(ch->comm, COMM_MOUNTED))
	{
		send_to_char("You need to be mounted to swoop.\n\r", ch );
		return;
	}


	if( ch->in_room == victim->in_room )
	    {
	      act( "$N is here!", ch, NULL, victim, TO_CHAR );
	      return;
	    }

	fArea = ( get_trust(ch) < MAX_LEVEL );

	send_to_char("You rear your mount and swoop toward your victim.\n\r", ch );
	SET_BIT(ch->comm,COMM_BRIEF);

	for ( i = 0; i < 20; i++ )
	{
		direction = find_path( ch->in_room->vnum, victim->in_room->vnum, ch, -40000, fArea );

		if (number_percent() > ch->pcdata->learned[gsn_swoop])
		{
			send_to_char("You failed to swoop to them.\n\r", ch );
			return;
		}

		if( direction == -1 )
		{
			act( "You failed to swoop to them from here.",
		    ch, NULL, victim, TO_CHAR );
		    return;
		}

		if( direction < 0 || direction > 5 )
		{
			send_to_char( "Hmm... Something seems to be wrong.\n\r", ch );
		    return;
		}


		if ( ( mob = get_char_room( ch, arg ) ) != NULL )
		{
			if (IS_NPC(mob) && IS_AFFECTED(mob,AFF_CHARM)
			&& (mob->master == ch) )
			{
				move_char(mob, direction, TRUE );
				move_char(ch, direction, TRUE );
				check_improve(ch,gsn_swoop,TRUE,1);
			}
		}
		else
		{
			move_char(ch, direction, TRUE );
			check_improve(ch,gsn_swoop,TRUE,1);
		}
	}

	WAIT_STATE(ch,PULSE_VIOLENCE*1);
	check_improve(ch,gsn_swoop,FALSE,2);
	do_look(ch,"auto");
	REMOVE_BIT(ch->comm,COMM_BRIEF);
	return;

}
























