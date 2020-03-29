#ifndef COUNTER_H_
#define COUNTER_H_

typedef unsigned int Counter;

/**
 * counter_reset - resets global counter
 * @return: counter value
 */
Counter
counter_reset
(void);

/**
 * counter_increment - increments global counter by 1
 * @return: counter value
 */
Counter
counter_increment
(void);

/**
 * counter_decrement - decrements global counter by 1
 * @return: counter value
 */
Counter
counter_decrement
(void);

/**
 * counter_add - add value to counter
 * @value: value to add
 * @return: counter value
 */
Counter
counter_add
(unsigned int value);

/**
 * counter_sub - sub value from counter
 * @value: value to sub
 * @return: counter value
 */
Counter
counter_sub
(unsigned int value);

#endif