#ifndef __CONFIGMNGR_XMLPARSER_THREADACTION_H__
#define __CONFIGMNGR_XMLPARSER_THREADACTION_H__

#include <config_xmlParser.h>

#include "configMngr_xmlParser_common.h"
/************************************************/
/*  Defines                                     */
/************************************************/

/************************************************/
/*  Prototypes                                  */
/************************************************/
void* configMngr_xmlParser_threadConfig_create(t_configMngr_xmlParserFunc* pFuncOperation);
void configMngr_xmlParser_threadAction_copy(void* pToData,  void* pFromData);

#endif  /*__CONFIGMNGR_XMLPARSER_THREADACTION_H__*/
