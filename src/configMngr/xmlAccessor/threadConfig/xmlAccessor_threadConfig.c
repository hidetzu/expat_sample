#include <common_capi.h>
#include <config.h>
#include <sys/queue.h>

#include "xmlAccessor_threadConfig.h"
#include "../expatAccessor/expatAccessor_element.h"

/************************************************/
/*  Defines                                     */
/************************************************/
typedef struct _t_expatAccessor_threadConfig_threadData {
    int  id;
    int  parentId;
    struct _t_expatAccessor_threadConfig_threadData* next;
}t_expatAccessor_threadConfig_threadData;

typedef struct _t_expatAccessor_threadConfig_parseData{
    t_xmlAccessor_parseData base;
    int threadCount;
    t_expatAccessor_threadConfig_threadData* pTop;
    t_expatAccessor_threadConfig_threadData* pTail;

}t_expatAccessor_threadConfig_parseData;


/************************************************/
/*  Prototypes                                  */
/************************************************/
static int copy(void* pToData,  t_xmlAccessor_parseData* pFromData);
static void cleanup(void* pData);
static int print(void* pParseData);

static t_expatAccessor_threadConfig_threadData* createThreadData(const XML_Char* atts[]);
static int element_start_threadConfig(void *userData, const XML_Char *name, const XML_Char *atts[]);

static const t_expatAccessor_startTagInfo* getStartTagInfoList(unsigned int *pListCount);

/************************************************/
/*  Gloval vars                                 */
/************************************************/
static const t_xmlAccesseInfo_expatAccessor s_xmlParseFunc = {
    .copy  = copy,
    .cleanup = cleanup,
    .getStartTagInfoList = getStartTagInfoList,
};


static const t_expatAccessor_startTagInfo s_elementStartFunc[] = {
    { .tagName = "threadConfig", .executeFunc = element_start_threadConfig },

    { .tagName = NULL, .executeFunc = NULL },
};

/************************************************/
/*  PublicFunctions                             */
/************************************************/
t_xmlAccessor_parseData* xmlAccesser_threadConfig_create(t_xmlAccesseInfo_expatAccessor* pFuncOperation)
{
    t_expatAccessor_threadConfig_parseData* pParaseData =MALLOC(t_expatAccessor_threadConfig_parseData);
    common_memset(pParaseData, 0x00, sizeof(t_expatAccessor_threadConfig_parseData));

    *pFuncOperation = s_xmlParseFunc;

    return (t_xmlAccessor_parseData*)pParaseData;
}

/************************************************/
/*  PrivateFuncitons                            */
/************************************************/

static int copy(void* pToData,  t_xmlAccessor_parseData* pFromData)
{
    t_threadConfigList* pThreadConfigList = (t_threadConfigList*)pToData;
    t_expatAccessor_threadConfig_parseData* pPaseData = (t_expatAccessor_threadConfig_parseData*)pFromData;

    pThreadConfigList->threadCount = pPaseData->threadCount;
    print(pPaseData);

    pThreadConfigList->list = MALLOC_ARRAY(pPaseData->threadCount, t_threadConfig);
    common_memset(pThreadConfigList->list, 0x00,sizeof(t_threadConfig)*pPaseData->threadCount); 

    t_expatAccessor_threadConfig_threadData* pThreadData = pPaseData->pTop;
    int i = 0;
    while(pThreadData) {
        t_threadConfig* toThreadCofig = &pThreadConfigList->list[i];
        toThreadCofig->id = pThreadData->id;
        toThreadCofig->parentId = pThreadData->parentId;
    
        pThreadData = pThreadData->next;
    }

    return 0;
}

static void cleanup(void* pData)
{
    t_expatAccessor_threadConfig_parseData* pPaseData = (t_expatAccessor_threadConfig_parseData*)pData;
    t_expatAccessor_threadConfig_threadData* pTop = pPaseData->pTop;
    
    if(!pTop)
        goto TERMINAL;

    t_expatAccessor_threadConfig_threadData* pDelTarget = pTop->next;
    while(pDelTarget) {
        pTop->next = pDelTarget->next;
        common_free(pDelTarget);
        pDelTarget->next = pTop->next;
    }

    common_free(pTop);

TERMINAL:
    common_free(pData);
    return;
}

static int print(void* pParseData)
{
    t_expatAccessor_threadConfig_parseData* pData = (t_expatAccessor_threadConfig_parseData*)pParseData;

    int i = 0;
    fprintf(stdout, "count=%d\n", pData->threadCount);
    t_expatAccessor_threadConfig_threadData* pTmp = pData->pTop;
    while(pTmp) {
        fprintf(stdout, "threadConfig[%d].id=%d\n", i, pTmp->id);
        fprintf(stdout, "threadConfig[%d].parentId=%d\n", i, pTmp->parentId);
        pTmp = pTmp->next;
    }
    return 0;
}

static const t_expatAccessor_startTagInfo* getStartTagInfoList(unsigned int *pListCount)
{
    *pListCount = sizeof(s_elementStartFunc)/sizeof(s_elementStartFunc[0]);
    return s_elementStartFunc;  
}

static int element_start_threadConfig(void *userData, const XML_Char *name, const XML_Char *atts[])
{
    t_expatAccessor_threadConfig_parseData* pData = (t_expatAccessor_threadConfig_parseData*)userData;

    t_expatAccessor_threadConfig_threadData* newThreadData
        = createThreadData(atts);
    if(NULL == newThreadData) {
        return -1;
    }
    if(!pData->pTop) {
        pData->pTop = pData->pTail = newThreadData;
    } else {
        pData->pTail->next = newThreadData;
        pData->pTail = newThreadData;
    }

    pData->threadCount++;

    return 0;
}


static t_expatAccessor_threadConfig_threadData* createThreadData(const XML_Char* atts[])
{
    t_expatAccessor_threadConfig_threadData* newThreadData = MALLOC(t_expatAccessor_threadConfig_threadData);
    common_memset(newThreadData, 0x00, sizeof(t_expatAccessor_threadConfig_threadData));

    const char* value = xmlAccessor_common_findAttributeValue(atts, "Id");
    if( NULL == value )
        goto ERROR;

    newThreadData->id = common_atoi(value);

    value = xmlAccessor_common_findAttributeValue(atts, "parentId");
    if( NULL == value )
        goto ERROR;
    
    newThreadData->parentId = common_atoi(value);

    return newThreadData;
ERROR:
    common_free(newThreadData);
    return NULL;
}
