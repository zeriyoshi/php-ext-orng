--TEST--
Check range method in unexcepted values. (PHP >= 8.0)
--SKIPIF--
<?php
if (!extension_loaded('orng')) {
    die('skip: extension not loaded.');
}
if (version_compare(PHP_VERSION, '8.0.0', '<')) {
    die('skip: this test only available on PHP >= 8.0.');
}
?>
--FILE--
<?php

const SEED = 10;

$classes = [
    \ORNG\XorShift128Plus::class,
    \ORNG\GLibCRand::class,
    \ORNG\MT19937::class,
    \ORNG\MT19937PHP::class,
];

foreach ($classes as $class) {
    try {
        (new $class(SEED))->range(10, 1);
    } catch (ValueError $e) {
        echo $e . PHP_EOL;
    }
}
?>
--EXPECTF--
ValueError: ORNG\XorShift128Plus::range(): Argument #2 ($max) must be greater than or equal to argument #1 ($min) in %s:%d
Stack trace:
#0 %s(%d): ORNG\XorShift128Plus->range(10, 1)
#1 {main}
ValueError: ORNG\GLibCRand::range(): Argument #2 ($max) must be greater than or equal to argument #1 ($min) in %s:%d
Stack trace:
#0 %s(%d): ORNG\GLibCRand->range(10, 1)
#1 {main}
ValueError: ORNG\MT19937::range(): Argument #2 ($max) must be greater than or equal to argument #1 ($min) in %s:%d
Stack trace:
#0 %s(%d): ORNG\MT19937->range(10, 1)
#1 {main}
ValueError: ORNG\MT19937PHP::range(): Argument #2 ($max) must be greater than or equal to argument #1 ($min) in %s:%d
Stack trace:
#0 %s(%d): ORNG\MT19937PHP->range(10, 1)
#1 {main}