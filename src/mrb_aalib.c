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
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/array.h"
#include "mruby/variable.h"
#include "mrb_aalib.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

static void
aacontext_free(mrb_state *mrb, void *ptr) {
// fprintf(stderr, "aa_close %p\n", ptr);
/*
  if (ptr != NULL) {
    aa_close(ptr);
  }
*/
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

  argc = mrb_get_args(mrb, "|z", &driver_name);
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
  if (aa != NULL) {
    aa_close(aa);
  }

  return mrb_nil_value();
}

static mrb_value
mrb_aacontext_hidecursor(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);
  aa_hidecursor(aa);

  return mrb_nil_value();
}

static mrb_value
mrb_aacontext_initkbd(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);
  int argc, mode, ret, i;
  char *driver_name;

  argc = mrb_get_args(mrb, "i|z", &mode, &driver_name);
  if (argc == 1) {
    ret = aa_autoinitkbd(aa, mode);
  } else {
    for (i = 0; aa_kbddrivers[i] != NULL; i++) {
      if (strcmp(driver_name, aa_kbddrivers[i]->shortname) == 0) {
        ret = aa_initkbd(aa, aa_kbddrivers[i], mode);
        break;
      }
    }
    if (aa_kbddrivers[i] == NULL) {
      mrb_raisef(mrb, E_ARGUMENT_ERROR, "unknown driver \"%S\"", mrb_str_new_cstr(mrb, driver_name));
    }
  }
  return mrb_fixnum_value(ret);
}

static mrb_value
mrb_aacontext_fastrender(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);
  int x1, y1, x2, y2;
  mrb_get_args(mrb, "iiii", &x1, &y1, &x2, &y2);
  aa_fastrender(aa, x1, y1, x2, y2);

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
mrb_aacontext_puts(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);
  int x, y, mode;
  char *s;

  mrb_get_args(mrb, "iiiz", &x, &y, &mode, &s);
  aa_puts(aa, x, y, mode, s);
  return mrb_nil_value();
}

static mrb_value
mrb_aacontext_text(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);

  return mrb_str_new_cstr(mrb, (const char *)aa_text(aa));
}

static mrb_value
mrb_aacontext_getkey(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);
  int wait;

  mrb_get_args(mrb, "i", &wait);
  return mrb_fixnum_value(aa_getkey(aa, wait));
}

static mrb_value
mrb_aacontext_edit(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);
  int x, y, size, maxsize;
  char *s1, *s2;

  mrb_get_args(mrb, "iiizi", &x, &y, &size, &s2, &maxsize);
  s1 = (char *)mrb_malloc(mrb, sizeof(char)*maxsize);
  strcpy(s1, s2);
  aa_edit(aa, x, y, size, s1, maxsize);
  return mrb_str_new_cstr(mrb, s1);
}

static mrb_value
mrb_aacontext_initmouse(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);
  int argc, mode, ret, i;
  char *driver_name;

  argc = mrb_get_args(mrb, "i|z", &mode, &driver_name);
  if (argc == 1) {
    ret = aa_autoinitmouse(aa, mode);
  } else {
    for (i = 0; aa_mousedrivers[i] != NULL; i++) {
      if (strcmp(driver_name, aa_mousedrivers[i]->shortname) == 0) {
        ret = aa_initmouse(aa, aa_mousedrivers[i], mode);
        break;
      }
    }
    if (aa_mousedrivers[i] == NULL) {
      mrb_raisef(mrb, E_ARGUMENT_ERROR, "unknown driver \"%S\"", mrb_str_new_cstr(mrb, driver_name));
    }
  }
  return mrb_fixnum_value(ret);
}

static mrb_value
mrb_aacontext_getevent(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);
  int wait, ret;

  mrb_get_args(mrb, "i", &wait);
  ret = aa_getevent(aa, wait);
  return mrb_fixnum_value(ret);
}

static mrb_value
mrb_aacontext_getmouse(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);
  mrb_value ret;
  int x, y, b;

  aa_getmouse(aa, &x, &y, &b);
  ret = mrb_ary_new(mrb);
  mrb_ary_push(mrb, ret, mrb_fixnum_value(x));
  mrb_ary_push(mrb, ret, mrb_fixnum_value(y));
  mrb_ary_push(mrb, ret, mrb_fixnum_value(b));
  return ret;
}

