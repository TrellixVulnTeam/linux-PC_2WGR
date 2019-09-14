#include <stdio.h> //��Ҫ�������õ�����/�������
#include <string.h> //��Ҫ�������õ��ַ�����������
#include <stdlib.h> //�ַ���ת���������ڴ���亯�������̿��ƺ���

//���������⺯����ʹ��֮һ����ɿ⺯���ĵ���
#include <windows.h>//�����ǽ��������������Ļ�� ��ӡ ������
//#include <conio.h>//�ַ������롢����������ַ�������������ɫ���ú���

/*****Huffman�ṹ����**********/
typedef struct node 
{ 
    long    w;//wΪȨֵ
    short   p,l,r; //pΪparent��lΪ���ӣ�rΪ�Һ���
}htnode,*htnp;//��̬��������洢��������

/*****Huffman�����ṹ����*****/
typedef struct huffman_code 
{ 
    unsigned char len;//����
    unsigned char  *codestr; 
}hufcode;

typedef char **huffmancode;//��̬��������洢�����������


/*********��������*********/
int initial_files(char *source_filename,FILE **inp,char *obj_filename,FILE **outp);
//1.��ʼ���ļ���
char *create_filename(char *source_filename,char* obj_filename);
//2.�����ļ���
int compress(char *source_filename,char *obj_filename);
//3.ѹ���ļ�
long frequency_data(FILE *in,long fre[]);
//4.Ƶ������
int search_set(htnp ht,int n,int *s1, int *s2);
//5.ѡȡ���Ȩֵ��С����
int create_hftree(long w[],int n,htnode ht[]);
//6.������������
int encode_hftree(htnp htp,int n,hufcode hc[]);
//7.�����������
unsigned char chars_to_bits(const unsigned char chars[8]);
//8.���ַ����ö��������ֱ�ʾ������λ�����֪ʶ
int write_compress_file(FILE *in,FILE *out,htnp ht,hufcode hc[],char* source_filename,long source_filesize);
//9.д��ѹ���ļ�
int decompress(char *source_filename,char *obj_filename);
//10.��ѹ��
void get_mini_huffmantree(FILE* in,short mini_ht[][2]);
//11.��ѹ����΢��������
int write_decompress_file(FILE *in,FILE* out,short mini_ht[][2],long bits_pos,long obj_filesize);
//12.д���ѹ��
int d_initial_files(char *source_filename,FILE **inp,char *obj_filename,FILE **outp);
//13.���±��ԭʼ�ļ�
/*�˵�����*/
void main()
{
	int s;
	char filename[10];
	//system("color 3F");//���ñ�����ɫ 1.����ɫ 2.ī��ɫ 3.����ɫ4.�غ�ɫ 5.��ɫ6.��ɫ7.��ɫ
	    printf("        ****************************************************\n");
		printf("                    *           �˵���            *\n");
		printf("                    *   1.------ѹ��--------      *\n");
		printf("                    *   2.------��ѹ��------      *\n");
		printf("                    *   0.------�˳�--------      *\n");
		printf("        ****************************************************\n");
	scanf("%d",&s);
	while(s!=0)
	{
		getchar();
		switch(s)
		{
			case 1:
				puts("�������ѹ���ļ�·����");
				gets(filename);
				compress(filename,NULL);
				break;
			case 2:
				puts("���������ѹ�ļ�·����");
				gets(filename);
				decompress(filename,NULL);
				break;
			default : 
				printf("ָ���������������ָ�\n");
		}
		puts(" ");
		printf("        ****************************************************\n");
		printf("                    *           �˵���              *\n");
		printf("                    *   1.------ѹ��--------         *\n");
		printf("                    *   2.------��ѹ��------        *\n");
		printf("                    *   0.------�˳�--------          *\n");
		printf("        ****************************************************\n");
		scanf("%d",&s);
	}
}

                                       //FILE*����ָ���ļ���ָ����� inp,  outp
