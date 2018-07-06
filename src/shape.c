#include "include.h"

/*
 * Local functions.
 */

int get_shifted args ((CHAR_DATA *ch));
bool isShifted args ((CHAR_DATA *ch));
int form_lookup args((char *argument));
CHAR_DATA *get_a_victim args((CHAR_DATA *ch, char *argument));
void one_hit args ((CHAR_DATA * ch, CHAR_DATA * victim, int dt));
char *format_obj_to_char args ((OBJ_DATA * obj, CHAR_DATA * ch, bool fShort));
void show_list_to_char args ((OBJ_DATA * list, CHAR_DATA * ch,
                              bool fShort, bool fShowNothing));
void show_char_to_char_0 args ((CHAR_DATA * victim, CHAR_DATA * ch));
void show_char_to_char_1 args ((CHAR_DATA * victim, CHAR_DATA * ch));
void show_char_to_char args ((CHAR_DATA * list, CHAR_DATA * ch));


const struct form_type form_table [MAX_FORM]	=
{
	{
	"reserved", /* form name, e.g. fox */
	"reserved", /* short description, e.g. a fox */
	"reserved\n\r", /* long description, e.g. a fox walks stealthily here */
	{25, 25, 25, 25, 25, 25}, /* stats, e.g str, dex, int, wis con, chr */
	{-800, -800, -800, -500}, /* AC */
	25, /* dam modification i.e. 100 is normal dam, 50 is half dam, 150 is 1.5 dam. So, the higher the number the more damage taken (by the person in the form) */
	1, /* max attacks per round */
	"pound", /* attack type (must be on the attacke table) */
	50, //parry mod (adds to parry percentage, so higher the number the better the chance to parry) */
	100, //dice_num the 5 in 5d4+35
	100, //dice type the 4 in 5d4+35
	100, //dice bonus the 35 in 5d4+35
	200, //hitroll
	200, //damroll
	-100, //svs
	0,//immunity flags
	0,//resistance flags
	0,//vuln flags
	0,//perm affects
	FORM_QUEST, //form type
	FORM_UTILITY, //form foci
	"A huge roar fills your ears.\n\r", //form yell
	"You see a large hippopotamus with wings.\n\r", /*form descrition */
	0, //special progs
	"", //skill 1 i.e. bite
	"", //skill 2
	"" //skill 3
	},
		
	{
	"bulette", /* form name, e.g. fox */
	"an armored land-shark", /* short description, e.g. a fox */
	"A gargantuan, scaly bulette thrashes its tail furiously here, eyeing potential meals.\n\r", /* long description, e.g. a fox walks stealthily here */
	{24, 14, 15, 11, 25, 13}, /* stats, e.g str, dex, int, wis con, chr */
	{-500, -500, -500, -500}, /* AC */
	10, /* dam modification i.e. 100 is normal dam, 50 is half dam, 150 is 1.5 dam. So, the higher the number the more damage taken (by the person in the form) */
	1, /* max attacks per round */
	"chomp", /* attack type (must be on the attacke table) */
	40, //parry mod (adds to parry percentage, so higher the number the better the chance to parry) */
	8, //dice_num the 5 in 5d4+35
	12, //dice type the 4 in 5d4+35
	30, //dice bonus the 35 in 5d4+35
	55, //hitroll
	110, //damroll
	0, //svs
	0,//immunity flags
	0,//resistance flags
	0,//vuln flags
	0,//perm affects
	FORM_QUEST, //form type
	FORM_QUEST, //form foci
	"You hear the violent snapping of teeth.\n\r", //form yell
	"A deafening crack accompanies the movement of this massive beast's whiplike tail\n\ras it thrashes its head around violently.  Atop its thickly corded neck, this\n\rland shark's head is tiny, save for its massive jaws and multiple rows of teeth.\n\rScales adorn its back and sides, protecting it from the elements and from most\n\rglancing blows.  The bulette's underside is softer, lighter... but difficult to\n\rreach beyond its wildly thrashing claws.  Its tiny red eyes swivel about in their\n\rsockets, surveying the area as if possessed by a need for flesh to rend and bones\n\rto splinter.  A shiver passes up and down your spine in half a heartbeat as its\n\rhungry gaze fixes itself upon you...\n\r", /*form descrition */
	0, //special progs
	"", //skill 1 i.e. bite
	"", //skill 2
	"" //skill 3
	},

	{
	"ram",
	"a savage ram",
	"A grizzled ram with huge, tightly-curled horns snorts ferociously at nothing in particular.\n\r",
	{25, 17, 16, 15, 21, 14},
	{-100, -100, -100, -100},
	100,
	9,
	"charge",
	30, //parry
	12, //dice_num
	12,
	50,
	98, //hit
	98, //dam
	-40, //svs
	0,
	0,
	0,
	0,
	FORM_QUEST,
	FORM_QUEST,
	"The loud snort of a ram echoes somewhere nearby.\n\r",
	"Awkwardly clamoring over the stones and dirt betwixt you and he, the ram before\n\ryou charges nonetheless at its maximum speed.  Its hooves carve a strange,\n\rlumbering pattern in the ground, dashing bits of wood and sedimentary rock to\n\rpieces.  You strain to get a look at its face, covered in stringy white fur, but\n\rit has lowered its head for the moment.  Suddenly, it becomes apparent that this\n\renraged animal is charging.  Charging who or what, you do not know.  Perhaps you\n\rwill meet the blunt force of its tightly curled horns next.\n\r",
	0,
	"",
	"",
	""
	},

        {
        "oak tree", /* form name, e.g. fox */
        "a majestic oak tree", /* short description, e.g. a fox */
        "A gigantic tree stretches over your head, its branches looming over you.\n\r",
        {25, 14, 12, 9, 23, 13}, /* stats, e.g str, int, wis, dex, con, chr */
        {-300, -350, -100, -100}, /* AC */
        100, /* dam modification 100 is normal dam, 50 is half dam, 150 is 1.5 dam. */
        6, /* max attacks per round */
        "smash", /* attack type (must be on the attack table) */
        50, //parry mod (parry percentage, higher the number the better the chance to parry) */
        20, //dice_num the 5 in 5d4+35
        10, //dice type the 4 in 5d4+35
        40, //dice bonus the 35 in 5d4+35
        95, //hitroll
        95, //damroll
        -10, //svs
        0,//immunity flags
        RES_BASH,//resistance flags
        VULN_SLASH,//vuln flags
        0,//perm affects
        FORM_PLANT, //form type
        FORM_OFFENSE, //form foci
        "You heard the cracking of branches in the distance.\n\r", //form yell
        "", /*form descrition*/
        0, //special progs
        "", //skill 1 i.e. bite
        "", //skill 2
        "" //skill 3
        },

	{
	"lion", /* form name, e.g. fox */
	"a ferocious lion", /* short description, e.g. a fox */
	"A massive, yellow maned lion whips around and regards you dispassionately before turning away.\n\r", /* long description, e.g. a fox walks stealthily here */
	{23, 17, 18, 16, 20, 13}, /* stats, e.g str, in, dex, com, chr */
	{-170, -170, -170, -170}, /* AC */
	125, /* dam modification i.e. 100 is normal dam, 50 is half dam, 150 is 1.5 dam. So, the higher the number the more damage taken (by the person in the form) */
	5, /* max attacks per round */
	"claw", /* attack type (must be on the attack table) */
	30, //parry mod (adds to parry percentage, so higher the number the better the chance to parry) */
	20, //dice_num the 5 in 5d4+35
	12, //dice type the 4 in 5d4+35
	30, //dice bonus the 35 in 5d4+35
	40, //hitroll
	51, //damroll
	-30, //svs
	0,//immunity flags
	0,//resistance flags
	0,//vuln flags
	0,//perm affects
	FORM_ANML, //form type
	FORM_OFFENSE, //form foci
	"The loud roar of a lion echoes nearby.\n\r", //form yell
	"The low rumble of a lion's snarl alerts you to its presence before anything else.\n\rThe noise is born deep in the lion's chest cavity, where it is slowly cooked to a\n\rrolling boil.  From there, it travels upward through the huge feline's body\n\rtoward its throat, lingering momentarily in the vicinity of arching shoulders and\n\rpowerful neck, until finally erupting into a full-fledged roar.  The king of the\n\rjungle tosses its mane, and its tail darts back and forth as it bares its massive\n\rteeth.  Churning at the dirt with its huge paws, the lion begins to circle... its\n\rgrowl never ceasing, its dagger-like fangs never retreating once more to within\n\rits gaping maw.  You would do well to avoid angering this monstrous beast.\n\r", /*form descrition*/ 
	0, //special progs
	"rake", //skill 1 i.e. bite
	"", //skill 2
	"" //skill 3
	},

	{
	"armadillo", /* form name, e.g. fox */
	"an armadillo", /* short description, e.g. a fox */
	"A bony plated armadillo crawls slowly by.\n\r", /* long description, e.g. a fox walks stealthily here */
	{20, 14, 15, 11, 24, 13}, /* stats, e.g str, dex, int, wis con, chr */
	{-500, -500, -500, -500}, /* AC */
	10, /* dam modification i.e. 100 is normal dam, 50 is half dam, 150 is 1.5 dam. So, the higher the number the more damage taken (by the person in the form) */
	1, /* max attacks per round */
	"claw", /* attack type (must be on the attacke table) */
	30, //parry mod (adds to parry percentage, so higher the number the better the chance to parry) */
	1, //dice_num the 5 in 5d4+35
	12, //dice type the 4 in 5d4+35
	1, //dice bonus the 35 in 5d4+35
	20, //hitroll
	10, //damroll
	-50, //svs
	0,//immunity flags
	DAM_SLASH + DAM_PIERCE + DAM_BASH,//resistance flags
	0,//vuln flags
	0,//perm affects
	FORM_ANML, //form type
	FORM_DEFENSE, //form foci
	"A loud roar of an armadillo sounds throughout.\n\r", //form yell
	"", /*form descrition */
	0, //special progs
	"", //skill 1 i.e. bite
	"", //skill 2
	"" //skill 3
	},

        {
        "poison sumac", /* form name, e.g. fox */
        "a large poison sumac", /* short description, e.g. a fox */
        "A tree stands here covered in masses of a green, sickly vine.\n\r", 
        {20, 18, 17, 10, 21, 13}, /* stats, e.g str, dex, int, wis con, chr */
        {-300, -150, -50, -420}, /* AC */
        100, /* dam modification i.e. 100 is normal dam, 50 is half dam, ect */
        4, /* max attacks per round */
        "bite", /* attack type (must be on the attacke table) */
        15, //parry mod (adds to parry percentage, so higher the number the better the chance to parry) */
        12, //dice_num the 5 in 5d4+35
        8, //dice type the 4 in 5d4+35
        40, //dice bonus the 35 in 5d4+35
        35, //hitroll
        35, //damroll
        -25, //svs
        0,//immunity flags
        RES_BASH,//resistance flags
        VULN_SLASH,//vuln flags
        0,//perm affects
        FORM_PLANT, //form type
        FORM_OFFENSE, //form foci
        "You hear a violent swishing of leaves.\n\r", //form yell
        "Sumac desc here.\n\r", /*form descrition */
        0, //special progs
        "poison cloud", //skill 1 i.e. bite
        "", //skill 2
        "" //skill 3
        },

        {
        "yeti", /* form name, e.g. fox */
        "a muscular yeti", /* short description, e.g. a fox */
        "An enormous, hairy beast stands here, his muscular body caked with snow.\n\r", 
        {25, 17, 16, 16, 22, 13}, /* stats, e.g str, dex, int, wis con, chr */
        {-400, -200, -300, -400}, /* AC */
        95, /* dam modification i.e. 100 is normal dam, 50 is half dam, 150 is 1.5 dam. */
        1, /* max attacks per round */
        "crush", /* attack type (must be on the attack table) */
        60, //parry mod (adds to parry percentage, so higher the number the better the chance to parry) */
        26, //dice_num the 5 in 5d4+35
        10, //dice type the 4 in 5d4+35
        40, //dice bonus the 35 in 5d4+35
        60, //hitroll
        60, //damroll
        0, //svs
        IMM_COLD,//immunity flags
        RES_BASH,//resistance flags
        VULN_FIRE + VULN_ENERGY,//vuln flags
        0,//perm affects
        FORM_MYTH_ANIMAL, //form type
        FORM_OFFENSE, //form foci
        "You hear the unmistakable sound of a yeti being attacked.\n\r", //form yell
        "", /*form descrition*/
        0, //special progs
        "", //skill 1 i.e. bite
        "", //skill 2
        "" //skill 3
        },
	
        {
        "wolverine", /* form name, e.g. fox */
        "a wolverine", /* short description, e.g. a fox */
        "A vicious wolverine scans the area with a bloodthirsty look.\n\r",
        {23, 8, 8, 18, 21, 13}, /* stats, e.g str, int, wis, dex, con, chr */
        {-100, -100, -100, -100}, /* AC */
        100, /* dam modification 100 is normal dam, 50 is half dam, 150 is 1.5 dam. */
        6, /* max attacks per round */
        "bite", /* attack type (must be on the attack table) */
        100, //parry mod (parry percentage, higher the number the better the chance to parry) */
        12, //dice_num the 5 in 5d4+35
        9, //dice type the 4 in 5d4+35
        40, //dice bonus the 35 in 5d4+35
        20, //hitroll
        80, //damroll
        -30, //svs
        0,//immunity flags
        0,//resistance flags
        0,//vuln flags
        0,//perm affects
        FORM_ANML, //form type
        FORM_UTILITY, //form foci
        "The cry of a wolverine echos the area.\n\r", //form yell
        "Write a wolverine desc.\n\r", /*form descrition*/
        0, //special progs
        "", //skill 1 i.e. bite
        "", //skill 2
        "" //skill 3
        },

        {
        "hydra", /* form name, e.g. fox */
        "a two-headed hydra", /* short description, e.g. a fox */
        "A snake-like creature waves its two heads, one fiery red, the other an icy blue, at you.\n\r",
        {22, 19, 17, 17, 20, 10}, /* stats, e.g str, dex, int, wis con, chr */
        {-200, -210, -210, -333}, /* AC */
        100, /* dam modification i.e. 100 is normal dam, 50 is half dam, ect */
        2, /* max attacks per round */
        "bite", /* attack type (must be on the attacke table) */
        0, //parry mod (adds to parry percentage, so higher the number the better the chance to parry) */
        20, //dice_num the 5 in 5d4+35
        4, //dice type the 4 in 5d4+35
        15, //dice bonus the 35 in 5d4+35
        55, //hitroll
        55, //damroll
        -10, //svs
        0,//immunity flags
        RES_COLD + RES_FIRE,//resistance flags
        0,//vuln flags
        0,//perm affects
        FORM_MYTH_ANIMAL, //form type
        FORM_OFFENSE, //form foci
        "You hear two seperate screeches in the distance.\n\r", //form yell
        "", /*form descrition */
        0, //special progs
        "", //skill 1 i.e. bite
        "", //skill 2
        "" //skill 3
        },
        
        {
        "horse",
        "a glowing white mare",
        "A silvery mare prances around scattering starlight.\n\r",
        { 25, 25, 25, 25, 25, 25 },
        { -500, -500, -500, -500 },
        200,
        12,
        "kick",
        200,
        60,
        20,
        80,
        200,
        200,
        -80,
        0,
        0,
        0,
        0,
        FORM_ANML,
        FORM_OFFENSE,
        "You hear the white mare whines.\n\r",
        "The sweet smell of white roses caresses your senses as a wave of moonlight dust rolls around you. Swirling colors of the Heavens weave together to bring forth the form of a silvery white mare. The mystical creature catches your eye and pulls you inside her world of magic. She has come all the way from Heaven to see you, don't make her send you to Hell!\n\r",
	0,
	"",
	"",
	""
	}
};

