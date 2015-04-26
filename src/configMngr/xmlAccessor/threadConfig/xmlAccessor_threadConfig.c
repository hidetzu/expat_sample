#include <common_capi.h>
#include <config.h>

#include "xmlAccessor_threadConfig.h"
#include "../expatAccessor/expatAccessor_element.h"
#include "../expatAccessor/expatAccessor_common.h"

/************************************************/
/*  Defines                                     */
/************************************************/
typedef struct _t_expatAccessor_threadConfig_threadData {
    int  id;
    int  parentId;
}t_expatAccessor_threadConfig_threadData;

typedef struct _t_expatAccessor_threadConfig_parseData{
    int threadCount;
    t_expatAccessor_threadConfig_threadData threadConfigList[10];
    int isParse;
    t_elementData  elementData;
}t_expatAccessor_threadConfig_parseData;


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
static const t_xmlAccesseInfo_expatAccessor s_xmlParseFunc = {
    .init = expatAccesser_initalize,
    .cleanup = NULL,
    .element_start = expatAccesser_element_start,
    .element_end   = expatAccesser_element_end,
    .value_handler = expatAccesser_value_handler,
};

static const t_elementType s_elementTypeTbl[] = {
    { "id",         ELEMENTDATA_TYPE_INT},
    { "parentId",   ELEMENTDATA_TYPE_INT},

/*終端エントリ */
    { "",           ELEMENTDATA_TYPE_INVALID},
};

/************************************************/
/*  PublicFunctions                             */
/************************************************/

/************************************************/
/*  PublicFunctions                             */
/************************************************/
void* xmlAccesser_threadConfig_create(t_xmlAccesseInfo_expatAccessor* pFuncOperation)
{
    t_expatAccessor_threadConfig_parseData* pParaseData =
        (t_expatAccessor_threadConfig_parseData*)common_malloc(sizeof(t_expatAccessor_threadConfig_parseData));
    common_memset(pParaseData, 0x00, sizeof(t_expatAccessor_threadConfig_parseData));

    *pFuncOperation = s_xmlParseFunc;
    pFuncOperation->init(pParaseData);

    return pParaseData;
}

void xmlAccesser_threadConfig_copy(void* pToData,  void* pFromData)
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

    common_free(pPaseData);
}

/************************************************/
/*  PrivateFuncitons                            */
/************************************************/
static int expatAccesser_initalize(void* pParseData)
{
    t_expatAccessor_threadConfig_parseData* pData = (t_expatAccessor_threadConfig_parseData*)pParseData;
    common_memset(pParseData, 0x00, sizeof(t_expatAccessor_threadConfig_parseData));
    pData->threadCount=0;
    pData->isParse = 0;
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

static void expatAccesser_value_handler( void *userData, const XML_Char *string, int len )
{
    t_expatAccessor_threadConfig_parseData* pData = (t_expatAccessor_threadConfig_parseData*)userData;
    if( !pData->isParse )
        return;

    if( !pData->isParse )
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
    t_expatAccessor_threadConfig_parseData* pData = (t_expatAccessor_threadConfig_parseData*)userData;
    printf("[ELEMENT] %s Start!\n", name);

    if(common_strcmp(name, "threadConfig") == 0 ) {
        pData->threadCount++;
        pData->isParse = 1;
        return;
    }

    if( !pData->isParse )
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
    t_expatAccessor_threadConfig_parseData* pData = (t_expatAccessor_threadConfig_parseData*)userData;
    printf("[ELEMENT] %s End!\n", name);
    if(strcmp(name, "threadConfig") == 0 ) {
        pData->isParse = 0;
        return;
    }
    if( !pData->isParse )
        return;

    int idx = 0;
    for( idx = 0; s_elementTypeTbl[idx].type != ELEMENTDATA_TYPE_INVALID; idx++) {
        t_elementData* elmData = &pData->elementData;
        if(common_strcmp(elmData->name, s_elementTypeTbl[idx].name ) != 0 )
            continue;

        int threadCount = pData->threadCount;
        t_expatAccessor_threadConfig_threadData* threadConfig = &pData->threadConfigList[threadCount];

        if(strcmp(elmData->name, "id") == 0 ) {
            expatAccessor_element_getElementData(&threadConfig->id, elmData);
        } else if( strcmp(elmData->name, "parentId") == 0){
            expatAccessor_element_getElementData(&threadConfig->parentId, elmData);
        }

        expatAccessor_element_cleanup(elmData);
        break;
    }
}
