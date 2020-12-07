--TEST--
Check range method.
--SKIPIF--
<?php
if (!extension_loaded('orng')) {
    echo 'skip';
}
?>
--FILE--
<?php

const EXCEPT_MIN = 10;
const EXCEPT_MAX = 20;

$classes = [
    \ORNG\GLibCRand::class,
    \ORNG\XorShift128Plus::class,
    \ORNG\MT19937::class,
    \ORNG\MT19937PHP::class,
];
$excepts = range(EXCEPT_MIN, EXCEPT_MAX);

foreach ($classes as $class) {
    foreach (range(1, 10000) as $seed) {
        $value = (new $class($seed))->range(EXCEPT_MIN, EXCEPT_MAX);
        if (! in_array($value, $excepts, true)) {
            die("NG, class: ${class} seed: ${seed} value: ${value}");
        }
    }
}
die('OK, range is working correctly.');
--EXPECT--
OK, range is working correctly.
