#include "cli.h"

/**
 * _transaction_print_brief_loop - Prints a transaction (brief mode)
 * @transaction: Pointer to the transaction to print
 * @idx:         Index of the transaction in the block
 * @indent:      Lines prefix
 *
 * Return: always 0
 */
int _transaction_print_brief_loop(transaction_t const *transaction,
				  unsigned int idx, char const *indent)
{
	tx_out_t const *out;

	if (!transaction)
		return (0);

	out = llist_get_head(transaction->outputs);

	printf("%sTransaction: {\n", indent);

	printf("%s\tamount: %u from %d inputs,\n", indent, out->amount,
	       llist_size(transaction->inputs));
	printf("%s\treceiver: ", indent);
	_print_hex_buffer(out->pub, EC_PUB_LEN);
	printf("\n");
	printf("%s\tid: ", indent);
	_print_hex_buffer(transaction->id, sizeof(transaction->id));
	printf("\n");

	printf("%s}\n", indent);

	(void)idx;
	return (0);
}

/**
 * _block_print - Prints information about a Block
 *
 * @block:  Pointer to the Block to be printed
 * @index:  Index of the Block in the Blockchain
 * @indent: Lines prefix
 *
 * Return: Always 0
 */
int _block_print(block_t const *block, unsigned int index,
		 char const *indent)
{
	char indent2[10] = {0};

	if (!block)
	{
		printf("%snil\n", indent);
		return (0);
	}

	printf("%sBlock: {\n", indent);

	printf("%s\tinfo: {\n", indent);
	printf("%s\t\tindex: %u,\n", indent, block->info.index);
	printf("%s\t\tdifficulty: %u,\n", indent, block->info.difficulty);
	printf("%s\t\ttimestamp: %lu,\n", indent, block->info.timestamp);
	printf("%s\t\tnonce: %lu,\n", indent, block->info.nonce);
	printf("%s\t\tprev_hash: ", indent);
	_print_hex_buffer(block->info.prev_hash, SHA256_DIGEST_LENGTH);
	printf("\n%s\t},\n", indent);

	printf("%s\tdata: {\n", indent);
	printf("%s\t\tbuffer: \"%s\",\n", indent, block->data.buffer);
	printf("%s\t\tlen: %u\n", indent, block->data.len);
	printf("%s\t},\n", indent);

	printf("%s\ttransactions [%d]: [\n", indent,
	       llist_size(block->transactions));
	memcpy(indent2, indent, strlen(indent));
	memcpy(indent2 + strlen(indent), "\t\t", 2);
	llist_for_each(block->transactions,
		       (node_func_t)_transaction_print_loop, (void *)indent2);
	printf("%s\t]\n", indent);

	printf("%s\thash: ", indent);
	_print_hex_buffer(block->hash, SHA256_DIGEST_LENGTH);

	printf("\n%s}\n", indent);

	(void)index;
	return (0);
}

int _transaction_print_loop(transaction_t const *transaction,
			    unsigned int idx, char const *indent)
{
	if (!transaction)
		return (0);

	printf("%sTransaction: {\n", indent);

	printf("%s\tinputs [%u]: [\n", indent, llist_size(transaction->inputs));
	llist_for_each(transaction->inputs, (node_func_t)_tx_in_print,
		       (void *)indent);
	printf("%s\t],\n", indent);
	printf("%s\toutputs [%u]: [\n", indent, llist_size(transaction->outputs));
	llist_for_each(transaction->outputs, (node_func_t)_tx_out_print,
		       (void *)indent);
	printf("%s\t],\n", indent);
	printf("%s\tid: ", indent);
	_print_hex_buffer(transaction->id, sizeof(transaction->id));
	printf("\n");

	printf("%s}\n", indent);

	(void)idx;
	return (0);
}

/**
 * _tx_in_print - print transaction
 * @in:     Transaction
 * @idx:    Index
 * @indent: Indent
 *
 * Return: Always 0
 */
int _tx_in_print(tx_in_t const *in, unsigned int idx,
		 char const *indent)
{
	printf("%s\t\t{\n", indent);

	printf("%s\t\t\tblock_hash: ", indent);
	_print_hex_buffer(in->block_hash, sizeof(in->block_hash));
	printf(",\n");
	printf("%s\t\t\ttx_id: ", indent);
	_print_hex_buffer(in->tx_id, sizeof(in->tx_id));
	printf(",\n");
	printf("%s\t\t\ttx_out_hash: ", indent);
	_print_hex_buffer(in->tx_out_hash, sizeof(in->tx_out_hash));
	printf(",\n");
	printf("%s\t\t\tsig: ", indent);
	if (in->sig.len)
		_print_hex_buffer(in->sig.sig, in->sig.len);
	else
		printf("null");
	printf("\n");

	printf("%s\t\t}\n", indent);

	(void)idx;
	return (0);
}

int _tx_out_print(tx_out_t const *out, unsigned int idx,
		  char const *indent)
{
	printf("%s\t\t{\n", indent);

	printf("%s\t\t\tamount: %u,\n", indent, out->amount);
	printf("%s\t\t\tpub: ", indent);
	_print_hex_buffer(out->pub, sizeof(out->pub));
	printf(",\n");
	printf("%s\t\t\thash: ", indent);
	_print_hex_buffer(out->hash, sizeof(out->hash));
	printf("\n");

	printf("%s\t\t}\n", indent);

	(void)idx;
	return (0);
}
