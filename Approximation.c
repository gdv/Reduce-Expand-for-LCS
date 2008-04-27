/* -------------------------------------------------------------------
   Program to compute the approximate solutions

   Arguments:
                 Instance FileName
	    Distribution FileName
	    Output of LongRun
	    Output Of Asymmetric
	    Output Of Symmetric
	    Output Of Strong Symmetric
	    Output Of Linear Symmetric

Version 1.0 of 08/24/1999

------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXSTR 40     /* maximum number of strings in an instance */
#define MAXLEN 1000    /* maximum length of a string */
#define MAXALPHABET 20 /* maximum size of the alphabet */
#define MAXINT 255


char alphabet[MAXALPHABET];
int mat[MAXLEN][MAXLEN];
int num_inst,num_seq,min_length,max_length;
int max_run,size_alphabet,type_run;
char s[MAXSTR][MAXLEN];
char min_alphabet,max_alphabet;

int Truelen(char *seq)
{
int i;
for(i=0;seq[i] <= max_alphabet && seq[i] >= min_alphabet &&	i<MAXLEN; i++)
    {}
return i;
}


int blocks(char *s)
{
  int i,j=1;
  int k;

  k=Truelen(s);
  for (i=1;i<k;i++)
    if (s[i]!=s[i-1])
      j++;
  return(j);
}




/* ---------------------------------------------------------
   This procedure receives the stream and the array expo of all
   exponents and compute the string represented by such array */
void ComputeSeq(char *actual_cand,char *base,int expo[MAXLEN])
{
		int i,j,k;

		for (i=0, k=0;i<Truelen(base);i++)
				for (j=1;j<=expo[i];j++,k++)
						actual_cand[k] = base[i];
		for (i=k ; i<MAXLEN; i++)
				actual_cand[i]='\0';
}

/* Get the alphabet from the distribution file */
void get_alphabet(FILE *disfil, char distrib[MAXLEN], int size)
{
  int i;
  char symbol, temp[MAXLEN];
 
  for(i=1;i<=size; i++)
    {
      fscanf(disfil,"%c%s\n", &symbol , temp);
      alphabet[i-1]=symbol;
    }
  for(i=1,max_alphabet=alphabet[0],min_alphabet=alphabet[0];i<size; i++)
    if (alphabet[i]>max_alphabet)
      max_alphabet=alphabet[i];
    else 
      if(alphabet[i]<min_alphabet)
	min_alphabet=alphabet[i];
  fclose(disfil);
}

void WriteOutputFile(char *bestlcs, FILE *fout)
{
    fputs(bestlcs,fout);
    fputs("\n",fout);
}

char *tournament(char *passing, int num_seq)
{
		int i,j;
		char tempstr[MAXSTR][MAXLEN];
		//		printf("Begin inizialization\n");
		for (i=0; i<num_seq; i++)
				strcpy(tempstr[i],s[i]);
		//		printf("End inizialization\n");
		for( ; num_seq>1; num_seq=(num_seq+1)/2)
				for (	i= num_seq % 2, j=i; j<num_seq ; j+=2, i++)
						{
								//								printf("Comparing seq %d and %d\n", j, j+1);
								ExactLcs2(passing,tempstr[j],tempstr[j+1]);
								strcpy(tempstr[i],passing);
						}
}
								 
								


/* This function returns 1 iff subseq is a subsequence of seq */
int is_subseq(char *seq, char *subseq)
{
  int j,pos;
  int len_seq,len_subseq;
  len_seq=Truelen(seq);
  len_subseq=Truelen(subseq);
    for (j=0, pos=0; j<len_subseq && pos<len_seq ; pos++)
	if (seq[pos]==subseq[j])
	    j++;
    if (j<len_subseq)
      return 0;
    else
      return 1;
}

/* This function returns 0 iff cand is a common subsequence of  all
sequences in the instance 
otherwise it give the sequence the does not allow 
a thdreding scheme */
int is_common_subseq(char *cand)  
{ 
  int i;  
 
   for (i=0;i<num_seq;i++) 
     if (is_subseq(s[i],cand)==0)
       return i+1;

   return 0;  
}

/* Dynamic programming procedure to compute the exact LCS of 2
   sequences */
