<?php

/** @generate-function-entries */

namespace ORNG;

interface RNGInterface
{
    public function __construct(int $seed);
    public function next(): int;
    public function range(int $min, int $max): int;
}

class GLibCRand implements RNGInterface
{
    public function __construct(int $seed) {}
    public function next(): int {}
    public function range(int $min, int $max): int {}
}

class XorShift128Plus implements RNGInterface
{
    public function __construct(int $seed) {}
    public function next(): int {}
    public function range(int $min, int $max): int {}
}

class MT19937 implements RNGInterface
{
    public function __construct(int $seed) {}
    public function next(): int {}
    public function range(int $min, int $max): int {}
}

class MT19937PHP implements RNGInterface
{
    public function __construct(int $seed) {}
    public function next(): int {}
    public function range(int $min, int $max): int {}
}
