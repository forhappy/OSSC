/*
 * =============================================================================
 *
 *       Filename:  test-constants.c
 *
 *    Description:  test constants
 *
 *        Created:  09/06/2012 06:32:49 PM
 *
 *         Author:  Fu Haiping (forhappy), haipingf@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#define _OSS_CONSTANTS_H
#include "oss_constants.h"
#undef _OSS_CONSTANTS_H
#include <stdio.h>

int main()
{
	OSS_HTTP_METHOD method = HEAD;

	printf("%s\n", constants_get_http_method(method));
}
