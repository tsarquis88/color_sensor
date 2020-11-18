#include "lpc17xx_spi.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "../include/lcd.h"
#include "../include/utils.h"

const char char_array[94]=
{
  '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+',
  ',', '-', '.', '/', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', ':', ';', '<', '=', '>', '?', '@', 'A', 'B',
  'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
  'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
  'Y', 'Z', '[', '?', ']', '^', '_', '`', 'a', 'b', 'c',
  'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
  'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
  'z', '{', '|', '}', '?', '?'
};

void config_lcd_pinsel(){

	PINSEL_CFG_Type config_pin;
	/*
	 * P0.15 -> SCK
	 * P0.18 .> MOSI
	 * P0.24 -> rs_lcd
	 * P0.25 -> reset_lcd
	 */

	config_pin.OpenDrain	=	PINSEL_PINMODE_NORMAL;
	config_pin.Pinmode		=	PINSEL_PINMODE_PULLUP;
	config_pin.Portnum		=	PINSEL_PORT_0;

	/*
	 * SCK,MOSI,MISO son funciones #3
	 */

	config_pin.Funcnum		=	PINSEL_FUNC_3;
	config_pin.Pinnum		=	PINSEL_PIN_15;
	PINSEL_ConfigPin(&config_pin);

	config_pin.Pinnum		=	PINSEL_PIN_18;
	PINSEL_ConfigPin(&config_pin);

	config_pin.Funcnum		=	PINSEL_FUNC_0;
	config_pin.Pinnum		=	PINSEL_PIN_24;
	PINSEL_ConfigPin(&config_pin);
	GPIO_SetDir(PORT_0,PIN_24,OUTPUT);

	config_pin.Pinnum		=	PINSEL_PIN_25;
	PINSEL_ConfigPin(&config_pin);
	GPIO_SetDir(PORT_0,PIN_25,OUTPUT);

	config_pin.Pinnum		=	PINSEL_PIN_26;
	PINSEL_ConfigPin(&config_pin);
	GPIO_SetDir(PORT_0,PIN_26,OUTPUT);

	//	lpc mosi
	config_pin.Portnum		=	PINSEL_PORT_0;
	config_pin.Funcnum		=	PINSEL_FUNC_3;
	config_pin.Pinnum		=	PINSEL_PIN_17;
	PINSEL_ConfigPin(&config_pin);

	return;
}

void config_lcd_spi(){
	SPI_CFG_Type config_spi;

	//Flanco de reloj #2 para la fase del reloj
	config_spi.CPHA	=	SPI_CPHA_FIRST;

	//Polaridad baja del reloj
	config_spi.CPOL	=	SPI_CPOL_HI;

	//Frecuencia del reloj, deberia ser menor a (SPI peripheral clock)/8
	config_spi.ClockRate	=	8000000;

	//Primero se transmite el MSB
	config_spi.DataOrder	=	SPI_DATA_MSB_FIRST;

	//Numero de bits de datos
	config_spi.Databit		=	SPI_DATABIT_8;

	//Modo maestro
	config_spi.Mode			= 	SPI_MASTER_MODE;

	/*
	 * Inicializamos SPI de acuerdo a los parametros definidos
	 */

	SPI_Init(LPC_SPI, &config_spi);

	return;
}



