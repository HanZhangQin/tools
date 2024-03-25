#!/bin/bash

FLASH_TYPE="X3"
OUTPUT_FILE="NONE"
while getopts ":o:f:" opt
do
    case $opt in
        o)
        echo "output file name: $OPTARG"
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

gcc pba.c -o $OUTPUT_FILE -D $FLASH_TYPE

