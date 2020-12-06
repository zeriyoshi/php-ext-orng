/* For compatibility with older PHP versions */
#ifndef ZEND_ABSTRACT_ME_WITH_FLAGS
#define ZEND_ABSTRACT_ME_WITH_FLAGS(classname, name, arg_info, flags) ZEND_RAW_FENTRY(#name, NULL, arg_info, flags)
#endif