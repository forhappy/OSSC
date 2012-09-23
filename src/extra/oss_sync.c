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
			FILE *fp = fopen(full_path, "r");
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
oss_sync_upload(const char * dir, const char *bucket_name)
{
	assert(dir != NULL);
	assert(bucket_name != NULL);
	unsigned short retcode; 
	size_t dir_len = strlen(dir);
	char *tmp_dir = (char *) malloc(sizeof(char) * dir_len + 1);
	strncpy(tmp_dir, dir, dir_len);
	tmp_dir[dir_len] = '\0';
	if(tmp_dir[dir_len - 1] == '/')
		tmp_dir[dir_len - 1] = '\0';
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
	_pre_sync_upload(object_listing, tmp_dir, client, bucket_name);
	return 0;
}

int
_get_dir_file_number(char *dir, int *file_number)
{

	struct dirent *dirp = NULL;
	DIR *dp;
	if(access(dir, F_OK) < 0) {
		if (mkdir(dir, S_IXUSR | S_IRUSR | S_IWUSR) < 0) 
		{
			return -1;
		}
	} 
	if((dp = opendir(dir)) == NULL) {
		return -1;
	}
	while((dirp = readdir(dp)) != NULL) {
		if((dirp->d_name)[0] == '.') {
			continue;
		}
		char full_path[100];
		sprintf(full_path, "%s/%s", dir, dirp->d_name);
		if(_is_folder(full_path)) {
			_get_dir_file_number(full_path, file_number);
		} else {
			(*file_number)++;
		}
	}
	return 0;

}

int
_get_dir_file_etags(const char *dir, char **file_etags, int *file_count)
{
	struct dirent *dirp = NULL;
	DIR *dp;
	if((dp = opendir(dir)) == NULL) {
		return -1;
	}
	while((dirp = readdir(dp)) != NULL) {
		if((dirp->d_name)[0] == '.') {
			continue;
		}
		char full_path[100];
		sprintf(full_path, "%s/%s", dir, dirp->d_name);
		if(_is_folder(full_path)) {
			_get_dir_file_etags(full_path, file_etags, file_count);
		} else {
			file_etags[(*file_count)++] = oss_get_file_md5_digest_2nd(full_path);
		}
	}
	return 0;

}

int 
oss_sync_download(const char *dir, const char *bucket_name)
{
	assert(dir != NULL);
	assert(bucket_name != NULL);
	char **file_etags;
	int file_number = 0;
	int file_count = 0;
	int i, j;
	unsigned short retcode; 
	size_t dir_len = strlen(dir);
	char full_path[100];
	char *tmp_dir = (char *) malloc(sizeof(char) * dir_len + 1);
	strncpy(tmp_dir, dir, dir_len);
	tmp_dir[dir_len] = '\0';
	if(tmp_dir[dir_len - 1] == '/')
		tmp_dir[dir_len - 1] = '\0';
	oss_client_t *client = client_initialize_with_endpoint(access_id, access_key, endpoint);
	oss_object_listing_t *object_listing = NULL;
	object_listing = client_list_objects_with_bucket_name(client, bucket_name, &retcode);
	if(retcode != OK) {
		return -1;
	}
	_get_dir_file_number(tmp_dir, &file_number);
	if(file_number == 0) {
		file_etags = NULL;
	} else {
		file_etags = (char **)malloc(sizeof(char *) * file_number);
		_get_dir_file_etags(tmp_dir, file_etags, &file_count);
	}
	for(i = 0; i < object_listing->_counts_summaries; i++) {
		_delete_etag_quotation((object_listing->summaries)[i]->etag);
		(object_listing->summaries)[i]->etag ++;
		for(j = 0; j < file_number; j++) {
			if((strcasecmp((object_listing->summaries)[i]->etag, file_etags[j]) == 0) || 
					strchr((object_listing->summaries)[i]->key, '/')) {
				break;
			}
		}
		if(j == file_number) {
			oss_get_object_request_t *request = get_object_request_initialize(bucket_name, (object_listing->summaries)[i]->key);
			sprintf(full_path, "%s/%s", tmp_dir, (object_listing->summaries)[i]->key);
			FILE *fp = fopen(full_path, "w");
			client_get_object_to_file(client, request, fp, &retcode);
		}
	}
	return 0;
}

