#ifndef __XMLACCESSER_THREADCONFIG_H__
#define __XMLACCESSER_THREADCONFIG_H__

#include <config_xmlParser.h>

#include "../xmlAccesser_common.h"

/************************************************/
/*  Defines                                     */
/************************************************/

/************************************************/
/*  Prototypes                                  */
/************************************************/
void* xmlAccesser_threadConfig_create(t_configMngr_xmlParserFunc* pFuncOperation);
void xmlAccesser_threadConfig_copy(void* pToData,  void* pFromData);

#endif  /*__XMLACCESSER_THREADCONFIG_H__*/
