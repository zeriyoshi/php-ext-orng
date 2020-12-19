--TEST--
Check consistency for GLibCRand.
--SKIPIF--
<?php
if (! extension_loaded('orng')) {
    die('skip: extension not loaded.');
}
if (! file_exists('data/tables/rand.inc')) {
    die('skip: result table not found.');
}
?>
--FILE--
<?php

foreach (include('data/tables/rand.inc') as $seed => $values) {
    $rng = new \ORNG\GLibCRand($seed);
    foreach ($values as $value) {
        $next = $rng->next();
        if ($value !== $next) {
            die("NG, GLibCrand wrong result seed: ${seed} correct: ${value} result: ${next}");
        }
    }
}

die('OK, GLibCrand results correctly.');
?>
--EXPECT--
OK, GLibCrand results correctly.