void TFT_init()
{
   TFT_reset();
   TFT_write_reg(0x000, 0x0001);
   retardo_systick(1);

   TFT_write_reg(0x100, 0x0000);
   TFT_write_reg(0x101, 0x0000);
   TFT_write_reg(0x102, 0x3110);
   TFT_write_reg(0x103, 0xE200);
   TFT_write_reg(0x110, 0x009D);
   TFT_write_reg(0x111, 0x0022);
   TFT_write_reg(0x100, 0x0120);
   retardo_systick(2);

   TFT_write_reg(0x100, 0x3120);
   retardo_systick(8);

   TFT_write_reg(0x001, 0x0100);
   TFT_write_reg(0x002, 0x0000);
   TFT_write_reg(0x003, 0x1038);
   TFT_write_reg(0x006, 0x0000);
   TFT_write_reg(0x007, 0x0101);
   TFT_write_reg(0x008, 0x0808);
   TFT_write_reg(0x009, 0x0000);
   TFT_write_reg(0x00B, 0x0000);
   TFT_write_reg(0x00C, 0x0000);
   TFT_write_reg(0x00D, 0x0018);

   TFT_write_reg(0x012, 0x0000);
   TFT_write_reg(0x013, 0x0000);
   TFT_write_reg(0x018, 0x0000);
   TFT_write_reg(0x019, 0x0000);

   TFT_write_reg(0x203, 0x0000);
   TFT_write_reg(0x204, 0x0000);

   TFT_write_reg(0x210, 0x0000);
   TFT_write_reg(0x211, 0x00EF);
   TFT_write_reg(0x212, 0x0000);
   TFT_write_reg(0x213, 0x013F);
   TFT_write_reg(0x214, 0x0000);
   TFT_write_reg(0x215, 0x0000);
   TFT_write_reg(0x216, 0x0000);
   TFT_write_reg(0x217, 0x0000);

   TFT_write_reg(0x300, 0x5343);
   TFT_write_reg(0x301, 0x1021);
   TFT_write_reg(0x302, 0x0003);
   TFT_write_reg(0x303, 0x0011);
   TFT_write_reg(0x304, 0x050A);
   TFT_write_reg(0x305, 0x4342);
   TFT_write_reg(0x306, 0x1100);
   TFT_write_reg(0x307, 0x0003);
   TFT_write_reg(0x308, 0x1201);
   TFT_write_reg(0x309, 0x050A);

   TFT_write_reg(0x400, 0x4027);
   TFT_write_reg(0x401, 0x0000);
   TFT_write_reg(0x402, 0x0000);
   TFT_write_reg(0x403, 0x013F);
   TFT_write_reg(0x404, 0x0000);

   TFT_write_reg(0x200, 0x0000);
   TFT_write_reg(0x201, 0x0000);

   TFT_write_reg(0x100, 0x7120);
   TFT_write_reg(0x007, 0x0103);
   retardo_systick(1);
   TFT_write_reg(0x007, 0x0113);

   retardo_systick(153);
}

void TFT_reset()
{
   write_pin(reset_port, reset_pin,LOW);
   retardo_systick(1);
   write_pin(reset_port, reset_pin,HIGH);
   retardo_systick(1);
}

void TFT_write_word(long int value)
{
   send_spi(value >> 8);
   send_spi(value);
}

void TFT_write(long int value, short mode)
{
   write_pin(rs_port, rs_pin, mode);
   write_pin(cs_lcd_port, cs_lcd_pin,LOW);
   TFT_write_word(value);
   write_pin(cs_lcd_port, cs_lcd_pin,HIGH);
}

void TFT_write_RAM_prepare()
{
   TFT_write(0x0202, CMD);
}

void TFT_write_reg(long int cmd_value, long int data_value)
{
   TFT_write(cmd_value, CMD);
   TFT_write(data_value, DAT);
}

void TFT_set_display_address_window(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye)
{
   TFT_write_reg(0x210, ys);
   TFT_write_reg(0x211, ye);
   TFT_write_reg(0x212, xs);
   TFT_write_reg(0x213, xe);
}

void TFT_set_cursor(long int x_pos, long int y_pos)
{
   TFT_write_reg(0x200, y_pos);
   TFT_write_reg(0x201, x_pos);

   TFT_write_RAM_prepare();
}

void TFT_fill(long int colour)
{
   long int index = (res_x * res_y);
   TFT_set_display_address_window(0, 0,res_x,res_y);
   TFT_set_cursor(0, 0);
   while(index)
   {
      TFT_write(colour, DAT);
      index--;
   };
}

/*
*	Funciones para dibujo de geometria basica
*/

void Draw_Pixel(long int x_pos, long int y_pos, long int colour)
{

    if((x_pos >= res_x) || (y_pos >= res_y) || (x_pos < 0) || (y_pos < 0))
    {
        return;
    }

    TFT_set_cursor(x_pos, y_pos);

    TFT_write(colour, DAT);

}

