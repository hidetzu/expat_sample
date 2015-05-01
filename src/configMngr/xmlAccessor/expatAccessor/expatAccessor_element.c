
#include <common_capi.h>
#include "expatAccessor_element.h"

/************************************************/
/*  Defines                                     */
/************************************************/

typedef void (*t_elementData_setData)(char* buf, t_elementData* elmData);
typedef void (*t_elementData_getData)(void* buf, t_elementData* elmData);

typedef struct _t_elementDataFunc {
    t_elementData_setData setFunc;
    t_elementData_getData getFunc;
}t_elementDataFunc ; 

/************************************************/
/*  Prototypes                                  */
/************************************************/
static void set_elmentIntData(char* buf, t_elementData* elmData);
static void get_elmentIntData(void* buf, t_elementData* elmData);
static void set_elmentStringData(char* buf, t_elementData* elmData);
static void get_elmentStringData(void* buf, t_elementData* elmData);

/************************************************/
/*  Gloval vars                                 */
/************************************************/
const static t_elementDataFunc s_config_elementDataTbl[] = {
    [ELEMENTDATA_TYPE_INT] = {set_elmentIntData, get_elmentIntData },
    [ELEMENTDATA_TYPE_STRING] = {set_elmentStringData, get_elmentStringData },
};

/************************************************/
/*  PublicFunctions                             */
/************************************************/

/************************************************/
/*  PublicFunctions                             */
/************************************************/
int expatAccessor_element_initalize(char* name, e_elementDataType type, t_elementData* elmData)
{
    if(strlen(name)+1 > ELEMENT_BUF_SIZE )
        return -1;

    common_memset(elmData->name, 0x00, ELEMENT_BUF_SIZE);
    common_memcpy(elmData->name, name, common_strlen(name));
    elmData->name[common_strlen(name)]='\0';
    elmData->type = type;

    return 0;
}

void expatAccessor_element_cleanup(t_elementData* elmData)
{
    common_memset(elmData->name, 0x00, ELEMENT_BUF_SIZE);
    elmData->name[0]='\0';
}

int expatAccessor_element_setElementData(char* buf, t_elementData* elmData)
{
    s_config_elementDataTbl[elmData->type].setFunc(buf, elmData);
    return 0;
}

int expatAccessor_element_getElementData(void* buf, t_elementData* elmData)
{
    s_config_elementDataTbl[elmData->type].getFunc(buf, elmData);
    return 0;
}

/************************************************/
/*  PrivateFuncitons                            */
/************************************************/
static void set_elmentIntData(char* buf, t_elementData* elmData)
{
    elmData->intData = common_atoi(buf);
}

static void get_elmentIntData(void* buf, t_elementData* elmData)
{
    *(int*)buf = elmData->intData;
}

static void set_elmentStringData(char* buf, t_elementData* elmData)
{
    common_memcpy(elmData->string, buf, strlen(buf));
    elmData->string[common_strlen(buf)]='\0';
}

static void get_elmentStringData(void* buf, t_elementData* elmData)
{
    common_memcpy((char*)buf, elmData->string, common_strlen(elmData->string));
    ((char*)buf)[common_strlen(elmData->string)]='\0';
} 
