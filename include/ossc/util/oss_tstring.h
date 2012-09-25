/*
 * =============================================================================
 *
 *       Filename:  tstring.h
 *
 *    Description:  tiny string utility implementation.
 *
 *        Created:  08/26/2012 05:24:11 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef UTILITY_TSTRING_H
#define UTILITY_TSTRING_H

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
typedef	bool bool_t;
# define TRUE true
# define FALSE false
#else
typedef int bool_t;
# define TRUE	(1)
# define FALSE	(!TRUE)
#endif

/*
 * min()/max()/clamp() macros that also do
 * strict type-checking.. See the
 * "unnecessary" pointer comparison.
 */
#define min(x, y) ({						\
			__typeof__(x) _min1 = (x);			\
			__typeof__(y) _min2 = (y);			\
			(void) (&_min1 == &_min2);		\
			_min1 < _min2 ? _min1 : _min2; })

#define max(x, y) ({						\
			__typeof__(x) _max1 = (x);			\
			__typeof__(y) _max2 = (y);			\
			(void) (&_max1 == &_max2);		\
			_max1 > _max2 ? _max1 : _max2; })

typedef struct _tstring_t tstring_t;
struct _tstring_t {
    char *str;
    unsigned int len;
    unsigned int allocated_len;
};

extern tstring_t * tstring_new(const char *init);
extern tstring_t * tstring_new_len(const char *init, int32_t len);
extern tstring_t * tstring_sized_new(uint32_t default_size);
extern void tstring_free(tstring_t *str);
extern int tstring_equal(const tstring_t *str1, const tstring_t *str2);
extern uint32_t tstring_hash(const tstring_t *str);
extern tstring_t * tstring_assign(tstring_t *str, const char *rval);
extern tstring_t * tstring_truncate(tstring_t *str, int32_t len);    
extern tstring_t * tstring_set_size(tstring_t *str, int32_t len);
extern tstring_t * tstring_insert_len(tstring_t *str, int32_t pos, const char *val, int32_t len);  
extern tstring_t * tstring_append(tstring_t *str, const char *val);
extern tstring_t * tstring_append_len(tstring_t *str, const char *val, int32_t len);  
extern tstring_t * tstring_append_c(tstring_t *str, char c);
extern tstring_t * tstring_append_unichar(tstring_t *str, unsigned int wc);
extern tstring_t * tstring_prepend(tstring_t *str, const char *val);
extern tstring_t * tstring_prepend_c(tstring_t *str, char c);
extern tstring_t * tstring_prepend_unichar(tstring_t *str, unsigned int wc);
extern tstring_t * tstring_prepend_len(tstring_t *str, const char *val, int32_t len);  
extern tstring_t * tstring_insert(tstring_t *str, int32_t pos, const char *val);
extern tstring_t * tstring_insert_c(tstring_t *str, int32_t pos, const char c);
extern tstring_t * tstring_insert_unichar(tstring_t *str, int32_t pos, unsigned int wc);
extern tstring_t * tstring_overwrite(tstring_t *str, int32_t pos, const char *val);
extern tstring_t * tstring_overwrite_len(tstring_t *str, int32_t pos, const char *val, int32_t len);
extern tstring_t * tstring_erase(tstring_t *str, int32_t pos, int32_t len);
extern tstring_t * tstring_ascii_down(tstring_t *str);
extern tstring_t * tstring_ascii_up(tstring_t *str);
extern void tstring_vprintf(tstring_t *str, const char *format, va_list args);
extern void tstring_printf(tstring_t *str, const char *format, ...) __attribute__((format(printf, 2, 3)));
extern void tstring_append_vprintf(tstring_t *str, const char *format, va_list args);
extern void tstring_append_printf(tstring_t *str, const char *format, ...) __attribute__((format(printf, 2, 3)));
extern tstring_t * tstring_down(tstring_t *str);
extern tstring_t * tstring_up(tstring_t *str);

/* optimize tstring_append_c */
static inline tstring_t *
tstring_append_c_inline(tstring_t *str, char c)
{
  if (str->len + 1 < str->allocated_len) {
      str->str[str->len++] = c;
      str->str[str->len] = 0;
  } else
    tstring_insert_c(str, -1, c);
  return str;
}
#define tstring_append_c(str,c)     tstring_append_c_inline(str, c)

static inline uint32_t tstring_size(const tstring_t *str)
{
    assert(str != NULL);

    return str->len;
}

static inline char * tstring_data(const tstring_t *str)
{
    assert(str != NULL);

    return str->str;
}

#endif // UTILITY_TSTRING_H
