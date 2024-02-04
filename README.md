# PCG Random Extension for Defold

This extension wraps PCG, A Family of Better Random Number Generators. C++ implementation v0.98.

Official website https://www.pcg-random.org

Official documentation https://www.pcg-random.org/using-pcg-cpp.html

# The PCG Family
The PCG family combines properties not previously seen together in the same generation scheme:
- It's really easy to use, and yet its very flexible and offers powerful features.
- It's very fast, and can occupy very little space.
- It's performance in statistical tests is excellent (see the PCG paper for full details).
- It's much less predictable and thus more secure than most generators.

The PCG family offers various generators with different properties. There are 32-bit and 64-bit generators, some with huge periods, some with streams support, some take their own memory address as an addition to the seed, some can be initialized with extended data.

# Setup

Paste this URL as a dependency in your `game.project` file.

```
https://github.com/Lerg/extension-pcgrandom/archive/master.zip
```

# API reference

### pcgrandom.new_rng(`pcg_type`: _number_, `seed`: _number_, `[stream]`: _number_) -> `rng`: _lightuserdata_

Creates a new specified random number generator with a given seed and an optional stream for generators that support it.

- `pcg_type`: _number_ Generator type id.
- `seed`: _number_ Seed number.
- `stream`: _number_ Stream number.
- return: `rng`: _lightuserdata_ RNG instance.

---
### pcgrandom.new_rng_array(`pcg_type`: _number_, `array`: _table_, `seed`: _number_, `[stream]`: _number_) -> `rng`: _lightuserdata_

Creates a new specified extended random number generator with a given array for the extended data, seed and an optional stream for generators that support it.

- `pcg_type`: _number_ Generator type id.
- `array`: _table_ Array with UINT32 numbers for the extended data, the size of the array must match the generator type.
- `seed`: _number_ Seed number.
- `stream`: _number_ Stream number.
- return: `rng`: _lightuserdata_ RNG instance.

---
### pcgrandom.next(`rng`: _lightuserdata_) -> `value`: _number_

Generates a pseudorandom integer number.

- `rng`: _lightuserdata_ RNG instance.
- return: `value`: _number_ Random value.

---
### pcgrandom.bound(`rng`: _lightuserdata_, `upper_bound`: _number_) -> `value`: _number_

Generates a pseudorandom integer number on [0,upper_bound) interval.

- `rng`: _lightuserdata_ RNG instance.
- `upper_bound`: _number_ Upper bound.
- return: `value`: _number_ Random value.

---
### pcgrandom.range(`rng`: _lightuserdata_, `lower_bound`: _number_, `upper_bound`: _number_) -> `value`: _number_

Generates a pseudorandom integer number on [lower_bound,upper_bound) interval.

- `rng`: _lightuserdata_ RNG instance.
- `lower_bound`: _number_ Lower bound.
- `upper_bound`: _number_ Upper bound.
- return: `value`: _number_ Random value.

---
### pcgrandom.advance(`rng`: _lightuserdata_, `delta`: _number_)

Advances the generator forward delta steps, but does so in logarithmic time. (Not available for non-equidistributed extended generators.)

- `rng`: _lightuserdata_ RNG instance.
- `delta`: _number_ Delta.

---
### pcgrandom.backstep(`rng`: _lightuserdata_, `delta`: _number_)

Moves the generator backwards delta steps, but does so in logarithmic time. (Not available for non-equidistributed extended generators.)

- `rng`: _lightuserdata_ RNG instance.
- `delta`: _number_ Delta.

---
### pcgrandom.max(`rng`: _lightuserdata_) -> `max`: _number_

Returns the greatest possible random value from the generator.

- `rng`: _lightuserdata_ RNG instance.
- return: `max`: _number_ Max.

---
### pcgrandom.period_pow2(`rng`: _lightuserdata_) -> `period`: _number_

Returns the period of the generator as a power of two. For PCG32, it returns 64, indicating a period of 2^64.

- `rng`: _lightuserdata_ RNG instance.
- return: `period`: _number_ Period.

