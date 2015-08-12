int __stdcall CheckWin(unsigned char*);
int __stdcall GetBestMove(unsigned char*);
int __stdcall GetClassSize();
int __stdcall GetJinShou(unsigned char*);
int __stdcall GetLevel();
int __stdcall GetNodeNum();
int __stdcall StopThinking();
int __stdcall Vc3Solver(unsigned char*);

// *********************************************************************
// Input:
// *********************************************************************
// The functions with input "unsigned char*" have the same format:
// 
//   char[0] :
//   char[1] : the maxmum time in seconds for the computation: char[0]*256 + char[1]
//   char[2] : BestMove level:1-9, actually this is the search depth; if it is 0 ,try to find VCF.
//   char[3] :
//   char[4] :
//   char[5] :
//   char[6] :
//   char[7] :
//   char[8]   : the total number of stones in the board.
//   char[8+1] : the first stone on the board, // usually it is 7*15+7
//   char[8+2] : the second stone on the board:
//   ...
//   ...
//   char[8+totalnumberstone]: the last stone in the board.
// 
// *********************************************************************
// Output:
// *********************************************************************
// 
// 1
// *************************************
// int  GetBestMove(unsigned char*);
// The move value is stored at char[0],
// 
// 2
// **************************************
// int  Vc3Solver(unsigned char*);
// The move value is stored at char[0],
// 
// If there exists a VCF, returns 2000+;
// samples for use for  finding vcf:
// 
// 	unsigned char temp[260];
// 	temp[1]=(unsigned char)120;
// 	temp[2]=(unsigned char)5;
// 	temp[3]=(unsigned char)0;
// 	temp[4]=(unsigned char)0;
// 	temp[5]=(unsigned char)0;
// 	temp[6]=(unsigned char)2;//HashTableSize;0--8M, 1--16M,2--32M,
// 	temp[7]=(unsigned char)0;
// 
// 	int i;
// 	for(i=0;i<=qipu[0];i++)
// 	{
// 		temp[i+8]=qipu[i];
// 	}
// 
// 
// 	////call the function in the dll.
// 	int vc3=Vc3Solver(temp);
// 	VC4Chain[0]=0;
// 	if(vc3>=20000)  //found VCF,
// 	{
// 		VC4Chain[0]=vc3-20000+1;
// 		for(i=0;i<=vc3-20000;i++)
// 		{
// 			VC4Chain[i+1]=temp[i];//store VCF chain
// 		}
// 	}else // no VCF.
// 	{
// 	}
// 
// 
// 3
// **************************************
// int  GetJinShou(unsigned char*);
// char[0]: the total number of jinshou position in the board
// char[1]: first jinshou pos;
// char[2]: second jinshou pos
// ...
// char[totaljinshou]: the last jinshou position.
// 
// 4
// ***************************************
// int  GetNodeNum();
// returns the current number of nodes that has been searched.
// It could be used in the middle of computation.
// 
// 5
// ****************************************
// int  StopThinking();
// the program stop searching and returns 0
// 
// 6
// ****************************************
// int CheckWin():
// 0: no win no lose
// 3: the last stone is black and is jinshou, black lose,
// 4: win
// ****************************************
