#include "blockchain.h"

/**
 * blockchain_serialize - Serializes a Blockchain into a file
 * @blockchain: Points to the Blockchain to be serialized.
 * @path:       Contains the path to a file to serialize the Blockchain into
 * If path points to an existing file, it must be overwritten
 *
 * Return: 0 upon success, or -1 upon failure
 */
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	block_t *block = NULL;
	char *hblk_magic = "\x48\x42\x4c\x4b";
	char *hblk_version = "\x30\x2E\x31";
	uint8_t hblk_endian = _get_endianness();
	uint32_t hblk_blocks = 0, index = 0;
	int fd = open(path, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);

	if (!blockchain || !path || fd == -1)
		return (-1);
	hblk_blocks = llist_size(blockchain->chain);
	if (write(fd, hblk_magic, strlen(hblk_magic)) == -1)
		return (close(fd), -1);
	if (write(fd, hblk_version, strlen(hblk_version)) == -1)
		return (close(fd), -1);
	if (write(fd, &hblk_endian, 1) == -1)
		return (close(fd), -1);
	if (write(fd, &hblk_blocks, 4) == -1)
		return (close(fd), -1);
	for (index = 0; index < hblk_blocks; index++)
	{
		block = llist_get_node_at(blockchain->chain, (unsigned int)index);
		if (!block ||
		    write(fd, &(block->info), sizeof(block_info_t)) == -1 ||
		    write(fd, &(block->data.len), sizeof(uint32_t)) == -1 ||
		    write(fd, block->data.buffer, block->data.len) == -1 ||
		    write(fd, block->hash, SHA256_DIGEST_LENGTH) == -1)
			return (close(fd), -1);
	}
	close(fd);
	return (0);
}
