#include <common_capi.h>

#include "configMngr_xmlParser_threadAction.h"

/************************************************/
/*  Defines                                     */
/************************************************/

/************************************************/
/*  Prototypes                                  */
/************************************************/
/************************************************/
/*  Gloval vars                                 */
/************************************************/
static const t_configMngr_xmlParserFunc s_xmlParseFunc = {
    .init = NULL,
    .cleanup = NULL,
    .element_start = NULL,
    .element_end   = NULL,
    .value_handler = NULL,
};

/************************************************/
/*  PublicFunctions                             */
/************************************************/

/************************************************/
/*  PublicFunctions                             */
/************************************************/
void* configMngr_xmlParser_threadAction_create(t_configMngr_xmlParserFunc* pFuncOperation)
{
     return NULL;
}

void configMngr_xmlParser_threadAction_copy(void* pToData,  void* pFromData)
{
}

/************************************************/
/*  PrivateFuncitons                            */
/************************************************/
