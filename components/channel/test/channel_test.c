#include "unity.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "channel.h"
#include "channel_internal.h"

static void *callback_ctx;
static void *callback_data;
static TickType_t callback_timeout;
static BaseType_t callback_flags;

BaseType_t
returnPdPASS
(void *ctx,
 const void *data,
 TickType_t timeout,
 const BaseType_t flags)
{
  callback_ctx = ctx; 
  callback_data = (void*) data; 
  callback_timeout = timeout; 
  callback_flags = flags; 
  return pdPASS;
}

BaseType_t
returnErrQUEUE_FULL
(void *ctx,
 const void *data,
 TickType_t timeout,
 const BaseType_t flags)
{
  callback_ctx = ctx; 
  callback_data = (void*) data; 
  callback_timeout = timeout; 
  callback_flags = flags; 
  return errQUEUE_FULL;
}

#define TEST_ASSERT_LIST_LENGTH(head, expect) \
{ \
  uint length = 1; \
  struct list_head *pos; \
  TEST_ASSERT_NOT_NULL((head)->next); \
  TEST_ASSERT_NOT_NULL((head)->prev); \
  list_for_each(pos,(head)) { \
    TEST_ASSERT_NOT_NULL((head)->next); \
    TEST_ASSERT_NOT_NULL((head)->prev); \
    length++; \
  } \
  TEST_ASSERT_EQUAL_UINT((expect),length); \
}

#define TEST_ASSERT_LIST_OCCURRENCE(head,item,expect) \
{ \
  uint count = 0; \
  struct list_head *pos; \
  TEST_ASSERT_NOT_NULL((head)->next); \
  TEST_ASSERT_NOT_NULL((head)->prev); \
  TEST_ASSERT_NOT_NULL((item)->next); \
  TEST_ASSERT_NOT_NULL((item)->prev); \
  list_for_each(pos,(head)) { \
    TEST_ASSERT_NOT_NULL((head)->next); \
    TEST_ASSERT_NOT_NULL((head)->prev); \
    if ((head) == (item)) { \
      count++; \
    } \
  } \
  TEST_ASSERT_EQUAL_UINT((expect),count); \
}

#define TEST_ASSERT_LIST_EMPTY(head) \
{ \
  TEST_ASSERT_NOT_NULL((head)->next); \
  TEST_ASSERT_NOT_NULL((head)->prev); \
  TEST_ASSERT_EQUAL_PTR((head)->next, (head)); \
  TEST_ASSERT_EQUAL_PTR((head)->next, (head)); \
  TEST_ASSERT_EQUAL_PTR((head)->next, (head)->prev); \
} 

#ifdef CONFIG_CHANNEL_TEST_ENABLE_INIT
TEST_CASE("channel_init", "[channel]")
{
  Channel ch;
  char identifier[] = "test_channel";
  void *ctx = (void*)0x1;
  BaseType_t flags = 0x2;
  Channel_callback callback = &returnPdPASS;
  channel_internal_init(&ch, identifier, ctx, flags, callback);

  TEST_ASSERT_NOT_NULL(ch.identifier);
  TEST_ASSERT_EQUAL_STRING(identifier, ch.identifier);

  TEST_ASSERT_LIST_EMPTY(&ch.same);
  TEST_ASSERT_LIST_EMPTY(&ch.unique);

  TEST_ASSERT_NOT_NULL(ch.ctx);
  TEST_ASSERT_EQUAL_PTR(ch.ctx, ctx);

  TEST_ASSERT_EQUAL_INT(ch.flags, flags);

  TEST_ASSERT_NOT_NULL(ch.callback);
  TEST_ASSERT_EQUAL_PTR(ch.callback, callback);
}
#endif

#ifdef CONFIG_CHANNEL_TEST_ENABLE_SETCONTEXT
TEST_CASE("channel_setContext", "[channel]")
{
  Channel ch;
  char identifier[] = "test_channel";
  void *ctx0 = (void*)0x1;
  void *ctx1 = (void*)0x2;
  BaseType_t flags = 0x3;
  Channel_callback callback = &returnPdPASS;
  channel_internal_init(&ch, identifier, ctx0, flags, callback);

  channel_setContext(&ch, ctx1);

  TEST_ASSERT_NOT_NULL(ch.ctx);
  TEST_ASSERT_EQUAL_PTR(ch.ctx, ctx1);
}
#endif

