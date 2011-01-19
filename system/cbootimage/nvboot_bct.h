/**
 * Copyright (c) 2011 NVIDIA Corporation.  All rights reserved.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef INCLUDED_NVBOOT_BCT_H
#define INCLUDED_NVBOOT_BCT_H

#include <sys/types.h>

/**
 * Defines the number of 32-bit words in the customer_data area of the BCT.
 */
#define NVBOOT_BCT_CUSTOMER_DATA_WORDS 298

/**
 * Defines the number of bytes in the customer_data area of the BCT.
 */
#define NVBOOT_BCT_CUSTOMER_DATA_SIZE \
                (NVBOOT_BCT_CUSTOMER_DATA_WORDS * 4)

/**
 * Defines the number of bytes in the reserved area of the BCT.
 */
#define NVBOOT_BCT_RESERVED_SIZE       3

/**
 * Defines the maximum number of bootloader descriptions in the BCT.
 */
#define NVBOOT_MAX_BOOTLOADERS         4

#define NVBOOT_BCT_USED_DATA_SIZE	534

/**
 * Defines the number of entries (bits) in the bad block table.
 * The consequences of changing its value are as follows.  Using P as the
 * # of physical blocks in the boot loader and B as the value of this
 * constant:
 *    B > P: There will be unused storage in the bad block table.
 *    B < P: The virtual block size will be greater than the physical block
 *           size, so the granularity of the bad block table will be less than
 *           one bit per physical block.
 *
 * 4096 bits is enough to represent an 8MiB partition of 2KiB blocks with one
 * bit per block (1 virtual block = 1 physical block).  This occupies 512 bytes
 * of storage.
 */
#define NVBOOT_BAD_BLOCK_TABLE_SIZE 4096

/**
 * Defines the maximum number of blocks to search for BCTs.
 *
 * This value covers the initial block and a set of journal blocks.
 *
 * Ideally, this number will span several erase units for reliable updates
 * and tolerance for blocks to become bad with use.  Safe updates require
 * a minimum of 2 erase units in which BCTs can appear.
 *
 * To ensure that the BCT search spans a sufficient range of configurations,
 * the search block count has been set to 64. This allows for redundancy with
 * a wide range of parts and provides room for greater problems in this
 * region of the device.
 */
#define NVBOOT_MAX_BCT_SEARCH_BLOCKS   64

/*
 * Defines the CMAC-AES-128 hash length in 32 bit words. (128 bits = 4 words)
 */
enum {NVBOOT_CMAC_AES_HASH_LENGTH = 4};

/**
 * Defines the storage for a hash value (128 bits).
 */
typedef struct nvboot_hash_rec
{
	u_int32_t hash[NVBOOT_CMAC_AES_HASH_LENGTH];
} nvboot_hash;

/**
 * Stores information needed to locate and verify a boot loader.
 *
 * There is one \c nv_bootloader_info structure for each copy of a BL stored on
 * the device.
 */
typedef struct nv_bootloader_info_rec
{
	u_int32_t version;
	u_int32_t start_blk;
	u_int32_t start_page;
	u_int32_t length;
	u_int32_t load_addr;
	u_int32_t entry_point;
	u_int32_t attribute;
	nvboot_hash crypto_hash;
} nv_bootloader_info;

/**
 * Defines the bad block table structure stored in the BCT.
 */
typedef struct nvboot_badblock_table_rec
{
	u_int32_t entries_used;
	u_int8_t virtual_blk_size_log2;
	u_int8_t block_size_log2;
	u_int8_t bad_blks[NVBOOT_BAD_BLOCK_TABLE_SIZE / 8];
} nvboot_badblock_table;

/**
 * Contains the information needed to load BLs from the secondary boot device.
 *
 * - Supplying NumParamSets = 0 indicates not to load any of them.
 * - Supplying NumDramSets  = 0 indicates not to load any of them.
 * - The \c random_aes_blk member exists to increase the difficulty of
 *   key attacks based on knowledge of this structure.
 */
typedef struct nvboot_config_table_rec
{
	nvboot_hash crypto_hash;
	nvboot_hash random_aes_blk;
	u_int32_t boot_data_version;
	u_int32_t block_size_log2;
	u_int32_t page_size_log2;
	u_int32_t partition_size;
	u_int32_t bct_used_data[NVBOOT_BCT_USED_DATA_SIZE];
	nvboot_badblock_table badblock_table;
	u_int32_t bootloader_used;
	nv_bootloader_info bootloader[NVBOOT_MAX_BOOTLOADERS];
	u_int8_t customer_data[NVBOOT_BCT_CUSTOMER_DATA_SIZE];
	u_int8_t enable_fail_back;
	u_int8_t reserved[NVBOOT_BCT_RESERVED_SIZE];
} nvboot_config_table;

/** @} */
#endif /* #ifndef INCLUDED_NVBOOT_BCT_H */
