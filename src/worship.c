#include "include.h"

#define OBJ_VNUM_WORSHIP 507
#define OBJ_VNUM_WORSHIP_LIRIEL 112
#define OBJ_VNUM_WORSHIP_ELOCIN 508

void do_worship(CHAR_DATA *ch,char *argument)
{
	OBJ_DATA *collar;
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_INPUT_LENGTH];
	int i;


    one_argument(argument,arg);

	if ( (arg == '\0') && (ch->pcdata->god != NULL) && (!IS_IMMORTAL(ch)) )
	{
		send_to_char("You are already worshipping a god.\n\r" , ch );
		return;
	}

    if (!strcmp(arg,"ceial"))
    {
		send_to_char("You are now worshipping Ceial.\n\r", ch );
	    collar = create_object( get_obj_index( OBJ_VNUM_WORSHIP ), 0 );
	    obj_to_char( collar, ch );
	    do_wear( ch, "holycollar" );
	    send_to_char("You are now bound to your god.\n\r", ch );
	    ch->pcdata->god	= str_dup( arg );

	}
	else if (!strcmp(arg,"aino"))
	{
		send_to_char("You are now worshipping Aino.\n\r", ch );
		collar = create_object( get_obj_index( OBJ_VNUM_WORSHIP ), 0 );
		obj_to_char( collar, ch );
		do_wear( ch, "holycollar" );
	    send_to_char("You are now bound to your god.\n\r", ch );
	    ch->pcdata->god	= str_dup( arg );
	}
	else if (!strcmp(arg,"liriel"))
	{
		send_to_char("You are now worshipping Liriel.\n\r", ch );
		collar = create_object( get_obj_index( OBJ_VNUM_WORSHIP_LIRIEL ), 0 );
		obj_to_char( collar, ch );
		do_wear( ch, "earthgemstone" );
	    send_to_char("You are now bound to your god.\n\r", ch );
	    ch->pcdata->god	= str_dup( arg );
	}
	else if (!strcmp(arg,"gawinn"))
	{
		send_to_char("You are now worshipping Gawinn.\n\r", ch );
		collar = create_object( get_obj_index( OBJ_VNUM_WORSHIP ), 0 );
		obj_to_char( collar, ch );
		do_wear( ch, "holycollar" );
	    send_to_char("You are now bound to your god.\n\r", ch );
	    ch->pcdata->god	= str_dup( arg );
	}
	else if (!strcmp(arg,"elocin"))
	{
		send_to_char("You are now worshipping Elocin.\n\r", ch );
		collar = create_object( get_obj_index( OBJ_VNUM_WORSHIP_ELOCIN ), 0 );
		obj_to_char( collar, ch );
		do_wear( ch, "holycollar" );
	    send_to_char("You are now bound to your god.\n\r", ch );
	    ch->pcdata->god	= str_dup( arg );
	}
	else if (!strcmp(arg,"kador"))
	{
		send_to_char("You are now worshipping Kador.\n\r", ch );
		collar = create_object( get_obj_index( OBJ_VNUM_WORSHIP ), 0 );
		obj_to_char( collar, ch );
		do_wear( ch, "holycollar" );
	    send_to_char("You are now bound to your god.\n\r", ch );
	    ch->pcdata->god	= str_dup( arg );
	}
	else if (!strcmp(arg,"alisandra"))
	{
		send_to_char("You are now worshipping Alisandra.\n\r", ch );
		collar = create_object( get_obj_index( OBJ_VNUM_WORSHIP ), 0 );
		obj_to_char( collar, ch );
		do_wear( ch, "holycollar" );
	    send_to_char("You are now bound to your god.\n\r", ch );
	    ch->pcdata->god	= str_dup( arg );
	}else{

			send_to_char("You can do this once. So choose wisely.\n\r", ch );
			send_to_char("Gods that are open to worship:\n\r\n\r", ch );

			for (i = 1; i < MAX_GOD; i++)
			{
				sprintf(buf,"%9s  %-8s\n\r", god_table[i].god_name, god_table[i].god_belief);
				send_to_char(buf,ch);
			}


		}

		return;
	}