#ifdef CONFIG_CHANNEL_TEST_ENABLE_SETCONTEXT
TEST_CASE("channel_setCallback", "[channel]")
{
  Channel ch;
  char identifier[] = "test_channel";
  void *ctx = (void*)0x1;
  BaseType_t flags = 0x2;
  Channel_callback callback0 = &returnPdPASS;
  Channel_callback callback1 = &returnErrQUEUE_FULL;
  channel_internal_init(&ch, identifier, ctx, flags, callback0);

  channel_setCallback(&ch, callback1);

  TEST_ASSERT_NOT_NULL(ch.callback);
  TEST_ASSERT_EQUAL_PTR(ch.callback, callback1);
}
#endif

#ifdef CONFIG_CHANNEL_TEST_ENABLE_INTERNAL_REGISTER
TEST_CASE(INCREMENT"channel_internal_register", "[channel]")
{
  channel_internal_resetRoot();
  Channel ch0, ch1, ch2, ch3, ch4, ch5;
  char identifier0[] = "test_channel0";
  char identifier1[] = "test_channel1";
  char identifier2[] = "test_channel2";
  void *ctx = (void*)0x1;
  BaseType_t flags = 0x2;
  Channel_callback callback = &returnPdPASS;
  channel_internal_init(&ch0, identifier0, ctx, flags, callback);
  channel_internal_init(&ch1, identifier0, ctx, flags, callback);
  channel_internal_init(&ch2, identifier0, ctx, flags, callback);
  channel_internal_init(&ch3, identifier1, ctx, flags, callback);
  channel_internal_init(&ch4, identifier1, ctx, flags, callback);
  channel_internal_init(&ch5, identifier2, ctx, flags, callback);

  channel_internal_register(&ch0);
  TEST_ASSERT_LIST_EMPTY(&ch0.same);
  TEST_ASSERT_LIST_LENGTH(&ch0.unique, 2);

  channel_internal_register(&ch1);
  TEST_ASSERT_LIST_LENGTH(&ch0.unique, 2);
  TEST_ASSERT_LIST_EMPTY (&ch1.unique);
  TEST_ASSERT_LIST_LENGTH(&ch0.same, 2);
  TEST_ASSERT_LIST_LENGTH(&ch1.same, 2);

  channel_internal_register(&ch2);
  TEST_ASSERT_LIST_LENGTH(&ch0.unique, 2);
  TEST_ASSERT_LIST_EMPTY (&ch1.unique);
  TEST_ASSERT_LIST_EMPTY (&ch2.unique);
  TEST_ASSERT_LIST_LENGTH(&ch0.same, 3);
  TEST_ASSERT_LIST_LENGTH(&ch1.same, 3);
  TEST_ASSERT_LIST_LENGTH(&ch2.same, 3);

  channel_internal_register(&ch3);
  TEST_ASSERT_LIST_LENGTH(&ch0.unique, 3);
  TEST_ASSERT_LIST_EMPTY (&ch1.unique);
  TEST_ASSERT_LIST_EMPTY (&ch2.unique);
  TEST_ASSERT_LIST_LENGTH(&ch3.unique, 3);
  TEST_ASSERT_LIST_LENGTH(&ch0.same, 3);
  TEST_ASSERT_LIST_LENGTH(&ch1.same, 3);
  TEST_ASSERT_LIST_LENGTH(&ch2.same, 3);
  TEST_ASSERT_LIST_EMPTY (&ch3.same);

  channel_internal_register(&ch4);
  TEST_ASSERT_LIST_LENGTH(&ch0.unique, 3);
  TEST_ASSERT_LIST_EMPTY (&ch1.unique);
  TEST_ASSERT_LIST_EMPTY (&ch2.unique);
  TEST_ASSERT_LIST_LENGTH(&ch3.unique, 3);
  TEST_ASSERT_LIST_EMPTY (&ch4.unique);
  TEST_ASSERT_LIST_LENGTH(&ch0.same, 3);
  TEST_ASSERT_LIST_LENGTH(&ch1.same, 3);
  TEST_ASSERT_LIST_LENGTH(&ch2.same, 3);
  TEST_ASSERT_LIST_LENGTH(&ch3.same, 2);
  TEST_ASSERT_LIST_LENGTH(&ch4.same, 2);

  channel_internal_register(&ch5);
  TEST_ASSERT_LIST_LENGTH(&ch0.unique, 4);
  TEST_ASSERT_LIST_EMPTY (&ch1.unique);
  TEST_ASSERT_LIST_EMPTY (&ch2.unique);
  TEST_ASSERT_LIST_LENGTH(&ch3.unique, 4);
  TEST_ASSERT_LIST_EMPTY (&ch4.unique);
  TEST_ASSERT_LIST_LENGTH(&ch5.unique, 4);
  TEST_ASSERT_LIST_LENGTH(&ch0.same, 3);
  TEST_ASSERT_LIST_LENGTH(&ch1.same, 3);
  TEST_ASSERT_LIST_LENGTH(&ch2.same, 3);
  TEST_ASSERT_LIST_LENGTH(&ch3.same, 2);
  TEST_ASSERT_LIST_LENGTH(&ch4.same, 2);
  TEST_ASSERT_LIST_EMPTY (&ch5.same);

  channel_internal_resetRoot();
}
#endif

