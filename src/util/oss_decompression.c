/*
 * =============================================================================
 *
 *       Filename:  oss_decompression.c
 *
 *    Description:  oss decompression utility.
 *
 *        Created:  09/21/2012 10:59:41 PM
 *
 *         Author:  Fu Haiping (forhappy), haipingf@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <stdio.h>

#include <lib/lz4.h>
#include <lib/lz4hc.h>
#include <ossc/util/oss_common.h>
#include <ossc/util/oss_compression.h>
#include <ossc/util/oss_decompression.h>

static const int one = 1;

#define CPU_LITTLE_ENDIAN (*(char*)(&one))
#define CPU_BIG_ENDIAN (!CPU_LITTLE_ENDIAN)
#define LITTLE_ENDIAN32(i)   if (CPU_BIG_ENDIAN) { i = swap32(i); }

static inline unsigned int swap32(unsigned int x)
{
	return ((x << 24) & 0xff000000 ) |
		((x <<  8) & 0x00ff0000 ) |
		((x >>  8) & 0x0000ff00 ) |
		((x >> 24) & 0x000000ff );
}

oss_compression_header_t *
oss_read_compression_header(FILE *fp)
{
	assert(fp != NULL);
	size_t ret = 0;

	/** 头部长度最长为 255.*/
	char *header_buf = (char *)malloc(sizeof(char) * 256);
	memset(header_buf, 0, 256);
	
	oss_compression_header_t *header =
		(oss_compression_header_t *)malloc(sizeof(oss_compression_header_t));
	if (header == NULL) {
		fprintf(stderr, "malloc failed.\n");
		return NULL;
	}

	memset(header, 0, sizeof(oss_compression_header_t));

	ret = fread(header_buf, 1, 256, fp);
	if (ret <0 || memcmp(header_buf, OSS_COMPRESSION_MAGIC, OSS_COMPRESSION_MAGIC_LEN) != 0) {
		fprintf(stderr, "oops, it's not a oss compression file.\n");
		return NULL;
	}
	oss_compression_header_t *hdr = (oss_compression_header_t *)header_buf;
	strncpy(header->magic, OSS_COMPRESSION_MAGIC, OSS_COMPRESSION_MAGIC_LEN);
	header->version = (char)hdr->version; 
	header->algorithm = hdr->algorithm;
	header->flag = hdr->flag;
	header->length = hdr->length;
	memcpy(header->md5, hdr->md5, 16);
	header->optional = NULL; /**< 目前首部不扩展，所以直接将optional置为NULL */

	free(header_buf);
	return header;
}

oss_compression_header_t *
oss_read_compression_header_in_memory(char *buffer)
{
	assert(buffer != 0);
	oss_compression_header_t *hdr = (oss_compression_header_t *)buffer;

	oss_compression_header_t *header =
		(oss_compression_header_t *)malloc(sizeof(oss_compression_header_t));
	if (header == NULL) {
		fprintf(stderr, "malloc failed.\n");
		return NULL;
	}

	memset(header, 0, sizeof(oss_compression_header_t));

	if (memcmp(buffer, OSS_COMPRESSION_MAGIC, OSS_COMPRESSION_MAGIC_LEN) != 0) {
		fprintf(stderr, "oops, it's not a oss compression buffer\n");
		return NULL;

	}
	strncpy(header->magic, OSS_COMPRESSION_MAGIC, OSS_COMPRESSION_MAGIC_LEN);
	header->version = (char)hdr->version; 
	header->algorithm = hdr->algorithm;
	header->flag = hdr->flag;
	header->length = hdr->length;
	memcpy(header->md5, hdr->md5, 16);
	header->optional = NULL; /**< 目前首部不扩展，所以直接将optional置为NULL */

	return header;
}

/**
 * 压缩整块内存，该块内存不包含压缩文件的头部内容
 * */
static int 
_decompress_block_with_lz4(
		char *inbuf, size_t inbuf_len, /** 输入参数，必须预先分配空间 */
		char *outbuf, size_t outbuf_len/** 输出参数，必须预先分配空间 */
		)
{
	assert(inbuf != NULL);
	assert(outbuf != NULL);

	return LZ4_uncompress_unknownOutputSize(
			(const char *)inbuf, outbuf, (int)inbuf_len, (int)outbuf_len);
}

/**
 * 压缩整块内存，该块内存包含压缩文件的头部内容
 * */
