#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <rbus/rbus.h>

#define PARAM_COUNT 5
const char* paramNames[PARAM_COUNT] = {
    "Device.Test.Param1",
    "Device.Test.Param2",
    "Device.Test.Param3",
    "Device.Test.Param4",
    "Device.Test.Param5"
};

char paramValues[PARAM_COUNT][64] = {
    "Value1", "Value2", "Value3", "Value4", "Value5"
};

rbusHandle_t handle;

int getIndex(const char* name) {
    for (int i = 0; i < PARAM_COUNT; i++) {
        if (strcmp(name, paramNames[i]) == 0)
            return i;
    }
    return -1;
}

rbusError_t getHandler(rbusHandle_t handle, rbusProperty_t prop, rbusGetHandlerOptions_t* opts) {
    const char* name = rbusProperty_GetName(prop);
    int index = getIndex(name);

    if(index >= 0) {
        rbusValue_t value;
        rbusValue_Init(&value);
        rbusValue_SetString(value, paramValues[index]);
        rbusProperty_SetValue(prop, value);
        rbusValue_Release(value);
        return RBUS_ERROR_SUCCESS;
    }

    return RBUS_ERROR_INVALID_INPUT;
}

rbusError_t setHandler(rbusHandle_t handle, rbusProperty_t prop, rbusSetHandlerOptions_t* opts)
{
    const char* name = rbusProperty_GetName(prop);
    rbusValue_t val = rbusProperty_GetValue(prop);

    char strVal[128] = {0};
    rbusValue_ToString(val, strVal, sizeof(strVal));

    int index = getIndex(name);
    if(index >= 0)
    {
        // Just update the stored value
        strncpy(paramValues[index], strVal, sizeof(paramValues[index]) - 1);
        paramValues[index][sizeof(paramValues[index]) - 1] = '\0';

        printf("[Provider] %s updated to %s\n", name, strVal);

        // No manual rbusEvent_Publish() here — RBus will send the event automatically
        return RBUS_ERROR_SUCCESS;
    }

    return RBUS_ERROR_INVALID_INPUT;
}



int main() {
    printf("[Provider] Starting...\n");

    int rc = RBUS_ERROR_BUS_ERROR;

    rc = rbus_open(&handle, "TestRBusProvider");

    if (rc != RBUS_ERROR_SUCCESS)
    {
        printf("[Provider] rbus_open failed.\n");
        return 1;
    }

    rbusDataElement_t elements[PARAM_COUNT];
    memset(elements, 0, sizeof(elements));

    for(int i = 0; i < PARAM_COUNT; i++)
    {
        elements[i].name = (char*)paramNames[i];
        elements[i].type = RBUS_ELEMENT_TYPE_PROPERTY;

        // Callback table setup
        rbusCallbackTable_t cbTable = {0};
        cbTable.getHandler = getHandler;
        cbTable.setHandler = setHandler;

        elements[i].cbTable = cbTable;
    }

    rc = rbus_regDataElements(handle, PARAM_COUNT, elements);
    if (rc != RBUS_ERROR_SUCCESS)
    {
        printf("[Provider] Failed to register data elements\n");
        rbus_close(handle);
        return 1;
    }

    printf("[Provider] Registered %d parameters. Waiting for set calls...\n", PARAM_COUNT);

    while(1)
        sleep(1);

    rbus_close(handle);
    return 0;
}
