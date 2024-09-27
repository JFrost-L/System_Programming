#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include"ku_fs_input.h"

void checkPipe(int checkingPipe);
void checkSizePipe(int i);
void solvingProblem(int  argc, char* argv[]);

int main(int argc, char *argv[]){
	solvingProblem(argc, argv);
	return 0;
}

void checkPipe(int checkingPipe){
	if(checkingPipe== -1){
		perror("pipe Error");
		exit(EXIT_FAILURE);
	}
}
void checkSizePipe(int i){
	if(i>MAXS){
		i=MAXS;
	}else if(i<=0){
		i=2;
	}
}

void solvingProblem(int  argc, char* argv[]){
	char* str = argv[1];
	int i = (argv[2][0])-'0';
	int pipefd[2];
	int child_status;
	int checkingPipe = pipe(pipefd);
	checkPipe(checkingPipe);
	checkSizePipe(i);
	pid_t pid[i];

	int processExe = MAXS/i-1;//파이프 당 처리할 기본 string 개수
	int strLength = strlen(str);//패턴의 길이
	char *ptr;
	int index[MAXS]={0};//인덱스 저장 배열
	int count=0;
	
	for(int j=0;j<i;j++){
	    	if((pid[j]=fork())==0){//자식 프로세스
			close(pipefd[0]);//read는 필요 없으니 매번 close
			if(j==0){//첫 번째 파이프
				ptr = strstr(input, str);
				if(ptr!=NULL&&ptr<input+processExe){
					index[count++]=(int)(ptr-input);
					write(pipefd[1], index+(count-1), sizeof(4));//찾으면 보내기
				}
				while(ptr!=NULL){
					ptr=strstr(ptr+1, str);
					if(ptr!=NULL&&ptr<input+processExe){
						index[count++]=(int)(ptr-input);
						write(pipefd[1], index+(count-1), sizeof(4));
						//찾으면 보내기
					}else{
						break;
					}
				}
				break;
			}else if(j==i-1){//마지막 파이프
				ptr= strstr(input + j * processExe - (strlen(str)-1), str);
				if(ptr!=NULL){
					index[count++]=(int)(ptr-input);
					write(pipefd[1], index+(count-1), sizeof(4));
				}
				while(ptr!=NULL){
					ptr=strstr(ptr+1, str);
					if(ptr!=NULL){
						index[count++] = (int)(ptr-input);
						write(pipefd[1], index+(count-1), sizeof(4));
					}
				}
				break;
				
			}else{//사이 파이프
				ptr=strstr(input + j*processExe-(strlen(str)-1), str);
				if(ptr!=NULL&&ptr<input+processExe*(j+1)){
					index[count++]=(int)(ptr-input);
					write(pipefd[1], index+(count-1), sizeof(4));
				}
				while(ptr!=NULL){
					ptr=strstr(ptr+1, str);
					if(ptr!=NULL&&ptr<input+processExe*(j+1)){
						index[count++] = (int)(ptr-input);
						write(pipefd[1], index+(count-1), sizeof(4));
					}
				}
				break;
			}
			close(pipefd[1]);
			exit(EXIT_SUCCESS);

		}
	}	

	if(pid[0]!=0){//부모
		close(pipefd[1]);
		int k[MAXS] ={0};
		int counting=0;
		for(int j=0;j<i;j++){
			pid_t wpid = waitpid(pid[j], &child_status, 0);
			if(wpid==pid[j]){
				if(WIFEXITED(child_status)){
					while(read(pipefd[0], k+counting, sizeof(4))>0){
						counting++;
					}
				}else{
					printf("%d번째 자식 비정상 종료\n", j+1);
					break;
				}
			}
		}		
		for(int i=0;i<counting;i++){
			for(int j=i;j<counting;j++){
				if(k[i]>k[j]){
					int temp=k[i];
					k[i]=k[j];
					k[j]=temp;
				}
			}
			printf("%d\n", k[i]);
		}	
		close(pipefd[0]);
		exit(EXIT_SUCCESS);
	}
}
