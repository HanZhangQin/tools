#ifndef COMMON_TYPE_H
#define COMMON_TYPE_H

#include "std_type.h"

#ifdef X3
#define FLASH_TYPE TO_STRING(X3)
#include "x3_9070.h"
#else
#define FLASH_TYPE TO_STRING(X3)
#include "x3_9070.h"
#endif

#define DECLARE_BIT_FIELD32(name, lsb, msb)     struct { u32 : (lsb); u32 name : ((msb)-(lsb)+1); }
#define L2P_ENTRY_LAU_MSB                       NAND_L2P_ENTRY_AU_MSB
#define L2P_ENTRY_LAU_LSB                       NAND_L2P_ENTRY_AU_LSB
#define L2P_ENTRY_PL_MSB                        NAND_L2P_ENTRY_PL_MSB
#define L2P_ENTRY_PL_LSB                        NAND_L2P_ENTRY_PL_LSB
#define L2P_ENTRY_DF_MSB                        NAND_L2P_ENTRY_DF_MSB
#define L2P_ENTRY_DF_LSB                        NAND_L2P_ENTRY_DF_LSB
#define L2P_ENTRY_XLUN_MSB                      NAND_L2P_ENTRY_XLUN_MSB
#define L2P_ENTRY_XLUN_LSB                      NAND_L2P_ENTRY_XLUN_LSB
#define L2P_ENTRY_BLKxPAGE_MSB                  NAND_L2P_ENTRY_BLKxPAGE_MSB
#define L2P_ENTRY_BLKxPAGE_LSB                  NAND_L2P_ENTRY_BLKxPAGE_LSB

typedef union
{
    u32 val;
    u32 pba;
    DECLARE_BIT_FIELD32(blkPage, L2P_ENTRY_BLKxPAGE_LSB, L2P_ENTRY_BLKxPAGE_MSB);
    DECLARE_BIT_FIELD32(xLun,    L2P_ENTRY_XLUN_LSB,     L2P_ENTRY_XLUN_MSB);
    DECLARE_BIT_FIELD32(df,      L2P_ENTRY_DF_LSB,       L2P_ENTRY_DF_MSB);
    DECLARE_BIT_FIELD32(pl,      L2P_ENTRY_PL_LSB,       L2P_ENTRY_PL_MSB);
    DECLARE_BIT_FIELD32(plDf,    L2P_ENTRY_DF_LSB,       L2P_ENTRY_PL_MSB);
    DECLARE_BIT_FIELD32(LAU,     L2P_ENTRY_LAU_LSB,      L2P_ENTRY_LAU_MSB);
} l2p_entry_t;

#define PBA_GET_CHN(pba)    (((l2p_entry_t)pba).xLun % MAX_CHANNEL)
#define PBA_GET_LUN(pba)    (((l2p_entry_t)pba).xLun / MAX_CHANNEL)
#define PBA_GET_BLK(pba)    (((l2p_entry_t)pba).blkPage / NAND_PAGE_NUM_PER_BLOCK)
#define PBA_GET_PG(pba)     (((l2p_entry_t)pba).blkPage % NAND_PAGE_NUM_PER_BLOCK)

#define IS_CHAR_DEC(ch)     (((ch) >= '0') && ((ch) <='9'))
#define IS_CHAR_HEX(ch)     (IS_CHAR_DEC(ch) \
                            || (((ch) >= 'a') && ((ch) <= 'f')) \
                            || (((ch) >= 'A') && ((ch) <= 'F')))

static inline u32 string_to_u64(const char *str, u64 *val)
{
    char tmp_str[128];
    const char *ch = str;
    u32 str_len = 0, isHex = false, num = 0;
    u64 tmp_val = 0;

    while ((*ch == ' ') || (*ch == '\t'))
    {
        ch++;
    }

    if ((ch[0] == '0')
        && ((ch[1] == 'x') || (ch[1] == 'X')))
    {
        isHex = true;
        ch += 2;
    }

    while ((isHex ? IS_CHAR_HEX(ch[str_len]) : IS_CHAR_DEC(ch[str_len]))
          && str_len < (sizeof(tmp_str)/ sizeof(tmp_str[0]) - 1))
    {
        tmp_str[str_len] = ch[str_len];
        str_len++;
    }

    tmp_str[str_len] = '\0';
    ch = tmp_str;
    while (*ch)
    {
        u64 tmp;
        if (isHex)
        {
            if (IS_CHAR_DEC(*ch))
            {
                tmp = *ch - '0';
            }
            else if (*ch >= 'a' && *ch <= 'f')
            {
                tmp = *ch - 'a' + 10;
            }
            else
            {
                tmp = *ch - 'A' + 10;
            }
            tmp_val = ((tmp_val << 4) + tmp);
        }
        else
        {
            tmp = *ch - '0';
            tmp_val = tmp_val * 10 + tmp;
        }

        ch++;
    }

    *val = tmp_val;
    return str_len;
}
#endif