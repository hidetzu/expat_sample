#include <stdio.h>
#include <stdlib.h>

#include <expat.h>
#include <common_capi.h>

#include <config_xmlParser.h>

#include "configMngr_xmlParser_common.h"
#include "configMngr_xmlParser_threadConfig.h"
#include "configMngr_xmlParser_threadAction.h"

/************************************************/
/*  Defines                                     */
/************************************************/
#define BUFSIZE 1024  

typedef void* (*t_create)(t_configMngr_xmlParserFunc* pFileOperation);
typedef void  (*t_copy)(void* pToData, void* pFromData);

typedef struct _t_config_parseDataFunc {
    t_copy   copyFunc;
    t_configMngr_xmlParserFunc parseFunc;
}t_config_parseDataFunc;

typedef struct _t_config_xmlFileInfo {
    const char* filename;
    t_create createFunc;
    t_copy   copyFunc;
}t_config_xmlFileInfo;

typedef struct _t_configMngr_xmlPasrserAccesseInfo {
    const char* filename;
    FILE* fp;
    XML_Parser parser;
    void* parseData;

    t_config_parseDataFunc parseDataFunc;
}t_configMngr_xmlPasrserAccesseInfo;

/************************************************/
/*  Prototypes                                  */
/************************************************/
static int config_xmlParser_parser(const char* filename ,void* pResult);

static t_configMngr_xmlPasrserAccesseInfo* createAccessInfo(const char* filename);
static void deleteAccessInfo(t_configMngr_xmlPasrserAccesseInfo* pThis);

static void element_start(void *userData, const XML_Char *name, const XML_Char *atts[]);
static void element_end(void *userData, const XML_Char *name);
static void value_handler( void *userData, const XML_Char *string, int len );

/************************************************/
/*  Gloval vars                                 */
/************************************************/
static t_config_xmlFileInfo s_xmlFileInfoTbl[] = {
    { 
        .filename = "inputfile/threadConfig.xml",
        .createFunc = (void*)configMngr_xmlParser_threadConfig_create,
        .copyFunc =  configMngr_xmlParser_threadConfig_copy,
    },

    { 
        .filename = "inputfile/threadAction.xml",
        .createFunc = (void*)configMngr_xmlParser_threadAction_create,
        .copyFunc =  configMngr_xmlParser_threadAction_copy,
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
int config_xmlParser_getThreadConifg(const char* filename, t_threadConfigList* pList)
{
    fprintf(stdout, "%s  start\n", __func__);
    return config_xmlParser_parser(filename, (void*)pList);
}

int config_xmlParser_getThreadAction(const char* filename, t_threadActionListInfo* pList)
{
    fprintf(stdout, "%s  start\n", __func__);
    return config_xmlParser_parser(filename, (void*)pList);
}

/************************************************/
/*  PrivateFunctions                            */
/************************************************/
static int config_xmlParser_parser(const char* filename ,void* pResult)
{
    t_configMngr_xmlPasrserAccesseInfo* accessInfo = createAccessInfo(filename);
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
    accessInfo->parseDataFunc.copyFunc((void*)pResult, accessInfo->parseData);
    deleteAccessInfo(accessInfo);

    return 0;
}

static t_configMngr_xmlPasrserAccesseInfo* createAccessInfo(const char* filename)
{
    t_configMngr_xmlPasrserAccesseInfo* inst = (t_configMngr_xmlPasrserAccesseInfo*)common_malloc(sizeof(t_configMngr_xmlPasrserAccesseInfo));
    inst->filename = filename;
    inst->fp = NULL;
    inst->parser = NULL;
    inst->parseData = NULL;

    inst->fp = common_fopen(filename, "r");

    int idx = 0;
    for( idx = 0; s_xmlFileInfoTbl[idx].createFunc != NULL; idx++) {
        if(common_strcmp(filename, s_xmlFileInfoTbl[idx].filename ) != 0 )
            continue;

        inst->parseData = s_xmlFileInfoTbl[idx].createFunc(&inst->parseDataFunc.parseFunc);
        inst->parseDataFunc.copyFunc = s_xmlFileInfoTbl[idx].copyFunc;
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

static void deleteAccessInfo(t_configMngr_xmlPasrserAccesseInfo* pThis)
{
    XML_ParserFree(pThis->parser);
    common_fclose(pThis->fp);
    common_free(pThis);
}

static void value_handler( void *userData, const XML_Char *string, int len )
{
    t_configMngr_xmlPasrserAccesseInfo* accessInfo = (t_configMngr_xmlPasrserAccesseInfo*)userData;
    t_configMngr_xmlParserFunc* parseFunc = &accessInfo->parseDataFunc.parseFunc;
    parseFunc->value_handler(accessInfo->parseData, string, len);
}

static void element_start(void *userData, const XML_Char *name, const XML_Char *atts[])
{
    t_configMngr_xmlPasrserAccesseInfo* accessInfo = (t_configMngr_xmlPasrserAccesseInfo*)userData;
    t_configMngr_xmlParserFunc* parseFunc = &accessInfo->parseDataFunc.parseFunc;
    parseFunc->element_start(accessInfo->parseData, name, atts);
}

static void element_end(void *userData, const XML_Char *name)
{
    t_configMngr_xmlPasrserAccesseInfo* accessInfo = (t_configMngr_xmlPasrserAccesseInfo*)userData;
    t_configMngr_xmlParserFunc* parseFunc = &accessInfo->parseDataFunc.parseFunc;
    parseFunc->element_end(accessInfo->parseData, name);
}
