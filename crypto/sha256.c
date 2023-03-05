#include "hblk_crypto.h"

/**
 * sha256 - Computes the hash of a sequence of bytes
 * @s:      Is the sequence of bytes to be hashed
 * @len:    Is the number of bytes to hash in s
 * @digest: The resulting hash must be stored in digest
 *
 * Return:  A pointer to digest
 */
uint8_t *sha256(int8_t const *s, size_t len,
		uint8_t digest[SHA256_DIGEST_LENGTH])
{
	SHA256_CTX sha256_ctx;

	if (SHA256_Init(&sha256_ctx) == 0)
	{
		fprintf(stderr, "Error: Failed to SHA256_Init\n");
		return (NULL);
	}
	if (SHA256_Update(&sha256_ctx, s, len) == 0)
	{
		fprintf(stderr, "Error: Failed to SHA256_Update\n");
		return (NULL);
	}
	if (SHA256_Final(digest, &sha256_ctx) == 0)
	{
		fprintf(stderr, "Error: Failed to SHA256_Final\n");
		return (NULL);
	}
	return (digest);
}
