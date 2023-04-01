#include "blockchain.h"

/**
 * deserialize_blocks - Deserializes a Blocks from a file
 * @fd:          Filedescriptor to read from
 * @hblk_blocks: Number of blocks to read
 *
 * Return: A pointer to the deserialized Blocks upon success,
 */
llist_t *deserialize_blocks(int fd, uint32_t hblk_blocks)
{
	llist_t *list = llist_create(MT_SUPPORT_TRUE);
	block_t *block = NULL;
	uint32_t index = 0;

	if (!list)
		return (NULL);
	for (index = 0; index < hblk_blocks; index++)
	{
		block = calloc(1, sizeof(block_t));
		if (!block)
			return (NULL);
		if (read(fd, &(block->info), sizeof(block_info_t)) == -1)
			return (NULL);
		if (read(fd, &(block->data.len), sizeof(int)) == -1)
			return (NULL);
		if (read(fd, block->data.buffer, block->data.len) == -1)
			return (NULL);
		if (read(fd, block->hash, SHA256_DIGEST_LENGTH) == -1)
			return (NULL);
		if (llist_add_node(list, block, ADD_NODE_REAR) == -1)
			return (free(block), NULL);
	}
	return (list);
}

/**
 * blockchain_deserialize - Deserializes a Blockchain from a file
 * @path: Contains the path to a file to load the Blockchain from
 *
 * Return: A pointer to the deserialized Blockchain upon success,
 * or NULL upon failure
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	int fd = 0;
	blockchain_t *blockchain = NULL;
	char buff[9] = {0};
	u_int32_t hblk_blocks = 0;

	if (!path)
		return (NULL);
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (NULL);
	if (read(fd, buff, 8) == -1)
		return (NULL);
	if (buff[0] != '\x48' || buff[1] != '\x42' ||
	    buff[2] != '\x4c' || buff[3] != '\x4b')
		return (NULL);
	if (buff[4] != '\x30' || buff[5] != '\x2E' || buff[6] != '\x31')
		return (NULL);
	if (buff[7] != _get_endianness())
		return (NULL);
	if (read(fd, &hblk_blocks, sizeof(u_int32_t)) == -1)
		return (NULL);
	blockchain = malloc(sizeof(blockchain_t));
	if (!blockchain)
		return (NULL);
	blockchain->chain = deserialize_blocks(fd, hblk_blocks);
	if (!blockchain->chain)
		return (free(blockchain), NULL);
	close(fd);
	return (blockchain);
}
