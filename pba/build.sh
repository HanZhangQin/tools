#!/bin/bash
###
 # @Author: Hanzhang Qin hanzhang.qin@memblaze.com
 # @Date: 2024-03-25 16:02:28
 # @LastEditors: Hanzhang Qin hanzhang.qin@memblaze.com
 # @LastEditTime: 2024-03-27 11:39:48
 # @FilePath: \tools\pba\build.sh
 # @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
###

FLASH_TYPE="X3_9070"
OUTPUT_FILE="NONE"
while getopts ":o:f:" opt
do
    case $opt in
        o)
        # echo "output file name: $OPTARG"
        OUTPUT_FILE=$OPTARG
        ;;
        f)
        echo "flash tpye $OPTARG"
        FLASH_TYPE=$OPTARG
        ;;
        ?)
        echo "Usage: ./build.sh [-o output_file_name] [-f flash_type]"
        ;;
    esac
done

if [ $OUTPUT_FILE == "NONE" ]
then
    OUTPUT_FILE=pba_$FLASH_TYPE
fi

gcc pba.c -o $OUTPUT_FILE -D $FLASH_TYPE && echo "output file name: $OUTPUT_FILE"

