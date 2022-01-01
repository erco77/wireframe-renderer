// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#pragma once

#define TEK4010_SCREEN_XMAX 1024
#define TEK4010_SCREEN_YMAX 780

void init_TEK4010();
void Clear_TEK4010();
void DrawLine_TEK4010(int x1, int y1, int x2, int y2);
