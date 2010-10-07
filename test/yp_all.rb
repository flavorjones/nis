# -*- ruby -*-
# $Id: yp_all.rb,v 1.1.1.1 2001/04/20 05:38:57 ttate Exp $

require 'nis'

def print_data(status, key, val)
  case status
  when YP::YPERR_SUCCESS
    return status
  else
    if( key && val )
      print("#{key}: #{val}\n")
    end
    return 0
  end
end

ypdomain = YP.get_default_domain
YP.yp_all(ypdomain, 'passwd.byname', :print_data)
