#ifndef X3_9070_H
#define X3_9070_H

#define MAX_CHANNEL                             16
#define NAND_PAGE_NUM_PER_BLOCK                 4176
#define NAND_BLOCK_NUM_PER_PLANE                363
#define NAND_PLANE_NUM                          6

#define NAND_L2P_ENTRY_AU_MSB                   4
#define NAND_L2P_ENTRY_AU_LSB                   0
#define NAND_L2P_ENTRY_BLKxPAGE_MSB             31
#define NAND_L2P_ENTRY_BLKxPAGE_LSB             11
#define NAND_L2P_ENTRY_XLUN_MSB                 10
#define NAND_L2P_ENTRY_XLUN_LSB                 5
#define NAND_L2P_ENTRY_PL_MSB                   4
#define NAND_L2P_ENTRY_PL_LSB                   2
#define NAND_L2P_ENTRY_DF_MSB                   1
#define NAND_L2P_ENTRY_DF_LSB                   0

typedef union
{
    struct
    {
        u64 iu       : 2;
        u64 page     :15;
        u64 pl       : 3;
        u64 blk      :12;
        u64 die      : 8;
        u64 ch       : 4;
        u64 rsv      :20;
    };
    struct
    {
        u32 lo32;
        u32 hi32;
    };
    u64 val;
} U_FPA_44BIT;

#endif