void Draw_Triangle(long int x1, long int y1, long int x2, long int y2, long int x3, long int y3, short fill, long int colour)
{
    long int a = 0;
    long int b = 0;
    long int sa = 0;
    long int sb = 0;
    long int yp = 0;
    long int last = 0;
    long int dx12 = 0;
    long int dx23 = 0;
    long int dx13 = 0;
    long int dy12 = 0;
    long int dy23 = 0;
    long int dy13 = 0;

    switch(fill)
    {
        case YES:
        {
            if(y1 > y2)
            {
                swap_values(&y1, &y2);
                swap_values(&x1, &x2);
            }
            if(y2 > y3)
            {
                swap_values(&y3, &y2);
                swap_values(&x3, &x2);
            }
            if(y1 > y2)
            {
                swap_values(&y1, &y2);
                swap_values(&x1, &x2);
            }

            if(y1 == y3)
            {
                a = b = x1;

                if(x2 < a)
                {
                    a = x2;
                }
                else if(x2 > b)
                {
                    b = x2;
                }
                if(x2 < a)
                {
                    a = x3;
                }
                else if(x3 > b)
                {
                    b = x3;
                }

                Draw_H_Line(a, (a + (b - (a + 1))), y1, colour);
                return;
            }

            dx12 = (x2 - x1);
            dy12 = (y2 - y1);
            dx13 = (x3 - x1);
            dy13 = (y3 - y1);
            dx23 = (x3 - x2);
            dy23 = (y3 - y2);
            sa = 0,
            sb = 0;

            if(y2 == y3)
            {
                last = y2;
            }
            else
            {
                last = (y2 - 1);
            }

            for(yp = y1; yp <= last; yp++)
            {
                a = (x1 + (sa / dy12));
                b = (x1 + (sb / dy13));
                sa += dx12;
                sb += dx13;
                if(a > b)
                {
                    swap_values(&a, &b);
                }
                Draw_H_Line(a, (a + (b - (a + 1))), yp, colour);
            }

            sa = (dx23 * (yp - y2));
            sb = (dx13 * (yp - y1));
            for(; yp <= y3; yp++)
            {
                a = (x2 + (sa / dy23));
                b = (x1 + (sb / dy13));
                sa += dx23;
                sb += dx13;

                if(a > b)
                {
                    swap_values(&a, &b);
                }
                Draw_H_Line(a, (a + (b - (a + 1))), yp, colour);
            }


            break;
        }
        default:
        {
            Draw_Line(x1, y1, x2, y2, colour);
            Draw_Line(x2, y2, x3, y3, colour);
            Draw_Line(x3, y3, x1, y1, colour);
            break;
        }
    }
}

void Draw_Line(long int x1, long int y1, long int x2, long int y2, long int colour)
{
    long int dx = 0x0000;
    long int dy = 0x0000;
    long int stepx = 0x0000;
    long int stepy = 0x0000;
    long int fraction = 0x0000;

    dy = (y2 - y1);
    dx = (x2 - x1);

    if(dy < 0)
    {
        dy = -dy;
        stepy = -1;
    }
    else
    {
        stepy = 1;
    }

    if(dx < 0)
    {
        dx = -dx;
        stepx = -1;
    }
    else
    {
        stepx = 1;
    }

    dx <<= 0x01;
    dy <<= 0x01;

    Draw_Pixel(x1, y1, colour);

    if(dx > dy)
    {
        fraction = (dy - (dx >> 1));
        while(x1 != x2)
        {
            if(fraction >= 0)
            {
                y1 += stepy;
                fraction -= dx;
            }
            x1 += stepx;
            fraction += dy;

            Draw_Pixel(x1, y1, colour);
        }
    }
    else
    {
        fraction = (dx - (dy >> 1));

        while(y1 != y2)
        {
            if (fraction >= 0)
            {
                x1 += stepx;
                fraction -= dy;
            }
            y1 += stepy;
            fraction += dx;
            Draw_Pixel(x1, y1, colour);
        }
    }
}

