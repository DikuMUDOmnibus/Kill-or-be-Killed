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
 *  Thanks to abaddon for proof-reading our comm.c and pointing out bugs.  *
 *  Any remaining bugs are, of course, our work, not his.  :)              *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
*	ROM 2.4 is copyright 1993-1996 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Tartarus/doc/rom.license                  *
***************************************************************************/

/***************************************************************************
*       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
*	In using this code you agree to comply with the Tartarus license   *
*       found in the file /Tartarus/doc/tartarus.doc                       *
***************************************************************************/

/*
 * This file contains all of the OS-dependent stuff:
 *   startup, signals, BSD sockets for tcp/ip, i/o, timing.
 *
 * The data flow for input is:
 *    Game_loop ---> Read_from_descriptor ---> Read
 *    Game_loop ---> Read_from_buffer
 *
 * The data flow for output is:
 *    Game_loop ---> Process_Output ---> Write_to_descriptor -> Write
 *
 * The OS-dependent functions are Read_from_descriptor and Write_to_descriptor.
 * -- Furey  26 Jan 1993
 */

#include "include.h"

/* command procedures needed */
DECLARE_DO_FUN(do_help		);
DECLARE_DO_FUN(do_look		);
DECLARE_DO_FUN(do_skills	);
DECLARE_DO_FUN(do_outfit	);
DECLARE_DO_FUN(do_unread	);
DECLARE_DO_FUN(easy_induct	);
//DECLARE_DO_FUN(gain_exp		);

void announce_login	args( ( CHAR_DATA *ch));
void announce_logout	args( ( CHAR_DATA *ch));
const char *lowstring(const char *i);
void  reverse_word	args( ( char *w, int n ) );
bool wizlock = FALSE;
bool newlock = FALSE;


/*
 * Malloc debugging stuff.
 */
#if defined(sun)
#undef MALLOC_DEBUG
#endif

#if defined(MALLOC_DEBUG)
#include <malloc.h>
extern	int	malloc_debug	args( ( int  ) );
extern	int	malloc_verify	args( ( void ) );
#endif



/*
 * Signal handling.
 * Apollo has a problem with __attribute(atomic) in signal.h,
 *   I dance around it.
 */
#if defined(apollo)
#define __attribute(x)
#endif

#if defined(unix)
#include <signal.h>
#endif

#if defined(apollo)
#undef __attribute
#endif



/*
 * Socket and TCP/IP stuff.
 */
#if	defined(macintosh) || defined(MSDOS)
const	char	echo_off_str	[] = { '\0' };
const	char	echo_on_str	[] = { '\0' };
const	char 	go_ahead_str	[] = { '\0' };
#endif

#if	defined(unix)
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "telnet.h"
const	char	echo_off_str	[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const	char	echo_on_str	[] = { IAC, WONT, TELOPT_ECHO, '\0' };
const	char 	go_ahead_str	[] = { IAC, GA, '\0' };
#endif



/*
 * OS-dependent declarations.
 */
#if	defined(_AIX)
#include <sys/select.h>
//int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
//int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int     gofday    args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if	defined(apollo)
#include <unistd.h>
void	bzero		args( ( char *b, int length ) );
#endif

#if	defined(__hpux)
//int	accept		args( ( int s, void *addr, int *addrlen ) );
//int	bind		args( ( int s, const void *addr, int addrlen ) );
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, void *addr, int *addrlen ) );
int	getsockname	args( ( int s, void *name, int *addrlen ) );
int     gofday    args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname,
 				const void *optval, int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if	defined(interactive)
#include <net/errno.h>
#include <sys/fnctl.h>
#endif

#if	defined(linux)
/*
    Linux shouldn't need these. If you have a problem compiling, try
    uncommenting accept and bind.
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
*/

int	close		args( ( int fd ) );
//int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
//int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int     gofday    args( ( struct timeval *tp, struct timezone *tzp ) );
//int	listen		args( ( int s, int backlog ) );
//int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	socket		args( ( int domain, int type, int protocol ) );
//int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if	defined(macintosh)
#include <console.h>
#include <fcntl.h>
#include <unix.h>
struct	timeval
{
	time_t	tv_sec;
	time_t	tv_usec;
};
#if	!defined(isascii)
#define	isascii(c)		( (c) < 0200 )
#endif
static	long			theKeys	[4];

int     gofday            args( ( struct timeval *tp, void *tzp ) );
#endif

#if	defined(MIPS_OS)
extern	int		errno;
#endif

#if	defined(MSDOS)
int     gofday    args( ( struct timeval *tp, void *tzp ) );
int	kbhit		args( ( void ) );
#endif

#if	defined(NeXT)
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
#if	!defined(htons)
u_short	htons		args( ( u_short hostshort ) );
#endif
#if	!defined(ntohl)
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if	defined(sequent)
//int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
//int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int     gofday    args( ( struct timeval *tp, struct timezone *tzp ) );
#if	!defined(htons)
u_short	htons		args( ( u_short hostshort ) );
#endif
int	listen		args( ( int s, int backlog ) );
#if	!defined(ntohl)
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, caddr_t optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

/* This includes Solaris Sys V as well */
#if defined(sun)
//int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
//int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int     gofday    args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
#if defined(SYSV)
int setsockopt		args( ( int s, int level, int optname,
			    const char *optval, int optlen ) );
#else
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
#endif
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if defined(ultrix)
//int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
//int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int     gofday    args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif



/*
 * Global variables.
 */
DESCRIPTOR_DATA *   descriptor_list;	/* All open descriptors		*/
DESCRIPTOR_DATA *   d_next;		/* Next descriptor in loop	*/
FILE *		    fpReserve;		/* Reserved file handle		*/
bool		    god;		/* All new chars are gods!	*/
bool		    merc_down;		/* Shutdown			*/
bool		    wizlock;		/* Game is wizlocked		*/
bool		    newlock;		/* Game is newlocked		*/
bool		    arena;		/* Game is in arena		*/
bool                MOBtrigger = TRUE;  /* act() switch                 */
char		    str_boot_time[MAX_INPUT_LENGTH];
time_t		    current_time;	/* time of this pulse */



/*
 * OS-dependent local functions.
 */
#if defined(macintosh) || defined(MSDOS)
void	game_loop_mac_msdos	args( ( void ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
bool	write_to_descriptor	args( ( int desc, char *txt, int length ) );
#endif

#if defined(unix)
void	game_loop_unix		args( ( int control ) );
int	init_socket		args( ( int port ) );
void	init_descriptor		args( ( int control ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
bool	write_to_descriptor	args( ( int desc, char *txt, int length ) );
#endif




/*
 * Other local functions (OS-independent).
 */
bool	check_parse_name	args( ( char *name ) );
bool	check_reconnect		args( ( DESCRIPTOR_DATA *d, char *name,
				    bool fConn ) );
bool	check_playing		args( ( DESCRIPTOR_DATA *d, char *name ) );
int	main			args( ( int argc, char **argv ) );
void	nanny			args( ( DESCRIPTOR_DATA *d, char *argument ) );
bool	process_output		args( ( DESCRIPTOR_DATA *d, bool fPrompt ) );
void	read_from_buffer	args( ( DESCRIPTOR_DATA *d ) );
void	stop_idling		args( ( CHAR_DATA *ch ) );
void    bust_a_prompt           args( ( CHAR_DATA *ch ) );
bool 	output_buffer		args( ( DESCRIPTOR_DATA *d ) );
void    process_text            args( ( CHAR_DATA *ch, char *text ) );

int port, control;

int main( int argc, char **argv )
{
    struct timeval now_time;
//    int port;
//
//#if defined(unix)
//    int control;
//#endif
	bool fCopyOver = FALSE;

    /*
     * Memory debugging if needed.
     */
#if defined(MALLOC_DEBUG)
    malloc_debug( 2 );
#endif

    /*
     * Init time.
     */
    gettimeofday( &now_time, NULL );
    current_time 	= (time_t) now_time.tv_sec;
    strcpy( str_boot_time, ctime( &current_time ) );

    /*
     * Macintosh console initialization.
     */
#if defined(macintosh)
    console_options.nrows = 31;
    cshow( stdout );
    csetmode( C_RAW, stdin );
    cecho2file( "log file", 1, stderr );
#endif

    /*
     * Reserve one channel for our use.
     */
    if ( ( fpReserve = fopen( NULL_FILE, "r" ) ) == NULL )
    {
	perror( NULL_FILE );
	exit( 1 );
    }

    /*
     * Get the port number.
     */
    port = 9990;
    if ( argc!=1)
    {
	if ( !is_number( argv[1] ) )
	{
	    fprintf( stderr, "Usage: %s [port #]\n", argv[0] );
	    exit( 1 );
	}
	else if ( ( port = atoi( argv[1] ) ) <= 1024 )
	{
	    fprintf( stderr, "Port number must be above 1024.\n" );
	    exit( 1 );
	}

	/* Are we recovering from a copyover? */
 	if (argv[2] && argv[2][0])
 	{
 		fCopyOver = TRUE;
 		control = atoi(argv[3]);
 	}
 	else
 		fCopyOver = FALSE;

    }


    /*
     * Run the game.
     */
#if defined(macintosh) || defined(MSDOS)
//    boot_db( );
	boot_
    log_string( "Merc is ready to rock." );
    game_loop_mac_msdos( );
#endif

#if defined(unix)
//    control = init_socket( port );
//    boot_db( );

	if (!fCopyOver)
	    control = init_socket( port );

    boot_db();
    sprintf( log_buf, "Inahn has booted, binding on port %d.", port );
    log_string( log_buf );

    if (fCopyOver)
    	copyover_recover();

    game_loop_unix( control );
    close (control);
#endif

    /*
     * That's all, folks.
     */
    log_string( "Normal termination of game." );
    exit( 0 );
    return 0;
}

#if defined(unix)
int init_socket( int port )
{
    static struct sockaddr_in sa_zero;
    struct sockaddr_in sa;
    int x = 1;
    int fd;

    if ( ( fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
	perror( "Init_socket: socket" );
	exit( 1 );
    }

    if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR,
    (char *) &x, sizeof(x) ) < 0 )
    {
	perror( "Init_socket: SO_REUSEADDR" );
	close(fd);
	exit( 1 );
    }

#if defined(SO_DONTLINGER) && !defined(SYSV)
    {
	struct	linger	ld;

	ld.l_onoff  = 1;
	ld.l_linger = 1000;

	if ( setsockopt( fd, SOL_SOCKET, SO_DONTLINGER,
	(char *) &ld, sizeof(ld) ) < 0 )
	{
	    perror( "Init_socket: SO_DONTLINGER" );
	    close(fd);
	    exit( 1 );
	}
    }
#endif

    sa		    = sa_zero;
    sa.sin_family   = AF_INET;
    sa.sin_port	    = htons( port );

    if ( bind( fd, (struct sockaddr *) &sa, sizeof(sa) ) < 0 )
    {
	perror("Init socket: bind" );
	close(fd);
	exit(1);
    }


    if ( listen( fd, 3 ) < 0 )
    {
	perror("Init socket: listen");
	close(fd);
	exit(1);
    }

    return fd;
}
#endif



#if defined(macintosh) || defined(MSDOS)
void game_loop_mac_msdos( void )
{
    struct timeval last_time;
    struct timeval now_time;
    static DESCRIPTOR_DATA dcon;

    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;

    /*
     * New_descriptor analogue.
     */
    dcon.descriptor	= 0;
    dcon.connected	= CON_GET_NAME;
    dcon.host		= str_dup( "localhost" );
    dcon.outsize	= 2000;
    dcon.outbuf		= alloc_mem( dcon.outsize );
    dcon.next		= descriptor_list;
    dcon.showstr_head	= NULL;
    dcon.showstr_point	= NULL;
    dcon.pEdit		= NULL;
    dcon.pString	= NULL;
    dcon.editor		= 0;
    descriptor_list	= &dcon;

    /*
     * Send the greeting.
     */
    {
	extern char * help_greeting;
	if ( help_greeting[0] == '.' )
	    write_to_buffer( &dcon, help_greeting+1, 0 );
	else
	    write_to_buffer( &dcon, help_greeting  , 0 );
    }

    /* Main loop */
    while ( !merc_down )
    {
	DESCRIPTOR_DATA *d;

	/*
	 * Process input.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next	= d->next;
	    d->fcommand	= FALSE;

#if defined(MSDOS)
	    if ( kbhit( ) )
#endif
	    {
		if ( d->character != NULL )
		    d->character->timer = 0;
		if ( !read_from_descriptor( d ) )
		{
		    if ( d->character != NULL)
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		    continue;
		}
	    }

	    if (d->character != NULL && d->character->daze > 0)
		--d->character->daze;

	    if ( d->character != NULL && d->character->wait > 0 )
	    {
		--d->character->wait;
		continue;
	    }

	    read_from_buffer( d );
	    if ( d->incomm[0] != '\0' )
	    {
		d->fcommand	= TRUE;
		stop_idling( d->character );

/*		if ( d->connected == CON_PLAYING )
		    substitute_alias( d, d->incomm );
		else
		    nanny( d, d->incomm );*/

	        /* OLC */
	        if ( d->showstr_point )
	            show_string( d, d->incomm );
	        else
	        if ( d->pString )
	            string_add( d->character, d->incomm );
	        else
	            switch ( d->connected )
	            {
	                case CON_PLAYING:
			    if ( !run_olc_editor( d ) )
				substitute_alias( d, d->incomm );
			    break;
	                default:
			    nanny( d, d->incomm );
			    break;
	            }

		d->incomm[0]	= '\0';
	    }
	}



	/*
	 * Autonomous game motion.
	 */
	update_handler( );



	/*
	 * Output.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;

	    if ( ( d->fcommand || d->outtop > 0 ) )
	    {
		if ( !process_output( d, TRUE ) )
		{
		    if ( d->character != NULL && d->character->level > 1)
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		}
	    }
	}



	/*
	 * Synchronize to a clock.
	 * Busy wait (blargh).
	 */
	now_time = last_time;
	for ( ; ; )
	{
	    int delta;

#if defined(MSDOS)
	    if ( kbhit( ) )
#endif
	    {
		if ( dcon.character != NULL )
		    dcon.character->timer = 0;
		if ( !read_from_descriptor( &dcon ) )
		{
		    if ( dcon.character != NULL && d->character->level > 1)
			save_char_obj( d->character );
		    dcon.outtop	= 0;
		    close_socket( &dcon );
		}
#if defined(MSDOS)
		break;
#endif
	    }

	    gettimeofday( &now_time, NULL );
	    delta = ( now_time.tv_sec  - last_time.tv_sec  ) * 1000 * 1000
		  + ( now_time.tv_usec - last_time.tv_usec );
	    if ( delta >= 1000000 / PULSE_PER_SECOND )
		break;
	}
	last_time    = now_time;
	current_time = (time_t) last_time.tv_sec;
    }

    return;
}
#endif



#if defined(unix)
void game_loop_unix( int control )
{
    static struct timeval null_time;
    struct timeval last_time;

    signal( SIGPIPE, SIG_IGN );
    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;

    /* Main loop */
    while ( !merc_down )
    {
	fd_set in_set;
	fd_set out_set;
	fd_set exc_set;
	DESCRIPTOR_DATA *d;
	int maxdesc;

#if defined(MALLOC_DEBUG)
	if ( malloc_verify( ) != 1 )
	    abort( );
#endif

	/*
	 * Poll all active descriptors.
	 */
	FD_ZERO( &in_set  );
	FD_ZERO( &out_set );
	FD_ZERO( &exc_set );
	FD_SET( control, &in_set );
	maxdesc	= control;
	for ( d = descriptor_list; d; d = d->next )
	{
	    maxdesc = UMAX( maxdesc, d->descriptor );
	    FD_SET( d->descriptor, &in_set  );
	    FD_SET( d->descriptor, &out_set );
	    FD_SET( d->descriptor, &exc_set );
	}

	if ( select( maxdesc+1, &in_set, &out_set, &exc_set, &null_time ) < 0 )
	{
	    perror( "Game_loop: select: poll" );
	    exit( 1 );
	}

	/*
	 * New connection?
	 */
	if ( FD_ISSET( control, &in_set ) )
	    init_descriptor( control );

	/*
	 * Kick out the freaky folks.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;
	    if ( FD_ISSET( d->descriptor, &exc_set ) )
	    {
		FD_CLR( d->descriptor, &in_set  );
		FD_CLR( d->descriptor, &out_set );
		if ( d->character && d->character->level > 1)
		    save_char_obj( d->character );
		d->outtop	= 0;
		close_socket( d );
	    }
	}

	/*
	 * Process input.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next	= d->next;
	    d->fcommand	= FALSE;

	    if ( FD_ISSET( d->descriptor, &in_set ) )
	    {
		if ( d->character != NULL )
		    d->character->timer = 0;
		if ( !read_from_descriptor( d ) )
		{
		    FD_CLR( d->descriptor, &out_set );
		    if ( d->character != NULL && d->character->level > 1)
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		    continue;
		}
	    }

	    if (d->character != NULL && d->character->daze > 0)
		--d->character->daze;

	    if ( d->character != NULL && d->character->wait > 0 )
	    {
		--d->character->wait;
		continue;
	    }

	    read_from_buffer( d );
	    if ( d->incomm[0] != '\0' )
	    {
		d->fcommand	= TRUE;
		stop_idling( d->character );

/*		if (d->showstr_point)
		    show_string(d,d->incomm);
		else if ( d->connected == CON_PLAYING )
		    substitute_alias( d, d->incomm );
		else*/

	/* OLC */
	if ( d->showstr_point )
	    show_string( d, d->incomm );
	else
	if ( d->pString )
	    string_add( d->character, d->incomm );
	else
	    switch ( d->connected )
	    {
	        case CON_PLAYING:
		    if ( !run_olc_editor( d ) )
    		        substitute_alias( d, d->incomm );
		    break;
	        default:
	        nanny( d, d->incomm );
	        break;

	    }

		d->incomm[0]	= '\0';
	    }
	}



	/*
	 * Autonomous game motion.
	 */
	update_handler( );



	/*
	 * Output.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;

	    if ( ( d->fcommand || d->outtop > 0 )
	    &&   FD_ISSET(d->descriptor, &out_set) )
	    {
		if ( !process_output( d, TRUE ) )
		{
		    if ( d->character != NULL && d->character->level > 1)
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		}
	    }
	}



	/*
	 * Synchronize to a clock.
	 * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
	 * Careful here of signed versus unsigned arithmetic.
	 */
	{
	    struct timeval now_time;
	    long secDelta;
	    long usecDelta;

	    gettimeofday( &now_time, NULL );
	    usecDelta	= ((int) last_time.tv_usec) - ((int) now_time.tv_usec)
			+ 1000000 / PULSE_PER_SECOND;
	    secDelta	= ((int) last_time.tv_sec ) - ((int) now_time.tv_sec );
	    while ( usecDelta < 0 )
	    {
		usecDelta += 1000000;
		secDelta  -= 1;
	    }

	    while ( usecDelta >= 1000000 )
	    {
		usecDelta -= 1000000;
		secDelta  += 1;
	    }

	    if ( secDelta > 0 || ( secDelta == 0 && usecDelta > 0 ) )
	    {
		struct timeval stall_time;

		stall_time.tv_usec = usecDelta;
		stall_time.tv_sec  = secDelta;
		if ( select( 0, NULL, NULL, NULL, &stall_time ) < 0 )
		{
		    perror( "Game_loop: select: stall" );
		    exit( 1 );
		}
	    }
	}

	gettimeofday( &last_time, NULL );
	current_time = (time_t) last_time.tv_sec;
    }

    return;
}
#endif



