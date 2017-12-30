#pragma once
#define GAP                                   (20)
#define BORDER_GAP                            (30)
#define SEND_EDITOR_HEIGHT                    (40)


#define HEIGHT_TOTAL_PORTION                  (11)
#define HEIGHT_RECEIVE_PORTION                (7)
#define HEIGHT_GAP_PORTION                    (1)
#define HEIGHT_SEND_PORTION                   (3)

#define HEIGHT_RECEIVE_AREA_TOTOAL_PORTION    (9)
#define HEIGHT_RECEIVE_AREA_EDITOR_PORTION    (8)
#define HEIGHT_RECEIVE_AREA_CONTROL_PORTION   (1)

#define HEIGHT_SEND_AREA_TOTOAL_PORTION      (10)
#define HEIGHT_SEND_AREA_EDITOR_PORTION      (4)
#define HEIGHT_SEND_AREA_CONTROL1_PORTION    (3)
#define HEIGHT_SEND_AREA_CONTROL2_PORTION    (3)

void init_layout(HWND hwnd);
BOOL set_layout(HWND hwnd);
BOOL fresh_serial_port(HWND hwnd);