bool knows_skill(CHAR_DATA *ch, char *argument)
{
	if(!isShifted(ch))
	{
		send_to_char("Huh?\n\r",ch);
		return FALSE;
	}
	if(!str_cmp(form_table[ch->pcdata->shifted].spec_one,argument))
		return TRUE;
        if(!str_cmp(form_table[ch->pcdata->shifted].spec_two,argument))
                return TRUE;
        if(!str_cmp(form_table[ch->pcdata->shifted].spec_three,argument))
                return TRUE;
        send_to_char("Huh?\n\r",ch);
        return FALSE;

}

void cast_shapeshift(int sn, int level, CHAR_DATA * ch, void *vo, int target)
{
		do_shapeshift(ch, (char *)vo);
}

void do_shapeshift (CHAR_DATA *ch, char *argument)
{
	int i;
	int form = -1;

	if (argument[0] == '\0')
	{
		send_to_char("Shapeshift into what?\n\r", ch);
		do_forms(ch,"");
		return;
	}
	if((form = form_lookup(argument))==-1 || IS_NPC(ch))
		return send_to_char("You don't know any forms of that name.\n\r",ch);
	if(ch->pcdata->forms[form]<75)
		return send_to_char("You don't know any forms of that name.\n\r",ch);

	if( !isShifted(ch) ) {
	ch->pcdata->old	=	new_char();

	ch->pcdata->old->name = str_dup(ch->original_name);
	ch->pcdata->old->short_descr = str_dup(ch->short_descr);
	ch->pcdata->old->long_descr = str_dup(ch->long_descr);
	ch->pcdata->old->description = str_dup(ch->description);
	ch->pcdata->old->carry_number = ch->carry_number;
	ch->pcdata->old->carry_weight = ch->carry_weight;
	ch->pcdata->old->saving_throw = ch->saving_throw;
	for(i=0; i < MAX_STATS; i++)
		ch->pcdata->old->perm_stat[i] = ch->perm_stat[i];
	for(i=0; i <= 3; i++)
		ch->pcdata->old->armor[i] = ch->armor[i];
	ch->pcdata->old->dam_reduc = ch->dam_reduc;
	ch->pcdata->old->carrying = ch->carrying;
	ch->pcdata->old->carry_weight = ch->carry_weight;
	ch->pcdata->old->carry_number = ch->carry_number;
	ch->pcdata->old->saving_throw = ch->saving_throw;
	ch->pcdata->old->imm_flags = ch->imm_flags;
	ch->pcdata->old->res_flags = ch->res_flags;
	ch->pcdata->old->vuln_flags = ch->vuln_flags;
	ch->pcdata->old->damroll = ch->damroll;
	ch->pcdata->old->hitroll = ch->hitroll;
	ch->pcdata->old->affected_by = ch->affected_by; /* kurt */
	}

	act("You flicker and blur into the shape of $t.", ch, form_table[form].short_descr,0,TO_CHAR);
	act("The edges of $n twists and stretch and $e becomes $t.", ch, form_table[form].short_descr,0,TO_ROOM);

	free_string(ch->name);
	ch->name = str_dup(form_table[form].short_descr);

	free_string(ch->short_descr);
	ch->short_descr = str_dup(form_table[form].short_descr);

	free_string(ch->long_descr);
	ch->long_descr = str_dup(form_table[form].long_descr);

	free_string(ch->description);
	ch->description = str_dup(form_table[form].description);

	ch->dam_reduc = form_table[form].dam_reduc;

	for(i = 0; i < MAX_STATS; i++)
		ch->perm_stat[i] = form_table[form].form_stats[i];

	for(i = 0; i <= 3; i++)
		ch->armor[i] = form_table[form].armor[i];

	ch->carry_number = 0;
	ch->carry_weight = 0;
	ch->saving_throw = form_table[form].saves_spell;
	ch->pcdata->shifted = form;
	ch->carrying = NULL;
	ch->imm_flags = form_table[form].imm_flags;
	ch->res_flags = form_table[form].res_flags;
	ch->vuln_flags = form_table[form].vuln_flags;
	ch->damroll = form_table[form].dam_roll;
	ch->hitroll = form_table[form].hit_roll;
	ch->affected_by |= form_table[form].affected_by; /* kurt */
}

