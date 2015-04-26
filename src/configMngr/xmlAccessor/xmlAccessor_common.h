#ifndef __XMLACCESSER_COMMON_H__
#define __XMLACCESSER_COMMON_H__

#include <expat.h>

/************************************************/
/*  Defines                                     */
/************************************************/
typedef void  (*t_xmlAccesseor_dataCopy)(void* pToData, void* pFromData);
typedef int (*t_xmlAccessor_finalize)(void* pData);
typedef void (*t_xmlAccessor_element_start)(void *userData, const XML_Char *name, const XML_Char *atts[]);
typedef void (*t_xmlAccessor_element_end)(void *userData, const XML_Char *name);
typedef void (*t_xmlAccessor_value_handler)( void *userData, const XML_Char *string, int len );

typedef struct _t_xmlAccesseInfo_expatAccessor{
    t_xmlAccesseor_dataCopy copy;
    t_xmlAccessor_finalize  cleanup;
    t_xmlAccessor_element_start element_start;
    t_xmlAccessor_element_end element_end;
    t_xmlAccessor_value_handler value_handler;
}t_xmlAccesseInfo_expatAccessor;

/************************************************/
/*  Prototypes                                  */
/************************************************/

#endif  /*__XMLACCESSER_COMMON_H__*/
