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
int ssock,csock;
// creating server and client socket discriptor
int a,b,c;
unsigned int len;
struct sockaddr_in server,client;
// creating server & client socket object
if((ssock=socket(AF_INET,SOCK_STREAM,0))==-1)
// creating socket
{
perror("socket: ");
exit(-1);
}
server.sin_family=AF_INET;
server.sin_port=htons(10000);
// initializing server socket parameters.
server.sin_addr.s_addr=INADDR_ANY;
bzero(&server.sin_zero,0);
// appending 8 byte zeroes to 'struct sockaddr_in', to make it equal in size with 'struct sockaddr'.
len=sizeof(struct sockaddr_in);
if((bind(ssock,(struct sockaddr *)&server,len))==-1)
// binding port & IP
{
perror("bind error: ");
exit(-1);
}
if((listen(ssock,5))==-1)
// listening for client
{
perror("listen error: ");
exit(-1);
}
if((csock=accept(ssock,(struct sockaddr *)&client,&len))==-1)
// accepting connection
{
perror("accept error: ");
exit(-1);
}
int row,col,x;
ofstream file;
char filename[7];
recv(csock,&filename,sizeof(filename),0);
cout<<"Recieving file "<<filename<<endl;
file.open("copyfile.txt");
recv(csock,&row,sizeof(row),0);
recv(csock,&col,sizeof(col),0);
file<<row<<" "<<col<<"\n";
int mat[row][col];
int i=0,choice;
while (i<row){
	int j=0;
	while (j<col){
		recv(csock,&x,sizeof(x),0);
		mat[i][j]=x;		
		file<<x<<" ";
		j++;
	}
	file<<"\n";
	i++;
}
cout<<"Recieved file successfully"<<endl;
file.close();
cout<<"Enter a choice: "<<endl;
cout<<"1. Sum of all elements of matrix"<<endl<<"2. Show row-wise sum of matrix"<<endl<<"3. Show maximum and minimum element row-wise"<<endl<<"4. Show column-wise sum of matrix"<<endl<<"5. Show max and min element column-wise"<<endl<<"6. Show left-diagonal and right-diagonal sum"<<endl<<"7. Create transpose of matrix"<<endl<<"8. Show max and min element in matrix"<<endl;


while (1){
recv(csock,&choice,sizeof(choice),0);
cout<<"choice "<<choice<<" recieved"<<endl;
	if (choice==1){
		int sum=0;
		for (int i=0;i<row;i++){
			for (int j=0;j<col;j++){
				sum+=mat[i][j];
			}
		}
		send(csock,&sum,sizeof(sum),0);
		cout<<"Sum of matrix sent: "<<sum<<endl;
	}
	else if (choice==2){
		int i=0;
		while (i<row){
			int sum=0;
			for (int j=0;j<col;j++){
				sum+=mat[i][j];
			}
			send(csock,&sum,sizeof(sum),0);
			cout<<"Sum of row "<<i+1<<" sent: "<<sum<<endl;
			i++;
		}
	}
	else if (choice==3){
		for (int i=0;i<row;i++){
			int maxx=-1,minn=999999;			
			for (int j=0;j<col;j++){
				maxx=max(maxx,mat[i][j]);
				minn=min(minn,mat[i][j]);
			}
			send(csock,&maxx,sizeof(maxx),0);
			send(csock,&minn,sizeof(minn),0);
			cout<<"Maximum element of row "<<i+1<<" sent: "<<maxx<<endl;
			cout<<"Minimum element of row "<<i+1<<" sent: "<<minn<<endl;
		}	
	}				
	else if (choice==4){
		for (int i=0;i<col;i++){
			int sum=0;
			for (int j=0;j<row;j++){
				sum+=mat[j][i];
			}
			send(csock,&sum,sizeof(sum),0);
			cout<<"Sum of column "<<i+1<<" sent: "<<sum<<endl;
		}
		}
	else if (choice==5){
		for (int i=0;i<col;i++){
			int maxx=-1,minn=999999;			
			for (int j=0;j<row;j++){
				maxx=max(maxx,mat[j][i]);
				minn=min(minn,mat[j][i]);
			}
			send(csock,&maxx,sizeof(maxx),0);
			send(csock,&minn,sizeof(minn),0);
			cout<<"Maximum element of col "<<i+1<<" sent: "<<maxx<<endl;
			cout<<"Minimum element of col "<<i+1<<" sent: "<<minn<<endl;
		}
	}
	else if (choice==6){
		int notpossible=-1;
		if (row!=col){
			send(csock,&notpossible,sizeof(notpossible),0);
			cout<<"Not possible to compute left and right diagonal"<<endl;
		}
		else{
			int left=0,right=0;
			for (int i=0;i<row;i++){
				for (int j=0;j<col;j++){
					if (i==j) left+=mat[i][j];
					if (col-i-1==j) right+=mat[i][j];
				}
			}
			send(csock,&left,sizeof(left),0);
			send(csock,&right,sizeof(right),0);
			cout<<"Left diagonal sum sent: "<<left<<endl;
			cout<<"Right diagonal sum sent: "<<right<<endl;
		}
	}
	else if (choice==7){
		int transpose[col][row];
		for (int i=0;i<row;i++){
			for (int j=0;j<col;j++){
				transpose[j][i]=mat[i][j];
			}
		}
		for (int i=0;i<col;i++){
			for (int j=0;j<row;j++){
				send(csock,&transpose[i][j],sizeof(transpose[i][j]),0);
			}
		}
		cout<<"TRanspose of matrix sent!"<<endl;
	}
	else{
		int maxx=-1,minn=99999;
		for (int i=0;i<row;i++){
			for (int j=0;j<col;j++){
				maxx=max(maxx,mat[i][j]);
				minn=min(minn,mat[i][j]);
			}
		}
		send(csock,&maxx,sizeof(maxx),0);
		cout<<"Maximum element of matrix sent:"<<maxx<<endl;
		send(csock,&minn,sizeof(minn),0);
		cout<<"Minimum element of matrix sent:"<<minn<<endl;
		}
char letter;
recv(csock,&letter,sizeof(letter),0);
if (letter=='n' || letter=='N') break;
}
close(ssock);
}
