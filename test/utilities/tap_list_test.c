#define TAP_LOG_IMPLEMENTATION
#define TAP_ALLOC_IMPLEMENTATION

#include "util/tap_alloc.h"
#include "util/tap_def.h"
#include "util/tap_list.h"
#include "util/tap_log.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int top_speed;
    const char *brand;
    TapList wheels_anchor;
} Car;

typedef struct {
    int diameter;
    int width;
    TapList wheels_node;
} Wheel;

static Wheel pile_of_wheels[5] = {0};

static void tap_list_test(void)
{
    Car car = {0};
    TapList *iter = NULL;
    tap_list_init(&car.wheels_anchor);
    tap_list_insert(&car.wheels_anchor, &pile_of_wheels[0].wheels_node);
    tap_list_insert(&car.wheels_anchor, &pile_of_wheels[1].wheels_node);
    tap_list_insert(&car.wheels_anchor, &pile_of_wheels[2].wheels_node);
    tap_list_insert(&car.wheels_anchor, &pile_of_wheels[3].wheels_node);

    tap_list_for_each(&car.wheels_anchor, iter)
    {
        Wheel *wheel = tap_def_containerof(iter, Wheel, wheels_node);
        wheel->width = 42;
    }

    tap_list_for_each(&car.wheels_anchor, iter)
    {
        Wheel *wheel = tap_def_containerof(iter, Wheel, wheels_node);
        assert(wheel->width == 42);
    }

    {
        tap_list_insert(&car.wheels_anchor, &pile_of_wheels[4].wheels_node);
        Wheel *wheel = tap_def_containerof(car.wheels_anchor.next, Wheel, wheels_node);
        assert(wheel->width == 0);
        assert(pile_of_wheels[4].wheels_node.next != NULL);
        assert(pile_of_wheels[4].wheels_node.prev != NULL);
    }

    {
        tap_list_remove(car.wheels_anchor.next);
        Wheel *wheel = tap_def_containerof(car.wheels_anchor.next, Wheel, wheels_node);
        assert(wheel->width == 42);
        assert(pile_of_wheels[4].wheels_node.next == NULL);
        assert(pile_of_wheels[4].wheels_node.prev == NULL);
    }

    tap_list_clear(&car.wheels_anchor);
    assert(car.wheels_anchor.next == &car.wheels_anchor);
    assert(car.wheels_anchor.prev == &car.wheels_anchor);
}

int main(void)
{
    {
        TapLogLogger logger = {0};
        TapAllocAllocator allocator = {0};
        allocator.malloc = malloc;
        allocator.free = free;
        tap_alloc_set_allocator(allocator);
        logger.printf = printf;
        tap_log_set_logger(logger);
    }

    tap_list_test();
    return 0;
}
