
#include "communication_data.h"

extern volatile const CTOM_Data *CTOM_DATA;
extern volatile MTOC_Data *MTOC_DATA;

Void KeepAliveTask(UArg arg0, UArg arg1);

void netOpenHook();
Void OsciConnectionHandler(UArg arg0, UArg arg1);
Void OsciServer(UArg arg0, UArg arg1);
