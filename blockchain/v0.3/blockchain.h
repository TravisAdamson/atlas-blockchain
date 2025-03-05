#ifndef _BLOCKCHAIN_H_
#define _BLOCKCHAIN_H_

#include "provided/endianness.h"
#include <openssl/sha.h>
#include <llist.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define GENESIS_BLK_HASH 00000000000000000000000000000000
#define BLOCKCHAIN_DATA_MAX 1024
#define SHA256_DIGEST_LENGTH 32
#define ATLAS_HASH \
	"\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d" \
	"\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"
#define MAGIC_NUMS "\x48\x42\x4c\x4b"
#define VERS "\x30\x2e\x31"
#define END ((_get_endianness() == 1) ? "\x01" : "\x02")
#define FHEADER "\x48\x42\x4c\x4b\x30\x2e\x31"
#define BLOCK_GENERATION_INTERVAL 1
#define DIFF_ADJUSTMENT_INTERVAL 5
#define DIFF block->info.difficulty
#define EXP(x, y) \
	((x->info.index - y->info.index) * BLOCK_GENERATION_INTERVAL)
#define ACT(x, y) (x->info.timestamp - y->info.timestamp)

/**
 * struct blockchain_s - Blockchain structure
 *
 * @chain: Linked list of pointers to block_t objects
 * @unspent: Linked list of the transaction outputs that are unspent
 */
typedef struct blockchain_s
{
	llist_t     *chain;
	llist_t		*unspent;
} blockchain_t;

/**
 * struct block_data_s - Block data
 *
 * @buffer: Data buffer
 * @len:    Data size (in bytes)
 */
typedef struct block_data_s
{
	int8_t      buffer[BLOCKCHAIN_DATA_MAX];
	uint32_t    len;
} block_data_t;

/**
 * struct block_info_s - Block info structure
 *
 * @index:      Index of this Block in the Blockchain
 * @difficulty: Difficulty of proof of work (hash leading zero bits)
 * @timestamp:  Time the Block was created at (UNIX timestamp)
 * @nonce:      Salt value used to alter the Block hash
 * @prev_hash:  Hash of the previous Block in the Blockchain
 */
typedef struct block_info_s
{
	uint32_t    index;
	uint32_t    difficulty;
	uint64_t    timestamp;
	uint64_t    nonce;
	uint8_t     prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

/**
 * struct block_s - Block structure
 *
 * @info: Block info
 * @data: Block data
 * @hash: 256-bit digest of the Block, to ensure authenticity
 */
typedef struct block_s
{
	block_info_t    info;
	block_data_t    data;
	llist_t		*transactions;
	uint8_t     hash[SHA256_DIGEST_LENGTH];
} block_t;

blockchain_t *blockchain_create(void);
block_t *block_create(block_t const *prev, int8_t const *data,
					  uint32_t data_len);
void block_destroy(block_t *block);
void blockchain_destroy(blockchain_t *blockchain);
uint8_t *block_hash(block_t const *block,
					uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
int blockchain_serialize(blockchain_t const *blockchain, char const *path);
int write_each(llist_node_t list, unsigned int index, void *arg);
blockchain_t *blockchain_deserialize(char const *path);
int block_is_valid(block_t const *block, block_t const *prev_block);
int genesis_blk(block_t const *block);

int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
	uint32_t difficulty);
void block_mine(block_t *block);
uint32_t blockchain_difficulty(blockchain_t const *blockchain);

#endif
