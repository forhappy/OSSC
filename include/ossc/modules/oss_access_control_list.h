/*
 * =============================================================================
 *
 *       Filename:  oss_access_control_list.h
 *
 *    Description:  oss_access_control_list.
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 *
 * =============================================================================
 */
 /* 内部使用的头文件，用户开发应包含<ossc/client.h> */
#ifndef _OSS_ACCESS_CONTROL_LIST_H
# error Never include <ossc/modules/oss_access_control_list.h> directly, use <ossc/client.h> instead.
#endif

/* 头文件保护 */
#ifndef OSS_ACCESS_CONTROL_LIST_H
#define OSS_ACCESS_CONTROL_LIST_H
#include "ossc-config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define _OSS_OWNER_H
#include "ossc/modules/oss_owner.h"
#undef _OSS_OWNER_H

#define _OSS_GRANT_H
#include "ossc/modules/oss_grant.h"
#undef _OSS_GRANT_H

/**
 * @addtogroup oss_access_control_list_t oss_access_control_list_t
 * @{
 */
 
/** 
 *struct oss_access_control_list_s 内部使用 \n
 *oss_access_control_list_t 开发者使用
 */
typedef struct oss_access_control_list_s oss_access_control_list_t;

/**
 *oss_access_control_list_t 标识访问控制列表
 */
struct oss_access_control_list_s {
	oss_owner_t *owner; /**< Bucket 所有者*/
	//oss_grant_t *grants;
	//int grant_number_count;
	char *grant; /**< 授权信息*/


	oss_owner_t * (*get_owner)(oss_access_control_list_t 
	        *acl);/**< 返回 Owner*/
	void (*set_owner)(oss_access_control_list_t *acl,
	        oss_owner_t * owner);/**< 设置 Owner*/

	const char * (*get_grant)(oss_access_control_list_t 
	        *acl);/**< 返回授权信息*/
	void (*set_grant)(oss_access_control_list_t *acl,
	        const char *grant);/**< 返回授权信息*/

	//void (*grant_permission)(oss_access_control_list_t *acl, 
	//const char *identifier, const char *permission);
	//void (*revoke_all_permissions)(oss_access_control_list_t* acl,
	//const char *identifier);
	//oss_grant_t * (*get_grants)(oss_access_control_list_t *acl);

};

/**
 * oss_access_control_list_t 构造函数
 * @return 返回一个 ACL 结构
 * @retval 非空 表示成功
 * @retval NULL 表示失败
 * @note 用户不需要句柄后要调用相应的finalize函数释放空间
 * @code
 * oss_owner_t *owner = owner_initialize();
 * oss_access_control_list_t * acl = access_control_list_initialize();
 * acl->set_owner(acl, owner);
 * @endcode
 */
extern oss_access_control_list_t *
access_control_list_initialize(void);

/**
 * oss_access_control_list_t 析构函数
 * @param access_control_list [in] 标识一个ACL的句柄
 * @retval void
 * @pre access_control_list 必须使用access_control_list_initialize的返回值
 */
extern void 
access_control_list_finalize(oss_access_control_list_t *access_control_list);

/** @} */

#endif
