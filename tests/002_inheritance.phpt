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

$classes = include('data/classes.inc');

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
