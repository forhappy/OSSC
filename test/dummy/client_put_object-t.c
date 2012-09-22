/*
 * =============================================================================
 *
 *       Filename:  client-t.c
 *
 *    Description:  client testcase.
 *
 *        Created:  09/09/2012 02:55:40 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ossc/client.h>
#include <ossc/util/oss_common.h>

int main()
{
	const char *access_id = "ACSGmv8fkV1TDO9L";
	const char *access_key = "BedoWbsJe2";
	const char *bucket_name = "bucketname001";
	const char *key = "put-test.data";
	//FILE *file = fopen("proactor.pdf", "r");
	//size_t file_len = oss_get_file_size(file);
	//unsigned short retcode = 0;

	//char *buffer = (char *)malloc(sizeof(char) * file_len + 1);
	//memset(buffer, '\0', file_len + 1);
	//fread(buffer, file_len, 1, file);

	//oss_client_t *client = client_initialize(access_id, access_key);

	//oss_object_metadata_t *metadata = object_metadata_initialize();
	//metadata->set_content_length(metadata, file_len);
	//metadata->set_content_type(metadata, "application/octet-stream");

	//client_put_object_from_file(client, bucket_name, key, file, metadata, &retcode);
	oss_get_file_md5_digest("mysql-5.1.52.tar.gz");
	//client_finalize(client);
	//fclose(file);
}
