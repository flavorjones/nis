# -*- ruby -*-
# $Id: yp_first.rb,v 1.1.1.1 2001/04/20 05:38:57 ttate Exp $

require 'nis'

ypdomain = YP.get_default_domain
key,val = YP.yp_first(ypdomain, 'passwd.byname')
while( key && val )
  print("#{key}: #{val}\n")
  key,val = YP.yp_next(ypdomain, 'passwd.byname', key)
end
