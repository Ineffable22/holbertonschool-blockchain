#include "hblk_crypto.h"

/**
 * ec_create - Creates a new EC key pair
 *
 * Return: A  pointer to an EC_KEY structure, containing both the public
 * and private keys, or NULL upon failure
 */
EC_KEY *ec_create(void)
{
	struct ec_key_st *key;
	
	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (key == NULL)
		return (NULL);
	if (!EC_KEY_generate_key(key))
	{
		fprintf(stderr, "Key verification failed\n");
		EC_KEY_free(key);
        	return (NULL);
	}
	return (key);
}
