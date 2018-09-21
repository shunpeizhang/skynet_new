#ifndef SKYNET_MODULE_H
#define SKYNET_MODULE_H

struct skynet_context;

typedef void * (*skynet_dl_create)(void);
typedef int (*skynet_dl_init)(void * inst, struct skynet_context *, const char * parm);
typedef void (*skynet_dl_release)(void * inst);
typedef void (*skynet_dl_signal)(void * inst, int signal);

struct skynet_module {
	const char * name;// C服务名称，一般是C服务的文件名
	void * module;// 访问该so库的dl句柄，该句柄通过dlopen函数获得
	skynet_dl_create create;// 绑定so库中的xxx_create函数，通过dlsym函数实现绑定，调用该create即是调用xxx_create
	skynet_dl_init init;// 绑定so库中的xxx_init函数，调用该init即是调用xxx_init
	skynet_dl_release release;// 绑定so库中的xxx_release函数，调用该release即是调用xxx_release
	skynet_dl_signal signal;// 绑定so库中的xxx_signal函数，调用该signal即是调用xxx_signal
};

void skynet_module_insert(struct skynet_module *mod);
struct skynet_module * skynet_module_query(const char * name);
void * skynet_module_instance_create(struct skynet_module *);
int skynet_module_instance_init(struct skynet_module *, void * inst, struct skynet_context *ctx, const char * parm);
void skynet_module_instance_release(struct skynet_module *, void *inst);
void skynet_module_instance_signal(struct skynet_module *, void *inst, int signal);

void skynet_module_init(const char *path);

#endif
