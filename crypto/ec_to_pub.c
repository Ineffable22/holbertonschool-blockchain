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
	const EC_POINT *point_curve;
	const EC_GROUP *info_curve;
	size_t len;

	if (!key || !pub)
		return (NULL);

	point_curve = EC_KEY_get0_public_key(key);
	if (!point_curve)
	{
		fprintf(stderr, "EC_KEY_get0_public_key failed\n");
		EC_KEY_free((EC_KEY *)key);
		return (NULL);
	}
	memset(pub, 0, EC_PUB_LEN);
	info_curve = EC_KEY_get0_group(key);
	if (!info_curve)
	{
		fprintf(stderr, "EC_KEY_get0_group failed\n");
		EC_KEY_free((EC_KEY *)key);
		return (NULL);
	}
	len = EC_POINT_point2oct(info_curve, point_curve,
				 POINT_CONVERSION_UNCOMPRESSED, pub, EC_PUB_LEN, NULL);
	if (len == 0)
	{
		fprintf(stderr, "EC_POINT_point2oct failed\n");
		EC_KEY_free((EC_KEY *)key);
		EC_GROUP_free((EC_GROUP *)info_curve);
		return (NULL);
	}
	return (pub);
}
