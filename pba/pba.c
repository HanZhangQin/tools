/*
 * @Author: Hanzhang Qin hanzhang.qin@memblaze.com
 * @Date: 2024-03-22 16:19:10
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2024-03-25 15:20:45
 * @FilePath: \undefinedz:\useful\PBA\pba.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/common_type.h"

#define DOUBLE_CHECK_32_44  1

#define CHECK_OPT_PARAM(opt, num_param) do{ if (argc < (num_param))\
                                            {\
                                                usage(opt);\
                                                return RET_FAIL;\
                                            }\
                                          } while (0)
void print_pba44(U_FPA_44BIT pba44);

void usage(char opt)
{
    u32 print_all = 0;
    while (1)
    {
        switch (opt)
        {
            case 'p':
            {
                printf ("-p input: 1 or more 32 bit pbas. output: parse 32 bit pbas\n");
                printf("Usage: pba -p <pba0 pba1...>\n");
                if (!print_all)
                {
                    return;
                }
            }
            case 'f':
            {
                printf("-f input: 1 or more files with 32bit fpas. output: parse 32 bit pbas\n");
                printf("Usage: pba -f <files with pba list>\n");
                if (!print_all)
                {
                    return;
                }
            }
            case 'u':
            {
                printf("-u input 1 or more 44 bit pbas with lo32 & hi32 pairs. output: parse 44 bit pbas & convert to 32 bit pbas\n");
                printf("Usage: pba -u <lo32 hi32> <lo32 hi32> ...\n");
                if (!print_all)
                {
                    return;
                }
            }
            case 't':
            {
                printf("-t input 1 or more 44 bit pbas with u64 format. output: parse 44 bit pbas & convert to 32 bit pbas\n");
                printf("Usage: pba -t <pba44> <pba44> ...\n");
                if (!print_all)
                {
                    return;
                }
            }
            case 'v':
            {
                printf("-v show version information\n");
                if (!print_all)
                {
                    return;
                }
            }
            default:
                if (!print_all)
                {
                    print_all = 1;
                    opt = 'p';
                    break;
                }
                else
                {
                    return;
                }
        }
    }
}

u64 str_to_u64(const char *str)
{
    char tmp_str[256];
    strcpy(tmp_str, str);
    char *endPtr = &tmp_str[strlen(tmp_str) - 1];
    while (endPtr >= tmp_str)
    {
        if (*endPtr >= '0')
        {
            break;
        } 
        endPtr--;
    }

    *(endPtr + 1) = '\0';
    if (endPtr < tmp_str)
    {
        printf ("Skip empty line\n");
        return INVALID_U64;
    }
    u64 ret = strtol(tmp_str, &endPtr, 0);
    if (*endPtr != '\0')
    {
        printf ("Pba %s not a valid number\n", str);
        return INVALID_U64;
    }

    return ret;
}

U_FPA_44BIT pba32_to_44(l2p_entry_t pba)
{
    u32 ch      = PBA_GET_CHN(pba);
    u32 lun     = PBA_GET_LUN(pba);
    u32 blk     = PBA_GET_BLK(pba);
    u32 page    = PBA_GET_PG(pba);

    U_FPA_44BIT pba44;
    pba44.val   = 0;
    pba44.iu    = pba.df;
    pba44.page  = page;
    pba44.pl    = pba.pl;
    pba44.blk   = blk;
    pba44.die   = lun;
    pba44.ch    = ch;

    return pba44;
}

void print_pba(l2p_entry_t pba)
{
    u32 ch      = PBA_GET_CHN(pba);
    u32 lun     = PBA_GET_LUN(pba);
    u32 blk     = PBA_GET_BLK(pba);
    u32 page    = PBA_GET_PG(pba);
    u32 sblk    = blk * NAND_PLANE_NUM + pba.pl;
    U_FPA_44BIT pba44 = pba32_to_44(pba);

    printf ("pba 0x%x: ch %d ln %d sb %d pg %d df %d blk %d pl %d xlun %d 44bit 0x%llx [l:0x%x h:0x%x]\n"  , pba.pba
                                                                                                                , ch
                                                                                                                , lun
                                                                                                                , sblk
                                                                                                                , page
                                                                                                                , pba.df
                                                                                                                , blk
                                                                                                                , pba.pl
                                                                                                                , pba.xLun
                                                                                                                , pba44.val
                                                                                                                , pba44.lo32
                                                                                                                , pba44.hi32);
}

u32 print_pba_by_string(const char *pba_str)
{
    l2p_entry_t pba;
    char *endPtr;
    pba.pba = (u32)str_to_u64(pba_str);
    if (pba.pba == INVALID_U32)
    {
        return RET_FAIL;
    }
    print_pba(pba);
 #if DOUBLE_CHECK_32_44
    print_pba44(pba32_to_44(pba));
 #endif
    return RET_SUCCESS;
}

u32 print_pba_by_file(char *pba_file)
{
    FILE *fp = fopen(pba_file, "r");
    char pba_str[32];
    u32 line = 1;
    if (fp == NULL)
    {
        printf ("file %s open fail\n", pba_file);
        return RET_FAIL;
    }

    printf ("######### %s #########\n", pba_file);
    while (fgets(pba_str, sizeof(pba_str), fp) != NULL)
    {
        pba_str[strlen(pba_str) - 1] = '\0';
        printf("%d: ", line++);
        print_pba_by_string(pba_str);
    }

    fclose(fp);
    return RET_SUCCESS;
}

l2p_entry_t pba44_to_pba32(U_FPA_44BIT pba44)
{
    l2p_entry_t pba;
    pba.pba = 0;
    pba.xLun = pba44.die * MAX_CHANNEL + pba44.ch;
    pba.df = pba44.iu;
    pba.pl = pba44.pl;
    pba.blkPage = pba44.blk * NAND_PAGE_NUM_PER_BLOCK + pba44.page;
    return pba;
}

void print_pba44(U_FPA_44BIT pba44)
{
    l2p_entry_t pba = pba44_to_pba32(pba44);
    printf ("pba44 0x%llx [l:0x%x h:0x%x]: ch %d ln %d sb %d pg %d df %d blk %d pl %d xlun %d 32bit 0x%x\n", pba44.val
                                                                                                                , pba44.lo32
                                                                                                                , pba44.hi32
                                                                                                                , pba44.ch
                                                                                                                , pba44.die
                                                                                                                , pba44.blk * NAND_PLANE_NUM + pba44.pl
                                                                                                                , pba44.page
                                                                                                                , pba44.iu
                                                                                                                , pba44.blk
                                                                                                                , pba44.pl
                                                                                                                , pba44.die * MAX_CHANNEL + pba44.ch
                                                                                                                , pba.val);
}

u32 print_pba44_by_string(const char *pba44_str[], u32 str_num)
{
    U_FPA_44BIT pba44;

    if (str_num == 1)
    {
        pba44.val = str_to_u64(pba44_str[0]);
        if (pba44.val == INVALID_U64)
        {
            return RET_FAIL;
        }
    }
    else if (str_num == 2)
    {
        u64 lo32 = str_to_u64(pba44_str[0]);
        u64 hi32 = str_to_u64(pba44_str[1]);
        if (lo32 == INVALID_U64 || hi32 == INVALID_U64)
        {
            return RET_FAIL;
        }
        pba44.lo32 = (u32)lo32;
        pba44.hi32 = (u32)hi32;
    }
    else
    {
        return RET_FAIL;
    }

    print_pba44(pba44);
#if DOUBLE_CHECK_32_44
    print_pba(pba44_to_pba32(pba44));
#endif
    return RET_SUCCESS;
}

int main(int argc, char **argv)
{
    int o, i;
    const char *optstrings = "p:f:u:t:v::";

    while ((o = getopt(argc, argv, optstrings)) != -1)
    {
        switch (o)
        {
            case 'p':
            {
                CHECK_OPT_PARAM('p', 3);
                for (i = optind - 1; i < argc; i++)
                {
                    if (RET_FAIL == print_pba_by_string(argv[i]))
                    {
                        return RET_FAIL;
                    }
                }
                break;
            }
            case 'f':
            {
                CHECK_OPT_PARAM('f', 3);
                for (i = optind - 1; i < argc; i++)
                {
                    if (RET_FAIL == print_pba_by_file(argv[i]))
                    {
                        return RET_FAIL;
                    }
                }
                break;
            }
            case 'u':
            {
                CHECK_OPT_PARAM('u', 4);
                if ((argc & 1) != 0)
                {
                    usage('u');
                    return RET_FAIL;
                }
                for (i = optind - 1; i < argc; i += 2)
                {
                    if (RET_FAIL == print_pba44_by_string((const char **)&argv[i], 2))
                    {
                        return RET_FAIL;
                    }
                }
                break;
            }
            case 't':
            {
                CHECK_OPT_PARAM('t', 3);
                for (i = optind - 1; i < argc; i += 1)
                {
                    if (RET_FAIL == print_pba44_by_string((const char **)&argv[i], 1))
                    {
                        return RET_FAIL;
                    }
                }
                break;
            }
            case 'v':
            {
                printf ("FLASH_TYPE: %s\n", FLASH_TYPE);
                printf ("NUM_PAGE: %d\nNUM_BLOCK: %d\nNUM_PLANE: %d\n", NAND_PAGE_NUM_PER_BLOCK, NAND_BLOCK_NUM_PER_PLANE, NAND_PLANE_NUM);
                break;
            }
            default:
            {
                usage(0);
                return RET_FAIL;
            }
        }
    }

    return RET_SUCCESS;
}