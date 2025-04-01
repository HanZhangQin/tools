/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2024-07-22 17:51:50
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2024-07-22 18:07:01
 * @FilePath: \tools\file\file.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "../include/file.h"

u32 readBinaryFile(char *fileName, void **buffer)
{
	u32 size = 0, entryCnt;
	FILE *fp = fopen(fileName, "rb");

	if (NULL == fp)
	{
		printf("OPEN FILE %s FAILED!\n", fileName);
		return 0;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	printf("File size %d\n", size);

	*buffer = malloc(size);
	if (NULL == *buffer)
	{
		return 0;
	}

	fseek(fp, 0, SEEK_SET);
	fread(*buffer, sizeof(char), size, fp);

	fclose(fp);

	return size;
}