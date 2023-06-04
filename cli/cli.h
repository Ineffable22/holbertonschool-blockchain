#ifndef _CLI_H_
#define _CLI_H_

#include <signal.h>
#include "../blockchain/v0.3/blockchain.h"
#include "../blockchain/v0.3/provided/endianness.h"
#include <readline/readline.h>
#include <readline/history.h>

#define BACKUP "backup/save.hblk"
#define SC(session, num, message) \
	(session->state.code = num, session->state.msg = message)

/**
 * struct wallet_s - Wallet structure
 *
 * @pub: Public key
 * @key: Private key
 */
typedef struct wallet_s
{
	char *username;
	uint32_t balance;
	uint8_t pub[EC_PUB_LEN];
	EC_KEY *key;
} wallet_t;

/**
 * struct error_s - Error structure
 *
 * @code: Error code
 * @msg:  Error message
 */
typedef struct error_s
{
	int code;
	char *msg;
} error_t;

/**
 * struct session_s - Session structure
 *
 * @blockchain: Blockchain
 * @tx_pool:    Transaction pool
 * @wallet:     Wallet
 * @state:      Error state
 */
typedef struct session_s
{
	blockchain_t *blockchain;
	llist_t *tx_pool;
	wallet_t *wallet;
	error_t state;
} session_t;

/**
 * struct cmd_s - Command structure
 *
 * @cmd: Command
 * @f:   Function to execute
 */
typedef struct cmd_s
{
	char *cmd;
	void (*f)(char *args[], session_t *error);
} cmd_t;

/* --- print_tools.c --- */
void _print_hex_buffer(uint8_t const *buf, size_t len);

/* --- current.c --- */
void current(char **arg, session_t *session);

/* --- cli.c --- */
void response_signal(int x);
void parse_str(char *buffer, session_t *session);
void search_cmd(char **arg, session_t *state);

void load(char **arg, session_t *state);
void exit_cli(char **arg, session_t *state);
void help(char **arg, session_t *state);
void save(char **arg, session_t *state);
void send(char **arg, session_t *state);
void serialize(char **arg, session_t *session);
void start_blockchain(session_t *session);
void deserialize(char **arg, session_t *session);
void mine(char **arg, session_t *session);
uint8_t _get_endianness(void);
void _unspent_tx_out_print(unspent_tx_out_t const *unspent);
void info(char **arg, session_t *session);
void _blockchain_print_brief(blockchain_t const *blockchain);
int _block_print_brief(block_t const *block, unsigned int index,
		       char const *indent);
int _transaction_print_brief_loop(transaction_t const *transaction,
				  unsigned int idx, char const *indent);
int _block_print(block_t const *block, unsigned int index,
		 char const *indent);
int _transaction_print_loop(transaction_t const *transaction,
			    unsigned int idx, char const *indent);
int _tx_in_print(tx_in_t const *in, unsigned int idx,
		 char const *indent);
int _tx_out_print(tx_out_t const *out, unsigned int idx,
		  char const *indent);
void deserialize(char **arg, session_t *session);
void serialize(char **arg, session_t *session);
EC_KEY *get_receiver(char **arg, session_t *session, EC_KEY *receiver,
		     uint8_t pub[EC_PUB_LEN]);
void add_utxo(session_t *session, wallet_t *wallet);
block_t *new_block(session_t *session, block_t **block, EC_KEY **miner,
		   transaction_t **coin_tx);
int add_tx(llist_node_t node, unsigned int idx, void *_block);
int invalid_tx(llist_node_t node, void *unspent);
transaction_t *create_utxo(session_t *session, EC_KEY *receiver, int32_t amount);
void save_file(char **arg, session_t *session);
uint32_t get_balance(llist_t *all_unspent, uint8_t pub_sender[EC_PUB_LEN]);

#endif /* _CLI_H_ */
