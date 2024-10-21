#include "mymain.h"


//LCD
char text[50];
uint8_t lcd_show=0;

extern vu16 dummy;
void my_LCD_Init(uint8_t mode)
{
    LCD_CtrlLinesConfig();
    dummy = LCD_ReadReg(0);

    if(dummy == 0x8230)
    {
        REG_8230_Init();
    }
    else
    {
        my_REG_932X_Init(mode);
    }
		
    dummy = LCD_ReadReg(0);

}

void my_REG_932X_Init(uint8_t mode)
{
    LCD_WriteReg(R227, 0x3008);   // Set internal timing
    LCD_WriteReg(R231, 0x0012); // Set internal timing
    LCD_WriteReg(R239, 0x1231);   // Set internal timing
	
	
		if(mode==0)LCD_WriteReg(R1, 0x0000);   // set SS and SM bit		  //0x0100
		else LCD_WriteReg(R1, 0x0100);
	
	
    LCD_WriteReg(R2, 0x0700);   // set 1 line inversion
    LCD_WriteReg(R3, 0x1030);     // set GRAM write direction and BGR=1.
    LCD_WriteReg(R4, 0x0000);     // Resize register
    LCD_WriteReg(R8, 0x0207);     // set the back porch and front porch
    LCD_WriteReg(R9, 0x0000);     // set non-display area refresh cycle ISC[3:0]
    LCD_WriteReg(R10, 0x0000);    // FMARK function
    LCD_WriteReg(R12, 0x0000);  // RGB interface setting
    LCD_WriteReg(R13, 0x0000);    // Frame marker Position
    LCD_WriteReg(R15, 0x0000);  // RGB interface polarity
    /**************Power On sequence ****************/
    LCD_WriteReg(R16, 0x0000);    // SAP, BT[3:0], AP, DSTB, SLP, STB
    LCD_WriteReg(R17, 0x0007);    // DC1[2:0], DC0[2:0], VC[2:0]
    LCD_WriteReg(R18, 0x0000);  // VREG1OUT voltage
    LCD_WriteReg(R19, 0x0000);    // VDV[4:0] for VCOM amplitude
    //	Delay_Ms(200);                    // Delay 200 MS , Dis-charge capacitor power voltage
    HAL_Delay(200);
    LCD_WriteReg(R16, 0x1690);    // SAP, BT[3:0], AP, DSTB, SLP, STB
    LCD_WriteReg(R17, 0x0227);  // R11H=0x0221 at VCI=3.3V, DC1[2:0], DC0[2:0], VC[2:0]
    //	Delay_Ms(50);      // Delay 50ms
    HAL_Delay(50);
    LCD_WriteReg(R18, 0x001D);  // External reference voltage= Vci;
    //	Delay_Ms(50);      // Delay 50ms
    HAL_Delay(50);
    LCD_WriteReg(R19, 0x0800);  // R13H=1D00 when R12H=009D;VDV[4:0] for VCOM amplitude
    LCD_WriteReg(R41, 0x0014);  // R29H=0013 when R12H=009D;VCM[5:0] for VCOMH
    LCD_WriteReg(R43, 0x000B);    // Frame Rate = 96Hz
    //	Delay_Ms(50);      // Delay 50ms
    HAL_Delay(50);
    LCD_WriteReg(R32, 0x0000);  // GRAM horizontal Address
    LCD_WriteReg(R33, 0x0000);  // GRAM Vertical Address
    /* ----------- Adjust the Gamma Curve ---------- */
    LCD_WriteReg(R48, 0x0007);
    LCD_WriteReg(R49, 0x0707);
    LCD_WriteReg(R50, 0x0006);
    LCD_WriteReg(R53, 0x0704);
    LCD_WriteReg(R54, 0x1F04);
    LCD_WriteReg(R55, 0x0004);
    LCD_WriteReg(R56, 0x0000);
    LCD_WriteReg(R57, 0x0706);
    LCD_WriteReg(R60, 0x0701);
    LCD_WriteReg(R61, 0x000F);
    /* ------------------ Set GRAM area --------------- */
    LCD_WriteReg(R80, 0x0000);    // Horizontal GRAM Start Address
    LCD_WriteReg(R81, 0x00EF);    // Horizontal GRAM End Address
    LCD_WriteReg(R82, 0x0000);  // Vertical GRAM Start Address
    LCD_WriteReg(R83, 0x013F);  // Vertical GRAM Start Address
		
    if(mode==0)LCD_WriteReg(R96, 0x2700);  // Gate Scan Line		  0xA700
		else LCD_WriteReg(R96, 0xA700);

		
    LCD_WriteReg(R97, 0x0001);  // NDL,VLE, REV
    LCD_WriteReg(R106, 0x0000); // set scrolling line
    /* -------------- Partial Display Control --------- */
    LCD_WriteReg(R128, 0x0000);
    LCD_WriteReg(R129, 0x0000);
    LCD_WriteReg(R130, 0x0000);
    LCD_WriteReg(R131, 0x0000);
    LCD_WriteReg(R132, 0x0000);
    LCD_WriteReg(R133, 0x0000);
    /* -------------- Panel Control ------------------- */
    LCD_WriteReg(R144, 0x0010);
    LCD_WriteReg(R146, 0x0000);
    LCD_WriteReg(R147, 0x0003);
    LCD_WriteReg(R149, 0x0110);
    LCD_WriteReg(R151, 0x0000);
    LCD_WriteReg(R152, 0x0000);
    /* Set GRAM write direction and BGR = 1 */
    /* I/D=01 (Horizontal : increment, Vertical : decrement) */
    /* AM=1 (address is updated in vertical writing direction) */
    LCD_WriteReg(R3, 0x01018);    //0x1018

    LCD_WriteReg(R7, 0x0173);   // 262K color and display ON
}





//LED
uint16_t led_sta;
void led_set(){
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,led_sta<<7,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}


//adc
float pa4_adc,pa5_adc;
void adc2_read(){
	HAL_ADC_Start(&hadc2);
	uint16_t tmp1=HAL_ADC_GetValue(&hadc2);
	
	
	HAL_ADC_Start(&hadc2);
	uint16_t tmp2=HAL_ADC_GetValue(&hadc2);
	
	pa4_adc=tmp1*3.3/4096;
	pa5_adc=tmp2*3.3/4096;
}


//OC
void PA7_Pulse_Out(uint32_t x){
	uint32_t oc_tmp=800000/x;
	__HAL_TIM_SET_PRESCALER(&htim3,oc_tmp);
}

uint8_t oc_mode=0;


//PARA
uint8_t X=1,Y=1;

//REC
uint8_t rec_swit=0;
uint32_t N_pa4=0,N_pa5=0;
float A_pa4,A_pa5,T_pa4,T_pa5,H_pa4,H_pa5;

float pa4_dat[120],pa5_dat[120];

