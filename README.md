
# RBUS Publish-Subscribe Demo

This project demonstrates a simple **Publish-Subscribe** implementation using the **[RBUS](https://github.com/rdkcentral/rbus)** with **TR-181-style parameters**.

It consists of two components:
- **Publisher**: Registers and exposes parameters that can be read and updated.
- **Subscriber**: Subscribes to those parameters and listens for changes.

---

## 🧩 Components

### 1. `rbusPublisher.c`
- Registers 5 TR-181-style parameters:
  - `Device.Test.Param1`
  - `Device.Test.Param2`
  - `Device.Test.Param3`
  - `Device.Test.Param4`
  - `Device.Test.Param5`
- Implements `getHandler` and `setHandler`.
- Automatically updates parameter values and relies on RBUS to publish events on value change.

### 2. `rbusSubscriber.c`
- Subscribes to the same 5 parameters.
- Receives events whenever a parameter value changes.
- Displays the new and old values in the terminal.


## 🚀 How to Build & Run

### ✅ Prerequisites:
- RBUS development environment set up
- RBUS library installed (`-lrbus`)
- `rbuscli` tool (optional, for testing)

### 🛠️ Build:
```bash
gcc rbusPublisher.c -o publisher -lrbus
gcc rbusSubscriber.c -o subscriber -lrbus
```
> If you haven't setup build environment :
```
gcc rbusPublisher.c -o publisher -I${RBUS_INSTALL_DIR}/usr/include -I${RBUS_INSTALL_DIR}/usr/include/rtmessage -L${RBUS_INSTALL_DIR}/usr/lib -lrbus -lrbuscore
gcc rbusSubscriber.c -o subscriber   -I${RBUS_INSTALL_DIR}/usr/include   -I${RBUS_INSTALL_DIR}/usr/include/rtmessage   -L${RBUS_INSTALL_DIR}/usr/lib   -lrbus -lrbuscore
```

### ⚙️ Run:
In Terminal 1:
```
./publisher
```
In Terminal 2:
```
./subscriber
```

### 🎮 Test Using rbuscli:

`rbuscli set Device.Test.Param3 string "HelloWorld"`

### 📄 Sample Logs
```
user@spaceship:~/user/event-rbus$ ./publisher 
[Provider] Starting...
[Provider] Registered 5 parameters. Waiting for set calls...

user@spaceship:~/user/event-rbus$ ./subscriber 
[Subscriber] Starting...
[Subscriber] Subscribing to: Device.Test.Param1
[Subscriber] Subscribing to: Device.Test.Param2
[Subscriber] Subscribing to: Device.Test.Param3
[Subscriber] Subscribing to: Device.Test.Param4
[Subscriber] Subscribing to: Device.Test.Param5
[Subscriber] Subscribed to all. Waiting for events...
[Subscriber] Subscribed successfully to Device.Test.Param1
[Subscriber] Subscribed successfully to Device.Test.Param2
[Subscriber] Subscribed successfully to Device.Test.Param3
[Subscriber] Subscribed successfully to Device.Test.Param4
[Subscriber] Subscribed successfully to Device.Test.Param5


user@spaceship:~/user/event-rbus$ rbuscli set Device.Test.Param3 string "HelloWorld"
setvalues succeeded..
[Provider] Device.Test.Param3 updated to HelloWorld
[Subscriber] Event received for Device.Test.Param3
[Subscriber] New value = HelloWorld, Old value = Value3


user@spaceship:~/user/event-rbus$ rbuscli set Device.Test.Param2 string "HelloWorld2"
setvalues succeeded..[Provider] Device.Test.Param2 updated to HelloWorld2
[Provider] Device.Test.Param2 updated to HelloWorld2
[Subscriber] Event received for Device.Test.Param2
[Subscriber] New value = HelloWorld2, Old value = Value2

user@spaceship:~/user/event-rbus$ rbuscli set Device.Test.Param6 string "HelloWorld"
12:22:06.183643  RBUS       ERROR rbus.c:4028 -- Thread-7279: _setInternal for Device.Test.Param6 failed with error: Entry not found error
setvalues failed with return value: 5
```
