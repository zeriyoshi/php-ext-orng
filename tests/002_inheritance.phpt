--TEST--
Check inheritance classes.
--SKIPIF--
<?php
if (!extension_loaded('orng')) {
    echo 'skip';
}
?>
--FILE--
<?php
const SEED = 50;

$classes = [
    \ORNG\GLibCRand::class,
    \ORNG\XorShift128Plus::class,
    \ORNG\MT19937::class,
    \ORNG\MT19937PHP::class,
];

foreach ($classes as $class) {
    $object = new $class(SEED);
    if (! $object instanceof \Orng\RNGInterface) {
        die("NG, ${class} has not implemented \\ORNG\\RNGInterface.");
    }
}
die('OK, inheritance is corrected.');
?>
--EXPECT--
OK, inheritance is corrected.