void Draw_Circle(long int xc, long int yc, long int radius, short fill, long int colour)
{
     long int a = 0x0000;
     long int b = 0x0000;
     long int p = 0x0000;

     b = radius;
     p = (1 - b);

     do
     {
          switch(fill)
          {
              case YES:
              {
                  Draw_Line((xc - a), (yc + b), (xc + a), (yc + b), colour);
                  Draw_Line((xc - a), (yc - b), (xc + a), (yc - b), colour);
                  Draw_Line((xc - b), (yc + a), (xc + b), (yc + a), colour);
                  Draw_Line((xc - b), (yc - a), (xc + b), (yc - a), colour);
                  break;
              }
              default:
              {
                  Draw_Pixel((xc + a), (yc + b), colour);
                  Draw_Pixel((xc + b), (yc + a), colour);
                  Draw_Pixel((xc - a), (yc + b), colour);
                  Draw_Pixel((xc - b), (yc + a), colour);
                  Draw_Pixel((xc + b), (yc - a), colour);
                  Draw_Pixel((xc + a), (yc - b), colour);
                  Draw_Pixel((xc - a), (yc - b), colour);
                  Draw_Pixel((xc - b), (yc - a), colour);
                  break;
              }
          }

          if(p < 0)
          {
              p += (0x03 + (0x02 * a++));
          }
          else
          {
              p += (0x05 + (0x02 * ((a++) - (b--))));
          }
    }while(a <= b);
}

void Draw_Rectangle(long int x1, long int y1, long int x2, long int y2, short fill, unsigned char type, long int colour, long int back_colour)
{
     long int i = 0x0000;
     long int xmin = 0x0000;
     long int xmax = 0x0000;
     long int ymin = 0x0000;
     long int ymax = 0x0000;

     switch(fill)
     {
         case YES:
         {
             if(x1 < x2)
             {
                xmin = x1;
                xmax = x2;
             }
             else
             {
                xmin = x2;
                xmax = x1;
             }

             if(y1 < y2)
             {
                ymin = y1;
                ymax = y2;
             }
             else
             {
                ymin = y2;
                ymax = y1;
             }

             for(; xmin <= xmax; ++xmin)
             {
                 for(i = ymin; i <= ymax; ++i)
                 {
                     Draw_Pixel(xmin, i, colour);
                 }
             }

             break;
         }
         default:
         {
             Draw_V_Line(x1, y1, y2, colour);
             Draw_V_Line(x2, y1, y2, colour);
             Draw_H_Line(x1, x2, y1, colour);
             Draw_H_Line(x1, x2, y2, colour);
             break;
         }
     }

     if(type != SQUARE)
     {
         Draw_Pixel(x1, y1, back_colour);
         Draw_Pixel(x1, y2, back_colour);
         Draw_Pixel(x2, y1, back_colour);
         Draw_Pixel(x2, y2, back_colour);
     }
}

void Draw_V_Line(long int x1, long int y1, long int y2, unsigned colour)
{
    if(y1 > y2)
    {
       swap_values(&y1, &y2);
    }

    while(y2 > (y1 - 1))
    {
        Draw_Pixel(x1, y2, colour);
        y2--;
    }
}

void Draw_H_Line(long int x1, long int x2, long int y1, unsigned colour)
{
    if(x1 > x2)
    {
       swap_values(&x1, &x2);
    }

    while(x2 > (x1 - 1))
    {
        Draw_Pixel(x2, y1, colour);
        x2--;
    }
}

/*
*	Funciones para manejo de chars y strings
*/

void print_str(unsigned char x_pos, unsigned char y_pos, unsigned char font_size, unsigned long colour, char *ch)
{
  char *aux =ch;
  int tam=0;
  do{
    aux++;
    tam++;
     }while((*aux >= 0x20) && (*aux <= 0x7F));


  for(int i=tam;i>0;i--){
    print_char(x_pos, y_pos, font_size, colour, ch[i-1]);
    retardo_systick(30);
    x_pos += 0x06*font_size;
  }
   print_char(x_pos, y_pos, font_size, colour, 0x20);
}


