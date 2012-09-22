/*
 * =============================================================================
 *
 *       Filename:  client_get_object-t.c
 *
 *    Description:  client_get_object testcase
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <ossc/client.h>

int main()
{
	const char *access_id = "ACSGmv8fkV1TDO9L";
	const char *access_key = "BedoWbsJe2";
	unsigned short retcode = 0;
	const char *bucket_name = "bucketname001";
	const char *key = "glib-2.32.4.tar.xz";
	//FILE *file = fopen("proactor.pdf", "r");
	FILE *local_file = fopen(key, "w");
	size_t file_len = 0;//2 * 1024 * 1024;//oss_get_file_size(file);
	void *buffer = NULL;

	//char *buffer = (char *)malloc(sizeof(char) * file_len + 1);
	//memset(buffer, '\0', file_len + 1);
	//fread(buffer, file_len, 1, file);

	oss_client_t *client = client_initialize(access_id, access_key);

	//oss_object_metadata_t *metadata = object_metadata_initialize();

	//metadata->set_content_length(metadata, file_len);
	//metadata->set_content_type(metadata, "application/octet-stream");
	//metadata->set_cache_control(metadata, "no-cache");
	//metadata->set_content_encoding(metadata, "utf-8");
	//metadata->set_content_disposition(metadata, "attachment;");
	//metadata->set_expiration_time(metadata, "Thu, 13 Sep 2012 21:08:42 GMT");

	//oss_client_t *client = client_initialize(access_id, access_key);
	oss_get_object_request_t *request = get_object_request_initialize(bucket_name, key);
	request->set_range(request, 0, 2 * 1024);
	client_get_object_to_file(client, request, local_file, &retcode);
	//client_get_object_to_buffer_2nd(client, request, &buffer, &file_len, NULL);

	//fwrite(buffer, file_len, 1, local_file);
	//printf("length: %d\n", file_len);
	//fclose(file);
	//fclose(local_file);

	//const char *source_bucket_name = "bucketname001";
	//const char *destination_bucket_name = "bucketname002";
	//const char *source_key = "put.png";
	//const char *destination_key = "PUT.png";
	//oss_client_t *client = client_initialize(access_id, access_key);

	//oss_copy_object_result_t *result = client_copy_object_ext(client, source_bucket_name, source_key,
	//		destination_bucket_name, destination_key, NULL);
	//oss_copy_object_request_t *request = copy_object_request_initialize(source_bucket_name, source_key,
	//	destination_bucket_name, destination_key);
	//request->set_modified_since_constraints(request, "Wed, 12 Sep 2012 12:06:55 GMT");
	//oss_copy_object_result_t *result = client_copy_object(client, request, NULL);
	//if (result != NULL) {
	//	printf("ETag: %s", result->get_etag(result));
	//	printf("LastModified: %s\n", result->get_last_modified(result));
	//}
	//oss_object_metadata_t *metadata = client_get_object_metadata(client, bucket_name, key, NULL);
	//printf("ETag: %s\n", metadata->get_etag(metadata));
	//printf("Content-Type: %s\n", metadata->get_content_type(metadata));
	//printf("Content-Length: %d\n", metadata->get_content_length(metadata));

	//client_delete_object(client, source_bucket_name, source_key, &retcode);
	//printf("%d\n", retcode);
	//	oss_delete_multiple_object_request_t *request = 
	//	delete_multiple_object_request_initialize(source_bucket_name, keys, 4, false);

	//client_delete_multiple_object(client, request, &retcode);
}

