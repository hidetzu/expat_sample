#ifndef __XMLACCESSER_THREADACTION_H__
#define __XMLACCESSER_THREADACTION_H__

#include "../xmlAccessor_common.h"
/************************************************/
/*  Defines                                     */
/************************************************/

/************************************************/
/*  Prototypes                                  */
/************************************************/
void* xmlAccesser_threadAction_create(t_xmlAccesseInfo_expatAccessor* pFuncOperation);
void xmlAccesser_threadAction_copy(void* pToData,  void* pFromData);

#endif  /*__XMLACCESSER_THREADACTION_H__*/
