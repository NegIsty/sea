/*int max_time = 20;
tproc tasks[] = {
    {1,0,3,3},
    {2,2,6,6},
    {3,4,4,4},
    {4,6,5,5},
    {5,8,2,2}
};*/

/*int max_time = 40;
tproc tasks[] = {
     //pid  //activation  //length  //remaining  //period
    {1,     0,            1,        1,           3       },
    {2,     0,            1,        1,           4       },
    {3,     0,            2,        2,           6       },
};*/


/* Question 20 */

/* Exemple RM 1 */
/*int max_time = 20;
tproc tasks[] = {
     //pid  //activation  //length  //remaining  //period
    {1,     0,            1,        1,           2       },
    {2,     0,            3,        3,           10      },
};*/
/* Même résultat */

/* Exemple RM 2 */
/*int max_time = 20;
tproc tasks[] = {
     //pid  //activation  //length  //remaining  //period
    {1,     0,            1,        1,           2       },
    {2,     0,            5,        5,           10      },
};*/
/* Même résultat */

/* Exemple EDF 1 */
/*int max_time = 40;
tproc tasks[] = {
     //pid  //activation  //length  //remaining  //period
    {1,     0,            1,        1,           4       },
    {2,     0,            2,        2,           6       },
    {3,     0,            3,        3,           8       },
};*/
/* Même résultat */

/* Exemple Effet Domino */
int max_time = 24;
tproc tasks[] = {
     //pid  //activation  //length  //remaining  //period
    {1,     0,            2,        2,           5       },
    {2,     0,            2,        2,           6       },
    {3,     0,            2,        2,           7       },
    {4,     0,            2,        2,           8       },
};
/* Même résultat pour EDF, mais pas pour RM */
