/*
 * =============================================================================
 *
 *       Filename:  client.h
 *
 *    Description:  client interface, you should always include this file if 
 *                  you want to use oss client.
 *
 *        Created:  09/04/2012 03:22:51 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef OSS_INCLUDE_CLIENT_H
#define OSS_INCLUDE_CLIENT_H

#define _OSS_CONSTANTS_H
#include "oss_constants.h"
#undef _OSS_CONSTANTS_H

#define _OSS_CLIENT_H
#include "oss_client.h"
#undef _OSS_CLIENT_H


#define _OSS_BUCKET_H
#include "oss_bucket.h"
#undef _OSS_BUCKET_H


#define _OSS_ABORT_MULTIPART_UPLOAD_REQUEST_H
#include "oss_abort_multipart_upload_request.h"
#undef _OSS_ABORT_MULTIPART_UPLOAD_REQUEST_H

#define _OSS_LIST_OBJECTS_REQUEST_H
#include "oss_list_objects_request.h"
#undef _OSS_LIST_OBJECTS_REQUEST_H

#define _OSS_LIST_PARTS_REQUEST_H
#include <ossc/oss_list_parts_request.h>
#undef _OSS_LIST_PARTS_REQUEST_H

#define _OSS_LIST_MULTIPART_UPLOADS_REQUEST_H
#include "oss_list_multipart_uploads_request.h"
#undef _OSS_LIST_MULTIPART_UPLOADS_REQUEST_H

#define _OSS_INITIATE_MULTIPART_UPLOAD_RESULT_H
#include "oss_initiate_multipart_upload_result.h"
#undef _OSS_INITIATE_MULTIPART_UPLOAD_RESULT_H

#define _OSS_INITIATE_MULTIPART_UPLOAD_REQUEST_H
#include "oss_initiate_multipart_upload_request.h"
#undef _OSS_INITIATE_MULTIPART_UPLOAD_REQUEST_H

#define _OSS_OBJECT_METADATA_H
#include "oss_object_metadata.h"
#undef _OSS_OBJECT_METADATA_H

#endif
