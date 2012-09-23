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
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <ossc/client.h>




static const char *access_id = "ACSfLOiddaOzejOP";   //设置用户帐号
static const char *access_key = "MUltNpuYqE";  //设置用户密码
static const char *endpoint = "storage.aliyun.com";    //设置hostname


bool
_is_folder(const char *path)
{
	struct stat st;
	int ret = stat(path, &st);
	return ret >= 0 && S_ISDIR(st.st_mode);
}


void 
_delete_etag_quotation(char * etag)
{
	size_t etag_len = strlen(etag);
	etag[etag_len - 1] = '\0';
}

void 
_pre_sync_upload(oss_object_listing_t *object_listing, 
		const char *dir,
		oss_client_t *client,
		const char *bucket_name) 
{
	int i;
	struct dirent *dirp = NULL;
	DIR *dp;
	if((dp = opendir(dir)) == NULL) {
		return;
	}
	while((dirp = readdir(dp)) != NULL) {
		if((dirp->d_name)[0] == '.') {
			continue;
		}
		char full_path[100];
		sprintf(full_path, "%s/%s", dir, dirp->d_name);
		if(_is_folder(full_path)) {
			_pre_sync_upload(object_listing, full_path, client, bucket_name);
		} else {
			//printf("full_path = %s\n", full_path);
			FILE *fp = fopen(full_path, "r");
			//size_t file_len = oss_get_file_size(file);
			//char *retbuf = (char *)malloc(sizeof(char) * (file_len + 1));
			//memset(retbuf, 0, file_len + 1);
			//size_t retlen = fread(retbuf, 1, file_len, file);
			//if (retlen != file_len)
			//	fprintf(stderr, "file mode should be set for both read and write\n");
			char *etag = oss_get_file_md5_digest_2nd(full_path);
			for(i = 0; i < object_listing->_counts_summaries; i++) {
				_delete_etag_quotation((object_listing->summaries)[i]->etag);
				(object_listing->summaries)[i]->etag ++;
				if((strcasecmp(etag, (object_listing->summaries)[i]->etag) == 0) &&
						(strcmp(dirp->d_name, (object_listing->summaries)[i]->key) == 0)) {
					break;
				}
			}
			if(i == object_listing->_counts_summaries) {
				client_put_object_from_file(client, bucket_name, dirp->d_name, fp, NULL, NULL);
			}
		}
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
	if(retcode != OK) {
		if(retcode == NO_SUCH_BUCKET) {
			client_create_bucket(client, bucket_name, &retcode);
			object_listing = client_list_objects_with_bucket_name(client, bucket_name, &retcode);
		} else {
			return -1;
		}
	}
	_pre_sync_upload(object_listing, dir, client, bucket_name);
	fd = inotify_init();
	if(fd < 0) {
		return -2;
	}
	return 0;
}

int main()
{
	//int fd;
	int ret;
	ret = oss_sync_upload("/home/wangwei/Documents/lua_learning/qqaazz");
	printf("ret = %d\n", ret);
	return 0;
}
