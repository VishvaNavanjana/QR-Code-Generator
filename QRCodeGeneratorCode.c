/*
qr code generator 

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h> //for system()
#define SIZE 256

//prototype
void printUsage(char programname[]);//function that print usage of the program
int colorCode(char color[]);//function that returns color code of the color or if argument is wrong then return -1
int binaryVal(int number,int index);//function that returns the binary value of a given positions of a number
void hashString(char hash[],char hashed[],char input[],int s1,int lstIndex);//function that create hashed string. s1 is the size of the string
void encodedPattern(int asciinumber,int color);//function that print the encoded pattern of one character using given color
void printBasicshape(int color,int size);//this function print the basic frame and position identification squares.(20 for small frame 40 for large frame)
void printBlock(char hashed[],int rows,int columns,int firstindex,int color);//this function prints blocks contain patterns of characters
void moveUp(int lines);//cursor move upward for given number of lines
void moveDown(int lines);
void moveRight(int spaces);//cursor move right for given number of spaces
void moveLeft(int spaces);

int main(int argc,char* argv[]){

       //argument handling
       //int counter;
       char programname[100];//array to store program name
       strcpy(programname,argv[0]);//copy programe name in argv[0] to string array program

       int colorcode=40;

       if((argc==2)&&(!strcmp(argv[1],"-h"))){//argument is "./programname -h"
                     printUsage(programname);
                     return -1;
               }

       else if(((argc>=2)&&(strcmp(argv[1],"-c")))||((argc==2)&&(!strcmp(argv[1],"-c"))) ) {//invalid arguments
                     printf("Incorrect usage of arguments.\n");
                     printUsage(programname);
                     return -1;
               }
       else if(argc>3){
                     printf("Incorrect usage of arguments.\n");
                     printUsage(programname);
                     return -1;
       



       }

       else if((argc==3)&&(!strcmp(argv[1],"-c"))){//checking for color input
                    char color[10]={0};//array to store 3rd argument
                    strcpy(color,argv[2]);//copy 3rd argument to string array color

                    //lower case the 3rd argument
                    for(int r=0;color[r]!='\0';r++){
                         char c = color[r];
                         if( c>=65 && c<=90 ){
                                        c=c+32;
                                        color[r]=c;
                                       }
                            }


                    colorcode=colorCode(color);

                    //printf("color code is %i\n",colorcode);

                    if(colorcode==-1){//if 3rd argument is not a color
                     printf("Incorrect usage of arguments.\n");
                     printUsage(programname);
                     return -1;
                    }

                }

        else{//if there are no arguments other than program name then default color is black
                    int colorcode=40;
                }

      //clearing the terminal
      //system("clear");//clear output screen

      unsigned char input[SIZE];

      unsigned char hash24[25]={0};
      unsigned char hash132[133]={0};

      //taking input for url
      printf("Enter the URL: ");
      scanf("%s",input);

      //checking whether input url has more than 3 characters and less than 120
      if(strlen(input)<=3){
        printf("String is too short. Not supported by available QR versions\n");
      }
      else if(strlen(input)>=120){
        printf("String is too long. Not supported by available QR versions\n");
      }

      else{//url length is correct


        //hashed string for characters less than or equal to 20
        if(strlen(input)<=20){

              const int s1=24;
              int lstIndex=23;

              unsigned char hashed24[s1];


              //calling hashing function
              hashString(hash24,hashed24,input,s1,lstIndex);


              //printf("%s\n%lu\n",hashed24,strlen(hashed24));
              int c1=colorcode;
              //printf("color is %i\n",c1);

              system("clear");//clear output screen
              


             //first printing the basic frame and position identification squares
              printBasicshape(c1,20);

              //save current cursor position after printing basic frame
              printf("\033[s");

              moveUp(19);

              moveRight(14);

              //print ch 17 to 20
              printBlock(hashed24,2,2,16,c1);

              moveLeft(12);
              //print ch 21 to 24
              printBlock(hashed24,2,2,20,c1);

              moveUp(6);

              moveRight(12);
              //print ch1 to ch16
              printBlock(hashed24,4,4,0,c1);

              printf("\033[u");//Finally return to saved cursor position
              printf("\n");


    }



    //hashed string for characters more than 20 and less than 120

    else if(strlen(input)<120){

        const int s1=132;
        int lstIndex=131;

        unsigned char hashed132[s1];

        //calling hashing function
        hashString(hash132,hashed132,input,s1,lstIndex);
        //printf("%s\n%lu\n",hashed132,strlen(hashed132));
        int c1=colorcode;

        system("clear");//clear output screen


        //first printing the basic frame and position identification squares
        printBasicshape(c1,40);

        //save current cursor position after printing basic frame
        printf("\033[s");

        moveUp(37);
        moveRight(14);

        //print ch 101 to 116
        printBlock(hashed132,2,8,100,c1);

        moveLeft(12);

        //print ch 117 to 132
        printBlock(hashed132,8,2,116,c1);

        moveRight(12);
        moveUp(24);

        //print ch 1 to 100
        printBlock(hashed132,10,10,0,c1);

        //Finally return to saved cursor position
        printf("\033[u");
        printf("\n");

      }

      }
   return 0;
}




int binaryVal(int number,int index){
        int val=(number>>(8-index))&1;
        return val;
}


void hashString(char hash[],char hashed[],char input[],int s1,int lstIndex){

        int i;

        //ASCII value of the first character of the hashed string
        int firstletter=strlen(input)+50;

        //appending first character to the hashed string
        hash[0]=firstletter;


        for(i=1;i<s1;i++){//hashed string before increment
                if(i%strlen(input)==0){//append the final character of input
                    hash[i]=input[strlen(input)-1];
                }
                else{
                    hash[i]=input[(i%strlen(input))-1];
                }
        }


        //hashed string with increment part
        int inc=0;
        for(i=1;i<s1;i++){
                hash[i]=hash[i]+inc;
                if(i%strlen(input)==0){
                    inc++;//after one round incrementing the inc
                }
        }


        //make a copy of hashed array
        strcpy(hashed,hash);

        //reverse the error detection part
        int j;

        for(i=lstIndex,j=strlen(input)+1;i>strlen(input);i--,j++){
                hashed[i]=hash[j];
        }



}


void encodedPattern(int asciinumber,int color){
             int binaryval[9]={0};//array to store binary value

             int i,j;
             int number=asciinumber;

             for(i=0;i<9;i++){//store binary value to an array
                          binaryval[i]=binaryVal(number,i);
                    }


             int c1=47,c2=color;//Colors (c1 is white)

             for(i=0;i<3;i++){//print 3*3 square
                    for(j=0;j<3;j++){
                            int val;
                            val=binaryval[(i*3)+j];

                            if(val==0){
                                  printf("\x1b[%im  \x1b[0m",c1);
                              }
                            else if(val==1){
                                  printf("\x1b[%im  \x1b[0m",c2);
                              }
                           }
                       moveDown(1);
                       if(i<2){
                        moveLeft(6);
                       }

                   }


}


void printUsage(char programname[]){
                printf("usage :\n");
                printf("%s -c [black|red|green|yellow|blue|magenta|cyan]\n",programname);
                printf("%s -h for the help about the program\n",programname);
}


int colorCode(char color[]){
          if(!strcmp(color,"red"))return 41;
          if(!strcmp(color,"black"))return 40;
          if(!strcmp(color,"green"))return 42;
          if(!strcmp(color,"yellow"))return 43;
          if(!strcmp(color,"blue"))return 44;
          if(!strcmp(color,"cyan"))return 46;
          if(!strcmp(color,"magenta"))return 45;

          else return -1;
}


void printBasicshape(int color,int size){


    if(size==20){//small frame

    //making 2d array
    int array2d[20][20]={0};//initialize all values to 0


    int i,j;

    //adding values to 2d array
    for(i=0;i<20;i++){
        for(j=0;j<20;j++){
            //adding values for white border
            if((i==0)||(j==0)||(i==19)||(j==19)){
                array2d[i][j]=0;
            }

            //PI squares
            if(((i==1||i==6)&&((j<=6)&&(j!=0)))||((j==1||j==6)&&((i<=6)&&(i!=0)))||((i==13||i==18)&&((j<=6)&&(j!=0)))||((j==1||j==6)&&((i<=18)&&(i>=13)))||((i==1||i==6)&&((j<=18)&&(j>=13)))||((j==13||j==18)&&((i<=6)&&(i!=0)))){
                array2d[i][j]=1;
            }

            if(((i==3||i==4)&&((j==3)||(j==4)))||((i==3||i==4)&&((j==15)||(j==16)))||((j==3||j==4)&&((i==15)||(i==16)))){
                array2d[i][j]=1;

            }
        }
    }



    //printing basic shape
    for(i=0;i<20;i++){
        for(j=0;j<20;j++){

            if(array2d[i][j]==1){
                printf("\x1b[%im  \x1b[0m",color);
            }
            else if(array2d[i][j]==0){
                printf("\x1b[%im  \x1b[0m",47);
            }

        }
        printf("\n");
      }
    }


    else if(size==40){//large frame

    //making 2d array
    int array2d[38][38]={0};//initialize all values to 0


    int i,j;

    //adding values to 2d array
    for(i=0;i<38;i++){
        for(j=0;j<38;j++){
            //adding values for white border
            if((i==0)||(j==0)||(i==37)||(j==37)){
                array2d[i][j]=0;
            }

            //PI squares
            if(((i==1||i==6)&&((j<=6)&&(j!=0)))||((j==1||j==6)&&((i<=6)&&(i!=0)))||((i==31||i==36)&&((j<=6)&&(j!=0)))||((j==1||j==6)&&((i<=36)&&(i>=31)))||((i==1||i==6)&&((j<=36)&&(j>=31)))||((j==31||j==36)&&((i<=6)&&(i!=0)))){
                array2d[i][j]=1;
            }

            if(((i==3||i==4)&&((j==3)||(j==4)))||((i==3||i==4)&&((j==33)||(j==34)))||((j==3||j==4)&&((i==33)||(i==34)))){
                array2d[i][j]=1;

            }
        }
    }

    //printing basic shape
    for(i=0;i<38;i++){
        for(j=0;j<38;j++){

            if(array2d[i][j]==1){
                printf("\x1b[%im  \x1b[0m",color);
            }
            else if(array2d[i][j]==0){
                printf("\x1b[%im  \x1b[0m",47);
            }

        }
        printf("\n");
      }



    }




}


void printBlock(char hashed[],int rows,int columns,int firstindex,int color){
         int i,j;

         for(i=0;i<rows;i++){

                for(j=0;j<columns;j++){
                      encodedPattern(hashed[firstindex+(i*columns)+j],color);

                      //moving cursor 3 lines up
                      moveUp(3);
                }
                //moving cursor 3 lines down
                moveDown(3);
                moveLeft(6*columns);
              }

}


void moveUp(int lines){
    printf("\033[%iA",lines);
}


void moveRight(int spaces){
    printf("\033[%iC",spaces);
}


void moveDown(int lines){
    printf("\033[%iB",lines);
}


void moveLeft(int spaces){
    printf("\033[%iD",spaces);
}





