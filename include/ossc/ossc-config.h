/*
 * =============================================================================
 *
 *       Filename:  ossc-config.h
 *
 *    Description:  ossc config.
 *
 *        Created:  09/12/2012 09:24:01 AM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef OSSC_CONFIG_H
#define OSSC_CONFIG_H

#include "config.h"

#if defined (HAVE_STDBOOL_H)
#include <stdbool.h>
#else 
typedef unsigned char bool;
#define false (0)
#define FALSE (0)
#define true (!false)
#define TRUE (!FALSE)
#endif

#endif // OSSC_CONFIG_H
