--TEST--
Check consistency for MT19937PHP.
--SKIPIF--
<?php
if (!extension_loaded('orng')) {
    die('skip: extension not loaded.');
}
?>
--FILE--
<?php

foreach (include('data/tables/mt_php.inc') as $seed => $values) {
    $rng = new \ORNG\MT19937PHP($seed);
    foreach ($values as $value) {
        $next = $rng->next();
        if ($value !== $next) {
            die("NG, MT19937PHP wrong result seed: ${seed} correct: ${value} result: ${next}");
        }
    }
}

die('OK, MT19937PHP results correctly.');
?>
--EXPECT--
OK, MT19937PHP results correctly.
