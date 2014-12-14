#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

  
main()
{
	int fd; /* File descriptor for the port */
	struct termios options;
	FILE *fi,*fo;
	void *a;
	size_t size=1, count=1;
	int c=100;
	char cmd[1024];
	char t;


	fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);

/*O_RDWR read+write mode

O_NOCTTY flag tells UNIX that this program doesn't want to be the "controlling terminal" for that port. 
If we don't specify this then any input (such as keyboard abort signals and so forth) will affect our process.

O_NDELAY flag tells UNIX that this program doesn't care what state the DCD signal line is in - whether the 
other end of the port is up and running. If we do not specify this flag, your process will be put to sleep 
until the DCD signal line is the space voltage.
*/

      	if (fd == -1)
		perror("open_port: Unable to open /dev/ttyACM0 - ");
      	else
		fcntl(fd, F_SETFL, 0);

	tcgetattr(fd, &options);		//Get the current options for the port	
	cfsetispeed(&options, B1200);		//Set the baud rates to 1200.
	options.c_cflag |= (CLOCAL | CREAD);	//Enable the receiver and set local mode
    	tcsetattr(fd, TCSANOW, &options);	//Set the new options for the port


	fi=fopen("/dev/ttyACM0","r+");
	while(1)
	{
		
		while( !fread(&t,1,1,fi) );
		if(t==102)
		{

			while( !fread(&t,1,1,fi) );
			if(t==102)
			{
				while(1)
				{
					while( !fread(&t,1,1,fi) );
					if(t=='\n')
					{
						c=8;
						while(c>0)
						{	
							while( !fread(&t,1,1,fi) );
							if(t=='\n')
							c=c-1;
						}
						break;
					}


					{
						printf("%c",t);
						fo=fopen("updates.txt","a+");
						fseek(fo,0,SEEK_CUR);
						fwrite(&t,1,1,fo);
						fflush(fo);
						fclose(fo);
					}
						
				}
			}
		}
		
	}
	printf("\n");
	

}


 
 
