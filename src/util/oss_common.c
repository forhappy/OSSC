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

#include <lib/md5.h>
#include <lib/base64.h>
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

char *
oss_get_file_md5_digest(const char *file)
{
	char *md5_digest = NULL;
	char *md5buf = NULL;

	FILE *fp = fopen(file, "rb");
	if (fp == NULL) {
		fprintf(stderr, "error occured when opening file %s\n", file);
		return NULL;
	}

	md5_digest = (char *)malloc(sizeof(char) * 17);
	memset(md5_digest, '\0', 17);

	md5buf = (char *)malloc(sizeof(char) * MD5_BUFFER_SIZE);
	memset(md5buf, 0, MD5_BUFFER_SIZE);

	md5_state_t md5_state;
	md5_init(&md5_state);

	while (1) {
		size_t ret = fread(md5buf, 1, MD5_BUFFER_SIZE, fp);
		if (ret == MD5_BUFFER_SIZE) {
			md5_append(&md5_state, (const md5_byte_t *)md5buf, MD5_BUFFER_SIZE);
			continue;
		} else {
			if (ferror(fp) == 0) {
				md5_append(&md5_state, (const md5_byte_t *)md5buf, ret);
				break;
			} else {
				fprintf(stderr, "error occured when reading file %s\n", file);
				break;
			}
		}
	}

	md5_finish(&md5_state, (md5_byte_t *)md5_digest);

	fclose(fp);
	free(md5buf);
	
	return md5_digest;
}

char *
oss_get_file_md5_digest_2nd(const char *file)
{
	char *md5_digest = NULL;
	char *md5buf = NULL;
	char *md5_digest32 = NULL;
	char tmpbytes[3] = {0};
	int i = 0;

	FILE *fp = fopen(file, "rb");
	if (fp == NULL) {
		fprintf(stderr, "error occured when opening file %s\n", file);
		return NULL;
	}

	md5_digest = (char *)malloc(sizeof(char) * 17);
	memset(md5_digest, '\0', 17);

	md5buf = (char *)malloc(sizeof(char) * MD5_BUFFER_SIZE);
	memset(md5buf, 0, MD5_BUFFER_SIZE);

	md5_state_t md5_state;
	md5_init(&md5_state);

	while (1) {
		size_t ret = fread(md5buf, 1, MD5_BUFFER_SIZE, fp);
		if (ret == MD5_BUFFER_SIZE) {
			md5_append(&md5_state, (const md5_byte_t *)md5buf, MD5_BUFFER_SIZE);
			continue;
		} else {
			if (ferror(fp) == 0) {
				md5_append(&md5_state, (const md5_byte_t *)md5buf, ret);
				break;
			} else {
				fprintf(stderr, "error occured when reading file %s\n", file);
				break;
			}
		}
	}

	md5_finish(&md5_state, (md5_byte_t *)md5_digest);
	
	md5_digest32 = (char *) malloc(sizeof(char) * 33);
	memset(md5_digest32, 0, 33);

	for (i = 0; i < 16; i++) {
		sprintf(tmpbytes, "%02x", (unsigned char)md5_digest[i]);
		sprintf(md5_digest32 + i * 2, "%s", tmpbytes);
	}
	fclose(fp);
	free(md5_digest);
	free(md5buf);

	return md5_digest32;

	
}

char *
oss_get_buffer_md5_digest(void *ptr, size_t len)
{
	char *md5_digest = (char *)malloc(sizeof(char) * 17);
	md5_state_t md5_state;

	memset(md5_digest, '\0', 17);

	md5_init(&md5_state);
	md5_append(&md5_state, (md5_byte_t *)ptr, len);
	md5_finish(&md5_state, (md5_byte_t *)md5_digest);
	
	return md5_digest;
}

char *
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

