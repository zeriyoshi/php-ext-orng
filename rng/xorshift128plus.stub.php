<?php

/** @generate-function-entries */

namespace ORNG;

class XorShift128Plus implements RNGInterface
{
    public function __construct(int $seed) {}
    public function next(): int {}
    public function range(int $min, int $max): int {}
    public function shuffle(array &$array): bool {}
    public function arrayRand(array $array, int $num): mixed {}
    /* public function arrayRand(array $array, int $num = 1): int|string|array {} */
    public function strShuffle(string $string): string {}
}
