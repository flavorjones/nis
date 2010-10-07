# -*- ruby -*-
# $Id: yp_match.rb,v 1.1.1.1 2001/04/20 05:38:57 ttate Exp $

require 'nis'

ypdomain = YP.get_default_domain
p YP.yp_match(ypdomain, 'passwd.byname', 'ttate')