---
### pcgrandom.streams_pow2(`rng`: _lightuserdata_) -> `streams`: _number_

Returns the number of streams as a power of two. For PCG32, it returns 63, indicating 2^63 streams.

- `rng`: _lightuserdata_ RNG instance.
- return: `streams`: _number_ Streams.

---
### pcgrandom.wrapped(`rng`: _lightuserdata_) -> `wrapped`: _boolean_

Tells you when the generator wraps around to the beginning (which we define as the zero state).

- `rng`: _lightuserdata_ RNG instance.
- return: `wrapped`: _boolean_ Wrapped.

---
### pcgrandom.next_uint64(`rng`: _lightuserdata_) -> `value`: _string_

Generates a UINT64 pseudorandom number when used with a 64-bit generator.

- `rng`: _lightuserdata_ RNG instance.
- return: `value`: _string_ UINT64 random value represented as an 8 character long string - memory representation of a UINT64 number.

---
### pcgrandom.next_real1(`rng`: _lightuserdata_) -> `value`: _number_

Generates a random number on [0,1]-real-interval.

- `rng`: _lightuserdata_ RNG instance.
- return: `value`: _number_ Random value.

---
### pcgrandom.next_real2(`rng`: _lightuserdata_) -> `value`: _number_

Generates a random number on [0,1)-real-interval.

- `rng`: _lightuserdata_ RNG instance.
- return: `value`: _number_ Random value.

---
### pcgrandom.genrand_res53(`rng`: _lightuserdata_) -> `value`: _number_

Generates a random number on [0,1)-real-interval with 53-bit resolution using a 64-bit integer when used with a 64-bit generator.

- `rng`: _lightuserdata_ RNG instance.
- return: `value`: _number_ Random value.

---
### pcgrandom.genrand_res53_mix(`rng`: _lightuserdata_) -> `value`: _number_

Generates a random number on [0,1)-real-interval with 53-bit resolution using two 32bit integers.

- `rng`: _lightuserdata_ RNG instance.
- return: `value`: _number_ Random value.

---
### pcgrandom.d4(`rng`: _lightuserdata_) -> `value`: _number_

Generates a random integer on [1,4] interval like from a dice.

- `rng`: _lightuserdata_ RNG instance.
- return: `value`: _number_ Random value.

---
### pcgrandom.d6(`rng`: _lightuserdata_) -> `value`: _number_

Generates a random integer on [1,6] interval like from a dice.

- `rng`: _lightuserdata_ RNG instance.
- return: `value`: _number_ Random value.

---
### pcgrandom.d8(`rng`: _lightuserdata_) -> `value`: _number_

Generates a random integer on [1,8] interval like from a dice.

- `rng`: _lightuserdata_ RNG instance.
- return: `value`: _number_ Random value.

---
### pcgrandom.d10(`rng`: _lightuserdata_) -> `value`: _number_

Generates a random integer on [1,10] interval like from a dice.

- `rng`: _lightuserdata_ RNG instance.
- return: `value`: _number_ Random value.

---
### pcgrandom.d12(`rng`: _lightuserdata_) -> `value`: _number_

Generates a random integer on [1,12] interval like from a dice.

- `rng`: _lightuserdata_ RNG instance.
- return: `value`: _number_ Random value.

---
### pcgrandom.d20(`rng`: _lightuserdata_) -> `value`: _number_

Generates a random integer on [1,20] interval like from a dice.

- `rng`: _lightuserdata_ RNG instance.
- return: `value`: _number_ Random value.

---
### pcgrandom.toss(`rng`: _lightuserdata_) -> `value`: _number_

Generates a random integer on [0,1] interval like from a toss of a coin.

- `rng`: _lightuserdata_ RNG instance.
- return: `value`: _number_ Random value.

---
### pcgrandom.card(`rng`: _lightuserdata_) -> `value`: _number_

Generates a random integer on [1,52] interval like picking a random card from a standard deck.

- `rng`: _lightuserdata_ RNG instance.
- return: `value`: _number_ Random value.