CHAR_DATA *get_a_victim(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	if(ch->fighting && !str_cmp(argument,""))
		return ch->fighting;
	if(!str_cmp(argument,""))
	{
		send_to_char("Who do you want to do that to?\n\r",ch);
		return NULL;
	}
	if((victim=get_char_room(ch,argument)))
	{
		if(is_safe(ch,victim))
			return NULL;
		if(victim==ch)
		{
			send_to_char("Ouch.\n\r",ch);
			return NULL;
		}
		return victim;
	}
	send_to_char("They aren't here.\n\r",ch);
	return NULL;
}

int get_shifted(CHAR_DATA *ch)
{
	int form = 0;
	form = ch->pcdata->shifted;
	if (form > 0)
	{	
		return form;
	}

	form = 0;
	return form;

}

bool isShifted(CHAR_DATA *ch)
{
        if( !IS_NPC(ch) && ch->pcdata->shifted > 0 )
	    return TRUE;
	
        return FALSE;

}

void do_revert (CHAR_DATA *ch, char *argument)
{
	int i;

	if (!isShifted(ch))
	{
		send_to_char("You are already in your normal form.\n\r", ch);
		return;
	}

	if (IS_NPC(ch) || !isShifted(ch))
	{
		send_to_char("You're already in your normal form.\n\r", ch);
		return;
	}

	act("You return to your normal shape.", ch,0,0,TO_CHAR);
	act("The edges of $n twists and stretch and $e returns to normal.", ch, 0,0,TO_ROOM);

	free_string(ch->name);
	ch->name = str_dup(ch->pcdata->old->name);

	free_string(ch->short_descr);
	ch->short_descr = str_dup(ch->pcdata->old->short_descr);

	free_string(ch->long_descr);
	ch->long_descr = str_dup(ch->pcdata->old->long_descr);

	free_string(ch->description);
	ch->description = str_dup(ch->pcdata->old->description);

	for(i=0; i < MAX_STATS; i++)
		ch->perm_stat[i] = ch->pcdata->old->perm_stat[i];

	for(i=0; i <= 3; i++)
		ch->armor[i] = ch->pcdata->old->armor[i];

	ch->dam_reduc = ch->pcdata->old->dam_reduc;
	ch->carrying = ch->pcdata->old->carrying;
	ch->carry_weight = ch->pcdata->old->carry_weight;
	ch->carry_number = ch->pcdata->old->carry_number;
	ch->saving_throw = ch->pcdata->old->saving_throw;
	ch->imm_flags = ch->pcdata->old->imm_flags;
	ch->res_flags = ch->pcdata->old->res_flags;
	ch->vuln_flags = ch->pcdata->old->vuln_flags;
	ch->damroll = ch->pcdata->old->damroll;
	ch->hitroll = ch->pcdata->old->hitroll;

	ch->affected_by &= ch->pcdata->old->affected_by; /* kurt */
	ch->pcdata->shifted = -1;
	ch->pcdata->old = NULL;
}