#if defined(unix)
void init_descriptor( int control )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *dnew;
    struct sockaddr_in sock;
    struct hostent *from;
    int desc;
    int size;

    size = sizeof(sock);
    getsockname( control, (struct sockaddr *) &sock, &size );
    if ( ( desc = accept( control, (struct sockaddr *) &sock, &size) ) < 0 )
    {
	perror( "New_descriptor: accept" );
	return;
    }

#if !defined(FNDELAY)
#define FNDELAY O_NDELAY
#endif

    if ( fcntl( desc, F_SETFL, FNDELAY ) == -1 )
    {
	perror( "New_descriptor: fcntl: FNDELAY" );
	return;
    }

    /*
     * Cons a new descriptor.
     */
    dnew = new_descriptor(); /* new_descriptor now also allocates things */
    dnew->descriptor = desc;

    size = sizeof(sock);
    if ( getpeername( desc, (struct sockaddr *) &sock, &size ) < 0 )
    {
	perror( "New_descriptor: getpeername" );
	dnew->host = str_dup( "(unknown)" );
    }
    else
    {
	/*
	 * Would be nice to use inet_ntoa here but it takes a struct arg,
	 * which ain't very compatible between gcc and system libraries.
	 */
	int addr;

	addr = ntohl( sock.sin_addr.s_addr );
	sprintf( buf, "%d.%d.%d.%d",
	    ( addr >> 24 ) & 0xFF, ( addr >> 16 ) & 0xFF,
	    ( addr >>  8 ) & 0xFF, ( addr       ) & 0xFF
	    );
	sprintf( log_buf, "Sock.sinaddr:  %s", buf );
	log_string( log_buf );
	from = gethostbyaddr( (char *) &sock.sin_addr,
	    sizeof(sock.sin_addr), AF_INET );
	dnew->host = str_dup( from ? from->h_name : buf );
    }

    /*
     * Swiftest: I added the following to ban sites.  I don't
     * endorse banning of sites, but Copper has few descriptors now
     * and some people from certain sites keep abusing access by
     * using automated 'autodialers' and leaving connections hanging.
     *
     * Furey: added suffix check by request of Nickel of HiddenWorlds.
     */
    if ( check_ban(dnew->host,BAN_ALL))
    {
	write_to_descriptor( desc,
	    "Your site has been banned from this mud.\n\r", 0 );
	close( desc );
	free_descriptor(dnew);
	return;
    }
    /*
     * Init descriptor data.
     */
    dnew->next			= descriptor_list;
    descriptor_list		= dnew;

    /*
     * Send the greeting.
     */
    {
	extern char * help_greeting;
	if ( help_greeting[0] == '.' )
	    write_to_buffer( dnew, help_greeting+1, 0 );
	else
	    write_to_buffer( dnew, help_greeting  , 0 );
    }

    return;
}
#endif



void close_socket( DESCRIPTOR_DATA *dclose )
{
    CHAR_DATA *ch;
    char buf[MAX_STRING_LENGTH];

    if ( dclose->outtop > 0 )
	process_output( dclose, FALSE );

    if ( dclose->snoop_by != NULL )
    {
	write_to_buffer( dclose->snoop_by,
	    "Your victim has left the game.\n\r", 0 );
    }

    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->snoop_by == dclose )
		d->snoop_by = NULL;
	}
    }

    if ( ( ch = dclose->character ) != NULL )
    {
	sprintf( log_buf, "Closing link to %s.", ch->name );
	log_string( log_buf );
	/* cut down on wiznet spam when rebooting */
	if ( dclose->connected == CON_PLAYING && !merc_down)
	{
	//act( "$n has lost $s link.", ch, NULL, NULL, TO_ROOM );
	sprintf(buf,"$N has lost $S link: (Last fought %s %d seconds ago).",
	ch->last_fight_name != NULL ? ch->last_fight_name : "nobody",
	ch->last_fight_time ? (int)(current_time - ch->last_fight_time) : -1);

	wiznet(buf,ch,NULL,WIZ_LINKS,0,get_trust(ch));
	ch->desc = NULL;
	}
	else
	{
	    free_char(dclose->original ? dclose->original :
		dclose->character );
	}
    }

    if ( d_next == dclose )
	d_next = d_next->next;

    if ( dclose == descriptor_list )
    {
	descriptor_list = descriptor_list->next;
    }
    else
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d && d->next != dclose; d = d->next )
	    ;
	if ( d != NULL )
	    d->next = dclose->next;
	else
	    bug( "Close_socket: dclose not found.", 0 );
    }

    close( dclose->descriptor );
    free_descriptor(dclose);
#if defined(MSDOS) || defined(macintosh)
    exit(1);
#endif
    return;
}



