//gcc -lssl -std=c99 UltraC-Crack.c -o UltraC-Crack
//
//Lame attempt at bruteforcing Ultralight C keyspace from a captured authentication handshake.
//Copyright (c) 2014 Andy Davies of Pentura.
//Software provided AS-IS, Pentura and myself bear no responsibility over its use.
//

#include <openssl/des.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>

//some default keys
static uint8_t key1_blnk_data[16] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
static uint8_t key2_defa_data[16] = { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f };
static uint8_t key3_3des_data[16] = { 0x49,0x45,0x4D,0x4B,0x41,0x45,0x52,0x42,0x21,0x4E,0x41,0x43,0x55,0x4F,0x59,0x46 };
static uint8_t key4_nfc_data[16]  = { 0x42,0x52,0x45,0x41,0x4b,0x4d,0x45,0x49,0x46,0x59,0x4f,0x55,0x43,0x41,0x4e,0x21 };

//what we want to crack
//id  page-byte          0-0 0-1  0-2  1-0   1-1  1-2  1-3
static uint8_t cuid[7]={0x04,0x0e,0x6b,0xca,0x0b,0x28,0x80};
static  uint8_t e_RndB[8]={0x1e,0xae,0x15,0xf8,0x5b,0x05,0xe3,0x2d};
static uint8_t RndARndB[16]={0x62,0x7a,0xb7,0x02,0x0c,0xfe,0xc7,0x8b,0xa2,0x4e,0x6b,0x43,0x5e,0x0f,0xa0,0xb7};

uint8_t * diversify_key(uint8_t * key){
 for(int i=0; i<16; i++){
   if(i<=6) key[i]^=cuid[i];
   if(i>6) key[i]^=cuid[i%7];
 }
 return key;
}

// Needed to Authenticate to Ultralight C tags
void rol (uint8_t *data, const size_t len){
    uint8_t first = data[0];
    for (size_t i = 0; i < len-1; i++) {
        data[i] = data[i+1];
    }
    data[len-1] = first;
}

//Print Nice HEX Strings
char * sprint_hex(const uint8_t * data, const size_t len) {
	static char buf[1024];
	char * tmp = buf;
	size_t i;

	for (i=0; i < len && i < 1024/3; i++, tmp += 3)
		sprintf(tmp, "%02x ", data[i]);

	return buf;
}

//Needed to compare decrypted Random numbers
int hexcompare(uint8_t *a, uint8_t *b){
  int sizea=0, sizeb=0;
  
  sizea=sizeof(a)/sizeof(a[0]);
  sizeb=sizeof(b)/sizeof(b[0]);
  if (sizea != sizeb) return -1;
  
  for (int i=0; i<sizea-1; i++){
      if(a[i]!=b[i]) return -1; 
  }
  
  return 0;
}

int test_key(uint8_t *key){

    DES_cblock key1,key2;
    DES_cblock RndB={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    DES_cblock dRndB={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    DES_cblock rRndB={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    DES_cblock iv={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t RndARndBout[16] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
    DES_key_schedule ks1,ks2;
    
    memcpy(key1,key,8);
    memcpy(key2,key+8,8);
    DES_set_key((DES_cblock *)key1,&ks1);
    DES_set_key((DES_cblock *)key2,&ks2);

    DES_ede2_cbc_encrypt(e_RndB,RndB,sizeof(e_RndB),&ks1,&ks2,&iv,0);
    //copy RndB and rotate;  at this point we have e_RndB, our guess RndB and rotated rRndB
    memcpy(rRndB,RndB,8);
    rol(rRndB,8);
    //printf("     rRndB:%s\n",sprint_hex(rRndB,8));
    memcpy(iv,e_RndB,8);
    DES_ede2_cbc_encrypt(RndARndB,RndARndBout,sizeof(RndARndB),&ks1,&ks2,&iv,0);
    memcpy(dRndB,RndARndBout+8,8);

    //printf("dec(RndB'):%s\n",sprint_hex(dRndB,8));
    
    if (hexcompare(dRndB,rRndB)==0){
	printf("We have a winner!\n %s\n", sprint_hex(key,16));
	return 1;
    }
    return 0;
}

int main(int argc, char *argv[]){
  uint8_t test[16] = { 0x49,0x45,0x4D,0x4B,0x41,0x45,0x52,0x42,0x21,0x4E,0x41,0x43,0x55,0x4F,0x59,0x45 };
  uint8_t * key;
  //check default keys
  printf("Checking Default Keys:\n");
  for(int i=0; i<4; i++){
    switch(i){
      case 0: if(test_key(key1_blnk_data)) exit(0);
	      break;
      case 1: if(test_key(key2_defa_data)) exit(0);
	      break;
      case 2: if(test_key(key3_3des_data)) exit(0);
	      break;
      case 3: if(test_key(key4_nfc_data)) exit(0);
	      break;
      default:
	      break;
    }
  }
  printf("  No Keys Found.\n");
  printf("Checking Diversified Default Keys:\n");
  for(int i=0; i<4; i++){
    switch(i){
      case 0: key=diversify_key(key1_blnk_data);
	      if(test_key(key)) exit(0);
	      //printf("%s\n",sprint_hex(key,16));
	      break;
      case 1: key=diversify_key(key2_defa_data);
	      if(test_key(key)) exit(0);
	      //printf("%s\n",sprint_hex(key,16));
	      break;
      case 2: key=diversify_key(key3_3des_data);
	      if(test_key(key)) exit(0);
	      //printf("%s\n",sprint_hex(key,16));
	      break;
      case 3: key=diversify_key(key4_nfc_data);
	      if(test_key(key)) exit(0);
	      //printf("%s\n",sprint_hex(key,16));
	      break;
      default:
	      break;
    }
  }
  //default check complete, now bruteforcing... {not the most eligant way}
  printf("  No Keys Found.\n");
  printf("Brute-forcing Keys:\n");
 
  for(uint8_t y=0x0;y<0xff;y++){
    for(uint8_t x=0x0;x<0xff;x++){
      for(uint8_t w=0x0;w<0xff;w++){
        for(uint8_t v=0x0;v<0xff;v++){
          for(uint8_t u=0x0;u<0xff;u++){
            for(uint8_t t=0x0;t<0xff;t++){
    for(uint8_t s=0x0;s<0xff;s++){
      for(uint8_t r=0x0;r<0xff;r++){
        for(uint8_t q=0x0;q<0xff;q++){
          for(uint8_t p=0x0;p<0xff;p++){
            for(uint8_t o=0x0;o<0xff;o++){
              for(uint8_t n=0x0;n<0xff;n++){
                for(uint8_t m=0x0;m<0xff;m++){
                  for(uint8_t l=0x0;l<0xff;l++){
                    for(uint8_t k=0x0;k<0xff;k++){
                     for(uint8_t j=0x0;j<0xff;j++){
  	               test[15]=j;
  	               test[14]=k;
  	               test[13]=l;
	               test[12]=m;
	               test[11]=n;
	               test[10]=o;
	               test[9]=p;
	               test[8]=q;
	               test[7]=r;
	               test[6]=s;
	               test[5]=t;
	               test[4]=u;
	               test[3]=v;
	               test[2]=w;
	               test[1]=x;
	               test[0]=y;
                       if(test_key(test)) exit(0);
	               //printf("%s\n", sprint_hex(test,16));
		     }
		    }
		  }
		}
	      }
	    }
	  }
        }    
      }
    }
            }
          }
        }
      }
    }
  }
}