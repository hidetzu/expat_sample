#ifndef __XMLACCESSER_THREADCONFIG_H__
#define __XMLACCESSER_THREADCONFIG_H__

#include "../xmlAccessor_common.h"

/************************************************/
/*  Defines                                     */
/************************************************/

/************************************************/
/*  Prototypes                                  */
/************************************************/
void* xmlAccesser_threadConfig_create(t_xmlAccesseInfo_expatAccessor* pFuncOperation);
void xmlAccesser_threadConfig_copy(void* pToData,  void* pFromData);

#endif  /*__XMLACCESSER_THREADCONFIG_H__*/
