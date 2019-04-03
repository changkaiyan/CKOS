#include<Object.h>

class SystemTable
{
    private DriverTable*    driverTable;
    private WindowTable*    windowTable;
    private FileTable*      fileTable;
    private ProcessTable*   processTable;
    private ThreadTable*    threadTable;
    private ProcessorTable* processorTable;
    private MemoryTable*    memoryTable;
    private NetworkTable*   networkTable;
    public ErrorTable* errorTable;
#ifdef x86
    private SegmentTable*   segmentTable;
    private InterruptTable* interruptTable;
#endif 
    public DriverTable* (*handleDriver)();
    public DriverTable* (*handleWindow)();
    public DriverTable* (*handleFile)();
    public DriverTable* (*handleProcess)();
    public DriverTable* (*handleThread)();
    public DriverTable* (*handleProcessor)();
    public DriverTable* (*handleMemory)();
#ifdef x86
    public DriverTable* (*handleSegment)();
    public DriverTable* (*handleInterrupt)();
#endif
}SystemTable;