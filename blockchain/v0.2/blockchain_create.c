#include "blockchain.h"

/**
 * blockchain_create - Creates a Blockchain structure, and initializes it.
 *
 * Return: Pointer to the new Blockchain
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *blockchain = NULL;
	block_t *block = NULL;
	llist_t *list = NULL;
	char *hash = "\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97"
		     "\xd4\x8d\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f"
		     "\x04\x51\x58\x03";
	/* c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803 */

	block = calloc(1, sizeof(block_t));
	if (!block)
		return (NULL);
	block->info.timestamp = 1537578000;
	memcpy(&block->data.buffer, "Holberton School", 16);
	block->data.len = 16;
	memcpy(&(block->hash), hash, SHA256_DIGEST_LENGTH);

	list = llist_create(MT_SUPPORT_FALSE);
	if (!list)
		return (free(block), NULL);
	if (llist_add_node(list, block, ADD_NODE_FRONT) == -1)
		return (free(block), free(list), NULL);

	blockchain = calloc(1, sizeof(blockchain_t));
	if (!blockchain)
		return (free(block), free(list), NULL);
	blockchain->chain = list;
	return (blockchain);
}
