#include <stdio.h>  // stdlib is not needed to operate on files

int main(int argc, char const *argv[])
{
	FILE *stream;  // remember FILE* and not FILE
	stream=fopen("ayush","w");  // .dat for binary file not compulsory on unix systems
	int arr[2][2]={1,2,					// use of fwrite and fread is responsible for binary files
	             3,4};
	int i,j;
	printf("The original content of the array are: \n");
	
	for(i=0;i<2;i++)
	{
       for(j=0;j<2;j++)
       	printf("%d ",arr[i][j]);
	  printf("\n");
	}
	
	size_t os=sizeof(int); // object size
	size_t oc=4;   // object count
	size_t check;
	check=fwrite(&arr,os,oc,stream);
     if(check!=oc)
     	printf("Error writing data.\n");
     fclose(stream); // Closing is necessary, because the stream was initialized for "w" only, so a new stream will be needed for "r".

     printf("Rewriting the content of the array with 0's we have:\n");

    for(i=0;i<2;i++)
	{
       for(j=0;j<2;j++)
       {
       	arr[i][j]=0;
       	printf("%d ",arr[i][j]);
	    }
	  printf("\n");
	}
	 
	 printf("Upon retriving the content of the array we have: \n");
	
	stream=fopen("ayush","r");
	check=fread(&arr,os,oc,stream);
    if(check!=oc)
     	printf("Error reading data.\n");
     

    for(i=0;i<2;i++)
	{
       for(j=0;j<2;j++)
       	printf("%d ",arr[i][j]);
	  printf("\n");
	}
	 
	 fclose(stream); 


	/*printf("Enter the file name: \n");  
	char name[10];
	scanf("%s",name);
	FILE *stream;
	stream=fopen(name,"w");  
    fprintf(stream,"This is a test."); // this is ascii form of writing data
    int error=fclose(stream);*/


	// system("mkdir ~/code/ayush"); // creating directories.
    //system("touch ~/code/ayush"); // totally acceptable form to create files in particular locations
    // [the above two are specific to linux distributions]

	
	/*printf("Enter the file name: \n");  // providing the complete path seems to generate the segmentation fault.
	char *loc;                      
	scanf("%s",loc);  // this is the dangerous line becoz there is no pointee: can cause error {..segemntation fault} with rest of the code
                              
	FILE *str;
	str=fopen(loc,"w");  // File is created if doenot exist already
    int error=fclose(str); */
	
	return 0;
}
