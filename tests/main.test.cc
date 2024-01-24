#define _CRT_NONSTDC_NO_DEPRECATE
#define strdup _strdup

#include "../include/pale_noel.h"
#include "../ext/minunit.h"

int tests_run = 0;

static char *test_given_param_data()
{
  int destination = 0, source = 0;
  mu_assert(_strdup("Assume Destination isn't Source"), destination != source);
  return 0;
}

static char *test_units()
{
  mu_run_test(test_given_param_data);
  return 0;
}

int main()
{
  char *res = test_units();

  if (res != 0)
    std::printf("[FAILED]:\t%s\n", res);
  else
    std::printf("[PASSED]:\t%s\n", res);

  return res == 0;
}