void do_forms(CHAR_DATA *ch, char *argument)
{
	int i;
	char buf[1000];
	bool found = FALSE;

	if(IS_NPC(ch))
		return;
	send_to_char("You can shapeshift into the following forms:\n\r",ch);
	for(i=0;i<MAX_FORM;i++)
	{
		if(ch->pcdata->forms[i]<70)
			continue;
		sprintf(buf,"%s\n\r",capitalize(form_table[i].name));
		send_to_char(buf,ch);
		found = TRUE;
	}
	if(!found)
		send_to_char("You know no forms.\n\r",ch);
}

int form_lookup(char *argument)
{
   int form;
   for (form = 0; form < MAX_FORM; form++)
   {
        if (LOWER(argument[0]) == LOWER(form_table[form].name[0])
        &&  !str_prefix(argument,form_table[form].name))
            return form;
   }  
   return -1;
}

void do_fstat(CHAR_DATA *ch, char *argument)
{
	int i;
	char buf[1000];
	i = form_lookup(argument);
	if(i<0)
		return send_to_char("No form by that name found.\n\r",ch);
	sprintf(buf,"Form Name: %s\n\r",form_table[i].name);
	send_to_char(buf,ch);
	sprintf(buf,"Form Short: %s\n\r",form_table[i].short_descr);
	send_to_char(buf,ch);
	sprintf(buf,"Form Long: %s",form_table[i].long_descr);
	send_to_char(buf,ch);
        sprintf(buf,"Form Yell: %s\n\r",
        form_table[i].yell);
        send_to_char(buf,ch);
    	sprintf( buf,
          "Form Str: %d  Int: %d  Wis: %d  Dex: %d Con: %d\n\r",
	form_table[i].form_stats[STAT_STR],
        form_table[i].form_stats[STAT_INT],
        form_table[i].form_stats[STAT_WIS],
        form_table[i].form_stats[STAT_DEX],
        form_table[i].form_stats[STAT_CON]);
	send_to_char(buf,ch);
	sprintf(buf, 
	  "Form ACs:  pierce: %d  bash: %d  slash: %d  magic: %d\n\r",
	form_table[i].armor[0],
        form_table[i].armor[1],
        form_table[i].armor[2],
        form_table[i].armor[3]);
	send_to_char(buf,ch);
	sprintf(buf,"Max Attacks/Round: %d  Dam_reduc: %d%%  Attack Type: %s\n\r",
	form_table[i].attacks,
	form_table[i].dam_reduc,
	form_table[i].attack_type);
	send_to_char(buf,ch);
	sprintf(buf,"Parry Modifier: %d%%  Damage: %dd%d+%d  Hitroll: %d  Damroll: %d  Saves_spell: %d\n\r",
	form_table[i].parry_mod,
	form_table[i].dice_num,
        form_table[i].dice_type,
        form_table[i].dice_add,
	form_table[i].hit_roll,
	form_table[i].dam_roll,
	form_table[i].saves_spell);
	send_to_char(buf,ch);
	if(str_cmp(form_table[i].spec_one,""))
	{
		sprintf(buf,"Special Attack: %s\n\r",capitalize(form_table[i].spec_one));
		send_to_char(buf,ch);
	}
        if(str_cmp(form_table[i].spec_two,""))
        {
                sprintf(buf,"Special Attack: %s\n\r",capitalize(form_table[i].spec_two));
                send_to_char(buf,ch);
        }
        if(str_cmp(form_table[i].spec_three,""))
        {
                sprintf(buf,"Special Attack: %s\n\r",capitalize(form_table[i].spec_three));
                send_to_char(buf,ch);
        }
}

