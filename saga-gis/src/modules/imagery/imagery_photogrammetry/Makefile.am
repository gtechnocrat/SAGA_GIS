if DEBUG
DBGFLAGS = -g -DDEBUG
endif
if SAGA_UNICODE
UC_DEFS = -D_SAGA_UNICODE
endif
DEF_SAGA           = -D_SAGA_LINUX -D_TYPEDEF_BYTE -D_TYPEDEF_WORD
CXX_INCS           = -I$(top_srcdir)/src/saga_core
AM_CXXFLAGS        = -fPIC $(CXX_INCS) $(DEF_SAGA) $(UC_DEFS) $(DBGFLAGS) $(GOMPFLAGS)
AM_LDFLAGS         = -fPIC -shared -avoid-version
pkglib_LTLIBRARIES = libimagery_photogrammetry.la
libimagery_photogrammetry_la_SOURCES =\
Colorisation.cpp\
methods.cpp\
MLB_Interface.cpp\
Resection.cpp\
Colorisation.h\
methods.h\
MLB_Interface.h\
Resection.h

libimagery_photogrammetry_la_LIBADD = $(top_srcdir)/src/saga_core/saga_api/libsaga_api.la

