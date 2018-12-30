#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <glpk.h>

#include "probleme.h"
#include "sacDos.h"
#include "simplexeGLPK.h"

int borne = 0;
int max(int a, int b)
{
    return (a > b) ? a : b;
}

//rstG rstFin;
//rstFin.sum = 0;
//rstFin.x = (double*)malloc(sizeof(double)*(probleme.nVar * 2));

/*
struct coefficient
{
    int index;
    int numRst;
    double val;
    double fVal;
};
*/
void sortCof(coef *listCof, int n, char type)
{
    coef tmpCof;
    tmpCof.val = 0;
    tmpCof.fVal = 0;
    tmpCof.numRst = 0;
    tmpCof.index = 0;
    
    int i=0,j=0;
    if(type == 'v')
    {
        for(i=0; i<n; i++)
        {
            for(j=1; j<n; j++)
            {
                if(listCof[j].val > listCof[j-1].val)
                {
                    tmpCof = listCof[j];
                    listCof[j] = listCof[j-1];
                    listCof[j-1] = tmpCof;
                }
            }
        }
    }
    if(type == 'i')
    {
        for(i=0; i<n; i++)
        {
            for(j=1; j<n; j++)
            {
                if(listCof[j].index < listCof[j-1].index)
                {
                    tmpCof = listCof[j];
                    listCof[j] = listCof[j-1];
                    listCof[j-1] = tmpCof;
                }
            }
        }
    }
    
}


int printRst(const coef *listCof, int n)
{
    int i = 0, sum = 0;
    printf("Heuristique qualite-prix :\n");
    for(i=0; i<n; i++)
    {
        printf("x%d = %d, ",listCof[i].index, listCof[i].numRst);
        sum += listCof[i].fVal*listCof[i].numRst;
        
    }
    printf("\nUtilite totale : %d\n", sum);
    return sum;
}

void borneCal(const prob_t* prob, coef *listCof)
{
    int i=0;
    for(i=0; i<prob->nVar; i++)
    {
        listCof[i].index = i+1;
        listCof[i].numRst = 0;
        listCof[i].val = prob->cont[0][i];
        listCof[i].fVal = prob->fonc[i];
    }
   
    sortCof(listCof,prob->nVar,'v');
   // printRst(listCof, 3);
    int interValue = 0;
    for(i=0; i<prob->nVar; i++)
    {
        if(!((listCof[i].val+interValue) > prob->valCont[0]))
        {
            listCof[i].numRst = (prob->valCont[0]-interValue)/listCof[i].val;
      
            interValue += listCof[i].numRst*listCof[i].val;
            
        }
    }
    
    sortCof(listCof,prob->nVar,'i');
    //printRst(listCof, 3);

}

int determineEntiers(rstG* rst, int n)
{
    int b_entier = 1;
    double fractpart=0, intpart=0;
    int i=0;
    for(i=0; i<n; i++)
    {
        fractpart = modf(rst->x[i], &intpart);
        if(fractpart != 0)
        {
            b_entier = 0;
            return b_entier;
        }
        
    }
    return b_entier;
}

