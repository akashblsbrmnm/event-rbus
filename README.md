# RBUS Publish-Subscribe Demo

This project demonstrates a simple **Publish-Subscribe (Pub-Sub)** implementation using the **RDK-B RBUS** (RDK Message Bus) system with **TR-181-style parameters**.

It consists of two components:
- **Publisher (Provider)**: Registers and exposes parameters that can be read and updated.
- **Subscriber (Consumer)**: Subscribes to those parameters and listens for changes.

---

## 🧱 Components

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

---

## 🚀 How to Build & Run

### ✅ Prerequisites:
- RBUS development environment set up
- RBUS library installed (`-lrbus`)
- `rbuscli` tool (optional, for testing)

### 🛠️ Build:
```bash
gcc rbusPublisher.c -o publisher -lrbus
gcc rbusSubscriber.c -o subscriber -lrbus

### Run:
In Terminal 1:
`./publisher`
In Terminal 2:
`./subscriber`

### Test Using rbuscli:

`rbuscli set Device.Test.Param3 string "HelloWorld"`



