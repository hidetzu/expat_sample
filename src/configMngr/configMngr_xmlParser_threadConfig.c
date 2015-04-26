#include <common_capi.h>

#include "configMngr_xmlParser_threadConfig.h"
#include "./expatAccesser/expatAccesser_threadConfig.h"

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
    .init = expatAccesser_threadConfig_initalize,
    .cleanup = NULL,
    .element_start = expatAccesser_threadConfig_element_start,
    .element_end   = expatAccesser_threadConfig_element_end,
    .value_handler = expatAccesser_threadConfig_value_handler,
};

/************************************************/
/*  PublicFunctions                             */
/************************************************/

/************************************************/
/*  PublicFunctions                             */
/************************************************/
void* configMngr_xmlParser_threadConfig_create(t_configMngr_xmlParserFunc* pFuncOperation)
{
    t_expatAccesser_threadConfig_parseData* pParaseData =
        (t_expatAccesser_threadConfig_parseData*)common_malloc(sizeof(t_expatAccesser_threadConfig_parseData));
    common_memset(pParaseData, 0x00, sizeof(t_expatAccesser_threadConfig_parseData));

    *pFuncOperation = s_xmlParseFunc;
    pFuncOperation->init(pParaseData);

    return pParaseData;
}

void configMngr_xmlParser_threadConfig_copy(void* pToData,  void* pFromData)
{
    int i = 0;
    t_threadConfigList* pThreadConfigList = (t_threadConfigList*)pToData;
    t_expatAccesser_threadConfig_parseData* pParaseData = (t_expatAccesser_threadConfig_parseData*)pFromData;
    pThreadConfigList->threadCount = pParaseData->threadCount;
    for( i = 0; i < pParaseData->threadCount; i ++) {
        pThreadConfigList->list[i].id = pParaseData->threadConfigList[i].id;
        pThreadConfigList->list[i].parentId = pParaseData->threadConfigList[i].parentId;
    }

    common_free(pParaseData);
}

/************************************************/
/*  PrivateFuncitons                            */
/************************************************/
