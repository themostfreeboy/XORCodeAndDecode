#include "Myzlib.h"

int Myzlib_Compress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen)//ͨ��zlibѹ��
{
	int err;
	err = compress(dest, destLen, source, sourceLen);
	if (err != Z_OK)//ѹ������
	{
		printf("\nzlib compress error!\n");
		return 0;//ѹ������
	}
	return 1;//ѹ���ɹ�
}

int Myzlib_Uncompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen)//ͨ��zlib��ѹ��
{
	int err;
	err = uncompress(dest, destLen, source, sourceLen);
	if (err != Z_OK)//��ѹ������
	{
		printf("\nzlib uncompress error!\n");
		return 0;//��ѹ������
	}
	return 1;//��ѹ���ɹ�
}

void Zlib_File_Compress(char* Origin_filename, char* Zlib_Compress_filename)//�ļ�zlib�㷨ѹ��
{
	long filesize=MyGetFileSize(Origin_filename);//��ȡ�ļ���С�����ô˺���ʱ���ļ����봦��δ�򿪶�д״̬
	errno_t err_read, err_write;
	FILE *fp_read = NULL;
	FILE *fp_write = NULL;
	err_read = fopen_s(&fp_read, Origin_filename, "rb");
	err_write = fopen_s(&fp_write, Zlib_Compress_filename, "wb");
	if (err_read != 0)
	{
		return;//�ļ���ʧ��
	}
	if (err_write != 0)
	{
		return;//�ļ���ʧ��
	}

	Bytef *Origin_data;
    Origin_data=(Bytef*)malloc(sizeof(Bytef)*filesize*10);//���ھ�̬��������С��ϵͳ�������Ķ�ջӰ�죬�޷�����ù��󣬷������ɶ�ջ��������Բ���malloc��̬�����ڴ�ķ�ʽ

	Bytef *Zlib_Compress_data;
    Zlib_Compress_data=(Bytef*)malloc(sizeof(Bytef)*filesize*10);

	if(Origin_data==NULL || Zlib_Compress_data==NULL==NULL)//malloc��δ���뵽�㹻���ڴ�����
	{
		printf("\n�ڴ治�㣬���������ڴ���Ƭ��...\n");
		while(Origin_data==NULL || Zlib_Compress_data==NULL);//һֱ�ȴ�
		printf("\n�ڴ��������\n");
	}

	uLong compress_length;
	long i=0;
	while (fscanf_s(fp_read, "%c", &(Origin_data[i])) != EOF)
	{
		i++;
	}
	Myzlib_Compress(Zlib_Compress_data, &compress_length, (const Bytef*)Origin_data, i);//ͨ��zlibѹ��
	for(i=0;i<compress_length;i++)
	{
		fprintf_s(fp_write, "%c", Zlib_Compress_data[i]);
	}

	fclose(fp_read);
	fclose(fp_write);

	free(Origin_data);
	free(Zlib_Compress_data);
}

void Zlib_File_Uncompress(char* Origin_filename, char* Zlib_Compress_filename)//�ļ�zlib�㷨��ѹ��
{
	long filesize=MyGetFileSize(Origin_filename);//��ȡ�ļ���С�����ô˺���ʱ���ļ����봦��δ�򿪶�д״̬
	errno_t err_read, err_write;
	FILE *fp_read = NULL;
	FILE *fp_write = NULL;
	err_read = fopen_s(&fp_read, Origin_filename, "rb");
	err_write = fopen_s(&fp_write, Zlib_Compress_filename, "wb");
	if (err_read != 0)
	{
		return;//�ļ���ʧ��
	}
	if (err_write != 0)
	{
		return;//�ļ���ʧ��
	}

	Bytef *Origin_data;
    Origin_data=(Bytef*)malloc(sizeof(Bytef)*filesize*10);//���ھ�̬��������С��ϵͳ�������Ķ�ջӰ�죬�޷�����ù��󣬷������ɶ�ջ��������Բ���malloc��̬�����ڴ�ķ�ʽ

	Bytef *Zlib_Uncompress_data;
    Zlib_Uncompress_data=(Bytef*)malloc(sizeof(Bytef)*filesize*10);

	if(Origin_data==NULL || Zlib_Uncompress_data==NULL)//malloc��δ���뵽�㹻���ڴ�����
	{
		printf("\n�ڴ治�㣬���������ڴ���Ƭ��...\n");
		while(Origin_data==NULL || Zlib_Uncompress_data==NULL);//һֱ�ȴ�
		printf("\n�ڴ��������\n");
	}

	uLong uncompress_length;
	long i=0;
	while (fscanf_s(fp_read, "%c", &(Origin_data[i])) != EOF)
	{
		i++;
	}
	Myzlib_Uncompress(Zlib_Uncompress_data, &uncompress_length, (const Bytef*)Origin_data, i);//�ļ�zlib�㷨��ѹ������ѹһ����Zlib_File_Compress����ѹ���������ļ��ᵼ��һ��δ������쳣�������������bug��δ���
	for(i=0;i<uncompress_length;i++)
	{
		fprintf_s(fp_write, "%c", Zlib_Uncompress_data[i]);
	}

	fclose(fp_read);
	fclose(fp_write);

	free(Origin_data);
	free(Zlib_Uncompress_data);
}

long MyGetFileSize(const char* filename)//��ȡ�ļ���С�����ô˺���ʱ���ļ����봦��δ�򿪶�д״̬
{
	FILE *fp = NULL;
	errno_t err;
	err = fopen_s(&fp, filename, "rb");
	if (err != 0)
	{
		return -1;//�ļ���ʧ��
	}
    fseek(fp,0,SEEK_SET);
    fseek(fp,0,SEEK_END);
    long filesize=ftell(fp);
    rewind(fp);
	fclose(fp);
	return filesize;
}

