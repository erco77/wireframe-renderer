// vim: autoindent tabstop=8 shiftwidth=4 expandtab softtabstop=4

#pragma once

#define VT100_SCREEN_XMAX 600 //126  // 600
#define VT100_SCREEN_YMAX 150 //125  // 150

void init_VT100();
void Clear_VT100();
void DrawLine_VT100(int x1, int y1, int x2, int y2);
