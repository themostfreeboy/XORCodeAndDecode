#pragma once

#ifndef __MYZLIB_H
#define __MYZLIB_H

#include "zlib.h"
#pragma comment(lib, "ZLib.lib")

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int Myzlib_Compress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);//ͨ��zlibѹ��
int Myzlib_Uncompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);//ͨ��zlib��ѹ��
void Zlib_File_Compress(char* Origin_filename, char* Zlib_Compress_filename);//�ļ�zlib�㷨ѹ��
void Zlib_File_Uncompress(char* Origin_filename, char* Zlib_Compress_filename);//�ļ�zlib�㷨��ѹ������ѹһ����Zlib_File_Compress����ѹ���������ļ��ᵼ��һ��δ������쳣�������������bug��δ���
long MyGetFileSize(const char* filename);//��ȡ�ļ���С�����ô˺���ʱ���ļ����봦��δ�򿪶�д״̬

#endif