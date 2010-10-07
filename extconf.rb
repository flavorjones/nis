require 'mkmf'

dir_config('yp')
dir_config('nis')

nis_h = have_header('rpcsvc/nis.h')
ypclnt_h = have_header('rpcsvc/ypclnt.h')
ypprot_h = have_header('rpcsvc/yp_prot.h')
libnsl = have_library('nsl')

if( libnsl && ypclnt_h && ypprot_h )
  create_makefile('nis')
end
