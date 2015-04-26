#ifndef __CONFIG_XMLPARSER_H__
#define __CONFIG_XMLPARSER_H__

/************************************************/
/*  Defines                                     */
/************************************************/
typedef struct _t_threadConfig {
    int id;
    int parentId;
}t_threadConfig;

typedef struct _t_threadConfigList {
    int threadCount;
    t_threadConfig  list[10];
}t_threadConfigList;

typedef struct _t_threadAction {
    char name[256];
    struct _t_threadAction* next;
}t_threadAction;

typedef struct _t_threadActionList {
    int threadId;
    t_threadAction* top;
}t_threadActionList;

typedef struct _t_threadActionListInfo {
    int threadCount;
    t_threadActionList list[10];
}t_threadActionListInfo ;


/************************************************/
/*  Prototypes                                  */
/************************************************/
int config_xmlParser_getThreadConifg(const char* filename, t_threadConfigList* pList);
int config_xmlParser_getThreadAction(const char* filename, t_threadActionListInfo* pList);

#endif  /*__CONFIG_XMLPARSER_H__*/
