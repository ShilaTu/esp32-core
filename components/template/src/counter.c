#include "counter.h"

static Counter counter;

Counter
counter_reset
(void)
{
  counter = 0;
  return counter;
}

Counter
counter_increment
(void)
{
  return ++counter;
}

Counter
counter_decrement
(void)
{
  return --counter;
}

Counter
counter_add
(unsigned int value)
{
  counter += value;
  return counter;
}

Counter
counter_sub
(unsigned int value)
{
  counter -= value;
  return counter;
}