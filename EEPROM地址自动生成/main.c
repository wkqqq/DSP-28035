#include "stdio.h"
#include "string.h"
#include "time.h"

#define MAXINDEX 2000
unsigned int NumIndex[MAXINDEX];
enum ENUM_STATUS
{
    LINE_START,
    NUM_START, //��Ҫ��ȡ�����ֵĿ�ͷ
    NUM_END,   //��Ҫ��ȡ�����ֵĽ�β
    LINE_END,
    FILE_END    //�ļ������
};
enum ENUM_STATUS fp_Status = LINE_START;
    
unsigned int LineCount = 0; //ÿһ�еĶ��ż�������������0
unsigned int num = 0;
unsigned int displaycount = 0;
void main(void)
{
    FILE *fp_r,*fp_w;
    char m_temp;
	unsigned int tempChange,i;
	time_t t;
    if((fp_r = fopen("f_funcCode.c","r")) == NULL)    //��Ҫ��ȡ���ļ�
	{
		printf("The file can not be opened.\n");
		while(1);//���������ִ��
	}
    if((fp_w = fopen("AutoGenerate.c","w+")) == NULL) //��д����ļ�
	{
		printf("The file can not be created.\n");
		while(1);//���������ִ��
	}
    while(1)
    {
		if(++displaycount > 10)
	   {
		   displaycount = 0;
		   m_temp =  fgetc(fp_r); //��ȡһ���ַ�
		   
		   if(m_temp == ',')
		   {
				LineCount++;
		   }
		   else if(m_temp == '\n')
		   {
				fp_Status = LINE_START;
				LineCount = 0;
		   }

		   if(LineCount == 4)   //ÿһ�е�4�����ź͵���������е�����Ϊ��Ҫ��eeprom��ַ
		   {
				fp_Status == NUM_START;
				m_temp =  fgetc(fp_r); //�ȶ�ȡ','���һ���ַ�
				while(m_temp != ',')
				{
					if(m_temp == ' ')
					{}
					else
					{
						fputc(m_temp,fp_w); //д�뵽�ļ���
					}
					m_temp =  fgetc(fp_r); //��ȡһ���ַ�
				}
				//fputc(',',fp_w);
			   // fprintf(fp_w,"  // %d",num);
				fputc('\n',fp_w);
				fp_Status == NUM_END;
				num++;
				LineCount = 0;
		   }

		   if(m_temp == EOF)    //�����������ļ�
		   {
				fp_Status = FILE_END;
				printf("�������!\n");
				fclose(fp_r);
				fclose(fp_w);	//�ȹر�f_funcCode.c����ȡ��eeprom��ַ���м��ļ�
				
				if((fp_r = fopen("AutoGenerate.c","r")) == NULL)    //��Ҫ��ȡ���ļ�
				{
					printf("The file can not be opened.\n");
					while(1);//���������ִ��
				}
				if((fp_w = fopen("f_table_eeprom2Fc.c","w+")) == NULL) //��д����ļ�
				{
					printf("The file can not be created.\n");
					while(1);//���������ִ��
				}
				time(&t);	//��ȡ��ǰʱ��
				fprintf(fp_w,"// C�����Զ�����eeprom����-by wk\n");
				fprintf(fp_w,"// ʱ���:%s\n",ctime(&t));	// ctime(&t)������תΪ�ַ�������ӡ
				fprintf(fp_w,"const Uint16 eeprom2Fc[] =\n{\n//index eeprom��ַ\n");	//�����ʽ
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
					printf("�޷�ɾ���м���������\n");
				else
					printf("�м�������ɾ��\n");
				
				printf("�Զ��������!\n");
				while(1);
			}
		}
	}
	
      
}