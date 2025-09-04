#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <rbus/rbus.h>

#define PARAM_COUNT 5

const char* notifyList[PARAM_COUNT] = {
    "Device.Test.Param1",
    "Device.Test.Param2",
    "Device.Test.Param3",
    "Device.Test.Param4",
    "Device.Test.Param5"
};

rbusHandle_t handle = NULL;

void eventCallback(
    rbusHandle_t handle,
    rbusEvent_t const* event,
    rbusEventSubscription_t* subscription)
{
    char const* param = event->name;
    rbusObject_t obj = event->data;

    printf("[Subscriber] Event received for %s\n", param);

    // Get new and old values from the event object
    rbusValue_t newValue = rbusObject_GetValue(obj, "value");
    rbusValue_t oldValue = rbusObject_GetValue(obj, "oldValue");

    if (newValue != NULL)
    {
        char newValStr[256];
        rbusValue_ToString(newValue, newValStr, sizeof(newValStr));

        if (oldValue != NULL)
        {
            char oldValStr[256];
            rbusValue_ToString(oldValue, oldValStr, sizeof(oldValStr));
            printf("[Subscriber] New value = %s, Old value = %s\n", newValStr, oldValStr);
        }
        else
        {
            printf("[Subscriber] New value = %s, Old value = <none>\n", newValStr);
        }
    }
    else
    {
        printf("[Subscriber] Event received but no new value found.\n");
    }
}


void subscribeResponseHandler(rbusHandle_t handle,
                              rbusEventSubscription_t* subscription,
                              rbusError_t err)
{
    if(err == RBUS_ERROR_SUCCESS)
        printf("[Subscriber] Subscribed successfully to %s\n", subscription->eventName);
    else
        printf("[Subscriber] Subscription failed with error %d\n", err);
}

int main()
{
    printf("[Subscriber] Starting...\n");

    rbusError_t err = rbus_open(&handle, "TestRBusSubscriber");
    if(err != RBUS_ERROR_SUCCESS) {
        printf("[Subscriber] rbus_open failed: %d\n", err);
        return 1;
    }

    for(int i = 0; i < PARAM_COUNT; ++i)
    {
        printf("[Subscriber] Subscribing to: %s\n", notifyList[i]);
        rbusError_t subErr = rbusEvent_SubscribeAsync(
            handle,
            notifyList[i],
            eventCallback,
            subscribeResponseHandler,
            NULL,
            0
        );

        if(subErr != RBUS_ERROR_SUCCESS)
            printf("[Subscriber] Failed to subscribe to %s (error %d)\n", notifyList[i], subErr);
    }

    printf("[Subscriber] Subscribed to all. Waiting for events...\n");

    while(1)
        sleep(1);

    rbus_close(handle);
    return 0;
}
