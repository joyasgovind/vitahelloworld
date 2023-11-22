/*
 * Copyright (c) 2015 Sergi Granell (xerpi)
 */

#include <stdlib.h>
#include <stdio.h>

#include <psp2/ctrl.h>
#include <psp2/touch.h>
#include <psp2/display.h>
#include <psp2/gxm.h>
#include <psp2/kernel/processmgr.h>

#include "utils.h"
#include "draw.h"

#define PASSWORD "dbit"

int main()
{
    char enteredPassword[20];

    // Ask for the password
    printf("Enter the password: ");
    scanf("%s", enteredPassword);

    // Check if the entered password is correct
    if (strcmp(enteredPassword, PASSWORD) != 0)
    {
        printf("Incorrect password. Exiting...\n");
        sceKernelExitProcess(0);
        return 0;
    }

    // If the password is correct, proceed with the rest of the code

    init_video();

    /* Enable analog stick */
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);

    /* Enable front touchscreen */
    sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, 1);

    /* FPS counting */
    SceUInt64 cur_micros = 0, delta_micros = 0, last_micros = 0;
    uint32_t frames = 0;
    float fps = 0.0f;

    /* Input variables */
    SceCtrlData pad;
    SceTouchData touch;

    while (1)
    {
        clear_screen();

        /* Read controls and touchscreen */
        sceCtrlPeekBufferPositive(0, &pad, 1);
        sceTouchPeek(0, &touch, 1);

        if (pad.buttons & SCE_CTRL_START)
        {
            break;
        }

        font_draw_string(10, 10, RGBA8(0, 0, 255, 255), "PSVita sample by xerpi!");
        font_draw_stringf(SCREEN_W - 160, 10, RGBA8(0, 255, 0, 255), "FPS: %.2f", fps);
        font_draw_stringf(10, 30, RGBA8(255, 0, 0, 255),
                          "Password correct. Welcome!");

        /* Calculate FPS */
        cur_micros = sceKernelGetProcessTimeWide();

        if (cur_micros >= (last_micros + 1000000))
        {
            delta_micros = cur_micros - last_micros;
            last_micros = cur_micros;
            fps = (frames / (double)delta_micros) * 1000000.0f;
            frames = 0;
        }

        swap_buffers();
        sceDisplayWaitVblankStart();
        frames++;
    }

    end_video();
    sceKernelExitProcess(0);
    return 0;
}
