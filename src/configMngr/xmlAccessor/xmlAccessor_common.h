#ifndef __XMLACCESSER_COMMON_H__
#define __XMLACCESSER_COMMON_H__

#include <expat.h>

/************************************************/
/*  Defines                                     */
/************************************************/
typedef int (*t_xmlAccesser_initalize)(void* pData);
typedef int (*t_xmlAccesser_finalize)(void* pData);
typedef void (*t_xmlAccesser_element_start)(void *userData, const XML_Char *name, const XML_Char *atts[]);
typedef void (*t_xmlAccesser_element_end)(void *userData, const XML_Char *name);
typedef void (*t_xmlAccesser_value_handler)( void *userData, const XML_Char *string, int len );

typedef struct _t_xmlAccesseInfo_expatAccessor{
    t_xmlAccesser_initalize init;
    t_xmlAccesser_finalize  cleanup;
    t_xmlAccesser_element_start element_start;
    t_xmlAccesser_element_end element_end;
    t_xmlAccesser_value_handler value_handler;
}t_xmlAccesseInfo_expatAccessor;

/************************************************/
/*  Prototypes                                  */
/************************************************/

#endif  /*__XMLACCESSER_COMMON_H__*/
