#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    


//OLEDÄ£Ê½ÉèÖ
#define OLED_RD  PGout(13)	   Ã
//0: 4Ïß´®ÐÐÄ£Ê½  £¨Ä£¿éµÄBS1£¬BS2¾ù½ÓGND£©
//1: ²¢ÐÐ8080Ä£Ê½ £¨Ä£¿éµÄBS1£¬BS2¾ù½ÓVCC£©
#define OLED_MODE 0 
		    						  
//---------------------------OLED¶Ë¿Ú¶¨Òå--------------------------  					   
#define OLED_CS  PDout(6)
#define OLED_RST PGout(15) 	
#define OLED_RS  PDout(3)
#define OLED_WR  PGout(14)		  
//PC0~7,×÷ÎªÊý¾ÝÏß
 
#define DATAOUT(x) GPIO_Write(GPIOC,x);//Êä³ö  
  
//Ê¹ÓÃ4Ïß´®ÐÐ½Ó¿ÚÊ±Ê¹ÓÃ 
#define OLED_SCLK PCout(0)
#define OLED_SDIN PCout(1)
		     
#define OLED_CMD  0	//Ð´ÃüÁî
#define OLED_DATA 1	//Ð´Êý¾Ý
//OLED¿ØÖÆÓÃº¯Êý
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);  		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);	 
#endif  
	 







 

