#ifndef SKYNET_MESSAGE_QUEUE_H
#define SKYNET_MESSAGE_QUEUE_H

#include <stdlib.h>
#include <stdint.h>

struct skynet_message {
	uint32_t source;// 消息发送方的服务地址

	// 如果这是一个回应消息，那么要通过session找回对应的一次请求，在lua层，我们每次调用call的时候，都会往对  
    // 方的消息队列中，push一个消息，并且生成一个session，然后将本地的协程挂起，挂起时，会以session为key，协程句  
    // 柄为值，放入一个table中，当回应消息送达时，通过session找到对应的协程，并将其唤醒。后面章节会详细讨论
	int session;
	void * data;
	size_t sz;
};

// type is encoding in skynet_message.sz high 8bit
#define MESSAGE_TYPE_MASK (SIZE_MAX >> 8)
#define MESSAGE_TYPE_SHIFT ((sizeof(size_t)-1) * 8)

struct message_queue;

void skynet_globalmq_push(struct message_queue * queue);
struct message_queue * skynet_globalmq_pop(void);

struct message_queue * skynet_mq_create(uint32_t handle);
void skynet_mq_mark_release(struct message_queue *q);

typedef void (*message_drop)(struct skynet_message *, void *);

void skynet_mq_release(struct message_queue *q, message_drop drop_func, void *ud);
uint32_t skynet_mq_handle(struct message_queue *);

// 0 for success
int skynet_mq_pop(struct message_queue *q, struct skynet_message *message);
void skynet_mq_push(struct message_queue *q, struct skynet_message *message);

// return the length of message queue, for debug
int skynet_mq_length(struct message_queue *q);
int skynet_mq_overload(struct message_queue *q);

void skynet_mq_init();

#endif
