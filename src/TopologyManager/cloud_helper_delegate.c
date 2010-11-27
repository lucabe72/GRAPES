#include <stdlib.h>
#include <stdint.h>
#include <dlfcn.h>

#include "cloud_helper_iface.h"
#include "config.h"

struct delegate_iface {
  void* (*cloud_helper_init)(struct nodeID *local, const char *config);
  int (*get_from_cloud)(void *context, char *key, uint8_t *header_ptr, int header_size);
  int (*put_on_cloud)(void *context, char *key, uint8_t *buffer_ptr, int buffer_size);
  struct nodeID* (*get_cloud_node)(void *context);
  int (*wait4cloud)(void *context, struct timeval *tout);
  int (*recv_from_cloud)(void *context, uint8_t *buffer_ptr, int buffer_size);
};

struct cloud_helper_impl_context {
  struct delegate_iface *delegate;
  void *delegate_context;
};

static struct cloud_helper_impl_context* delegate_cloud_init(struct nodeID *local, const char *config)
{
  struct cloud_helper_impl_context *ctx;
  struct tag *cfg_tags;
  const char *dlib_name;
  struct delegate_iface *delegate_impl;
  void *dlib;

  cfg_tags = config_parse(config);
  dlib_name = config_value_str(cfg_tags, "delegate_lib");
  dlib = dlopen(dlib_name, RTLD_NOW);
  if (dlib == NULL) {
    printf("error: %s", dlerror());
    return NULL;
  }
  

  delegate_impl = (struct delegate_iface *) dlsym(dlib, "delegate_impl");
  if (!delegate_impl) return NULL;

  ctx = malloc(sizeof(struct cloud_helper_impl_context));
  ctx->delegate = delegate_impl;
 
  ctx->delegate_context = ctx->delegate->cloud_helper_init(local, config);
  if(!ctx->delegate_context) {
    free(ctx);
    return NULL;
  }

  return ctx;
}

static int delegate_cloud_get_from_cloud(struct cloud_helper_impl_context *context, char *key, uint8_t *header_ptr, int header_size)
{
  return context->delegate->get_from_cloud(context->delegate_context, key, header_ptr, header_size);
}

static int delegate_cloud_put_on_cloud(struct cloud_helper_impl_context *context, char *key, uint8_t *buffer_ptr, int buffer_size)
{
  return context->delegate->put_on_cloud(context->delegate_context, key, buffer_ptr, buffer_size);
}

static struct nodeID* delegate_cloud_get_cloud_node(struct cloud_helper_impl_context *context)
{
  return context->delegate->get_cloud_node(context->delegate_context);
}

static int delegate_cloud_wait4cloud(struct cloud_helper_impl_context *context, struct timeval *tout)
{
  return context->delegate->wait4cloud(context->delegate_context, tout);
}

static int delegate_cloud_recv_from_cloud(struct cloud_helper_impl_context *context, uint8_t *buffer_ptr, int buffer_size)
{
  return context->delegate->recv_from_cloud(context->delegate_context, buffer_ptr, buffer_size);
}

struct cloud_helper_iface delegate = {
  .cloud_helper_init = delegate_cloud_init,
  .get_from_cloud = delegate_cloud_get_from_cloud,
  .put_on_cloud = delegate_cloud_put_on_cloud,
  .get_cloud_node = delegate_cloud_get_cloud_node,
  .wait4cloud = delegate_cloud_wait4cloud,
  .recv_from_cloud = delegate_cloud_recv_from_cloud,
};
