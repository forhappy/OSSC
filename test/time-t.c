/*
 * =============================================================================
 *
 *       Filename:  time-t.c
 *
 *    Description:  gmtime
 *
 *        Created:  09/09/2012 12:10:06 PM
 *
 *         Author:  Fu Haiping (forhappy), haipingf@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <ossc/client.h>

int main()
{
	printf("%s", oss_get_gmt_time());
	printf("\n");
	printf("%s", oss_get_asctime());
	return 0;
}
