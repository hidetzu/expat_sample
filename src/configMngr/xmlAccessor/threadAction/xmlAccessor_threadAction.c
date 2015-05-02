#include <config.h>
#include <common_capi.h>
#include <string.h>

#include "xmlAccessor_threadAction.h"
#include "../expatAccessor/expatAccessor_element.h"

/************************************************/
/*  Defines                                     */
/************************************************/
typedef struct _t_expatAccessor_threadAction {
    char funcName[ELEMENT_BUF_SIZE];
    struct _t_expatAccessor_threadAction* next;
}t_expatAccessor_threadAction;

typedef struct _t_expatAccessor_threadActionList {
    int threadId;
    int arrayCount;
    t_expatAccessor_threadAction* top;
    t_expatAccessor_threadAction* tail;
    struct _t_expatAccessor_threadActionList* next;
}t_expatAccessor_threadActionList;

typedef struct _t_expatAccessor_threadActionListInfo_parseData{
    t_xmlAccessor_parseData base;
    int threadCount;
    t_expatAccessor_threadActionList* pTop;
    t_expatAccessor_threadActionList* pTail;
}t_expatAccessor_threadActionListInfo_parseData;

/************************************************/
/*  Prototypes                                  */
/************************************************/
static void cleanup(void* pData);
static int copy(void* pToData, t_xmlAccessor_parseData* pFromData);

static int print(void* pParseData);

static void actionList_addAction(t_expatAccessor_threadActionList* actionList,
        t_expatAccessor_threadAction* action);
static t_expatAccessor_threadActionList* createActionList(int threadId);
static void deleteActionList(t_expatAccessor_threadActionList* pThis);

static t_expatAccessor_threadAction* createAction(const XML_Char *atts[]);

static int elmentStart_threadActionList(void* userData,const XML_Char *name, const XML_Char *atts[]);
static int elmentStart_threadAction(void* userData,const XML_Char *name, const XML_Char *atts[]);
static const t_expatAccessor_startTagInfo* getStartTagInfoList(unsigned int *pListCount);

/************************************************/
/*  Gloval vars                                 */
/************************************************/
static const t_xmlAccesseInfo_expatAccessor s_xmlParseFunc = {
    .copy = copy,
    .cleanup = cleanup,
    .getStartTagInfoList = getStartTagInfoList,
};

static const t_expatAccessor_startTagInfo s_elementStartFunc[] = {
    { .tagName = "threadActionList", .executeFunc = elmentStart_threadActionList },
    { .tagName = "threadAction", .executeFunc = elmentStart_threadAction },

    { .tagName = NULL, .executeFunc = NULL },
};

/************************************************/
/*  PublicFunctions                             */
/************************************************/
t_xmlAccessor_parseData* xmlAccesser_threadAction_create(t_xmlAccesseInfo_expatAccessor* pFuncOperation)
{
    printf("%s start\n", __func__);

    t_expatAccessor_threadActionListInfo_parseData* pPaseData =MALLOC_ARRAY(1, t_expatAccessor_threadActionListInfo_parseData);
    common_memset(pPaseData, 0x00, sizeof(t_expatAccessor_threadActionListInfo_parseData));

    *pFuncOperation = s_xmlParseFunc;

    return (t_xmlAccessor_parseData*)pPaseData;
}

static void cleanup(void* pData)
{

    t_expatAccessor_threadActionListInfo_parseData* pPaseData =
        (t_expatAccessor_threadActionListInfo_parseData*)pData;

    t_expatAccessor_threadActionList* pTop = pPaseData->pTop;
    if(!pTop)
        goto TERMINAL;

    t_expatAccessor_threadActionList* pTmp = pTop->next;
    while(pTmp) {
        pTop->next = pTmp->next;
        deleteActionList(pTmp);
        pTmp = pTop->next;
    }

    deleteActionList(pTop);
TERMINAL:
    common_free(pPaseData);
    return;
}

static int threadAction_copyActionList(
t_threadActionList* toData,
t_expatAccessor_threadActionList* fromData)
{
    toData->threadId = fromData->threadId;

    toData->arrayCount = fromData->arrayCount;
    toData->array =MALLOC_ARRAY(toData->arrayCount, t_threadAction);

    t_expatAccessor_threadAction* actionFromData = fromData->top;
    int idx = 0;
    while(actionFromData) {
        strcpy(toData->array[idx].name, actionFromData->funcName);
    
        actionFromData = actionFromData->next;
        idx++;
    }
    return 0;
}

static int copy(void* pToData, t_xmlAccessor_parseData* pFromData)
{
    t_expatAccessor_threadActionListInfo_parseData* pData
        = (t_expatAccessor_threadActionListInfo_parseData*)pFromData;

    if(pData->base.isError)
        return -1;

    t_threadActionListInfo*  actionListInfo
        = (t_threadActionListInfo*)pToData;

    print(pData);
    actionListInfo->threadCount = pData->threadCount;
    actionListInfo->list = MALLOC_ARRAY(actionListInfo->threadCount, t_threadActionList);
    
    common_memset(actionListInfo->list, 0x00, sizeof(t_threadActionList)* 2);

    int idx = 0;

    t_expatAccessor_threadActionList* actionListFromData = pData->pTop;
    while(actionListFromData) {
        t_threadActionList* actionListToData = &actionListInfo->list[idx];

        threadAction_copyActionList(actionListToData, actionListFromData);
        actionListFromData = actionListFromData->next;

        idx++;
    }

    return 0;
}