---
### pcgrandom.card2(`rng`: _lightuserdata_) -> `suit`: _number_, `card`: _number_

Generates two random integers on [1,4] and [1,13] intervals like picking a random card suit and a random card from a suit.

- `rng`: _lightuserdata_ RNG instance.
- return: `suit`: _number_ Random suit [1,4], `card`: _number_ Random card [1,13].

---
### pcgrandom.shuffle(`rng`: _lightuserdata_, `array`: _table_)

Shuffles the provided array in place.

- `rng`: _lightuserdata_ RNG instance.
- `array`: _table_ The array of values.

---
### pcgrandom.delete(`rng`: _lightuserdata_)

Deletes the random generator and frees the alocated memory for it.

- `rng`: _lightuserdata_ RNG instance.

---
### Available generators

The number after `PCG` specifies the bit size of the generator (8, 16, 32, 64). It is possible to use 64-bit generators but Lua in Defold doesn't fully support such numbers.

The `K` variants are k-dimensionally equidistributed. The `C` variants offer better crypographic security, uniform but not equidistributed, harder to predict. The number after `K` or `C` specifies the extended data length, the higher the number the higher is the period of the generator.

`ONESEQ` variants offer a single stream.

`UNIQUE` variants have an extra seed based on memory address.

`FAST` variants are faster but slightly less statistically good output function (nevertheless still very good).

`ONCE_INSECURE` variants only output each number exactly once during their period. They are compact, and are statistically excellent, but should not be used except in specialized scenarios.

- `pcgrandom.PCG32`
- `pcgrandom.PCG32_ONESEQ`
- `pcgrandom.PCG32_UNIQUE`
- `pcgrandom.PCG32_FAST`
- `pcgrandom.PCG64`
- `pcgrandom.PCG64_ONESEQ`
- `pcgrandom.PCG64_UNIQUE`
- `pcgrandom.PCG64_FAST`
- `pcgrandom.PCG8_ONCE_INSECURE`
- `pcgrandom.PCG16_ONCE_INSECURE`
- `pcgrandom.PCG32_ONCE_INSECURE`
- `pcgrandom.PCG64_ONCE_INSECURE`
- `pcgrandom.PCG8_ONESEQ_ONCE_INSECURE`
- `pcgrandom.PCG16_ONESEQ_ONCE_INSECURE`
- `pcgrandom.PCG32_ONESEQ_ONCE_INSECURE`
- `pcgrandom.PCG64_ONESEQ_ONCE_INSECURE`
- `pcgrandom.PCG32_K2`
- `pcgrandom.PCG32_K2_FAST`
- `pcgrandom.PCG32_K64`
- `pcgrandom.PCG32_K64_ONESEQ`
- `pcgrandom.PCG32_K64_FAST`
- `pcgrandom.PCG32_C64`
- `pcgrandom.PCG32_C64_ONESEQ`
- `pcgrandom.PCG32_C64_FAST`
- `pcgrandom.PCG32_K1024`
- `pcgrandom.PCG32_K1024_FAST`
- `pcgrandom.PCG32_C1024`
- `pcgrandom.PCG32_C1024_FAST`
- `pcgrandom.PCG32_K16384`
- `pcgrandom.PCG32_K16384_FAST`
- `pcgrandom.PCG64_K32`
- `pcgrandom.PCG64_K32_ONESEQ`
- `pcgrandom.PCG64_K32_FAST`
- `pcgrandom.PCG64_C32`
- `pcgrandom.PCG64_C32_ONESEQ`
- `pcgrandom.PCG64_C32_FAST`
- `pcgrandom.PCG64_K1024`
- `pcgrandom.PCG64_K1024_FAST`
- `pcgrandom.PCG64_C1024`
- `pcgrandom.PCG64_C1024_FAST`

### Example

```language-lua
local rng = pcgrandom.new_rng(pcgrandom.PCG32, socket.gettime())

print('uint32', pcgrandom.next(rng))
print('real2', pcgrandom.next_real2(rng))
print('d6', pcgrandom.d6(rng))

pcgrandom.delete(rng)
rng = nil
```
