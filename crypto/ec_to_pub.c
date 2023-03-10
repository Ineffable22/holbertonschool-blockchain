#include "hblk_crypto.h"

/**
 * ec_to_pub - Extracts the public key from an EC_KEY opaque structure
 * @key: Is a pointer to the EC_KEY structure to retrieve the public key from.
 *       If it is NULL, your function must do nothing and fail
 * @pub: Is the address at which to store the extracted public key
 *       (The key shall not be compressed)
 *
 * Return: a pointer to pub, otherwise NULL should be returned on failure
 */
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	const EC_POINT *point_curve = NULL;
	const EC_GROUP *info_curve = NULL;

	if (!key || !pub)
		return (NULL);

	point_curve = EC_KEY_get0_public_key(key);
	if (!point_curve)
	{
		fprintf(stderr, "EC_KEY_get0_public_key failed\n");
		return (NULL);
	}
	memset(pub, 0, EC_PUB_LEN);
	info_curve = EC_KEY_get0_group(key);
	if (!info_curve)
	{
		fprintf(stderr, "EC_KEY_get0_group failed\n");
		return (NULL);
	}

	if (!EC_POINT_point2oct(info_curve, point_curve,
				POINT_CONVERSION_UNCOMPRESSED, pub,
				EC_PUB_LEN, NULL))
	{
		fprintf(stderr, "EC_POINT_point2oct failed\n");
		return (NULL);
	}
	return (pub);
}
