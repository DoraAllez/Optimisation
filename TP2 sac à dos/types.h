#ifndef _TYPES_H
#define _TYPES_H

typedef struct prob_type {
	int nVar;			/*nombre de variables*/
	int nCont;			/*nombre de contraintes*/
	int typeOpt;		/*type d'optimisation : 0 si min et 1 si max*/
	double *fonc;		/*fonction objectif, coefficients des xj*/
	double **cont;		/*contraintes, coefficients des aij*/
	int *signeCont;		/*signe de chaque contrainte, 0 si <=, 1 si >= */
	double *valCont;	/*valeur associee a chaque contrainte : bi*/
} prob_t;

typedef struct coefficient
{
    int index;
    int numRst;
    double val;
    double fVal;
}coef;

typedef struct rstGLPK
{
    double sum;
    double *x;
}rstG;

#endif
