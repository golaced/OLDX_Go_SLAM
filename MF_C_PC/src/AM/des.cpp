#include "ukey.h"
#include <iostream>
using namespace std;
LIS license;
char KEY_LOCK[9]={'3','2','3','8','3','2','6','6'}; 
char KEY_UNLOCK[9]={'3','2','3','8','3','2','6','6'}; 
void des_test(char *data,char *key_lock,char * key_unlock,unsigned int num);
char license_check(char *data,char *key_lock,char lis1[20],unsigned int num);
#define MAX_CHECK 20

#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <string.h>

char cpuidGetId(void)
{  char RX_BUF[125]={1};
 struct ifreq ifreq;
        int sock;

	  if ((sock = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
	    perror ("socket");
	    return 0;
	}
	strcpy (ifreq.ifr_name, "eth0");    //Currently, only get eth0

	if (ioctl (sock, SIOCGIFHWADDR, &ifreq) < 0)
	{
	    perror ("ioctl");
	    return 0;
	}
        /*printf("ADDR_MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
                        (unsigned char)ifreq.ifr_hwaddr.sa_data[0],
                        (unsigned char)ifreq.ifr_hwaddr.sa_data[1],
                        (unsigned char)ifreq.ifr_hwaddr.sa_data[2],
                        (unsigned char)ifreq.ifr_hwaddr.sa_data[3],
                        (unsigned char)ifreq.ifr_hwaddr.sa_data[4],
                        (unsigned char)ifreq.ifr_hwaddr.sa_data[5]);*/
	  RX_BUF[0]=(int)ifreq.ifr_hwaddr.sa_data[0]+(int)ifreq.ifr_hwaddr.sa_data[1];
	  RX_BUF[1]=(int)ifreq.ifr_hwaddr.sa_data[2]+(int)ifreq.ifr_hwaddr.sa_data[3];
	  RX_BUF[2]=(int)ifreq.ifr_hwaddr.sa_data[4]+(int)ifreq.ifr_hwaddr.sa_data[5];
	  cout<<"Your ID is: "<<(int)RX_BUF[0]<<"."<<(int)RX_BUF[1]<<"."<<(int)RX_BUF[2]<<endl;
	  license.state=license_check(RX_BUF,KEY_LOCK,license.lis,125);
	  if(license.state)
	  cout<<"Lisence Check"<<endl;
	  else
	  cout<<"Lisence is not correct!!"<<endl; 
	  return license.state;
}

char set_lisence(char *in)
{ 
  int i=0;
  for(i=0;i<20;i++)
   license.lis[i]=in[i];
   cout<<"Set License: "<<license.lis<<endl;
   return cpuidGetId();
}	

static const char IP_Table[64]={             
	58,50,42,34,26,18,10, 2,60,52,44,36,28,20,12, 4,
	62,54,46,38,30,22,14, 6,64,56,48,40,32,24,16, 8,
	57,49,41,33,25,17, 9, 1,59,51,43,35,27,19,11, 3,
	61,53,45,37,29,21,13, 5,63,55,47,39,31,23,15, 7 
};

// ¶Ôµü´úºóµÄL16,R16Ö´ÐÐIPÄæÖÃ»»,Êä³öÃÜÎÄ
static const char IPR_Table[64]={              
	40, 8,48,16,56,24,64,32,39, 7,47,15,55,23,63,31,
	38, 6,46,14,54,22,62,30,37, 5,45,13,53,21,61,29,
	36, 4,44,12,52,20,60,28,35, 3,43,11,51,19,59,27,
	34, 2,42,10,50,18,58,26,33, 1,41, 9,49,17,57,25	
};

/*--------------------------- µü´ú·¨Ôò ----------------------------*/ 

// Fº¯Êý,32Î»µÄR0½øÐÐE±ä»»,À©Îª48Î»Êä³ö (R1~R16)        [±¸ÓÃA]  [Ã÷ÎÄ²Ù×÷] 
static char E_Table[48]={
	32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
	 8, 9,10,11,12,13,12,13,14,15,16,17,
    16,17,18,19,20,21,20,21,22,23,24,25,
    24,25,26,27,28,29,28,29,30,31,32, 1
};

// ×ÓÃÜÔ¿K(i)µÄ»ñÈ¡ ÃÜÔ¿ÎªK Å×ÆúµÚ6,16,24,32,40,48,64Î»          [ÃÜÔ¿²Ù×÷] 
// ÓÃPC1Ñ¡Î» ·ÖÎª Ç°28Î»C0,ºó28Î»D0 Á½²¿·Ö  
static char PC1_Table[56]={
	57,49,41,33,25,17, 9, 1,58,50,42,34,26,18,
	10, 2,59,51,43,35,27,19,11, 3,60,52,44,36,
	63,55,47,39,31,23,15, 7,62,54,46,38,30,22,
	14, 6,61,53,45,37,29,21,13, 5,28,20,12, 4
};

// ¶ÔC0,D0·Ö±ð½øÐÐ×óÒÆ,¹²16´Î,×óÒÆÎ»ÊýÓëÏÂÃæ¶ÔÓ¦                 [ÃÜÔ¿²Ù×÷]
static char Move_Table[16]={
	 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

// C1,D1ÎªµÚÒ»´Î×óÒÆºóµÃµ½,½øÐÐPC2Ñ¡Î»,µÃµ½48Î»Êä³öK1   [±¸ÓÃB]   [ÃÜÔ¿²Ù×÷]     
static char PC2_Table[48]={
	14,17,11,24, 1, 5, 3,28,15, 6,21,10,
	23,19,12, 4,26, 8,16, 7,27,20,13, 2,
	41,52,31,37,47,55,30,40,51,34,33,48,
	44,49,39,56,34,53,46,42,50,36,29,32	
};

/*------------- Fº¯Êý ±¸ÓÃAºÍ±¸ÓÃB Òì»ò µÃµ½48Î»Êä³ö ---------------*/ 

// Òì»òºóµÄ½á¹û48Î»·ÖÎª8×é,Ã¿×é6Î»,×÷Îª8¸öSºÐµÄÊäÈë             [×éºÏ²Ù×÷] 
// SºÐÒÔ6Î»×÷ÎªÊäÈë(8×é),4Î»×÷ÎªÊä³ö(4*(8×é)=32Î»)
// S¹¤×÷Ô­Àí ¼ÙÉèÊäÈëÎªA=abcdef ,ÔòbcdeËù´ú±íµÄÊýÊÇ0-15Ö®¼äµÄ
// Ò»¸öÊý¼ÇÎª X=bcde ,af´ú±íµÄÊÇ0-3Ö®¼äµÄÒ»¸öÊý,¼ÇÎª Y=af 
// ÔÚS1µÄXÁÐ,YÐÐÕÒµ½Ò»¸öÊýValue,ËüÔÚ0-15Ö®¼ä,¿ÉÒÔÓÃ¶þ½øÖÆ±íÊ¾
// ËùÒÔÎª4bit (¹²32Î»)  
static char S_Box[8][4][16]={
	//S1
	14, 4,13, 1, 2,15,11, 8, 3,10, 6,12, 5, 9, 0, 7,
	 0,15, 7, 4,14, 2,13, 1,10, 6,12,11, 9, 5, 3, 8,
	 4, 1,14, 8,13, 6, 2,11,15,12, 9, 7, 3,10, 5, 0,
	15,12, 8, 2, 4, 9, 1, 7, 5,11, 3,14,10, 0, 6,13,
	//S2
	15, 1, 8,14, 6,11, 3, 4, 9, 7, 2,13,12, 0, 5,10,
	 3,13, 4, 7,15, 2, 8,14,12, 0, 1,10, 6, 9,11, 5,
	 0,14, 7,11,10, 4,13, 1, 5, 8,12, 6, 9, 3, 2,15,
	13, 8,10, 1, 3,15, 4, 2,11, 6, 7,12, 0, 5,14, 9,
	//S3
	10, 0, 9,14, 6, 3,15, 5, 1,13,12, 7,11, 4, 2, 8,
	13, 7, 0, 9, 3, 4, 6,10, 2, 8, 5,14,12,11,15, 1,
	13, 6, 4, 9, 8,15, 3, 0,11, 1, 2,12, 5,10,14, 7,
	 1,10,13, 0, 6, 9, 8, 7, 4,15,14, 3,11, 5, 2,12,
	//S4
	 7,13,14, 3, 0, 6, 9,10, 1, 2, 8, 5,11,12, 4,15,
	13, 8,11, 5, 6,15, 0, 3, 4, 7, 2,12, 1,10,14, 9,
	10, 6, 9, 0,12,11, 7,13,15, 1, 3,14, 5, 2, 8, 4,
	 3,15, 0, 6,10, 1,13, 8, 9, 4, 5,11,12, 7, 2,14,
	//S5
	 2,12, 4, 1, 7,10,11, 6, 8, 5, 3,15,13, 0,14, 9,
	14,11, 2,12, 4, 7,13, 1, 5, 0,15,10, 3, 9, 8, 6,
	 4, 2, 1,11,10,13, 7, 8,15, 9,12, 5, 6, 3, 0,14,
	11, 8,12, 7, 1,14, 2,13, 6,15, 0, 9,10, 4, 5, 3,
	//S6
	12, 1,10,15, 9, 2, 6, 8, 0,13, 3, 4,14, 7, 5,11,
	10,15, 4, 2, 7,12, 0, 5, 6, 1,13,14, 0,11, 3, 8,
	 9,14,15, 5, 2, 8,12, 3, 7, 0, 4,10, 1,13,11, 6,
   	 4, 3, 2,12, 9, 5,15,10,11,14, 1, 7, 6, 0, 8,13,
	//S7
	 4,11, 2,14,15, 0, 8,13, 3,12, 9, 7, 5,10, 6, 1,
	13, 0,11, 7, 4, 0, 1,10,14, 3, 5,12, 2,15, 8, 6,
	 1, 4,11,13,12, 3, 7,14,10,15, 6, 8, 0, 5, 9, 2,
	 6,11,13, 8, 1, 4,10, 7, 9, 5, 0,15,14, 2, 3,12,
	//S8
	13, 2, 8, 4, 6,15,11, 1,10, 9, 3,14, 5, 0,12, 7,
	 1,15,13, 8,10, 3, 7, 4,12, 5, 6,11, 0,14, 9, 2,
	 7,11, 4, 1, 9,12,14, 2, 0, 6,10,13,15, 3, 5, 8,
	 2, 1,14, 7, 4,10, 8,13,15,12, 9, 0, 3, 5, 6,11
};

// Fº¯Êý ×îºóµÚ¶þ²½,¶ÔSºÐÊä³öµÄ32½øÐÐPÖÃ»»                     [×éºÏ²Ù×÷]
// Êä³öµÄÖµ²ÎÓëÒ»´Îµü´ú:
// L(i)=R(i-1)
// R(i)=L(i-1)^f(R(i-1),K(i)) Òì»ò 
static char P_Table[32]={
	16, 7,20,21,29,12,28,17, 1,15,23,26, 5,18,31,10,
	 2, 8,24,14,32,27, 3, 9,19,13,30, 6,22,11, 4,25
};

// 16¸ö×ÓÃÜÔ¿K(1~16) 
static bool SubKey[16][48]={0};                              
void SetKey(char KeyIn[8]);                         // ÉèÖÃÃÜÔ¿
void PlayDes(char MesOut[8],char MesIn[8]);       // Ö´ÐÐDES¼ÓÃÜ
void KickDes(char MesOut[8],char MesIn[8]);             // Ö´ÐÐDES½âÃÜ 
char MesHex_OUT[128]={0};         // 16¸ö×Ö·ûÊý×éÓÃÓÚ´æ·Å 64Î»16½øÖÆµÄÃÜÎÄ
char MyMessage_Kick[125]={0};
void des_test(char *data,char *key_lock,char * key_unlock,unsigned int num)
{
    int i=0; 
    char MesHex[16]={0};         // 16¸ö×Ö·ûÊý×éÓÃÓÚ´æ·Å 64Î»16½øÖÆµÄÃÜÎÄ
		char MesHexr[16]={0};
    char MyKey[8]={0};           // ³õÊ¼ÃÜÔ¿ 8×Ö½Ú*8
    char YourKey[8]={0};         // ÊäÈëµÄ½âÃÜÃÜÔ¿ 8×Ö½Ú*8
    char MyMessage[8]={0};       // ³õÊ¼Ã÷ÎÄ 
    char MyMessage_OUT[8]={0};       // ³õÊ¼Ã÷ÎÄ 
/*-----------------------------------------------*/
    u16 group,group_ero;
		u16 j=0,k=0,k1=0,l=0,l1=0;		
		group_ero=num%8;
		group=num/8;
		if(group_ero!=0)
    group++;
		
		for (i=0;i<8;i++){MyKey[i]=key_lock[i];YourKey[i]=key_unlock[i];}//copy key
		
		SetKey(MyKey);               // set key master

		for(j=0;j<group;j++)//lock
		{
		for (i=0;i<8;i++)
			MesHex[i]=0;//clear hex_buf
			
	  if(group_ero!=0&&j==group-2)
		{for (i=0;i<group_ero;i++)
			MyMessage[i]=data[l++];//copy group	
		 for (i=group_ero;i<8;i++)
			MyMessage[i]=0x00;//copy group	
		}
		else
		for (i=0;i<8;i++)
		MyMessage[i]=data[l++];//copy group
			
		PlayDes(MesHex,MyMessage);   // DSE
			
		for (i=0;i<16;i++)
		MesHex_OUT[k++]=MesHex[i];//out hex	
			
    }
    SetKey(YourKey);             // set key slave
  	for(j=0;j<group;j++)//unlock
		{
			for (i=0;i<16;i++)
			MesHexr[i]=MesHex_OUT[k1++];//out hex	
       KickDes(MyMessage_OUT,MesHexr);                     // ½âÃÜÊä³öµ½MyMessage   
      for (i=0;i<8;i++)
			MyMessage_Kick[l1++]=MyMessage_OUT[i];//copy group
		}
}


char license_check(char *data,char *key_lock,char lis1[20],unsigned int num)
{
     int  i=0; 
    char MesHex[16]={0};         // 16¸ö×Ö·ûÊý×éÓÃÓÚ´æ·Å 64Î»16½øÖÆµÄÃÜÎÄ
		char MesHexr[16]={0};
    char MyKey[8]={0};           // ³õÊ¼ÃÜÔ¿ 8×Ö½Ú*8
    char YourKey[8]={0};         // ÊäÈëµÄ½âÃÜÃÜÔ¿ 8×Ö½Ú*8
    char MyMessage[8]={0};       // ³õÊ¼Ã÷ÎÄ 
    char MyMessage_OUT[8]={0};       // ³õÊ¼Ã÷ÎÄ 
    char lis_reg[20];
    
    for(int k=0;k<MAX_CHECK;k++){
      lis_reg[k]=lis1[k];
    }
/*-----------------------------------------------*/
    u16 group,group_ero;
		u16 j=0,k=0,k1=0,l=0,l1=0;		
		group_ero=num%8;
		group=num/8;
		if(group_ero!=0)
    group++;
		
		for (i=0;i<8;i++){MyKey[i]=key_lock[i];}//copy key
		
		SetKey(MyKey);               // set key master
	      
		for(j=0;j<group;j++)//lock
		{
		for (i=0;i<8;i++)
			MesHex[i]=0;//clear hex_buf
			
	  if(group_ero!=0&&j==group-2)
		{for (i=0;i<group_ero;i++)
			MyMessage[i]=data[l++];//copy group	
		 for (i=group_ero;i<8;i++)
			MyMessage[i]=0x00;//copy group	
		}
		else
		for (i=0;i<8;i++)
		MyMessage[i]=data[l++];//copy group
			
		PlayDes(MesHex,MyMessage);   // DSE
			
		for (i=0;i<16;i++)
		MesHex_OUT[k++]=MesHex[i];//out hex	
			
    }
            //cout<<lis_reg<<endl;
	    //cout<<MesHex_OUT<<endl;
	
	       int off=0;
		for(int k=0;k<MAX_CHECK-1;k++){
		  //cout<<license.lis[k]<<" "<<MesHex_OUT[k+off]<<endl;
		  if(lis_reg[k]!=MesHex_OUT[k+off])
		  {//cout<<k<<endl; 
		    return 0;}
		}
			
		return 1;	
}
/*-------------------------------
 °ÑDatIn¿ªÊ¼µÄ³¤¶ÈÎ»LenÎ»µÄ¶þ½øÖÆ
 ¸´ÖÆµ½DatOutºó
--------------------------------*/
void BitsCopy(bool *DatOut,bool *DatIn,int Len)     // Êý×é¸´ÖÆ OK 
{
    int i=0;
    for(i=0;i<Len;i++)
    {
        DatOut[i]=DatIn[i];
    }
}

/*-------------------------------
 ×Ö½Ú×ª»»³ÉÎ»º¯Êý 
 Ã¿8´Î»»Ò»¸ö×Ö½Ú Ã¿´ÎÏòÓÒÒÆÒ»Î»
 ºÍ1ÓëÈ¡×îºóÒ»Î» ¹²64Î» 
--------------------------------*/
void ByteToBit(bool *DatOut,char *DatIn,int Num)       // OK
{
    int i=0;
    for(i=0;i<Num;i++)
    {
        DatOut[i]=(DatIn[i/8]>>(i%8))&0x01;   
    }                                       
}

/*-------------------------------
 Î»×ª»»³É×Ö½Úº¯Êý
 ×Ö½ÚÊý×éÃ¿8´ÎÒÆÒ»Î»
 Î»Ã¿´ÎÏò×óÒÆ ÓëÉÏÒ»´Î»ò   
---------------------------------*/
void BitToByte(char *DatOut,bool *DatIn,int Num)        // OK
{
    int i=0;
    for(i=0;i<(Num/8);i++)
    {
        DatOut[i]=0;
    } 
    for(i=0;i<Num;i++)
    {
        DatOut[i/8]|=DatIn[i]<<(i%8);    
    }        
}


/*----------------------------------
 ¶þ½øÖÆÃÜÎÄ×ª»»ÎªÊ®Áù½øÖÆ
 ÐèÒª16¸ö×Ö·û±íÊ¾
-----------------------------------*/
void BitToHex(char *DatOut,bool *DatIn,int Num)
{
    int i=0;
    for(i=0;i<Num/4;i++)
    {
        DatOut[i]=0;
    }
    for(i=0;i<Num/4;i++)
    {
        DatOut[i] = DatIn[i*4]+(DatIn[i*4+1]<<1)
                    +(DatIn[i*4+2]<<2)+(DatIn[i*4+3]<<3);
        if((DatOut[i]%16)>9)
        {
            DatOut[i]=DatOut[i]%16+'7';       //  ÓàÊý´óÓÚ9Ê±´¦Àí 10-15 to A-F
        }                                     //  Êä³ö×Ö·û 
        else
        {
            DatOut[i]=DatOut[i]%16+'0';       //  Êä³ö×Ö·û       
        }
    }
    
}

/*---------------------------------------------
 Ê®Áù½øÖÆ×Ö·û×ª¶þ½øÖÆ
----------------------------------------------*/
void HexToBit(bool *DatOut,char *DatIn,int Num)
{
    int i=0;                        // ×Ö·ûÐÍÊäÈë 
    for(i=0;i<Num;i++)
    {
        if((DatIn[i/4])>'9')         //  ´óÓÚ9 
        {
            DatOut[i]=((DatIn[i/4]-'7')>>(i%4))&0x01;               
        }
        else
        {
            DatOut[i]=((DatIn[i/4]-'0')>>(i%4))&0x01;     
        } 
    }    
}

// ±íÖÃ»»º¯Êý  OK
void TablePermute(bool *DatOut,bool *DatIn,const char *Table,int Num)  
{
    int i=0;
    static bool Temp[256]={0};
    for(i=0;i<Num;i++)                // NumÎªÖÃ»»µÄ³¤¶È 
    {
        Temp[i]=DatIn[Table[i]-1];  // Ô­À´µÄÊý¾Ý°´¶ÔÓ¦µÄ±íÉÏµÄÎ»ÖÃÅÅÁÐ 
    }
    BitsCopy(DatOut,Temp,Num);       // °Ñ»º´æTempµÄÖµÊä³ö 
} 

// ×ÓÃÜÔ¿µÄÒÆÎ»
void LoopMove(bool *DatIn,int Len,int Num) // Ñ­»·×óÒÆ LenÊý¾Ý³¤¶È NumÒÆ¶¯Î»Êý
{
    static bool Temp[256]={0};    // »º´æ   OK
    BitsCopy(Temp,DatIn,Num);       // ½«Êý¾Ý×î×ó±ßµÄNumÎ»(±»ÒÆ³öÈ¥µÄ)´æÈëTemp 
    BitsCopy(DatIn,DatIn+Num,Len-Num); // ½«Êý¾Ý×ó±ß¿ªÊ¼µÄµÚNumÒÆÈëÔ­À´µÄ¿Õ¼ä
    BitsCopy(DatIn+Len-Num,Temp,Num);  // ½«»º´æÖÐÒÆ³öÈ¥µÄÊý¾Ý¼Óµ½×îÓÒ±ß 
} 

// °´Î»Òì»ò
void Xor(bool *DatA,bool *DatB,int Num)           // Òì»òº¯Êý
{
    int i=0;
    for(i=0;i<Num;i++)
    {
        DatA[i]=DatA[i]^DatB[i];                  // Òì»ò 
    }
} 

// ÊäÈë48Î» Êä³ö32Î» ÓëRiÒì»ò
void S_Change(bool DatOut[32],bool DatIn[48])     // SºÐ±ä»»
{
    int i,X,Y;                                    // iÎª8¸öSºÐ 
    for(i=0,Y=0,X=0;i<8;i++,DatIn+=6,DatOut+=4)   // Ã¿Ö´ÐÐÒ»´Î,ÊäÈëÊý¾ÝÆ«ÒÆ6Î» 
    {                                              // Ã¿Ö´ÐÐÒ»´Î,Êä³öÊý¾ÝÆ«ÒÆ4Î»
        Y=(DatIn[0]<<1)+DatIn[5];                          // af´ú±íµÚ¼¸ÐÐ
        X=(DatIn[1]<<3)+(DatIn[2]<<2)+(DatIn[3]<<1)+DatIn[4]; // bcde´ú±íµÚ¼¸ÁÐ
        ByteToBit(DatOut,&S_Box[i][Y][X],4);      // °ÑÕÒµ½µÄµãÊý¾Ý»»Îª¶þ½øÖÆ    
    }
}

// Fº¯Êý
void F_Change(bool DatIn[32],bool DatKi[48])       // Fº¯Êý
{
    static bool MiR[48]={0};             // ÊäÈë32Î»Í¨¹ýEÑ¡Î»±äÎª48Î»
    TablePermute(MiR,DatIn,E_Table,48); 
    Xor(MiR,DatKi,48);                   // ºÍ×ÓÃÜÔ¿Òì»ò
    S_Change(DatIn,MiR);                 // SºÐ±ä»»
    TablePermute(DatIn,DatIn,P_Table,32);   // PÖÃ»»ºóÊä³ö
}



void SetKey(char KeyIn[8])               // ÉèÖÃÃÜÔ¿ »ñÈ¡×ÓÃÜÔ¿Ki 
{
    int i=0;
    static bool KeyBit[64]={0};                // ÃÜÔ¿¶þ½øÖÆ´æ´¢¿Õ¼ä 
    static bool *KiL=&KeyBit[0],*KiR=&KeyBit[28];  // Ç°28,ºó28¹²56
    ByteToBit(KeyBit,KeyIn,64);                    // °ÑÃÜÔ¿×ªÎª¶þ½øÖÆ´æÈëKeyBit 
    TablePermute(KeyBit,KeyBit,PC1_Table,56);      // PC1±íÖÃ»» 56´Î
    for(i=0;i<16;i++)
    {
        LoopMove(KiL,28,Move_Table[i]);       // Ç°28Î»×óÒÆ 
        LoopMove(KiR,28,Move_Table[i]);          // ºó28Î»×óÒÆ 
         TablePermute(SubKey[i],KeyBit,PC2_Table,48); 
         // ¶þÎ¬Êý×é SubKey[i]ÎªÃ¿Ò»ÐÐÆðÊ¼µØÖ· 
         // Ã¿ÒÆÒ»´ÎÎ»½øÐÐPC2ÖÃ»»µÃ Ki 48Î» 
    }        
}

void PlayDes(char MesOut[8],char MesIn[8])  // Ö´ÐÐDES¼ÓÃÜ
{                                           // ×Ö½ÚÊäÈë BinÔËËã HexÊä³ö 
    int i=0;
    static bool MesBit[64]={0};        // Ã÷ÎÄ¶þ½øÖÆ´æ´¢¿Õ¼ä 64Î»
    static bool Temp[32]={0};
    static bool *MiL=&MesBit[0],*MiR=&MesBit[32]; // Ç°32Î» ºó32Î» 
    ByteToBit(MesBit,MesIn,64);                 // °ÑÃ÷ÎÄ»»³É¶þ½øÖÆ´æÈëMesBit
    TablePermute(MesBit,MesBit,IP_Table,64);    // IPÖÃ»» 
    for(i=0;i<16;i++)                       // µü´ú16´Î 
    {
        BitsCopy(Temp,MiR,32);            // ÁÙÊ±´æ´¢
        F_Change(MiR,SubKey[i]);           // Fº¯Êý±ä»»
        Xor(MiR,MiL,32);                  // µÃµ½Ri 
        BitsCopy(MiL,Temp,32);            // µÃµ½Li 
    }                    
    TablePermute(MesBit,MesBit,IPR_Table,64);
    BitToHex(MesOut,MesBit,64);
}

void KickDes(char MesOut[8],char MesIn[8])       // Ö´ÐÐDES½âÃÜ
{                                                // HexÊäÈë BinÔËËã ×Ö½ÚÊä³ö 
    int i=0;
    static bool MesBit[64]={0};        // ÃÜÎÄ¶þ½øÖÆ´æ´¢¿Õ¼ä 64Î»
    static bool Temp[32]={0};
    static bool *MiL=&MesBit[0],*MiR=&MesBit[32]; // Ç°32Î» ºó32Î»
    HexToBit(MesBit,MesIn,64);                 // °ÑÃÜÎÄ»»³É¶þ½øÖÆ´æÈëMesBit
    TablePermute(MesBit,MesBit,IP_Table,64);    // IPÖÃ»» 
    for(i=15;i>=0;i--)
    {
        BitsCopy(Temp,MiL,32);
        F_Change(MiL,SubKey[i]);
        Xor(MiL,MiR,32);
        BitsCopy(MiR,Temp,32);
    }    
    TablePermute(MesBit,MesBit,IPR_Table,64);
    BitToByte(MesOut,MesBit,64);        
} 
