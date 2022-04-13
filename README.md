Random access noise algorithms & test programs
==============================================

This repository contains a copy of my PRNG algorithms, some others for comparison, and related things. It includes test programs, to generate streams of pseudo-random `int` data sent to `stdout` (for use with statistical tests, e.g. `dieharder` or `TestU01_stdin`), and other code of my own. Note that most testing is for _sequential_ use of a PRNG, while the whole point of my own PRNGs is to also allow _random access_ (similar to integer-to-integer hashing) with the simplest code that does the job well enough. Other PRNGs are usually better for sequential purposes.

The basic idea of the "ranoise" functions is to use a simple counter as the state for random number generation. Position can be changed with various positive or negative frequencies, and "jumps" in the stream of noise are trivial. Chaotic waveshaping replaces the current input number with the pseudo-random number it indexes. A sawtooth-ish signal is transformed into white noise. Given their simplicity, these functions can be a good alternative to storing and using arrays of white noise.

For each "ranoise" function, a function named with a `_next`-suffix is also provided; it instead behaves like a normal sequential access PRNG function for getting a next value, updating a passed state on each call. The state should be initialized before the first call to some value (any seed value is fine, including 0).

See the article "[Random access noise: Avalanche effect through chaotic waveshaping](https://joelkp.frama.io/blog/ran-chaos-waveshape.html)" for more on the development of all this. Currently, programs for the following versions are included:
 * `ranoise32` -- Minimal lower-quality version (compares well to 32-bit LCGs)
 * `ranoise32_old` -- Older medium-quality version (mostly obsoleted by `ranoise32b`)
 * `ranoise32a` -- Higher-quality version (compares well to SplitMix32)
 * `ranoise32b` -- Slightly different version, does better in PractRand testing
 * `ranoise32c` -- Does even better in PractRand, not as well in TestU01

There's also a header file for the stuff they and other variations on the same theme have in common, [muvaror32.h](include/muvaror32.h).

The C code files in this repository are licensed under the public domain-equivalent 0BSD license, meaning they can be copied from _without_ an attribution requirement (though credit is nice). The same applies to the code in this README.

The bare functions
------------------

While arguably uglier than using the macros or inline functions for bitrotation, etc., here's everything-in-one-go versions of the function definitions.

### My ranoise functions
For audio, there's no audible improvement when including the xor-and-rightshift steps which were added to improve avalanching and quality of bits below the highest, assuming the higher bits define most of the amplitude of the signal listened to in the usual way. The same may be true for graphics and what's visible. However, those looking to extract two rather than one samples from each output value (splitting each 32-bit sample into two 16-bit samples) need one of the highest-quality options.

#### `ranoise32`
Stripped-down version of `ranoise32a`, containing only the most important part of the algorithm.

Tested with an increasing counter as the argument, it fails some TestU01 SmallCrush tests and as such is comparable to both 32-bit LCGs and xorshift32. The failures are more like those of LCGs, but fewer; this is also the case in both PractRand and dieharder testing.
```
int32_t ranoise32(uint32_t x) {
        x *= 2654435769UL;
        x = (x | 1) * ((x >> (x >> 27)) | (x << (32-(x >> 27))));
        return x;
}
```
This is the function most likely to be useful for signal processing purposes, in situations where how it sounds or looks matters far more than statistical properties. The higher output bits are the best.

It's possible to add a bitrotation offset so as to produce one of 31 alternative outputs (easiest to do using the `MUVAROR32` macro used in the C file, by changing the last argument from 0 to another number).

#### `ranoise32_old`
This is an earlier version with some quirks and flaws, worth keeping for the distinctive smoothness of the output. It works well as long as changes to x between calls are small or have lower bits set.

Tested with an increasing counter as the argument, it fails 3 of TestU01's medium-sized Crush tests. In PractRand testing, like other low-quality PRNGs (32-bit LCGs, xorshift32, etc.), it fails at a 1 MB size, though only once.
```
int32_t ranoise32_old(uint32_t x) {
        x *= 2654435769UL;
        x *= (x >> (x + 14)) | (x << (32-(x + 14)));
        x ^= (x >> 7) ^ (x >> 16);
        return x;
}
```
If the last line before the `return` is removed, the result is a function with failure patterns in dieharder testing which are very similar to those of a 32-bit LCG. (With that last step included, dieharder testing however passes.)

#### `ranoise32a`
This is a reworked version which does very well in TestU01 testing for a PRNG with only 32 bits of state. (Many PRNGs with 64-bits or more of state can do better.) Tested with an increasing counter as the argument, it passes TestU01's medium-sized Crush tests, but still fails 5 BigCrush tests (mainly 4 "Gap" tests); though in `-r` mode, there's 3 Crush failures and 7 BigCrush failures. In PractRand, there's a failed test during the 2 GB stage, one round above how well SplitMix32 does.
```
int32_t ranoise32a(uint32_t x) {
        x *= 2654435769UL;
        x ^= x >> 14;
        x = (x | 1) * ((x >> (x >> 27)) | (x << (32-(x >> 27))));
        x ^= x >> 13;
        return x;
}
```

#### `ranoise32b`
Changing the bitshift lengths from 14 and 13 to 15 and 14 improves PractRand's evaluation (some failures appearing at 4 GB), but increases TestU01 test failures a little (one normal Crush failure, 10 `-r` BigCrush failures).
```
int32_t ranoise32b(uint32_t x) {
        x *= 2654435769UL;
        x ^= x >> 15;
        x = (x | 1) * ((x >> (x >> 27)) | (x << (32-(x >> 27))));
        x ^= x >> 14;
        return x;
}
```