bool read_from_descriptor( DESCRIPTOR_DATA *d )
{
    int iStart;

    /* Hold horses if pending command already. */
    if ( d->incomm[0] != '\0' )
	return TRUE;

    /* Check for overflow. */
    iStart = strlen(d->inbuf);
    if ( iStart >= sizeof(d->inbuf) - 10 )
    {
	sprintf( log_buf, "%s input overflow!", d->host );
	log_string( log_buf );
	write_to_descriptor( d->descriptor,
	    "\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
	return FALSE;
    }

    /* Snarf input. */
#if defined(macintosh)
    for ( ; ; )
    {
	int c;
	c = getc( stdin );
	if ( c == '\0' || c == EOF )
	    break;
	putc( c, stdout );
	if ( c == '\r' )
	    putc( '\n', stdout );
	d->inbuf[iStart++] = c;
	if ( iStart > sizeof(d->inbuf) - 10 )
	    break;
    }
#endif

#if defined(MSDOS) || defined(unix)
    for ( ; ; )
    {
	int nRead;

	nRead = read( d->descriptor, d->inbuf + iStart,
	    sizeof(d->inbuf) - 10 - iStart );
	if ( nRead > 0 )
	{
	    iStart += nRead;
	    if ( d->inbuf[iStart-1] == '\n' || d->inbuf[iStart-1] == '\r' )
		break;
	}
	else if ( nRead == 0 )
	{
	    log_string( "EOF encountered on read." );
	    return FALSE;
	}
        else if ( errno == EAGAIN )
	    break;
	else
	{
	    perror( "Read_from_descriptor" );
	    return FALSE;
	}
    }
#endif

    d->inbuf[iStart] = '\0';
    return TRUE;
}



/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer( DESCRIPTOR_DATA *d )
{
    int i, j, k;

    /*
     * Hold horses if pending command already.
     */
    if ( d->incomm[0] != '\0' )
	return;

    /*
     * Look for at least one new line.
     */
    for ( i = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( d->inbuf[i] == '\0' )
	    return;
    }

    /*
     * Canonical input processing.
     */
    for ( i = 0, k = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( k >= MAX_INPUT_LENGTH - 2 )
	{
	    write_to_descriptor( d->descriptor, "Line too long.\n\r", 0 );

	    /* skip the rest of the line */
	    for ( ; d->inbuf[i] != '\0'; i++ )
	    {
		if ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
		    break;
	    }
	    d->inbuf[i]   = '\n';
	    d->inbuf[i+1] = '\0';
	    break;
	}

	if ( d->inbuf[i] == '\b' && k > 0 )
	    --k;
	else if ( isascii(d->inbuf[i]) && isprint(d->inbuf[i]) )
	    d->incomm[k++] = d->inbuf[i];
    }

    /*
     * Finish off the line.
     */
    if ( k == 0 )
	d->incomm[k++] = ' ';
    d->incomm[k] = '\0';

    /*
     * Deal with bozos with #repeat 1000 ...
     */

    if ( k > 1 || d->incomm[0] == '!' )
    {
    	if ( d->incomm[0] != '!' && strcmp( d->incomm, d->inlast ) )
	{
	    d->repeat = 0;
	}
	else
	{
	    if (++d->repeat >= 25 && d->character
	    &&  d->connected == CON_PLAYING)
	    {
		sprintf( log_buf, "%s input spamming!", d->host );
		log_string( log_buf );
		wiznet("Spam spam spam $N spam spam spam spam spam!",
		       d->character,NULL,WIZ_SPAM,0,get_trust(d->character));
		if (d->incomm[0] == '!')
		    wiznet(d->inlast,d->character,NULL,WIZ_SPAM,0,
			get_trust(d->character));
		else
		    wiznet(d->incomm,d->character,NULL,WIZ_SPAM,0,
			get_trust(d->character));

		d->repeat = 0;
		d->character->move/=2;
		d->character->mana-=20;
		d->character->exp=d->character->exp-100;
		write_to_descriptor(d->descriptor,
		"\n\rYour mind feels drained from trying to do the same thing over and over again...\n\r",0);
/*
		write_to_descriptor( d->descriptor, "\n\r*** DON'T SPAM!!! ***\n\r", 0 );
*/
		if (!IS_IMMORTAL(d->character)) {
		WAIT_STATE(d->character, 24);
		}

	    }
	}
    }


    /*
     * Do '!' substitution.
     */
    if ( d->incomm[0] == '!' )
	strcpy( d->incomm, d->inlast );
    else
	strcpy( d->inlast, d->incomm );

    /*
     * Shift the input buffer.
     */
    while ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
	i++;
    for ( j = 0; ( d->inbuf[j] = d->inbuf[i+j] ) != '\0'; j++ )
	;
    return;
}



/*
 * Low level output function.
 */
bool process_output( DESCRIPTOR_DATA *d, bool fPrompt )
{
    extern bool merc_down;

    /*
     * Bust a prompt.
     */
    if ( !merc_down && d->showstr_point )
	    write_to_buffer( d, "[Hit Return to continue]\n\r", 0 );
	else if ( fPrompt && d->pString && d->connected == CON_PLAYING )
	    write_to_buffer( d, "> ", 2 );
	else if ( fPrompt && d->connected == CON_PLAYING )
	{
	    CHAR_DATA *ch;

	CHAR_DATA *victim;

	ch = d->character;

        /* battle prompt */
        if ((victim = ch->fighting) != NULL && can_see(ch,victim))
        {
            int percent;
            char wound[100];
            char *pbuff;
	    char buf[MAX_STRING_LENGTH];
            char buffer[MAX_STRING_LENGTH*2];

        if (victim->max_hit > 0)
            percent = victim->hit * 100 / victim->max_hit;
        else
            percent = -1;

                 if (percent >= 100)
                sprintf(wound,"is in perfect condition.");
            else if (percent >= 90)
                sprintf(wound,"has a few scratches.");
            else if (percent >= 75)
                sprintf(wound,"has some small wounds.");
            else if (percent >= 50)
                sprintf(wound,"is covered in bleeding wounds.");
            else if (percent >= 30)
                sprintf(wound,"is gushing blood.");
            else if (percent >= 15)
                sprintf(wound,"is writhing in pain.");
            else if (percent >= 0)
                sprintf(wound,"is convulsing on the ground.");
            else
                sprintf(wound,"is nearly dead.");

	    sprintf(buf,"%s %s \n\r",
	            IS_NPC(victim) ? victim->short_descr : victim->name,wound);
            buf[0]      = UPPER( buf[0] );
            pbuff       = buffer;
            colorconv( pbuff, buf, d->character );
            write_to_buffer( d, buffer, 0);
        }

	ch = d->original ? d->original : d->character;
	if (!IS_SET(ch->comm, COMM_COMPACT) )
	    write_to_buffer( d, "\n\r", 2 );


        if ( IS_SET(ch->comm, COMM_PROMPT) )
            bust_a_prompt( d->character );

	if (IS_SET(ch->comm,COMM_TELNET_GA))
	    write_to_buffer(d,go_ahead_str,0);
    }

    /*
     * Short-circuit if nothing to write.
     */
    if ( d->outtop == 0 )
	return TRUE;

    /*
     * Snoop-o-rama.
     */
    if ( d->snoop_by != NULL )
    {
	if (d->character != NULL)
	    write_to_buffer( d->snoop_by, d->character->name,0);
	write_to_buffer( d->snoop_by, "> ", 2 );
	write_to_buffer( d->snoop_by, d->outbuf, d->outtop );
    }

    /*
     * OS-dependent output.
     *
     * now done at output_buffer( ) to deal with color codes.
     * - Wreck
     */
/*    return output_buffer( d );*/
    if ( !write_to_descriptor( d->descriptor, d->outbuf, d->outtop ) )
    {
	d->outtop = 0;
	return FALSE;
    }
    else
    {
	d->outtop = 0;
	return TRUE;
    }
}

/*
 * Bust a prompt (player settable prompt)
 * coded by Morgenes for Aldara Mud
 */
void bust_a_prompt( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    const char *str;
    const char *i;
    char *point;
    char *pbuff;
    char buffer[ MAX_STRING_LENGTH*2 ];
    char doors[MAX_INPUT_LENGTH];
    EXIT_DATA *pexit;
    bool found;
    const char *dir_name[] = {"N","E","S","W","U","D"};
    int door;

    point = buf;
    str = ch->prompt;
    if( !str || str[0] == '\0')
    {
        sprintf( buf, "{c<%dhp %dm %dmv>{x %s",
	    ch->hit,ch->mana,ch->move,ch->prefix);
	send_to_char(buf,ch);
	return;
    }

    if ( ch->invis_level > 1 )
	sprintf2(ch,"(Wizi %d) ",ch->invis_level);
    if ( ch->incog_level > 1 )
	sprintf2(ch,"(Incog %d) ",ch->incog_level);

   if (IS_SET(ch->comm,COMM_AFK))
   {
	send_to_char("<AFK> ",ch);
        return;
   }

   if (IS_SET(ch->comm,COMM_OLCEDIT))
   {
	send_to_char("[OLC Edit] ",ch);
	return;
   }

   while( *str != '\0' )
   {
      if( *str != '%' )
      {
         *point++ = *str++;
         continue;
      }
      ++str;
      switch( *str )
      {
         default :
            i = " "; break;
	case 'e':
	    found = FALSE;
	    doors[0] = '\0';
	    for (door = 0; door < 6; door++)
	    {
		if ((pexit = ch->in_room->exit[door]) != NULL
		&&  pexit ->u1.to_room != NULL
		&&  (can_see_room(ch,pexit->u1.to_room)
		||   (IS_AFFECTED(ch,AFF_INFRARED)
		&&    !IS_AFFECTED(ch,AFF_BLIND)))
		&&  !IS_SET(pexit->exit_info,EX_CLOSED))
		{
		    found = TRUE;
		    strcat(doors,dir_name[door]);
		}
	    }
	    if (!found)
	 	strcat(buf,"none");
	    sprintf(buf2,"%s",doors);
	    i = buf2; break;

 	 case 'c' :
	    sprintf(buf2,"%s","\n\r");
	    i = buf2; break;

         case 'h' :
            if ( !IS_SET( ch->act,PLR_COLOR ) )
              sprintf( buf2, "%d%s",
                ch->level>=30?ch->hit:PERCENT(ch->hit,ch->max_hit),
                ch->level>=30?"":"%" );
            else
              sprintf( buf2, "%s%d%s%s",
                          ch->hit<(ch->max_hit*4)/10?
                          ch->hit<(ch->max_hit*2)/10?C_B_RED:C_B_YELLOW:
                          CLEAR,
               ch->level>=30?ch->hit:PERCENT(ch->hit, ch->max_hit),
               ch->level>=30?"":"%",
               CLEAR );
            i = buf2; break;

         case 'H' :
            sprintf( buf2, "%d", ch->max_hit );
            i = buf2; break;
         case 'm' :
         if(ch->level >= 30)
            sprintf( buf2, "%d", ch->mana );
         else
            sprintf( buf2, "%d%%",
 (ch->max_mana == 0) ? 0 :  (ch->mana*100)/ch->max_mana );
         i = buf2; break;
         case 'M' :
            sprintf( buf2, "%d", ch->max_mana );
            i = buf2; break;
         case 'v':
         if(ch->level >= 30)
            sprintf( buf2, "%d", ch->move );
         else
            sprintf( buf2, "%d%%",
 (ch->max_move == 0) ? 0 : (ch->move*100)/ch->max_move );
         i = buf2; break;
         case 'V' :
            sprintf( buf2, "%d", ch->max_move );
            i = buf2; break;
         case 'x' :
            sprintf( buf2, "%d", ch->exp );
            i = buf2; break;
	 case 'X' :
	    sprintf(buf2, "%d", IS_NPC(ch) ? 0 :
            ch->level * exp_per_level(ch) - ch->exp);
	    i = buf2; break;
         case 'g' :
            sprintf( buf2, "%ld", ch->gold);
            i = buf2; break;
	 case 's' :
	    sprintf( buf2, "%ld", ch->silver);
	    i = buf2; break;
         case 'a' :
            if( ch->level > 9 )
               sprintf( buf2, "%d", ch->alignment );
            else
               sprintf( buf2, "%s", IS_GOOD(ch) ? "good" : IS_EVIL(ch) ?
                "evil" : "neutral" );
            i = buf2; break;
         case 'r' :
            if( ch->in_room != NULL )
               sprintf( buf2, "%s",
		((!IS_NPC(ch) && IS_SET(ch->act,PLR_HOLYLIGHT)) ||
		 (!IS_AFFECTED(ch,AFF_BLIND) && !room_is_dark( ch->in_room )))
		? ch->in_room->name : "darkness");
            else
               sprintf( buf2, " " );
            i = buf2; break;
         case 'R' :
            if( IS_IMMORTAL( ch ) && ch->in_room != NULL )
               sprintf( buf2, "%d", ch->in_room->vnum );
            else
               sprintf( buf2, " " );
            i = buf2; break;
         case 'z' :
            if( IS_IMMORTAL( ch ) && ch->in_room != NULL )
               sprintf( buf2, "%s", ch->in_room->area->name );
            else
               sprintf( buf2, " " );
            i = buf2; break;
         case '%' :
            sprintf( buf2, "%%" );
            i = buf2; break;
         case 'o' :
            sprintf( buf2, "%s", olc_ed_name(ch) );
            i = buf2; break;
	 case 'O' :
	    sprintf( buf2, "%s", olc_ed_vnum(ch) );
	    i = buf2; break;
	 case 'w' :
	    sprintf( buf2, "%d", ch->invis_level );
	    i = buf2; break;
      }
      ++str;
      while( (*point = *i) != '\0' )
         ++point, ++i;
   }
//   write_to_buffer( ch->desc, buf, point - buf );
    *point      = '\0';
    pbuff       = buffer;
    colorconv( pbuff, buf, ch );
    write_to_buffer( ch->desc, buffer, 0 );


   if (ch->prefix[0] != '\0')
        write_to_buffer(ch->desc,ch->prefix,0);
   return;
}

/*
 * output_buffer( descriptor )
 * this function sends output down a socket. Color codes are stripped off
 * is the player is not using color, or converted to ANSI color sequences
 * to provide colored output.
 * When using ANSI, the buffer can become a lot larger due to the (sometimes)
 * lengthy ANSI sequences, thus potentially overflowing the buffer. Therefor
 * *new* buffer is send in chunks.
 * The 'bzero's may seem unnecessary, but i didn't want to take risks.
 *
 * - Wreck
 */

bool output_buffer( DESCRIPTOR_DATA *d )
{
    char	buf[MAX_STRING_LENGTH];
    char	buf2[128];
    const char 	*str;
    char 	*i;
    char 	*point;
    bool	flash=FALSE, o_flash,
    		bold=FALSE, o_bold;
    bool	act=FALSE, ok=TRUE, color_code=FALSE;
    int		color=7, o_color;

    /* discard NULL descriptor */
    if ( d==NULL )
    	return FALSE;

    bzero( buf, MAX_STRING_LENGTH );
    point=buf;
    str=d->outbuf;
    o_color=color;
    o_bold=bold;
    o_flash=flash;

    while ( *str != '\0' && (str-d->outbuf)<d->outtop )
    {
    	if ( (int)(point-buf)>=MAX_STRING_LENGTH-32 )
    	{
    	    /* buffer is full, so send it through the socket */
    	    *point++='\0';
    	    if ( !(ok=write_to_descriptor( d->descriptor,
    	    				   buf,
    	    				   strlen( buf ) )) )
    	        break;
    	    bzero( buf, MAX_STRING_LENGTH );
    	    point=buf;
    	}

    	if ( *str != '#' )
    	{
    	    color_code=FALSE;
    	    *point++ = *str++;
    	    continue;
    	}

    	if ( !color_code && *(str+1)!='<' )
    	{
    	    o_color=color;
    	    o_bold=bold;
    	    o_flash=flash;
    	}
    	color_code=TRUE;

    	act=FALSE;
    	str++;
    	switch ( *str )
    	{
    	    default:    sprintf( buf2, "#%c", *str ); 		       break;
    	    case 'x': 	sprintf( buf2, "#" );		     	       break;
    	    case '-': 	sprintf( buf2, "~" );		     	       break;
    	    case '<': 	color=o_color; bold=o_bold; flash=o_flash;
    	    						     act=TRUE; break;
    	    case '0':	color=0;	 		     act=TRUE; break;
    	    case '1':	color=1; 			     act=TRUE; break;
    	    case '2':	color=2; 			     act=TRUE; break;
    	    case '3':	color=3;	 		     act=TRUE; break;
    	    case '4':	color=4;		 	     act=TRUE; break;
    	    case '5':	color=5; 			     act=TRUE; break;
    	    case '6':	color=6;		 	     act=TRUE; break;
    	    case '7':	color=7;			     act=TRUE; break;
    	    case 'B':	bold=TRUE;   			     act=TRUE; break;
    	    case 'b':	bold=FALSE;  			     act=TRUE; break;
    	    case 'F':	flash=TRUE; 		 	     act=TRUE; break;
    	    case 'f':	flash=FALSE; 			     act=TRUE; break;
    	    case 'n':	if ( d->character && IS_ANSI( d->character ) )
    	    		    sprintf( buf2, "%s", ANSI_NORMAL );
    	    		else
    	    		    buf2[0]='\0';
    	    		bold=FALSE; color=7; flash=FALSE;	break;
    	}
	if ( act )
	{
	    if ( d->character && IS_ANSI( d->character ) )
 	    {
	        sprintf( buf2, "%s", color_value_string( color, bold, flash ) );
    	        color_code=TRUE;
	    }
	    else
	        buf2[0]='\0';
        }

        i=buf2;
        str++;
        while ( ( *point = *i ) != '\0' )
            ++point, ++i;
    }

    *point++='\0';
    ok=ok && (write_to_descriptor( d->descriptor, buf, strlen( buf ) ));
    d->outtop=0;

    return ok;
}


/*
 * Append onto an output buffer.
 */
void write_to_buffer( DESCRIPTOR_DATA *d, const char *txt, int length )
{

    int reverse;
    if( d->character != NULL
    && IS_AFFECTED( d->character, AFF_LOOKING_GLASS )
    && d->connected == CON_PLAYING )
	reverse = TRUE;
    else
	reverse = FALSE;

    /*
     * Find length in case caller didn't.
     */
    if ( length <= 0 )
	length = strlen(txt);

    /*
     * Initial \n\r if needed.
     */
    if ( d->outtop == 0 && !d->fcommand )
    {
	d->outbuf[0]	= '\n';
	d->outbuf[1]	= '\r';
	d->outtop	= 2;
    }

    /*
     * Expand the buffer as needed.
     */
    while ( d->outtop + length >= d->outsize )
    {
	char *outbuf;
        if (d->outsize >= 32000)
	{
	    bug("Buffer overflow. Closing.\n\r",0);
	    close_socket(d);
	    return;
 	}
	outbuf      = alloc_mem( 2 * d->outsize );
	strncpy( outbuf, d->outbuf, d->outtop );
	free_mem( d->outbuf, d->outsize );
	d->outbuf   = outbuf;
	d->outsize *= 2;
    }

    /*
     * Copy.
     */
    strcpy( d->outbuf + d->outtop, txt );
    if( reverse )
	reverse_txt( d->outbuf + d->outtop, length );
    d->outtop += length;
    return;
}



/*
 * Lowest level output function.
 * Write a block of text to the file descriptor.
 * If this gives errors on very long blocks (like 'ofind all'),
 *   try lowering the max block size.
 */
bool write_to_descriptor( int desc, char *txt, int length )
{
    int iStart;
    int nWrite;
    int nBlock;

#if defined(macintosh) || defined(MSDOS)
    if ( desc == 0 )
	desc = 1;
#endif

    if ( length <= 0 )
	length = strlen(txt);

    for ( iStart = 0; iStart < length; iStart += nWrite )
    {
	nBlock = UMIN( length - iStart, 4096 );
	if ( ( nWrite = write( desc, txt + iStart, nBlock ) ) < 0 )
	    { perror( "Write_to_descriptor" ); return FALSE; }
    }

    return TRUE;
}



/*
 * Deal with sockets that haven't logged in yet.
 */
void nanny( DESCRIPTOR_DATA *d, char *argument )
{
    DESCRIPTOR_DATA *d_old, *d_next;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *ch;
    OBJ_DATA *fobj; /* For pfile limit bug */
    OBJ_DATA *fobj_next;
    char *pwdnew;
    char *p;
    int iClass;
	int  race, i, type, sn;
    bool fOld;
    int dh, col;
    CHAR_DATA *zombie;
    AFFECT_DATA af;
    char *name=0;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    int z_level;
    int the_num=0;
    int other_num=0;

    while ( isspace(*argument) ) argument++;

    ch = d->character;

    switch ( d->connected )
    {

    default:
	bug( "Nanny: bad d->connected %d.", d->connected );
	close_socket( d );
	return;

    case CON_GET_NAME:
	if ( argument[0] == '\0' )
	{
	    close_socket( d );
	    return;
	}

	argument[0] = UPPER(argument[0]);
	if ( !check_parse_name( argument ) )
	{
	    write_to_buffer( d, "Illegal name, try another.\n\rName: ", 0 );
	    return;
	}

	fOld = load_char_obj( d, argument );
	ch   = d->character;

	if (IS_SET(ch->act, PLR_DENY))
	{
	    sprintf( log_buf, "Denying access to %s@%s.", argument, d->host );
	    log_string( log_buf );
	    write_to_buffer( d, "You are denied access.\n\r", 0 );
	    close_socket( d );
	    return;
	}

	if (check_ban(d->host,BAN_PERMIT) && !IS_SET(ch->act,PLR_PERMIT))
	{
	    write_to_buffer(d,"Your site has been banned from this mud.\n\r",0);
	    close_socket(d);
	    return;
	}

	if ( check_reconnect( d, argument, FALSE ) )
	{
	    fOld = TRUE;
	}
	else
	{
	    if ( arena && !IS_IMMORTAL(ch))
	    {
		write_to_buffer( d, "The game is in arena mode, try again later.\n\r", 0 );
		close_socket( d );
		return;
	    }

	    if ( wizlock && !IS_IMMORTAL(ch))
	    {
		write_to_buffer( d, "The game is wizlocked.\n\r", 0 );
		close_socket( d );
		return;
	    }
	}

	if ( fOld )
	{
	    /* Old player */
	    write_to_buffer( d, "Password: ", 0 );
	    write_to_buffer( d, echo_off_str, 0 );
	    d->connected = CON_GET_OLD_PASSWORD;
	    return;
	}
	else
	{
	    /* New player */
 	    if (newlock)
	    {
                write_to_buffer( d, "The game is newlocked.\n\r", 0 );
                close_socket( d );
                return;
            }

	    if (check_ban(d->host,BAN_NEWBIES))
	    {
		write_to_buffer(d,
		    "New players are not allowed from your site.\n\r",0);
		close_socket(d);
		return;
	    }

	    sprintf( buf, "Did I get that right, %s (Y/N)? ", argument );
	    write_to_buffer( d, buf, 0 );
	    d->connected = CON_CONFIRM_NEW_NAME;
	    return;
	}
	break;

    case CON_GET_OLD_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

	if ( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ))
	{
	    write_to_buffer( d, "Wrong password.\n\r", 0 );
/*
 * This  should hopefully handle the pfile obj->limcount bug with bad pwds
 */
	for (fobj = ch->carrying; fobj != NULL; fobj = fobj_next)
	{
	fobj_next = fobj->next_content;
	fobj->pIndexData->limcount++; /* Increment count due to drop from */
				      /* bad password bug */
	}

	    close_socket( d );
	    return;
	}

	write_to_buffer( d, echo_on_str, 0 );

	if (check_playing(d,(ch->original_name ? ch->original_name :ch->name)))
	    return;

	if ( check_reconnect( d, ch->name, TRUE ) )
	    return;

        free_string(ch->pcdata->logon_time);
	ch->pcdata->logon_time = str_dup(ctime( &current_time ));
	ch->pcdata->logon_time[strlen(ch->pcdata->logon_time)-1] = '\0';

	sprintf( log_buf, "%s@%s has connected.%s",
	(ch->original_name?ch->original_name:ch->name), d->host,
	auto_check_multi(d,d->host) ? " (MULTI-CHAR?)" : "");

	log_string( log_buf );
	login_log( log_buf );

	update_sitetrack(ch,d->host);

	wiznet(log_buf,NULL,NULL,WIZ_SITES,0,get_trust(ch));

	if ( IS_IMMORTAL(ch) )
	{
	    do_help( ch, "imotd" );
	    d->connected = CON_READ_IMOTD;
 	}
	else
	{
	    do_help( ch, "motd" );
	    d->connected = CON_READ_MOTD;
	}
	break;

/* RT code for breaking link */

    case CON_BREAK_CONNECT:
	switch( *argument )
	{
	case 'y' : case 'Y':
            for ( d_old = descriptor_list; d_old != NULL; d_old = d_next )
	    {
		d_next = d_old->next;
		if (d_old == d || d_old->character == NULL)
		    continue;

		if (str_cmp((ch->original_name ? ch->original_name :
			ch->name),d_old->original ?
		    d_old->original->name : (d_old->character->original_name
			? d_old->character->original_name :d_old->character->name)))
		    continue;

		close_socket(d_old);
	    }
	    if (check_reconnect(d,ch->name,TRUE))
	    	return;
	    write_to_buffer(d,"Reconnect attempt failed.\n\rName: ",0);
            if ( d->character != NULL )
            {
                free_char( d->character );
                d->character = NULL;
            }
	    d->connected = CON_GET_NAME;
	    break;

	case 'n' : case 'N':
	    write_to_buffer(d,"Name: ",0);
            if ( d->character != NULL )
            {
                free_char( d->character );
                d->character = NULL;
            }
	    d->connected = CON_GET_NAME;
	    break;

	default:
	    write_to_buffer(d,"Please type Y or N? ",0);
	    break;
	}
	break;

    case CON_CONFIRM_NEW_NAME:
	switch ( *argument )
	{
	case 'y': case 'Y':
	    sprintf( buf, "New character.\n\rGive me a password for %s: %s",
		ch->name, echo_off_str );
	    write_to_buffer( d, buf, 0 );
	    d->connected = CON_GET_NEW_PASSWORD;
	    break;

	case 'n': case 'N':
	    write_to_buffer( d, "Ok, what IS it, then? ", 0 );
	    free_char( d->character );
	    d->character = NULL;
	    d->connected = CON_GET_NAME;
	    break;

	default:
	    write_to_buffer( d, "Please type Yes or No? ", 0 );
	    break;
	}
	break;

    case CON_GET_NEW_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

	if ( strlen(argument) < 5 )
	{
	    write_to_buffer( d,
		"Password must be at least five characters long.\n\rPassword: ",
		0 );
	    return;
	}

	pwdnew = crypt( argument, ch->name );
	for ( p = pwdnew; *p != '\0'; p++ )
	{
	    if ( *p == '~' )
	    {
		write_to_buffer( d,
		    "New password not acceptable, try again.\n\rPassword: ",
		    0 );
		return;
	    }
	}

	free_string( ch->pcdata->pwd );
	ch->pcdata->pwd	= str_dup( pwdnew );
	write_to_buffer( d, "Please retype password: ", 0 );
	d->connected = CON_CONFIRM_NEW_PASSWORD;
	break;

    case CON_CONFIRM_NEW_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

	if ( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
	{
	    write_to_buffer( d, "Passwords don't match.\n\rRetype password: ",
		0 );
	    d->connected = CON_GET_NEW_PASSWORD;
	    return;
	}

	write_to_buffer( d, echo_on_str, 0 );
        write_to_buffer( d, "\n\rChoose your race:\n\r",0);
        col = 0;
        for ( race = 1; race < MAX_PC_RACE; race++ )
	{
	        if (race == race_lookup("angel")
                || race == race_lookup("lich")
                || race == race_lookup("vuuhu")
                || race == race_lookup("ruugrah")
                || race == race_lookup("malefisti")
				|| race == race_lookup("changeling"))
                {
                        continue;
                }

                ( (pc_race_table[race].xpadd == 0) ?
        sprintf(buf,"%-15s [ no extra xp ]",pc_race_table[race].name) :
        sprintf(buf,"%-15s [%3d extra xp ]",pc_race_table[race].name,
        pc_race_table[race].xpadd));
                write_to_buffer(d, buf, 0);
        if (++col == 2)
                {
                write_to_buffer(d, "\n\r",0);
                col = 0;
                }
        else
                write_to_buffer(d, "   ",0);
        }

        write_to_buffer(d,"\n\r",0);
	race = race_lookup(argument);

	write_to_buffer(d,"What is your race (type 'help' for more information)? ",0);
	d->connected = CON_GET_NEW_RACE;
	break;

    case CON_GET_NEW_RACE:
	one_argument(argument,arg);

	if (!strcmp(arg,"help"))
	{
	    argument = one_argument(argument,arg);
	    if (argument[0] == '\0')
		do_help(ch,"race");
	    else
		do_help(ch,argument);
            write_to_buffer(d,
		"\n\rWhat is your race (type 'help' for more information)?",0);
	    break;
  	}

	race = race_lookup(argument);

	if (race == 0 || !race_table[race].pc_race
	|| race == race_lookup("angel")
	|| race == race_lookup("lich")
	|| race == race_lookup("vuuhu")
	|| race == race_lookup("ruugrah")
	|| race == race_lookup("malefisti")
	|| race == race_lookup("changeling"))
	{
	    write_to_buffer(d,"\n\rThat is not a valid race. Choose one of the following:\n\r",0);

        col = 0;
        for ( race = 1; race < MAX_PC_RACE; race++ )
        {

		if (race == race_lookup("angel")
		|| race == race_lookup("lich")
	        || race == race_lookup("vuuhu")
	        || race == race_lookup("ruugrah")
	        || race == race_lookup("malefisti")
		|| race == race_lookup("changeling"))
	 	{
        	        continue;
	        }

                ( (pc_race_table[race].xpadd == 0) ?
        sprintf(buf,"%-15s [ no extra xp ]",pc_race_table[race].name) :
        sprintf(buf,"%-15s [%3d extra xp ]",pc_race_table[race].name,
        pc_race_table[race].xpadd));
                write_to_buffer(d, buf, 0);
        if (++col == 2)
                {
                write_to_buffer(d, "\n\r",0);
                col = 0;
                }
        else
                write_to_buffer(d, "   ",0);
        }
            write_to_buffer(d,"\n\rWhat is your race? (type 'help' for more information)",0);
	    break;
	}
        ch->race = race;

	/* initialize stats */

	for (i = 0; i < MAX_STATS; i++)
	    ch->perm_stat[i] = pc_race_table[race].stats[i];
        ch->race=race;
	ch->affected_by = ch->affected_by|race_table[race].aff;
	ch->imm_flags	= ch->imm_flags|race_table[race].imm;
	ch->res_flags	= ch->res_flags|race_table[race].res;
	ch->vuln_flags	= ch->vuln_flags|race_table[race].vuln;
	ch->form	= race_table[race].form;
	ch->parts	= race_table[race].parts;

	/* add skills */
	for (i = 0; i < 5; i++)
	{
	    if (pc_race_table[race].skills[i] == NULL)
	 	break;
	    sn = skill_lookup(pc_race_table[race].skills[i]);
	    ch->pcdata->learned[sn] = 100;
	}

	ch->size = pc_race_table[race].size;

        write_to_buffer( d, "\n\rWhat is your sex (M/F)? ", 0 );
        d->connected = CON_GET_NEW_SEX;
        break;


    case CON_GET_NEW_SEX:
	switch ( argument[0] )
	{
	case 'm': case 'M': ch->sex = SEX_MALE;
			    ch->pcdata->true_sex = SEX_MALE;
			    break;
	case 'f': case 'F': ch->sex = SEX_FEMALE;
			    ch->pcdata->true_sex = SEX_FEMALE;
			    break;
	default:
	    write_to_buffer( d, "That's not a sex.\n\rWhat IS your sex? ", 0 );
	    return;
	}

        write_to_buffer( d, "\n\rThe following classes are available:\n\r",0);

        col = 0;
        //for ( race = 0; race < MAX_CLASS-1; race++ )
        for ( race = 0; race < MAX_CLASS; race++ )
        {
           ( (class_table[race].xpadd == 0) ? sprintf(buf,"%-15s [ no extra xp ]",class_table[race].name) :
                           sprintf(buf,"%-15s [%3d extra xp ]",class_table[race].name, class_table[race].xpadd));

           write_to_buffer(d, buf, 0);
           if (++col == 2)
            {
                write_to_buffer(d, "\n\r",0);
                col = 0;
            }
           else write_to_buffer(d, "   ",0);
		}
    buf[0]='\0';
    write_to_buffer(d, "\n\rYour race may be one of these classes:\n\r",0 );

	for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
	{
	    if (pc_race_table[ch->race].classes[iClass] == 1)
            {
                strcat( buf, class_table[iClass].name );
                strcat( buf, " " );
            }
	}
	strcat( buf, "\n\r" );
	write_to_buffer( d, buf, 0 );
    write_to_buffer(d,"Choose your class (type 'help' for more infomation): ",0);
	d->connected = CON_GET_NEW_CLASS;
	break;

    case CON_GET_NEW_CLASS:

    one_argument(argument,arg);

	if (!strcmp(arg,"help"))
	{
	    argument=one_argument(argument,arg); // first argument is 'help'
		argument=one_argument(argument,arg); // second is ...

	    if (arg[0] == '\0') do_help(ch,"class");
	    else do_help(ch,arg);

        write_to_buffer(d,"\n\rChoose your class (type 'help' for more information):",0);
	    break;
  	}

	iClass = class_lookup(argument);

	if ( iClass == -1 )
	{
		write_to_buffer( d,"That's not a class.\n\rChoose your class (type 'help' for more information): ", 0 );
	    return;
     }

	if (pc_race_table[ch->race].classes[iClass] != 1)
        {
		  strcpy( buf, "Your race may only be one of these classes:\n\r" );
	      for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
		  {
	        if (pc_race_table[ch->race].classes[iClass] == 1)
			{
                strcat( buf, class_table[iClass].name );
                strcat( buf, " " );
            }
		  }
	      strcat( buf, "\n\r" );
	      write_to_buffer( d, buf, 0 );
          write_to_buffer(d,"What is your class ? (type 'help' for more infomation): ",0);
	      return;
        }

    ch->class = iClass;
    group_add(ch,"class basics",FALSE);
    ch->pcdata->learned[gsn_recall] = 100;
    for (type = 0; weapon_table[type].name != NULL; type++)
      {
            if (class_table[ch->class].weapon == weapon_table[type].vnum)
                ch->pcdata->learned[*weapon_table[type].gsn] = 40;
      }
    SET_BIT(ch->comm,COMM_SHOW_AFFECTS);

	sprintf( log_buf, "%s@%s new player.", ch->name, d->host );
		log_string( log_buf );
		wiznet("Newbie alert!  $N sighted.",ch,NULL,WIZ_NEWBIE,0,0);
	        wiznet(log_buf,NULL,NULL,WIZ_SITES,0,get_trust(ch));
	        ch->perm_stat[STAT_STR] = UMIN(
		dice(3,6) + 3 + pc_race_table[ch->race].stats[STAT_STR],
		pc_race_table[ch->race].max_stats[STAT_STR]);
	        ch->perm_stat[STAT_INT] = UMIN(
		dice(3,6) + 3 + pc_race_table[ch->race].stats[STAT_INT],
		pc_race_table[ch->race].max_stats[STAT_INT]);
	        ch->perm_stat[STAT_WIS] = UMIN (
		dice(3,6) + 3 + pc_race_table[ch->race].stats[STAT_WIS],
		pc_race_table[ch->race].max_stats[STAT_WIS]);
	        ch->perm_stat[STAT_DEX] = UMIN(
	 	dice(3,6) + 3 + pc_race_table[ch->race].stats[STAT_DEX],
		pc_race_table[ch->race].max_stats[STAT_DEX]);
	        ch->perm_stat[STAT_CON] = UMIN(
		dice(3,6) + 3 + pc_race_table[ch->race].stats[STAT_CON],
		pc_race_table[ch->race].max_stats[STAT_CON]);

		write_to_buffer(d,"\n\rTime to roll stats...\n\r",0);
	        write_to_buffer(d, "You only get one train every 5 levels so choose your stats wisely...\n\r", 0);
	        sprintf(buf, "Str: %2d Int: %2d Wis: %2d Dex: %2d Con: %2d Accept these? (Y/N) ",
	        ch->perm_stat[STAT_STR],
	        ch->perm_stat[STAT_INT],
	        ch->perm_stat[STAT_WIS],
	        ch->perm_stat[STAT_DEX],
	        ch->perm_stat[STAT_CON]);
	        write_to_buffer(d, buf, 0);
	        d->connected = CON_ROLLING_STATS;
	        break;


	    case CON_ROLLING_STATS:
	        switch ( argument[0] )
	        {
	        case 'y': case 'Y':
	                {
	                    ch->mod_stat[STAT_STR]=0;
	                    ch->mod_stat[STAT_INT]=0;
	                    ch->mod_stat[STAT_WIS]=0;
	                    ch->mod_stat[STAT_DEX]=0;
	                    ch->mod_stat[STAT_CON]=0;
	                    break;
	                }
	        case 'n': case 'N':
	                {
	        ch->perm_stat[STAT_STR] = UMIN(
		dice(3,6) + 3 + pc_race_table[ch->race].stats[STAT_STR],
		pc_race_table[ch->race].max_stats[STAT_STR]);
	        ch->perm_stat[STAT_INT] = UMIN(
		dice(3,6) + 3 + pc_race_table[ch->race].stats[STAT_INT],
		pc_race_table[ch->race].max_stats[STAT_INT]);
	        ch->perm_stat[STAT_WIS] = UMIN (
		dice(3,6) + 3 + pc_race_table[ch->race].stats[STAT_WIS],
		pc_race_table[ch->race].max_stats[STAT_WIS]);
	        ch->perm_stat[STAT_DEX] = UMIN(
	 	dice(3,6) + 3 + pc_race_table[ch->race].stats[STAT_DEX],
		pc_race_table[ch->race].max_stats[STAT_DEX]);
	        ch->perm_stat[STAT_CON] = UMIN(
		dice(3,6) + 3 + pc_race_table[ch->race].stats[STAT_CON],
		pc_race_table[ch->race].max_stats[STAT_CON]);
	        sprintf(buf, "Str: %2d Int: %2d Wis: %2d Dex: %2d Con: %2d Accept these? (Y/N) ",
	                    ch->perm_stat[STAT_STR],
	                    ch->perm_stat[STAT_INT],
	                    ch->perm_stat[STAT_WIS],
	                    ch->perm_stat[STAT_DEX],
	                    ch->perm_stat[STAT_CON]);
	                    write_to_buffer(d, buf, 0);
	                    return;
	                }
	        default:
	                {
	                    sprintf(buf, "Please type Y or N...\n\rStr: %d Int: %d Wis: %d Dex: %d Con: %d Accept these?(Y/N) ",
	                    ch->perm_stat[STAT_STR],
	                    ch->perm_stat[STAT_INT],
	                    ch->perm_stat[STAT_WIS],
	                    ch->perm_stat[STAT_DEX],
	                    ch->perm_stat[STAT_CON]);
	                    write_to_buffer(d, buf, 0);
	                    return;
	                }
	        }
	        if (pc_race_table[ch->race].align == ALIGN_ANY
		&& class_table[ch->class].align == ALIGN_ANY)
	        {
	            write_to_buffer( d, "\n\r", 2 );
	            write_to_buffer( d, "You may be good, neutral, or evil.\n\r",0);
	            write_to_buffer( d, "Which alignment (G/N/E)? ",0);
	        }
	        else if ((pc_race_table[ch->race].align == ALIGN_GN
		&& class_table[ch->class].align == ALIGN_GN)
	        || (pc_race_table[ch->race].align == ALIGN_ANY
		&& class_table[ch->class].align == ALIGN_GN)
	        || (pc_race_table[ch->race].align == ALIGN_GN
		&& class_table[ch->class].align == ALIGN_ANY))
	        {
	            write_to_buffer( d, "\n\r", 2 );
	            write_to_buffer( d, "You may be good or neutral.\n\r",0);
	            write_to_buffer( d, "Which alignment (G/N)? ",0);
	        }
	        else if ((pc_race_table[ch->race].align == ALIGN_NE
		&& class_table[ch->class].align == ALIGN_NE)
	        || (pc_race_table[ch->race].align == ALIGN_ANY
		&& class_table[ch->class].align == ALIGN_NE)
	        || (pc_race_table[ch->race].align == ALIGN_NE
		&& class_table[ch->class].align == ALIGN_ANY))
	        {
	            write_to_buffer( d, "\n\r", 2 );
	            write_to_buffer( d, "You may be neutral or evil.\n\r",0);
	            write_to_buffer( d, "Which alignment (N/E)? ",0);
	        }
	        else
	        {
	            write_to_buffer( d, "[Hit Return to Continue]", 0);
	        }
	        d->connected = CON_GET_ALIGNMENT;
	        break;



case CON_GET_ALIGNMENT:
        if (pc_race_table[ch->race].align == ALIGN_ANY
	&& class_table[ch->class].align == ALIGN_ANY)
        {
	switch( argument[0])
	{
            case 'g' : case 'G' : ch->alignment = 1000;  break;
	    case 'n' : case 'N' : ch->alignment = 0;	break;
            case 'e' : case 'E' : ch->alignment = -1000; break;
	    default:
		write_to_buffer(d,"That's not a valid alignment.\n\r",0);
		write_to_buffer(d,"Which alignment (G/N/E)? ",0);
		return;
	}
        }
        else if ((pc_race_table[ch->race].align == ALIGN_GE
	&& class_table[ch->class].align == ALIGN_GE)
        || (pc_race_table[ch->race].align == ALIGN_ANY
	&& class_table[ch->class].align == ALIGN_GE)
        || (pc_race_table[ch->race].align == ALIGN_GE
	&& class_table[ch->class].align == ALIGN_ANY))
        {
	switch( argument[0])
	{
            case 'g' : case 'G' : ch->alignment = 1000;  break;
	    case 'e' : case 'E' : ch->alignment = -1000;	break;
	    default:
		write_to_buffer(d,"That's not a valid alignment.\n\r",0);
                write_to_buffer(d,"Which alignment (G/E)? ",0);
		return;
	}
        }
        else if ((pc_race_table[ch->race].align == ALIGN_GN
	&& class_table[ch->class].align == ALIGN_GN)
        || (pc_race_table[ch->race].align == ALIGN_ANY
	&& class_table[ch->class].align == ALIGN_GN)
        || (pc_race_table[ch->race].align == ALIGN_GN
	&& class_table[ch->class].align == ALIGN_ANY))
        {
	switch( argument[0])
	{
            case 'g' : case 'G' : ch->alignment = 1000;  break;
	    case 'n' : case 'N' : ch->alignment = 0;	break;
	    default:
		write_to_buffer(d,"That's not a valid alignment.\n\r",0);
                write_to_buffer(d,"Which alignment (G/N)? ",0);
		return;
	}
        }
        else if ((pc_race_table[ch->race].align == ALIGN_NE
	&& class_table[ch->class].align == ALIGN_NE)
        || (pc_race_table[ch->race].align == ALIGN_ANY
	&& class_table[ch->class].align == ALIGN_NE)
        || (pc_race_table[ch->race].align == ALIGN_NE
	&& class_table[ch->class].align == ALIGN_ANY))
        {
	switch( argument[0])
	{
	    case 'n' : case 'N' : ch->alignment = 0;	break;
            case 'e' : case 'E' : ch->alignment = -1000; break;
	    default:
		write_to_buffer(d,"That's not a valid alignment.\n\r",0);
                write_to_buffer(d,"Which alignment (N/E)? ",0);
		return;
	}
        }
        else if (pc_race_table[ch->race].align == ALIGN_G
	|| class_table[ch->class].align == ALIGN_G)
        {
	write_to_buffer( d, "\n\rYou must be of good alignment.\n\r",0);
                ch->alignment = 1000;
        }
        else if (pc_race_table[ch->race].align == ALIGN_N
	|| class_table[ch->class].align == ALIGN_N
        || (pc_race_table[ch->race].align == ALIGN_GN
	&& class_table[ch->class].align == ALIGN_NE)
        || (pc_race_table[ch->race].align == ALIGN_NE
	&& class_table[ch->class].align == ALIGN_GN))
        {
	write_to_buffer( d, "\n\rYou must be of neutral alignment.\n\r",0);
                ch->alignment = 0;
        }
        else if (pc_race_table[ch->race].align == ALIGN_E
	|| class_table[ch->class].align == ALIGN_E)
        {
	write_to_buffer( d, "\n\rYou must be of evil alignment.\n\r",0);
                ch->alignment = -1000;
        }

	if ( (ch->class != 4) && (ch->class != 4) && (ch->class != 6) )
	{
		write_to_buffer( d, "\n\r", 2 );
        write_to_buffer( d, "You may be lawful, neutral, or chaotic.\n\r",0);
        write_to_buffer( d, "Which ethos (L/N/C)? ",0);
	}
	else
	{
	write_to_buffer(d,"\n\r",0);
        write_to_buffer(d,"[Hit Return to Continue]",0);
	}
        d->connected = CON_GET_ETHOS;
	break;

case CON_GET_ETHOS:
	if ( (ch->class == 3) || (ch->class == 4) || (ch->class == 6) )
	{
		if (ch->class == 4)
		{
			write_to_buffer( d, "\n\rYou must be chaotic.\n\r", 0 );
			ch->pcdata->ethos = -1000;
		}
		else
		{
			write_to_buffer( d, "\n\rYou must be lawful.\n\r",0);
	    	ch->pcdata->ethos = 1000;
		}
	}
	else
	{
	  switch( argument[0])
	  {
        case 'l' : case 'L' : ch->pcdata->ethos = 1000;  break;
        case 'n' : case 'N' : ch->pcdata->ethos = 0;    break;
        case 'c' : case 'C' : ch->pcdata->ethos = -1000; break;
	    default:
                write_to_buffer(d,"That's not a valid ethos.\n\r",0);
                write_to_buffer(d,"Which ethos (L/N/C)? ",0);
		        return;
	  }
	}

	//do_help(ch,"hometown");


	if (ch->alignment > 0)
	{
	    write_to_buffer(d,"Your hometown can be Midgaard or Ofcol.\n\r",0);
		write_to_buffer(d,"Midgaard is suggested, because that is where everyone else will be.\n\r",0);
	    write_to_buffer(d,"Which hometown (M/O)? ",0);
	}
	else if (ch->alignment == 0)
	{
	    write_to_buffer(d,"Your hometown can be Midgaard or New Thalos.\n\r",0);
		write_to_buffer(d,"Midgaard is suggested, because that is where everyone else will be.\n\r",0);
	    write_to_buffer(d,"Which hometown (M/N)? ",0);
	}
	else
	{
	    write_to_buffer(d,"Your hometown can be Midgaard or Arkham\n\r",0);
		write_to_buffer(d,"Midgaard is suggested, because that is where everyone else will be.\n\r",0);
	    write_to_buffer(d,"Which hometown (M/A)? ",0);
	}
	d->connected = CON_DEFAULT_CHOICE;
	break;

case CON_DEFAULT_CHOICE:
	if (ch->alignment > 0)
	{
	    switch(argument[0])
	    {
		case 'M': case 'm': ch->temple = TEMPLE_MIDGAARD; break;
		case 'O': case 'o': ch->temple = TEMPLE_OFCOL; break;
		default:
		write_to_buffer(d,"\n\rThat is not a valid hometown for you.\n\r",0);
		write_to_buffer(d,"Your hometown can be Midgaard or Ofcol.\n\r",0);
		write_to_buffer(d,"Which hometown (M/O)? ",0);
		return;
	    }
	}
	else if (ch->alignment == 0)
	{
	    switch(argument[0])
	    {
		case 'M': case 'm': ch->temple = TEMPLE_MIDGAARD; break;
		case 'N': case 'n': ch->temple = TEMPLE_NEW_THALOS; break;
		default:
		write_to_buffer(d,"\n\rThat is not a valid hometown for you.\n\r",0);
		write_to_buffer(d,"Your hometown can be Midgaard or New Thalos.\n\r",0);
		write_to_buffer(d,"Which hometown (M/N)? ",0);
		return;
	    }
	}
	else
	{
	    switch(argument[0])
	    {
		case 'M': case 'm': ch->temple = TEMPLE_MIDGAARD; break;
		case 'A': case 'a': ch->temple = TEMPLE_ARKHAM; break;
		default:
		write_to_buffer(d,"\n\rThat is not a valid hometown for you.\n\r",0);
		write_to_buffer(d,"Your hometown can be Midgaard or Arkham.\n\r",0);
		write_to_buffer(d,"Which hometown (M/A)? ",0);
		return;
	    }
	}
        write_to_buffer(d,"\n\r[Hit Return to Continue]\n\r",0);
	  d->connected = CON_NEW_CHAR;
    case CON_NEW_CHAR:
	do_help(ch,"motd");
	d->connected = CON_READ_MOTD;
	break;

    case CON_GEN_GROUPS:
	send_to_char("\n\r",ch);
        do_help(ch,"menu choice");
        break;

    case CON_READ_IMOTD:
	write_to_buffer(d,"\n\r",2);
        do_help( ch, "motd" );
        d->connected = CON_READ_MOTD;
	break;

    case CON_READ_MOTD:
        if ( ch->pcdata == NULL || ch->pcdata->pwd[0] == '\0')
        {
            write_to_buffer( d, "Warning! Null password!\n\r",0 );
            write_to_buffer( d, "Please report old password with bug.\n\r",0);
            write_to_buffer( d,
                "Type 'password null <new password>' to fix.\n\r",0);
        }

	write_to_buffer( d,
    "\n\rWelcome to Inahn's Doom.\n\r\n\r",
	    0 );

	if (is_affected(ch, gsn_target) && ch != NULL )
	{
		affect_strip(ch, gsn_target);
		ch->pcdata->target = NULL;
	}

	if ( is_affected(ch,gsn_rot)  && ch != NULL )

	{

		AFFECT_DATA af;

		AFFECT_DATA *paf;

		AFFECT_DATA *paf_next;

		int affectduration = 0, affectmodifier = 0, affectlevel = 0;

		for ( paf = ch->affected; paf != NULL; paf = paf_next )

		{

			paf_next = paf->next;

			if ( paf->type == gsn_rot )

			{

				affectduration = paf->duration;

				affectmodifier = paf->modifier;

				affectlevel = paf->level;

				affect_strip(ch,gsn_rot);

				init_affect(&af);

				af.where		= TO_AFFECTS;

				af.aftype		= AFT_MALADY;

				af.type 		= gsn_rot;

				af.level 		= affectlevel;

				af.duration		= affectduration;

				af.location		= APPLY_CON;

				af.modifier 	= affectmodifier;

				af.owner		= ch;

				af.bitvector 	= 0;

				affect_to_char(ch,&af);

			}

		}

	}


	ch->next	= char_list;
	char_list	= ch;
	d->connected	= CON_PLAYING;
	/*if ((d->host) == ("24-158-15-97.mazo.wi.charter.com"))
	{
		set_extitle(ch,", The Impotent Bastard");
	}*/
	reset_char(ch);

	free_string(ch->pcdata->logon_time);
	ch->pcdata->logon_time = str_dup(ctime( &current_time ));
	ch->pcdata->logon_time[strlen(ch->pcdata->logon_time)-1] = '\0';

	if ( ch->level == 0 )
	{
/*
Let's set up the basic auto-actions players usually set on logging in
*/
	SET_BIT(ch->act,PLR_AUTOASSIST);
	SET_BIT(ch->act,PLR_AUTOEXIT);
	SET_BIT(ch->act,PLR_AUTOGOLD);
	SET_BIT(ch->act,PLR_COLOR);

	send_to_char("Autoassist, autoexit and autogold have been set.\n\r",ch);
	send_to_char("Type 'auto' to see a list of all auto-actions that may be set.\n\r",ch);

    ch->level   = 1;
	ch->exp     = 0;
	ch->hit	= ch->max_hit;
	ch->mana	= ch->max_mana;
	ch->move	= ch->max_move;
	ch->train	 = 3;
	ch->practice = 5;
    ch->original_name = str_dup(ch->name);

	for(col = 0; col <= MAX_FORM; col++)
	    ch->pcdata->forms[col] = 0;
	    ch->pcdata->special = 0;
	    sprintf( buf, "the %s",
		title_table [ch->class] [ch->level]
		[ch->sex == SEX_FEMALE ? 1 : 0] );
	    set_title( ch, buf );

	    char_to_room( ch, get_room_index( TEMPLE_NEWBIE ) );
	    send_to_char("\n\r",ch);
	    do_help(ch,"NEWBIE INFO");
	    send_to_char("\n\r",ch);
	}
	else if ( ch->in_room != NULL )
	{
	    char_to_room( ch, ch->in_room );
	}
	else if ( IS_IMMORTAL(ch) )
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_CHAT ) );
	}
	else
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_TEMPLE ) );
	}

	act( "$n has entered the game.", ch, NULL, NULL, TO_ROOM );

	group_add(ch,"class basics",FALSE);

