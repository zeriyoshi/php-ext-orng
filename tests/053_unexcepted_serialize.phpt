--TEST--
Check serialize() and unserialize().
--SKIPIF--
<?php
if (! extension_loaded('orng')) {
    echo 'skip';
}
?>
--FILE--
<?php

const SEED = 1234;

foreach (include(__DIR__ . '/data/classes.inc') as $class) {
    $t = new $class(SEED);

    if (! $t instanceof \Serializable) {
        continue;
    }

    $s = serialize($t);
    try {
        $ut = unserialize(str_replace('r:', 'b:', $s));
    } catch (\Throwable $e) {
        continue;
    }
    die('NG, No exceptions were detected.');
}
die("OK, All exceptions correctly.");
?>
--EXPECT--
OK, All exceptions correctly.