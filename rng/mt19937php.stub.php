<?php

/** @generate-function-entries */

namespace ORNG;

class MT19937PHP implements RNGInterface
{
    public function __construct(int $seed) {}
    public function next(): int {}
    public function range(int $min, int $max): int {}
}