/* Debug age_mod on backward compatible pfiles...(Ceran) */
	if (ch->pcdata->age_mod == 0)
		get_age_mod(ch);

/* Another real dodgy fix here...this time it has been left in. Problem in
the dehydration and starving code if these lines aren't left in here...
(Ceran)
*/
	dh = ch->pcdata->condition[COND_DEHYDRATED];

	if (ch->pcdata->condition[COND_DEHYDRATED] > 80)
		ch->pcdata->condition[COND_DEHYDRATED] = 0;

/* Set race specific abilities now...(Ceran) */
	if (ch->class != CLASS_WARRIOR)
	{

	sn = skill_lookup("staves");
	if (ch->pcdata->learned[sn] < 2)
		ch->pcdata->learned[sn] = 1;

	sn = skill_lookup("wands");
	if (ch->pcdata->learned[sn] < 2)
		ch->pcdata->learned[sn] = 1;
	}

    	race = ch->race;

/* This is messy...assigns each race special skills...(Ceran) */
	for (i = 0; i < 5; i++)
	{
	    if (pc_race_table[race].skills[i] == NULL)
	 	break;
	    sn = skill_lookup(pc_race_table[race].skills[i]);
	if ((sn == gsn_gaseous_form || sn == gsn_forest_blending
	|| sn == gsn_awareness || sn == gsn_breath_fire
	|| sn == skill_lookup("globe of darkness")
	|| sn == skill_lookup("psionic blast")
	|| sn == gsn_door_bash)
	&& ch->pcdata->learned[sn] == 0)
	{
		ch->pcdata->learned[sn] = 75;
	}
	else if ( sn == gsn_regeneration
	&& ch->pcdata->learned[sn] <= 3)
	{
		ch->pcdata->learned[sn] = 50;
	}
	else if (sn == gsn_tertiary_wield && ch->pcdata->learned[sn] == 0)
	{
		ch->pcdata->learned[sn] = 75;
	}
	else
	{
	    ch->pcdata->learned[sn] = 100;
	}
	}

	ch->pcdata->condition[COND_DEHYDRATED] = dh;

