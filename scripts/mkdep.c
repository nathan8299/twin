/*
 * Originally by Linus Torvalds.
 * Smart CONF_* processing by Werner Almesberger, Michael Chastain.
 * Tweaked and adapted to `twin' Makefile scheme by Massimiliano Ghilardi.
 * 
 * Usage: mkdep [-I<dir> [...]] file ...
 * 
 * 
 * Search each source file for #include "*.h", CONF_* and DEBUG_* symbol
 * references. Inspection is done dynamically, so the programmer doesn't have
 * to worry about correctly maintaining them.
 * Plus, only those symbols referenced are passed to the C compiler!
 * 
 * Create simple dependency lines for #include "*.h".
 * For instances of CONF_* and DEBUG_* generate dependencies like
 * ifeq ($(CONF_SOCKET),y)
 *   CFLAGS_hw_multi.o+=-DCONF_SOCKET
 * endif
 * 
 * 2.3.99-pre1, Andrew Morton <andrewm@uow.edu.au>
 * - Changed so that 'filename.o' depends upon 'filename.[cC]'.  This is so that
 *   missing source files are noticed, rather than silently ignored.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "autoconf.h"

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "Tw/datasizes.h"

/* Current input file */
const char *g_filename;

/*
 * This records all the configuration options seen.
 * In perl this would be a hash, but here it's a long string
 * of values separated by '\0'.  This is simple and
 * extremely fast.
 */
char * str_config  = NULL;
int    size_config = 0;
int    len_config  = 0;
int    hasdep      = 0;

char depname[512];

void do_depname(void)
{
    if (!hasdep) {
	hasdep = 1;
	printf("%s:", depname);
	if (g_filename)
	    printf(" %s", g_filename);
    }
}

/*
 * Grow the configuration string to a desired length.
 * Usually the first growth is plenty.
 */
void grow_config(int len)
{
    while (len_config + len > size_config) {
	if (size_config == 0)
	    size_config = 2048;
	str_config = realloc(str_config, size_config *= 2);
	if (str_config == NULL)
	{ perror("malloc config"); exit(1); }
    }
}



/*
 * Lookup a value in the configuration string.
 */
int is_defined_config(const char * name, int len)
{
    const char * pconfig;
    const char * plast = str_config + len_config - len;
    for ( pconfig = str_config + 1; pconfig < plast; pconfig++ ) {
	if (pconfig[ -1] == '\0'
	    &&  pconfig[len] == '\0'
	    &&  !memcmp(pconfig, name, len))
	    return 1;
    }
    return 0;
}



/*
 * Add a new value to the configuration string.
 */
void define_config(const char * name, int len)
{
    grow_config(len + 1);
    
    memcpy(str_config+len_config, name, len);
    len_config += len;
    str_config[len_config++] = '\0';
}



/*
 * Clear the set of configuration strings.
 */
void clear_config(void)
{
    hasdep = 0;
    len_config = 0;
    define_config("", 0);
}

void dump_config(void)
{
    char *pc = str_config, *end_config = str_config + len_config;
    while (pc && pc+1 < end_config && !*pc && *++pc) {
	printf("ifeq ($(%s),y)\n  CFLAGS_%s+=-D%s\nendif\n", pc, depname, pc);
	pc = memchr(pc, 0, end_config - pc);
    }
}

char str_path[3][512];
int  len_path[3];
int  max_path = 0;
int  limit_path = sizeof(len_path)/sizeof(len_path[0]);

void use_path(const char * name, int len)
{
    if (max_path < limit_path) {
	memcpy(str_path[max_path], name, len);
	if (name[len] != '/')
	    str_path[max_path][len++] = '/';
	len_path[max_path++] = len;
    } else {
	fprintf(stderr, "Mkdep: fatal: max %d -I directives allowed\n", limit_path);
	exit(1);
    }
}

/*
 * Handle an #include line.
 */
void handle_include(const char * name, int len)
{
    int i;
    
    do_depname();
    for (i = 0; i < max_path; i++) {
	memcpy(str_path[i] + len_path[i], name, len);
	str_path[i][len_path[i]+len] = '\0';
	if (access(str_path[i], R_OK) == 0) {
	    printf(" %s", str_path[i]);
	    return;
	}
    }
    printf(" %.*s", len, name);
}



/*
 * Record the use of a CONF_* word.
 */