int initial_files(char *source_filename,FILE **inp,char *obj_filename,FILE **outp) //1.��ʼ���ļ�
{                                     
    if(fopen(source_filename,"rb")==NULL) //��fopen�������򿪴�ѹ���ļ�
    { 
        return -1;//��-1���
    } 

    if(obj_filename==NULL) //ѹ���ļ�Ϊ��
    { 
        if((obj_filename=(char*)malloc(256*sizeof(char)))==NULL) //����ռ�
        { 
            return -1;
        }                     
        create_filename(source_filename,obj_filename);    // ���ú���create_filename(source_filename,obj_filename);
    } 
    if(strcmp(source_filename,obj_filename)==0) //�ַ����ȽϺ����������ѹ���ļ���ѹ���ļ��ȽϺ���ͬ
    { 
        return -1;
    } 
    printf("��ѹ���ļ�:%s,ѹ���ļ�:%s\n",source_filename,obj_filename);      //��ѹ���ļ�source_filename,ѹ���ļ�obj_filename
    if((*outp=fopen(obj_filename,"wb"))==NULL) //wb:���壺Ϊ�˶���д������һ���µĶ������ļ������ָ�����ļ������ڣ��������ļ�
    { 
        return -1;
    } 
    if((*inp=fopen(source_filename,"rb"))==NULL) //rb��Ϊ�˶���д,��һ���������ļ������ָ�����ļ������ڣ�����
    { 
        return -1;
    }
    free(obj_filename); //�ͷ�ѹ���ļ����ر�Ŀ���ļ���
    return 0; 
} 
 

char *create_filename(char *source_filename,char* obj_filename) //2.�����ļ�
{ 
    char *temp; 
    if((temp=strrchr(source_filename,'.'))==NULL) //��ԭʼ�ļ� ���� ��ʱ�ļ��� Ϊ��
    { 
        strcpy(obj_filename,source_filename);//�ַ��� ���� ����:��ԭʼ�ļ����Ƶ�Ŀ���ļ�
        strcat(obj_filename,".zip");         //�ַ��� ���� ��������.zip���ӵ�Ŀ���ļ���
    } 
    else 
    { 
        strncpy(obj_filename,source_filename,temp-source_filename); 
        obj_filename[temp-source_filename]='\0';
        strcat(obj_filename,".zip");//strcat���ַ������Ӻ���
    } 
	return obj_filename;
}

                   /*��ѹ���ļ�                ѹ���ļ� */