/* Destroy all cabal items...make sure players don't relog with items, can
occur if mud crashes, etc. Don't want cabal items being brought in while
another one exists.
*/
	{
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;

	for (obj = object_list; obj != NULL; obj = obj_next)
	{
	obj_next = obj->next;
	if (obj->carried_by == ch)
		{
		if (isCabalItem(obj))
			{
			extract_obj(obj);
			}
		else if (obj->pIndexData->limtotal != 0 && ch->level < 10)
			extract_obj(obj);
		}
	}
	}
	do_look( ch, "auto" );

	wiznet("$N has left real life behind.",ch,NULL,
		WIZ_LOGINS,WIZ_SITES,get_trust(ch));

	/*
	ch->pcdata->old_room=0;
	sprintf(buf,"$N (%s) has left real life behind.",(ch->cabal>0) ? capitalize(cabal_table[ch->cabal].name) : "Uncaballed");
	wiznet(buf,ch,NULL,WIZ_LOGINS,0,get_trust(ch));
	*/

	if (ch->pet != NULL)
	{
	    char_to_room(ch->pet,ch->in_room);
	    act("$n has entered the game.",ch->pet,NULL,NULL,TO_ROOM);
	}

        if (ch->cabal != 0)
		announce_login(ch);
	for(i=0;i<ch->pcdata->num_zombies;i=i+1)
	{
	z_level=ch->pcdata->zombie_level[i];
    	zombie = create_mobile(get_mob_index(MOB_VNUM_ZOMBIE));
	char_to_room(zombie,ch->in_room);
      if (z_level<51) { the_num = 2; other_num=17; }
      if (z_level>=51) { the_num = 4; other_num=18; }
      if (z_level>=55) { the_num = 5; other_num=18; }
      if (z_level<=60 && z_level>55) { the_num = 6; other_num=18; }
      if (z_level>60) { the_num = 8; other_num=35; }
      zombie->level = z_level;
      zombie->max_hit = (dice(z_level, 15));
      zombie->max_hit += (z_level * 33);
      zombie->hit = zombie->max_hit;
	if (z_level>60) { zombie->max_hit = 5000; zombie->hit = zombie->max_hit; }
     	zombie->damroll += (z_level*1.1);
	zombie->alignment = -1000;
      zombie->damage[DICE_NUMBER] = the_num;
      zombie->damage[DICE_TYPE]   = other_num;
      zombie->damage[DICE_BONUS]  = (z_level-48);
	if(i==0) {
      name=ch->pcdata->zombie_one;
	if(name==NULL){
	do_unread(ch,"");
	break;
	}
      if (!str_cmp(name, "@"))
		continue;
	}
	if(i==1) {
      name=ch->pcdata->zombie_two;
      if (!str_cmp(name, "@"))
		continue;
	}
	if(i==2) {
      name=ch->pcdata->zombie_three;
      if (!str_cmp(name, "@"))
		continue;
	}
	if(i==3) {
      name=ch->pcdata->zombie_four;
      if (!str_cmp(name, "@"))
		continue;
	}
	if(i==4) {
      name=ch->pcdata->zombie_five;
      if (!str_cmp(name, "@"))
		continue;
	}
      sprintf( buf1, "the zombie of %s", name);
      sprintf( buf2, "A zombie of %s is standing here.\n\r", name);
      free_string(zombie->short_descr);
      free_string(zombie->long_descr);
      free_string(zombie->description);
      zombie->short_descr = str_dup(buf1);
      zombie->long_descr = str_dup(buf2);
      sprintf( buf1, "%s", name);
      zombie->description = str_dup(buf1);
	sprintf(buf1, "With a loud rumble the zombie of %s tears itself free from the ground and kneels before you!\n\r", name);
	send_to_char(buf1,ch);
	sprintf(buf1, "With a loud rumble the zombie of %s tears itself free from the ground and kneels before %s!", name, ch->name);
      act(buf1,ch,0,0,TO_ROOM);
      add_follower(zombie,ch);
      zombie->leader = ch;

      init_affect(&af);
      af.where = TO_AFFECTS;
      af.type = gsn_animate_dead;
      af.aftype = AFT_SPELL;
      af.level = 51;
      af.modifier = 0;
      af.location = 0;
      af.duration = -1;
      af.bitvector = AFF_CHARM;
      affect_to_char(zombie,&af);

      }
	do_unread(ch,"");
	/* Temporary for WizLock and NewLock Stats
	 * to be removed when open mud.
	 */
	if (IS_IMMORTAL(ch))
        {
			if (IS_SET(ch->wiznet,WIZ_SECURE))
			{
                REMOVE_BIT(ch->wiznet,WIZ_SECURE);
                do_lockstat(ch, "");
				SET_BIT(ch->wiznet,WIZ_SECURE);
			}
        }
	break;
    }

    return;
}



