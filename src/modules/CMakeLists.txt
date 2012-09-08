SET(SRC
	oss_abort_multipart_upload_request.c
	oss_access_control_list.c
	oss_bucket.c
	oss_complete_multipart_upload_request.c
	oss_complete_multipart_upload_result.c
	oss_copy_object_request.c
	oss_copy_object_result.c
	oss_generate_presigned_url_request.c
	oss_get_object_request.c
	oss_grant.c
	oss_initiate_multipart_upload_request.c
	oss_initiate_multipart_upload_result.c
	oss_list_multipart_uploads_request.c
	oss_list_objects_request.c
	oss_list_parts_request.c
	oss_multipart_upload.c
	oss_multipart_upload_listing.c
	oss_object.c
	oss_object_listing.c
	oss_object_metadata.c
	oss_object_summary.c
	oss_owner.c
	oss_part_etag.c
	oss_part_listing.c
	oss_part_summary.c
	oss_put_object_result.c
	oss_response_header_overrides.c
	oss_upload_part_request.c
	oss_upload_part_result.c
	)

ADD_LIBRARY(ossmodules STATIC ${SRC})

set_modules_properties(ossmodules)
# ADD_DEPENDENCIES(modules ossutil)
