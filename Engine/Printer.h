#pragma once




#include <windows.h>  // ������������ ���� ��� Windows
#include <stdio.h>    // ������������ ���� ��� ����������� ���������� �����/������
#include "glad.h"// ������������ ���� ��� ���������� OpenGL32
#include <gl\glu.h>   // ������������ ���� ��� ���������� GLu32
#include <math.h>     // ������������ ���� ��� �������������� ���������� ( ����� )
#include <stdarg.h>

GLvoid glPrint(const char *fmt, ...);
void printer_build_font(GLvoid);
void printer_on_draw_frame(const char* fmt);
