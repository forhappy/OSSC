/*
 * =============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  main testcase.
 *
 *        Created:  09/18/2012 11:18:49 PM
 *
 *         Author:  Fu Haiping (forhappy), haipingf@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include "ossc/oss_extra.h"
#include "ossc/oss_helper.h"

int main()
{
	//const char *access_id = "ACSGmv8fkV1TDO9L";
	//const char *access_key = "BedoWbsJe2";
	
	const char *access_id = "ACSfLOiddaOzejOP";
	const char *access_key = "MUltNpuYqE";

	const char *bucket_name = "bucketname2";
	const char *key = "upload-mt.data";
	const char *local_file = "mysql-5.1.52.tar.gz";
	// const char *local_file = "Fedora-18.iso";
	unsigned short retcode = -1;

	oss_client_t *client = client_initialize(access_id, access_key);
	client_extra_put_object(client, bucket_name, key, local_file, &retcode);
	printf("%s\n", oss_get_error_message_from_retcode(retcode));


}
