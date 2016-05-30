//******************************************************************
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "NSProviderResource.h"

char* NSType = "oic.r.notification";
char* NSMessageType = "oic.r.notification.message";
char* NSSyncType = "oic.r.notification.sync";

char* NSInterface = "oic.if.baseline";
char* NSMessgeInterface = "oic.if.baseline.message";
char* NSSyncInterface = "oic.if.baseline.sync";

char* NSUri = "/notification";
char* NSMessageUri = "/notification/message";
char* NSSyncUri = "/notification/sync";

/* Structure to represent notification resources */
typedef struct
{
    OCResourceHandle handle;
    int accepter;
    char* message_uri;
    char* sync_uri;
} NSNotificationResource;

typedef struct
{
    OCResourceHandle handle;
    char* id;
    char* title;
    char* body;
} NSMessageResource;

typedef struct
{
    OCResourceHandle handle;
    char* id;
    char* state;
} NSSyncResource;

NSNotificationResource NotificationResource;
NSMessageResource NotificationMessageResource;
NSSyncResource NotificationSyncResource;

NSResult NSCreateResource(char *uri)
{
    NS_LOG(DEBUG, "NSCreateResource - IN");
    if (!uri)
    {
        NS_LOG(NS_ERROR, "Resource URI cannot be NULL");
        return NS_ERROR;
    }

    if (strcmp(uri, NSUri) == 0)
    {

        NotificationResource.accepter = 0;
        NotificationResource.message_uri = NSMessageUri;
        NotificationResource.sync_uri = NSSyncUri;
        NotificationResource.handle = NULL;

        if (OCCreateResource(&NotificationResource.handle, NSType, NSInterface, NSUri,
                NSEntityHandlerNotificationCb, NULL, OC_DISCOVERABLE) != OC_STACK_OK)
        {
            NS_LOG(NS_ERROR, "Fail to Create Notification Resource");
            return NS_ERROR;
        }
    }
    else if (strcmp(uri, NSMessageUri) == 0)
    {

        NotificationMessageResource.id = NULL;
        NotificationMessageResource.title = NULL;
        NotificationMessageResource.body = NULL;
        NotificationMessageResource.handle = NULL;

        if (OCCreateResource(&NotificationMessageResource.handle, NSMessageType, NSInterface,
                NSMessageUri, NSEntityHandlerMessageCb, NULL, OC_OBSERVABLE) != OC_STACK_OK)
        {
            NS_LOG(NS_ERROR, "Fail to Create Notification Message Resource");
            return NS_ERROR;
        }
    }
    else if (strcmp(uri, NSSyncUri) == 0)
    {
        NotificationSyncResource.id = NULL;
        NotificationSyncResource.state = NULL;
        NotificationSyncResource.handle = NULL;

        if (OCCreateResource(&(NotificationSyncResource.handle), NSSyncType, NSInterface, NSSyncUri,
                NSEntityHandlerSyncCb, NULL, OC_OBSERVABLE) != OC_STACK_OK)
        {
            NS_LOG(NS_ERROR, "Fail to Create Notification Sync Resource");
            return NS_ERROR;
        }
    }
    else
    {
        NS_LOG(ERROR, "Fail to create resource with invalid URI");
        return NS_ERROR;
    }

    NS_LOG(DEBUG, "NSCreateResource - OUT");
    return NS_OK;
}

NSResult NSRegisterResource()
{
    NS_LOG(DEBUG, "NSRegisterResource - IN");

    if (NSCreateResource(NSSyncUri) != NS_OK)
    {
        NS_LOG(ERROR, "Fail to register Sync Resource");
        return NS_ERROR;
    }

    if (NSCreateResource(NSMessageUri) != NS_OK)
    {
        NS_LOG(ERROR, "Fail to register Message Resource");
        return NS_ERROR;
    }

    if (NSCreateResource(NSUri) != NS_OK)
    {
        NS_LOG(ERROR, "Fail to register Notification Resource");
        return NS_ERROR;
    }

    NS_LOG(DEBUG, "NSRegisterResource - OUT");
    return NS_OK;
}

NSResult NSUnRegisterResource()
{
    NS_LOG(DEBUG, "NSUnRegisterResource - IN");

    if (OCDeleteResource(NotificationResource.handle) != OC_STACK_OK)
    {
        NS_LOG(ERROR, "Fail to Delete Notification Resource");
        return NS_ERROR;
    }

    if (OCDeleteResource(NotificationMessageResource.handle) != OC_STACK_OK)
    {
        NS_LOG(ERROR, "Fail to Delete Notification Message Resource");
        return NS_ERROR;
    }

    if (OCDeleteResource(NotificationSyncResource.handle) != OC_STACK_OK)
    {
        NS_LOG(ERROR, "Fail to Delete Notification Sync Resource");
        return NS_ERROR;
    }

    NS_LOG(DEBUG, "NSUnRegisterResource - OUT");
    return NS_OK;
}

NSResult NSPutNotificationResource(int accepter, OCResourceHandle * handle)
{
    NS_LOG(DEBUG, "NSPutNotificationResource - IN");

    NotificationResource.accepter = accepter;
    NotificationResource.message_uri = NSMessageUri;
    NotificationResource.sync_uri = NSSyncUri;

    *handle = NotificationResource.handle;

    NS_LOG(DEBUG, "NSPutNotificationResource - OUT");
    return NS_OK;
}

NSResult NSPutMessageResource(NSMessage *msg, OCResourceHandle * handle)
{
    NS_LOG(DEBUG, "NSPutMessageResource - IN");

    if(msg != NULL)
    {
        NS_LOG(DEBUG, "NSMessage is valid");

        NotificationMessageResource.id = OICStrdup(msg->mId);
        NotificationMessageResource.title = OICStrdup(msg->mTitle);
        NotificationMessageResource.body = OICStrdup(msg->mContentText);
    }
    else
    {
        NS_LOG(ERROR, "NSMessage is NULL");
    }

    *handle = NotificationMessageResource.handle;
    NS_LOG(DEBUG, "NSPutMessageResource - OUT");

    return NS_OK;
}

NSResult NSPutSyncResource(NSSync *sync, OCResourceHandle * handle)
{
    NS_LOG(DEBUG, "NSPutSyncResource - IN");

    (void) sync;

    *handle = NotificationSyncResource.handle;

    NS_LOG(DEBUG, "NSPutSyncResource - OUT");
    return NS_OK;
}

const char* NSGetNotificationUri()
{
    return NSUri;
}

const char* NSGetNotificationMessageUri()
{
    return NSMessageUri;
}

const char* NSGetNotificationSyncUri()
{
    return NSSyncUri;
}

NSResult NSCopyString(char** targetString, const char* sourceString)
{
    if (sourceString)
    {
        *targetString = (char *) malloc(strlen(sourceString) + 1);

        if (*targetString)
        {
            strncpy(*targetString, sourceString, (strlen(sourceString) + 1));
            return NS_SUCCESS;
        }
    }

    return NS_FAIL;
}