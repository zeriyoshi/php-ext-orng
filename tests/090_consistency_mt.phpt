--TEST--
Check consistency for MT19937.
--SKIPIF--
<?php
if (!extension_loaded('orng')) {
    die('skip: extension not loaded.');
}
?>
--FILE--
<?php

foreach (include('data/tables/mt.inc') as $seed => $values) {
    $rng = new \ORNG\MT19937($seed);
    foreach ($values as $value) {
        $next = $rng->next();
        if ($value !== $next) {
            die("NG, MT19937 wrong result seed: ${seed} correct: ${value} result: ${next}");
        }
    }
}

die('OK, MT19937 results correctly.');
?>
--EXPECT--
OK, MT19937 results correctly.