int compress(char *source_filename,char *obj_filename)       //3.ѹ���ļ�
{ 
    FILE *in,*out;//��������ָ���ļ���ָ����� in��out
	char ch;
    int error_code,i,j; 
    float compress_rate; //ѹ������
    hufcode hc[256]; 
    htnode  ht[256*2-1]; 
    long frequency[256],source_filesize,obj_filesize=0; 
    error_code=initial_files(source_filename,&in,obj_filename,&out);//���ó�ʼ���ļ��ĺ���
    if(error_code!=0) 
    { 
		puts("�ļ���ʧ�ܣ������������ļ�·����");
        return error_code; 
    } 
    source_filesize=frequency_data(in,frequency);   //����Ƶ�����ݸ���Դ�ļ���С
    printf("�ļ���С %ld �ֽ�\n",source_filesize); 
    error_code=create_hftree(frequency,256,ht); 
    if(error_code!=0) 
    { 
		puts("������������ʧ�ܣ�");
        return error_code; 
    } 

    error_code=encode_hftree(ht,256,hc); //���ù��������뺯��
    if(error_code!=0) 
    { 
		puts("��������������ʧ�ܣ�");
        return error_code; 
    } 
    for(i=0;i<256;i++) 
        obj_filesize+=frequency[i]*hc[i].len;
    obj_filesize=obj_filesize%8==0?obj_filesize/8:obj_filesize/8+1;
    for(i=0;i<256-1;i++) 
        obj_filesize+=2*sizeof(short);
    obj_filesize+=strlen(source_filename)+1;
    obj_filesize+=sizeof(long);
    obj_filesize+=sizeof(unsigned int);
    compress_rate=(float)obj_filesize/source_filesize; 
    printf("ѹ���ļ���С:%ld�ֽ�,ѹ������:%.2lf%%\n",obj_filesize,compress_rate*100); 
    error_code=write_compress_file(in,out,ht,hc,source_filename,source_filesize); 
	if(error_code!=0) 
    { 
		puts("д���ļ�ʧ�ܣ�");
        return error_code; 
    } 
    puts("ѹ�����!"); 
	puts(" ");
	puts("�Ƿ��ӡ���ļ����ַ���Ӧ��huffman�������룿");
	puts("          Please input Y OR N");
	do{
		scanf("%s",&ch);
		switch(ch)
		{
			case 'Y': 
				puts("�����ǹ���������");
				for(i=256;i<256*2-2;i++)
				{
					if(ht[i].w>0)
						printf("%-10d%-10d%-10d%-10d%-10d\n",i,ht[i].w,ht[i].p,ht[i].l,ht[i].r);
				}
				puts("�����ǹ��������룺");	
				for(i=0;i<256;i++)
				{
					if(frequency[i]==0)
						i++;
					else
					{
						printf("%d\t",frequency[i]);
						for(j=0;j<hc[i].len;j++)
							printf(" %d",hc[i].codestr[j]);
						printf("\n");
					}
				}
				break;
			case  'N':	break;
			default : 
				printf("ָ���������������ָ�\n");
		}
	}while(ch!='Y'&&ch!='N');
    fclose(in); 
    fclose(out);
    for(i=0;i<256;i++) 
    { 
        free(hc[i].codestr); 
    } 
    return 0; 
} 




long frequency_data(FILE *in,long frequency[]) //4.Ƶ������
{ 
    int     i,read_len; 
    unsigned char   buf[256];
    long    filesize;//�ļ���С
     
    for(i=0;i<256;i++) 
    { 
        frequency[i]=0; 
    } 
    fseek(in,0L,SEEK_SET); 
    read_len=256;     

    while(read_len==256) 
    { 
        read_len=fread(buf,1,256,in); 
        for(i=0;i<read_len;i++) 
        { 
            frequency[*(buf+i)]++;
        } 
    } 
    for(i=0,filesize=0;i<256;i++) 
    { 
        filesize+=frequency[i];
    } 

    return filesize; 
} 



int search_set(htnp ht,int n,int *s1, int *s2) //5.ѡȡ���Ȩֵ��С����
{ 
    int i,x; 
	long minValue = 1000000,min = 0;
    for(x=0;x<n;x++) 
    { 
        if(ht[x].p==-1)  break; 

    } 
    for(i=0;i<n;i++) 
    { 
        if(ht[i].p==-1 && ht[i].w < minValue) 
        { 
			minValue = ht[i].w;
            min=i;
        } 
    } 
    *s1 = min;

    minValue = 1000000,min = 0;
    for(i=0;i<n;i++) 
    { 
        if(ht[i].p==-1 && ht[i].w < minValue && i != *s1) 
        { 
           	minValue = ht[i].w;
            min=i;
        } 
    } 

    *s2 = min;
    return 1; 
} 



int create_hftree(long w[],int n,htnode ht[]) //6.������������
{ 
    int m,i,s1,s2; 

    if (n<1)    return -1; 
    m=2*n-1;
    if (ht==NULL)   return -1; 
    for(i=0;i<n;i++) 
    { 
        ht[i].w=w[i];
		ht[i].p=ht[i].l=ht[i].r=-1; 
    }  
    for(;i<m;i++) 
    { 
        ht[i].w=ht[i].p=ht[i].l=ht[i].r=-1;
    } 

    for(i=n;i<m;i++)
    { 
        search_set(ht,i,&s1,&s2); 

        ht[s1].p = ht[s2].p = i;
        ht[i].l  = s1;
		ht[i].r = s2; 
        ht[i].w  = ht[s1].w + ht[s2].w; 
    } 
    return 0; 
} 



