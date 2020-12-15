# Objective RNG extension for PHP
![CI](https://github.com/zeriyoshi/php-ext-orng/workflows/CI/badge.svg)

requires: PHP >= 7.3

## Introduction

"orng" is a provides an object-scoped random number generator for PHP.

This is useful for reproducing situations such as poker games.

## Reference

All classes implemented [`\ORNG\RNGInterface`](rng/rnginterface.stub.php).

- `\ORNG\XorShift128Plus`: [stub](rng/xorshift128plus.stub.php)
- `\ORNG\GLibCRand`: [stub](rng/glibcrand.stub.php)
- `\ORNG\MT19937`: [stub](rng/mt19937.stub.php)
- `\ORNG\MT19937PHP`: [stub](rng/mt19937php.stub.php)
- `\ORNG\MT19937MB`: [stub](rng/mt19937mb.stub.php)

+1, Some classes implemented `\Serializable`. they can serialize / unserialize RNG state.

- `\ORNG\XorShift128Plus`: [stub](rng/xorshift128plus.stub.php)
- `\ORNG\GLibCRand`: [stub](rng/glibcrand.stub.php)

## Implemented RNG's

- `\ORNG\XorShift128Plus`: Fast and smart PRNG.
- `\ORNG\GLibCRand`: Compatible for GNU libc based PHP 7.0 and earlier `srand()` and `rand()` functions.
- `\ORNG\MT19937`: Genuine implementation of Mersenne Twister from php-src.
- `\ORNG\MT19937PHP`: PHP specific incorrect Mersenne Twister implementation from php-src.
- `\ORNG\MT19937MB`: Genuine implementation with PHP 7.1's modulo-bias bug.

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
First generated number by $rng: 1656077336
  1: genrated by $rng: 840142771
  2: genrated by $rng: 1623151200
  3: genrated by $rng: 353778187
  4: genrated by $rng: 366931415
  5: genrated by $rng: 357580052
  6: genrated by $rng: 623439083
  7: genrated by $rng: 471473064
  8: genrated by $rng: 1854728523
  9: genrated by $rng: 1999487937
  10: genrated by $rng: 315225512
First generated number by $rng2: 1656077336
object-scope confirmed.
*/
```
