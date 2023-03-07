#include "hblk_crypto.h"

/**
 * ec_verify - Verifies the signature of a given set of bytes, using
 * a given EC_KEY public key
 * @key:    Points to the EC_KEY structure containing the public key to be used
 * to verify the signature
 * @msg:    Points to the msglen characters to verify the signature of
 * @msglen: Length of the msglen characters
 * @sig:    Points to the signature to be checked
 * Return: 1 if the signature is valid, and 0 otherwise
 * If either key, msg or sig is NULL, your function must fail
 */
int ec_verify(EC_KEY const *key, uint8_t const *msg, size_t msglen,
	      sig_t const *sig)
{
	if (!key || !msg || !sig)
		return (0);
	return (ECDSA_verify(0, msg, msglen, sig->sig, sig->len,
			     (EC_KEY *)key) == 1);
}
