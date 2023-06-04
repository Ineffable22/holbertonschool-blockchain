#include "hblk_crypto.h"

/**
 * ec_load - Loads an EC key pair from the disk
 * @folder: Is the path to the folder from which to load the keys
 *
 * Return: A pointer to the created EC key pair upon success,
 * or NULL upon failure
 */
EC_KEY *ec_load(char const *folder)
{
	FILE *fp = NULL;
	char path[1024] = {0};
	EC_KEY *key = NULL;

	if (!folder)
		return (NULL);
	sprintf(path, "keys/%s/%s", folder, PUB_FILENAME);
	fp = fopen(path, "r");
	if (!fp)
		return (NULL);
	key = PEM_read_EC_PUBKEY(fp, &key, NULL, NULL);
	if (!key)
		return (NULL);
	fclose(fp);

	sprintf(path, "keys/%s/%s", folder, PRI_FILENAME);
	fp = fopen(path, "r");
	if (!fp)
		return (NULL);
	key = PEM_read_ECPrivateKey(fp, &key, NULL, NULL);
	if (!key)
		return (NULL);
	fclose(fp);
	return (key);
}
