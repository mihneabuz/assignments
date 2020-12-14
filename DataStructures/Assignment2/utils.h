//Buzatu-Pahontu Mihnea-Nicolae
//315CB

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#ifndef _DOWNLOAD_RESOURCE_
#define _DOWNLOAD_RESOURCE_

typedef struct{
    char name[100];
    unsigned long dimension;
    unsigned long currently_downloaded;
} Resource;

Resource* get_page_resources(const char *URL_Name, int *n);

Resource* CopyResource(Resource res);
//returneaza o copie a resursei res

#endif /* _DOWNLOAD_RESOURCE_ */
