# snappy.m4 --
#
#	autoconf macros for nsfSnappy
#


#------------------------------------------------------------------------
# SC_PATH_SNAPPYCONFIG --
#
#	Locate the Snappy public header files (snappy-c.h)
#	Locate folder containing snappy library files
# Arguments:
#	none
#
# Results:
#
#	Adds the following arguments to configure:
#		--with-snappy-include=...
#               --with-snappy-lib=...
#
#------------------------------------------------------------------------

AC_DEFUN([TEA_PATH_SNAPPY], [
    if test x"${no_snappy_include}" = x ; then
	no_snappy_include=true
	AC_ARG_WITH(snappyinclude,
	    AS_HELP_STRING([--with-snappyinclude], [directory containing public header files (snappy-c.h)]),
	    [with_snappy_include=$withval], AC_MSG_ERROR([--with-snappyinclude is required]))
	AC_MSG_CHECKING([for Snappy public header files])
	AC_CACHE_VAL(ac_cv_c_snappy_include,[
	    if test x"${with_snappy_include}" != x ; then
		if test -f "${with_snappy_include}/snappy-c.h" ; then
		    ac_cv_c_snappy_include=`(cd ${with_snappy_include}; pwd)`
		else
		    AC_MSG_ERROR([${with_snappy_include} directory doesn't contain snappy-c.h])
		fi
	    fi
	])
	if test x"${ac_cv_c_snappy_include}" = x ; then
	    AC_MSG_WARN(Can't find snappy public header files)
	    exit 0
	else
	    AC_MSG_RESULT(found ${with_snappy_include}/snappy-c.h)
	fi
    fi

    if test x"${no_snappy_lib}" = x ; then
	no_snappy_lib=true
	AC_ARG_WITH(snappylib,
	    AS_HELP_STRING([--with-snappylib], [directory containing library files]),
	    [with_snappy_lib=$withval], AC_MSG_ERROR([--with-snappylib is required]))
	AC_MSG_CHECKING([for Snappy library files])
	AC_CACHE_VAL(ac_cv_c_snappy_lib,[
	    if test x"${with_snappy_lib}" != x ; then
		if test -d "${with_snappy_lib}" ; then
		    ac_cv_c_snappy_lib=`(cd ${with_snappy_lib}; pwd)`
		else
		    AC_MSG_ERROR([${with_snappy_lib} directory doesn't exist])
		fi
	    fi
	])
	if test x"${ac_cv_c_snappy_lib}" = x ; then
	    AC_MSG_WARN(Can't find snappy directory containing library files)
	    exit 0
	else
	    AC_MSG_RESULT(found ${with_snappy_lib})
	fi
    fi

])


#------------------------------------------------------------------------
# SC_LOAD_SNAPPY --
#
#	Add include and lib paths
#
# Arguments:
#	none
#
# Results:
#
#
#------------------------------------------------------------------------

AC_DEFUN(TEA_LOAD_SNAPPY, [
    if test x"${with_snappy_include}" != x ; then
	TEA_ADD_INCLUDES([-I${with_snappy_include}])
    fi

    if test x"${with_snappy_lib}" != x ; then
	TEA_ADD_LIBS([-L${with_snappy_lib} -lsnappy])
    fi
])

