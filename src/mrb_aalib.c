/*
** mrb_aalib.c - AAlib class
**
** Copyright (c) masahino 2018
**
** See Copyright Notice in LICENSE
*/

#include <stdlib.h>
#include <string.h>
#include <aalib.h>
#include "mruby.h"
#include "mruby/data.h"
#include "mruby/array.h"
#include "mruby/variable.h"
#include "mrb_aalib.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

static void
aacontext_free(mrb_state *mrb, void *ptr) {
 fprintf(stderr, "scinterm_free %p\n", ptr);
  if (ptr != NULL) {
    aa_close(ptr);
  }
}

static const struct mrb_data_type mrb_aacontext_data_type = {
  "mrb_aacontext_data", aacontext_free,
};

static mrb_value
mrb_aacontext_init(mrb_state *mrb, mrb_value self)
{
  aa_context *context = NULL;
  int i, argc;
  char *driver_name;

  argc = mrb_get_args(mrb, "z", &driver_name);
  if (argc == 0) {
    context = aa_autoinit(&aa_defparams);
  } else {
    for (i = 0; aa_drivers[i] != NULL; i++) {
      if (strcmp(driver_name, aa_drivers[i]->shortname) == 0) {
        context = aa_init(aa_drivers[i], &aa_defparams, NULL);
        break;
      }
    }
  }
  if (context == NULL) {
    fprintf(stderr, "Cannot initialize AA-lib.\n");
    exit(1);
  }
  DATA_TYPE(self) = &mrb_aacontext_data_type;
  DATA_PTR(self) = NULL;

  DATA_PTR(self) = context;

  return self;
}

static mrb_value
mrb_aacontext_putpixel(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);
  mrb_int x, y, color;
  mrb_get_args(mrb, "iii", &x, &y, &color);
  aa_putpixel(aa, x, y, color);

  return mrb_nil_value();
}

static mrb_value
mrb_aacontext_close(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);
  aa_close(aa);

  return mrb_nil_value();
}

static mrb_value
mrb_aacontext_render(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);

  aa_fastrender(aa, 0, 0, aa_scrwidth(aa), aa_scrheight(aa));

  return mrb_nil_value();
}

static mrb_value
mrb_aacontext_flush(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);

  aa_flush(aa);

  return mrb_nil_value();
}

static mrb_value
mrb_aacontext_text(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);

  return mrb_str_new_cstr(mrb, aa_text(aa));
}

static mrb_value
mrb_aacontext_image(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);

  return mrb_str_new_cstr(mrb, aa_image(aa));
}

static mrb_value
mrb_aacontext_scrwidth(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);

  return mrb_fixnum_value(aa_scrwidth(aa));
}


static mrb_value
mrb_aacontext_scrheight(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);

  return mrb_fixnum_value(aa_scrheight(aa));
}

static mrb_value
mrb_aacontext_imgwidth(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);

  return mrb_fixnum_value(aa_imgwidth(aa));
}


static mrb_value
mrb_aacontext_imgheight(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);

  return mrb_fixnum_value(aa_imgheight(aa));
}


static mrb_value
mrb_aalib_hi(mrb_state *mrb, mrb_value self)
{
  return mrb_str_new_cstr(mrb, "hi!!");
}

void
mrb_mruby_aalib_gem_init(mrb_state *mrb)
{
  struct RClass *aalib, *aa_context, *aa_driver;
  aalib = mrb_define_module(mrb, "AAlib");
  aa_context = mrb_define_class_under(mrb, aalib, "Context", mrb->object_class);
  mrb_define_method(mrb, aa_context, "initialize", mrb_aacontext_init, MRB_ARGS_OPT(1));
  mrb_define_method(mrb, aa_context, "putpixel", mrb_aacontext_putpixel, MRB_ARGS_REQ(3));
  mrb_define_method(mrb, aa_context, "close", mrb_aacontext_close, MRB_ARGS_NONE());

  mrb_define_method(mrb, aa_context, "render", mrb_aacontext_render, MRB_ARGS_NONE());
  mrb_define_method(mrb, aa_context, "flush", mrb_aacontext_flush, MRB_ARGS_NONE());

  mrb_define_method(mrb, aa_context, "image", mrb_aacontext_image, MRB_ARGS_NONE());
  mrb_define_method(mrb, aa_context, "text", mrb_aacontext_text, MRB_ARGS_NONE());

  mrb_define_method(mrb, aa_context, "scrwidth", mrb_aacontext_scrwidth, MRB_ARGS_NONE());
  mrb_define_method(mrb, aa_context, "scrheight", mrb_aacontext_scrheight, MRB_ARGS_NONE());
  mrb_define_method(mrb, aa_context, "imgwidth", mrb_aacontext_imgwidth, MRB_ARGS_NONE());
  mrb_define_method(mrb, aa_context, "imgheight", mrb_aacontext_imgheight, MRB_ARGS_NONE());

  aa_driver = mrb_define_class_under(mrb, aalib, "Driver", mrb->object_class);
  
  DONE;
}

void mrb_mruby_aalib_gem_final(mrb_state *mrb)
{
}

