#include <assert.h>
#include "q.h"
#include "car.h"

Q_GENERATE_FOR_TYPE(unsigned int, uint)

int main(void) {
    quint nums;
    unsigned int val;

    quint_init(&nums, 2);
    assert(nums.length == 0); assert(nums.capacity == 2);
    quint_enqueue(&nums, 111u);
    assert(nums.length == 1); assert(nums.capacity == 2);
    quint_enqueue(&nums, 222u);
    assert(nums.length == 2); assert(nums.capacity == 2);
    quint_enqueue(&nums, 333u);
    assert(nums.length == 3); assert(nums.capacity == 4);
    assert(quint_try_dequeue(&nums, &val)); assert(val == 111u);
    assert(nums.length == 2); assert(nums.capacity == 4);
    assert(quint_try_dequeue(&nums, &val)); assert(val == 222u);
    assert(nums.length == 1); assert(nums.capacity == 4);
    quint_enqueue(&nums, 444u);
    quint_enqueue(&nums, 555u);
    quint_enqueue(&nums, 666u);
    assert(nums.length == 4); assert(nums.capacity == 4);
    quint_enqueue(&nums, 777u);
    quint_enqueue(&nums, 888u);
    assert(nums.length == 6); assert(nums.capacity == 8);
    assert(quint_try_dequeue(&nums, &val)); assert(val == 333u);
    assert(nums.length == 5); assert(nums.capacity == 8);
    quint_enqueue(&nums, 999u);
    quint_enqueue(&nums, 1111u);
    quint_enqueue(&nums, 2222u);
    assert(nums.length == 8); assert(nums.capacity == 8);
    quint_enqueue(&nums, 3333u);
    assert(nums.length == 9); assert(nums.capacity == 16);
    quint_enqueue(&nums, 4444u);
    assert(nums.length == 10); assert(nums.capacity == 16);
    assert(quint_try_dequeue(&nums, &val)); assert(val == 444u);
    assert(quint_try_dequeue(&nums, &val)); assert(val == 555u);
    assert(quint_try_dequeue(&nums, &val)); assert(val == 666u);
    assert(quint_try_dequeue(&nums, &val)); assert(val == 777u);
    assert(quint_try_dequeue(&nums, &val)); assert(val == 888u);
    assert(quint_try_dequeue(&nums, &val)); assert(val == 999u);
    assert(nums.length == 4); assert(nums.capacity == 16);
    quint_enqueue(&nums, 5555u);
    assert(nums.length == 5); assert(nums.capacity == 16);
    assert(quint_try_dequeue(&nums, &val)); assert(val == 1111u);
    assert(quint_try_dequeue(&nums, &val)); assert(val == 2222u);
    assert(nums.length == 3); assert(nums.capacity == 16);
    quint_clear(&nums);
    assert(nums.length == 0); assert(nums.capacity == 16);
    assert(!quint_try_dequeue(&nums, &val)); assert(val == 2222u);
    quint_enqueue(&nums, 6666u);
    quint_enqueue(&nums, 7777u);
    assert(quint_try_dequeue(&nums, &val)); assert(val == 6666u);
    assert(quint_try_dequeue(&nums, &val)); assert(val == 7777u);
    assert(!quint_try_dequeue(&nums, &val)); assert(val == 7777u);
    assert(nums.length == 0); assert(nums.capacity == 16);
    quint_clear(&nums);
    assert(nums.length == 0); assert(nums.capacity == 16);
    assert(!quint_try_dequeue(&nums, &val)); assert(val == 7777u);
    quint_enqueue(&nums, 8888u);
    assert(quint_try_dequeue(&nums, &val)); assert(val == 8888u);
    assert(nums.length == 0); assert(nums.capacity == 16);

    qcar cars;
    car getaway_car;
    qcar_init(&cars, 32);
    qcar_enqueue(&cars, (car){2000, 1000});
    qcar_enqueue(&cars, (car){2001, 1001});
    assert(cars.length == 2); assert(cars.capacity == 32);
    assert(qcar_try_dequeue(&cars, &getaway_car));
    assert(getaway_car.year == 2000);
    assert(getaway_car.mass_kg == 1000);

    quint_deinit(&nums);
    qcar_deinit(&cars);
    return 0;
}