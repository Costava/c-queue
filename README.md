# C Queue

- Generic, type-safe queue using a circular buffer
- Uses macros to generate a struct (where queue state is managed) and functions to act on that struct for any given type
- The internal buffer is automatically resized (2x) when necessary to fit additional items

Dependencies:

- C99 standard library

## Usage

```C
#include "q.h"

// Generate the code for the type `unsigned int`
// "uint" is suffixed to "q" in the function names
Q_GENERATE_FOR_TYPE(unsigned int, uint)

int main(void) {
    quint nums;
    unsigned int val;

    // Initialize with capacity of 64
    quint_init(&nums, 64);
    quint_enqueue(&nums, 111u);
    quint_enqueue(&nums, 222u);
    quint_enqueue(&nums, 333u);
    quint_try_dequeue(&nums, &val); // Returned true and val is 111u
    quint_try_dequeue(&nums, &val); // Returned true and val is 222u
    quint_try_dequeue(&nums, &val); // Returned true and val is 333u
    quint_try_dequeue(&nums, &val); // Returned false and val remains 333u
    quint_enqueue(&nums, 444u);
    quint_clear(&nums);
    quint_try_dequeue(&nums, &val); // Returned false and val remains 333u

    quint_deinit(&nums);
    return 0;
}
```

`Q_GENERATE_FOR_TYPE` is a convenience macro that calls both
`Q_GENERATE_HEADER_CODE` and `Q_GENERATE_IMPLEMENTATION_CODE`.  
Alternatively, these two macros can be called separately
as seen in `test/car.h` and `test/car.c` respectively.

## License
BSD 2-Clause License. See file `LICENSE.txt`.
