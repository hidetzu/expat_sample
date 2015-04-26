#include <config.h>
#include <common_capi.h>

#include "xmlAccessor_threadAction.h"
#include "../expatAccessor/expatAccessor_element.h"
#include "../expatAccessor/expatAccessor_common.h"

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
}t_expatAccessor_threadActionList;

typedef struct _t_expatAccessor_threadActionListInfo_parseData{
    int threadCount;
    int currentIdx;
    t_expatAccessor_threadActionList list[10];
    int isThreadActionParse;
    t_elementData  elementData;
}t_expatAccessor_threadActionListInfo_parseData;

/************************************************/
/*  Prototypes                                  */
/************************************************/
static int xmlAccessor_threadAction_free(void* pData);
static void expatAccesser_element_start(void *userData, const XML_Char *name, const XML_Char *atts[]);
static void expatAccesser_element_end(void *userData, const XML_Char *name);
static void expatAccesser_value_handler( void *userData, const XML_Char *string, int len );
static int expatAccesser_print(void* pParseData);
/************************************************/
/*  Gloval vars                                 */
/************************************************/
static const t_xmlAccesseInfo_expatAccessor s_xmlParseFunc = {
    .copy = xmlAccesser_threadAction_copy,
    .cleanup = xmlAccessor_threadAction_free,
    .element_start = expatAccesser_element_start,
    .element_end   = expatAccesser_element_end,
    .value_handler = expatAccesser_value_handler,
};

static const t_elementType s_elementTypeTbl[] = {
    { "executeFunc", ELEMENTDATA_TYPE_STRING},

/*終端エントリ */
    { "",           ELEMENTDATA_TYPE_INVALID},
};

/************************************************/
/*  PublicFunctions                             */
/************************************************/
void* xmlAccesser_threadAction_create(t_xmlAccesseInfo_expatAccessor* pFuncOperation)
{
    printf("%s start\n", __func__);

    t_expatAccessor_threadActionListInfo_parseData* pPaseData =
        (t_expatAccessor_threadActionListInfo_parseData*)common_malloc(sizeof(t_expatAccessor_threadActionListInfo_parseData));
    common_memset(pPaseData, 0x00, sizeof(t_expatAccessor_threadActionListInfo_parseData));

    pPaseData->threadCount = 0;
    pPaseData->currentIdx  = 0;

    *pFuncOperation = s_xmlParseFunc;

    return pPaseData;
}

static int xmlAccessor_threadAction_free(void* pData)
{

    t_expatAccessor_threadActionListInfo_parseData* pPaseData =
        (t_expatAccessor_threadActionListInfo_parseData*)pData;

    int threadCount = 0;
    int threadCountMax = pPaseData->threadCount;
    for(threadCount = 0; threadCount < threadCountMax; threadCount++) {
        t_expatAccessor_threadActionList* actionList = &pPaseData->list[threadCount];
        t_expatAccessor_threadAction* action = actionList->top;

        if( NULL != action && NULL != action->next ) {
            t_expatAccessor_threadAction* tmp = action->next;
            while(tmp) {
                action->next = tmp->next;
                common_free(tmp);
                tmp = action->next;
            }
        
        }
        common_free(action);
        
    }

    common_free(pPaseData);
    return 0;
}

void xmlAccesser_threadAction_copy(void* pToData,  void* pFromData)
{
    t_expatAccessor_threadActionListInfo_parseData* pData
        = (t_expatAccessor_threadActionListInfo_parseData*)pFromData;
    t_threadActionListInfo* actionListIinfo = (t_threadActionListInfo*) pToData;

    expatAccesser_print(pData);

    actionListIinfo->threadCount = pData->threadCount;

    int threadCountMax = actionListIinfo->threadCount;
    int threadCount = 0;
    for(threadCount = 0; threadCount < threadCountMax; threadCount++) {
        t_expatAccessor_threadActionList* actionList_fromData = &pData->list[threadCount];
        t_threadActionList* actionList_toData = &actionListIinfo->list[threadCount];

        actionList_toData->array = common_malloc(sizeof(t_threadAction)*actionList_fromData->arrayCount);
        common_memset(actionList_toData->array, 0x00, sizeof(t_threadAction)*actionList_toData->arrayCount);

        t_expatAccessor_threadAction* action_fromData = actionList_fromData->top;
        actionList_toData->arrayCount = 0;
        while(action_fromData) {
            t_threadAction* action_toData = &actionList_toData->array[actionList_toData->arrayCount];
            common_memcpy(action_toData->name, action_fromData->funcName, common_strlen(action_fromData->funcName));
            action_toData->name[common_strlen(action_fromData->funcName)] ='\0';
            actionList_toData->arrayCount++;
            action_fromData = action_fromData->next;
        }

    }
}

