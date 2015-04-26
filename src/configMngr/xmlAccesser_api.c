#include <stdio.h>
#include <stdlib.h>

#include <expat.h>
#include <common_capi.h>

#include <config_xmlParser.h>

#include "xmlAccesser_core.h"

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
int config_xmlParser_getThreadConifg(const char* filename, t_threadConfigList* pList)
{
    fprintf(stdout, "%s  start\n", __func__);

    return xmlAccesser_parser(filename ,(void*)pList);
}

int config_xmlParser_getThreadAction(const char* filename, t_threadActionListInfo* pList)
{
    fprintf(stdout, "%s  start\n", __func__);
    return xmlAccesser_parser(filename ,(void*)pList);
}

/************************************************/
/*  PrivateFunctions                            */
/************************************************/
