#include <config.h>
#include <common_capi.h>
#include <dlfcn.h>
#include <sched.h>

void print_threadConfig(void);
void print_threadAction(void);

void testFunc1(void);
void testFunc2(void);

void testFunc1(void)
{
}

void testFunc2(void)
{
}



void print_threadConfig(void)
{
    t_threadConfigList threadConfigList;
    common_memset(&threadConfigList, 0x00, sizeof(t_threadConfigList));
    config_getThreadConifg("inputfile/threadConfig.xml",&threadConfigList);

    int i = 0;
    fprintf(stdout, "count=%d\n", threadConfigList.threadCount);
    for( i=0; i < threadConfigList.threadCount; i++) {
        fprintf(stdout, "threadConfig[%d].id=%d\n", i, threadConfigList.list[i].id);
        fprintf(stdout, "threadConfig[%d].parentId=%d\n", i, threadConfigList.list[i].parentId);
    }

    common_free(threadConfigList.list);
}

void print_threadAction(void)
{
    t_threadActionListInfo  actionListInfo;
    common_memset(&actionListInfo, 0x00, sizeof(t_threadActionListInfo));

#if 0
    void* handle = dlopen(NULL, RTLD_LAZY);
#endif

    config_getThreadAction("inputfile/threadAction.xml", &actionListInfo);

    int threadCount = 0;
#if 0
    fprintf(stdout, "threadCount=%d\n", actionListInfo.threadCount);
    for( threadCount = 0; threadCount < actionListInfo.threadCount; threadCount++) {
        t_threadActionList* actionList = &actionListInfo.list[threadCount];
        fprintf(stdout, "threadID=%d arrayCount=%d\n", actionList->threadId, actionList->arrayCount);
        int arrayIdx = 0;
        for( arrayIdx = 0; arrayIdx < actionList->arrayCount; arrayIdx++) {
            t_threadAction* action = &actionList->array[arrayIdx];
            fprintf(stdout, "name=%s\n", action->name);
            fprintf(stdout, "%p\n", dlsym(handle, action->name)); 
        }
    }
#endif

    threadCount = 0;
    for( threadCount = 0; threadCount < actionListInfo.threadCount; threadCount++) {
        t_threadActionList* actionList = &actionListInfo.list[threadCount];
        common_free(actionList->array);
    }
    common_free(actionListInfo.list);

#if 0
    dlclose(handle);
#endif
}


int main (int argc, char *argv[])
{
    print_threadConfig();
    print_threadAction();

    return 0;
}
