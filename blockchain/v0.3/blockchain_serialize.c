#include "blockchain.h"

/**
 * write_list_utx_out - Writes an unspent transaction output to a file
 * @utxo:  Unspent transaction output to write
 * @index: Index of the transaction input in the transaction
 * @fd:    File descriptor to write to
 *
 * Return: 0 on success, 1 on failure
 */
int write_list_utx_out(unspent_tx_out_t *utxo, unsigned int index, int *fd)
{
	(void)index;
	if (!utxo || !fd)
		return (1);
	if (write(*fd, &(utxo->block_hash), SHA256_DIGEST_LENGTH) == -1 ||
	    write(*fd, &(utxo->tx_id), SHA256_DIGEST_LENGTH) == -1 ||
	    write(*fd, &(utxo->out.amount), sizeof(uint32_t)) == -1 ||
	    write(*fd, &(utxo->out.pub), EC_PUB_LEN) == -1 ||
	    write(*fd, &(utxo->out.hash), SHA256_DIGEST_LENGTH) == -1)
		return (1);
	return (0);
}

/**
 * write_list_tx_out - Writes a transaction output to a file
 * @tx_out: Points to the transaction output to write
 * @index:  Index of the transaction output in the transaction
 * @fd:     File descriptor to write to
 *
 * Return: 0 upon success, or 1 upon failure
 */
int write_list_tx_out(tx_out_t *tx_out, unsigned int index, int *fd)
{
	(void)index;
	if (!tx_out || !fd)
		return (1);
	if (write(*fd, &(tx_out->amount), sizeof(uint32_t)) == -1 ||
	    write(*fd, &(tx_out->pub), EC_PUB_LEN) == -1 ||
	    write(*fd, &(tx_out->hash), SHA256_DIGEST_LENGTH) == -1)
		return (1);
	return (0);
}

/**
 * write_list_tx_in - Writes a transaction input to a file
 * @tx_in: Points to the transaction input to write
 * @index: Index of the transaction input in the transaction
 * @fd:    File descriptor to write to
 *
 * Return: 0 upon success, or 1 upon failure
 */
int write_list_tx_in(tx_in_t *tx_in, unsigned int index, int *fd)
{
	(void)index;
	if (!tx_in || !fd)
		return (1);
	if (write(*fd, &(tx_in->block_hash), SHA256_DIGEST_LENGTH) == -1 ||
	    write(*fd, &(tx_in->tx_id), SHA256_DIGEST_LENGTH) == -1 ||
	    write(*fd, &(tx_in->tx_out_hash), SHA256_DIGEST_LENGTH) == -1 ||
	    write(*fd, tx_in->sig.sig, tx_in->sig.len) == -1 ||
	    write(*fd, &(tx_in->sig.len), sizeof(uint8_t)) == -1)
		return (1);
	return (0);
}

/**
 * write_list_tx - Writes a transaction to a file
 * @tx:    Points to the transaction to write
 * @index: Index of the transaction in the block
 * @fd:    File descriptor to write to
 *
 * Return: 0 upon success, or 1 upon failure
 */
int write_list_tx(transaction_t *tx, unsigned int index, int *fd)
{
	uint32_t len_tx_in = 0, len_tx_out = 0;

	printf("BB\n");
	(void)index;
	if (!tx || !fd)
		return (1);
	len_tx_in = llist_size(tx->inputs);
	len_tx_out = llist_size(tx->outputs);
	if (write(*fd, &len_tx_in, 4) == -1 ||
	    write(*fd, &len_tx_out, 4) == -1)
		return (1);
	if (llist_for_each(tx->inputs, (node_func_t)write_list_tx_in, fd) == -1 ||
	    llist_for_each(tx->outputs, (node_func_t)write_list_tx_out, fd) == -1)
		return (1);
	return (0);
}

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
	int32_t hblk_blocks = 0, index = 0, len_utxo = 0;
	int fd = open(path, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);

	if (!blockchain || !path || fd == -1)
		return (-1);
	hblk_blocks = llist_size(blockchain->chain);
	len_utxo = llist_size(blockchain->unspent);
	if (write(fd, hblk_magic, strlen(hblk_magic)) == -1 ||
	    write(fd, hblk_version, strlen(hblk_version)) == -1 ||
	    write(fd, &hblk_endian, 1) == -1 ||
	    write(fd, &hblk_blocks, 4) == -1 ||
	    write(fd, &len_utxo, 4) == -1)
		return (close(fd), -1);
	for (index = 0; index < hblk_blocks; index++)
	{
		block = llist_get_node_at(blockchain->chain, (unsigned int)index);
		len_utxo = llist_size(block->transactions);
		if (!block ||
		    write(fd, &(block->info), sizeof(block_info_t)) == -1 ||
		    write(fd, &(block->data.len), sizeof(uint32_t)) == -1 ||
		    write(fd, block->data.buffer, block->data.len) == -1 ||
		    write(fd, block->hash, SHA256_DIGEST_LENGTH) == -1 ||
		    write(fd, &len_utxo, 4) == -1)
		    			return (close(fd), -1);
		if (len_utxo > 0 && llist_for_each(block->transactions, (node_func_t)write_list_tx, &fd))
			return (close(fd), -1);
	}
	if (llist_for_each(blockchain->unspent, (node_func_t)write_list_utx_out, &fd))
		return (close(fd), -1);
	close(fd);
	return (0);
}
