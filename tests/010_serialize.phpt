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
    $e = explode('\\', $class);
    $define_class = 'X' . end($e);
    eval("class ${define_class} extends ${class} { public \$foo; public \$bar; public \$baz; }");
    $t = new $define_class(SEED);
    $t->foo = 'bar';
    $t->bar = 1234;
    $t->baz = [1, 2, 3, 4];
    $t->next();

    if (! $t instanceof \Serializable) {
        continue;
    }

    $s = serialize($t);
    $t_next = $t->next();
    $t_next64 = $t->next64();
    $ut = unserialize($s);
    $ut_next = $ut->next();
    $ut_next64 = $ut->next64();
    if ($ut_next !== $t_next) {
        die("NG, broken detected. class: ${class} method: next() correct: ${t_next} result: ${ut_next}");
    }
    if ($ut_next64 !== $t_next64) {
        die("NG, broken detected. class: ${class} method: next64() correct: ${t_next64} result: ${ut_next64}");
    }
    if ($ut->foo !== 'bar' ||
        $ut->bar !== 1234 ||
        $ut->baz[0] !== 1 ||
        $ut->baz[1] !== 2 ||
        $ut->baz[2] !== 3 ||
        $ut->baz[3] !== 4
    ) {
        die('NG, broken detected in properties: ' . print_r($ut, true));
    }
}
die('OK, serialize / unserialize works correctly.');
?>
--EXPECT--
OK, serialize / unserialize works correctly.