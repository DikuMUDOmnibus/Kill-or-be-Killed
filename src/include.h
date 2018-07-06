
/* This is what I include in nearly every file.
 *
 */

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <arpa/telnet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <signal.h>

#include "merc.h"
#include "interp.h"
#include "recycle.h"
#include "tables.h"
#include "olc.h"
#include "lookup.h"
#include "magic.h"
#include "song.h"
#include "db.h"
#include "mob_cmds.h"
#include "telnet.h"
#include "shape.h"

