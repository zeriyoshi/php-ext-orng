--TEST--
Check clone object.
--SKIPIF--
<?php
if (! extension_loaded('orng')) {
    echo 'skip';
}
?>
--FILE--
<?php

$classes = include('data/classes.inc');

foreach ($classes as $class) {
    $rng1 = new $class(1);
    $rng1->next();

    $rng2 = clone $rng1;

    $rng1_value = $rng1->next();
    $rng2_value = $rng2->next();

    if ($rng1_value !== $rng2_value) {
        die("NG, state is not cloned. class: ${class} RNG1 value: ${rng1_value} RNG2 value: ${rng2_value}");
    }
}
die('OK, state is cloned correctly.');
?>
--EXPECT--
OK, state is cloned correctly.
