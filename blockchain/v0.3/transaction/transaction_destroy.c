#include "transaction.h"

/**
 * transaction_destroy - Deallocates a transaction structure
 * @transaction: Points to the transaction to delete
 */
void transaction_destroy(transaction_t *transaction)
{
	if (!transaction)
		return;
	if (llist_destroy(transaction->inputs, 1, free) ||
	    llist_destroy(transaction->outputs, 1, free))
		return;
	free(transaction);
}
