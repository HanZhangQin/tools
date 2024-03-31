/*
 * @Author: Hanzhang Qin hanzhang.qin@memblaze.com
 * @Date: 2024-03-22 16:20:56
 * @LastEditors: Hanzhang Qin hanzhang.qin@memblaze.com
 * @LastEditTime: 2024-03-27 15:03:24
 * @FilePath: \undefinedz:\useful\include\std_type.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef char                s8;
typedef short               s16;
typedef int                 s32;
typedef long long           s64;
 
typedef enum
{
    RET_SUCCESS,
    RET_FAIL,
} ret_st_e;

#define TO_STRING(x)        #x
#define false               0
#define true                1
#define INVALID_U64         ((~(u64)0))
#define INVALID_U32         ((u32)INVALID_U64)


