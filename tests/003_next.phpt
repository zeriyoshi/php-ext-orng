--TEST--
Check next and next64.
--SKIPIF--
<?php
if (! extension_loaded('orng')) {
    echo 'skip';
}
?>
--FILE--
<?php
const SEED = 50;

$classes = include('data/classes.inc');

foreach ($classes as $class) {
    $origin = new $class(SEED);
    $children = [];
    foreach (range(1, 100) as $i) {
        $children[] = new $class(SEED);
    }
    foreach (range(1, 1000) as $i) {
        $next = $origin->next();
        foreach ($children as $child) {
            if ($child->next() !== $next) {
                die("NG, invalid result in ${class} (count: ${i})");
            }
        }
    }
    foreach (range(1, 1000) as $i) {
        $next64 = $origin->next64();
        foreach ($children as $child) {
            if ($child->next64() !== $next64) {
                die("NG, invalid result (64) in ${class} (count: ${i})");
            }
        }
    }
}
die('OK, all results corrected.');
?>
--EXPECT--
OK, all results corrected.
