#include <math.h>
#include <string.h>
#include <unistd.h> // getopt
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#include <arm_neon.h>
#include <errno.h>

#define DEBUG 0

/*
#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)

// RGB -> YUV
#define RGB2Y(R, G, B) CLIP(( (  66 * (R) + 129 * (G) +  25 * (B) + 128) >> 8) +  16)
#define RGB2U(R, G, B) CLIP(( ( -38 * (R) -  74 * (G) + 112 * (B) + 128) >> 8) + 128)
#define RGB2V(R, G, B) CLIP(( ( 112 * (R) -  94 * (G) -  18 * (B) + 128) >> 8) + 128)

*/




void print_autor(){
	printf("#############################################\n");
	printf("#           RGB to YUV tool                #\n");
	printf("#############################################\n");
	printf("# Autores: Antonio Aguilar Bravo            #\n");
	printf("#          Brayan Alfaro Cerdas             #\n");
	printf("# Maestria Sistemas Embebidos               #\n");
	printf("# Instituo Tecnologico de Costa Rica        #\n");
	printf("#############################################\n");
	exit(0);
}


void print_help(){
	printf("###################################################################\n");
	printf("#     RGB to YUV tool                                             #\n");
	printf("###################################################################\n");
	printf("# Valid arguments:                                                #\n");
	printf("#   -a: displays the information of the author of the program.    #\n");
	printf("#   -h: displays this help menu                                   #\n");
	printf("#   -i: RGBfile specifies the RGB file to be converted.           #\n");
	printf("#   -o: YUVfile specifies the output file name.                   #\n");
	printf("#                                                                 #\n");
	printf("# Command line examples                                           #\n");
	printf("#  ./rgb2yuv_neon -i image.rgb -o image.yuv                       #\n");
	printf("###################################################################\n");


}










//____________________________________________________________

