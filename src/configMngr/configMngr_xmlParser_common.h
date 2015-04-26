#ifndef __CONFIGMNGR_XMLPARSER_COMMON_H__
#define __CONFIGMNGR_XMLPARSER_COMMON_H__

#include <expat.h>

/************************************************/
/*  Defines                                     */
/************************************************/
typedef int (*t_configMngr_xmlPaser_initalize)(void* pData);
typedef int (*t_configMngr_xmlPaser_finalize)(void* pData);
typedef void (*t_configMngr_xmlPaser_element_start)(void *userData, const XML_Char *name, const XML_Char *atts[]);
typedef void (*t_configMngr_xmlPaser_element_end)(void *userData, const XML_Char *name);
typedef void (*t_configMngr_xmlPaser_value_handler)( void *userData, const XML_Char *string, int len );

typedef struct _t_configMngr_xmlParserFunc {
    t_configMngr_xmlPaser_initalize init;
    t_configMngr_xmlPaser_finalize  cleanup;
    t_configMngr_xmlPaser_element_start element_start;
    t_configMngr_xmlPaser_element_end element_end;
    t_configMngr_xmlPaser_value_handler value_handler;
}t_configMngr_xmlParserFunc;

/************************************************/
/*  Prototypes                                  */
/************************************************/

#endif  /*__CONFIGMNGR_XMLPARSER_COMMON_H__*/
