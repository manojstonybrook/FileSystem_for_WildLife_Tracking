/*
-------------------------------------------------------------------------------
Project = Ayush(WildHawk)
Author = Manoj Alwani
About = This is a low cost Animal Tracking System designed for a 24 hour 
health monitoring and emergency situation calling. 
-------------------------------------------------------------------------------
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>

struct FAT
{
	char hid[2];	//hid={h1,h2,h3,...,h9}
       	long offset;    	
};

struct HMonitor
{
        int temp;
        int pulseRate;	
};


void main()
{  
//Initialization
struct FAT WHMonitor,tmp ;
struct HMonitor one;
FILE *fp,*ft,*ff,*f,*fd;
long idxsize,recsize;
int another;
int choice,a,c;
char cmd[1024];
char HMonitorname[2],*h;
long pos,tail,cp,head,t;

//Filesystem creation

	ff=fopen("FAT.DAT","r+");//try reading a file if it exists

	if(ff==NULL)//if file doesnot exists
	{
		sprintf(cmd,"dd if=/dev/zero of=FAT.DAT bs=1 count=10000");
		system(cmd);
		
		ff=fopen("FAT.DAT","w+");//open file to write and read
		fseek(ff,0,SEEK_SET);
		tail=ftell(ff);

		fseek(ff,992,SEEK_SET);
		fwrite(&tail,sizeof(long),1,ff);


		fseek(ff,1000,SEEK_SET);
		head=ftell(ff);

		fseek(ff,996,SEEK_SET);
		fwrite(&head,sizeof(long),1,ff);
		
		if(ff==NULL)
  		{
			puts("cannot open file");//error
			exit(1);
  		}
	}
	else
	{
		
		fseek(ff,992,SEEK_SET);
		fread(&tail,sizeof(long),1,ff);
		printf("previous tail restored: %ld",tail);

		fseek(ff,996,SEEK_SET);
		fread(&head,sizeof(long),1,ff);
		printf("\nprevious head restored: %ld",head);

		
	}

	
	idxsize=sizeof(WHMonitor);
	recsize=sizeof(one);

//Welcome Note
sprintf(cmd,"clear");
system(cmd);

printf("\n\n\n\t\t\t!!Welcome to Ayush!!");
printf("\nChecks:");
printf("\n1.Plug in your Rx");
printf("\n2.New updates will go to updates.txt.");
printf("\n3.In case you don't have Rx manually write updates in Updates.txt.");
printf("\n4.Run rx.c in background or in another terminal this will enable Rx \n  to receive data wirelessly.");
printf("\n5.Advanced Mode hides the details, Command Prompt Mode shows all details. \n");
printf("\n\nAdvanced Mode/Command Prompt Mode(1/0):");
scanf("%d",&c);
//printf("\n\nAyush is loading its Control Panel...\nThis menu will be clear automatically in 4 seconds....\n");
//sleep(4);
sprintf(cmd,"clear");
system(cmd);

while(1)
{	if(c!=0)
	{
	sprintf(cmd,"clear");
	system(cmd);
	}
	//control panel
	printf("\n\n\n\t\t\t!!Ayush Control Panel!!\n");
	printf("\n1.Update Records.");
	printf("\n2.List Existing Records.");
	printf("\n3.Modify Existing Records.");
	printf("\n4.Delete Records.");
	printf("\n5.Open Help Manual.");
	printf("\n6.Open System Architecture.");

	printf("\n0.EXIT.");
	printf("\n\nGive No. Of Your Choice: ");

	fflush(stdin);
	scanf("%d",&choice);

//switching section

switch(choice)
{

case 1:
fd=fopen("updates.txt","r+");
printf("\nNew updates:\n");
while(1)
{

 	a=fscanf(fd,"%s %d %d\n",WHMonitor.hid,&one.temp,&one.pulseRate);
	if(a==-1)
	break;	
	
	printf("\n%s %d %d",WHMonitor.hid,one.temp,one.pulseRate);

	fseek(ff,head,SEEK_SET);
	fwrite(&one,recsize,1,ff);
	WHMonitor.offset=head;
	head=ftell(ff);

	fseek(ff,tail,SEEK_SET);
	fwrite(&WHMonitor,idxsize,1,ff);
	tail=ftell(ff);

}
printf("\n");
fclose(fd);
fd=fopen("updates.txt","w+");
fclose(fd);
break;



case 2:
rewind(ff);//set origin to the begining SEEK_SET

f=fopen("msg","w");
while( ftell(ff)<tail && fread(&WHMonitor,idxsize,1,ff)==1)
{	
	t=ftell(ff);
	pos=WHMonitor.offset;
	fseek(ff,pos,SEEK_SET);
	fread(&one,recsize,1,ff);
	if(c)
	fprintf(f,"\nHMonitor_ID: %s\t Temp: %d\t pulseRate:%d\t ",WHMonitor.hid,one.temp,one.pulseRate);
	else
	printf("\nHMonitor_ID: %s\t Temp: %d\t pulseRate:%d\t ",WHMonitor.hid,one.temp,one.pulseRate);	
		
	fseek(ff,t,SEEK_SET);

cp=ftell(ff);
if(cp==tail)
break;
}
fclose(f);
if(c)
{	
	printf("**************************************************");
	printf("\nNOTE:\nPlease close this xmessage window to proceed.\nDrag window from below to get full view.\n");
	printf("**************************************************\n");
	sprintf(cmd,"xmessage -button \"\" -file msg");
	system(cmd);
}


break;



case 3:
another=1;

while(another)
{
  another=0;
  printf("\nEnter name of HMonitor to modify:");
  scanf("%s",HMonitorname);
  printf("\nGiven name: %s",HMonitorname);

  rewind(ff);
  	while(ftell(ff)<tail && fread(&WHMonitor,idxsize,1,ff)==1)
       {
        	if(strcmp(WHMonitor.hid,HMonitorname)==0)
	  	{
			t=ftell(ff);
			pos=WHMonitor.offset;//getting offet pointer to data
			fseek(ff,pos,SEEK_SET);//setting the cursor in database
			fread(&one,recsize,1,ff);//reading data written at cursor**

			printf("\nEnter new HMonitor name, temp, pulseRate:");
			scanf("%s%d%d",WHMonitor.hid,&one.temp,&one.pulseRate);

			fseek(ff,pos,SEEK_SET);//setting the cursor in database
			fwrite(&one,recsize,1,ff);

			fseek(ff,t,SEEK_SET);
			fseek(ff,-idxsize,SEEK_CUR);
			fwrite(&WHMonitor,idxsize,1,ff);


			break;
	 	}
	

	}	
	fflush(stdin);

	if(c==0)
	{
	printf("\nModify another Record(1/0)");
	scanf("%d",&another);
	}

}


break;




case 4:
another=1;
while(another==1)
{
   another=0;
   printf("\nEnter name of HMonitor to delete:");
   scanf("%s",HMonitorname);
   
   
   rewind(ff);
   while(fread(&WHMonitor,idxsize,1,ff)==1)
	{
		pos=ftell(ff);
		if(strcmp(WHMonitor.hid,HMonitorname)==0)
	   	{
			if(c==0)
			printf("\nDeleting: %s...",WHMonitor.hid);
			
			fseek(ff,tail,SEEK_SET);
			fseek(ff,-idxsize,SEEK_CUR);
			fread(&WHMonitor,idxsize,1,ff);
			if(c==0)
			printf("\nUpgrading: %s...",WHMonitor.hid);

			fseek(ff,-idxsize,SEEK_CUR);	
			tail=ftell(ff);

			fseek(ff,pos,SEEK_SET);
			fseek(ff,-idxsize,SEEK_CUR);

			fread(&tmp,idxsize,1,ff);
			fseek(ff,-idxsize,SEEK_CUR);
			t=ftell(ff);
			
		pos=WHMonitor.offset;//getting offset pointer to data
		fseek(ff,pos,SEEK_SET);//setting the cursor in database
		fread(&one,recsize,1,ff);//reading data written at cursor

		fseek(ff,tmp.offset,SEEK_SET);
		fwrite(&one,recsize,1,ff);
		WHMonitor.offset=tmp.offset;

		fseek(ff,t,SEEK_SET);
		fwrite(&WHMonitor,idxsize,1,ff);

			break;
		}
	}
	fflush(stdin);

	if(c==0)
	{
	printf("Delete Another Record(1/0)");
	scanf("%d",&another);
	}
}
break;

case 5:

	sprintf(cmd,"xmessage -button \"\" -file Read_me");
	system(cmd);
break;

case 6:

	sprintf(cmd,"xmessage -button \"\" -file architecture");
	system(cmd);
break;


case 0:

	fseek(ff,992,SEEK_SET);
	fwrite(&tail,sizeof(long),1,ff);
	if(c==0)
	printf("\nwriting tail...: %ld",tail);
	
	fseek(ff,996,SEEK_SET);
	fwrite(&head,sizeof(long),1,ff);
	
	if(c==0)
	printf("\nwriting head...: %ld",head);

   	fclose(ff);

	sprintf(cmd,"clear");
	system(cmd);

   	exit(0);

}

}




}
