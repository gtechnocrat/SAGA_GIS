#
# $Id$
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
pkglib_LTLIBRARIES = libtable_calculus.la
libtable_calculus_la_SOURCES =\
Fit.cpp\
LMFit.cpp\
MLB_Interface.cpp\
Table_Calculator.cpp\
table_cluster_analysis.cpp\
table_field_analyzer.cpp\
table_field_statistics.cpp\
table_fill_record_gaps.cpp\
table_mRMR.cpp\
table_pca.cpp\
table_running_average.cpp\
Fit.h\
LMFit.h\
MLB_Interface.h\
Table_Calculator.h\
table_cluster_analysis.h\
table_field_analyzer.h\
table_field_statistics.h\
table_fill_record_gaps.h\
table_mRMR.h\
table_pca.h\
table_running_average.h

libtable_calculus_la_LIBADD = $(top_srcdir)/src/saga_core/saga_api/libsaga_api.la

