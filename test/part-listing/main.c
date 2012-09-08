/*
 * =============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  test_part_listing.
 *
 *        Created:  09/04/2012 10:40:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_PART_LISTING_H
#include "oss_part_listing.h"
#undef _OSS_PART_LISTING_H


#include <stdio.h>

int main()
{
	oss_part_listing_t *pl;
	pl = part_listing_initialize();
	pl->set_bucket_name(pl, "bucket_name");
	pl->set_key(pl, "key");
	pl->set_upload_id(pl, "upload_id");
	pl->set_storage_class(pl, "storage_class");
	oss_owner_t *initiator;
	initiator = owner_initialize();
	printf("dn = %s, id = %s\n", initiator->display_name, initiator->id);
	initiator->set_display_name(initiator, "display_name");
	initiator->set_id(initiator, "id");
	pl->set_initiator(pl, initiator);
	pl->set_max_parts(pl, 2);
	pl->set_is_truncated(pl, 1);
	pl->set_next_part_number_marker(pl, 3);
	pl->set_part_number_marker(pl, 4);
	oss_owner_t *owner;
	owner = owner_initialize();
	owner->set_display_name(owner, "display_name_owner");
	owner->set_id(owner, "id_owner");

	pl->set_owner(pl, owner);


	int i, n = 3;
	oss_part_summary_t **parts = (oss_part_summary_t **)malloc(sizeof(oss_part_summary_t *) * n);
	for(i = 0; i < n; i++) {
		parts[i] = part_summary_initialize();
		parts[i]->set_etag(parts[i], "etag");
		parts[i]->set_part_number(parts[i], i);
		parts[i]->set_last_modified(parts[i], "last_modified");
		parts[i]->set_size(parts[i], i);
	}
	pl->set_parts(pl, parts, n);
	
	printf("bucket_name = %s\nkey = %s\nupload_id = %s\nstorage_class = %s\nmax_parts = %d\nis_truncated = %d\nnext_part_number_marker = %d\npart_number_marker = %d\ninitiator->display_name = %s\ninitiator->id = %s\nowner->display_name = %s\nowner->id = %s\n", 
			pl->get_bucket_name(pl), pl->get_key(pl), pl->get_upload_id(pl), pl->get_storage_class(pl), pl->get_max_parts(pl), pl->get_is_truncated(pl), pl->get_next_part_number_marker(pl), pl->get_part_number_marker(pl), pl->initiator->display_name, pl->initiator->id, pl->owner->display_name, pl->owner->id);

	initiator = pl->get_initiator(pl);
	printf("initiator->display_name = %s\ninitator->id = %s\n", initiator->display_name, initiator->id);

	printf("initiator->display_name = %s\ninitator->id = %s\n", initiator->get_display_name(initiator), initiator->get_id(initiator));
	owner = pl->get_owner(pl);
	printf("owner->display_name = %s\nowner->id = %s\n", owner->display_name, owner->id);
	
	parts = pl->get_parts(pl, &n);

	for(i = 0; i < n; i++) {
		printf("parts[%d]->etag = %s\tparts[%d]->part_number = %d\nparts[%d]->last_modified = %s\nparts[%d]->size = %ld\n", i, parts[i]->get_etag(parts[i]), i, parts[i]->get_part_number(parts[i]), i, parts[i]->get_last_modified(parts[i]), i, parts[i]->get_size(parts[i]));
	}

	for(i = 0; i < n; i++) {
		part_summary_finalize(parts[i]);
	}
	
	owner_finalize(initiator);
	owner_finalize(owner);
	
	part_listing_finalize(pl);
	return 0;
}