int ExactLcs2(char lcs[MAXLEN], char *seq1, char *seq2)
{
    int i,j,length_lcs;
    for (i=0;i<MAXLEN;i++)
      for (j=0;j<MAXLEN;j++)
	mat[i][j]=0;
    /* compute the matrix */
    for (i=0;i<Truelen(seq1);i++)
      for (j=0;j<Truelen(seq2);j++)
	if (seq1[i]==seq2[j])
	    mat[i+1][j+1]=mat[i][j]+1;
	else
			if (mat[i][j+1]>mat[i+1][j])
					mat[i+1][j+1]=mat[i][j+1];
			else
					mat[i+1][j+1]=mat[i+1][j];
    i=Truelen(seq1);
    j=Truelen(seq2);
    length_lcs=mat[i][j];
    lcs[length_lcs]='\0';
    for (;length_lcs>0;length_lcs--)
	{
	    for (;mat[i][j]==mat[i][j-1];j--);
	    for (;mat[i][j]==mat[i-1][j];i--);
	    lcs[length_lcs-1]=seq1[i-1];
	    i--;
	    j--;
	}
    if   ((is_subseq(seq1,lcs)*is_subseq(seq2,lcs))==0) 
					{
							printf("Internal Error #0020\n");
							exit(20);
					}
    return Truelen(lcs);  
}

/* Expansion */
int Expand(char *cand, char *base)
{
    int j,l, r;
   int expo[MAXLEN];
	 //   int ampl[MAXLEN];
    int augmented;
    
    for (j=0;j<Truelen(base);j++)
		    expo[j]=1;
	   
	
    ComputeSeq(cand,base,expo);
    for (augmented=1;augmented!=0;)
				{
						augmented=0;
						for (j=0;j<Truelen(base);j++)
								{
										expo[j]=2*expo[j];
										ComputeSeq(cand,base,expo);
										if (is_common_subseq(cand)>0)
												{
														expo[j]=expo[j]/2;
														ComputeSeq(cand,base,expo);
												}
										else
												augmented=1;
								}
				}
		
		for (j=0;j<Truelen(base);j++)
				{
						for (l=expo[j], r = 2*l-1;  r>l; )
								{
										
										expo[j] = (l+r+ ((l+r) %2 ))/2;
										ComputeSeq(cand,base,expo);
										if (is_common_subseq(cand)>0)
												r=expo[j]-1;
										else 
												l=expo[j];
								}
						expo[j]=l;
				}
    ComputeSeq(cand,base,expo);
    if   (is_common_subseq(cand)>0) 
      {
	printf("Internal Error #0004\n");
	printf("Sequence %s is not a common subsequence\n", cand);
	printf("It is not a subsequence of sequence number %3d\n",
				 is_common_subseq(cand) -1);
	printf("Sequence %s\n", s[is_common_subseq(cand)-1]);
	exit(9);
      }
    return Truelen(cand);
}

int main(argc,argv)
    int argc;
