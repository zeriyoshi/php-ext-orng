--TEST--
Check pollution another objects.
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
    $origin = new $class(SEED);
    $children = [];
    foreach (range(1, 100) as $i) {
        $children[] = new $class(SEED);
    }
    foreach (range(1, 1000) as $i) {
        $next = $origin->next();
        foreach ($children as $child) {
            if ($child->next() !== $next) {
                die("NG, polltion detected in ${class} (count: ${i})");
            }
        }
    }
    foreach (range(1, 1000) as $i) {
        $next64 = $origin->next64();
        foreach ($children as $child) {
            if ($child->next64() !== $next64) {
                die("NG, polltion detected (64) in ${class} (count: ${i})");
            }
        }
    }
}
die('OK, pollution is NOT detected.');
?>
--EXPECT--
OK, pollution is NOT detected.
