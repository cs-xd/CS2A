#include<sys/time.h>
#include"CSA.h"
int main(){
	struct timeval start;
	struct timeval end;
	gettimeofday(&start,NULL);
	CSA csa("/home/hongweihuo/data/Data_set_BWA/hg19_2.fa");
	//CSA csa("main.cpp");
	gettimeofday(&end,NULL);
	cout<<(int)(csa.GetN()/(1024*1024.0))<<"build time "<<(end.tv_sec-start.tv_sec)<<" "<<endl;
	long num=0;
	csa.Counting("ATTCGT",num);
	cout<<num<<endl;
	csa.Save("hg.6G.csa");
	return 0;
}