/*
 * Parse a name for acceptability.
 */
bool check_parse_name( char *name )
{
    /*
     * Reserved words.
     */
    if ( is_name( name,
	"all auto immortal self zzz someone something the you demise balance circle loner honor outlaw rager arcana knight life enforcer ancient empire sylvan builder") )
	return FALSE;

    if (str_cmp(capitalize(name),"Alander") && (!str_prefix("Alan",name)
    || !str_suffix("Alander",name)))
	return FALSE;

    /*
     * Length restrictions.
     */

    if ( strlen(name) <  2 )
	return FALSE;

#if defined(MSDOS)
    if ( strlen(name) >  8 )
	return FALSE;
#endif

#if defined(macintosh) || defined(unix)
    if ( strlen(name) > 12 )
	return FALSE;
#endif

    /*
     * Alphanumerics only.
     * Lock out IllIll twits.
     */
    {
	char *pc;
	bool fIll,adjcaps = FALSE,cleancaps = FALSE;
 	int total_caps = 0;

	fIll = TRUE;
	for ( pc = name; *pc != '\0'; pc++ )
	{
	    if ( !isalpha(*pc) )
		return FALSE;

	    if ( isupper(*pc)) /* ugly anti-caps hack */
	    {
		if (adjcaps)
		    cleancaps = TRUE;
		total_caps++;
		adjcaps = TRUE;
	    }
	    else
		adjcaps = FALSE;

	    if ( LOWER(*pc) != 'i' && LOWER(*pc) != 'l' )
		fIll = FALSE;
	}

	if ( fIll )
	    return FALSE;

	if (cleancaps || (total_caps > (strlen(name)) / 2 && strlen(name) < 3))
	    return FALSE;
    }

    /*
     * Prevent players from naming themselves after mobs.
     */
    {
	extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
	MOB_INDEX_DATA *pMobIndex;
	int iHash;

	for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
	{
	    for ( pMobIndex  = mob_index_hash[iHash];
		  pMobIndex != NULL;
		  pMobIndex  = pMobIndex->next )
	    {
		if ( is_name( name, pMobIndex->player_name ) )
		    return FALSE;
	    }
	}
    }

    return TRUE;
}



/*
 * Look for link-dead player to reconnect.
 */
bool check_reconnect( DESCRIPTOR_DATA *d, char *name, bool fConn )
{
    CHAR_DATA *ch;
    OBJ_DATA *obj;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
	if ( !IS_NPC(ch)
	&&   (!fConn || ch->desc == NULL)
	&&   !str_cmp( (d->character->original_name ?
		d->character->original_name: d->character->name),
		 (ch->original_name?
		ch->original_name : ch->name ) ))
	{
	    if ( fConn == FALSE )
	    {
		free_string( d->character->pcdata->pwd );
		d->character->pcdata->pwd = str_dup( ch->pcdata->pwd );
	    }
	    else
	    {
		free_char( d->character );
		d->character = ch;
		ch->desc	 = d;
		ch->timer	 = 0;
		send_to_char(
		    "Reconnecting. Type replay to see missed tells.\n\r", ch );
		act( "$n has reconnected.", ch, NULL, NULL, TO_ROOM );

/* Limit crap to balance reconnect objects from extracted link object */
	for (obj = ch->carrying; obj != NULL; obj = obj->next_content)
	{
	obj->pIndexData->limcount++;
	}

		sprintf( log_buf, "%s@%s reconnected.", ch->name, d->host );
		log_string( log_buf );
		wiznet("$N groks the fullness of $S link.",
		    ch,NULL,WIZ_LINKS,0,get_trust(ch));
		d->connected = CON_PLAYING;
	    }
	    return TRUE;
	}
    }

    return FALSE;
}



/*
 * Check if already playing.
 */
bool check_playing( DESCRIPTOR_DATA *d, char *name )
{
    DESCRIPTOR_DATA *dold;

    for ( dold = descriptor_list; dold; dold = dold->next )
    {
	if ( dold != d
	&&   dold->character != NULL
	&&   dold->connected != CON_GET_NAME
	&&   dold->connected != CON_GET_OLD_PASSWORD
	&&   !str_cmp( name, dold->original
	         ? dold->original->name : (dold->character->original_name ?
		dold->character->original_name : dold->character->name )) )
	{
	    write_to_buffer( d, "That character is already playing.\n\r",0);
	    write_to_buffer( d, "Do you wish to connect anyway (Y/N)?",0);
	    d->connected = CON_BREAK_CONNECT;
	    return TRUE;
	}
    }

    return FALSE;
}



void stop_idling( CHAR_DATA *ch )
{
    if ( ch == NULL
    ||   ch->desc == NULL
    ||   ch->desc->connected != CON_PLAYING
    ||   ch->was_in_room == NULL
    ||   ch->in_room != get_room_index(ROOM_VNUM_LIMBO))
	return;

    ch->timer = 0;
    char_from_room( ch );
    char_to_room( ch, ch->was_in_room );
    ch->was_in_room	= NULL;
    act( "$n has returned from the void.", ch, NULL, NULL, TO_ROOM );
    return;
}



/*
 * Write to one char.
 */
void send_to_char_bw( const char *txt, CHAR_DATA *ch )
{
    if ( txt != NULL && ch->desc != NULL )
        write_to_buffer( ch->desc, txt, strlen(txt) );
    return;
}

void send_to_char( const char *txt, CHAR_DATA *ch )
{
    const      char    *point;
               char    *point2;
               char    buf[ MAX_STRING_LENGTH*4 ];
               int     skip = 0;

    buf[0] = '\0';
    point2 = buf;
    if( txt && ch->desc )
       {
           if( IS_SET( ch->act, PLR_COLOR ) )
           {
               for( point = txt ; *point ; point++ )
               {
                   if( *point == '{' )
                   {
                       point++;
                       skip = color( *point, ch, point2 );
                       while( skip-- > 0 )
                           ++point2;
                       continue;
                   }
                   *point2 = *point;
                   *++point2 = '\0';
               }
               *point2 = '\0';
               write_to_buffer( ch->desc, buf, point2 - buf );
           }
           else
           {
               for( point = txt ; *point ; point++ )
               {
                   if( *point == '{' )
                   {
                       point++;
                       continue;
                   }
                   *point2 = *point;
                   *++point2 = '\0';
               }
               *point2 = '\0';
               write_to_buffer( ch->desc, buf, point2 - buf );
           }
       }
    return;
}

void send_to_chars( const char *txt, CHAR_DATA *ch, int min, ... )
{
    if ( txt != NULL && ch->desc != NULL )
        write_to_buffer( ch->desc, txt, strlen(txt) );
    return;
}

/*
 * Send a page to one char.
 */
void page_to_char_bw( const char *txt, CHAR_DATA *ch )
{
    if ( txt == NULL || ch->desc == NULL)
        return;

    if (ch->lines == 0 )
    {
	send_to_char(txt,ch);
	return;
    }

#if defined(macintosh)
	send_to_char(txt,ch);
#else
    ch->desc->showstr_head = alloc_mem(strlen(txt) + 1);
    strcpy(ch->desc->showstr_head,txt);
    ch->desc->showstr_point = ch->desc->showstr_head;
    show_string(ch->desc,"");
#endif
}

/*
 * Page to one char, new color version, by Lope.
 */
void page_to_char( const char *txt, CHAR_DATA *ch )
{
    const      char    *point;
               char    *point2;
               char    buf[ MAX_STRING_LENGTH * 4 ];
               int     skip = 0;

    buf[0] = '\0';
    point2 = buf;
    if( txt && ch->desc )
       {
           if( IS_SET( ch->act, PLR_COLOR ) )
           {
               for( point = txt ; *point ; point++ )
               {
                   if( *point == '{' )
                   {
                       point++;
                       skip = color( *point, ch, point2 );
                       while( skip-- > 0 )
                           ++point2;
                       continue;
                   }
                   *point2 = *point;
                   *++point2 = '\0';
               }
               *point2 = '\0';
               ch->desc->showstr_head  = alloc_mem( strlen( buf ) + 1 );
               strcpy( ch->desc->showstr_head, buf );
               ch->desc->showstr_point = ch->desc->showstr_head;
               show_string( ch->desc, "" );
           }
           else
           {
               for( point = txt ; *point ; point++ )
               {
                   if( *point == '{' )
                   {
                       point++;
                       continue;
                   }
                   *point2 = *point;
                   *++point2 = '\0';
               }
               *point2 = '\0';
               ch->desc->showstr_head  = alloc_mem( strlen( buf ) + 1 );
               strcpy( ch->desc->showstr_head, buf );
               ch->desc->showstr_point = ch->desc->showstr_head;
               show_string( ch->desc, "" );
           }
       }
    return;
}