#ifdef CONFIG_CHANNEL_TEST_ENABLE_INTERNAL_UNREGISTER
TEST_CASE("channel_internal_unregister", "[channel]")
{
  channel_internal_resetRoot();
  Channel ch0, ch1, ch2, ch3, ch4, ch5;
  char identifier0[] = "test_channel0";
  char identifier1[] = "test_channel1";
  char identifier2[] = "test_channel2";
  void *ctx = (void*)0x1;
  BaseType_t flags = 0x2;
  Channel_callback callback = &returnPdPASS;
  channel_init(&ch0, identifier0, ctx, flags, callback);
  channel_init(&ch1, identifier0, ctx, flags, callback);
  channel_init(&ch2, identifier0, ctx, flags, callback);
  channel_init(&ch3, identifier1, ctx, flags, callback);
  channel_init(&ch4, identifier1, ctx, flags, callback);
  channel_init(&ch5, identifier2, ctx, flags, callback);

  TEST_ASSERT_LIST_LENGTH(&ch0.unique, 4);
  TEST_ASSERT_LIST_EMPTY (&ch1.unique);
  TEST_ASSERT_LIST_EMPTY (&ch2.unique);
  TEST_ASSERT_LIST_LENGTH(&ch3.unique, 4);
  TEST_ASSERT_LIST_EMPTY (&ch4.unique);
  TEST_ASSERT_LIST_LENGTH(&ch5.unique, 4);
  TEST_ASSERT_LIST_LENGTH(&ch0.same, 3);
  TEST_ASSERT_LIST_LENGTH(&ch1.same, 3);
  TEST_ASSERT_LIST_LENGTH(&ch2.same, 3);
  TEST_ASSERT_LIST_LENGTH(&ch3.same, 2);
  TEST_ASSERT_LIST_LENGTH(&ch4.same, 2);
  TEST_ASSERT_LIST_EMPTY (&ch5.same);

  // unregister unique& same channel
  channel_internal_unregister(&ch3);
  TEST_ASSERT_LIST_LENGTH(&ch0.unique, 4);
  TEST_ASSERT_LIST_EMPTY (&ch1.unique);
  TEST_ASSERT_LIST_EMPTY (&ch2.unique);
  //ch3.unique should be empty
  TEST_ASSERT_LIST_EMPTY (&ch3.unique);
  //ch4 should now be unique
  TEST_ASSERT_LIST_LENGTH(&ch4.unique, 4);
  TEST_ASSERT_LIST_LENGTH(&ch5.unique, 4);
  TEST_ASSERT_LIST_LENGTH(&ch0.same, 3);
  TEST_ASSERT_LIST_LENGTH(&ch1.same, 3);
  TEST_ASSERT_LIST_LENGTH(&ch2.same, 3);
  //ch3.same should be empty
  TEST_ASSERT_LIST_EMPTY (&ch3.same);
  //ch4.same should be empty
  TEST_ASSERT_LIST_EMPTY (&ch4.same);
  TEST_ASSERT_LIST_EMPTY (&ch5.same);

  // unregister same channel
  channel_internal_unregister(&ch2);
  TEST_ASSERT_LIST_LENGTH(&ch0.unique, 4);
  TEST_ASSERT_LIST_EMPTY (&ch1.unique);
  TEST_ASSERT_LIST_EMPTY (&ch2.unique);
  TEST_ASSERT_LIST_EMPTY (&ch3.unique);
  TEST_ASSERT_LIST_LENGTH(&ch4.unique, 4);
  TEST_ASSERT_LIST_LENGTH(&ch5.unique, 4);
  //ch0 & ch1 same should decrement
  TEST_ASSERT_LIST_LENGTH(&ch0.same, 2);
  TEST_ASSERT_LIST_LENGTH(&ch1.same, 2);
  //ch2 same should be empty
  TEST_ASSERT_LIST_EMPTY (&ch2.same);
  TEST_ASSERT_LIST_EMPTY (&ch3.same);
  TEST_ASSERT_LIST_EMPTY (&ch4.same);
  TEST_ASSERT_LIST_EMPTY (&ch5.same);

  // unregister unique channel
  channel_internal_unregister(&ch5);
  //ch0 unique should decrement
  TEST_ASSERT_LIST_LENGTH(&ch0.unique, 3);
  TEST_ASSERT_LIST_EMPTY (&ch1.unique);
  TEST_ASSERT_LIST_EMPTY (&ch2.unique);
  TEST_ASSERT_LIST_EMPTY (&ch3.unique);
  //ch4 unique should decrement
  TEST_ASSERT_LIST_LENGTH(&ch4.unique, 3);
  //ch5 uniq should be empty
  TEST_ASSERT_LIST_EMPTY (&ch5.unique);
  TEST_ASSERT_LIST_LENGTH(&ch0.same, 2);
  TEST_ASSERT_LIST_LENGTH(&ch1.same, 2);
  TEST_ASSERT_LIST_EMPTY (&ch2.same);
  TEST_ASSERT_LIST_EMPTY (&ch3.same);
  TEST_ASSERT_LIST_EMPTY (&ch4.same);
  TEST_ASSERT_LIST_EMPTY (&ch5.same);

  channel_internal_resetRoot();
}
#endif