//void solveSacDoc(rstG rst, int borne, int n)
//void solveSacDoc(prob_t* probleme, rstG* rst, coef* listCof, int* valList)
void solveSacDoc(prob_t* probleme, int* valList, int index, int* final)
{
   
    if(index > probleme->nVar)
        return;
    //double rstList[probleme->nVar + 1][probleme.valCont[0] + 1];
    //double rstList[50][probleme->nVar + 2];
    int i=0, j=0, sumW=0, sumV=0, nMax =0;
    //int[] f = new int[maxw+1];
    //int length = probleme->valCont[0];
    //int f[length + 1];
    for(j = 0; j < probleme->nVar ; j++)
    {
        sumW += valList[j] * probleme->cont[0][j];
        sumV += valList[j] * probleme->fonc[j];
    }
    printf("sumW = %d\n", sumW);
    printf("sumV = %d\n", sumV);
    if((!(sumW >  probleme->valCont[0])) && (sumV > borne) )
    {
        printf("change value");
        borne = sumV;
        final[probleme->nVar] =borne;
        
        for(j = 0; j < index ; j++)
            final[j] = valList[j];
        
        for(j = index; j < probleme->nVar ; j++)
            final[j] = 0;
    }
    else
    {
        nMax = probleme->valCont[0] / probleme->cont[0][index];
        printf("nMax = %d\n", nMax);
        
        for(i =0; i < nMax+1; i++)
        {
            valList[index] = i;
            solveSacDoc(probleme, valList, index+1, final);
            
        }
        //valList[index] = 0;
    }
    
    
  
    
  
    
    
    /*
    for( i=0;i<probleme->nVar;i++){
        //for( j=weight[i];j<f.length;j++){
        for( j=probleme->cont[0][i];j<probleme->valCont[0];j++){
            int preW = probleme->cont[0][i];
            f[j] = max(f[j], f[j-preW]+probleme->fonc[i]);
        }
    }

    printf("%d\n", f[length-1]);
    
    */
    
    
    /*rstList[0][0] = rst->sum;
    int i = 1;
    for (i = 1; i < probleme->nVar; i++ )
    {
        rstList[0][i] = rst->x[i-1];
    }
    
    printf("rstlist rst sum = %f, rst x1 = %f, x2 = %f, x3 = %f\n", rstList[0][0], rstList[0][1], rstList[0][2], rstList[0][3]);
    */
    
    
    
    
    /*
    int b_entier = determineEntiers(rst, probleme->nVar);;
    if((rst->sum > borne) && (b_entier == 1 ) )
    {
        borne = rst->sum;
        printf("best result is GLPKrst sum = %f, rst x1 = %f, x2 = %f, x3 = %f\n", rst->sum, rst->x[0], rst->x[1], rst->x[2]);
        return;
    }
    
    sortCof(listCof,probleme->nVar,'v');
    int i,j, n,k;
    int interValue = 0, hasValue = 0;
    for(i = 0; i < probleme->nVar; i++)
    {
        
        for(n = 0; n < listCof[i].numRst + 1; n++ )
        {
            for(j = i+1; j < probleme->nVar; j++)
            {
                
                for (k = 0; k <= i; k++)
                    hasValue += n*listCof[i].val;
                if(!((listCof[j].val+interValue + n*listCof[i].val) > probleme->valCont[0]))
                {
                    listCof[j].numRst = (probleme->valCont[0]-interValue)/listCof[j].val;
                
                    interValue += listCof[j].numRst*listCof[j].val;
                
                }
            }
        }
    }
    
    sortCof(listCof,probleme->nVar,'i');
    
    
    
    */
    
    //printf("rstlist rst sum = %f, rst x1 = %f, x2 = %f, x3 = %f\n", rstList[0][0], rstList[0][1], rstList[0][2], rstList[0][3]);
   /* int i = 0, j = 0;
    double fractpart=0, intpart=0;
    int b_entier = 0;
    if(rst->sum > borne)
    {
        b_entier = determineEntiers(rst, probleme->nVar);
        if(b_entier == 1)
        {
            borne = rst->sum;
            //rstFin.x = rst.x;
            printf("renew rst sum = %f, rst x1 = %f, x2 = %f, x3 = %f\n", rst->sum, rst->x[0], rst->x[1], rst->x[2]);
        }
        else
        {
            for(i=0; i < probleme->nVar; i++)
            {
                fractpart = modf(rst->x[i], &intpart);
                for(j = 0; j < intpart; j++ )
                {
                    rst->x[i] = j;
                    solveSacDoc(probleme, rst);
                
                }
            }
        }
    }
 
    printf("solveSacDoc rst sum = %f, rst x1 = %f, x2 = %f, x3 = %f\n", rst->sum, rst->x[0], rst->x[1], rst->x[2]);
   */
}

int main (int argc, char *argv[]) {
	char nomFichier[20];
	prob_t probleme = {0,0,0,NULL,NULL,NULL,NULL};
    //int borne = 0;
    
    //struct coefficient *listCof = (struct cofficient*)malloc(sizeof(struct coefficient)*probleme.nVar);
    int i=0;
    coef *listCof = (coef*)malloc(sizeof(coef)*(probleme.nVar));//[probleme.nVar+1] ;
    
    for(i = 0; i <probleme.nVar + 1; i++ )
    {
        listCof[i].index = 0;
        listCof[i].numRst = 0;
        listCof[i].val = 0;
        listCof[i].fVal = 0;
    }
    //coef listCof[probleme.nVar+1];
	strcpy(nomFichier, argv[1]);

	if (!lireProbleme(nomFichier, &probleme)) {
		afficherProbleme(probleme);
	}
	else {
		printf("Probleme lecture Probleme\n");
		return -1;
	}
    
    printf("Borne Initiale\n");
    borneCal(&probleme, listCof);
    borne = printRst(listCof, probleme.nVar);
    
    rstG rst;
    rst.sum = 0;
    rst.x = (double*)malloc(sizeof(double)*(probleme.nVar * 2));
    
    //int sum=0;
    //double *x = (double*)malloc(sizeof(double)*(probleme.nVar+2));
    //printf("probleme nVar = %d   %f\n", probleme.nVar,probleme.cont[0][0]);
    getSimplexeGLPK(&probleme,&rst);
    printf("rst sum = %f, rst x1 = %f, x2 = %f, x3 = %f\n", rst.sum, rst.x[0], rst.x[1], rst.x[2]);
    
    //solveSacDoc(rst, borne, probleme.nVar);
    /*double* rstList[50];
    
    rstList[0] = (double*)malloc(sizeof(double)*(probleme.nVar + 2 ) );
    globali++;
    
    rstList[0][0] = rst.sum;
    int i = 1;
    for(i = 1; i < probleme.nVar ; i++)
    {
        rstList[globali][i] = rst.x[i];
    }
    */
    
    int* valList = (int*)malloc(sizeof(int) * probleme.nVar+1);
    for(i=0; i<probleme.nVar; i++)
    {
        valList[i] = 0;
    }
    int* final = (int*)malloc(sizeof(int) * probleme.nVar + 2);
    for(i=0; i<probleme.nVar; i++)
    {
        final[i] = listCof[i].numRst;
    }
    final[probleme.nVar] = borne;
    //solveSacDoc(&probleme, &rst, listCof, valList);
    solveSacDoc(&probleme, valList, 0, final);
    
    for(i=0; i < probleme.nVar; i++)
    {
        printf("x%d = %d\t", i,final[i]);
    }
    
    printf("final rst = %d\n", final[probleme.nVar]);
    
    
    if(listCof != NULL)
        free(listCof);
    if(rst.x != NULL)
        free(rst.x);
   
	libererMemoireProbleme(probleme);

	return 0;
}
