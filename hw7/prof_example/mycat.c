#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
	const int SIZE=1024;
	int lineNum = 0,endLine = 0,tabReplace = 0,args = argc,num = 1,eolFlag=0,numRead=0,j=0,fd;
	char buf[SIZE];
	char c;
//CHECK FOR NO ARGS
	if(argc==1)
	{
		printf("Usage: %s -EnT file.txt\n",argv[0]);
		exit(0);
	}
//PARSE ARGS FOR SWITCHES
	while(args--)
	{
		if(argv[args][0]=='-')
		{
			int switchLen=(int)strlen(argv[args]);
			while(switchLen--)
			{
				if(argv[args][switchLen]=='n')lineNum=1;
				if(argv[args][switchLen]=='E')endLine=1;
				if(argv[args][switchLen]=='T')tabReplace=1;
			}
		}
	}	
//OPEN FILE
	fd = open( argv[argc-1], O_RDONLY );
	if ( fd == -1 )
	{
        	perror( argv[argc-1] ); 
        	exit(1);
	}
//READ FILE
	while((numRead=read(fd,buf,SIZE))>0)
	{
//PARSE BUFFER
	for(j=0;j<numRead;j++)
	{
		c=buf[j];
		if(lineNum&&(eolFlag||num==1))
		{
			printf("     %d  ",num++);
			eolFlag=0;
		}
		if(tabReplace&&c=='\t')
		{
			printf("^I");
		}
		else if(endLine&&c=='\n')
		{
			printf("$\n");
		}
		else
		{
			printf("%c",c);
		}
		if(lineNum&&c=='\n')
		{
			eolFlag=1;
		}
	}
	}
//CLOSE FILE
	close(fd);
	return 0;
}