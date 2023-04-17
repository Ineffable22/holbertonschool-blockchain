#include "hblk_crypto.h"

/**
 * ec_save - Saves an existing EC key pair on the disk
 * @key:    Points to the EC key pair to be saved on disk
 * @folder: Iis the path to the folder in which to save the keys
 *
 * Return: 1 or 0 upon success or failure
 */
int ec_save(EC_KEY *key, char const *folder)
{
	FILE *fp = NULL;
	char path[1024] = {0};

	if (!key || !folder)
		return (0);
	mkdir(folder, S_IRWXU | S_IRWXG | S_IRWXO);
	sprintf(path, "%s/%s", folder, PRI_FILENAME);
	fp = fopen(path, "w");
	if (!fp)
		return (0);
	if (!PEM_write_ECPrivateKey(fp, key, NULL, NULL, 0, NULL, NULL))
		return (0);
	fclose(fp);
	sprintf(path, "%s/%s", folder, PUB_FILENAME);
	fp = fopen(path, "w");
	if (!fp)
		return (0);
	if (!PEM_write_EC_PUBKEY(fp, key))
		return (0);
	fclose(fp);
	return (1);
}