static mrb_value
mrb_aacontext_hidemouse(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);
  aa_hidemouse(aa);
  return mrb_nil_value();
}

static mrb_value
mrb_aacontext_uninitmouse(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);
  aa_uninitmouse(aa);
  return mrb_nil_value();
}

static mrb_value
mrb_aacontext_image(mrb_state *mrb, mrb_value self)
{
  aa_context *aa = (aa_context *)DATA_PTR(self);

  return mrb_str_new_cstr(mrb, (const char *)aa_image(aa));
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

void
mrb_mruby_aalib_gem_init(mrb_state *mrb)
{
  struct RClass *aalib, *aa_context, *aa_palette;
  aalib = mrb_define_module(mrb, "AAlib");
  aa_context = mrb_define_class_under(mrb, aalib, "Context", mrb->object_class);
  MRB_SET_INSTANCE_TT(aa_context, MRB_TT_DATA);
  aa_palette = mrb_define_class_under(mrb, aalib, "Palette", mrb->object_class);
  mrb_define_method(mrb, aa_context, "initialize", mrb_aacontext_init, MRB_ARGS_OPT(1));
  mrb_define_method(mrb, aa_context, "putpixel", mrb_aacontext_putpixel, MRB_ARGS_REQ(3));
  mrb_define_method(mrb, aa_context, "close", mrb_aacontext_close, MRB_ARGS_NONE());
  mrb_define_method(mrb, aa_context, "hidecursor", mrb_aacontext_hidecursor, MRB_ARGS_NONE());

  mrb_define_method(mrb, aa_context, "initkbd", mrb_aacontext_initkbd, MRB_ARGS_ARG(1, 1));
  mrb_define_method(mrb, aa_context, "fastrender", mrb_aacontext_fastrender, MRB_ARGS_REQ(4));
  mrb_define_method(mrb, aa_context, "flush", mrb_aacontext_flush, MRB_ARGS_NONE());
  mrb_define_method(mrb, aa_context, "puts", mrb_aacontext_puts, MRB_ARGS_REQ(4)); 
  mrb_define_method(mrb, aa_context, "image", mrb_aacontext_image, MRB_ARGS_NONE());
  mrb_define_method(mrb, aa_context, "text", mrb_aacontext_text, MRB_ARGS_NONE());
  mrb_define_method(mrb, aa_context, "getkey", mrb_aacontext_getkey, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, aa_context, "edit", mrb_aacontext_edit, MRB_ARGS_REQ(5));
  mrb_define_method(mrb, aa_context, "initmouse", mrb_aacontext_initmouse, MRB_ARGS_ARG(1, 1));
  mrb_define_method(mrb, aa_context, "getevent", mrb_aacontext_getevent, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, aa_context, "getmouse", mrb_aacontext_getmouse, MRB_ARGS_NONE());
  mrb_define_method(mrb, aa_context, "hidemouse", mrb_aacontext_hidemouse, MRB_ARGS_NONE());
  mrb_define_method(mrb, aa_context, "uninitmouse", mrb_aacontext_uninitmouse, MRB_ARGS_NONE());

  mrb_define_method(mrb, aa_context, "scrwidth", mrb_aacontext_scrwidth, MRB_ARGS_NONE());
  mrb_define_method(mrb, aa_context, "scrheight", mrb_aacontext_scrheight, MRB_ARGS_NONE());
  mrb_define_method(mrb, aa_context, "imgwidth", mrb_aacontext_imgwidth, MRB_ARGS_NONE());
  mrb_define_method(mrb, aa_context, "imgheight", mrb_aacontext_imgheight, MRB_ARGS_NONE());

  mrb_define_const(mrb, aalib, "SPECIAL", mrb_fixnum_value(AA_SPECIAL));
  mrb_define_const(mrb, aalib, "NONE", mrb_fixnum_value(AA_NONE));
  mrb_define_const(mrb, aalib, "MOUSEALLMASK", mrb_fixnum_value(AA_MOUSEALLMASK));
  DONE;
}

void mrb_mruby_aalib_gem_final(mrb_state *mrb)
{
}

