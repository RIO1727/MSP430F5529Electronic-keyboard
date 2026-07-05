#include "keyboard.h"
#include "system.h"

/************************ 矩阵键盘引脚定义 ************************/
#define COL_MASK    (BIT5 | BIT4 | BIT3 | BIT2)
#define ROW0        BIT3
#define ROW1        BIT0
#define ROW2        BIT2
#define ROW3        BIT1
#define ROW_MASK    (ROW0 | ROW1 | ROW2 | ROW3)

/************************ 内部状态变量 ************************/
static unsigned char last_key = KEY_NONE;
static unsigned char current_key = KEY_NONE;
static unsigned char pressed_key = KEY_NONE;
static unsigned char released_key = KEY_NONE;

void MatrixKey_Init(void)
{
    P4DIR |= ROW_MASK;
    P4OUT |= ROW_MASK;

    P1DIR &= ~COL_MASK;
    P1REN |= COL_MASK;
    P1OUT |= COL_MASK;

    last_key = KEY_NONE;
    current_key = KEY_NONE;
    pressed_key = KEY_NONE;
    released_key = KEY_NONE;
}

/************************ 内部：扫描单行 ************************/
static unsigned char scan_single_row(unsigned char row_bit, unsigned char base_idx)
{
    unsigned char col_stat;
    unsigned char key_val = KEY_NONE;

    P4OUT &= ~row_bit;
    __delay_cycles(100);
    col_stat = P1IN & COL_MASK;
    if(col_stat != COL_MASK) {
        delay_debounce();
        col_stat = P1IN & COL_MASK;
        if(col_stat != COL_MASK) {
            if((col_stat & BIT2) == 0) key_val = base_idx + 0;
            else if((col_stat & BIT3) == 0) key_val = base_idx + 1;
            else if((col_stat & BIT4) == 0) key_val = base_idx + 2;
            else if((col_stat & BIT5) == 0) key_val = base_idx + 3;
        }
    }
    P4OUT |= row_bit;
    return key_val;
}

/************************ 扫描全键盘（非阻塞）************************/
void MatrixKey_Scan(void)
{
    unsigned char key = KEY_NONE;

    // 逐行扫描，优先响应先按下的键（单键模式）
    key = scan_single_row(ROW0, 0);
    if(key == KEY_NONE) key = scan_single_row(ROW1, 4);
    if(key == KEY_NONE) key = scan_single_row(ROW2, 8);
    if(key == KEY_NONE) key = scan_single_row(ROW3, 12);

    current_key = key;

    // 检测按下/松开边沿事件
    if(current_key != KEY_NONE && last_key == KEY_NONE) {
        pressed_key = current_key;
        released_key = KEY_NONE;
    } else if(current_key == KEY_NONE && last_key != KEY_NONE) {
        released_key = last_key;
        pressed_key = KEY_NONE;
    } else {
        pressed_key = KEY_NONE;
        released_key = KEY_NONE;
    }

    last_key = current_key;
}

unsigned char MatrixKey_GetPressed(void)
{
    unsigned char ret = pressed_key;
    pressed_key = KEY_NONE; // 读取后清除事件
    return ret;
}

unsigned char MatrixKey_GetReleased(void)
{
    unsigned char ret = released_key;
    released_key = KEY_NONE; // 读取后清除事件
    return ret;
}

unsigned char MatrixKey_GetCurrent(void)
{
    return current_key;
}
