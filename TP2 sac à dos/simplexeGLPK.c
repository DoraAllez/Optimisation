#include "simplexeGLPK.h"

void getSimplexeGLPK(prob_t *probOrg, rstG *rst)
{
    glp_prob *prob1;
    int ia[1+probOrg->nVar];
    int ja[1+probOrg->nVar];
    double ar[1+probOrg->nVar];
    char colName[10];
    
    prob1 = glp_create_prob();
    glp_set_prob_name(prob1, "sacDos");
    
    glp_set_obj_dir(prob1,GLP_MAX);
    glp_add_rows(prob1, 1);
    glp_set_row_name(prob1, 1, "cont1");
    glp_set_row_bnds(prob1, 1, GLP_UP, 0.0, probOrg->valCont[0]);
    
    int i = 0;
    glp_add_cols(prob1, probOrg->nVar);
    
    for(i=0; i < probOrg->nVar; i++ )
    {
        sprintf(colName, "X%d", i+1);
        glp_set_col_name(prob1, i+1, colName);
        glp_set_col_bnds(prob1, i+1, GLP_LO, 0.0, 0.0);
        glp_set_obj_coef(prob1, i+1, probOrg->fonc[i]);
        
        ia[i+1] = 1, ja[i+1] = i+1, ar[i+1] = probOrg->cont[0][i];
        
    }
    
    glp_load_matrix(prob1, probOrg->nVar, ia, ja, ar);
    glp_simplex(prob1,NULL);
    
    rst->sum = glp_get_obj_val(prob1);
    for(i=0; i<probOrg->nVar; i++)
    {
        rst->x[i] = glp_get_col_prim(prob1, i+1);
      //  printf("%f\n",rst->x[i] );
    }
    //printf("%f\n",rst->sum );
    glp_delete_prob(prob1);

}

