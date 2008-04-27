/* -------------------------------------------------------------------
   Program name:   LCS_GEN

   random sequence generation

   arguments   filename 
               number of instances
	   number of sequences in each instance
	   max length of a sequence
	   min length of a sequence
	   maximum run
	   size of the alphabet
	   distribution of symbols (filename)
	   distribution of run lengths (1=uniform, 2=linear,
	   3=exponential)

------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define MAXSTR 100     /* maximum number of strings in an instance */
#define MAXLEN 1000    /* maximum length of a string */
#define MAXALPHABET 20 /* maximum size of the alphabet */
#define MAXINT 65535
#define MAXSEQ_PD 4
#define MAXLENGTH_PD 100

#define LONG 1
#define SHORT 0
/* Get the alphabet from the distribution file */


/* global variables */
int invertedlookup[101];
int minlength, maxlength, numseq, numinst, sizealphabet;
int  maxrun, max_alphabet, min_alphabet;
char alphabet[MAXALPHABET+1];
char mysequence[MAXLEN+1];


int Truelen(char *seq)
{
int i;
for(i=0;seq[i] <= max_alphabet && seq[i] >= min_alphabet &&	i<MAXLEN; i++)
    {}
return i;
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



int generate(int maxarg)
    /* gives a random integer between 1 and maxarg */
{
  int p;
  p=  (rand() % maxarg);
    return(p+1);
}


void gen_sequence(int nowlength, int distribrun)
{
	char oldsymbol;
	int ve, nowrun, nowpos, nextpos, k;
	oldsymbol='*';
	strcpy(mysequence, "*");
    
	for (nowpos=0 ; nowpos < nowlength; )
		{	      
			float rnd;
			for(ve=generate(100); (oldsymbol==alphabet[invertedlookup[ve]]); )
				ve=generate(100);

			switch (distribrun)
			{
			case 1 :
				/* uniform distribution */
				nowrun = generate(maxrun);
				break;
			case 2 :
				/* linear case */
				rnd = (float) generate(MAXINT)/MAXINT;
				for
					(nowrun=1 ;nowrun<maxrun &&
						 (rnd - (float)
						  (nowrun+1)*nowrun/(maxrun+1)*maxrun
						  + (float) 2*nowrun/maxrun > FLT_EPSILON) 
						 ; nowrun++) 
					{}
				break;
			case 3 :
				/* exponential distribution */
				nowrun=1;
			}
			
			nextpos = nowpos + nowrun;
			  
			
			if (nextpos > nowlength)
				nextpos = nowlength;
			
			oldsymbol=alphabet[invertedlookup[ve]];
			
			for(k= nowpos ; (k<nextpos) ; k++)
				mysequence[k]=oldsymbol;
			nowpos = nextpos;
		}
	
	
	
		  
}

int modify_sequence(char *base_sequence,  int length, int variability)
{
	int i,j;
	int where_mutation[MAXLEN]; 
	char what_mutation[MAXLEN];
	int number_mutation;
	
	number_mutation=generate(variability);
	for (i=0 ; i < number_mutation; i++)
		{
			where_mutation[i] = generate(length+1)-1;
			j=generate(200);
			if (j>100)
					what_mutation[i]='*';
			else
					what_mutation[i]=alphabet[invertedlookup[j]];
		}

	for (i=0; i<number_mutation; i++)
			if(where_mutation[i] < length)
					{
							if (what_mutation[i]='*')
									{
											for(j=i; j<length; j++)
													base_sequence[j]=base_sequence[j+1];
											base_sequence[length]='\0';
									}
							else
									base_sequence[where_mutation[i]]=what_mutation[i];
							length--;
					}
 	return(length);
}


		 
		 
		 



int main(argc,argv)
    int argc;
char *argv[];

{
    FILE *fout,*disfil;
    char symbol, temp[MAXLEN+1];
    int dist, test, shortorlong,variability;
    int i,j,k, nowlength, distribrun;
    
    
    if (argc<9)
	{
	    printf("At least one mandatory argument is missing\n");
	    printf("Arguments: filename\n");
	    printf("             number of  instances\n");
	    printf("             number of sequences in each instance\n");
	    printf("             max length of a sequence\n");
	    printf("             min length of a sequence\n");
	    printf("             maximum run\n");
	    printf("             size of the alphabet\n");
	    printf("             filename of the distribution of symbols\n");
	  printf("             distribution of the run\n");
	  exit(EXIT_FAILURE);
	}
    
    if ((fout=fopen(argv[1],"w"))==NULL)
	{
	    printf("File <%s> is present and overwriting is not allowed\n",argv[1]);
	    exit(EXIT_FAILURE);
	}
    

    if ((disfil=fopen(argv[8],"r"))==NULL)
	{
	    printf("File <%s> cannot be accessed\n",argv[8]);
	    exit(EXIT_FAILURE);
	}
  numinst=atoi(argv[2]);
  numseq=atoi(argv[3]);   
  if (numseq>=MAXSTR)
    {
	printf("Too many sequences for an instance\n");
	exit(EXIT_FAILURE);
    }
  maxlength=atoi(argv[4]);  
  if (maxlength >= MAXLEN)
      {
	  printf("Sequences too long\n");
	  exit(EXIT_FAILURE);
      }
  if   (numseq <= MAXSEQ_PD && maxlength <= MAXLENGTH_PD) 
      shortorlong= SHORT;
  else
      shortorlong= LONG;
  minlength=atoi(argv[5]);  
  variability=2*( maxlength - minlength);
  maxrun=atoi(argv[6]);   
  sizealphabet=atoi(argv[7]);

  distribrun=atoi(argv[9]);
  if (distribrun <1 || distribrun>3)
      {
	  printf("Incorrect distribution type\n");
	  exit(EXIT_FAILURE);
      }
   
  /* In NAMEFILE the first rows summarize the arguments given */
  fprintf(fout,"number of  instances                   : %d\n",numinst);
  fprintf(fout,"number of sequences in each instance   : %d\n", numseq);
  fprintf(fout,"max length of a sequence               : %d\n",maxlength);
  fprintf(fout,"min length of a sequence               : %d\n",minlength);
  fprintf(fout,"maximum run                            : %d\n",maxrun);
  fprintf(fout,"size of the alphabet                   : %d\n",sizealphabet);
  fprintf(fout,"filename of the distribution of symbols: %s\n",argv[8]);
  fprintf(fout,"distribution of the run                : %d\n",distribrun);



  for(i=1,j=0;i<=sizealphabet; i++)
      {
					test=fscanf(disfil,"%c%s\n", &symbol , temp);
					alphabet[i]=symbol;
					dist=atoi(temp);
					for (k=j+1;k<=j+dist;k++)
							{
									invertedlookup[k]=i;
							}
					j=j+dist;
			}					
	if (j!=100)
			{
					printf("Error in file %s ,  Sum of Probabilities:%d\n",  argv[8],j);
					exit(6);
			}
	
	
	for(i=1, max_alphabet=alphabet[0], min_alphabet=alphabet[0];
			i<sizealphabet ; i++)
			if (alphabet[i]>max_alphabet)
					max_alphabet=alphabet[i];
			else 
					if(alphabet[i]<min_alphabet)
							min_alphabet=alphabet[i];	  
	
	srand( (unsigned)time( NULL ) );

	if (shortorlong == SHORT)
			for (i=1; i<=numinst;  i++)
					{
							for ( j=1;  j<=numseq;  j++)
									{
											nowlength=minlength + generate(maxlength -
																										 minlength +1) -1;
											gen_sequence(nowlength, distribrun);
											for (k=0; k< nowlength; k++)
													fprintf(fout,"%c",mysequence[k]);
											fprintf(fout,"\n");
									}		  
					}
	else
			for (i=1; i<=numinst; i++)
					{
							char base_sequence[MAXLEN+1];
							gen_sequence(minlength- variability, distribrun);
							strcpy(base_sequence, mysequence);
							//							printf("Base : %s\n", base_sequence);
							for ( j=1;  j<=numseq;  j++)
									{

											strcpy(temp,base_sequence);
											nowlength = modify_sequence(temp,  maxlength,
																									variability);
											//											for (k=0; k< nowlength; k++)
											fprintf(fout,"%s\n", temp);
									}
					}
					 
  
  (void)  fclose(disfil);
  for(i=fclose(fout); i!=0; );  
  {}
  return 0;
}
