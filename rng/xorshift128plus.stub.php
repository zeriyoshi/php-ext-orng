<?php

/** @generate-function-entries */

namespace ORNG;

class XorShift128Plus implements RNGInterface
{
    public function __construct(int $seed) {}
    public function next(): int {}
    public function range(int $min, int $max): int {}
    public function shuffle(array &$array): bool {}
}
