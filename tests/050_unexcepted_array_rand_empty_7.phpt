--TEST--
Check arrayRand method in unexcepted empty array. (PHP >= 8.0)
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

$classes = include('data/classes.inc');

foreach ($classes as $class) {
    try {
        (new $class(SEED))->arrayRand([], 1);
    } catch (ValueError $e) {
        echo $e . PHP_EOL;
    }
}
?>
--EXPECTF--
Warning: ORNG\GLibCRand::arrayRand(): Array is empty in %s on line %d

Warning: ORNG\XorShift128Plus::arrayRand(): Array is empty in %s on line %d

Warning: ORNG\MT19937::arrayRand(): Array is empty in %s on line %d

Warning: ORNG\MT19937PHP::arrayRand(): Array is empty in %s on line %d

Warning: ORNG\MT19937MB::arrayRand(): Array is empty in %s on line %d