/************************************************/
/*  PrivateFuncitons                            */
/************************************************/
static int print(void* pParseData)
{
    t_expatAccessor_threadActionListInfo_parseData* pData
        = (t_expatAccessor_threadActionListInfo_parseData*)pParseData;

    fprintf(stdout, "count=%d\n", pData->threadCount);
    t_expatAccessor_threadActionList* actionList = pData->pTop;

    while(actionList) {
        fprintf(stdout, "%d:actionList=%p\n", __LINE__, actionList);
        fprintf(stdout, "actionList.threadID=%d\n", actionList->threadId);
        fprintf(stdout, "actionList.arrayCount=%d\n", actionList->arrayCount);
        t_expatAccessor_threadAction* action = actionList->top;
        while(action) {
            fprintf(stdout, "action=%p\n",
                    action);
            fprintf(stdout, "funcName=%s\n",
                    action->funcName);
        
            action = action->next;
        }
        actionList=actionList->next;
    }

    return 0;
}

static const t_expatAccessor_startTagInfo* getStartTagInfoList(unsigned int *pListCount)
{
    printf("%s:%d:%s start \n", __FILE__,__LINE__, __func__);
    *pListCount = sizeof(s_elementStartFunc)/sizeof(s_elementStartFunc[0]);
    return s_elementStartFunc;  
}

static int elmentStart_threadActionList(void* userData,const XML_Char *name, const XML_Char *atts[])
{
    t_expatAccessor_threadActionListInfo_parseData* pData =
        (t_expatAccessor_threadActionListInfo_parseData*)userData;
    printf("%s:%d:%s start \n", __FILE__,__LINE__, __func__);

    t_expatAccessor_threadActionList* newActionList =
        createActionList(common_atoi(atts[1]));
    if(!pData->pTop) {
        pData->pTop = pData->pTail = newActionList;
    } else {
        pData->pTail->next = newActionList;
        pData->pTail = newActionList;
    }
    newActionList->next = NULL;
    pData->threadCount++;
    printf("%s:%d:%s end \n", __FILE__,__LINE__, __func__);
    return 0;
}

static int elmentStart_threadAction(void* userData,const XML_Char *name, const XML_Char *atts[])
{
    t_expatAccessor_threadActionListInfo_parseData* pData =
        (t_expatAccessor_threadActionListInfo_parseData*)userData;
    printf("%s:%d:%s start \n", __FILE__,__LINE__, __func__);

    t_expatAccessor_threadActionList* actionList = pData->pTail;
    t_expatAccessor_threadAction* newAction = createAction(atts);
    if( NULL == newAction ) {
        pData->base.isError = 1;
        return -1;
    }

    actionList_addAction(actionList, newAction);
    actionList->arrayCount++;
    printf("%s:%d:%s end \n", __FILE__,__LINE__, __func__);
    return 0;
}

static t_expatAccessor_threadAction* createAction(const XML_Char *atts[])
{
    t_expatAccessor_threadAction* newAction = MALLOC(t_expatAccessor_threadAction);

    const char* value = xmlAccessor_common_findAttributeValue(atts, "functionName");
    if( NULL == value )
        goto ERROR;

    if( strlen(value) > ELEMENT_BUF_SIZE-1 )
        goto ERROR;

    common_memcpy(newAction->funcName, value, strlen(value));
    newAction->funcName[strlen(value)] = '\0';

    return newAction;

ERROR:
    common_free(newAction);
    return NULL;
}

static void actionList_addAction(t_expatAccessor_threadActionList* actionList,
        t_expatAccessor_threadAction* action)
{
    if(!actionList->top) {
        actionList->top = actionList->tail = action;
    } else {
        actionList->tail->next = action;
        actionList->tail = action;
    }
    action->next = NULL;
}

static t_expatAccessor_threadActionList* createActionList(int threadId)
{
    t_expatAccessor_threadActionList* newActionList = MALLOC(t_expatAccessor_threadActionList);
    common_memset(newActionList, 0x00, sizeof(t_expatAccessor_threadActionList));
    newActionList->threadId = threadId;
    return newActionList;
}

static void deleteActionList(t_expatAccessor_threadActionList* pThis)
{
    t_expatAccessor_threadAction* pTop = pThis->top;
    if(!pTop)
        goto TERMINAL;

    t_expatAccessor_threadAction* pDelTarget = pTop->next;
    while(pDelTarget) {
        pTop->next = pDelTarget->next;
        common_free(pDelTarget);
        pDelTarget = pTop->next;
    }

    common_free(pTop);
TERMINAL:
    common_free(pThis);
}