/************************************************/
/*  PrivateFuncitons                            */
/************************************************/
static int expatAccesser_print(void* pParseData)
{
    t_expatAccessor_threadActionListInfo_parseData* pData
        = (t_expatAccessor_threadActionListInfo_parseData*)pParseData;

    int i = 0;
    fprintf(stdout, "count=%d\n", pData->threadCount);
    for( i=0; i < pData->threadCount; i++) {
        t_expatAccessor_threadActionList* actionList = &pData->list[i];
        fprintf(stdout, "threadList[%d].threadID=%d\n",
                i, actionList->threadId);
        t_expatAccessor_threadAction* action = NULL;
        action = actionList->top;
        while(action) {
            fprintf(stdout, "funcName=%s\n",
                    action->funcName);
            action = action->next;
        }
    }
    return 0;
}

static void expatAccesser_value_handler( void *userData, const XML_Char *string, int len )
{
    t_expatAccessor_threadActionListInfo_parseData* pData =
        (t_expatAccessor_threadActionListInfo_parseData*)userData;

    if(!pData->isThreadActionParse)
        return;

    int idx = 0;
    for( idx = 0; s_elementTypeTbl[idx].type != ELEMENTDATA_TYPE_INVALID; idx++) {
        t_elementData* elemData = &pData->elementData;
        if(common_strcmp(elemData->name, s_elementTypeTbl[idx].name ) != 0 )
            continue;

        char buf[ELEMENT_BUF_SIZE];
        common_memset( buf, 0x00, len );
        common_memcpy( buf, string, len );
        buf[len] = '\0';

        printf("elemData->type=%d\n", elemData->type);
        printf("elementName=%s len=%d data=%s\n", elemData->name, len, buf );
        
        expatAccessor_element_setElementData(buf, elemData);
        break;
    }
}

static void expatAccesser_element_start(void *userData, const XML_Char *name, const XML_Char *atts[])
{
    t_expatAccessor_threadActionListInfo_parseData* pData =
        (t_expatAccessor_threadActionListInfo_parseData*)userData;

    printf("[ELEMENT] %s Start!\n", name);
    if(strcmp(name, "threadActionList") == 0 ) {
        pData->list[pData->currentIdx].threadId = common_atoi(atts[1]);
        pData->list[pData->currentIdx].top = NULL;
        pData->list[pData->currentIdx].tail = NULL;
        pData->list[pData->currentIdx].arrayCount=0;
        return;
    }

    if(strcmp(name, "threadAction") == 0 ) {
        t_expatAccessor_threadActionList* actionList = 
            &pData->list[pData->currentIdx];

        t_expatAccessor_threadAction* newAction = 
        (t_expatAccessor_threadAction*)common_malloc(sizeof(t_expatAccessor_threadAction));
        if(!actionList->top) {
            actionList->top = actionList->tail = newAction;
            newAction->next = NULL;
        } else {
            actionList->tail->next = newAction;
            actionList->tail = newAction;
            newAction->next = NULL;
        }
        actionList->arrayCount++;
        pData->isThreadActionParse = 1;
        return;
    }

    if(!pData->isThreadActionParse)
        return;

    int idx = 0;
    for( idx = 0; s_elementTypeTbl[idx].type != ELEMENTDATA_TYPE_INVALID; idx++) {
        if(common_strcmp(name, s_elementTypeTbl[idx].name ) != 0 )
            continue;

        t_elementData* elmData = &pData->elementData;
        expatAccessor_element_initalize((char*)name, s_elementTypeTbl[idx].type,elmData);
        break;
    }
}

static void expatAccesser_element_end(void *userData, const XML_Char *name)
{
    t_expatAccessor_threadActionListInfo_parseData* pData =
        (t_expatAccessor_threadActionListInfo_parseData*)userData;
    printf("[ELEMENT] %s End!\n", name);
    if(strcmp(name, "threadActionList") == 0 ) {
        pData->threadCount++;
        pData->currentIdx++;
        return;
    }

    if(strcmp(name, "threadAction") == 0 ) {
        pData->isThreadActionParse = 0;
        return;
    }

    int idx = 0;
    for( idx = 0; s_elementTypeTbl[idx].type != ELEMENTDATA_TYPE_INVALID; idx++) {
        t_elementData* elmData = &pData->elementData;
        if(common_strcmp(elmData->name, s_elementTypeTbl[idx].name ) != 0 )
            continue;

        t_expatAccessor_threadActionList* actionList = &pData->list[pData->currentIdx];
        t_expatAccessor_threadAction* action = actionList->tail;

        expatAccessor_element_getElementData(action->funcName, elmData);

        expatAccessor_element_cleanup(elmData);
        break;
    }
}