void test_2D_graphics()
{
    long int i = 0;

    TFT_fill(Black);

    for(i = 10; i <= res_x; i += 10)
    {
        Draw_Line(i,0,i,res_y-1,White);
    }
    for(i = 10; i <= res_y; i += 10)
    {
        Draw_Line(0,i,res_x-1,i,White);
    }

    retardo_systick(153);

    TFT_fill(Black);
    for(i = 0; i < res_x; i += 5)
    {
        Draw_Line(0, 0, i, res_y-1, Light_Blue);
    }

    for(i = res_x-1; i > -1; i -= 5)
    {
        Draw_Line(res_x-1, res_y-1, i, 0, Blue);
    }

    retardo_systick(153);

    TFT_fill(Black);

    Draw_Rectangle(9, 9, res_x-10, res_y-10, NO, ROUND, Blue, Black);
    Draw_Rectangle(19, 19, res_x-20, res_y-20, YES, SQUARE, Cyan, Black);

    retardo_systick(153);

    TFT_fill(Black);

    Draw_Circle(res_x/2, res_y/2, 80, NO, Red);
    Draw_Circle(res_x/2, res_y/2, 40, YES, Yellow);


    retardo_systick(153);

    TFT_fill(Black);

    Draw_Triangle(10, res_y/2, res_x-10, 10, res_x-10, res_y-10, NO, Green);
    retardo_systick(90);
    Draw_Triangle(10, res_y/2, res_x-10, 10, res_x-10, res_y-10, YES, Grey_Blue);

    retardo_systick(153);

    TFT_fill(Black);
}


//TODO: creo que imprime al reves porque  el doble for imprime haciendo i--
void print_char(unsigned long x_pos, unsigned long y_pos, unsigned char font_size, unsigned long colour, char ch)
{
     char i = 0x00;
     char j = 0x00;

     unsigned long value = 0x0000;

     if(font_size <= 0)
     {
         font_size = 1;
     }

     if(x_pos < font_size)
     {
         x_pos = font_size;
     }

     for(i = 0x04; i >= 0x00; i--)
     {
         for(j = 0x00; j < 0x08; j++)
         {
             value = 0x0000;
             value = font[((char)ch) - 0x20][i];
             if(((value >> j) & 0x01) != 0x00)
             {
                 Draw_Font_Pixel(x_pos, y_pos, colour, font_size);
             }

             y_pos += font_size;
          }
          y_pos -= (font_size << 0x03);
          x_pos += font_size;
      }
}

void Draw_Font_Pixel(unsigned long x_pos, unsigned long y_pos, unsigned long colour, unsigned char pixel_size)
{

     TFT_set_cursor(x_pos, y_pos);

     for(int i=0;i<pixel_size;i++){
      for(int j=0;j<pixel_size;j++){
        Draw_Pixel(x_pos+i,y_pos+j,colour);
      }
     }
}



void test_colours()
{
  int k=18;
  int xz = res_x / k;
  for(int i=0;i<k;i++){
    Draw_Rectangle(i*xz,0,(i*xz)+xz ,res_y,YES,SQUARE,colors[i],colors[i]);
  }

    retardo_systick(50);
    TFT_fill(Black);
}


/*
*	Funciones para convertir datos.
*/

long int TFT_BGR2RGB(long int colour)
{
   long int r = 0x0000;
   long int g = 0x0000;
   long int b = 0x0000;
   long int rgb_colour = 0x0000;

   b = ((colour >> 0)  & 0x1F);
   g = ((colour >> 5)  & 0x3F);
   r = ((colour >> 11) & 0x1F);

   rgb_colour = ((b << 11) + (g << 5) + r);

   return rgb_colour;
}

long int RGB565_converter(unsigned char r, unsigned char g, unsigned char b)
{
   return (((((long int)r) >> 3) << 11) | ((((long int)g) >> 2) << 5) | (((long int)b) >> 3));
}

void swap_values(long int *a, long int *b)
{
    long int temp = 0x0000;

    temp = *b;
    *b = *a;
    *a = temp;
}