#ifdef CONFIG_CHANNEL_TEST_ENABLE_SEND
TEST_CASE("channel_send", "[channel]")
{
  Channel ch;
  char identifier[] = "test_channel";
  void *ctx = (void*)0x1;
  BaseType_t flags = 0x2;
  Channel_callback callback = &returnPdPASS;
  channel_internal_init(&ch, identifier, ctx, flags, callback);

  callback_ctx = NULL;
  callback_data = NULL;
  callback_timeout = 0;
  callback_flags = 0;

  void *data = (void*)0x3;
  TickType_t timeout = 0x4;
  channel_send(&ch, data, timeout);

  TEST_ASSERT_EQUAL_PTR(callback_ctx, ctx);
  TEST_ASSERT_EQUAL_PTR(callback_data, data);
  TEST_ASSERT_EQUAL_UINT(callback_timeout, timeout);
  TEST_ASSERT_EQUAL_UINT(callback_flags, flags);
}
#endif

#ifdef CONFIG_CHANNEL_TEST_ENABLE_BROADCAST_INIT
TEST_CASE("channel_broadcast_init", "[channel]")
{
  Channel ch;
  char identifier[] = "test_channel";
  void *ctx = (void*) 0x1;
  BaseType_t flags = 0x2;
  Channel_callback callback = &returnPdPASS;
  channel_internal_init(&ch, identifier, ctx, flags, callback);

  Channel_broadcast handle;
  void *data = (void*) 0x3;
  TickType_t timeout = 0x4;
  channel_broadcast_init(&handle, &ch, data, timeout);

  TEST_ASSERT_EQUAL_PTR(handle.ch,&ch);
  TEST_ASSERT_EQUAL_PTR(handle.pos,&ch);
  TEST_ASSERT_EQUAL_PTR(handle.data,data);
  TEST_ASSERT_EQUAL_UINT(handle.timeout,timeout);
  TEST_ASSERT_EQUAL_UINT(handle.elapsed,0);
}
#endif

#ifdef CONFIG_CHANNEL_TEST_ENABLE_BROADCAST_FINISHED
TEST_CASE("channel_broadcast_finished", "[channel]")
{
  Channel ch;
  char identifier[] = "test_channel";
  void *ctx = (void*) 0x1;
  BaseType_t flags = 0x2;
  Channel_callback callback = NULL;
  channel_internal_init(&ch, identifier, ctx, flags, callback);

  Channel_broadcast handle;
  void *data = (void*) 0x3;
  TickType_t timeout = 0x4;
  channel_broadcast_init(&handle, &ch, data, timeout);

  TEST_ASSERT_FALSE(channel_broadcast_finished(&handle));
  channel_broadcast(&handle);
  TEST_ASSERT_TRUE(channel_broadcast_finished(&handle));
}
#endif

