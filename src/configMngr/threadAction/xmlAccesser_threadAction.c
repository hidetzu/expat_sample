#include <common_capi.h>

#include "xmlAccesser_threadAction.h"
#include "../expatAccesser/expatAccesser_element.h"
#include "../expatAccesser/expatAccesser_common.h"

/************************************************/
/*  Defines                                     */
/************************************************/
typedef struct _t_expatAccesser_threadAction {
    char funcName[ELEMENT_BUF_SIZE];
    struct _t_expatAccesser_threadAction* next;
}t_expatAccesser_threadAction;

typedef struct _t_expatAccesser_threadActionList {
    int threadId;
    int arrayCount;
    t_expatAccesser_threadAction* top;
    t_expatAccesser_threadAction* tail;
}t_expatAccesser_threadActionList;

typedef struct _t_expatAccesser_threadActionListInfo_parseData{
    int threadCount;
    int currentIdx;
    t_expatAccesser_threadActionList list[10];
    int isThreadActionParse;
    t_elementData  elementData;
}t_expatAccesser_threadActionListInfo_parseData;

/************************************************/
/*  Prototypes                                  */
/************************************************/
static int expatAccesser_initalize(void* pParseData);
static void expatAccesser_element_start(void *userData, const XML_Char *name, const XML_Char *atts[]);
static void expatAccesser_element_end(void *userData, const XML_Char *name);
static void expatAccesser_value_handler( void *userData, const XML_Char *string, int len );
static int expatAccesser_print(void* pParseData);
/************************************************/
/*  Gloval vars                                 */
/************************************************/
static const t_configMngr_xmlParserFunc s_xmlParseFunc = {
    .init = expatAccesser_initalize,
    .cleanup = NULL,
    .element_start = expatAccesser_element_start,
    .element_end   = expatAccesser_element_end,
    .value_handler = expatAccesser_value_handler,
};

static const t_elementType s_config_elementTypeTbl[] = {
    { "executeFunc", ELEMENTDATA_TYPE_STRING},

/*終端エントリ */
    { "",           ELEMENTDATA_TYPE_INVALID},
};

/************************************************/
/*  PublicFunctions                             */
/************************************************/
void* xmlAccesser_threadAction_create(t_configMngr_xmlParserFunc* pFuncOperation)
{
    printf("%s start\n", __func__);

    t_expatAccesser_threadActionListInfo_parseData* pPaseData =
        (t_expatAccesser_threadActionListInfo_parseData*)common_malloc(sizeof(t_expatAccesser_threadActionListInfo_parseData));
    common_memset(pPaseData, 0x00, sizeof(t_expatAccesser_threadActionListInfo_parseData));

    pPaseData->threadCount = 0;
    pPaseData->currentIdx  = 0;

    *pFuncOperation = s_xmlParseFunc;
    pFuncOperation->init(pPaseData);

    return pPaseData;
}

void xmlAccesser_threadAction_copy(void* pToData,  void* pFromData)
{
    t_expatAccesser_threadActionListInfo_parseData* pData
        = (t_expatAccesser_threadActionListInfo_parseData*)pFromData;
    t_threadActionListInfo* actionListIinfo = (t_threadActionListInfo*) pToData;

    expatAccesser_print(pData);

    actionListIinfo->threadCount = pData->threadCount;

    int threadCountMax = actionListIinfo->threadCount;
    int threadCount = 0;
    for(threadCount = 0; threadCount < threadCountMax; threadCount++) {
        t_expatAccesser_threadActionList* actionList_fromData = &pData->list[threadCount];
        t_threadActionList* actionList_toData = &actionListIinfo->list[threadCount];

        actionList_toData->array = common_malloc(sizeof(t_threadAction)*actionList_fromData->arrayCount);
        common_memset(actionList_toData->array, 0x00, sizeof(t_threadAction)*actionList_toData->arrayCount);

        t_expatAccesser_threadAction* action_fromData = actionList_fromData->top;
        actionList_toData->arrayCount = 0;
        while(action_fromData) {
            t_threadAction* action_toData = &actionList_toData->array[actionList_toData->arrayCount];
            common_memcpy(action_toData->name, action_fromData->funcName, common_strlen(action_fromData->funcName));
            action_toData->name[common_strlen(action_fromData->funcName)] ='\0';
            actionList_toData->arrayCount++;
            action_fromData = action_fromData->next;
        }

    }

    threadCount = 0;
    for(threadCount = 0; threadCount < threadCountMax; threadCount++) {
        t_expatAccesser_threadActionList* actionList = &pData->list[threadCount];
        t_expatAccesser_threadAction* action = actionList->top;

        if( NULL != action && NULL != action->next ) {
            t_expatAccesser_threadAction* tmp = action->next;
            while(tmp) {
                action->next = tmp->next;
                common_free(tmp);
                tmp = action->next;
            }
        
        }
        common_free(action);
        
    }

    common_free(pData);
}

