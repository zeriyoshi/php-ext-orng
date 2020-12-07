/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 0fff57d9fb0ba45108e8ab960daf6e757c2ad069 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_ORNG_MT19937PHP___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, seed, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_MT19937PHP_next, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_ORNG_MT19937PHP_range, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, min, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, max, IS_LONG, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(ORNG_MT19937PHP, __construct);
ZEND_METHOD(ORNG_MT19937PHP, next);
ZEND_METHOD(ORNG_MT19937PHP, range);


static const zend_function_entry class_ORNG_MT19937PHP_methods[] = {
	ZEND_ME(ORNG_MT19937PHP, __construct, arginfo_class_ORNG_MT19937PHP___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_MT19937PHP, next, arginfo_class_ORNG_MT19937PHP_next, ZEND_ACC_PUBLIC)
	ZEND_ME(ORNG_MT19937PHP, range, arginfo_class_ORNG_MT19937PHP_range, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
