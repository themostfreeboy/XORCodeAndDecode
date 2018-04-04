#include "Myzlib.h"

int Myzlib_Compress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen)//通过zlib压缩
{
	int err;
	err = compress(dest, destLen, source, sourceLen);
	if (err != Z_OK)//压缩出错
	{
		printf("\nzlib compress error!\n");
		return 0;//压缩出错
	}
	return 1;//压缩成功
}

int Myzlib_Uncompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen)//通过zlib解压缩
{
	int err;
	err = uncompress(dest, destLen, source, sourceLen);
	if (err != Z_OK)//解压缩出错
	{
		printf("\nzlib uncompress error!\n");
		return 0;//解压缩出错
	}
	return 1;//解压缩成功
}

void Zlib_File_Compress(char* Origin_filename, char* Zlib_Compress_filename)//文件zlib算法压缩
{
	long filesize=MyGetFileSize(Origin_filename);//获取文件大小，调用此函数时此文件必须处于未打开读写状态
	errno_t err_read, err_write;
	FILE *fp_read = NULL;
	FILE *fp_write = NULL;
	err_read = fopen_s(&fp_read, Origin_filename, "rb");
	err_write = fopen_s(&fp_write, Zlib_Compress_filename, "wb");
	if (err_read != 0)
	{
		return;//文件打开失败
	}
	if (err_write != 0)
	{
		return;//文件打开失败
	}

	Bytef *Origin_data;
    Origin_data=(Bytef*)malloc(sizeof(Bytef)*filesize*10);//由于静态数组分配大小受系统编译器的堆栈影响，无法分配得过大，否则会造成堆栈溢出，所以采用malloc动态分配内存的方式

	Bytef *Zlib_Compress_data;
    Zlib_Compress_data=(Bytef*)malloc(sizeof(Bytef)*filesize*10);

	if(Origin_data==NULL || Zlib_Compress_data==NULL==NULL)//malloc尚未申请到足够的内存区域
	{
		printf("\n内存不足，正在整理内存碎片中...\n");
		while(Origin_data==NULL || Zlib_Compress_data==NULL);//一直等待
		printf("\n内存整理完毕\n");
	}

	uLong compress_length;
	long i=0;
	while (fscanf_s(fp_read, "%c", &(Origin_data[i])) != EOF)
	{
		i++;
	}
	Myzlib_Compress(Zlib_Compress_data, &compress_length, (const Bytef*)Origin_data, i);//通过zlib压缩
	for(i=0;i<compress_length;i++)
	{
		fprintf_s(fp_write, "%c", Zlib_Compress_data[i]);
	}

	fclose(fp_read);
	fclose(fp_write);

	free(Origin_data);
	free(Zlib_Compress_data);
}

void Zlib_File_Uncompress(char* Origin_filename, char* Zlib_Compress_filename)//文件zlib算法解压缩
{
	long filesize=MyGetFileSize(Origin_filename);//获取文件大小，调用此函数时此文件必须处于未打开读写状态
	errno_t err_read, err_write;
	FILE *fp_read = NULL;
	FILE *fp_write = NULL;
	err_read = fopen_s(&fp_read, Origin_filename, "rb");
	err_write = fopen_s(&fp_write, Zlib_Compress_filename, "wb");
	if (err_read != 0)
	{
		return;//文件打开失败
	}
	if (err_write != 0)
	{
		return;//文件打开失败
	}

	Bytef *Origin_data;
    Origin_data=(Bytef*)malloc(sizeof(Bytef)*filesize*10);//由于静态数组分配大小受系统编译器的堆栈影响，无法分配得过大，否则会造成堆栈溢出，所以采用malloc动态分配内存的方式

	Bytef *Zlib_Uncompress_data;
    Zlib_Uncompress_data=(Bytef*)malloc(sizeof(Bytef)*filesize*10);

	if(Origin_data==NULL || Zlib_Uncompress_data==NULL)//malloc尚未申请到足够的内存区域
	{
		printf("\n内存不足，正在整理内存碎片中...\n");
		while(Origin_data==NULL || Zlib_Uncompress_data==NULL);//一直等待
		printf("\n内存整理完毕\n");
	}

	uLong uncompress_length;
	long i=0;
	while (fscanf_s(fp_read, "%c", &(Origin_data[i])) != EOF)
	{
		i++;
	}
	Myzlib_Uncompress(Zlib_Uncompress_data, &uncompress_length, (const Bytef*)Origin_data, i);//文件zlib算法解压缩，解压一个非Zlib_File_Compress函数压缩的任意文件会导致一个未处理的异常，程序崩溃，此bug尚未解决
	for(i=0;i<uncompress_length;i++)
	{
		fprintf_s(fp_write, "%c", Zlib_Uncompress_data[i]);
	}

	fclose(fp_read);
	fclose(fp_write);

	free(Origin_data);
	free(Zlib_Uncompress_data);
}

long MyGetFileSize(const char* filename)//获取文件大小，调用此函数时此文件必须处于未打开读写状态
{
	FILE *fp = NULL;
	errno_t err;
	err = fopen_s(&fp, filename, "rb");
	if (err != 0)
	{
		return -1;//文件打开失败
	}
    fseek(fp,0,SEEK_SET);
    fseek(fp,0,SEEK_END);
    long filesize=ftell(fp);
    rewind(fp);
	fclose(fp);
	return filesize;
}

