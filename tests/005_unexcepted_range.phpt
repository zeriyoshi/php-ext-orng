--TEST--
Check range method in unexcepted values. (PHP < 8.0)
--SKIPIF--
<?php
if (!extension_loaded('orng')) {
    die('skip: extension not loaded.');
}
if (version_compare(PHP_VERSION, '8.0.0', '>=')) {
    die('skip: this test only available on PHP < 8.0.');
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
    (new $class(SEED))->range(10, 1);
}
?>
--EXPECTF--
Warning: ORNG\XorShift128Plus::range(): max(1) is smaller than min(10) in %s on line %d

Warning: ORNG\GLibCRand::range(): max(1) is smaller than min(10) in %s on line %d

Warning: ORNG\MT19937::range(): max(1) is smaller than min(10) in %s on line %d

Warning: ORNG\MT19937PHP::range(): max(1) is smaller than min(10) in %s on line %d