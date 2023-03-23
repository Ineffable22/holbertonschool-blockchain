#include "blockchain.h"

/**
 * hash_matches_difficulty - Checks whether a given hash matches
 * a given difficulty
 * @hash:       Is the hash to check
 * @difficulty: Is the minimum difficulty the hash should match
 *
 * Return:  1 if the difficulty is respected, or 0 otherwise
 */
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
			    uint32_t difficulty)
{
	uint8_t i = 0, j = 0, zeros = 0, binary = 0, bit = 0;

	if (!hash || difficulty > (SHA256_DIGEST_LENGTH * 8))
		return (0);

	for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		binary = hash[i];
		if (binary == 0x0)
		{
			zeros += 8;
			continue;
		}
		for (j = 0; j < 8; j++, binary >>= 1)
		{
			bit++;
			if (binary & 1)
				bit = 0;
		}
		break;
	}
	if (zeros + bit != difficulty)
		return (0);
	return (1);
}
