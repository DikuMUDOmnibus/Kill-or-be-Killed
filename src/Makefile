O_FILES = act_comm.o act_ente.o act_info.o act_move.o act_obj.o act_wiz.o \
          alias.o ban.o comm.o const.o db.o db2.o effects.o fight.o \
	  handler.o healer.o interp.o note.o lookup.o magic.o recycle.o\
	  save.o skills.o special.o tables.o update.o mob_cmds.o\
          mob_prog.o tattoo.o dioextra.o morecabal.o detextra.o morefight.o\
	  moremagic.o lich.o music.o titles.o necro.o olc.o olc_act.o \
          graelik.o olc_save.o mem.o string.o bit.o olc_mpcode.o commune.o hedit.o \
          shape.o elementalist.o worship.o ravager.o sage.o acadian.o
                    
tartarus: $(O_FILES)
	
	gcc -O -g -lcrypt -o inahn $(O_FILES)
	tar -cpf inahn_backup3.tar.gz -z *.c *.h Makefile
	mv inahn_backup3.tar.gz ../backup
	mv inahn ../area
	cd ..

.c.o: merc.h
	gcc -c -Wall -O -ggdb $<

clean: 
	rm -f *.o

backup:
	tar --cpf inahn_backup.tar.gz -z *.c *.h Makefile inahn