void rgb2yuv(char *src_image, char *out_image){


	int test =99;
	int r1;
	int r2;





	//size of individual image
	int width  = 640;
	int height = 480;
	int depthInBytes = 3;
	int depthYUVInBytes = 4;
	int sizeImage = width * height;

    	FILE* pInput  = NULL;
    	FILE* pOutput = NULL;

    	pInput  = fopen( src_image,"rb" );
    	pOutput = fopen( out_image,"wb" );


        uint8_t* bufRGB = (uint8_t*) malloc (sizeof(uint8_t)*sizeImage*depthInBytes);
    	memset( bufRGB, 0, sizeImage*depthInBytes );

        fread( bufRGB, 1, sizeImage*depthInBytes, pInput );




  uint8_t* bufY = (uint8_t*) malloc (sizeof(uint8_t)* 8);
  memset( bufY, 0, 8 );

  uint8_t* bufU = (uint8_t*) malloc (sizeof(uint8_t)* 8);
  memset( bufU, 0, 8 );

  uint8_t* bufV = (uint8_t*) malloc (sizeof(uint8_t)* 8);
  memset( bufV, 0, 8 );

  uint8_t* bufYUV = (uint8_t*) malloc (sizeof(uint8_t)* 16);
  memset( bufYUV, 0, 16 );








asm (
     "mov r1, %[bufRGB]\n\t"
    : /* no outputs*/
    : [bufRGB] "r" (bufRGB)
    : "r1");




asm (	
	"mov r2, %[sizeImage]\n\t"
    : /* no outputs*/
    : [sizeImage]"r" (sizeImage)
    : "r2");






  /* 
  uint8x8_t rfac_y = vdup_n_u8 (66);//VDUP.8 d0,r0
  uint8x8_t gfac_y = vdup_n_u8 (129);
  uint8x8_t bfac_y = vdup_n_u8 (25);
  */
  asm(
	"mov r3, #66\n\t"
	"mov r4, #129\n\t"
        "mov r5, #25\n\t"

        "vdup.8 d3, r3\n\t"
        "vdup.8 d4, r4\n\t"
        "vdup.8 d5, r5\n\t"

  );
  ////$


  ////#
  /*
  uint8x8_t rfac_u = vdup_n_u8 (38);
  uint8x8_t gfac_u = vdup_n_u8 (74);
  uint8x8_t bfac_u = vdup_n_u8 (112);
  */
  asm(
	"mov r3, #38\n\t"
	"mov r4, #74\n\t"
        "mov r5, #112\n\t"

        "vdup.8 d6, r3\n\t"
        "vdup.8 d7, r4\n\t"
        "vdup.8 d8, r5\n\t"

  );
  ////#

  ////@
  /*
  uint8x8_t rfac_v = vdup_n_u8 (112);
  uint8x8_t gfac_v = vdup_n_u8 (94);
  uint8x8_t bfac_v = vdup_n_u8 (18);
  */

  asm(
	"mov r3, #112\n\t"
	"mov r4, #94\n\t"
        "mov r5, #18\n\t"

        "vdup.8 d9, r3\n\t"
        "vdup.8 d10, r4\n\t"
        "vdup.8 d11, r5\n\t"

  );
  ////@



  //uint16x8_t v128pq = vdupq_n_u16(128);  //q0
  asm( 
	"mov r3, #128\n\t"
	//"vdup.16 q0,r3\n\t"
	"vdup.16 q14,r3\n\t"
	
	);	

  //uint8x8_t v16p   = vdup_n_u8(16); //d12

  asm(
      "mov r3, #16\n\t"
      "vdup.8 d12, r3\n\t"
	);


  //uint8x8_t v128p = vdup_n_u8(128);  //d13
  asm(
      "mov r3, #128\n\t"
      "vdup.8 d13, r3\n\t"
      );




  ////!
  //sizeImage/=8;

  asm(
	"lsr r2,r2,#3"
	);
  ////!












  //int i;
  //for (i=0; i<sizeImage; i++)
  //{
   asm(".loop:");
  

	asm (
	     "vld3.8 {d0,d1,d2}, [r1]! \n\t");






    //Calculating Y
    /*
    temp = vmull_u8 (rgb.val[0],      rfac_y);
    temp = vmlal_u8 (temp,rgb.val[1], gfac_y);
    temp = vmlal_u8 (temp,rgb.val[2], bfac_y);
    */



    asm(

	"vmull.u8 q13, d0, d3\n\t"
        "vmlal.u8 q13, d1, d4\n\t"
        "vmlal.u8 q13, d2, d5\n\t"



	);


    //temp = vaddq_u16(temp, v128pq);
    asm("vadd.i16 q13, q13, q14\n\t");

    //result = vshrn_n_u16 (temp, 8);
    asm("vshrn.i16 d15,q13,#8\n\t");

    //result = vadd_u8(result, v16p);
    asm("vadd.i8 d15,d15,d12\n\t");




asm (
     "VST1.8 {d15}, [%[bufY]]\n\t"
    : // no outputs
    : [bufY] "r" (bufY)
    : "d15");







    //Calculating U
    /*
    temp = vmull_u8 (rgb.val[2],      bfac_u);
    temp = vmlsl_u8 (temp,rgb.val[1], gfac_u);
    temp = vmlsl_u8 (temp,rgb.val[0], rfac_u);
    */

    asm(

	"vmull.u8 q13, d2, d8\n\t"
        "vmlsl.u8 q13, d1, d7\n\t"
        "vmlsl.u8 q13, d0, d6\n\t"
	);


    //temp = vaddq_u16(temp, v128pq);
    //asm("vadd.i16 q3, q3, q0\n\t");
     asm("vadd.i16 q13, q13, q14\n\t");


    //result = vshrn_n_u16 (temp, 8);
    asm("vshrn.i16 d15,q13,#8\n\t");


    //result = vadd_u8(result, v128p);
    asm("vadd.i8 d15,d15,d13\n\t");

    //vst1_u8 (bufU, result);



asm (

     "VST1.8 {d15}, [%[bufU]]\n\t"
    : //no outputs
    : [bufU] "r" (bufU)
    : "d15");










    //Calculating V

    /*
    temp = vmull_u8 (rgb.val[0],      rfac_v);
    temp = vmlsl_u8 (temp,rgb.val[1], gfac_v);
    temp = vmlsl_u8 (temp,rgb.val[2], bfac_v);
    */

    asm(

	"vmull.u8 q13, d0, d9\n\t"
        "vmlsl.u8 q13, d1, d10\n\t"
        "vmlsl.u8 q13, d2, d11\n\t"

	);



    //temp = vaddq_u16(temp, v128pq);
    //asm("vadd.i16 q3, q3, q0\n\t");
    asm("vadd.i16 q13, q13, q14\n\t");


    //result = vshrn_n_u16 (temp, 8);
    asm("vshrn.i16 d15,q13,#8\n\t");


    //result = vadd_u8(result, v128p);
    asm("vadd.i8 d15,d15,d13\n\t");




asm (
     "VST1.8 {d15}, [%[bufV]]!\n\t"
    :// no outputs
    : [bufV] "r" (bufV)
    : "d15");






//--------------------------------------
//saving r1 and r2

asm (	
	"mov %[r2],r2 \n\t"
    : [r2]"=r" (r2)
    : //no input
    : "r2");

asm (	
	"mov %[r1],r1 \n\t"
    : [r1]"=r" (r1)
    : //no input
    : "r2");


    bufYUV[0] = bufU[1];
    bufYUV[1] = bufY[0];
    bufYUV[2] = bufV[1];
    bufYUV[3] = bufY[1];

    bufYUV[4] = bufU[3];
    bufYUV[5] = bufY[2];
    bufYUV[6] = bufV[3];
    bufYUV[7] = bufY[3];

    bufYUV[8] = bufU[5];
    bufYUV[9] = bufY[4];
    bufYUV[10] = bufV[5];
    bufYUV[11] = bufY[5];

    bufYUV[12] = bufU[7];
    bufYUV[13] = bufY[6];
    bufYUV[14] = bufV[7];
    bufYUV[15] = bufY[7];

/*
fprintf(stdout, "r1: %d\n", r1);
fprintf(stdout, "r2: %d\n", r2);
fprintf(stdout, "bufYUV: %d\n", bufYUV);
fprintf(stdout, "bufYUV[0]: %d\n", bufYUV[0]);
fprintf(stdout, "bufYUV[1]: %d\n", bufYUV[1]);
fprintf(stdout, "bufYUV[2]: %d\n", bufYUV[2]);
fprintf(stdout, "bufYUV[3]: %d\n", bufYUV[3]);
fprintf(stdout, "bufYUV[4]: %d\n", bufYUV[4]);
fprintf(stdout, "bufYUV[5]: %d\n", bufYUV[5]);
fprintf(stdout, "bufYUV[6]: %d\n", bufYUV[6]);
printf("--------------------------------------\n");


*/




    //write pixel, YUYV
    fwrite( bufYUV, 1, 16, pOutput );



//restauring

asm (	
	"mov r2,%[r2] \n\t"
    : //no outputs
    : [r2]"r" (r2)
    : "r2");

asm (	
	"mov r1,%[r1] \n\t"
    : //no outputs
    : [r1]"r" (r1)
    : "r1");
//--------------------------------------







  //}
  asm( 
      "subs        r2, r2, #1\n\t"
      "bne         .loop\n\t"	
	);


    	fclose( pInput );
    	fclose( pOutput );


}

