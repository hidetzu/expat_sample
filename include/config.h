#ifndef __CONFIG_H__
#define __CONFIG_H__

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
    int id;
    char name[256];
}t_threadAction;

typedef struct _t_threadActionList {
    int threadId;
    int arrayCount;
    t_threadAction* array;
}t_threadActionList;

typedef struct _t_threadActionListInfo {
    int threadCount;
    t_threadActionList list[10];
}t_threadActionListInfo ;


/************************************************/
/*  Prototypes                                  */
/************************************************/
int config_initalize(void);
int config_finalize(void);
int config_getThreadConifg(const char* filename, t_threadConfigList* pList);
int config_getThreadAction(const char* filename, t_threadActionListInfo* pList);

#endif  /*__CONFIG_H__*/
