#include "unity.h"
#include "counter.h"

#ifdef CONFIG_COUNTER_TEST_ENABLE_RESET
TEST_CASE("counter_reset", "[counter]") {
  counter_add(5);
  Counter zero = counter_reset();
  TEST_ASSERT_EQUAL_UINT(zero, 0);
}
#endif

#ifdef CONFIG_COUNTER_TEST_ENABLE_INCREMENT
TEST_CASE("counter_increment", "[counter]") {
  counter_reset();
  Counter one = counter_increment();
  TEST_ASSERT_EQUAL_UINT(one, 1);
}
#endif

#ifdef CONFIG_COUNTER_TEST_ENABLE_DECREMENT
TEST_CASE("counter_decrement", "[counter]") {
  counter_reset();
  Counter five = counter_add(5);
  TEST_ASSERT_EQUAL_UINT(five, 5);
  Counter four = counter_decrement();
  TEST_ASSERT_EQUAL_UINT(four, 4);
}
#endif

#ifdef CONFIG_COUNTER_TEST_ENABLE_ADD
TEST_CASE("counter_add", "[counter]") {
  counter_reset();
  Counter five = counter_add(5);
  TEST_ASSERT_EQUAL_UINT(five, 5);
}
#endif

#ifdef CONFIG_COUNTER_TEST_ENABLE_SUB
TEST_CASE("counter_sub", "[counter]") {
  counter_reset();
  Counter five = counter_add(5);
  TEST_ASSERT_EQUAL_UINT(five, 5);
  Counter three = counter_sub(2);
  TEST_ASSERT_EQUAL_UINT(three, 3);
}
#endif