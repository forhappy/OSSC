/*
 * =============================================================================
 *
 *       Filename:  oss_helper.c
 *
 *    Description:  oss helper routines.
 *
 *        Created:  09/16/2012 09:41:30 AM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <ossc/client.h>
#include <ossc/oss_helper.h>
#define _OSS_CLIENT_H
#include <ossc/modules/oss_client.h>
#undef _OSS_CLIENT_H


void
oss_free_partial_user_data(curl_request_param_t *user_data) 
{
	if(user_data != NULL) {
		if(user_data->send_buffer != NULL) {
			if(user_data->send_buffer->ptr != NULL) {
				free(user_data->send_buffer->ptr);
				user_data->send_buffer->ptr = NULL;
			}
			free(user_data->send_buffer);
			user_data->send_buffer = NULL;
		}
		if(user_data->header_buffer != NULL) {
			if(user_data->header_buffer->ptr != NULL) {
				free(user_data->header_buffer->ptr);
				user_data->header_buffer->ptr = NULL;
			}
			free(user_data->header_buffer);
			user_data->header_buffer = NULL;
		}
		if(user_data->recv_buffer != NULL) {
			free(user_data->recv_buffer);
		}
		free(user_data);
	}
}

void
oss_free_partial_user_data_2nd(curl_request_param_t *user_data) 
{
	if(user_data != NULL) {
		if (user_data->send_buffer != NULL) free(user_data->send_buffer);
		if(user_data->recv_buffer != NULL) {
			if(user_data->recv_buffer->ptr != NULL) {
				free(user_data->recv_buffer->ptr);
				user_data->recv_buffer->ptr = NULL;
			}
			free(user_data->recv_buffer);
			user_data->recv_buffer= NULL;
		}
		if(user_data->header_buffer != NULL) {
			if(user_data->header_buffer->ptr != NULL) {
				free(user_data->header_buffer->ptr);
				user_data->header_buffer->ptr = NULL;
			}
			free(user_data->header_buffer);
			user_data->header_buffer = NULL;
		}
		free(user_data);
	}
}

void
oss_free_user_data(curl_request_param_t *user_data) 
{
	if(user_data != NULL) {
		if(user_data->send_buffer != NULL) {
			if(user_data->send_buffer->ptr != NULL) {
				free(user_data->send_buffer->ptr);
				user_data->send_buffer->ptr = NULL;
			}
			free(user_data->send_buffer);
			user_data->send_buffer = NULL;
		}
		if(user_data->recv_buffer != NULL) {
			if(user_data->recv_buffer->ptr != NULL) {
				free(user_data->recv_buffer->ptr);
				user_data->recv_buffer->ptr = NULL;
			}
			free(user_data->recv_buffer);
			user_data->recv_buffer = NULL;
		}
		if(user_data->header_buffer != NULL) {
			if(user_data->header_buffer->ptr != NULL) {
				free(user_data->header_buffer->ptr);
				user_data->header_buffer->ptr = NULL;
			}
			free(user_data->header_buffer);
			user_data->header_buffer = NULL;
		}
		free(user_data);
	}
}

const char *
oss_get_error_message_from_retcode(unsigned short retcode)
{
	if (retcode == OK) {
		return "OK";
	} else if(retcode == ACCESS_DENIED) {
		return "AccessDenied";
	} else if(retcode == BUCKET_ALREADY_EXISTS) {
		return "BucketAlreadyExists";
	} else if(retcode == BUCKET_NOT_EMPTY) {
		return "BucketNotEmpty";
	} else if(retcode == ENTITY_TOO_LARGE) {
		return "EntityTooLarge";
	} else if(retcode == ENTITY_TOO_SMALL) {
		return "EntityTooSmall";
	} else if(retcode == FILE_GROUP_TOO_LARGE) {
		return "FileGroupTooLarge";
	} else if(retcode == FILE_PART_NOT_EXIST) {
		return "FilePartNotExist";
	} else if(retcode == FILE_PART_STALE) {
		return "FilePartStale";
	} else if(retcode == INTERNAL_ERROR) {
		return "InternalError";
	} else if(retcode == INVALID_ACCESS_KEY_ID) {
		return "InvalidAccessKeyId";
	} else if(retcode == INVALID_ARGUMENT) {
		return "InvalidArgument";
	} else if(retcode == INVALID_BUCKET_NAME) {
		return "InvalidBucketName";
	} else if(retcode == INVALID_DIGEST) {
		return "InvalidDigest";
	} else if(retcode == INVALID_OBJECT_NAME) {
		return "InvalidObjectName";
	} else if(retcode == INVALID_PART) {
		return "InvalidPart";
	} else if(retcode == INVALID_PART_ORDER) {
		return "InvalidPartOrder";
	} else if(retcode == MALFORMED_XML) {
		return "MalformedXML";
	} else if(retcode == METHOD_NOT_ALLOWED) {
		return "MethodNotAllowed";
	} else if(retcode == MISSING_ARGUMENT) {
		return "MissingArgument";
	} else if(retcode == MISSING_CONTENT_LENGTH) {
		return "MissingContentLength";
	} else if (retcode == NO_CONTENT) {
		return "NoContent";
	}else if(retcode == NO_SUCH_BUCKET) {
		return "NoSuchBucket";
	} else if(retcode == NO_SUCH_KEY) {
		return "NoSuchKey";
	} else if(retcode == NO_SUCH_UPLOAD) {
		return "NoSuchUpload";
	} else if(retcode == NOT_IMPLEMENTED) {
		return "NotImplemented";
	} else if(retcode == PRECONDITION_FAILED) {
		return "PreconditionFailed";
	} else if(retcode == REQUEST_TIME_TOO_SKEWED) {
		return "RequestTimeTooSkewed";
	} else if(retcode == REQUEST_TIMEOUT) {
		return "RequestTimeout";
	} else if(retcode ==SIGNATURE_DOES_NOT_MATCH) {
		return "SignatureDoesNotMatch";
	} else if(retcode == TOO_MANY_BUCKETS) {
		return "TooManyBuckets";
	} else if(retcode == FILE_PART_INTERITY) {
		return "FilePartInterity";
	} else if(retcode == FILE_NOT_FOUND) {
		return "FileNotFound";
	} else if(retcode == NOT_MODIFIED) {
		return "NotModified";
	} else if(retcode == FILE_TOO_LARGE) {
		return "FileTooLarge";
	} else if(retcode == IO_ERROR) {
		return "IOError";
	} else {
		return "UnknowError";
	}
}

unsigned short
oss_get_retcode_from_response(const char *response)
{
	unsigned short ret;
	assert(response != NULL);
	XmlNode *xml, *code_tag;
	char *retinfo;
	unsigned int response_len = strlen(response); 
	xml = xml_load_buffer(response, response_len);

	code_tag = xml_find(xml, "Code");
	retinfo = *code_tag->child->attrib;
	ret = oss_get_retcode_from_retinfo(retinfo);
	xml_free(xml);
	return ret;
}

unsigned short
oss_get_retcode_from_retinfo(const char *retinfo) 
{
	OSS_ERROR_CODE retcode;
	if(strcmp(retinfo, "AccessDenied") == 0) {
		retcode = ACCESS_DENIED;
	} else if(strcmp(retinfo, "BucketAlreadyExists") == 0) {
		retcode = BUCKET_ALREADY_EXISTS;
	} else if(strcmp(retinfo, "BucketNotEmpty") == 0) {
		retcode = BUCKET_NOT_EMPTY;
	} else if(strcmp(retinfo, "EntityTooLarge") == 0) {
		retcode = ENTITY_TOO_LARGE;
	} else if(strcmp(retinfo, "EntityTooSmall") == 0) {
		retcode = ENTITY_TOO_SMALL;
	} else if(strcmp(retinfo, "FileGroupTooLarge") == 0) {
		retcode = FILE_GROUP_TOO_LARGE;
	} else if(strcmp(retinfo, "FilePartNotExist") == 0) {
		retcode = FILE_PART_NOT_EXIST;
	} else if(strcmp(retinfo, "FilePartStale") == 0) {
		retcode = FILE_PART_STALE;
	} else if(strcmp(retinfo, "InternalError") == 0) {
		retcode = INTERNAL_ERROR;
	} else if(strcmp(retinfo, "InvalidAccessKeyId") == 0) {
		retcode = INVALID_ACCESS_KEY_ID;
	} else if(strcmp(retinfo, "InvalidArgument") == 0) {
		retcode = INVALID_ARGUMENT;
	} else if(strcmp(retinfo, "InvalidBucketName") == 0) {
		retcode = INVALID_BUCKET_NAME;
	} else if(strcmp(retinfo, "InvalidDigest") == 0) {
		retcode = INVALID_DIGEST;
	} else if(strcmp(retinfo, "InvalidObjectName") == 0) {
		retcode = INVALID_OBJECT_NAME;
	} else if(strcmp(retinfo, "InvalidPart") == 0) {
		retcode = INVALID_PART;
	} else if(strcmp(retinfo, "InvalidPartOrder") == 0) {
		retcode = INVALID_PART_ORDER;
	} else if(strcmp(retinfo, "MalformedXML") == 0) {
		retcode = MALFORMED_XML;
	} else if(strcmp(retinfo, "MethodNotAllowed") == 0) {
		retcode = METHOD_NOT_ALLOWED;
	} else if(strcmp(retinfo, "MissingArgument") == 0) {
		retcode = MISSING_ARGUMENT;
	} else if(strcmp(retinfo, "MissingContentLength") == 0) {
		retcode = MISSING_CONTENT_LENGTH;
	} else if(strcmp(retinfo, "NoSuchBucket") == 0) {
		retcode = NO_SUCH_BUCKET;
	} else if(strcmp(retinfo, "NoSuchKey") == 0) {
		retcode = NO_SUCH_KEY;
	} else if(strcmp(retinfo, "NoSuchUpload") == 0) {
		retcode = NO_SUCH_UPLOAD;
	} else if(strcmp(retinfo, "NotImplemented") == 0) {
		retcode = NOT_IMPLEMENTED;
	} else if(strcmp(retinfo, "PreconditionFailed") == 0) {
		retcode = PRECONDITION_FAILED;
	} else if(strcmp(retinfo, "RequestTimeTooSkewed") == 0) {
		retcode = REQUEST_TIME_TOO_SKEWED;
	} else if(strcmp(retinfo, "RequestTimeout") == 0) {
		retcode = REQUEST_TIMEOUT;
	} else if(strcmp(retinfo, "SignatureDoesNotMatch") == 0) {
		retcode = SIGNATURE_DOES_NOT_MATCH;
	} else if(strcmp(retinfo, "TooManyBuckets") == 0) {
		retcode = TOO_MANY_BUCKETS;
	} else if(strcmp(retinfo, "FilePartInterity") == 0) {
		retcode = FILE_PART_INTERITY;
	} else if(strcmp(retinfo, "FileNotFound") == 0) {
		retcode = FILE_NOT_FOUND;
	} else if(strcmp(retinfo, "NotModified") == 0) {
		retcode = NOT_MODIFIED;
	} else if(strcmp(retinfo, "FileTooLarge") == 0) {
		retcode = FILE_TOO_LARGE;
	} else if(strcmp(retinfo, "IOError") == 0) {
		retcode = IO_ERROR;
	}else {
		retcode = 1000;
	}
	return retcode;
}