int encode_hftree(htnp htp,int n,hufcode hc[]) //7.o-1�����������
{ 
    int i,j,p,codelen;
    unsigned char *code=(unsigned char*)malloc(n*sizeof(unsigned char)); 
     
    if (code==NULL) return -1; 
    for(i=0;i<n;i++)
    { 
        for(p=i,codelen=0;p!=2*n-2;p=htp[p].p,codelen++) 
        { 
            code[codelen]=(htp[htp[p].p].l==p?0:1);
        }        
        if((hc[i].codestr=(unsigned char *)malloc((codelen)*sizeof(unsigned char)))==NULL) 
        { 
            return -1;
        } 
        hc[i].len=codelen; 
        for(j=0;j<codelen;j++) 
        { 
            hc[i].codestr[j]=code[codelen-j-1];
        } 
    }
    free(code);
    return 0; 
} 



unsigned char chars_to_bits(const unsigned char chars[8]) //8.���ַ����ö��������ֱ�ʾ
{ 
    int i; 
    unsigned char bits=0; 
    bits|=chars[0]; 
    for(i=1;i<8;++i)
    { 
        bits<<=1; 
        bits|=chars[i];      
    } 
    return bits; 
} 


int write_compress_file(FILE *in,FILE *out,htnp ht,hufcode hc[],char* source_filename,long source_filesize) //9.д��ѹ���ļ�
{ 
    unsigned int    i,read_counter,write_counter,zip_head=0xFFFFFFFF;
    unsigned char   write_char_counter,code_char_counter,copy_char_counter,
            read_buf[256],write_buf[256],write_chars[8],filename_size=strlen(source_filename);
    hufcode *cur_hufcode; 

    fseek(in,0L,SEEK_SET); 
    fseek(out,0L,SEEK_SET); 
    fwrite(&zip_head,sizeof(unsigned int),1,out);
    fwrite(&filename_size,sizeof(unsigned char),1,out); 
    fwrite(source_filename,sizeof(char),filename_size,out); 
    fwrite(&source_filesize,sizeof(long),1,out); 
    for(i=256;i<256*2-1;i++) 
    { 
        fwrite(&(ht[i].l),sizeof(ht[i].l),1,out);
        fwrite(&(ht[i].r),sizeof(ht[i].r),1,out);
    } 
    write_counter=write_char_counter=0;
    read_counter=256;
    while(read_counter==256) 
    { 
        read_counter=fread(read_buf,1,256,in);
        for(i=0;i<read_counter;i++) 
        { 
            cur_hufcode=&hc[read_buf[i]];
            code_char_counter=0;
            while(code_char_counter!=cur_hufcode->len) 
            {
                copy_char_counter=  (8-write_char_counter > cur_hufcode->len-code_char_counter ?  
                                    cur_hufcode->len-code_char_counter : 8-write_char_counter); 
                memcpy(write_chars+write_char_counter,cur_hufcode->codestr+code_char_counter,copy_char_counter); 
                write_char_counter+=copy_char_counter;
                code_char_counter+=copy_char_counter;
                if(write_char_counter==8) 
                { 
                    write_char_counter=0;
                    write_buf[write_counter++]=chars_to_bits(write_chars); 
                    if(write_counter==256) 
                    { 
                        fwrite(write_buf,1,256,out);
                        write_counter=0;
                    } 
                } 
            } 
        } 
         
    } 
    fwrite(write_buf,1,write_counter,out);


    if(write_char_counter!=0) 
    { 
       
		
		write_char_counter=chars_to_bits(write_chars);//λ�����ĵ��ã�ʵ��ѹ������



        fwrite(&write_char_counter,1,1,out);
    } 
    return 0; 
} 



void get_mini_huffmantree(FILE* in,short mini_ht[][2]) //
{ 
    int i; 
    for(i=0;i<256;i++) 
    { 
        mini_ht[i][0]=mini_ht[i][1]=-1;
    } 
    fread(mini_ht[i],sizeof(short),2*(256-1),in);
} 



