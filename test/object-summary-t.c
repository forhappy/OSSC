/*
 * =============================================================================
 *
 *       Filename:  object-summary-t.c
 *
 *    Description:  object-summary.
 *
 *        Created:  09/04/2012 10:40:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_OBJECT_SUMMARY_H
#include "oss_object_summary.h"
#undef _OSS_OBJECT_SUMMARY_H

#include <stdio.h>

int main()
{
	oss_object_summary_t *os;
	os = object_summary_initialize();
	os->set_type(os, "type");
	os->set_key(os, "key");
	os->set_last_modified(os, "last_modified");
	os->set_storage_class(os, "storage_class");
	os->set_size(os, 2);
	os->set_etag(os, "etag");
	oss_owner_t *owner;
	owner = owner_initialize();
	owner->set_display_name(owner, "display_name_owner");
	owner->set_id(owner, "id_owner");

	os->set_owner(os, owner);

	printf("type = %s\nkey = %s\nlast_modified = %s\nstorage_class = %s\nsize = %ld\netag = %s\nowner->display_name = %s\nowner->id = %s\n", 
			os->get_type(os), os->get_key(os), os->get_last_modified(os), os->get_storage_class(os), os->get_size(os), os->get_etag(os), os->owner->display_name, os->owner->id);

	owner = os->get_owner(os);
	printf("owner->display_name = %s\nowner->id = %s\n", owner->display_name, owner->id);
	
	object_summary_finalize(os);
	return 0;
}
