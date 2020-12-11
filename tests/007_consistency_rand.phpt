--TEST--
Check consistency for GLibCRand.
--SKIPIF--
<?php
if (!extension_loaded('orng')) {
    die('skip: extension not loaded.');
}
?>
--FILE--
<?php

foreach (include('data/rand.inc') as $seed => $values) {
    $rng = new \ORNG\GLibCRand($seed);
    foreach ($values as $value) {
        $next = $rng->next();
        if ($value !== $next) {
            die("NG, GLibCrand wrong result seed: ${seed} correct: ${value} result: ${next}");
        }
    }
}

die('OK, GLibCrand results correctly.');
--EXPECT--
OK, GLibCrand results correctly.
