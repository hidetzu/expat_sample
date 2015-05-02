#ifndef __XMLACCESSOR_COMMON_H__
#define __XMLACCESSOR_COMMON_H__

#include <expat.h>
#include <common_capi.h>

/************************************************/
/*  Defines                                     */
/************************************************/
#define MALLOC(type) ((type *)common_malloc(sizeof(type)))
#define MALLOC_ARRAY(number, type) \
        ((type *)common_malloc((number) * sizeof(type)))


typedef struct _t_xmlAccessor_parseData {
    int isError;
}t_xmlAccessor_parseData;

typedef struct _t_expatAccessor_startTagInfo {
    const char*  tagName;
    int (*executeFunc)(void *userData, const XML_Char *name, const XML_Char *atts[]);
}t_expatAccessor_startTagInfo;

typedef struct _t_xmlAccesseInfo_expatAccessor{
    int (*copy)(void* pToData, t_xmlAccessor_parseData* pFromData);
    void (*cleanup)(void* pData);
    const t_expatAccessor_startTagInfo* (*getStartTagInfoList)(unsigned int *listCount);
}t_xmlAccesseInfo_expatAccessor;

/************************************************/
/*  Prototypes                                  */
/************************************************/
const char* xmlAccessor_common_findAttributeValue(const XML_Char *atts[], const char* attributeName);

#endif  /*__XMLACCESSOR_COMMON_H__*/