/* string pager */
void show_string(struct descriptor_data *d, char *input)
{
    char buffer[4*MAX_STRING_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    register char *scan, *chk;
    int lines = 0, toggle = 1;
    int show_lines;

    one_argument(input,buf);
    if (buf[0] != '\0')
    {
	if (d->showstr_head)
	{
	    free_mem(d->showstr_head,strlen(d->showstr_head));
	    d->showstr_head = 0;
	}
    	d->showstr_point  = 0;
	return;
    }

    if (d->character)
	show_lines = d->character->lines;
    else
	show_lines = 0;

    for (scan = buffer; ; scan++, d->showstr_point++)
    {
	if (((*scan = *d->showstr_point) == '\n' || *scan == '\r')
	    && (toggle = -toggle) < 0)
	    lines++;

	else if (!*scan || (show_lines > 0 && lines >= show_lines))
	{
	    *scan = '\0';
	    write_to_buffer(d,buffer,strlen(buffer));
	    for (chk = d->showstr_point; isspace(*chk); chk++);
	    {
		if (!*chk)
		{
		    if (d->showstr_head)
        	    {
            		free_mem(d->showstr_head,strlen(d->showstr_head));
            		d->showstr_head = 0;
        	    }
        	    d->showstr_point  = 0;
    		}
	    }
	    return;
	}
    }
    return;
}


/* quick sex fixer */
void fix_sex(CHAR_DATA *ch)
{
    if (ch->sex < 0 || ch->sex > 2)
    	ch->sex = IS_NPC(ch) ? 0 : ch->pcdata->true_sex;
}

void act (const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2,
	  int type)
{
    /* to be compatible with older code */
    act_new(format,ch,arg1,arg2,type,POS_RESTING);
}

void act_new( const char *format, CHAR_DATA *ch, const void *arg1,
	      const void *arg2, int type, int min_pos)
{
    static char * const he_she  [] = { "it",  "he",  "she" };
    static char * const him_her [] = { "it",  "him", "her" };
    static char * const his_her [] = { "its", "his", "her" };

/*    char buf[MAX_STRING_LENGTH];
    char pbuff[MAX_STRING_LENGTH];
    char fname[MAX_INPUT_LENGTH];
    CHAR_DATA *to;
    CHAR_DATA *vch = (CHAR_DATA *) arg2;
    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    OBJ_DATA *obj2 = (OBJ_DATA  *) arg2;
    const char *str;
    const char *i;
    char *point;
*/
    CHAR_DATA          *to;
    CHAR_DATA          *vch = ( CHAR_DATA * ) arg2;
    OBJ_DATA           *obj1 = ( OBJ_DATA  * ) arg1;
    OBJ_DATA           *obj2 = ( OBJ_DATA  * ) arg2;
    const      char    *str;
    char               *i = NULL;
    char               *point;
    char               *pbuff;
    char               buffer[ MAX_STRING_LENGTH*2 ];
    char               buf[ MAX_STRING_LENGTH   ];
    char               fname[ MAX_INPUT_LENGTH  ];
    bool               fColor = FALSE;

    /*
     * Discard null and zero-length messages.
     */
    if ( !format || !*format )
        return;

    /* discard null rooms and chars */
    if (ch == NULL || ch->in_room == NULL)
	return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
        if ( !vch )
        {
            bug( "Act: null vch with TO_VICT.", 0 );
            return;
        }

	if (!vch->in_room)
	    return;

        to = vch->in_room->people;
    }

    for ( ; to ; to = to->next_in_room )
    {
        if ( (!IS_NPC(to) && to->desc == NULL ) // Maybe needs to be code for color?
	||   ( IS_NPC(to) && !HAS_TRIGGER(to, TRIG_ACT) )
        ||    to->position < min_pos )
            continue;

        if ( (type == TO_CHAR) && to != ch )
            continue;
        if ( type == TO_VICT && ( to != vch || to == ch ) )
            continue;
        if ( type == TO_ROOM && to == ch )
            continue;
        if ( type == TO_NOTVICT && (to == ch || to == vch) )
            continue;
        if ( type == TO_IMMINROOM && (!(IS_IMMORTAL(to)) || (to == ch) || !(can_see(to,ch))))
            continue;

        point   = buf;
        str     = format;
        while ( *str != '\0' )
        {
            if ( *str != '$' )
            {
                *point++ = *str++;
                continue;
            }
            fColor = TRUE;
            ++str;
            i = " <@@@> ";
            if( !arg2 && *str >= 'A' && *str <= 'Z' )

            {
                bug( "Act: missing arg2 for code %d.", *str );
                i = " <@@@> ";
            }
            else
            {
                switch ( *str )
                {
                default:  bug( "Act: bad code %d.", *str );
                          i = " <@@@> ";                                break;
                /* Thx alex for 't' idea */
                case 't': i = (char *) arg1;                            break;
                case 'T': i = (char *) arg2;                            break;

                case 'n':
                    	if (ch && to)
                        	i = PERS (ch, to);
                        else
                        	bug("Act: bad code $n for 'ch' or 'to'",0);
			break;
                case 'N':
	        if (vch && to)
                        i = PERS (vch, to);
                else
                        bug("Act: bad code $N for 'vch' or 'to'",0);
			break;

		case 'f' :
			i = (ch->original_name?ch->original_name:ch->name);
			break;
		case 'F' :
			i=(vch->original_name?vch->original_name:vch->name);
			break;
                case 'e': i = he_she  [URANGE(0, ch  ->sex, 2)];        break;
                case 'E': i = he_she  [URANGE(0, vch ->sex, 2)];        break;
                case 'm': i = him_her [URANGE(0, ch  ->sex, 2)];        break;
                case 'M': i = him_her [URANGE(0, vch ->sex, 2)];        break;
                case 's': i = his_her [URANGE(0, ch  ->sex, 2)];        break;
                case 'S': i = his_her [URANGE(0, vch ->sex, 2)];        break;

                case 'p':
                    i = can_see_obj( to, obj1 )
                            ? obj1->short_descr
                            : "something";
                    break;

                case 'P':
                    i = can_see_obj( to, obj2 )
                            ? obj2->short_descr
                            : "something";
                    break;

                case 'd':
                    if ( arg2 == NULL || ((char *) arg2)[0] == '\0' )
                    {
                        i = "door";
                    }
                    else
                    {
                        one_argument( (char *) arg2, fname );
                        i = fname;
                    }
                    break;
                }
            }

            ++str;
            while ( ( *point = *i ) != '\0' )
                ++point, ++i;
        }

        *point++ = '\n';
        *point++ = '\r';
        *point   = '\0';
        buf[0]   = UPPER(buf[0]);
        pbuff    = buffer;
        colorconv( pbuff, buf, to );
        if ( to->desc != NULL )
        write_to_buffer( to->desc, buffer, 0 );
        else
        if ( MOBtrigger )
            mp_act_trigger( buf, to, ch, arg1, arg2, TRIG_ACT );
    }

    return;
}



/*
 * Macintosh support functions.
 */
#if defined(macintosh)
int gofday( struct timeval *tp, void *tzp )
{
    tp->tv_sec  = time( NULL );
    tp->tv_usec = 0;
}
#endif

void logf (char * fmt, ...)
{
	char buf [2*MSL];
	va_list args;
	va_start (args, fmt);
	vsprintf (buf, fmt, args);
	va_end (args);

	log_string (buf);
}

/* Messy hack for login announcements in cabals. */
void announce_login(CHAR_DATA *ch)
{
        char buf1[MAX_STRING_LENGTH];
        char buf2[MAX_STRING_LENGTH];
	char buf3[MAX_STRING_LENGTH];
	char buf4[MAX_STRING_LENGTH];
        CHAR_DATA *wch;

        if (ch->cabal == 0 || ch->cabal > MAX_CABAL)
        return;

switch (ch->cabal)
{
case (CABAL_COVEN):
if (IS_SET(ch->act,PLR_COLOR))
{
sprintf(buf1,"[COVEN] The Seneschal: \x01B[1;37mThe orb of magic hums briefly as %s enters the lands.\x01B[0;37m\n\r",ch->name);
sprintf(buf2,"[COVEN] The Seneschal: \x01B[1;37mThe orb of magic hums briefly as you enter the lands.\x01B[0;37m\n\r");
}
else
{
sprintf(buf1,"[COVEN] The Seneschal: The orb of magic hums briefly as %s enters the lands.\n\r",ch->name);
sprintf(buf2,"[COVEN] The Seneschal: The orb of magic hums briefly as you enter the lands.\n\r");
}
break;
case (CABAL_RAVAGER):
if (IS_SET(ch->act,PLR_COLOR))
{
sprintf(buf1,"[RAVAGER] The Destructor: \x01B[1;37mWelcome %s. You bring us strength.\x01B[0;37m\n\r",ch->name);
sprintf(buf2,"[RAVAGER] The Destructor: \x01B[1;37mWelcome %s. You bring us strength.\x01B[0;37m\n\r",ch->name);
}
else
{
sprintf(buf1,"[RAVAGER] The Destructor: Welcome %s. You bring us strength.\n\r",ch->name);
sprintf(buf2,"[RAVAGER] The Destructor: Welcome %s. You bring us strength.\n\r",ch->name);
}
break;
case (CABAL_PROTECTOR):
if (IS_SET(ch->act,PLR_COLOR))
{
sprintf(buf1,"[PROTECTOR] The Golden Knight: \x01B[1;37mThe Code of Honor gleams as %s enters the Castle.\x01B[0;37m\n\r",ch->name);
sprintf(buf2,"[PROTECTOR] The Golden Knight: \x01B[1;37mThe Code of Honor gleams as %s enters the Castle.\x01B[0;37m\n\r",ch->name);
}
else
{
sprintf(buf1,"[PROTECTOR] The Golden Knight: The Code of Honor gleams as %s enters the Castle.\n\r",ch->name);
sprintf(buf2,"[PROTECTOR] The Golden Knight: The Code of Honor gleams as %s enters the Castle.\n\r",ch->name);
}

break;
case (CABAL_ACADIAN):
if (IS_SET(ch->act,PLR_COLOR))
{
sprintf(buf1,"[ACADIAN] The Grand Assassin: \x01B[1;37m%s has joined the Hunt.\x01B[0;37m\n\r",ch->name);
sprintf(buf2,"[ACADIAN] The Grand Assassin: \x01B[1;37mGreetings %s, the Hunt awaits you.\x01B[0;37m\n\r",ch->name);
}
else
{
sprintf(buf1,"[ACADIAN] The Grand Assassin: %s has joined the Hunt.\n\r",ch->name);
sprintf(buf2,"[ACADIAN] The Grand Assassin: Greetings %s, the Hunt awaits you.\n\r",ch->name);
}

break;
case (CABAL_VIGILANTE):
if (IS_SET(ch->act,PLR_COLOR))
{
sprintf(buf1,"[VIGILANTE] The Enforcer: \x01B[1;37mThe law grows stronger with the arrival of %s.\x01B[0;37m\n\r",ch->name);
sprintf(buf2,"[VIGILANTE] The Enforcer: \x01B[1;37mThe law grows stronger with your arrival.\x01B[0;37m\n\r");
}
else
{
sprintf(buf1,"[VIGILANTE] The Enforcer: The law grows stronger with the arrival of %s.\n\r",ch->name);
sprintf(buf2,"[VIGILANTE] The Enforcer: The law grows stronger with your arrival.\n\r");
}

break;
/*case (CABAL_OUTLAW):
if (IS_SET(ch->act,PLR_COLOR))
{
sprintf(buf1,"[OUTLAW] The Guardian of Chaos: \x01B[1;37mThe encampment of Outlaws increase as %s enters the realm.\x01B[0;37m\n\r",ch->name);
sprintf(buf2,"[OUTLAW] The Guardian of Chaos: \x01B[1;37mThe encampment of Outlaws increases as you enter the lands.\x01B[0;37m\n\r");
}
else
{
sprintf(buf1,"[OUTLAW] The Guardian of Chaos: The encampment of Outlaws increase as %s enters the realm.\n\r",ch->name);
sprintf(buf2,"[OUTLAW] The Guardian of Chaos: The encampment of Outlaws increases as you enter the lands.\n\r");
}

break;
case (CABAL_EMPIRE):
if (IS_SET(ch->act,PLR_COLOR))
{
sprintf(buf1,"[EMPIRE] The Centurion Omegus: \x01B[1;37mImperial power grows as %s enters our realm.\x01B[0;37m\n\r",ch->name);
sprintf(buf2,"[EMPIRE] The Centurion Omegus: \x01B[1;37mImperial power grows as you enter our realm.\x01B[0;37m\n\r");
}
else
{
sprintf(buf1,"[EMPIRE] The Centurion Omegus: Imperial power grows as %s enters our realm.\n\r",ch->name);
sprintf(buf2,"[EMPIRE] The Centurion Omegus: Imperial power grows as you enter our realm.\n\r");
}

break;*/
case (CABAL_BRIAR):
if (IS_SET(ch->act,PLR_COLOR))
{
sprintf(buf1,"[BRIAR] The Warder: \x01B[1;37mThe cause strengthens as %s joins the pack!.\x01B[0;37m\n\r",ch->name);
sprintf(buf2,"[BRIAR] The Warder: \x01B[1;37mGreetings Warder, you strengthen our cause!.\x01B[0;37m\n\r");
}
else
sprintf(buf1,"[BRIAR] The Warder: The cause strengthens as %s joins the pack!\n\r",ch->name);
sprintf(buf2,"[BRIAR] The Warder: Greetings Warder, you strengthen our cause!\n\r");
break;
}
buf3[0] = '\0';
buf4[0] = '\0';
for (wch = char_list; wch != NULL; wch = wch->next)
{
if (wch->cabal != ch->cabal)
        continue;
if (IS_IMMORTAL(ch) && !can_see(wch,ch))
        continue;
        send_to_char(buf1,wch);
}
return;
}


