/* For compatibility with older PHP versions */
#ifndef ZEND_ABSTRACT_ME_WITH_FLAGS
#define ZEND_ABSTRACT_ME_WITH_FLAGS(classname, name, arg_info, flags) ZEND_RAW_FENTRY(#name, NULL, arg_info, flags)
#endif
#if PHP_API_VERSION >= 20200930
# define COMPAT_RETURN_ERROR_OR_THROW_MAX_SMALLER_THAN_MIN() \
	zend_argument_value_error(2, "must be greater than or equal to argument #1 ($min)"); \
	RETURN_THROWS();
#else
# define COMPAT_RETURN_ERROR_OR_THROW_MAX_SMALLER_THAN_MIN() \
	php_error_docref(NULL, E_WARNING, "max(" ZEND_LONG_FMT ") is smaller than min(" ZEND_LONG_FMT ")", max, min); \
	RETURN_FALSE;
#endif
