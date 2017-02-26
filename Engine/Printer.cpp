#include "Printer.h"

int base;
HDC hDC = NULL;
int text_last;
char text[1024];

void printer_build_font(GLvoid)  // Построение нашего растрового шрифта
{
	
}

void printer_kill_font(void)            // Удаление шрифта
{
	glDeleteLists(base, 96);        // Удаление всех 96 списков отображения ( НОВОЕ )
}



GLvoid glPrint(const char *fmt, ...){       // Заказная функция «Печати» GL{
	printer_on_draw_frame(fmt);
}

void printer_on_draw_frame(const char* str) {

}
