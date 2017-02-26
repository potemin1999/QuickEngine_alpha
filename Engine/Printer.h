#pragma once




#include <windows.h>  // Заголовочный файл для Windows
#include <stdio.h>    // Заголовочный файл для стандартной библиотеки ввода/вывода
#include "glad.h"// Заголовочный файл для библиотеки OpenGL32
#include <gl\glu.h>   // Заголовочный файл для библиотеки GLu32
#include <math.h>     // Заголовочный файл для математической библиотеки ( НОВОЕ )
#include <stdarg.h>

GLvoid glPrint(const char *fmt, ...);
void printer_build_font(GLvoid);
void printer_on_draw_frame(const char* fmt);
