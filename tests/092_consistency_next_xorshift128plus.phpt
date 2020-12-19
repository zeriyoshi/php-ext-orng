--TEST--
Check next and next64. (XorShift128Plus)
--SKIPIF--
<?php
if (! extension_loaded('orng')) {
    echo 'skip';
}
if (! file_exists('data/tables/xorshift128plus.tsv')) {
    die('skip: result table not found.');
}
?>
--FILE--
<?php

$f = fopen('data/tables/xorshift128plus.tsv', 'r');

$method = 'next';
while (($buf = fgets($f)) !== false) {
    $buf = trim($buf);
    if ($buf === "#32") {
        $method = 'next';
        continue;
    } else if ($buf === "#64") {
        $method = 'next64';
        continue;
    }

    $table = explode("\t", $buf);
    $seed = array_shift($table);
    $rng = new \ORNG\XorShift128Plus((int) $seed);
    foreach ($table as $value) {
        if ($rng->$method() !== (int) $value) {
            die("NG, invalid result in method ${method}.");
        }
    }
}
die('OK, all results corrected.')
?>
--EXPECT--
OK, all results corrected.