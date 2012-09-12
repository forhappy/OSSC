/*
 * =============================================================================
 *
 *       Filename:  oss_owner.h
 *
 *    Description:  oss owner.
 *
 *        Created:  09/05/2012 09:55:06 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#ifndef _OSS_OWNER_H
# error Never include <ossc/modules/oss_owner.h> directly, use <ossc/client.h> instead.
#endif

#ifndef OSS_OWNER_H
#define OSS_OWNER_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct oss_owner_s oss_owner_t;

struct oss_owner_s {
	char *id;
	char *display_name;
	/**
	 * 返回所有者的ID
	 */
	const char * (*get_id)(oss_owner_t *owner);

	/**
	 * 返回所有者的显示名称
	 */
	const char * (*get_display_name)(oss_owner_t *owner);

	/**
	 * 设置所有者的ID
	 */
	void (*set_id)(oss_owner_t *owner, const char *id);

	/**
	 * 设置所有者的显示名称
	 */
	void (*set_display_name)(oss_owner_t *owner, const char *name);
};

extern oss_owner_t *
owner_initialize(void);

extern oss_owner_t *
owner_initialize_with_id(const char *id,
		const char *name);

extern void
owner_finalize(oss_owner_t *owner);

#endif
