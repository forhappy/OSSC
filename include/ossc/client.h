/*
 * =============================================================================
 *
 *       Filename:  client.h
 *
 *    Description:  client interface, you should always include this file if 
 *                  you want to use oss client.
 *
 *        Created:  09/04/2012 03:22:51 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef OSS_INCLUDE_CLIENT_H
#define OSS_INCLUDE_CLIENT_H

#define _OSS_CONSTANTS_H_
#include "constants.h"
#undef _OSS_CONSTANTS_H_

#define _OSS_CLIENT_H
#include "oss_client.h"
#undef _OSS_CLIENT_H


#define _OSS_BUCKET_H
#include "oss_bucket.h"
#undef _OSS_BUCKET_H

#endif
