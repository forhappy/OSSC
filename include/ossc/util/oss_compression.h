/*
 * =============================================================================
 *
 *       Filename:  oss_compression.h
 *
 *    Description:  oss compression utility.
 *
 *        Created:  09/21/2012 04:36:10 PM
 *
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef OSS_COMPRESSION_H
#define OSS_COMPRESSION_H
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OSS_COMPRESSION_MAGIC "OSSC" /**< (OSS-C)ompression */
#define OSS_COMPRESSION_MAGIC_LEN (strlen(OSS_COMPRESSION_MAGIC))
#define OSS_COMPRESSION_VERSION 0x01 /**< 目前压缩文件格式版本为 1  */

#define OSS_CHUNK_SIZE (8 * 1024 * 1024)

typedef struct oss_compression_header_s oss_compression_header_t;

struct oss_compression_header_s {
	char magic[4];  /**< 4 Bytes，魔数，"OSSC"：\117\123\123\103 */
	char version;   /**< 1 Byte，压缩文件格式版本，目前为 1 */
	char algorithm; /**< 1 Bytes，压缩算法，1：LZO，2：LZ4，3：LZF */
	char flag;      /**< 1 Bytes，标志位 */
	char length;    /**< 1 Bytes，首部长度，最长255字节, 该字段便于今后扩展首部 */
	char md5[16];   /**< 16 Bytes，原始文件的MD5值 */
	void *optional; /**< 4 Bytes，可选数据项，今后可能会利用该字段 */
};

extern void
oss_write_compression_header(FILE *fp, char algorithm, char flag, char md5[]);

extern void oss_compress_file(
		const char *infile,
		const char *outfile,
		char algorithm,
		char flag,      /** 0: 不写入源文件的校验值，1:写入源文件的校验值 */
		int level);

void oss_compress_block(
		char *inbuf, size_t inbuf_len, /** 输入参数，必须预先分配空间 */
		char *outbuf, size_t outbuf_len,/** 输出参数，必须预先分配空间 */
		char algorithm, /**< 压缩算法  */
		int level /**< 该压缩算法的压缩等级*/);

extern void oss_compress_block_2nd(
		char *inbuf, size_t inbuf_len, /** 输入参数，必须预先分配空间 */
		char *outbuf, size_t outbuf_len,/** 输出参数，必须预先分配空间 */
		char algorithm, /**< 压缩算法  */
		char flag, /**< 标识位，0 不检查原文件MD5，1 检查原文件MD5值 */
		int level /**< 该压缩算法的压缩等级*/);

#endif // OSS_COMPRESSION_H
