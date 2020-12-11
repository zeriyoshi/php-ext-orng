/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: f7560d0c21166a068325709558ecf6eab4e2fa51 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_ORNG_MT19937__construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, seed, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_MT19937_next, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_MT19937_range, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, min, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, max, IS_LONG, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(ORNG_MT19937, __construct);
ZEND_METHOD(ORNG_MT19937, next);
ZEND_METHOD(ORNG_MT19937, range);


static const zend_function_entry class_ORNG_MT19937_methods[] = {
	ZEND_ME(ORNG_MT19937, __construct, arginfo_class_ORNG_MT19937__construct, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_MT19937, next, arginfo_class_ORNG_MT19937_next, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_MT19937, range, arginfo_class_ORNG_MT19937_range, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
