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

#define DECLARE_BIT_FIELD32(name, lsb, msb) struct { u32 : (lsb); u32 name : ((msb)-(lsb)+1); }
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
#endif