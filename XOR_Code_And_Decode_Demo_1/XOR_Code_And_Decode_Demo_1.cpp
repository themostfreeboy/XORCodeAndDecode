#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "md5.h"
#include "md5_file.h"
#include "Myzlib.h"
#include "MyMFC.h"

int main()
{
	printf("此为JXL编写的加密解密程序：\n加密请输入1；\n解密请输入2。\n");
	int choice=0;
	scanf("%d",&choice);
	if(choice==1)//加密
	{
		printf("请将待加密的文件直接拖拽到此控制台内\n");
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
		printf("请输入加密密码\n");
		scanf_s("%s",code,100);
		system("cls");//清屏函数
		printf("正在加密中...\n");
		MyCreatDirectory(_T(".\\cache"));//在当前目录下新建一个临时文件夹cache
		strcpy(origin_filename,MyGetFileName(origin_pathname));//从路径中获取文件名
		sprintf(XOR_Code_filename_temp,".\\cache\\%s",origin_filename);
		strcpy(XOR_Code_filename,MyAddFileName(XOR_Code_filename_temp,"(半加密临时文件)"));
		strcpy(zlib_Compress_filename,MyAddFileName(origin_pathname,"(已加密)"));
		md5_file_calc(origin_pathname, file_md5);//计算文件md5
		MD5Calc((unsigned char*)code, code_md5);//计算加密密码md5
		//计算code_modified异或值
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
			temp=temp^code_modified;//按位异或
			fprintf_s(fp_write,"%c",temp);
		}
		fclose(fp_read);
		fclose(fp_write);
		Zlib_File_Compress(XOR_Code_filename,zlib_Compress_filename);//zlib算法压缩
		DeleteFile(MyCharToLPCTSTR(XOR_Code_filename));//删除按位异或加密后但未进行zlib压缩时产生的临时文件
		RemoveDirectory(_T(".\\cache"));//删除产生的临时文件夹cache
		printf("加密完成!\n按任意键退出\n");
		getch();
		return 0;
	}
	else if(choice==2)//解密
	{
		printf("请将待解密的文件直接拖拽到此控制台内\n");
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
		printf("请输入解密密码\n");
		scanf_s("%s",code,100);
		system("cls");//清屏函数
		printf("正在解密中...\n");
		MyCreatDirectory(_T(".\\cache"));//在当前目录下新建一个临时文件夹cache
		strcpy(origin_filename,MyGetFileName(origin_pathname));//从路径中获取文件名
		sprintf(zlib_Uncompress_filename_temp,".\\cache\\%s",origin_filename);
		strcpy(zlib_Uncompress_filename,MyAddFileName(zlib_Uncompress_filename_temp,"(半解密临时文件)"));
		strcpy(XOR_Decode_filename,MyAddFileName(origin_pathname,"(已解密)"));
		MD5Calc((unsigned char*)code, code_md5);//计算解密密码md5
		//计算code_modified异或值
		code_modified=0;
		for(int i=0;i<16;i++)
		{
			code_modified+=code_md5[i];
			code_modified%=256;
		}
		Zlib_File_Uncompress(origin_pathname,zlib_Uncompress_filename);//zlib算法解压缩
		errno_t err_read, err_write;
	    FILE *fp_read = NULL;
	    FILE *fp_write = NULL;
	    err_read = fopen_s(&fp_read, zlib_Uncompress_filename, "rb");
	    err_write = fopen_s(&fp_write, XOR_Decode_filename, "wb");
		//获取原文件的md5值
		for(int i=0;i<16;i++)
		{
			fscanf_s(fp_read,"%c",&(file_md5[i]));
		}
		char temp;
	    while(fscanf_s(fp_read, "%c", &temp) != EOF)
		{
			temp=temp^code_modified;//按位异或
			fprintf_s(fp_write,"%c",temp);
		}
		fclose(fp_read);
		fclose(fp_write);
		DeleteFile(MyCharToLPCTSTR(zlib_Uncompress_filename));//删除zlib解压缩后但未进行按位异或解密时产生的临时文件
		//校检解密后文件是否正确
		md5_file_calc(XOR_Decode_filename, file_md5_get);//计算文件md5
		for(int i=0;i<16;i++)
		{
			if(file_md5[i]!=file_md5_get[i])//md5不一致
			{
				DeleteFile(MyCharToLPCTSTR(XOR_Decode_filename));//删除按位异或解密的最终文件
				RemoveDirectory(_T(".\\cache"));//删除产生的临时文件夹cache
				printf("解密密码错误!\n按任意键退出\n");
				getch();
				return 0;
			}
		}
		RemoveDirectory(_T(".\\cache"));//删除产生的临时文件夹cache
		printf("解密完成!\n按任意键退出\n");
		getch();
		return 0;
	}
	else if(choice==706)//暴力破解模式
	{
		system("cls");//清屏函数
		printf("此为JXL专用的暴力破解模式，请将待解密的文件直接拖拽到此控制台内\n");
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
		system("cls");//清屏函数
		printf("正在进行JXL专用的暴力破解模式的解密中...\n");
		MyCreatDirectory(_T(".\\cache"));//在当前目录下新建一个临时文件夹cache
		strcpy(origin_filename,MyGetFileName(origin_pathname));//从路径中获取文件名
		sprintf(zlib_Uncompress_filename_temp,".\\cache\\%s",origin_filename);
		strcpy(zlib_Uncompress_filename,MyAddFileName(zlib_Uncompress_filename_temp,"(半解密临时文件)"));
		strcpy(XOR_Decode_filename,MyAddFileName(origin_pathname,"(已解密)"));
		Zlib_File_Uncompress(origin_pathname,zlib_Uncompress_filename);//zlib算法解压缩
		errno_t err_read, err_write;
	    FILE *fp_read = NULL;
	    FILE *fp_write = NULL;
	    err_read = fopen_s(&fp_read, zlib_Uncompress_filename, "rb");
		//获取原文件的md5值
		for(int i=0;i<16;i++)
		{
			fscanf_s(fp_read,"%c",&(file_md5[i]));
		}
		fclose(fp_read);
		char temp;
		int tag=0;
		for(int code_modified=0;code_modified<256;code_modified++)//开始JXL专用给的暴力破解模式
		{
			char temp_filename_number[100];
			sprintf(temp_filename_number,"%03d",code_modified);
			strcpy(XOR_Decode_filename_force,MyAddFileName(zlib_Uncompress_filename_temp,temp_filename_number));
			err_read = fopen_s(&fp_read, zlib_Uncompress_filename, "rb");
			err_write = fopen_s(&fp_write, XOR_Decode_filename_force, "wb");
			fseek(fp_read,16,SEEK_SET);
			while(fscanf_s(fp_read, "%c", &temp) != EOF)
		    {
			    temp=temp^(code_modified & 0xff);//按位异或
			    fprintf_s(fp_write,"%c",temp);
		    }
            fclose(fp_read);
			fclose(fp_write);
			//校检解密后文件是否正确
		    md5_file_calc(XOR_Decode_filename_force, file_md5_get);//计算文件md5
			tag=0;
		    for(int i=0;i<16;i++)
		    {
			    if(file_md5[i]!=file_md5_get[i])//md5不一致
			    {
				    DeleteFile(MyCharToLPCTSTR(XOR_Decode_filename_force));//删除按位异或解密的最终文件
					tag=1;
					break;
			    }
		    }
			if(tag==0)//暴力破解模式解密文件成功
			{
				DeleteFile(MyCharToLPCTSTR(zlib_Uncompress_filename));//删除zlib解压缩后但未进行按位异或解密时产生的临时文件
				err_read = fopen_s(&fp_read, XOR_Decode_filename_force, "rb");
				err_write = fopen_s(&fp_write, XOR_Decode_filename, "wb");
				while(fscanf_s(fp_read, "%c", &temp) != EOF)
				{
					fprintf_s(fp_write,"%c",temp);
				}
				fclose(fp_read);
				fclose(fp_write);
				DeleteFile(MyCharToLPCTSTR(XOR_Decode_filename_force));//删除暴力破解模式下解密时产生的临时文件
				RemoveDirectory(_T(".\\cache"));//删除产生的临时文件夹cache
				printf("JXL专用的暴力破解模式解密成功!\n按任意键退出\n");
		        getch();
				return 0;
			}
		}
		DeleteFile(MyCharToLPCTSTR(zlib_Uncompress_filename));//删除zlib解压缩后但未进行按位异或解密时产生的临时文件
		RemoveDirectory(_T(".\\cache"));//删除产生的临时文件夹cache
		printf("JXL专用的暴力破解模式解密失败!\n按任意键退出\n");
		getch();
		return 0;
	}
	else//输入有误
	{
		printf("你输入的数据有误，按任意键退出。\n");
		getch();
		return 0;
	}
	return 0;
}