//______________________________














int main(int argc, char **argv)
{

    	struct timeval t1, t2;
    	double elapsedTime;


	int c;
	char *src_image = NULL;
	char *out_image = NULL;


	while ((c = getopt (argc, argv, "i:o:ah")) != -1){

		switch (c) {
			case 'a':
			//-a displays the information of the author of the program.
				print_autor();
				break;
			case 'i':
			//-i RGBfile specifies the RGB file to be converted.
				src_image = optarg;
				break;		
			case 'o':
			//-o YUVfile specifies the output file name.
				out_image = optarg;
				break;
			case 'h':
			//-h displays the usage message to let the user know how to execute the application.
				print_help();
				break;
			default:
			//In case the argument read is not supported
				printf("Option '-%i' not recognized.\n",c);
			}


	}



	if(src_image==NULL){
		fprintf(stderr, "-Error- Argument -i <source_image> is required\n-Error- Use -h to see the help menu\n");
		exit(0);
	}else{
		fprintf(stdout, "-Info- RGB to YUV tool will use the image %s as the source image\n", src_image);
	}

	if(out_image==NULL){
		fprintf(stderr, "-Error- Argument -o <output_image> is required\n-Error- Use -h to see the help menu\n");
		exit(0);
	}else{
		fprintf(stdout, "-Info- RGB to YUV tool will store the image resultant in the file %s\n", out_image);
	}



  	// start timer
    	gettimeofday(&t1, NULL);

	rgb2yuv(src_image, out_image);

    	// stop timer
    	gettimeofday(&t2, NULL);

        if(DEBUG){
        fprintf(stdout, "-Info- t1.tv_sec: %d\n", t1.tv_sec);
        fprintf(stdout, "-Info- t1.tv_usec: %d\n", t1.tv_usec);
        fprintf(stdout, "-Info- t2.tv_sec: %d\n", t2.tv_sec);
        fprintf(stdout, "-Info- t2.tv_usec: %d\n", t2.tv_usec);
	}


    	// compute and print the elapsed time in millisec
    	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms

	if(DEBUG){
        	fprintf(stdout, "-Info- The elapsed time of the funtion 'rgb2yuv' was: %f\n", elapsedTime);
	}

    	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

        fprintf(stdout, "-Info- The elapsed time of the function 'rgb2yuv' was: %f ms\n", elapsedTime);


	return 0;
}






