#pragma once

#ifndef __MYZLIB_H
#define __MYZLIB_H

#include "zlib.h"
#pragma comment(lib, "ZLib.lib")

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int Myzlib_Compress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);//通过zlib压缩
int Myzlib_Uncompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);//通过zlib解压缩
void Zlib_File_Compress(char* Origin_filename, char* Zlib_Compress_filename);//文件zlib算法压缩
void Zlib_File_Uncompress(char* Origin_filename, char* Zlib_Compress_filename);//文件zlib算法解压缩，解压一个非Zlib_File_Compress函数压缩的任意文件会导致一个未处理的异常，程序崩溃，此bug尚未解决
long MyGetFileSize(const char* filename);//获取文件大小，调用此函数时此文件必须处于未打开读写状态

#endif