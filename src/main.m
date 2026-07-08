#define SOKOL_IMPL
#define SOKOL_METAL

#define TAP_ERRNO_IMPLEMENTATION
#define TAP_ALLOC_IMPLEMENTATION
#define TAP_LOG_IMPLEMENTATION

#include "dep/sokol_app.h"
#include "dep/sokol_gfx.h"
#include "dep/sokol_gl.h"
#include "dep/sokol_glue.h"
#include "dep/sokol_log.h"
#include "tap_api.h"
#include "util/tap_errno.h"
#include <stdio.h>
#include <stdlib.h>

#define VIEWPORT_WIDTH 800UL
#define VIEWPORT_HEIGHT 800UL

static sg_image vram_framebuffer = {0}; //NOLINT
static sg_view vram_framebuffer_view = {0}; //NOLINT
static sg_sampler smp = {0}; //NOLINT

// Our accumulated keyboard & mouse events.
static TapApiInputEvents in_events = {0}; // NOLINT

static void init(void)
{
    // Setup sokol_gfx.
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
        .logger.func = slog_func,
    });

    // Setup sokol_gl - an openGL wrapper around the sokol_gfx API.
    sgl_setup(&(sgl_desc_t){.logger.func = slog_func});

    // Allocate memory for an image texture in vram via the sokol_gfx API.
    vram_framebuffer = sg_make_image(&(sg_image_desc){
        .width = VIEWPORT_WIDTH,
        .height = VIEWPORT_HEIGHT,
        .usage.stream_update = true,
    });

    // Make a view handle for the vram_framebuffer for use with sgl_ functions.
    vram_framebuffer_view = sg_make_view(&(sg_view_desc){.texture.image = vram_framebuffer});

    // Default texture sampler - e.g. whether to repeat texture, mipmap levels
    smp = sg_make_sampler(&(sg_sampler_desc){0});

    if (tap_api_init((TapApiInitOpt){ .viewport_width = VIEWPORT_WIDTH, .viewport_height = VIEWPORT_HEIGHT,
                .allocator.malloc = malloc, .allocator.free = free, .logger.printf = printf}).retcode != TAP_RESULT_OK)
    {
        printf("Failed with TAP_ERRNO: %u, source: %s\n", tap_errno_get(), tap_errno_source_get());
        exit(1);
    }

    sapp_lock_mouse(true);
}

static void frame(void)
{
    // CPU render onto our RAM allocated framebuffer.
    const TapApiFramebuffer *out_framebuffer = NULL;
    const size_t out_framebuffer_sz = (VIEWPORT_WIDTH * VIEWPORT_HEIGHT * sizeof(TapApiFramebuffer));
    if (tap_api_cycle(&in_events, &out_framebuffer).retcode != TAP_RESULT_OK)
    {
        printf("Failed with TAP_ERRNO: %u, source: %s\n", tap_errno_get(), tap_errno_source_get());
        exit(1);
    }

    // Copy our RAM allocated framebuffer into our VRAM allocated image texture
    sg_update_image(vram_framebuffer, &(sg_image_data){.mip_levels[0] = {.ptr = out_framebuffer, .size = out_framebuffer_sz}});

    // overlay our backbuffer-sized image texture across the actual backbuffer
    sgl_defaults();
    sgl_load_default_pipeline();
    sgl_enable_texture();
    sgl_texture(vram_framebuffer_view, smp);
    sgl_begin_quads();
    sgl_v2f_t2f(-1.0F, -1.0F, 0.0F, 0.0F); // Bottom-left (0,0)
    sgl_v2f_t2f(1.0F, -1.0F, 1.0F, 0.0F); // Bottom-right (1,0)
    sgl_v2f_t2f(1.0F, 1.0F, 1.0F, 1.0F); // Top-right (1,1)
    sgl_v2f_t2f(-1.0F, 1.0F, 0.0F, 1.0F); // Top-left (0,1)
    sgl_end();

    sg_begin_pass(&(sg_pass){.swapchain = sglue_swapchain()});
    sgl_draw();
    sg_end_pass();
    sg_commit();

    in_events = (TapApiInputEvents){0};
}

static void cleanup(void)
{
    sgl_shutdown();
    sg_shutdown();
    if (tap_api_deinit().retcode != TAP_RESULT_OK)
    {
        printf("Failed with TAP_ERRNO: %u, source: %s\n", tap_errno_get(), tap_errno_source_get());
        exit(1);
    }
}

static void input(const sapp_event *ev)
{
    switch (ev->type)
    {
        case SAPP_EVENTTYPE_MOUSE_MOVE:
            in_events.mouse_dx = ev->mouse_dx;
            in_events.mouse_dy = ev->mouse_dy;
        case SAPP_EVENTTYPE_CHAR:
            in_events.keypress_charcode = (char)ev->char_code;
        default:
            (void)ev;
    }
}

sapp_desc sokol_main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = input,
        .width = VIEWPORT_WIDTH,
        .height = VIEWPORT_HEIGHT,
        .sample_count = 4,
        .window_title = "sokol",
        .icon.sokol_default = true,
        .logger.func = slog_func,
    };
}
