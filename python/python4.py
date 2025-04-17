#!/usr/bin/env python




def shell2txt(buf):


def shell2cus(buf):
    lines = buf.splitlines()
    for i, line in enumerate(lines[1:], start=1):



def ansi_to_curses(stdscr, text):
    """将ANSI颜色代码转换为curses属性"""
    curses.start_color()
    curses.use_default_colors()

    # 定义ANSI到curses的颜色映射
    color_map = {
        '31': (curses.COLOR_RED, -1),
        '32': (curses.COLOR_GREEN, -1),
        '33': (curses.COLOR_YELLOW, -1),
        '34': (curses.COLOR_BLUE, -1),
        '35': (curses.COLOR_MAGENTA, -1),
        '36': (curses.COLOR_CYAN, -1),
        '37': (curses.COLOR_WHITE, -1),
        '1': curses.A_BOLD,
        '4': curses.A_UNDERLINE,
        '7': curses.A_REVERSE
    }

    # 初始化颜色对
    for i, (fg, bg) in enumerate(color_map.values(), start=1):
        if isinstance(fg, tuple):
            curses.init_pair(i, fg[0], fg[1])

    result = []
    current_attrs = 0
    pos = 0

    while pos < len(text):
        if text[pos] == '\x1B' and pos + 1 < len(text) and text[pos+1] == '[':
            # 找到ANSI转义序列
            end = text.find('m', pos)
            if end == -1:
                pos += 1
                continue

            codes = text[pos+2:end].split(';')
            for code in codes:
                if code in color_map:
                    attr = color_map[code]
                    if isinstance(attr, tuple):
                        # 颜色代码
                        pair_num = list(color_map.values()).index(attr) + 1
                        current_attrs |= curses.color_pair(pair_num)
                    else:
                        # 文本属性
                        current_attrs |= attr

            pos = end + 1
        else:
            result.append((text[pos], current_attrs))
            pos += 1

    return result

def render_ansi_text(stdscr, y, x, text):
    """在curses窗口中渲染包含ANSI代码的文本"""
    parsed = ansi_to_curses(stdscr, text)
    current_x = x
    for char, attrs in parsed:
        try:
            stdscr.addstr(y, current_x, char, attrs)
            current_x += 1
        except curses.error:
            break



def caurse_addstr(win, x, y, text, shell=False):
    colmaps = {
        '1': curses.A_BOLD,
        '4': curses.A_UNDERLINE,
        '7': curses.A_REVERSE
        '31': (curses.COLOR_RED,     -1),
        '32': (curses.COLOR_GREEN,   -1),
        '33': (curses.COLOR_YELLOW,  -1),
        '34': (curses.COLOR_BLUE,    -1),
        '35': (curses.COLOR_MAGENTA, -1),
        '36': (curses.COLOR_CYAN,    -1),
        '37': (curses.COLOR_WHITE,   -1),
    }
    for i, (fg, bg) in enumerate(colmaps.values(), start=1):
        if isinstance(fg, tuple):
            curses.init_pair(i, fg[0], fg[1])

    #
    while idx < len(text):
        if text[idx] == '\x1B' and idx + 1


def func(text):
    for char in text:
        if char == '\x1B':
            escape = True
        elif escape:
            if char.isalpha():
                escape = False



