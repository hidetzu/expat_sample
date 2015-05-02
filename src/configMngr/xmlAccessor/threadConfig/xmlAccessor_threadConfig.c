#include <common_capi.h>
#include <config.h>

#include "xmlAccessor_threadConfig.h"
#include "../expatAccessor/expatAccessor_element.h"

/************************************************/
/*  Defines                                     */
/************************************************/
typedef struct _t_expatAccessor_threadConfig_threadData {
    int  id;
    int  parentId;
}t_expatAccessor_threadConfig_threadData;

typedef struct _t_expatAccessor_threadConfig_parseData{
    t_xmlAccessor_parseData base;
    int threadCount;
    t_expatAccessor_threadConfig_threadData threadConfigList[10];
}t_expatAccessor_threadConfig_parseData;


/************************************************/
/*  Prototypes                                  */
/************************************************/
static int xmlAccesser_threadConfig_copy(void* pToData,  void* pFromData);
static int xmlAccessor_threadConfig_free(void* pData);
static void expatAccesser_element_start(void *userData, const XML_Char *name, const XML_Char *atts[]);
static int expatAccesser_print(void* pParseData);

/************************************************/
/*  Gloval vars                                 */
/************************************************/
static const t_xmlAccesseInfo_expatAccessor s_xmlParseFunc = {
    .copy  = xmlAccesser_threadConfig_copy,
    .cleanup = xmlAccessor_threadConfig_free,
    .element_start = expatAccesser_element_start,
    .element_end   = NULL,
    .value_handler = NULL,
};

/************************************************/
/*  PublicFunctions                             */
/************************************************/

/************************************************/
/*  PublicFunctions                             */
/************************************************/
t_xmlAccessor_parseData* xmlAccesser_threadConfig_create(t_xmlAccesseInfo_expatAccessor* pFuncOperation)
{
    t_expatAccessor_threadConfig_parseData* pParaseData =
        (t_expatAccessor_threadConfig_parseData*)common_malloc(sizeof(t_expatAccessor_threadConfig_parseData));
    common_memset(pParaseData, 0x00, sizeof(t_expatAccessor_threadConfig_parseData));

    *pFuncOperation = s_xmlParseFunc;

    return (t_xmlAccessor_parseData*)pParaseData;
}

/************************************************/
/*  PrivateFuncitons                            */
/************************************************/

static int xmlAccesser_threadConfig_copy(void* pToData,  void* pFromData)
{
    int i = 0;
    t_threadConfigList* pThreadConfigList = (t_threadConfigList*)pToData;
    t_expatAccessor_threadConfig_parseData* pPaseData = (t_expatAccessor_threadConfig_parseData*)pFromData;

    expatAccesser_print(pPaseData);

    pThreadConfigList->threadCount = pPaseData->threadCount;
    for( i = 0; i < pPaseData->threadCount; i ++) {
        pThreadConfigList->list[i].id = pPaseData->threadConfigList[i].id;
        pThreadConfigList->list[i].parentId = pPaseData->threadConfigList[i].parentId;
    }

    return 0;
}

static int xmlAccessor_threadConfig_free(void* pData)
{
    common_free(pData);
    return 0;
}

static int expatAccesser_print(void* pParseData)
{
    t_expatAccessor_threadConfig_parseData* pData = (t_expatAccessor_threadConfig_parseData*)pParseData;

    int i = 0;
    fprintf(stdout, "count=%d\n", pData->threadCount);
    for( i=0; i < pData->threadCount; i++) {
        fprintf(stdout, "threadConfig[%d].id=%d\n", i, pData->threadConfigList[i].id);
        fprintf(stdout, "threadConfig[%d].parentId=%d\n", i, pData->threadConfigList[i].parentId);
    }
    return 0;
}

static void expatAccesser_element_start(
    void *userData,
    const XML_Char *name,
    const XML_Char *atts[])
{
    t_expatAccessor_threadConfig_parseData* pData = (t_expatAccessor_threadConfig_parseData*)userData;
    printf("[ELEMENT] %s Start!\n", name);

    if(common_strcmp(name, "threadConfig") == 0 ) {

        int idx = 0;
        while(atts[idx] != NULL) {
            t_expatAccessor_threadConfig_threadData* threadConfig =
                &pData->threadConfigList[pData->threadCount];
            if(strcmp(atts[idx], "id") == 0 ) {
                idx++;
                threadConfig->id = common_atoi(atts[idx]);
            } else if(strcmp(atts[idx], "parentId") == 0 ) {
                idx++;
                threadConfig->parentId = common_atoi(atts[idx]);
            }
            idx++;
        }

        pData->threadCount++;

        return;
    }
}
