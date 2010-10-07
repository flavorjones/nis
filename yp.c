/* -*- C -*-
 * $Id: yp.c,v 1.4 2001/04/20 07:07:42 ttate Exp $
 */

#include "nis.h"
#include <rpcsvc/ypclnt.h>
#include <rpcsvc/yp_prot.h>

#ifndef YPERR_SUCCESS
# define YPERR_SUCCESS (0)
#endif

VALUE rb_mYP;
VALUE rb_eYPError;

void
rb_yp_check_yperr(err)
{
  switch( err ){
  case YPERR_SUCCESS:
    break;
  default:
    rb_raise(rb_eYPError, yperr_string(err));
  };
};

VALUE
rb_yp_get_default_domain(VALUE self)
{
  char *domainname;
  int err;

  if((err = yp_get_default_domain(&domainname)) != 0){
    rb_raise(rb_eYPError, yperr_string(err));
  };
  return rb_tainted_str_new2(domainname);
};

VALUE
rb_yperr_string(VALUE self, VALUE err)
{
  return rb_tainted_str_new2(yperr_string(FIX2INT(err)));
};

static int
rb_ypall_callback(int status, char *inkey, int inkeylen, char *inval,
		  int invallen, char *indata)
{
  VALUE cb = (VALUE)indata;
  VALUE res, key, val;

  if( inkeylen > 0 ){
    key = rb_tainted_str_new(inkey, inkeylen);
  }
  else{
    key = Qnil;
  };

  if( invallen > 0 ){
    val = rb_tainted_str_new(inval, invallen);
  }
  else{
    val = Qnil;
  };

  if( SYMBOL_P(cb) ){
    res = rb_funcall(rb_mKernel, SYM2ID(cb), 3,
		     INT2NUM(status), key, val);
  }
  else if( cb == Qnil ){
    res = rb_yield(rb_ary_new3(3, INT2NUM(status), key, val));
  }
  else{
    res = rb_funcall(cb, rb_intern("call"), 3,
		     INT2NUM(status), key, val);
  };

  /* return value:
   * 0: call again for additional key-value pair
   * nonzero: don't call again
   */
  if( TYPE(res) != T_FIXNUM ){
    if( res == Qfalse || res == Qnil ){
      return 0;
    }
    else{
      return 1;
    };
  }
  else{
    return FIX2INT(res);
  };
};

VALUE
rb_yp_all(int argc, VALUE argv[], VALUE self)
{
  int res;
  struct ypall_callback ypcb;
  VALUE domainname, map, cb;

  rb_scan_args(argc, argv, "21", &domainname, &map, &cb);
  if( domainname == Qnil ){
    domainname = rb_yp_get_default_domain(self);
  };
  switch( argc ){
  case 3:
    ypcb.foreach = rb_ypall_callback;
    ypcb.data    = (char*)cb;
    break;
  case 2:
    ypcb.foreach = rb_ypall_callback;
    ypcb.data    = (char*)Qnil;
    break;
  };

  res = yp_all(STR2CSTR(domainname), STR2CSTR(map), &ypcb);
  rb_yp_check_yperr(res);

  return Qnil;
};

VALUE
rb_yp_first(VALUE self, VALUE domain, VALUE map)
{
  char *key, *val;
  int keylen, vallen;
  int res;
  VALUE vkey, vval;

  if( domain == Qnil ){
    domain = rb_yp_get_default_domain(self);
  };

  res = yp_first(STR2CSTR(domain), STR2CSTR(map), &key, &keylen, &val, &vallen);
  rb_yp_check_yperr(res);

  if( keylen > 0 ){
    vkey = rb_tainted_str_new(key, keylen);
  }
  else{
    vkey = Qnil;
  };

  if( vallen > 0 ){
    vval = rb_tainted_str_new(val, vallen);
  }
  else{
    vval = Qnil;
  };

  return rb_assoc_new(vkey, vval);
};

VALUE
rb_yp_next(VALUE self, VALUE domain, VALUE map, VALUE inkey)
{
  char *key, *val;
  int keylen, vallen;
  int res;
  VALUE vkey, vval;

  if( domain == Qnil ){
    domain = rb_yp_get_default_domain(self);
  };

  res = yp_next(STR2CSTR(domain), STR2CSTR(map),
		STR2CSTR(inkey), RSTRING(inkey)->len,
		&key, &keylen, &val, &vallen);
  rb_yp_check_yperr(res);

  if( keylen > 0 ){
    vkey = rb_tainted_str_new(key, keylen);
  }
  else{
    vkey = Qnil;
  };

  if( vallen > 0 ){
    vval = rb_tainted_str_new(val, vallen);
  }
  else{
    vval = Qnil;
  };

  return rb_assoc_new(vkey, vval);
};

