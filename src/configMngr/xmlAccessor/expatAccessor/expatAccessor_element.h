#ifndef __EXPATACCESSER_ELEMENT_H__
#define __EXPATACCESSER_ELEMENT_H__

/************************************************/
/*  Defines                                     */
/************************************************/
#define ELEMENT_BUF_SIZE   (256)

typedef enum _e_elementDataType {
    ELEMENTDATA_TYPE_NONE,
    ELEMENTDATA_TYPE_INT,
    ELEMENTDATA_TYPE_STRING,

    ELEMENTDATA_TYPE_MAX,
    ELEMENTDATA_TYPE_INVALID = -1,
}e_elementDataType;

typedef struct _t_elementData {
    char name[ELEMENT_BUF_SIZE];
    e_elementDataType type;
    union {
        int intData;
        char string[ELEMENT_BUF_SIZE];
    };
}t_elementData;

/************************************************/
/*  Prototypes                                  */
/************************************************/
int expatAccessor_element_initalize(char* name, e_elementDataType type, t_elementData* elmData);
void expatAccessor_element_cleanup(t_elementData* elmData);
int expatAccessor_element_setElementData(char* buf, t_elementData* elmData);
int expatAccessor_element_getElementData(void* buf, t_elementData* elmData);


#endif  /*__EXPATACCESSER_ELEMENT_H__*/