#### `ranoise32c`
Instead adding the seemingly optimal bitrotation offset 16 mildens PractRand's suspicions until the decisive failures only happen during the 8 GB stage. TestU01 results however worsen almost in proportion, becoming more like those for SplitMix32. Further tweaking the bitshift lengths from 14 and 13 to 15 and 14 mildens PractRand's evaluation again, until suspicions are only expressed in all caps at 16 GB and a wall of failures suddenly appear at 32 GB. This version does even less well with TestU01, however; with 16 normal and 28 `-r` BigCrush failures, it's close to Mulberry32.
```
int32_t ranoise32c(uint32_t x) {
        uint32_t r;
        x *= 2654435769UL;
        x ^= x >> 15;
        r = (x >> 27) + 16;
        x = (x | 65537) * ((x >> r) | (x << (32-r)));
        x ^= x >> 14;
        return x;
}
```

Other interesting functions
---------------------------

Here's some other PRNGs that could very easily be used in place of ranoise32 functions. This also includes ease of random access use, or of changing a function for such.

### SplitMix32 variations
Variations of SplitMix32 are intuitively part of the main competition, given that SplitMix64 is a good PRNG which passes BigCrush. The 32-bit version of SplitMix does poorer in testing with both TestU01 (failing some Crush and more BigCrush tests) and PractRand, however. Higher-quality variations of ranoise32 do better in testing.

#### `splitmix32a`
A seemingly popular variation of SplitMix32 changes the first bitshift length from 16 to 15. It is included under the name `splitmix32a` in this repository, and fares slightly better than plain `splitmix32` in Crush and BigCrush tests (5 or 4 failures in Crush, 9 in BigCrush). In PractRand, this variation fails during the 1 GB stage, whereas plain `splitmix32` fails in the preceding 512 MB round.
```
uint32_t splitmix32a_next(uint32_t *pos) {
        uint32_t x = *pos += 2654435769UL;
        x ^= x >> 15;
        x *= 0x85ebca6b;
        x ^= x >> 13;
        x *= 0xc2b2ae35;
        x ^= x >> 16;
        return x;
}
```

#### `splitmix32b`
I also decided to include a `splitmix32b`, which fully replaces the 32-bit MurmurHash3 fmix function with another integer-to-integer hash function of the same form, the best currently found and provided by Christopher Wellons's ['hash-prospector' project](https://github.com/skeeto/hash-prospector). This was a little tricky and fiddly to get working well, because some failures at all test levels in TestU01 testing came and went haphazardly with variations. How well variations of this kind of function work _in testing as PRNGs_ seems more complicated than how strictly good they are as hash functions. The best result may also depend on finding a suitable increment constant peculiar to the function tested; the usual quick pick of the constant based on the golden ratio doesn't work well in SmallCrush testing for this function. On the whole, the below version is roughly as good as `splitmix32a` in both TestU01 and PractRand testing.
```
uint32_t splitmix32b_next(uint32_t *pos) {
        uint32_t x = *pos += 2452817881UL;
        x ^= x >> 15;
        x *= 0xd168aaad;
        x ^= x >> 15;
        x *= 0xaf723597;
        x ^= x >> 15;
        return x;
}
```

### Mulberry32
Mulberry32 is another PRNG with results comparable to SplitMix32, and included as `mulberry32`. In testing with TestU01 and PractRand it does roughly as well as `splitmix32a` above. (It does a little better in PractRand, but still has a failure during the 1 GB stage, and somewhat worse in TestU01, with 23 normal BigCrush failures and 16 in `-r` mode.)
```
uint32_t mulberry32_next(uint32_t *pos) {
        uint32_t z = *pos += 0x6D2B79F5UL;
        z = (z ^ (z >> 15)) * (z | 1UL);
        z ^= z + (z ^ (z >> 7)) * (z | 61UL);
        return z ^ (z >> 14);
}
```

Running statistical tests
-------------------------

If you install [`dieharder`](https://webhome.phy.duke.edu/~rgb/General/dieharder.php), then you can run a program named after a function from this repository through it as follows:

```
make && ./ranoise32 | dieharder -a -g 200
```

There is also a utility called [`TestU01_stdin`](https://framagit.org/-/snippets/6556) (my 2022 version with added options for bitreversal and verbosity) which, if built and installed after TestU01 is, allows similar testing with TestU01 (`-s` for SmallCrush, `-c` for Crush, `-b` for BigCrush, `-l` for LinearComp):

```
make && ./ranoise32 | TestU01_stdin -s
```

Or to write everything from a verbose TestU01 run (more than just the summary at the end), plus a final count of 32-bit samples generated, to a file:

```
make && ./ranoise32 | TestU01_stdin -sv 3>&1 2>&3 | tee file.txt
```

For [PractRand](http://pracrand.sourceforge.net/)'s `RNG_test` utility (which is far more critical than dieharder and way faster than TestU01 is beyond SmallCrush):

```
make && ./ranoise32 | RNG_test stdin32 -tlmin 1MB
```

Current results
---------------

Outputs for various tests with the unmodified current programs in this repository can be found under [results/](results/). PractRand (`practrand-*`) is good for quick comparisons. Full TestU01 results (`smallcrush-*`, `crush-*`, `bigcrush-*`) are rather verbose, but the short summary can be found at the end of each file. For a more detailed look at what goes wrong in lower-quality PRNGs, it can be interesting to look at dieharder results (`dieharder-*`).

The files for TestU01 tests named `*-rev.txt` use the `-r` option for `TestU01_stdin` to reverse the order of bits within each 32-bit word. When this results in more failures, it may point towards randomness in the lowest bits being of worse quality than that in higher bits. (TestU01 is less sensitive to flaws in the lowest bits.) However, when testing some PRNGs the number of BigCrush failures for both normal and `-r` testing varies haphazardly (and often in opposite directions), with small variations to the PRNG, so that looking at both results and considering the average may be more sensible.
