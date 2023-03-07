#include "hblk_crypto.h"

/**
 * ec_sign - Signs a given set of bytes, using a given EC_KEY private key
 * @key: Points to the EC_KEY structure containing the private key to be used
 * to perform the signature
 * @msg: Points to the msglen characters to be signed
 * @msglen: Length of the msglen characters
 * @sig: Holds the address at which to store the signature
 *
 * Return: If either key or msg is NULL, your function must fail
 */
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg, size_t msglen,
		 sig_t *sig)
{
	uint32_t len = 0;

	if (!key || !msg || !sig)
		return (NULL);
	len = sig->len;
	if (ECDSA_sign(0, msg, msglen, sig->sig, &len,
		       (EC_KEY *)key) != 1)
		return (NULL);
	sig->len = len;
	return (sig->sig);
}
