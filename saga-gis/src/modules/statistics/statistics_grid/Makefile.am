#
# $Id: Makefile.am 1554 2012-12-05 18:29:04Z oconrad $
#
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
pkglib_LTLIBRARIES = libstatistics_grid.la
libstatistics_grid_la_SOURCES =\
fast_representativeness.cpp\
grid_autocorrelation.cpp\
grid_latlon_statistics.cpp\
grid_pca.cpp\
multiband_variation.cpp\
GSGrid_Directional_Statistics.cpp\
GSGrid_Residuals.cpp\
GSGrid_Statistics.cpp\
GSGrid_Variance.cpp\
GSGrid_Variance_Radius.cpp\
GSGrid_Zonal_Statistics.cpp\
MLB_Interface.cpp\
fast_representativeness.h\
grid_autocorrelation.h\
grid_latlon_statistics.h\
grid_pca.h\
multiband_variation.h\
GSGrid_Directional_Statistics.h\
GSGrid_Residuals.h\
GSGrid_Statistics.h\
GSGrid_Variance.h\
GSGrid_Variance_Radius.h\
GSGrid_Zonal_Statistics.h\
MLB_Interface.h

libstatistics_grid_la_LIBADD = $(top_srcdir)/src/saga_core/saga_api/libsaga_api.la

