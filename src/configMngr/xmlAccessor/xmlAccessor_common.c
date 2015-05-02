#include <expat.h>
#include <common_capi.h>

/************************************************/
/*  Defines                                     */
/************************************************/

/************************************************/
/*  Prototypes                                  */
/************************************************/

/************************************************/
/*  Gloval vars                                 */
/************************************************/

/************************************************/
/*  PublicFunctions                             */
/************************************************/
const char* xmlAccessor_common_findAttributeValue(
    const XML_Char *atts[], const char* attributeName)
{
    int idx = 0;
    while(atts[idx] != NULL ) {
        if (common_strcmp(atts[idx], attributeName) == 0 ) {
            idx++;
            return atts[idx];
        }
        idx++;
    }
    return NULL;
}

/************************************************/
/*  PrivateFunctions                            */
/************************************************/
