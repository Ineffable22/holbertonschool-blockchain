#include "blockchain.h"

/**
 * deserialize_unspent - deserializes unspent transaction outputs
 * @fd:           File descriptor
 * @hblk_unspent: Number of unspent transaction outputs
 * Return: A linked list of unspent transaction outputs
 */
static llist_t *deserialize_unspent(int fd, int32_t hblk_unspent)
{
	unspent_tx_out_t *utxo = NULL;
	int32_t index = 0;
	llist_t *unspent = llist_create(MT_SUPPORT_FALSE);

	if (!unspent)
		return (NULL);
	for (index = 0; index < hblk_unspent; index++)
	{
		utxo = malloc(sizeof(unspent_tx_out_t));
		if (!utxo)
			return (llist_destroy(unspent, 1, NULL), NULL);
		if (read(fd, &(utxo->block_hash), SHA256_DIGEST_LENGTH) == -1 ||
		    read(fd, &(utxo->tx_id), SHA256_DIGEST_LENGTH) == -1 ||
		    read(fd, &(utxo->out.amount), sizeof(uint32_t)) == -1 ||
		    read(fd, &(utxo->out.pub), EC_PUB_LEN) == -1 ||
		    read(fd, &(utxo->out.hash), SHA256_DIGEST_LENGTH) == -1)
			return (free(utxo), llist_destroy(unspent, 1, NULL), NULL);
		if (llist_add_node(unspent, utxo, ADD_NODE_REAR) == -1)
			return (free(utxo), llist_destroy(unspent, 1, NULL), NULL);
	}
	return (unspent);
}

/**
 * read_list_tx_out - Reads a transaction output from a file
 * @outputs:    Points to the transaction output to read
 * @fd:         File descriptor to read from
 * @nb_outputs: Number of transaction outputs to read
 *
 * Return: 0 upon success, or 1 upon failure
 */
static int read_list_tx_out(llist_t **outputs, int fd, int32_t nb_outputs)
{
	int32_t index = 0;
	tx_out_t *tx_out = NULL;

	*outputs = llist_create(MT_SUPPORT_FALSE);
	if (!(*outputs))
		return (1);
	for (index = 0; index < nb_outputs; index++)
	{
		tx_out = malloc(sizeof(tx_out_t));
		if (!tx_out)
			return (1);
		if (read(fd, &(tx_out->amount), sizeof(uint32_t)) == -1)
			return (1);
		if (read(fd, &(tx_out->pub), EC_PUB_LEN) == -1)
			return (1);
		if (read(fd, &(tx_out->hash), SHA256_DIGEST_LENGTH) == -1)
			return (1);
		if (llist_add_node(*outputs, tx_out, ADD_NODE_REAR) == -1)
			return (free(tx_out), 1);
	}
	return (0);
}

/**
 * read_list_tx_in - reads a list of transaction inputs from a file
 * @inputs:    Pointer to the list of transaction inputs
 * @fd:        File descriptor from which to read
 * @nb_inputs: Number of transaction inputs to read
 * Return: 0 upon success, or 1 upon failure
 */
static int read_list_tx_in(llist_t **inputs, int fd, int32_t nb_inputs)
{
	int32_t index = 0;
	tx_in_t *tx_in = NULL;

	*inputs = llist_create(MT_SUPPORT_FALSE);
	if (!(*inputs))
		return (0);
	for (index = 0; index < nb_inputs; index++)
	{
		tx_in = malloc(sizeof(tx_in_t));
		if (!tx_in)
			return (1);
		if (read(fd, &(tx_in->block_hash), SHA256_DIGEST_LENGTH) == -1)
			return (1);
		if (read(fd, &(tx_in->tx_id), SHA256_DIGEST_LENGTH) == -1)
			return (1);
		if (read(fd, &(tx_in->tx_out_hash), SHA256_DIGEST_LENGTH) == -1)
			return (1);
		if (llist_add_node(*inputs, tx_in, ADD_NODE_REAR) == -1)
			return (free(tx_in), 1);
	}
	return (0);
}

/**
 * read_list_tx - Reads a list of transactions from a file
 * @block:           Block to add the transactions to
 * @fd:              Filedescriptor to read from
 * @nb_transactions: Number of transactions to read
 *
 * Return: 0 upon success, or 1 upon failure
 */
static int read_list_tx(block_t **block, int fd, int32_t nb_transactions)
{
	int32_t index = 0, nb_inputs = 0, nb_outputs = 0;
	transaction_t *tx = NULL;
	llist_t *list = nb_transactions > 0 ? llist_create(MT_SUPPORT_FALSE) : 0;

	if (!list)
		return (0);
	for (index = 0; index < nb_transactions; index++)
	{
		tx = malloc(sizeof(transaction_t));
		if (!tx)
			return (1);
		if (read(fd, &(tx->id), SHA256_DIGEST_LENGTH) == -1)
			return (1);
		if (read(fd, &(nb_inputs), sizeof(int32_t)) == -1)
			return (1);
		if (read(fd, &(nb_outputs), sizeof(int32_t)) == -1)
			return (1);
		if (read_list_tx_in(&tx->inputs, fd, nb_inputs))
			return (1);
		if (read_list_tx_out(&tx->outputs, fd, nb_outputs))
			return (1);
		if (llist_add_node(list, tx, ADD_NODE_REAR) == -1)
			return (free(tx), 1);
	}
	(*block)->transactions = list;
	return (0);
}

/**
 * deserialize_chain - Deserializes a Blocks from a file
 * @fd:          Filedescriptor to read from
 * @hblk_blocks: Number of blocks to read
 *
 * Return: A pointer to the deserialized Blocks upon success,
 */
static llist_t *deserialize_chain(int fd, int32_t hblk_blocks)
{
	block_t *block = NULL;
	int32_t index = 0, nb_transactions = 0;
	llist_t *list = hblk_blocks > 0 ? llist_create(MT_SUPPORT_FALSE) : 0;

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
		if (read(fd, &nb_transactions, sizeof(int32_t)) == -1)
			return (NULL);
		if (read_list_tx(&block, fd, nb_transactions))
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
	int32_t hblk_blocks = 0, hblk_unspent = 0;

	if (!path)
		return (NULL);
	if (fd == -1)
		return (NULL);
	fd = open(path, O_RDONLY);
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
	if (read(fd, &hblk_unspent, sizeof(u_int32_t)) == -1)
		return (NULL);
	blockchain = malloc(sizeof(blockchain_t));
	if (!blockchain)
		return (NULL);
	blockchain->chain = deserialize_chain(fd, hblk_blocks);
	if (!blockchain->chain)
		return (free(blockchain), NULL);
	blockchain->unspent = deserialize_unspent(fd, hblk_unspent);
	if (!blockchain->unspent)
		return (free(blockchain), NULL);
	close(fd);
	return (blockchain);
}
