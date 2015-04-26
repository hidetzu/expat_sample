#include <config_xmlParser.h>
#include <common_capi.h>

void print_threadConfig(void);
void print_threadAction(void);

void print_threadConfig(void)
{
    t_threadConfigList threadConfigList;
    common_memset(&threadConfigList, 0x00, sizeof(t_threadConfigList));
    config_xmlParser_getThreadConifg("inputfile/threadConfig.xml",&threadConfigList);

    int i = 0;
    fprintf(stdout, "count=%d\n", threadConfigList.threadCount);
    for( i=0; i < threadConfigList.threadCount; i++) {
        fprintf(stdout, "threadConfig[%d].id=%d\n", i, threadConfigList.list[i].id);
        fprintf(stdout, "threadConfig[%d].parentId=%d\n", i, threadConfigList.list[i].parentId);
    }

}

void print_threadAction(void)
{
    t_threadActionListInfo  actionListInfo;
    common_memset(&actionListInfo, 0x00, sizeof(t_threadActionListInfo));
    config_xmlParser_getThreadAction("inputfile/threadAction.xml", &actionListInfo);


    int threadCount = 0;
    fprintf(stdout, "threadCount=%d\n", actionListInfo.threadCount);
    for( threadCount = 0; threadCount < actionListInfo.threadCount; threadCount++) {
        t_threadActionList* actionList = &actionListInfo.list[threadCount];
        fprintf(stdout, "threadID=%d arrayCount=%d\n", actionList->threadId, actionList->arrayCount);
        int arrayIdx = 0;
        for( arrayIdx = 0; arrayIdx < actionList->arrayCount; arrayIdx++) {
            t_threadAction* action = &actionList->array[arrayIdx];
            fprintf(stdout, "name=%s\n", action->name);
        }
    }

    threadCount = 0;
    for( threadCount = 0; threadCount < actionListInfo.threadCount; threadCount++) {
        t_threadActionList* actionList = &actionListInfo.list[threadCount];
        common_free(actionList->array);
    }
}


int main (int argc, char *argv[])
{
//    print_threadConfig();
    print_threadAction();

    return 0;
}