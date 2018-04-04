#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "md5.h"
#include "md5_file.h"
#include "Myzlib.h"
#include "MyMFC.h"

int main()
{
	printf("��ΪJXL��д�ļ��ܽ��ܳ���\n����������1��\n����������2��\n");
	int choice=0;
	scanf("%d",&choice);
	if(choice==1)//����
	{
		printf("�뽫�����ܵ��ļ�ֱ����ק���˿���̨��\n");
		char origin_pathname[100];
		char origin_filename[100];
		char XOR_Code_filename_temp[100];
		char XOR_Code_filename[100];
		char zlib_Compress_filename[100];
		char code[100];
		unsigned char code_modified;
		unsigned char code_md5[16];
		unsigned char file_md5[16];
	    scanf_s("%s",origin_pathname,100);
		printf("�������������\n");
		scanf_s("%s",code,100);
		system("cls");//��������
		printf("���ڼ�����...\n");
		MyCreatDirectory(_T(".\\cache"));//�ڵ�ǰĿ¼���½�һ����ʱ�ļ���cache
		strcpy(origin_filename,MyGetFileName(origin_pathname));//��·���л�ȡ�ļ���
		sprintf(XOR_Code_filename_temp,".\\cache\\%s",origin_filename);
		strcpy(XOR_Code_filename,MyAddFileName(XOR_Code_filename_temp,"(�������ʱ�ļ�)"));
		strcpy(zlib_Compress_filename,MyAddFileName(origin_pathname,"(�Ѽ���)"));
		md5_file_calc(origin_pathname, file_md5);//�����ļ�md5
		MD5Calc((unsigned char*)code, code_md5);//�����������md5
		//����code_modified���ֵ
		code_modified=0;
		for(int i=0;i<16;i++)
		{
			code_modified+=code_md5[i];
			code_modified%=256;
		}
		errno_t err_read, err_write;
	    FILE *fp_read = NULL;
	    FILE *fp_write = NULL;
	    err_read = fopen_s(&fp_read, origin_pathname, "rb");
	    err_write = fopen_s(&fp_write, XOR_Code_filename, "wb");
		for(int i=0;i<16;i++)
		{
			fprintf_s(fp_write,"%c",file_md5[i]);
		}
		char temp;
	    while(fscanf_s(fp_read, "%c", &temp) != EOF)
		{
			temp=temp^code_modified;//��λ���
			fprintf_s(fp_write,"%c",temp);
		}
		fclose(fp_read);
		fclose(fp_write);
		Zlib_File_Compress(XOR_Code_filename,zlib_Compress_filename);//zlib�㷨ѹ��
		DeleteFile(MyCharToLPCTSTR(XOR_Code_filename));//ɾ����λ�����ܺ�δ����zlibѹ��ʱ��������ʱ�ļ�
		RemoveDirectory(_T(".\\cache"));//ɾ����������ʱ�ļ���cache
		printf("�������!\n��������˳�\n");
		getch();
		return 0;
	}
	else if(choice==2)//����
	{
		printf("�뽫�����ܵ��ļ�ֱ����ק���˿���̨��\n");
		char origin_pathname[100];
		char origin_filename[100];
		char zlib_Uncompress_filename_temp[100];
		char zlib_Uncompress_filename[100];
		char XOR_Decode_filename[100];
		char code[100];
		unsigned char code_modified;
		unsigned char code_md5[16];
		unsigned char file_md5[16];
		unsigned char file_md5_get[16];
	    scanf_s("%s",origin_pathname,100);
		printf("�������������\n");
		scanf_s("%s",code,100);
		system("cls");//��������
		printf("���ڽ�����...\n");
		MyCreatDirectory(_T(".\\cache"));//�ڵ�ǰĿ¼���½�һ����ʱ�ļ���cache
		strcpy(origin_filename,MyGetFileName(origin_pathname));//��·���л�ȡ�ļ���
		sprintf(zlib_Uncompress_filename_temp,".\\cache\\%s",origin_filename);
		strcpy(zlib_Uncompress_filename,MyAddFileName(zlib_Uncompress_filename_temp,"(�������ʱ�ļ�)"));
		strcpy(XOR_Decode_filename,MyAddFileName(origin_pathname,"(�ѽ���)"));
		MD5Calc((unsigned char*)code, code_md5);//�����������md5
		//����code_modified���ֵ
		code_modified=0;
		for(int i=0;i<16;i++)
		{
			code_modified+=code_md5[i];
			code_modified%=256;
		}
		Zlib_File_Uncompress(origin_pathname,zlib_Uncompress_filename);//zlib�㷨��ѹ��
		errno_t err_read, err_write;
	    FILE *fp_read = NULL;
	    FILE *fp_write = NULL;
	    err_read = fopen_s(&fp_read, zlib_Uncompress_filename, "rb");
	    err_write = fopen_s(&fp_write, XOR_Decode_filename, "wb");
		//��ȡԭ�ļ���md5ֵ
		for(int i=0;i<16;i++)
		{
			fscanf_s(fp_read,"%c",&(file_md5[i]));
		}
		char temp;
	    while(fscanf_s(fp_read, "%c", &temp) != EOF)
		{
			temp=temp^code_modified;//��λ���
			fprintf_s(fp_write,"%c",temp);
		}
		fclose(fp_read);
		fclose(fp_write);
		DeleteFile(MyCharToLPCTSTR(zlib_Uncompress_filename));//ɾ��zlib��ѹ����δ���а�λ������ʱ��������ʱ�ļ�
		//У����ܺ��ļ��Ƿ���ȷ
		md5_file_calc(XOR_Decode_filename, file_md5_get);//�����ļ�md5
		for(int i=0;i<16;i++)
		{
			if(file_md5[i]!=file_md5_get[i])//md5��һ��
			{
				DeleteFile(MyCharToLPCTSTR(XOR_Decode_filename));//ɾ����λ�����ܵ������ļ�
				RemoveDirectory(_T(".\\cache"));//ɾ����������ʱ�ļ���cache
				printf("�����������!\n��������˳�\n");
				getch();
				return 0;
			}
		}
		RemoveDirectory(_T(".\\cache"));//ɾ����������ʱ�ļ���cache
		printf("�������!\n��������˳�\n");
		getch();
		return 0;
	}
	else if(choice==706)//�����ƽ�ģʽ
	{
		system("cls");//��������
		printf("��ΪJXLר�õı����ƽ�ģʽ���뽫�����ܵ��ļ�ֱ����ק���˿���̨��\n");
		char origin_pathname[100];
		char origin_filename[100];
		char zlib_Uncompress_filename_temp[100];
		char zlib_Uncompress_filename[100];
		char XOR_Decode_filename[100];
		char XOR_Decode_filename_force[100];
		unsigned char code_modified;
		unsigned char code_md5[16];
		unsigned char file_md5[16];
		unsigned char file_md5_get[16];
	    scanf_s("%s",origin_pathname,100);
		system("cls");//��������
		printf("���ڽ���JXLר�õı����ƽ�ģʽ�Ľ�����...\n");
		MyCreatDirectory(_T(".\\cache"));//�ڵ�ǰĿ¼���½�һ����ʱ�ļ���cache
		strcpy(origin_filename,MyGetFileName(origin_pathname));//��·���л�ȡ�ļ���
		sprintf(zlib_Uncompress_filename_temp,".\\cache\\%s",origin_filename);
		strcpy(zlib_Uncompress_filename,MyAddFileName(zlib_Uncompress_filename_temp,"(�������ʱ�ļ�)"));
		strcpy(XOR_Decode_filename,MyAddFileName(origin_pathname,"(�ѽ���)"));
		Zlib_File_Uncompress(origin_pathname,zlib_Uncompress_filename);//zlib�㷨��ѹ��
		errno_t err_read, err_write;
	    FILE *fp_read = NULL;
	    FILE *fp_write = NULL;
	    err_read = fopen_s(&fp_read, zlib_Uncompress_filename, "rb");
		//��ȡԭ�ļ���md5ֵ
		for(int i=0;i<16;i++)
		{
			fscanf_s(fp_read,"%c",&(file_md5[i]));
		}
		fclose(fp_read);
		char temp;
		int tag=0;
		for(int code_modified=0;code_modified<256;code_modified++)//��ʼJXLר�ø��ı����ƽ�ģʽ
		{
			char temp_filename_number[100];
			sprintf(temp_filename_number,"%03d",code_modified);
			strcpy(XOR_Decode_filename_force,MyAddFileName(zlib_Uncompress_filename_temp,temp_filename_number));
			err_read = fopen_s(&fp_read, zlib_Uncompress_filename, "rb");
			err_write = fopen_s(&fp_write, XOR_Decode_filename_force, "wb");
			fseek(fp_read,16,SEEK_SET);
			while(fscanf_s(fp_read, "%c", &temp) != EOF)
		    {
			    temp=temp^(code_modified & 0xff);//��λ���
			    fprintf_s(fp_write,"%c",temp);
		    }
            fclose(fp_read);
			fclose(fp_write);
			//У����ܺ��ļ��Ƿ���ȷ
		    md5_file_calc(XOR_Decode_filename_force, file_md5_get);//�����ļ�md5
			tag=0;
		    for(int i=0;i<16;i++)
		    {
			    if(file_md5[i]!=file_md5_get[i])//md5��һ��
			    {
				    DeleteFile(MyCharToLPCTSTR(XOR_Decode_filename_force));//ɾ����λ�����ܵ������ļ�
					tag=1;
					break;
			    }
		    }
			if(tag==0)//�����ƽ�ģʽ�����ļ��ɹ�
			{
				DeleteFile(MyCharToLPCTSTR(zlib_Uncompress_filename));//ɾ��zlib��ѹ����δ���а�λ������ʱ��������ʱ�ļ�
				err_read = fopen_s(&fp_read, XOR_Decode_filename_force, "rb");
				err_write = fopen_s(&fp_write, XOR_Decode_filename, "wb");
				while(fscanf_s(fp_read, "%c", &temp) != EOF)
				{
					fprintf_s(fp_write,"%c",temp);
				}
				fclose(fp_read);
				fclose(fp_write);
				DeleteFile(MyCharToLPCTSTR(XOR_Decode_filename_force));//ɾ�������ƽ�ģʽ�½���ʱ��������ʱ�ļ�
				RemoveDirectory(_T(".\\cache"));//ɾ����������ʱ�ļ���cache
				printf("JXLר�õı����ƽ�ģʽ���ܳɹ�!\n��������˳�\n");
		        getch();
				return 0;
			}
		}
		DeleteFile(MyCharToLPCTSTR(zlib_Uncompress_filename));//ɾ��zlib��ѹ����δ���а�λ������ʱ��������ʱ�ļ�
		RemoveDirectory(_T(".\\cache"));//ɾ����������ʱ�ļ���cache
		printf("JXLר�õı����ƽ�ģʽ����ʧ��!\n��������˳�\n");
		getch();
		return 0;
	}
	else//��������
	{
		printf("��������������󣬰�������˳���\n");
		getch();
		return 0;
	}
	return 0;
}