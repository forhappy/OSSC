/*
 * =============================================================================
 *
 *       Filename:  oss_sync.c
 *
 *    Description:  synchronous upload.
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/inotify.h>

#include <ossc/client.h>




static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
static const char *access_key = "MUltNpuYqE";  //设置用户密码
static const char *endpoint = "storage.aliyun.com";    //设置hostname


void 
_pre_sync_upload(oss_object_listing_t *object_listing, const char *dir) 
{
	struct dirent *dirp = NULL;
	DIR *dp;
	if((dp = opendir(dir)) == NULL) {
		return;
	}
	while((dirp = readdir(dp)) != NULL) {
		
		char full_path[100];
		sprintf(full_path, "%s/%s", dir, dirp->d_name);
	}
}

int
oss_sync_upload(const char * dir)
{
	int fd;
	const char *bucket_name;
	const char *delimiter;
	unsigned short retcode; 
	if((delimiter = strrchr(dir, '/')) != NULL) {
		bucket_name = delimiter + 1;
	} else {
		bucket_name = dir;
	}
	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
	oss_object_listing_t *object_listing = NULL;
	object_listing = client_list_objects_with_bucket_name(client, bucket_name, &retcode);
	if(retcode == NO_SUCH_BUCKET) {
		client_create_bucket(client, bucket_name, &retcode);
		object_listing = client_list_objects_with_bucket_name(client, bucket_name, &retcode);
		if(retcode != OK) {
			return -1;
		}
	}
	_pre_sync_upload(object_listing, dir);
	fd = inotify_init();
	if(fd < 0) {
		return -2;
	}
	return 0;
}

int main()
{
	//int fd;
	oss_sync_upload("asd/qwe");
	return 0;
}
