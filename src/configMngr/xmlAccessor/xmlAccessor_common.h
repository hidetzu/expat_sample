#ifndef __XMLACCESSOR_COMMON_H__
#define __XMLACCESSOR_COMMON_H__

#include <expat.h>

/************************************************/
/*  Defines                                     */
/************************************************/
typedef struct _t_xmlAccessor_parseData {
    int isError;
}t_xmlAccessor_parseData;

typedef struct _t_expatAccessor_startTagInfo {
    const char*  tagName;
    int (*executeFunc)(void *userData, const XML_Char *name, const XML_Char *atts[]);
}t_expatAccessor_startTagInfo;


typedef int  (*t_xmlAccesseor_dataCopy)(void* pToData, t_xmlAccessor_parseData* pFromData);
typedef int (*t_xmlAccessor_finalize)(void* pData);

typedef const t_expatAccessor_startTagInfo* (*t_xmlAccessor_getStartTagInfoList)(unsigned int *listCount);

typedef struct _t_xmlAccesseInfo_expatAccessor{
    t_xmlAccesseor_dataCopy copy;
    t_xmlAccessor_finalize  cleanup;
    t_xmlAccessor_getStartTagInfoList getStartTagInfoList;
}t_xmlAccesseInfo_expatAccessor;

/************************************************/
/*  Prototypes                                  */
/************************************************/
const char* xmlAccessor_common_findAttributeValue(const XML_Char *atts[], const char* attributeName);

#endif  /*__XMLACCESSOR_COMMON_H__*/