void use_config(const char * name, int len)
{
    
    if (len == 16 && !memcmp(name, "CONF_THIS_MODULE", len))
	return;
    
    if (is_defined_config(name, len))
	return;
    
    define_config(name, len);
}



/*
 * Macros for stunningly fast map-based character access.
 * __buf is a register which holds the current word of the input.
 * Thus, there is one memory access per sizeof(unsigned long) characters.
 */

#if TW_BYTE_ORDER == TW_LITTLE_ENDIAN
# define next_byte(x) (x >>= 8)
# define current ((unsigned char) __buf)
#else
# define next_byte(x) (x <<= 8)
# define current (__buf >> 8*(sizeof(unsigned long)-1))
#endif

#define GETNEXT { \
	next_byte(__buf); \
	if ((unsigned long) next % sizeof(unsigned long) == 0) { \
		if (next >= end) \
			break; \
		__buf = * (const unsigned long *) next; \
	} \
	next++; \
}

/*
 * State machine macros.
 */
#define CASE(c,label) if (current == c) goto label
#define NOTCASE(c,label) if (current != c) goto label

/*
 * Yet another state machine speedup.
 */
#define MAX2(a,b) ((a)>(b)?(a):(b))
#define MIN2(a,b) ((a)<(b)?(a):(b))
#define MAX4(a,b,c,d) (MAX2(a,MAX2(b,MAX2(c,d))))
#define MIN4(a,b,c,d) (MIN2(a,MIN2(b,MIN2(c,d))))


static int is_alphanum(char c) {
    return
	(c >= '0' && c <= '9') ||
	(c >= 'A' && c <= 'Z') ||
	(c >= 'a' && c <= 'z') ||
	c == '_';
}


/*
 * The state machine looks for (approximately) these Perl regular expressions:
 *
 *    m|\/\*.*?\*\/|
 *    m|\/\/.*|
 *    m|'.*?'|
 *    m|".*?"|
 *    m|#\s*include\s*"(.*?)"|
 *    m|#\s*(if|ifdef)\s*(CONF|DEBUG)_(\w*)|
 *
 * About 98% of the CPU time is spent here, and most of that is in
 * the 'start' paragraph.  Because the current characters are
 * in a register, the start loop usually eats 4 or 8 characters
 * per memory read.  The MAX4 and MIN4 tests dispose of most
 * input characters with 1 or 2 comparisons.
 */
