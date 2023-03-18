#include "blockchain.h"

/**
 * create_block_info - Creates a block info structure and initializes it
 *
 * Return: Pointer to block_info structure
 */
block_info_t create_block_info(void)
{
	block_info_t block_info = {
	    0,		/* index */
	    0,		/* difficulty */
	    1537578000, /* timestamp */
	    0,		/* nonce */
	    {0}		/* prev_hash[SHA256_DIGEST_LENGTH] */
	};
	return (block_info);
}

/**
 * create_block_data - Creates a block data structure and initializes it
 *
 * Return: Pointer to block_data structure
 */
block_data_t create_block_data(void)
{
	char *data = "Holberton School";
	block_data_t block_data = {
	    {0}, /* buffer */
	    16	 /* len */
	};

	memcpy(block_data.buffer, data, BLOCKCHAIN_DATA_MAX);
	return (block_data);
}

/**
 * blockchain_create - Creates a Blockchain structure, and initializes it.
 *
 * Return: Pointer to the new Blockchain
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *blockchain = NULL;
	block_t *block = NULL;
	llist_t *chain = NULL;
	char *hash = "\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97"
		     "\xd4\x8d\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f"
		     "\x04\x51\x58\x03";
	/* c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803 */

	block = malloc(sizeof(block_t));
	if (!block)
		return (NULL);
	block->info = create_block_info();
	block->data = create_block_data();
	memcpy(&(block->hash), hash, SHA256_DIGEST_LENGTH);

	chain = llist_create(MT_SUPPORT_FALSE);
	if (!chain)
		return (free(block), NULL);
	if (llist_add_node(chain, block, ADD_NODE_FRONT) == -1)
		return (free(block), free(chain), NULL);

	blockchain = malloc(sizeof(blockchain_t));
	if (!blockchain)
		return (free(block), free(chain), NULL);
	blockchain->chain = chain;
	return (blockchain);
}
