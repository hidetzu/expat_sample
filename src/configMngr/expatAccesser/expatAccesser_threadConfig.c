#include <stdio.h>
#include <stdlib.h>

#include <expat.h>
#include <common_capi.h>

#include "expatAccesser_common.h"
#include "expatAccesser_element.h"
#include "expatAccesser_threadConfig.h"

/************************************************/
/*  Defines                                     */
/************************************************/

/************************************************/
/*  Prototypes                                  */
/************************************************/

/************************************************/
/*  Gloval vars                                 */
/************************************************/
static const t_elementType s_config_elementTypeTbl[] = {
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
int expatAccesser_threadConfig_initalize(void* pParseData)
{
    t_expatAccesser_threadConfig_parseData* pData = (t_expatAccesser_threadConfig_parseData*)pParseData;
    common_memset(pParseData, 0x00, sizeof(t_expatAccesser_threadConfig_parseData));
    pData->threadCount=0;
    pData->isParse = 0;
    return 0;
}

int expatAccesser_threadConfig_print(void* pParseData)
{
    t_expatAccesser_threadConfig_parseData* pData = (t_expatAccesser_threadConfig_parseData*)pParseData;

    int i = 0;
    fprintf(stdout, "count=%d\n", pData->threadCount);
    for( i=0; i < pData->threadCount; i++) {
        fprintf(stdout, "threadConfig[%d].id=%d\n", i, pData->threadConfigList[i].id);
        fprintf(stdout, "threadConfig[%d].parentId=%d\n", i, pData->threadConfigList[i].parentId);
    }
    return 0;
}

void expatAccesser_threadConfig_value_handler( void *userData, const XML_Char *string, int len )
{
    t_expatAccesser_threadConfig_parseData* pData = (t_expatAccesser_threadConfig_parseData*)userData;
    if( !pData->isParse )
        return;

    if( !pData->isParse )
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
        
        expatAccesser_setElementData(buf, elemData);
        break;
    }
}

void expatAccesser_threadConfig_element_start(void *userData, const XML_Char *name, const XML_Char *atts[])
{
    t_expatAccesser_threadConfig_parseData* pData = (t_expatAccesser_threadConfig_parseData*)userData;
    printf("[ELEMENT] %s Start!\n", name);

    if(common_strcmp(name, "threadConfig") == 0 ) {
        pData->threadCount++;
        pData->isParse = 1;
        return;
    }

    if( !pData->isParse )
        return;

    int idx = 0;
    for( idx = 0; s_config_elementTypeTbl[idx].type != ELEMENTDATA_TYPE_INVALID; idx++) {
        if(common_strcmp(name, s_config_elementTypeTbl[idx].name ) != 0 )
            continue;

        t_elementData* elmData = &pData->elementData;
        expatAccesser_initalize((char*)name, s_config_elementTypeTbl[idx].type,elmData);
        break;
    }
}

void expatAccesser_threadConfig_element_end(void *userData, const XML_Char *name)
{
    t_expatAccesser_threadConfig_parseData* pData = (t_expatAccesser_threadConfig_parseData*)userData;
    printf("[ELEMENT] %s End!\n", name);
    if(strcmp(name, "threadConfig") == 0 ) {
        pData->isParse = 0;
        return;
    }
    if( !pData->isParse )
        return;

    int idx = 0;
    for( idx = 0; s_config_elementTypeTbl[idx].type != ELEMENTDATA_TYPE_INVALID; idx++) {
        t_elementData* elmData = &pData->elementData;
        if(common_strcmp(elmData->name, s_config_elementTypeTbl[idx].name ) != 0 )
            continue;

        int threadCount = pData->threadCount;
        t_expatAccesser_threadConfig_threadData* threadConfig = &pData->threadConfigList[threadCount];

        if(strcmp(elmData->name, "id") == 0 ) {
            expatAccesser_getElementData(&threadConfig->id, elmData);
        } else if( strcmp(elmData->name, "parentId") == 0){
            expatAccesser_getElementData(&threadConfig->parentId, elmData);
        }

        expatAccesser_cleanup(elmData);
        break;
    }
}
