/* -*- C -*-
 * $Id: nis.h,v 1.1.1.1 2001/04/20 05:38:57 ttate Exp $
 */

#ifndef NIS_H
#define NIS_H

#include <ruby.h>

void Init_yp();
extern VALUE rb_mYP;
extern VALUE rb_eYPError;

#endif