void announce_logout(CHAR_DATA *ch)
{
        char buf1[MAX_STRING_LENGTH];
        char buf2[MAX_STRING_LENGTH];
	char buf3[MAX_STRING_LENGTH];
	char buf4[MAX_STRING_LENGTH];
        CHAR_DATA *wch;

        if (ch->cabal == 0 || ch->cabal > MAX_CABAL)
        return;

switch (ch->cabal)
{
case (CABAL_COVEN):
if (IS_SET(ch->act,PLR_COLOR))
{
sprintf(buf1,"[COVEN] the Seneschal: \x01B[1;37mThe crystal of power dims briefly as %s leaves the lands.\x01B[0;37m\n\r",ch->name);
sprintf(buf2,"[COVEN] the Seneschal: \x01B[1;37mThe crystal of power dims briefly as you leave the lands.\x01B[0;37m\n\r");
}
else
{
sprintf(buf1,"[COVEN] The Seneschal: The crystal of power dims briefly as %s leaves the lands.\n\r",ch->name);
sprintf(buf2,"[COVEN] The Seneschal: The crystal of power dims briefly as you leave the lands.\n\r");
}

break;
case (CABAL_RAVAGER):
if (IS_SET(ch->act,PLR_COLOR))
{
sprintf(buf1,"[RAVAGER] The Destructor: \x01B[1;37mFarewell %s. May your strength return to us soon.\x01B[0;37m\n\r",ch->name);
sprintf(buf2,"[RAVAGER] The Destructor: \x01B[1;37mFarewell %s. May your strength return to us soon.\x01B[0;37m\n\r",ch->name);
}
else
{
sprintf(buf1,"[RAVAGER] The Destructor: Farewell %s. May your strength return to us soon.\n\r",ch->name);
sprintf(buf2,"[RAVAGER] The Destructor: Farewell %s. May your strength return to us soon.\n\r",ch->name);
}

break;
case (CABAL_PROTECTOR):
if (IS_SET(ch->act,PLR_COLOR))
{
sprintf(buf1,"[PROTECTOR] The Golden Knight: \x01B[1;37mFarewell %s, the crusades await your return.\x01B[0;37m\n\r",ch->name);
sprintf(buf2,"[PROTECTOR] The Golden Knight: \x01B[1;37mFarewell, the crusades await your return.\x01B[0;37m\n\r");
}
else
{
sprintf(buf1,"[PROTECTOR] The Golden Knight: Farewell %s, the crusades await your return.\n\r",ch->name);
sprintf(buf2,"[PROTECTOR] The Golden Knight: Farewell, the crusades await your return.\n\r");
}
break;
case (CABAL_ACADIAN):
if (IS_SET(ch->act,PLR_COLOR))
{
sprintf(buf1,"[ACADIAN] The Grand Assassin: \x01B[1;37m%s has left from the Hunt.\x01B[0;37m\n\r",ch->name);
sprintf(buf2,"[ACADIAN] The Grand Assassin: \x01B[1;37mFarewell %s, the Hunt awaits your return.\x01B[0;37m\n\r",ch->name);
}
else
{
sprintf(buf1,"[ACADIAN] The Grand Assassin: %s has left from the Hunt.\n\r",ch->name);
sprintf(buf2,"[ACADIAN] The Grand Assassin: Farewell %s, the Hunt awaits your return.\n\r",ch->name);
}

break;
case (CABAL_VIGILANTE):
if (IS_SET(ch->act,PLR_COLOR))
{
sprintf(buf1,"[VIGILANTE] The Enforcer: \x01B[1;37m%s signs off from duty and leaves the chaotic streets.\x01B[0;37m\n\r",ch->name);
sprintf(buf2,"[VIGILANTE] The Enforcer: \x01B[1;37mMay you return to protect the lawful and protect the streets soon %s.\x01B[0;37m\n\r",ch->name);
}
else
{
sprintf(buf1,"[VIGILANTE] The Enforcer: %s signs off from duty and leaves the chaotic streets.\n\r",ch->name);
sprintf(buf2,"[VIGILANTE] The Enforcer: May you return to protect the lawful and protect the streets soon %s.\n\r",ch->name);
}
break;
/*case (CABAL_OUTLAW):
if (IS_SET(ch->act,PLR_COLOR))
{
sprintf(buf1,"[OUTLAW] The Guardian of Chaos: \x01B[1;37m%s disbands from the outlaw encampments for a while.\x01B[0;37m\n\r",ch->name);
sprintf(buf2,"[OUTLAW] The Guardian of Chaos: \x01B[1;37mYou disband from the outlaw encampments for a while.\x01B[0;37m\n\r");
}
else
{
sprintf(buf1,"[OUTLAW] The Guardian of Chaos: %s disbands from the outlaw encampments for a while.\n\r",ch->name);
sprintf(buf2,"[OUTLAW] The Guardian of Chaos: You disband from the outlaw encampments for a while.\n\r");
}

break;

case (CABAL_EMPIRE):
if (IS_SET(ch->act,PLR_COLOR))
{
sprintf(buf1,"[EMPIRE] The Centurion Omegus: \x01B[1;37mOur conquest is yet assured though %s leaves us.\x01B[0;37m\n\r",ch->name);
sprintf(buf2,"[EMPIRE] The Centurion Omegus: \x01B[1;37mOur conquest is yet assured though you leave us.\x01B[0;37m\n\r");
}
else
{
sprintf(buf1,"[EMPIRE] The Centurion Omegus: Our conquest is yet assured though %s leaves us.\n\r",ch->name);
sprintf(buf2,"[EMPIRE] The Centurion Omegus: Our conquest is yet assured though you leave us.\n\r");
}

break;*/

case (CABAL_BRIAR):
if (IS_SET(ch->act,PLR_COLOR))
{
sprintf(buf1,"[BRIAR] The Warder: \x01B[1;37m%s has left the pack.\x01B[0;37m\n\r",ch->name);
sprintf(buf2,"[BRIAR] The Warder: \x01B[1;37mYou depart from the pack for a while.\x01B[0;37m\n\r");
}
else
{
sprintf(buf1,"[BRIAR] The Warder: %s has left the pack.\n\r",ch->name);
sprintf(buf2,"[BRIAR] The Warder: You depart from the pack for a while.\n\r");
}
break;
}

buf3[0] = '\0';
buf4[0] = '\0';
for (wch = char_list; wch != NULL; wch = wch->next)
{
if (wch->cabal != ch->cabal)
        continue;
if (IS_IMMORTAL(ch) && !can_see(wch,ch))
        continue;
        send_to_char(buf1,wch);
}


return;
}

/* Fun rename function to rename bad named chars on-line...(Ceran) */
void do_rename (CHAR_DATA* ch, char* argument)
{
        char old_name[MAX_INPUT_LENGTH],
             new_name[MAX_INPUT_LENGTH],
             strsave [MAX_INPUT_LENGTH];

        CHAR_DATA* victim;
        FILE* file;

#if defined(unix)
        argument = one_argument(argument, old_name); /* find new/old name */
        one_argument (argument, new_name);

        /* Trivial checks */
        if (!old_name[0])
        {
                send_to_char ("Rename who?\n\r",ch);
                return;
        }

        victim = get_char_world (ch, old_name);

        if (!victim)
        {
                send_to_char ("There is no such a person online.\n\r",ch);
                return;
        }

        if (IS_NPC(victim))
        {
                send_to_char ("You cannot use Rename on NPCs.\n\r",ch);
                return;
        }

        /* allow rename "self new_name",but otherwise only lower level */
        if ( (victim != ch) && (get_trust (victim) >= get_trust (ch)) )
        {
                send_to_char ("You failed.\n\r",ch);
                return;
        }

/* We really don't want to rename someone who's link-dead and end up with
them logging in later and bitching about how their char is missing
(despite it being funny)...(Ceran)
*/
        if (!victim->desc || (victim->desc->connected != CON_PLAYING) )
        {
                send_to_char("They are link-dead.\n\r",ch);
                return;
        }

        if (!new_name[0])
        {
                send_to_char ("Rename to what new name?\n\r",ch);
                return;
        }

        if (!check_parse_name(new_name))
        {
                send_to_char("That new name is illegal..\n\r",ch);
                return;
        }

        /* First, check if the new name is already taken */
    sprintf( strsave, "%s%s.plr", PLAYER_DIR, capitalize( new_name ) );
        fclose (fpReserve);
        file = fopen (strsave, "r");
        if (file)
        {
                send_to_char ("A player with that name already exists.\n\r",ch);
                fclose (file);
        fpReserve = fopen( NULL_FILE, "r" );
                return;
        }

        if (get_char_world(ch,new_name)) /* check for playing level-1 non-saved */
        {
                send_to_char ("A player with the name you specified already exists.\n\r",ch);
                return;
        }
	/* Save char and then rename and move pfile */
        save_char_obj (victim);
        free_string (victim->name);
	free_string (victim->original_name);
        victim->name = str_dup (capitalize(new_name));
	victim->original_name = str_dup (capitalize(new_name));

	save_char_obj(victim);
	sprintf(strsave,"%s%s.plr",PLAYER_DIR,capitalize(victim->original_name));
	remove(strsave);

        send_to_char ("Character renamed.\n\r",ch);

        victim->position = POS_STANDING;
        act ("$n has renamed you to $N!",ch,NULL,victim,TO_VICT);
#endif

return;
}

void do_renam(CHAR_DATA *ch,char *argument)
{
#if defined(unix)
send_to_char("If you want to RENAME an existing player your must type rename in full.\n\r",ch);
send_to_char("rename <current name> <new name>\n\r",ch);
#else
send_to_char("Rename not supported on your platform.\n\r",ch);
#endif
return;
}


int color( char type, CHAR_DATA *ch, char *string )
{
    char	code[ 20 ];
    char	*p = '\0';

    if( IS_NPC( ch ) )
	return( 0 );

    switch( type )
    {
	default:
	    sprintf( code, CLEAR );
	    break;
	case 'x':
	    sprintf( code, CLEAR );
	    break;
	case 'b':
	    sprintf( code, C_BLUE );
	    break;
	case 'c':
	    sprintf( code, C_CYAN );
	    break;
	case 'g':
	    sprintf( code, C_GREEN );
	    break;
	case 'm':
	    sprintf( code, C_MAGENTA );
	    break;
	case 'r':
	    sprintf( code, C_RED );
	    break;
	case 'w':
	    sprintf( code, C_WHITE );
	    break;
	case 'y':
	    sprintf( code, C_YELLOW );
	    break;
	case 'B':
	    sprintf( code, C_B_BLUE );
	    break;
	case 'C':
	    sprintf( code, C_B_CYAN );
	    break;
	case 'G':
	    sprintf( code, C_B_GREEN );
	    break;
	case 'M':
	    sprintf( code, C_B_MAGENTA );
	    break;
	case 'R':
	    sprintf( code, C_B_RED );
	    break;
	case 'W':
	    sprintf( code, C_B_WHITE );
	    break;
	case 'Y':
	    sprintf( code, C_B_YELLOW );
	    break;
	case 'D':
	    sprintf( code, C_D_GREY );
	    break;
	case '*':
	    sprintf( code, "%c", 007 );
	    break;
	case '/':
	    sprintf( code, "%c", 012 );
	    break;
	case '{':
	    sprintf( code, "%c", '{' );
	    break;
	case '|':
	    sprintf( code, "\n\r");
	    break;
	case '1':
		sprintf( code, BG_BLUE );
		break;

    }

    p = code;
    while( *p != '\0' )
    {
	*string = *p++;
	*++string = '\0';
    }

    return( strlen( code ) );
}

void colorconv( char *buffer, const char *txt, CHAR_DATA *ch )
{
    const	char	*point;
		int	skip = 0;

    if( ch->desc && txt )
    {
	if( IS_SET( ch->act, PLR_COLOR ) )
	{
	    for( point = txt ; *point ; point++ )
	    {
		if( *point == '{' )
		{
		    point++;
		    skip = color( *point, ch, buffer );
		    while( skip-- > 0 )
			++buffer;
		    continue;
		}
		*buffer = *point;
		*++buffer = '\0';
	    }
	    *buffer = '\0';
	}
	else
	{
	    for( point = txt ; *point ; point++ )
	    {
		if( *point == '{' )
		{
		    point++;
		    continue;
		}
		*buffer = *point;
		*++buffer = '\0';
	    }
	    *buffer = '\0';
	}
    }
    return;
}

void process_text(CHAR_DATA *ch, char *text)
{
	char obuf[MSL*2], tbuf[MSL], rbuf[MSL];
	bool found = FALSE;
	int len, rcount, i, iloc;
	if(!str_cmp(text,""))
		return;
	if(!str_cmp(text,"exit"))
	{
		ch->pcdata->entering_text = FALSE;
		ch->pcdata->entered_text[0] = '\0';
		ch->pcdata->end_fun = NULL;
		send_to_char("Exiting without saving changes.\n\r",ch);
		return;
	}
	if(!str_cmp(text,"finish"))
	{
		send_to_char("Saving changes and exiting...\n\r",ch);
		if(ch->pcdata->end_fun)
			(*ch->pcdata->end_fun) (ch, ch->pcdata->entered_text);
		ch->pcdata->entering_text = FALSE;
		ch->pcdata->end_fun = NULL;
		return;
	}
	if(!str_cmp(text,"backline"))
	{
                if (ch->pcdata->entered_text == NULL || ch->pcdata->entered_text[0] == '\0')
                        return send_to_char("No lines left to remove.",ch);
                strcpy(obuf,ch->pcdata->entered_text);
                for (len = strlen(obuf); len > 0; len--)
                {
                        if (obuf[len] == '\r')
                        {
                                if (!found)
                                {
                                        if (len > 0)
                                                len--;
                                        found = TRUE;
                                }
                                else
                                {
                                        obuf[len + 1] = '\0';
                                        free_string(ch->pcdata->entered_text);
                                        ch->pcdata->entered_text = str_dup(obuf);
                                        return send_to_char("Line deleted.",ch);
                                }
                        }
                }
                obuf[0] = '\0';
                free_string(ch->pcdata->entered_text);
                ch->pcdata->entered_text = str_dup(obuf);
                send_to_char("Line deleted.",ch);
		return send_to_char(ch->pcdata->entered_text,ch);
	}
	if(!str_cmp(text,"show"))
		return send_to_char(ch->pcdata->entered_text,ch);
        if (text[0] != '\0')
        {
		obuf[0] = '\0';
		if(strlen(text)>80)
		{
			sprintf(tbuf, "%s", text);
			for(i = 1; i<=strlen(text) / 80; i++)
			{
				for(rcount = 80 * i; rcount > (80 * i) - 15; rcount--)
					if(tbuf[rcount] == ' ')
						break;
				if(rcount <= (80 * i) - 15)
					rcount = 80 * i;
				for(iloc = rcount; tbuf[iloc + 1]; iloc++)
					rbuf[iloc - rcount] = tbuf[iloc + 1];
				rbuf[iloc - rcount] = '\0';
				tbuf[rcount] 	= '\n';
				tbuf[rcount+1] 	= '\r';
				tbuf[rcount+2]	= '\0';
				strcat(tbuf,rbuf);
			}
			send_to_char("Over 80 characters, wrapping line.\n\r",ch);
		}
                if (ch->pcdata->entered_text)
	                strcat(obuf,ch->pcdata->entered_text);
		if(strlen(text)<80)
	                strcat(obuf,text);
		else
			strcat(obuf,tbuf);
                strcat(obuf,"\n\r");
        	if(strlen(obuf) >= MSL*2)
                	return send_to_char("String exceeds buffer.\n\r",ch);
                free_string(ch->pcdata->entered_text);
                ch->pcdata->entered_text = str_dup(obuf);
        } else if (text[0] == '\0')
                return send_to_char("Add what?",ch);
        return send_to_char("Line added.",ch);
}

const char *lowstring(const char *i)
{
	static char buf[MAX_STRING_LENGTH];
	char *o;

	for (o = buf; *i && o-buf < sizeof(buf)-1; i++, o++) {
		*o = LOWER(*i);
	}
	*o = '\0';
	return buf;
}

void reverse_txt( char *txt, int length )
{
    int i, state, c;
    char *word_ptr = NULL;
#define WALKTHRU        0
#define INWORD          1
#define INESC           2
    /* walk thru line looking for words, ignore color codes */
    for( i = 0, state = WALKTHRU; i < length; i++, txt++ )
    {
        c = *txt;
        switch(state)
        {
        case WALKTHRU:                  /* just leave in place */
            if( isalnum(c) )            /* start of word? */
            {
                word_ptr = txt;
                state = INWORD;
            }
            else if( c == '\033' )      /* start of color code escape seq? */
                state = INESC;
            break;
        case INESC:                     /* ignore contents of color code seq */
            if( c == 'm' )              /* 'm' marks end of code */
                state = WALKTHRU;
            break;

        case INWORD:                    /* traversing a word */
            if( isalnum(c)
             || (c == '-'  && isalnum(txt[1]) )         /* hypenated */
             || (c == '\'' && isalnum(txt[1]) ) )       /* contraction */
                break;                  /* still in word */

            /* end of word, do the swap */
            reverse_word( word_ptr, txt - word_ptr );
            if( c == '\033' )
                state = INESC;
            else
                state = WALKTHRU;
            word_ptr = NULL;
        }
    }
}

void reverse_word( char *w, int n )
{
    int i, hlen, left, right, up_left, up_right;
    hlen = n / 2;
    n--;
    for( i = 0; i < hlen; i++ )
    {
        left = w[i];
        right = w[n - i];
        up_left = isupper(left) ? TRUE : FALSE;
        up_right = isupper(right) ? TRUE : FALSE;
        if( up_left != up_right )
        {
            if( isupper(left) )
                left += 'a' - 'A';
            else
                left -= 'a' - 'A';
            if( isupper(right) )
                right += 'a' - 'A';
            else
                right -= 'a' - 'A';
        }
        w[i] = right;
        w[n - i] = left;
    }
}

void printf_to_char (CHAR_DATA *ch, char *fmt, ...)
{
	char buf [MAX_STRING_LENGTH];
	va_list args;
	va_start (args, fmt);
	vsprintf (buf, fmt, args);
	va_end (args);

	send_to_char (buf, ch);
}

void sprintf2 (CHAR_DATA *ch, char *fmt, ...)
{
    char buf [MSL];
    va_list args;
    va_start (args, fmt);
    vsnprintf (buf, MSL, fmt, args);
    va_end (args);
    send_to_char (buf, ch);
}

void bugf (char * fmt, ...)
{
	char buf [2*MSL];
	va_list args;
	va_start (args, fmt);
	vsprintf (buf, fmt, args);
	va_end (args);

	bug (buf, 0);
}

