/*
 * =============================================================================
 *
 *       Filename:  oss_decompression.h
 *
 *    Description:  oss decompression utility
 *
 *        Created:  09/21/2012 10:58:48 PM
 *
 *         Author:  Fu Haiping (forhappy), haipingf@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#ifndef OSS_DECOMPRESSION_H
#define OSS_DECOMPRESSION_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern oss_compression_header_t *
oss_read_compression_header(FILE *fp);

int
oss_decompress_block(
		char *inbuf, unsigned int inbuf_len, /** 输入参数，必须预先分配空间 */
		char *outbuf, unsigned int outbuf_len,/** 输出参数，必须预先分配空间 */
		char algorithm /** 压缩算法 */
		);

int
oss_decompress_block_2nd(
		char *inbuf, unsigned int inbuf_len, /** 输入参数，必须预先分配空间 */
		char *outbuf, unsigned int outbuf_len/** 输出参数，必须预先分配空间 */
		);

void oss_decompress_file(
		const char *infile,
		const char *outfile);

#endif // OSS_DECOMPRESSION_H