int write_decompress_file(FILE *in,FILE* out,short mini_ht[][2],long bits_pos,long obj_filesize) //д���ѹ��
{ 
    long    cur_size; 
    unsigned char   read_buf[256],write_buf[256],convert_bit; 
    unsigned int    read_counter,write_counter,cur_pos; 
    fseek(in,bits_pos,SEEK_SET);
    fseek(out,0L,SEEK_SET);
    read_counter=256-1;
    cur_size=write_counter=0;
    cur_pos=256*2-2;
    while(cur_size!=obj_filesize) 
    {   
        if(++read_counter==256) 
        { 
            fread(read_buf,1,256,in);
            read_counter=0;
        } 
        for(convert_bit=128;convert_bit!=0;convert_bit>>=1) 
        { 
            cur_pos=((read_buf[read_counter]&convert_bit)==0?mini_ht[cur_pos][0]:mini_ht[cur_pos][1]);
            if(cur_pos<256)
            { 
                write_buf[write_counter]=(unsigned char)cur_pos; 
                if(++write_counter==256)
                { 
                    fwrite(write_buf,1,256,out);
                    write_counter=0;
                } 
                cur_pos=256*2-2;
                if(++cur_size==obj_filesize) 
                { 
                    break;
                } 
            } 
        } 

    } 
    fwrite(write_buf,1,write_counter,out);
    return 0; 
} 





int decompress(char *source_filename,char *obj_filename) //��ѹ���ļ�
{ 
    int     error_code; 
    FILE    *in,*out; 
    short   mini_ht[256*2-1][2]; 
    long    obj_filesize; 

    error_code=d_initial_files(source_filename,&in,obj_filename,&out); //�������±�Ϊԭʼ�ļ��ĺ���
    if(error_code!=0) 
    { 
		puts("���ļ�ʧ�ܣ������������ļ�·����");
        return error_code; 
    } 
    fread(&obj_filesize,sizeof(long),1,in); 
    printf("��ѹ�ļ���С:%ld�ֽ�\n",obj_filesize); 
    get_mini_huffmantree(in,mini_ht); //���ú���
    error_code=write_decompress_file(in,out,mini_ht,ftell(in),obj_filesize); //���ú���
    if(error_code!=0) 
    { 
		puts("��ѹ��ʧ�ܣ�");
        return error_code; 
    } 
    puts("��ѹ�����!"); 
    fclose(in); 
    fclose(out); 
    return 0; 
} 


int d_initial_files(char *source_filename,FILE **inp,char *obj_filename,FILE **outp) //���±��ԭʼ�ļ�
{ 
    unsigned int    zip_head; //ѹ���ļ���ͷ�ļ�
    unsigned char   filename_size;    //�ļ���С 
    if ((*inp=fopen(source_filename,"rb"))==NULL) //rb��д��������ļ���ʹ*inpָ����ļ�
    { 
        return -1;
    } 
	printf("����ѹ���ļ�:%s,",source_filename); 
    fread(&zip_head,sizeof(unsigned int),1,*inp); 
    if(zip_head!=0xFFFFFFFF) 
    { 
        return -1;
    } 
    if(obj_filename==NULL) 
    { 
        if((obj_filename=(char*)malloc(256*sizeof(char)))==NULL) //����
        { 
            return -1;
        } 
        fread(&filename_size,sizeof(unsigned char),1,*inp);
        fread(obj_filename,sizeof(char),filename_size,*inp);
        obj_filename[filename_size]='\0';
    } 
    else 
    { 
        fread(&filename_size,sizeof(unsigned char),1,*inp); 
        fseek(*inp,filename_size,SEEK_CUR);
    } 
    printf("��ѹ���ļ�:%s\n",obj_filename); 
    if((*outp=fopen(obj_filename,"wb"))==NULL) 
    { 
        return -1;
    } 
    free(obj_filename);//�ͷ�ѹ���ļ�
    return 0; 
}