char *argv[];
{
  FILE *fin,*file_apx,*file_exact, *disfil;
  int i,j,m,k,l,pos;
	int actual_length, length_stream;
  char distrib[250],approx[MAXLEN], exact_lcs[MAXLEN];
  char basic[MAXSTR][MAXLEN];
  char stream[MAXLEN], tempstr[MAXLEN];
  char actual_stream[MAXLEN];

  if (argc<4)
    {
      printf("Missing arguments\n");
      printf("List of arguments: filename of Instances\n");
      printf("filename of Distribution of probability\n");
      printf("filename of Approximate solutions\n");
      printf("filename of Exact solutions\n");
      exit(5);
    }
  if ((fin=fopen(argv[1],"r"))==NULL)
    {
      printf("File <%s>: access denied\n",argv[1]);
      exit(5);
    }
  if ((file_apx=fopen(argv[3],"w"))==NULL)
    {
      printf("File <%s>: access denied\n",argv[3]);
      fclose(fin);
      exit(5);
    }
  /* the first 8 lines of the file contain some informations about the 
     sequence in the instances */
  fscanf(fin,"%40c%d\n",&tempstr,&num_inst);
  fscanf(fin,"%40c%d\n",&tempstr,&num_seq);
  fscanf(fin,"%40c%d\n",&tempstr,&max_length);
  fscanf(fin,"%40c%d\n",&tempstr,&min_length);
  fscanf(fin,"%40c%d\n",&tempstr,&max_run);
  fscanf(fin,"%40c%d\n",&tempstr,&size_alphabet);
  fscanf(fin,"%40c%s\n",&tempstr,&distrib);
  fscanf(fin,"%40c%d\n",&tempstr,&type_run);
  
  printf("Num. instances: %d\n",num_inst);
  printf("Num. sequences: %d\n",num_seq);
  printf("Max length: %d\n",max_length);
  printf("Min. length: %d\n",min_length);
  printf("Max Run: %d\n",max_run);
  printf("Size: %d\n",size_alphabet);
  printf("Distrib File: %s\n",distrib);
  printf("Distrib type: %d\n",type_run);

 if ((disfil=fopen(argv[2],"r"))==NULL) 
    {
      printf("File <%s> cannot be accessed\n",distrib); 
      exit(5); 
    }  
  get_alphabet(disfil, distrib,size_alphabet);     
  
  if ((num_seq==2)  && (file_exact=fopen(argv[4],"w"))==NULL)
    {
      printf("File <%s>: access denied\n",argv[4]);
      fclose(fin);
      fclose(file_apx);
      exit(5);
    }
  for (j=1;j<=num_inst;j++)
    {
    	printf("Begin of instance %5d out of %6d\n",j,num_inst);
      strcpy(approx,"*");
      strcpy(exact_lcs,"*");
      for (i=0;i<num_seq;i++)
			 {
				  strcpy(basic[i],"*");
				  (void) fgets(s[i],MAXLEN,fin);
				  for(pos=1,strncpy(basic[i],s[i],1);pos<Truelen(s[i]);pos++)
						if (s[i][pos]!=s[i][pos-1])
							 strncat(basic[i],&s[i][pos],1);
			 }

			 strcpy(tempstr,"*");
			if (size_alphabet >2)
					{
							/* Compute the stream of the instance */
							/* the stream is computed by the tournament algorithm */
							(void) tournament(tempstr, num_seq);
							//			printf("tournament = %s\n", tempstr);
							//							printf("Output of tournament is %s\n", tempstr);
							if   (is_common_subseq(tempstr)>0) 
									{
											printf("Internal Error #0009\n");
											exit(10);
									}
							for(pos=1 ; pos < MAXLEN; pos++)
									stream[pos]='\0';
							for(pos=1, stream[0]=tempstr[0]; pos<Truelen(tempstr); pos++)
									if (tempstr[pos]!=tempstr[pos-1]) 
											strncat(stream,&tempstr[pos],1);
							length_stream = Truelen(stream);
							//			printf("Stream :%s of length %4d\n", stream, length_stream);
							for (k=0 ; k<=MAXLEN; k++)
									actual_stream[k] = '\0';
							for ( actual_length = 2 ; actual_length <= length_stream
												; actual_length++)
									{
											//			 printf("Length %3d out of %4d\n", actual_length,  length_stream);
											for (k=0 ; k<= length_stream -actual_length ; k++)
											{

													for (i=0 ; i< actual_length; i++)
															actual_stream[i] =  stream[k+i];
													//		printf("k = %4d actual_stream = %s\n", 	 k,actual_stream);
		//	printf("Expanding %s\n", actual_stream); 
													Expand(tempstr,actual_stream); 
													if (Truelen(tempstr) >= Truelen(approx))
															strcpy(approx,tempstr);
											}      
									}
							/* Streams of one symbol */
							for (i=0, strcpy(actual_stream,"*") ; i< size_alphabet; i++)
									{
											actual_stream[0]=alphabet[i];
											if (is_common_subseq(actual_stream)==0)
													{
															Expand(tempstr,actual_stream); 
															if (Truelen(tempstr) >= Truelen(approx))
																	strcpy(approx,tempstr);
													}
									}
					}
			else // binary alphabet
					{  int num_blocks;
							for (num_blocks=MAXLEN,i=0;i<num_seq;i++)
									{int t;
											t=blocks(s[i]);
											if (t < num_blocks)
													num_blocks=t;
									}
							
							for (k=0 ; k< num_blocks ; k++)
									{
											int flip;
											for (flip=0, j=0; j<= k ; j++, flip = 1-flip)
													stream[j]=alphabet[flip];
											stream[j]='\0';
													Expand(tempstr,stream); 
													if (Truelen(tempstr)>= Truelen(approx))
															strcpy(approx, tempstr);
											for (flip=1, j=0; j<= k ; j++, flip = 1-flip)
													stream[j]=alphabet[flip];
											stream[j]='\0';
													Expand(tempstr,stream); 
													if (Truelen(tempstr)>= Truelen(approx))
															strcpy(approx, tempstr);
									}
					}
      printf("%d instances out of %d\n",j,num_inst);
      WriteOutputFile(approx,file_apx);
      if (num_seq==2)
	{
	  ExactLcs2(exact_lcs,s[0],s[1]);
	  if   (is_common_subseq(exact_lcs)>0) 
	    {
	      printf("Internal Error #0007\n");
	      exit(9);
	    }
	WriteOutputFile(exact_lcs,file_exact);
	}
    }
  printf("Closing files\n");
  fclose(fin);
  fclose(file_apx);
  if (num_seq==2)
    fclose(file_exact);
  return 0;
}
