/*
 * =============================================================================
 *
 *       Filename:  oss_time.c
 *
 *    Description:  oss time utility.
 *
 *        Created:  09/09/2012 12:13:42 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <ossc/util/oss_time.h>

const char * oss_get_asctime()
{
	time_t now;
	struct tm *gmt;

	time(&now);
	gmt = gmtime(&now);

	return asctime(gmt);
}

const char * oss_get_gmt_time()
{
	time_t now;
	struct tm *gmt;
	char *time_val;

	time(&now);
	gmt = gmtime(&now);

	time_val = (char *)malloc(sizeof(char) * 65);
	memset(time_val, '\0', 65);

	strftime(time_val, 64, "%a, %d %b %Y %H:%M:%S GMT", gmt);
	return time_val;
}