void state_machine(const char * map, const char * end)
{
	const char * next = map;
	const char * map_dot;
	unsigned long __buf = 0;

	for (;;) {
start:
	GETNEXT
__start:
	if (current > MAX4('/','\'','"','#')) goto start;
	if (current < MIN4('/','\'','"','#')) goto start;
	CASE('/',  slash);
	CASE('\'', squote);
	CASE('"',  dquote);
	CASE('#',  pound);
	goto start;

/* // */
slash_slash:
	GETNEXT
	CASE('\n', start);
	NOTCASE('\\', slash_slash);
	GETNEXT
	goto slash_slash;

/* / */
slash:
	GETNEXT
	CASE('/',  slash_slash);
	NOTCASE('*', __start);
slash_star_dot_star:
	GETNEXT
__slash_star_dot_star:
	NOTCASE('*', slash_star_dot_star);
	GETNEXT
	NOTCASE('/', __slash_star_dot_star);
	goto start;

/* '.*?' */
squote:
	GETNEXT
	CASE('\'', start);
	NOTCASE('\\', squote);
	GETNEXT
	goto squote;

/* ".*?" */
dquote:
	GETNEXT
	CASE('"', start);
	NOTCASE('\\', dquote);
	GETNEXT
	goto dquote;

/* #\s* */
pound:
	GETNEXT
	CASE(' ',  pound);
	CASE('\t', pound);
	CASE('i',  pound_i);
	goto __start;

/* #\s*i */
pound_i:
	GETNEXT CASE('f', pound_if);
		NOTCASE('n', __start);
	GETNEXT NOTCASE('c', __start);
	GETNEXT NOTCASE('l', __start);
	GETNEXT NOTCASE('u', __start);
	GETNEXT NOTCASE('d', __start);
	GETNEXT NOTCASE('e', __start);
	goto pound_include;

/* #\s*include\s* */
pound_include:
	GETNEXT CASE('\n', start);
	        CASE(' ',  pound_include);
	        CASE('\t', pound_include);
	map_dot = next;
	CASE('"',  pound_include_dquote);
	goto __start;

/* #\s*include\s*"(.*)" */
pound_include_dquote:
	GETNEXT CASE('\n', start);
	        NOTCASE('"', pound_include_dquote);
	handle_include(map_dot, next - map_dot - 1);
	goto start;

pound_if:
/* #\s*if */
	GETNEXT	CASE('\n', start);
	        CASE('\t', pound_ifdef);
		CASE(' ', pound_ifdef);
	        if (current == 'n') {
		    /* swallow the 'n' in 'ifndef' and treat as 'ifdef' */
		    GETNEXT;
		}
	        NOTCASE('d', __start);
	GETNEXT NOTCASE('e', __start);
	GETNEXT NOTCASE('f', __start);
	goto pound_ifdef;

/*
 * #\s*(if|ifdef)\s*CONF_(\w*)
 *
 * this does not define the word, because it could be inside another
 * conditional (#if 0).  But I do parse the word so that this instance
 * does not count as a use.  -- mec
 */
pound_ifdef:
	CASE('\n', start);
	GETNEXT CASE('C', pound_ifdef_CONF);
	        CASE('D', pound_ifdef_DEBUG);
	goto pound_ifdef;

pound_ifdef_CONF:
	GETNEXT NOTCASE('O', pound_ifdef);
	GETNEXT NOTCASE('N', pound_ifdef);
	GETNEXT NOTCASE('F', pound_ifdef);
	GETNEXT NOTCASE('_', pound_ifdef);
	goto pound_ifdef_CONF_word;

pound_ifdef_CONF_word:
	map_dot = next - 5;
	GETNEXT
	if (is_alphanum(current)) {
	    do {
		GETNEXT
	    } while (is_alphanum(current));
	    use_config(map_dot, next - map_dot - 1);
	}
	goto pound_ifdef;

pound_ifdef_DEBUG:
	GETNEXT NOTCASE('E', pound_ifdef);
	GETNEXT NOTCASE('B', pound_ifdef);
	GETNEXT NOTCASE('U', pound_ifdef);
	GETNEXT NOTCASE('G', pound_ifdef);
	GETNEXT NOTCASE('_', pound_ifdef);
	goto pound_ifdef_DEBUG_word;
	    
pound_ifdef_DEBUG_word:
	map_dot = next - 6;
	GETNEXT
	if (is_alphanum(current)) {
	    do {
		GETNEXT
	    } while (is_alphanum(current));
	    use_config(map_dot, next - map_dot - 1);
	}
	goto __start;
    }
    if (hasdep)
	putchar('\n');
    dump_config();
}



/*
 * Generate dependencies for one file.
 */
void do_depend(const char * filename)
{
    int mapsize;
    int pagesizem1 = getpagesize()-1;
    int fd;
    struct stat st;
    char * map;
    
    fd = open(filename, O_RDONLY);
    if (fd < 0) {
	perror(filename);
	return;
    }
    
    fstat(fd, &st);
    if (st.st_size == 0) {
	fprintf(stderr,"%s is empty\n",filename);
	close(fd);
	return;
    }
    
    mapsize = st.st_size;
    mapsize = (mapsize+pagesizem1) & ~pagesizem1;
    map = mmap(NULL, mapsize, PROT_READ, MAP_PRIVATE, fd, 0);
    if ((long) map == -1) {
	perror("mkdep: mmap");
	close(fd);
	return;
    }
    if ((unsigned long) map % sizeof(unsigned long) != 0)
    {
	fprintf(stderr, "do_depend: map not aligned\n");
	exit(1);
    }
    
    clear_config();
    state_machine(map, map+st.st_size);
    
    munmap(map, mapsize);
    close(fd);
}



/*
 * Generate dependencies for all files.
 */
int main(int argc, char **argv)
{
    int len;
    const char * filename;
    
    while (--argc > 0) {
	filename = *++argv;
	len = strlen(filename);
	memcpy(depname, filename, len+1);
	g_filename = 0;
	if (len > 2 && !memcmp("-I", filename, 2)) {
	    use_path(filename+2, len-2);
	    continue;
	}
	if (len > 2 && filename[len-2] == '.') {
	    if (filename[len-1] == 'c' || filename[len-1] == 'C') {
		depname[len-1] = 'o';
		g_filename = filename;
	    }
	}
	do_depend(filename);
    }
    return 0;
}