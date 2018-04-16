  
#include <SFE_TSL2561.h>
/*
功能：读数据
*/
void SFE_TSL2561_read(  unsigned char cmd,unsigned char count,unsigned char data[])
{
    //1、发开始信号
    //2、发从设备地址
    //3、等从设备应答
    //4、发命令字
    //5、等从设备应答
    //6、发开始信号
    //7、发从设备地址
    //8、等从设备应答
    //9、开始接收字节数据，每接收完一个字节后主设备应答
    int i;
    IIC_Start();
    IIC_Send_Byte(TSL2561_ADDR<<1);
    IIC_Wait_Ack();
    IIC_Send_Byte(cmd);
    IIC_Wait_Ack();
    
    IIC_Start();
    IIC_Send_Byte((TSL2561_ADDR<<1)+1);
    IIC_Wait_Ack();
    
    for(i=0;i<count;i++)
    {
        data[i]=IIC_Read_Byte(1);
    }
    IIC_Stop(); 
}
/*
功能：写数据
*/
void SFE_TSL2561_write( unsigned char cmd,unsigned char count,unsigned char data[])
{
    //1、发开始信号
    //2、发从设备地址
    //3、等从设备应答
    //4、发命令字
    //5、等从设备应答
    //6、开始发送字节数据1、2、3...,每发送完一个字节都要等从设备应答之后可以产生停止条件
  
    int i;
    IIC_Start();
    IIC_Send_Byte(TSL2561_ADDR<<1);
    IIC_Wait_Ack();
    IIC_Send_Byte(cmd);
    IIC_Wait_Ack();
    for(i=0;i<count;i++)
    {
        IIC_Send_Byte(data[i]);
        IIC_Wait_Ack();
    }
    IIC_Stop();
}
//初始化设备
void SFE_TSL2561_init(void)
{  
    //1、初始化通信接口
    //2、打开设备电源
    unsigned char data=3;
    IIC_Init();
    SFE_TSL2561_write(TSL2561_REG_COMMAND+TSL2561_REG_CONTROL,1,&data);
}
//读数据寄存器，data[0]、data[1]分别保存了data0和data1的数据
void SFE_TSL2561_read_data0_and_data1(unsigned int data[])
{
    unsigned char datas[4];
    SFE_TSL2561_read(TSL2561_REG_COMMAND|(1<<4)+TSL2561_REG_DATA0LOW,sizeof(datas),datas);
    data[0]=datas[0]|(datas[1]<<8);
    data[1]=datas[2]|(datas[3]<<8);  
}
void SFE_TSL2561_read_gain_and_intge(unsigned int data[])
{
    unsigned char dat;
    SFE_TSL2561_read(TSL2561_REG_COMMAND+TSL2561_REG_TIMING,1,&dat);
    if(dat&(1<<4))
        data[0]=1;
    else
        data[0]=0;
    data[1]=dat&0x3;
    
}






