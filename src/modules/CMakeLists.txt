SET(SRC
	oss_abort_multipart_upload_request.c
	oss_bucket.c
	oss_initiate_multipart_upload_request.c
	oss_initiate_multipart_upload_result.c
	oss_list_multipart_uploads_request.c
	oss_list_objects_request.c
	oss_list_parts_request.c
	oss_multipart_upload.c
	oss_multipart_upload_listing.c
	oss_object_listing.c
	oss_object_metadata.c
	oss_owner.c
	oss_part_etag.c
	oss_part_summary.c
	oss_put_object_result.c
	oss_response_header_overrides.c
	oss_upload_part_request.c
	)

ADD_LIBRARY(modules STATIC ${SRC})

set_modules_properties(modules)
# ADD_DEPENDENCIES(modules ossutil)
