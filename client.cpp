#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <fstream>

using namespace std;

int main()
{
int sock;
// client socket discriptor
unsigned int len;
char ch[3]="no";
char ch1[3];
struct sockaddr_in client;
if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
// client socket is created.
{
perror("socket: ");
exit(-1);
}
client.sin_family=AF_INET;
client.sin_port=htons(10000);
// initializing socket parameters
client.sin_addr.s_addr=INADDR_ANY;
bzero(&client.sin_zero,0);
// appending 8 byte zeroes to 'struct sockaddr_in', to make it equal in size with 'struct sockaddr'.
len=sizeof(struct sockaddr_in);
if((connect(sock,(struct sockaddr *)&client,len))==-1)
//connecting to client
{
perror("connect: ");
exit(-1);
}



char filename[7]="matrix";
ifstream file;
send(sock,&filename,sizeof(filename),0);
cout<<"Sending file matrix.txt"<<endl;
file.open("file.txt");
int x,row,col;
file>>row>>col;
send(sock,&row,sizeof(row),0);
send(sock,&col,sizeof(col),0);
while (file>>x){
	send(sock,&x,sizeof(x),0);
}
file.close();
while (1){
cout<<"Enter the choice: ";
int choice;
cin>>choice;
send(sock,&choice,sizeof(choice),0);
int sum,maxx,minn;
switch (choice){
	case 1:
		recv(sock,&sum,sizeof(sum),0);
		cout<<"Sum of matrix recieved: "<<sum<<endl;
		break;
	case 2:
		for (int i=0;i<row;i++){
			recv(sock,&sum,sizeof(sum),0);
			cout<<"Sum of row "<<i+1<<" recieved"<<sum<<endl;
		}
		break;
	case 3:
		for (int i=0;i<row;i++){
			recv(sock,&maxx,sizeof(maxx),0);
			recv(sock,&minn,sizeof(minn),0);
			cout<<"Maximum element of row "<<i+1<<" received: "<<maxx<<endl;
			cout<<"Minimum element of row "<<i+1<<" received: "<<minn<<endl;
		}
		break;
	case 4:	
		for (int i=0;i<col;i++){
			recv(sock,&sum,sizeof(sum),0);
			cout<<"Sum of column "<<i+1<<" recieved"<<sum<<endl;
		}
		break;				
	case 5:
		for (int i=0;i<col;i++){
			recv(sock,&maxx,sizeof(maxx),0);
			recv(sock,&minn,sizeof(minn),0);
			cout<<"Maximum element of column "<<i+1<<" received: "<<maxx<<endl;
			cout<<"Minimum element of column "<<i+1<<" received: "<<minn<<endl;
		}
		break;			
	case 6:
		int left,right;
		recv(sock,&left,sizeof(left),0);
		if (left==-1) cout<<"NOt a square matrix, left and right diagonal sum is not possible"<<endl;
		else{
			recv(sock,&right,sizeof(right),0);
			cout<<"Left diagonal sum: "<<left<<endl<<"Right diagonal sum: "<<right<<endl;
		}
		break;
	case 7:
		cout<<"Receiving transpose of matrix: "<<endl;
		int x;
		for (int i=0;i<col;i++){
			for (int j=0;j<row;j++){
				recv(sock,&x,sizeof(x),0);
				cout<<x<<" ";
			}
			cout<<endl;
		}
		break;
	case 8:
		recv(sock,&maxx,sizeof(maxx),0);
		recv(sock,&minn,sizeof(minn),0);
		cout<<"Maximum element of matrix: "<<maxx<<endl;
		cout<<"Minimum element of matrix: "<<minn<<endl;
		break;
}	
		
printf("\nTo exit...press 'no'\n");
scanf("%s",ch1);
char letter='y';
if((strcmp(ch,ch1))==0)
{
letter='n';
}
send(sock,&letter,sizeof(letter),0);
if (letter=='n') break;
}
close(sock);
exit(0);

}
