/* Form category (sh_int form_category) */
#define FORM_ANML		1
#define FORM_QUEST		2
#define FORM_MYTH_ANIMAL 	3

/* Foci (sh_int form_foci) */
#define FORM_OFFENSE	1
#define FORM_DEFENSE	2
#define FORM_UTILITY	3
#define FORM_AIR	4
#define FORM_WATER	5
#define FORM_SUB	6
#define FORM_PLANAR	7
#define FORM_PLANT	8

/* MAX_FORM moved to merc.h */

extern const struct form_type form_table [MAX_FORM];

struct form_type {
	char *		name;
	char *		short_descr;
	char *		long_descr;
	sh_int		form_stats[6];
	sh_int		armor[4];
	sh_int		dam_reduc; //100 is normal dam
	sh_int		attacks; //max attacks per round
	char *		attack_type; //must be on attack_table
	sh_int		parry_mod; //how difficult it is for the form to parry, 0 is normal, - is harder, + is easier (numbers are added to parry chance)
	sh_int		dice_num; //6d4+3 (6)
	sh_int		dice_type; //6d4+3 (4)
	sh_int		dice_add; //6d4+3 (3)
	sh_int		hit_roll;
	sh_int		dam_roll;
	sh_int		saves_spell;
	long		imm_flags; //immunity flags
	long		res_flags; //resistance flags
	long		vuln_flags; //vuln flags
	long		affected_by; //affects
	sh_int		form_category; //form_quest etc..
	sh_int		form_foci;
	char *		yell; //the yell when the form is attacked
	char *		description;
	long		specials; //special bits (progs)
	char *		spec_one;
	char *		spec_two;
	char *		spec_three;
	
};

