/*
 * =============================================================================
 *
 *       Filename:  decompression-t.c
 *
 *    Description:  decompression testcase.
 *
 *        Created:  09/21/2012 11:10:48 PM
 *
 *         Author:  Fu Haiping (forhappy), haipingf@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <src/lib/md5.h>

#include "util/oss_compression.h"
#include "util/oss_decompression.h"

int main()
{
	//FILE *fp = fopen("hello.ossz", "r");
	
	//oss_compression_header_t *header = oss_read_compression_header(fp);
	//printf("%s\n", header->magic);
	//printf("%d\n", header->length);

	//oss_decompress_file("btree.c.ossz", "btree.new");
	oss_decompress_file("sample.ossz", "sample.new.tar.bz2");
	//fclose(fp);

}

