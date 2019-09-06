#include "stdio.h"
#include "string.h"
#include "time.h"

#define MAXINDEX 2000
unsigned int NumIndex[MAXINDEX];
enum ENUM_STATUS
{
    LINE_START,
    NUM_START, //需要读取的数字的开头
    NUM_END,   //需要读取的数字的结尾
    LINE_END,
    FILE_END    //文件读完成
};
enum ENUM_STATUS fp_Status = LINE_START;
    
unsigned int LineCount = 0; //每一行的逗号计数器，换行清0
unsigned int num = 0;
unsigned int displaycount = 0;
void main(void)
{
    FILE *fp_r,*fp_w;
    char m_temp;
	unsigned int tempChange,i;
	time_t t;
    if((fp_r = fopen("f_funcCode.c","r")) == NULL)    //打开要读取的文件
	{
		printf("The file can not be opened.\n");
		while(1);//结束程序的执行
	}
    if((fp_w = fopen("AutoGenerate.c","w+")) == NULL) //打开写入的文件
	{
		printf("The file can not be created.\n");
		while(1);//结束程序的执行
	}
    while(1)
    {
		if(++displaycount > 10)
	   {
		   displaycount = 0;
		   m_temp =  fgetc(fp_r); //读取一个字符
		   
		   if(m_temp == ',')
		   {
				LineCount++;
		   }
		   else if(m_temp == '\n')
		   {
				fp_Status = LINE_START;
				LineCount = 0;
		   }

		   if(LineCount == 4)   //每一行第4个逗号和第五个逗号中的数据为需要的eeprom地址
		   {
				fp_Status == NUM_START;
				m_temp =  fgetc(fp_r); //先读取','后的一个字符
				while(m_temp != ',')
				{
					if(m_temp == ' ')
					{}
					else
					{
						fputc(m_temp,fp_w); //写入到文件中
					}
					m_temp =  fgetc(fp_r); //读取一个字符
				}
				//fputc(',',fp_w);
			   // fprintf(fp_w,"  // %d",num);
				fputc('\n',fp_w);
				fp_Status == NUM_END;
				num++;
				LineCount = 0;
		   }

		   if(m_temp == EOF)    //读完了整个文件
		   {
				fp_Status = FILE_END;
				printf("排序完成!\n");
				fclose(fp_r);
				fclose(fp_w);	//先关闭f_funcCode.c和提取出eeprom地址的中间文件
				
				if((fp_r = fopen("AutoGenerate.c","r")) == NULL)    //打开要读取的文件
				{
					printf("The file can not be opened.\n");
					while(1);//结束程序的执行
				}
				if((fp_w = fopen("f_table_eeprom2Fc.c","w+")) == NULL) //打开写入的文件
				{
					printf("The file can not be created.\n");
					while(1);//结束程序的执行
				}
				time(&t);	//获取当前时间
				fprintf(fp_w,"// C语言自动生成eeprom数组-by wk\n");
				fprintf(fp_w,"// 时间戳:%s\n",ctime(&t));	// ctime(&t)将日期转为字符串并打印
				fprintf(fp_w,"const Uint16 eeprom2Fc[] =\n{\n//index eeprom地址\n");	//数组格式
				for(i = 0;i < MAXINDEX; i++)
				{
					fscanf(fp_r,"%d",&tempChange);
					NumIndex[tempChange] = i;
				}
				for(i = 0;i < MAXINDEX; i++)
				{
					fprintf(fp_w,"    %d, //%d\n",NumIndex[i],i);
				}
				fprintf(fp_w,"}");
				
				fclose(fp_r);
				fclose(fp_w);
				if(remove("AutoGenerate.c"))
					printf("无法删除中间生成数组\n");
				else
					printf("中间数组已删除\n");
				
				printf("自动生成完成!\n");
				while(1);
			}
		}
	}
	
      
}