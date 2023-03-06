#include "hblk_crypto.h"

/**
 * ec_from_pub - Creates an EC_KEY structure given a public key
 * @pub: Contains the public key to be converted
 *
 * Return: a pointer to the created EC_KEY structure upon success,
 * or NULL upon failure
 */
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{
	EC_KEY *key = NULL;
	const EC_POINT *point = NULL;
	const EC_GROUP *group = NULL;

	if (!pub)
		return (NULL);
	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!key)
	{
		fprintf(stderr, "ec_create() failed\n");
		return (NULL);
	}
	group = EC_KEY_get0_group(key);
	if (!group)
	{
		fprintf(stderr, "EC_KEY_get0_group failed\n");
		EC_KEY_free((EC_KEY *)key);
		return (NULL);
	}
	point = EC_POINT_new(group);
	if (!point)
		return (EC_KEY_free(key), NULL);
	if (!EC_POINT_oct2point(group, (EC_POINT *)point, pub, EC_PUB_LEN, NULL))
	{
		fprintf(stderr, "EC_POINT_oct2point failed\n");
		EC_KEY_free((EC_KEY *)key);
		EC_POINT_free((EC_POINT *)point);
		return (NULL);
	}
	if (!EC_KEY_set_public_key(key, point))
	{
		fprintf(stderr, "EC_KEY_set_public_key failed\n");
		EC_KEY_free((EC_KEY *)key);
		EC_POINT_free((EC_POINT *)point);
		return (NULL);
	}
	EC_POINT_free((EC_POINT *)point);
	return (key);
}
