#ifndef _CRC_H
#define _CRC_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#if IS_LITTLE_ENDIAN
# define GET_BYTE(uint, shift) (((uint) >> (shift)) & 0xFF)
#else
# define GET_BYTE(uint, shift) (((uint) >> (24 - (shift))) & 0xFF)
#endif

#define CRC_POLY_32  0xedb88320
#define CRC_START_32 0xffffffff
#define CRC_TABLE_32 0xff

typedef enum crc_type_e
{
	CRC_CRC32,
	CRC_CRC32C,
	CRC_CCITT32,
	CRC_ZIP,
	CRC_TYPE_SIZE,
} crc_type_t;


typedef enum chksum_style_e
{
	CHKSUM_STYLE_SIMPLE,
	CHKSUM_STYLE_SFV,
	CHKSUM_STYLE_BSD,
	CHKSUM_STYLE_SIZE
} chksum_style_t;

extern crc_type_t crc_type;
extern chksum_style_t chksum_style;
extern char* chksum_style_str[CHKSUM_STYLE_SIZE];
extern char* chksum_type_str[CRC_TYPE_SIZE];

extern uint32_t crc32_crcinit;
extern uint32_t crc32c_crcinit;
extern uint32_t ccitt32_crcinit;
extern uint32_t zip_crcinit;

uint32_t crc32_updcrc(uint32_t icrc, uint8_t* icp, size_t icnt);
uint32_t crc32c_updcrc(uint32_t icrc, uint8_t* icp, size_t icnt);
uint32_t ccitt32_updcrc(uint32_t icrc, uint8_t* icp, size_t icnt);
uint32_t zip_updcrc(uint32_t icrc, uint8_t* icp, size_t icnt);
uint32_t crc32(crc_type_t crc_type, uint32_t start, uint8_t* bytes, size_t size);

#endif