#ifdef CONFIG_CHANNEL_TEST_ENABLE_BROADCAST
TEST_CASE("channel_broadcast", "[channel]")
{
  channel_internal_resetRoot();
  Channel ch0, ch1, ch2;
  char identifier0[] = "test_channel0";
  char identifier1[] = "test_channel1";
  void *ctx0 = (void*)0x1;
  void *ctx1 = (void*)0x2;
  void *ctx2 = (void*)0x3;
  BaseType_t flags0 = 0x4;
  BaseType_t flags1 = 0x5;
  BaseType_t flags2 = 0x6;
  channel_init(&ch0, identifier0, ctx0, flags0, &returnErrQUEUE_FULL);
  channel_init(&ch1, identifier1, ctx1, flags1, &returnPdPASS);
  channel_init(&ch2, identifier1, ctx2, flags2, &returnErrQUEUE_FULL);

  callback_ctx = NULL;
  callback_data = NULL;
  callback_timeout = 0;
  callback_flags = 0;

  void *data0 = (void*)0x7;
  void *data1 = (void*)0x8;
  void *data2 = (void*)0x9;

  TickType_t timeout0 = 0xA;
  TickType_t timeout1 = 0xB;
  TickType_t timeout2 = 0xC;

  BaseType_t result;
  Channel_broadcast handle;
  channel_broadcast_init(&handle, &ch0, data0, timeout0);
  result = channel_broadcast(&handle);

  TEST_ASSERT(channel_broadcast_finished(&handle));
  TEST_ASSERT_EQUAL_INT(result, errQUEUE_FULL);
  TEST_ASSERT_EQUAL_PTR(handle.pos, NULL);
  TEST_ASSERT_EQUAL_PTR(callback_ctx, ctx0);
  TEST_ASSERT_EQUAL_PTR(callback_data, data0);
  TEST_ASSERT_EQUAL_UINT(callback_timeout, timeout0);
  TEST_ASSERT_EQUAL_UINT(callback_flags, flags0);

  channel_broadcast_init(&handle, &ch1, data2, timeout2);
  result = channel_broadcast(&handle);

  TEST_ASSERT_FALSE(channel_broadcast_finished(&handle));
  TEST_ASSERT_EQUAL_INT(result, errQUEUE_FULL);
  TEST_ASSERT_EQUAL_PTR(handle.pos, &ch2);
  TEST_ASSERT_EQUAL_PTR(callback_ctx, ctx2);
  TEST_ASSERT_EQUAL_PTR(callback_data, data2);
  TEST_ASSERT_EQUAL_UINT(callback_timeout, timeout2);
  TEST_ASSERT_EQUAL_UINT(callback_flags, flags2);

  handle.data = data1;
  handle.timeout = timeout1;
  result = channel_broadcast(&handle);

  TEST_ASSERT(channel_broadcast_finished(&handle));
  TEST_ASSERT_EQUAL_INT(result, pdPASS);
  TEST_ASSERT_EQUAL_PTR(handle.pos, NULL);
  TEST_ASSERT_EQUAL_PTR(callback_ctx, ctx1);
  TEST_ASSERT_EQUAL_PTR(callback_data, data1);
  TEST_ASSERT_EQUAL_UINT(callback_timeout, timeout1);
  TEST_ASSERT_EQUAL_UINT(callback_flags, flags1);

  channel_internal_resetRoot();
}
#endif


#ifdef CONFIG_CHANNEL_TEST_QUEUE_CONSUMER

#define MP_TEST_STACK_SIZE 0x1000
#define CHANNEL_QUEUE_TEST_QUEUE_SIZE 2
#define CHANNEL_QUEUE_TEST_IDENTIFIER_C1 "test_c1"

static float data;
static Channel_consumer cc;


static void channel_consumer_task(void *pvParameters)
{
    for (;;) {
        xQueueReceive((QueueHandle_t)cc.ctx, &data, portMAX_DELAY);
    }
}