VALUE
rb_yp_update(VALUE self, VALUE domain, VALUE map, VALUE ypop, VALUE inkey, VALUE inval)
{
  int res;

  if( domain == Qnil ){
    domain = rb_yp_get_default_domain(self);
  };

  res = yp_update(STR2CSTR(domain), STR2CSTR(map), FIX2INT(ypop),
		  STR2CSTR(inkey), RSTRING(inkey)->len,
		  STR2CSTR(inval), RSTRING(inval)->len);
  rb_yp_check_yperr(res);

  return INT2NUM(res);
};

VALUE
rb_yp_match(VALUE self, VALUE domain, VALUE map, VALUE inkey)
{
  int res;
  char *val;
  int vallen;

  if( domain == Qnil ){
    domain = rb_yp_get_default_domain(self);
  };

  res = yp_match(STR2CSTR(domain), STR2CSTR(map),
		 STR2CSTR(inkey), RSTRING(inkey)->len,
		 &val, &vallen);
  rb_yp_check_yperr(res);

  if( vallen > 0 ){
    return rb_tainted_str_new(val, vallen);
  }
  else{
    return Qnil;
  };
};

VALUE
rb_yp_order(VALUE self, VALUE domain, VALUE map)
{
  int res, order;

  if( domain == Qnil ){
    domain = rb_yp_get_default_domain(self);
  };

  res = yp_order(STR2CSTR(domain), STR2CSTR(map), &order);
  rb_yp_check_yperr(res);

  return INT2NUM(order);
};

VALUE
rb_yp_master(VALUE self, VALUE domain, VALUE map)
{
  int res;
  char *master;
  VALUE obj;

  if( domain == Qnil ){
    domain = rb_yp_get_default_domain(self);
  };

  res = yp_master(STR2CSTR(domain), STR2CSTR(map), &master);
  rb_yp_check_yperr(res);

  obj = rb_tainted_str_new2(master);
  free(master);

  return obj;
};

void
Init_yp()
{
  rb_mYP = rb_define_module("YP");
  rb_eYPError = rb_define_class_under(rb_mYP, "YPError", rb_eRuntimeError);

#define rb_yp_define_yperr(x) rb_define_const(rb_mYP, #x, INT2FIX(x));
  rb_yp_define_yperr(YPERR_SUCCESS);
  rb_yp_define_yperr(YPERR_BADARGS);
  rb_yp_define_yperr(YPERR_RPC);
  rb_yp_define_yperr(YPERR_DOMAIN);
  rb_yp_define_yperr(YPERR_MAP);
  rb_yp_define_yperr(YPERR_KEY);
  rb_yp_define_yperr(YPERR_YPERR);
  rb_yp_define_yperr(YPERR_RESRC);
  rb_yp_define_yperr(YPERR_NOMORE);
  rb_yp_define_yperr(YPERR_PMAP);
  rb_yp_define_yperr(YPERR_YPBIND);
  rb_yp_define_yperr(YPERR_YPSERV);
  rb_yp_define_yperr(YPERR_NODOM);
  rb_yp_define_yperr(YPERR_BADDB);
  rb_yp_define_yperr(YPERR_VERS);
  rb_yp_define_yperr(YPERR_ACCESS);
  rb_yp_define_yperr(YPERR_BUSY);
#undef rb_yp_define_yperr

#define rb_yp_define_ypop(x) rb_define_const(rb_mYP, #x, INT2FIX(x));
  rb_yp_define_ypop(YPOP_CHANGE);
  rb_yp_define_ypop(YPOP_INSERT);
  rb_yp_define_ypop(YPOP_DELETE);
  rb_yp_define_ypop(YPOP_STORE);
#undef rb_yp_define_ypop

  rb_define_module_function(rb_mYP, "get_default_domain",
			    rb_yp_get_default_domain, 0);
  rb_define_module_function(rb_mYP, "yp_all", rb_yp_all, -1);
  rb_define_module_function(rb_mYP, "yp_first", rb_yp_first, 2);
  rb_define_module_function(rb_mYP, "yp_next", rb_yp_next, 3);
  rb_define_module_function(rb_mYP, "yp_update", rb_yp_update, 5);
  rb_define_module_function(rb_mYP, "yp_match", rb_yp_match, 3);
  rb_define_module_function(rb_mYP, "yp_order", rb_yp_order, 2);
  rb_define_module_function(rb_mYP, "yp_master", rb_yp_master, 2);
  rb_define_module_function(rb_mYP, "err_string", rb_yperr_string, 1);
};