static int 
_decompress_block_with_lz4_2nd(
		char *inbuf, size_t inbuf_len, /** 输入参数，必须预先分配空间 */
		char *outbuf, size_t outbuf_len/** 输出参数，必须预先分配空间 */
		)
{
	assert(inbuf != NULL);
	assert(outbuf != NULL);

	return LZ4_uncompress_unknownOutputSize(
			(const char *)inbuf, outbuf, (int)inbuf_len, (int)outbuf_len);
}

/**
 * 解压缩文件
 * */
static void _decompress_file_with_lz4(
		FILE *fin,
		FILE *fout)
{
	char* inbuf = NULL;
	char* outbuf = NULL;
	unsigned int chunk_size = 0;
	size_t ret;
	int decode_len;

	inbuf = (char*)malloc(LZ4_compressBound(OSS_CHUNK_SIZE));
	outbuf = (char*)malloc(OSS_CHUNK_SIZE);
	if (!inbuf || !outbuf) {
		fprintf(stderr, "Allocation error : not enough memory\n");
		return;
	}

	while (1)
	{
		ret = fread(&chunk_size, 1, 4, fin);
		if(ret == 0) break;   // Nothing to read : file read is completed
		LITTLE_ENDIAN32(chunk_size);
		
	    ret = fread(inbuf, 1, chunk_size, fin);

		decode_len = LZ4_uncompress_unknownOutputSize(inbuf, outbuf, chunk_size, OSS_CHUNK_SIZE);
		if (decode_len < 0) {
			fprintf(stderr, "decoding failed, file corrupted.\n");
			return;
		}

		fwrite(outbuf, 1, decode_len, fout);
	}

	free(inbuf);
	free(outbuf);

	return;
}

/**
 * 解压缩内存块，该内存块不包含压缩格式的头部内容
 * */
int
oss_decompress_block(
		char *inbuf, size_t inbuf_len, /** 输入参数，必须预先分配空间 */
		char *outbuf, size_t outbuf_len,/** 输出参数，必须预先分配空间 */
		char algorithm /** 压缩算法 */
		)
{

	assert(inbuf!= NULL);
	assert(outbuf != NULL);
	int ret = 0;

	if (algorithm == OSS_LZ4) {
		ret = _decompress_block_with_lz4(inbuf + 4, inbuf_len - 4,
				outbuf, outbuf_len);
	}

	return ret;
}

/**
 * 解压缩内存块，该内存块包含压缩格式的头部内容
 * */
int
oss_decompress_block_2nd(
		char *inbuf, size_t inbuf_len, /** 输入参数，必须预先分配空间 */
		char *outbuf, size_t outbuf_len/** 输出参数，必须预先分配空间 */
		)
{

	assert(inbuf!= NULL);
	assert(outbuf != NULL);

	int ret = 0;
	size_t header_len = 0;

	oss_compression_header_t *header =
		oss_read_compression_header_in_memory(inbuf);
	if (header != NULL) {
		header_len = header->length;
		if (header->algorithm == OSS_LZ4) {
			ret = _decompress_block_with_lz4_2nd(
					inbuf + header_len + 4 , inbuf_len - header_len - 4,
					outbuf, outbuf_len);
			if (ret != 0 && header->flag == 1) {
				char *md5 = oss_get_buffer_md5_digest(outbuf, ret);
				if (memcmp(header->md5, md5, 16) == 0)
					return ret;
				else {
					fprintf(stderr, "file corrupted and md5sum check failed.\n");
					return 0;
				}
			}
		}
	}

	return ret;
}

/**
 * 解压缩文件
 * */
void oss_decompress_file(
		const char *infile,
		const char *outfile)
{
	assert(infile != NULL);
	assert(outfile != NULL);

	FILE *fin = fopen(infile, "rb");
	if (fin == NULL) {
		fprintf(stderr, "error occured when read file %s.\n", infile);
		return;
	}
	
	FILE *fout = fopen(outfile, "wb");
	if (fout == NULL) {
		fprintf(stderr, "error occured when open file %s.\n", outfile);
		return;
	}

	oss_compression_header_t *header =
		oss_read_compression_header(fin);

	if (header != NULL) {
		if (header->algorithm == OSS_LZ4) {
			size_t header_len = header->length;
			fseek(fin, header_len, SEEK_SET); /**< 从头部开始往后读*/
			_decompress_file_with_lz4(fin, fout);
		}
	}

	free(header);
	fclose(fin);
	fclose(fout);
	return;
}

