/*
 * =============================================================================
 *
 *       Filename:  oss_common.c
 *
 *    Description:  oss common routines.
 *
 *        Created:  09/15/2012 10:15:33 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <stdlib.h>
#include <string.h>

#include <ossc/util/oss_common.h>

long
oss_get_file_size(FILE *fp)
{
    long int save_pos;
    long size_of_file;
    /* Save the current position. */
    save_pos = ftell(fp);
    /* Jump to the end of the file. */
    fseek(fp, 0L, SEEK_END);
    /* Get the end position. */
    size_of_file = ftell(fp);
    /* Jump back to the original position. */
    fseek(fp, save_pos, SEEK_SET);
    return size_of_file;
}

const char *
oss_compute_md5_digest(void *ptr, size_t len)
{
	char md5_digest[17];
	md5_state_t md5_state;

	char *base64_md5 = NULL;

	memset(md5_digest, '\0', 17);

	md5_init(&md5_state);
	md5_append(&md5_state, ptr, len);
	md5_finish(&md5_state, (md5_byte_t *)md5_digest);

	base64_md5 = (char *) malloc(sizeof(char) * 65);
	memset(base64_md5, 0, 65);
	base64_encode(md5_digest, 16, base64_md5, 65);

	return base64_md5;
}

