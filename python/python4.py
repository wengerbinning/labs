#!/usr/bin/env python

buffer = b'\x1b[0;0mdefault.hdoc\x1b[0m\r\n\x1b[0;0mdemo.conf\x1b[0m\r\n\x1b[0;0mdemo.hdoc\x1b[0m\r\n\x1b[1;32mshell.sh\x1b[0m\r\n\x1b[0;0mdefault.hdoc\x1b[0m\r\n\x1b[0;0mdemo.conf\x1b[0m\r\n\x1b[0;0mdemo.hdoc\x1b[0m\r\n\x1b[1;32mshell.sh\x1b[0m\r\n\x1b[0;0mdefault.hdoc\x1b[0m\r\n\x1b[0;0mdemo.conf\x1b[0m\r\n\x1b[0;0mdemo.hdoc\x1b[0m\r\n\x1b[1;32mshell.sh\x1b[0m\r\n\x1b[0;0mdefault.hdoc\x1b[0m\r\n\x1b[0;0mdemo.conf\x1b[0m\r\n\x1b[0;0mdemo.hdoc\x1b[0m\r\n\x1b[1;32mshell.sh\x1b[0m\r\n\x1b[0;0mdefault.hdoc\x1b[0m\r\n\x1b[0;0mdemo.conf\x1b[0m\r\n\x1b[0;0mdemo.hdoc\x1b[0m\r\n\x1b[1;32mshell.sh\x1b[0m\r\n\x1b[0;0mdefault.hdoc\x1b[0m\r\n\x1b[0;0mdemo.conf\x1b[0m\r\n\x1b[0;0mdemo.hdoc\x1b[0m\r\n\x1b[1;32mshell.sh\x1b[0m\r\n'
text = buffer.decode('utf-8')

context= []

idx = 0
esc = False
ctl = False
attr = ''
value = ''
attrs = []

while idx < len(buffer):
    if text[idx] == '\x1B':
        esc = True
        if len(value):
            # print(attrs, value.encode())
            context.append({'attrs':attrs, 'value':value.encode()})
            value = ''
        elif len(attrs):
            context.append({'attrs':attrs, 'value':b''})
        attrs = []
    elif esc and text[idx] == '\x5B':
        ctl = True
        # print(idx)
    elif ctl and text[idx] == '\x3B':
        # print("#" + attr)
        attrs.append(attr)
        attr = ""
    elif ctl and text[idx] == '\x6D':
        ctl = False
        attrs.append(attr)
        attr = ""
        # print("@" + attr)
    elif ctl:
        attr += text[idx]
    elif text[idx] == '\x0D':
        pass
    elif text[idx] == '\x0A':
        # value += "\n"
        pass
    else:
        value += text[idx]
    #
    idx += 1

if len(value):
    context.append({'attrs':attrs, 'value':value.encode()})
elif len(attrs):
    context.append({'attrs':attrs, 'value':b''})


for each in context:
    print(each)
