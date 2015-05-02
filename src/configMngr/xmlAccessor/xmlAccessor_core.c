#include <stdio.h>
#include <stdlib.h>

#include <expat.h>
#include <common_capi.h>

#include "xmlAccessor_core.h"

#include "xmlAccessor_common.h"
#include "./threadConfig/xmlAccessor_threadConfig.h"
#include "./threadAction/xmlAccessor_threadAction.h"

/************************************************/
/*  Defines                                     */
/************************************************/
#define BUFSIZE 1024  

typedef t_xmlAccessor_parseData* (*t_xmlAccesseInfo_dataCreate)(t_xmlAccesseInfo_expatAccessor* pFileOperation);

typedef struct _t_xmlAccesseInfo_dataOperator {
    const char* filename;
    t_xmlAccesseInfo_dataCreate createFunc;
}t_xmlAccesseInfo_dataOperator;

typedef struct _t_xmlAccesseInfo {
    const char* filename;
    FILE* fp;
    XML_Parser parser;
    t_xmlAccessor_parseData* parseData;

    t_xmlAccesseInfo_expatAccessor expatAccessor;
}t_xmlAccesseInfo;

/************************************************/
/*  Prototypes                                  */
/************************************************/
static t_xmlAccesseInfo* createAccessInfo(const char* filename);
static void deleteAccessInfo(t_xmlAccesseInfo* pThis);

static void element_start(void *userData, const XML_Char *name, const XML_Char *atts[]);
static void element_end(void *userData, const XML_Char *name);
static void value_handler( void *userData, const XML_Char *string, int len );

/************************************************/
/*  Gloval vars                                 */
/************************************************/
static t_xmlAccesseInfo_dataOperator s_xmlFileInfoTbl[] = {
    { 
        .filename = "inputfile/threadConfig.xml",
        .createFunc = xmlAccesser_threadConfig_create,
    },

    { 
        .filename = "inputfile/threadAction.xml",
        .createFunc = xmlAccesser_threadAction_create,
    },


/* 終端エントリ */
    { 
        .filename = "",
        .createFunc = NULL,
    }
};

/************************************************/
/*  PublicFunctions                             */
/************************************************/
int xmlAccesser_parser(const char* filename ,void* pResult)
{
    t_xmlAccesseInfo* accessInfo = createAccessInfo(filename);
    if( NULL == accessInfo ) {
        fprintf(stderr, "accessInfo create errro\n");
        return (-1);
    }

    /* read and parse */
    int isEof;
    do {
        char buf[BUFSIZE];
        size_t len;

        len = common_fread(buf, sizeof(char), BUFSIZE, accessInfo->fp);
        if (ferror(accessInfo->fp)) {
            fprintf(stderr, "file error\n");
            return (-1);
        }
        isEof = common_feof(accessInfo->fp);
        /* XML parse */
        if ((XML_Parse(accessInfo->parser, buf, (int)len, isEof)) == 0) {
            fprintf(stderr, "parser error\n");
            return (-1);
        }
    } while (!isEof);

    fprintf(stderr, "done.\n");
    int ret = 0;

    if(accessInfo->parseData->isError) {
        ret = -1;
        goto TERMINAL;
    }

    if( accessInfo->expatAccessor.copy)
        ret = accessInfo->expatAccessor.copy((void*)pResult, accessInfo->parseData);

TERMINAL:
    deleteAccessInfo(accessInfo);
    return ret;
}


/************************************************/
/*  PrivateFunctions                            */
/************************************************/
static t_xmlAccesseInfo* createAccessInfo(const char* filename)
{
    t_xmlAccesseInfo* inst = MALLOC(t_xmlAccesseInfo);
    inst->filename = filename;
    inst->fp = NULL;
    inst->parser = NULL;
    inst->parseData = NULL;

    inst->fp = common_fopen(filename, "r");

    int idx = 0;
    for( idx = 0; s_xmlFileInfoTbl[idx].createFunc != NULL; idx++) {
        if(common_strcmp(filename, s_xmlFileInfoTbl[idx].filename ) != 0 )
            continue;

        inst->parseData = s_xmlFileInfoTbl[idx].createFunc(&inst->expatAccessor);
        break;
    }

    if( NULL == inst->parseData ) {
        fprintf(stderr, "parse data creation error\n");
        return NULL;
    }

    /* create XML parser */
    if ((inst->parser = XML_ParserCreate(NULL)) == NULL) {
        fprintf(stderr, "parser creation error\n");
        return NULL;
    }

    /* register element handler */
    XML_SetElementHandler(inst->parser, element_start, element_end);
    XML_SetCharacterDataHandler( inst->parser, value_handler);
    XML_SetUserData( inst->parser, inst );

    return inst;
}

static void deleteAccessInfo(t_xmlAccesseInfo* pThis)
{

    XML_ParserFree(pThis->parser);
    common_fclose(pThis->fp);

    if(pThis->expatAccessor.cleanup)
        pThis->expatAccessor.cleanup(pThis->parseData);

    common_free(pThis);
}

static void value_handler( void *userData, const XML_Char *string, int len )
{
    printf("%d:%s start \n", __LINE__, __func__);
    printf("%d:%s end \n", __LINE__, __func__);
}

static void element_start(void *userData, const XML_Char *name, const XML_Char *atts[])
{
    t_xmlAccesseInfo* accessInfo = (t_xmlAccesseInfo*)userData;
    t_xmlAccesseInfo_expatAccessor* parseFunc = &accessInfo->expatAccessor;
    if(accessInfo->parseData->isError)
        return;

    unsigned int listCount = 0U;
    unsigned int idx = 0U;
    const t_expatAccessor_startTagInfo* startTagInfoList = parseFunc->getStartTagInfoList(&listCount);
    while( NULL != startTagInfoList[idx].tagName ) {
        if( common_strcmp(name, startTagInfoList [idx].tagName ) == 0 ) {
            int ret = startTagInfoList[idx].executeFunc(accessInfo ->parseData, name, atts);
            if( 0 != ret ) {
                accessInfo->parseData->isError = 1;
            }
            break;
        }
        idx++;
    }
}

static void element_end(void *userData, const XML_Char *name)
{
    printf("%d:%s start \n", __LINE__, __func__);
    printf("%d:%s end \n", __LINE__, __func__);
}
