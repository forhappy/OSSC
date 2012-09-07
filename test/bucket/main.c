/*
 * =============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  test_bucket.
 *
 *        Created:  09/04/2012 10:40:49 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#define _OSS_BUCKET_H
#include "oss_bucket.h"
#undef _OSS_BUCKET_H

#include <stdio.h>

int main()
{
	oss_bucket_t *bucket;
	bucket = bucket_initialize();
	bucket->set_create_date(bucket, "create_date");
	bucket->set_name(bucket, "name");
	oss_owner_t *owner;
	owner = owner_initialize();
	owner->set_display_name(owner, "display_name_owner");
	owner->set_id(owner, "id_owner");
	bucket->set_owner(bucket, owner);

	printf("create_date = %s\nname = %s\nowner->display_name = %s\nowner->id = %s\n", 
			bucket->get_create_date(bucket), bucket->get_name(bucket), bucket->owner->display_name, bucket->owner->id);
	
	//oss_owner_t * owner1 = bucket->get_owner(bucket);

	owner = bucket->get_owner(bucket);

	printf("owner1->display_name = %s\nowner1->id = %s\n", owner->get_display_name(owner), owner->get_id(owner));
	owner->set_id(owner, "dddddddddddd");

	printf("owner1->display_name = %s\nowner1->id = %s\n", owner->get_display_name(owner), owner->get_id(owner));
	

	owner_finalize(owner);
	bucket_finalize(bucket);

	bucket = bucket_initialize_with_name("user_define_name");
	bucket->set_create_date(bucket, "create_date2");
	owner = owner_initialize();
	owner->set_display_name(owner, "display_name_owner2");
	owner->set_id(owner, "id_owner2");
	bucket->set_owner(bucket, owner);

	printf("create_date = %s\nname = %s\nowner->display_name = %s\nowner->id = %s\n", 
			bucket->get_create_date(bucket), bucket->get_name(bucket), bucket->owner->display_name, bucket->owner->id);

	oss_owner_t *owner2 = bucket->get_owner(bucket);
	printf("owner2->display_name = %s\nowner2->id = %s\n", owner2->get_display_name(owner), owner2->get_id(owner));
	

	owner_finalize(owner);
	bucket_finalize(bucket);

	return 0;
}
