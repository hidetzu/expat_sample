#include <stdio.h>

#include <common_capi.h>

#include <config.h>

#include "./xmlAccessor/xmlAccessor_core.h"

/************************************************/
/*  Defines                                     */
/************************************************/

/************************************************/
/*  Prototypes                                  */
/************************************************/

/************************************************/
/*  Gloval vars                                 */
/************************************************/

/************************************************/
/*  PublicFunctions                             */
/************************************************/
int config_initalize(void)
{
    return 0;
}

int config_finalize(void)
{
    return 0;
}



int config_getThreadConifg(const char* filename, t_threadConfigList* pList)
{
    fprintf(stdout, "%s  start\n", __func__);

    return xmlAccesser_parser(filename ,(void*)pList);
}

int config_getThreadAction(const char* filename, t_threadActionListInfo* pList)
{
    fprintf(stdout, "%s  start\n", __func__);
    return xmlAccesser_parser(filename ,(void*)pList);
}

/************************************************/
/*  PrivateFunctions                            */
/************************************************/
