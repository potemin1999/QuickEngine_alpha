#include "Printer.h"

int base;
HDC hDC = NULL;
int text_last;
char text[1024];

void printer_build_font(GLvoid)  // ���������� ������ ���������� ������
{
	
}

void printer_kill_font(void)            // �������� ������
{
	glDeleteLists(base, 96);        // �������� ���� 96 ������� ����������� ( ����� )
}



GLvoid glPrint(const char *fmt, ...){       // �������� ������� ������� GL{
	printer_on_draw_frame(fmt);
}

void printer_on_draw_frame(const char* str) {

}
