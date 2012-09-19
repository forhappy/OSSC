/*
 * =============================================================================
 *
 *       Filename:  oss_access_control_list.h
 *
 *    Description:  oss_access_control_list.
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
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
 * @defgroup oss_access_control_list_t oss_access_control_list_t
 * @{
 */
 
/** 
 *struct oss_access_control_list_s 内部使用 \n
 *oss_access_control_list_t 开发者使用
 */
typedef struct oss_access_control_list_s oss_access_control_list_t;

/**
 * 表示OSS的访问控制列表（Access Control List, ACL),
 * 包含了一组为指定被授权者(Grantee)分配特定权限(Permission)的集合
 */
struct oss_access_control_list_s {
	oss_owner_t *owner; /**< Bucket 所有者*/
	//oss_grant_t *grants;
	//int grant_number_count;
	char *grant; /**< 授权信息*/

	/**
	 * 返回所有者 Owner
	 * @param acl [in] oss_access_control_list_t 指针
	 * @retval oss_owner_t *
	 * @return 所有这owner
	 */
	oss_owner_t * (*get_owner)(oss_access_control_list_t 
	        *acl);

	/**
	 * 设置所有者Owner
	 * @param acl [in] oss_access_control_list_t 指针
	 * @param owner [in] 所有者Owner结构
	 * @retval void
	 */
	void (*set_owner)(oss_access_control_list_t *acl,
	        oss_owner_t * owner);

	/**
	 * 返回该AccessControlList中包含的所有授权信息Grant
	 * @param acl [in] oss_access_control_list_t 指针
	 * @retval const char *
	 * @return AccessControlList中包含的所有授权信息Grant
	 */
	const char * (*get_grant)(oss_access_control_list_t 
	        *acl);

	/**
	 * 设置所有授权信息Grant
	 * @param acl [in] oss_access_control_list_t 指针
	 * @param grant [in] 所有授权信息内容
	 * @retval void
	 */
	void (*set_grant)(oss_access_control_list_t *acl,
	        const char *grant);

	//void (*grant_permission)(oss_access_control_list_t *acl, 
	//const char *identifier, const char *permission);
	//void (*revoke_all_permissions)(oss_access_control_list_t* acl,
	//const char *identifier);
	//oss_grant_t * (*get_grants)(oss_access_control_list_t *acl);

};

/**
 * oss_access_control_list_t 构造函数
 * @return 返回一个 ACL 结构指针
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
 * @param acl [in] 标识一个ACL的结构指针
 * @retval void
 * @pre acl 必须使用access_control_list_initialize的返回值
 */
extern void 
access_control_list_finalize(oss_access_control_list_t *acl);

/** @} */

#endif
