#!/usr/bin/env python3
from PIL import Image
from math import *

CW = Image.open("data/ColorWheel.png").convert('RGB')

DIM = CW.width

C = DIM//2 #image center

header = open("cnc_lib/GUI/ColorWheel.h",'w')

header.write("#ifndef COLOR_WHEEL_H\n#define COLOR_WHEEL_H\n#if defined(CNC_LIB)\n#define CNC_LIB_EXPORT Q_DECL_EXPORT\n#else\n#define CNC_LIB_EXPORT Q_DECL_IMPORT\n#endif\n\n")

header.write("#include <QColor>\n#include \"cnc_types.h\"\n")

header.write("namespace cnc {\n")

nb_segments = 300

header.write("constexpr static uint NB_SEG = " + str(nb_segments) + ";\n")

header.write("constexpr static unsigned char WHEEL[NB_SEG][3] = {\n")

radius = int(C*0.95)

for i in range(nb_segments):
    th = 2*pi*float(i)/nb_segments
    r,g,b = CW.getpixel((int(C + radius*cos(th)),int(C+radius*sin(th))))
    header.write("{" + str(r) + ","+str(g)+","+str(b) + "}")
    if (i < nb_segments -1):
        header.write(",\n")
    else:
        header.write("};\n")

header.write('''\n
QColor get_RGB_by_angle(scalar th);''')


header.write("\n}\n#endif")

source = open("cnc_lib/GUI/ColorWheel.cpp",'w')
source.write('''#include \"ColorWheel.h\"\n
QColor cnc::get_RGB_by_angle(scalar th){
    if (th < 0)
        th += 2*M_PI;
    uint i = th/(2*M_PI)*NB_SEG;
    auto color = WHEEL[i];
    return QColor::fromRgb(color[0],color[1],color[2]);\n}''')
