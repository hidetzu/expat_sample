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
static int xmlAccessor_threadAction_free(void* pData);
static int xmlAccesser_threadAction_copy(void* pToData,  void* pFromData);
static void expatAccesser_element_start(void *userData, const XML_Char *name, const XML_Char *atts[]);
static int expatAccesser_print(void* pParseData);

static t_expatAccessor_threadActionList* createActionList(int threadId);
static void deleteActionList(t_expatAccessor_threadActionList* pThis);
/************************************************/
/*  Gloval vars                                 */
/************************************************/
static const t_xmlAccesseInfo_expatAccessor s_xmlParseFunc = {
    .copy = xmlAccesser_threadAction_copy,
    .cleanup = xmlAccessor_threadAction_free,
    .element_start = expatAccesser_element_start,
    .element_end   = NULL,
    .value_handler = NULL,
};

/************************************************/
/*  PublicFunctions                             */
/************************************************/
t_xmlAccessor_parseData* xmlAccesser_threadAction_create(t_xmlAccesseInfo_expatAccessor* pFuncOperation)
{
    printf("%s start\n", __func__);

    t_expatAccessor_threadActionListInfo_parseData* pPaseData =
        (t_expatAccessor_threadActionListInfo_parseData*)common_malloc(sizeof(t_expatAccessor_threadActionListInfo_parseData));
    common_memset(pPaseData, 0x00, sizeof(t_expatAccessor_threadActionListInfo_parseData));

    *pFuncOperation = s_xmlParseFunc;

    return (t_xmlAccessor_parseData*)pPaseData;
}

static int xmlAccessor_threadAction_free(void* pData)
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
    return 0;
}

static int threadAction_copyActionList(
t_threadActionList* toData,
t_expatAccessor_threadActionList* fromData)
{
    toData->threadId = fromData->threadId;

    toData->arrayCount = fromData->arrayCount;
    toData->array =
        (t_threadAction*)common_malloc(sizeof(t_threadAction)*toData->arrayCount);
        

    t_expatAccessor_threadAction* actionFromData = fromData->top;
    int idx = 0;
    while(actionFromData) {
        strcpy(toData->array[idx].name, actionFromData->funcName);
    
        actionFromData = actionFromData->next;
        idx++;
    }
    return 0;
}

static int xmlAccesser_threadAction_copy(void* pToData,  void* pFromData)
{
    t_expatAccessor_threadActionListInfo_parseData* pData
        = (t_expatAccessor_threadActionListInfo_parseData*)pFromData;

    t_threadActionListInfo*  actionListInfo
        = (t_threadActionListInfo*)pToData;

    expatAccesser_print(pData);
    actionListInfo->threadCount = pData->threadCount;
    actionListInfo->list = common_malloc(sizeof(t_threadActionList) * actionListInfo->threadCount);
    
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
static int expatAccesser_print(void* pParseData)
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

static void expatAccesser_element_start(void *userData, const XML_Char *name, const XML_Char *atts[])
{
    t_expatAccessor_threadActionListInfo_parseData* pData =
        (t_expatAccessor_threadActionListInfo_parseData*)userData;

    printf("[ELEMENT] %s Start!\n", name);
    if(strcmp(name, "threadActionList") == 0 ) {
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

        return;
    }

    if(strcmp(name, "threadAction") == 0 ) {
        t_expatAccessor_threadActionList* actionList = pData->pTail;

        t_expatAccessor_threadAction* newAction = 
        (t_expatAccessor_threadAction*)common_malloc(sizeof(t_expatAccessor_threadAction));

        if(!actionList->top) {
            actionList->top = actionList->tail = newAction;
        } else {
            actionList->tail->next = newAction;
            actionList->tail = newAction;
        }
        newAction->next = NULL;

        int idx = 0;
        while(atts[idx] != NULL ) {
            if (strcmp(atts[idx], "functionName") == 0 ) {
                idx++;
                common_memcpy(newAction->funcName, atts[idx], strlen(atts[idx]));
                newAction->funcName[strlen(atts[idx])] = '\0';
                break;
            }
            idx++;
        }


        actionList->arrayCount++;
        return;
    }

    return;
}

static t_expatAccessor_threadActionList* createActionList(int threadId)
{
    t_expatAccessor_threadActionList* newActionList =
        (t_expatAccessor_threadActionList*)common_malloc(sizeof(t_expatAccessor_threadActionList));
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

