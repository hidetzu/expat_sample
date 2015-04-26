#include <config_xmlParser.h>
#include <common_capi.h>


int main (int argc, char *argv[])
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

    return 0;
}
