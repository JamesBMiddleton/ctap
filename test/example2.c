#include "test.h" 

i32 main(void)
{
    tst_setup();

    ctp_load_map();

    utl_powf(1, 1);
    utl_powi(1, 1);
    utl_powu(1, 1);

    char arr[100];
    utl_u32tostr(4294967295 / 2, arr);
    char arr2[100];
    utl_i32tostr(2147483647, arr2);
    char arr3[100];
    // utl_f32tostr(-1234.1000f, arr3, 1);
    utl_f32tostr(42.4f, arr3, 3);
    // utl_f32tostr(-0.0 / 0.0, arr3, 4);
    // utl_f32tostr(-1.0/0.0, arr3, 4);
    printf("%s\n", arr3);

    // sleep(10);

    ASSERT(1==1);

    // LOG_E("Hello %d %u %f", {.d=1}, {.u=42}, {.f=42.1f});
    LOG_E("Hello %d %u %f %s", {.d=1}, {.u=42}, {.f=42.1f}, {.s="hello"});
    // LOG_E("Hello %d", {.d=12});
    
    LOG_E("Helloo\n", 0);

    // ASSERT(1==4);

    return 0;
}
