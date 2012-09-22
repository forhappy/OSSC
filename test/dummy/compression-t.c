/*
 * =============================================================================
 *
 *       Filename:  compression-t.c
 *
 *    Description:  compression testcase.
 *
 *        Created:  09/21/2012 05:22:30 PM
 *
 *         Author:  Fu Haiping (forhappy), haipingf@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util/oss_compression.h"

int main()
{
	//oss_compress_file("btree.c", "btree.c.ossz", 1, 1, 0);
	oss_compress_file("mysql-5.1.52.tar.gz", "sample.ossz", 1, 1, 0);

}