/************************************************/
/*  PrivateFuncitons                            */
/************************************************/
static int expatAccesser_initalize(void* pParseData)
{
    t_expatAccesser_threadActionListInfo_parseData* pData
        = (t_expatAccesser_threadActionListInfo_parseData*)pParseData;
    common_memset(pParseData, 0x00, sizeof(t_expatAccesser_threadActionListInfo_parseData));
    pData->threadCount=0;
    pData->isThreadActionParse = 0;
    return 0;
}

static int expatAccesser_print(void* pParseData)
{
    t_expatAccesser_threadActionListInfo_parseData* pData
        = (t_expatAccesser_threadActionListInfo_parseData*)pParseData;

    int i = 0;
    fprintf(stdout, "count=%d\n", pData->threadCount);
    for( i=0; i < pData->threadCount; i++) {
        t_expatAccesser_threadActionList* actionList = &pData->list[i];
        fprintf(stdout, "threadList[%d].threadID=%d\n",
                i, actionList->threadId);
        t_expatAccesser_threadAction* action = NULL;
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
    t_expatAccesser_threadActionListInfo_parseData* pData =
        (t_expatAccesser_threadActionListInfo_parseData*)userData;

    if(!pData->isThreadActionParse)
        return;

    int idx = 0;
    for( idx = 0; s_config_elementTypeTbl[idx].type != ELEMENTDATA_TYPE_INVALID; idx++) {
        t_elementData* elemData = &pData->elementData;
        if(common_strcmp(elemData->name, s_config_elementTypeTbl[idx].name ) != 0 )
            continue;

        char buf[ELEMENT_BUF_SIZE];
        common_memset( buf, 0x00, len );
        common_memcpy( buf, string, len );
        buf[len] = '\0';

        printf("elemData->type=%d\n", elemData->type);
        printf("elementName=%s len=%d data=%s\n", elemData->name, len, buf );
        
        expatAccesser_element_setElementData(buf, elemData);
        break;
    }
}

static void expatAccesser_element_start(void *userData, const XML_Char *name, const XML_Char *atts[])
{
    t_expatAccesser_threadActionListInfo_parseData* pData =
        (t_expatAccesser_threadActionListInfo_parseData*)userData;

    printf("[ELEMENT] %s Start!\n", name);
    if(strcmp(name, "threadActionList") == 0 ) {
        pData->list[pData->currentIdx].threadId = common_atoi(atts[1]);
        pData->list[pData->currentIdx].top = NULL;
        pData->list[pData->currentIdx].tail = NULL;
        pData->list[pData->currentIdx].arrayCount=0;
        return;
    }

    if(strcmp(name, "threadAction") == 0 ) {
        t_expatAccesser_threadActionList* actionList = 
            &pData->list[pData->currentIdx];

        t_expatAccesser_threadAction* newAction = 
        (t_expatAccesser_threadAction*)common_malloc(sizeof(t_expatAccesser_threadAction));
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
    for( idx = 0; s_config_elementTypeTbl[idx].type != ELEMENTDATA_TYPE_INVALID; idx++) {
        if(common_strcmp(name, s_config_elementTypeTbl[idx].name ) != 0 )
            continue;

        t_elementData* elmData = &pData->elementData;
        expatAccesser_element_initalize((char*)name, s_config_elementTypeTbl[idx].type,elmData);
        break;
    }
}

static void expatAccesser_element_end(void *userData, const XML_Char *name)
{
    t_expatAccesser_threadActionListInfo_parseData* pData =
        (t_expatAccesser_threadActionListInfo_parseData*)userData;
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
    for( idx = 0; s_config_elementTypeTbl[idx].type != ELEMENTDATA_TYPE_INVALID; idx++) {
        t_elementData* elmData = &pData->elementData;
        if(common_strcmp(elmData->name, s_config_elementTypeTbl[idx].name ) != 0 )
            continue;

        t_expatAccesser_threadActionList* actionList = &pData->list[pData->currentIdx];
        t_expatAccesser_threadAction* action = actionList->tail;

        expatAccesser_element_getElementData(action->funcName, elmData);

        expatAccesser_element_cleanup(elmData);
        break;
    }
}
