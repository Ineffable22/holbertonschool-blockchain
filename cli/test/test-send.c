#include "../lib/unity.h"
#include "../cli.h"
/**
 * setUp - Set up function
 *
 * Return: Nothing
 */
void setUp(void)
{
}

/**
 * tearDown - Tear down function
 *
 * Return: Nothing
 */
void tearDown(void)
{
}

/**
 * test_send - Test send function
 *
 * Return: Nothing
 */
void test_send(void)
{
	char *arg[4] = {"send", "1", "receiver", NULL};
	session_t session = {0};

	send(arg, &session);
	TEST_ASSERT_EQUAL(0, session.state.code);
	TEST_ASSERT_EQUAL_STRING("Error: wallet is NULL", session.state.msg);
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_send);
	return (UNITY_END());
}
