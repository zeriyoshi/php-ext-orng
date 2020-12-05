# Objective RNG extension for PHP

## Introduction

"orng" is a provides an object-scoped random number generator for PHP.

This is useful for reproducing situations such as poker games.

## Example

```php
<?php
$seed = 5656;

$rng = new \ORNG\XorShift128Plus($seed);
$rng2 = new \ORNG\XorShift128Plus($seed);

$rng_first = $rng->next();

echo "First generated number by \$rng: ${rng_first}\n";

foreach (range(1, 10) as $i) {
    echo "  ${i}: genrated by \$rng: {$rng->next()}\n";
}

$rng2_first = $rng2->next();
echo "First generated number by \$rng2: ${rng2_first}\n";

if ($rng_first === $rng2_first) {
    echo "object-scope confirmed.\n";
}

/*
First generated number by $rng: 8625142298246790
  1: genrated by $rng: 37180929041868370
  2: genrated by $rng: 4802484992883707356
  3: genrated by $rng: 3167033282580571667
  4: genrated by $rng: -1346064048936446922
  5: genrated by $rng: -4617783150383278012
  6: genrated by $rng: -2144402002071887622
  7: genrated by $rng: 5615789191153882182
  8: genrated by $rng: 5370589511783527133
  9: genrated by $rng: 5087160346683555124
  10: genrated by $rng: 7098617399520635796
First generated number by $rng2: 8625142298246790
object-scope confirmed.
*/
```
