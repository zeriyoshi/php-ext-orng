# Objective RNG extension for PHP
![CI](https://github.com/zeriyoshi/php-ext-orng/workflows/CI/badge.svg)

requires: PHP >= 7.3

## Introduction

"orng" is a provides an object-scoped random number generator for PHP.

This is useful for reproducing situations such as poker games.

## Reference

All classes implemented `\ORNG\RNGInterface`. it's so simple.

```php
<?php

interface \ORNG\RNGInterface
{
  /**
   * Initialize RNG class with seed number.
   */
  public function __construct(int $seed);

  /**
   * Generate next random number.
   */
  public function next(): int;

  /**
   * Generate random number in range.
   */
  public function range(int $min, int $max): int;

  /**
   * Shuffle an array.
   * 
   * @see https://www.php.net/manual/en/function.shuffle.php
   */
  public function shuffle(array &$array): bool;

  /**
   * Pick one or more random keys out of an array.
   * 
   * @see https://www.php.net/manual/en/function.array-rand
   */
  public function arrayRand(array $array, int $num = 1): int|string|array;

  /**
   * Randomly shuffles a string.
   * 
   * @see https://www.php.net/manual/en/function.str-shuffle
   */
  public function strShuffle(string $string): string;
}
```

## Implemented RNG's

- `\ORNG\XorShift128Plus`
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
First generated number by $rng: 9158675354017783308
  1: genrated by $rng: 1441971726010927065
  2: genrated by $rng: 7969781667560663344
  3: genrated by $rng: 7634556340589501701
  4: genrated by $rng: 6091974329639073515
  5: genrated by $rng: 2182242786293063306
  6: genrated by $rng: 6432679252689450613
  7: genrated by $rng: 6376292334233783764
  8: genrated by $rng: 7389635648749466789
  9: genrated by $rng: 1149666737513751008
  10: genrated by $rng: 705725663761726164
First generated number by $rng2: 9158675354017783308
object-scope confirmed.
*/
```
