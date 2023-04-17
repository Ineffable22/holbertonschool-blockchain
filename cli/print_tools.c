#include "cli.h"

/**
 * _print_hex_buffer - Prints a buffer in its hexadecimal form
 *
 * @buf: Pointer to the buffer to be printed
 * @len: Number of bytes from @buf to be printed
 */
void _print_hex_buffer(uint8_t const *buf, size_t len)
{
	size_t i;

	for (i = 0; buf && i < len; i++)
		printf("%02x", buf[i]);

	fflush(NULL);
}

/**
 * _get_endianness - Get current endianness
 *
 * Return: 1 for little endian, 2 for big endian, 0 for unknown
 */
uint8_t _get_endianness(void)
{
	_endian_t order = {1};

	if (order.bytes[0] == 1)
		return (1); /* Little endian */
	if (order.bytes[0] == 0)
		return (2); /* Big endian */

	return (0); /* Should not happen */
}

/**
 * _unspent_tx_out_print - Print an unspent transaction output
 *
 * @unspent: Pointer to the unspent transaction output to print
 */
void _unspent_tx_out_print(unspent_tx_out_t const *unspent)
{
	if (!unspent)
		return;

	printf("Unspent transaction output: {\n");

	printf("\tblock_hash: ");
	_print_hex_buffer(unspent->block_hash, sizeof(unspent->block_hash));
	printf(",\n");
	printf("\ttx_id: ");
	_print_hex_buffer(unspent->tx_id, sizeof(unspent->tx_id));
	printf(",\n");

	printf("\tout: {\n");

	printf("\t\tamount: %u,\n", unspent->out.amount);
	printf("\t\tpub: ");
	_print_hex_buffer(unspent->out.pub, sizeof(unspent->out.pub));
	printf(",\n");
	printf("\t\thash: ");
	_print_hex_buffer(unspent->out.hash, sizeof(unspent->out.hash));
	printf("\n");

	printf("\t}\n}\n");
}

/**
 * _blockchain_print_brief - Prints an entire Blockchain (brief mode)
 *
 * @blockchain: Pointer to the Blockchain to be printed
 */
void _blockchain_print_brief(blockchain_t const *blockchain)
{
	printf("Blockchain: {\n");

	printf("\tchain [%d]: [\n", llist_size(blockchain->chain));
	llist_for_each(blockchain->chain,
		       (node_func_t)_block_print_brief, "\t\t");
	printf("\t]\n");

	printf("}\n");
	fflush(NULL);
}

/**
 * _block_print_brief - Prints information about a Block (brief mode)
 *
 * @block:  Pointer to the Block to be printed
 * @index:  Index of the Block in the Blockchain
 * @indent: Lines prefix
 *
 * Return: Always 0
 */
int _block_print_brief(block_t const *block, unsigned int index,
		       char const *indent)
{
	char indent2[10] = {0};

	if (!block)
	{
		printf("%snil\n", indent);
		return (0);
	}

	printf("%sBlock: {\n", indent);

	printf("%s\tinfo: { ", indent);
	printf("%u, ", block->info.index);
	printf("%u, ", block->info.difficulty);
	printf("%lu, ", block->info.timestamp);
	printf("%lu, ", block->info.nonce);
	_print_hex_buffer(block->info.prev_hash, SHA256_DIGEST_LENGTH);
	printf(" },\n");

	printf("%s\tdata: { ", indent);
	printf("\"%s\", ", block->data.buffer);
	printf("%u", block->data.len);
	printf(" },\n");

	printf("%s\ttransactions [%d]: [\n", indent,
	       llist_size(block->transactions));
	memcpy(indent2, indent, strlen(indent));
	memcpy(indent2 + strlen(indent), "\t\t", 2);
	llist_for_each(block->transactions,
		       (node_func_t)_transaction_print_brief_loop, (void *)indent2);
	printf("%s\t]\n", indent);

	printf("%s\thash: ", indent);
	_print_hex_buffer(block->hash, SHA256_DIGEST_LENGTH);

	printf("\n%s}\n", indent);

	(void)index;
	return (0);
}