TEST_CASE("channel_queue", "[channel]")
{
	static StaticQueue_t cc_queue;
	static QueueHandle_t cc_queue_handle;
	static uint8_t cc_queue_buffer[CHANNEL_QUEUE_TEST_QUEUE_SIZE*sizeof(float)];

	static StackType_t cc_task_stack[MP_TEST_STACK_SIZE];
	static StaticTask_t cc_task;
	static TaskHandle_t cc_task_handle;

	data = 0;

    cc_queue_handle = xQueueCreateStatic(
        CHANNEL_QUEUE_TEST_QUEUE_SIZE,
        sizeof(float),
        cc_queue_buffer,
        &cc_queue
    );
    configASSERT(cc_queue_handle);

    channel_init_consumer(&cc, CHANNEL_QUEUE_TEST_IDENTIFIER_C1, cc_queue_handle);
    
    static Channel_producer cp;
    channel_init_producer(&cp, CHANNEL_QUEUE_TEST_IDENTIFIER_C1);

    cc_task_handle = xTaskCreateStatic(
        channel_consumer_task,
        "channel_consumer_task",
        MP_TEST_STACK_SIZE,
        NULL,
        1,
        cc_task_stack,
        &cc_task
    );
    configASSERT(cc_task_handle);

    Channel_broadcast br;
    float test_data = 3.14;
    channel_broadcast_init(&br, &cp, &test_data, 1);

    TEST_ASSERT_EQUAL_INT(channel_broadcast(&br), pdPASS);
    vTaskDelay(5);
    TEST_ASSERT(channel_broadcast_finished(&br));
    
    TEST_ASSERT_EQUAL_FLOAT(data, test_data);
    vTaskDelete(cc_task_handle);
    vQueueDelete(cc_queue_handle);
    channel_internal_resetRoot();
}
#endif

#ifdef CONFIG_CHANNEL_TEST_QUEUE_TIMEOUT

#define MP_TEST_STACK_SIZE 0x1000
#define CHANNEL_QUEUE_TEST_QUEUE_1_SIZE 4
#define CHANNEL_QUEUE_TEST_QUEUE_2_SIZE 1
#define CHANNEL_QUEUE_TEST_IDENTIFIER_C1 "test_c1"

static float data;
static Channel_consumer cc_1, cc_2;


TEST_CASE("channel_queue timout", "[channel]")
{
	static StaticQueue_t cc_queue_1;
	static QueueHandle_t cc_queue_1_handle;
	static uint8_t cc_queue_1_buffer[CHANNEL_QUEUE_TEST_QUEUE_1_SIZE*sizeof(float)];

    cc_queue_1_handle = xQueueCreateStatic(
        CHANNEL_QUEUE_TEST_QUEUE_1_SIZE,
        sizeof(float),
        cc_queue_1_buffer,
        &cc_queue_1
    );
    configASSERT(cc_queue_1_handle);
    channel_init_consumer(&cc_1, CHANNEL_QUEUE_TEST_IDENTIFIER_C1, cc_queue_1_handle);
	
	static StaticQueue_t cc_queue_2;
	static QueueHandle_t cc_queue_2_handle;
	static uint8_t cc_queue_2_buffer[CHANNEL_QUEUE_TEST_QUEUE_2_SIZE*sizeof(float)];

    cc_queue_2_handle = xQueueCreateStatic(
        CHANNEL_QUEUE_TEST_QUEUE_2_SIZE,
        sizeof(float),
        cc_queue_2_buffer,
        &cc_queue_2
    );
    configASSERT(cc_queue_2_handle);
    channel_init_consumer(&cc_2, CHANNEL_QUEUE_TEST_IDENTIFIER_C1, cc_queue_2_handle);
    
    static Channel_producer cp;
    channel_init_producer(&cp, CHANNEL_QUEUE_TEST_IDENTIFIER_C1);

    Channel_broadcast br;
	float test_data = 0.5;
    channel_broadcast_init(&br, &cp, &test_data, 1);
    TEST_ASSERT_EQUAL_INT(channel_broadcast(&br), pdPASS);

	unsigned int timeout = 10;
    channel_broadcast_init(&br, &cp, &test_data, timeout);
	TickType_t start = xTaskGetTickCount();
    TEST_ASSERT_EQUAL_INT(channel_broadcast(&br), pdFAIL);
    TEST_ASSERT_FALSE(channel_broadcast_finished(&br));
	TickType_t stop = xTaskGetTickCount();
	TEST_ASSERT_EQUAL_UINT(start + timeout, stop);
	TEST_ASSERT_EQUAL_UINT(timeout, br.elapsed);
	TEST_ASSERT_EQUAL_PTR(&cc_2, br.pos);
	TEST_ASSERT(channel_broadcast_timeout(&br));
    vQueueDelete(cc_queue_1_handle);
    vQueueDelete(cc_queue_2_handle);
    channel_internal_resetRoot();
}
#endif