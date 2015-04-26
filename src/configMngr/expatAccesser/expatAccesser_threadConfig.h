#ifndef __EXPATACCESSER_THREADCONFIG_H__
#define __EXPATACCESSER_THREADCONFIG_H__

#include <stdio.h>
#include <stdlib.h>

#include <expat.h>
#include <common_capi.h>
#include "expatAccesser_element.h"

/************************************************/
/*  Defines                                     */
/************************************************/
typedef struct _t_expatAccesser_threadConfig_threadData {
    int  id;
    int  parentId;
}t_expatAccesser_threadConfig_threadData;

typedef struct _t_expatAccesser_threadConfig_parseData{
    int threadCount;
    t_expatAccesser_threadConfig_threadData threadConfigList[10];
    int isParse;
    t_elementData  elementData;
}t_expatAccesser_threadConfig_parseData;

/************************************************/
/*  Prototypes                                  */
/************************************************/
int expatAccesser_threadConfig_initalize(void* pParseData);
void expatAccesser_threadConfig_element_start(void *userData, const XML_Char *name, const XML_Char *atts[]);
void expatAccesser_threadConfig_element_end(void *userData, const XML_Char *name);
void expatAccesser_threadConfig_value_handler( void *userData, const XML_Char *string, int len );
int expatAccesser_threadConfig_print(void* pParseData);

#endif  /*__EXPATACCESSER_THREADCONFIG_H__*/
