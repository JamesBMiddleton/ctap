#include "util/tap_list.h"
#include "util/tap_def.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

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

static Wheel pile_of_wheels[8] = {0};

static void tap_list_init_test(void)
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
        printf("wheel width = %d\n", wheel->width);
    }
}

int main(void)
{
    tap_list_init_test();
    return 0;
}
