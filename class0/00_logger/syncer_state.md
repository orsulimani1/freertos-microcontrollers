tateDiagram-v2
    [*] --> RTC_INIT
    
    RTC_INIT --> SEND_TIME : Initialize RTC
    
    SEND_TIME --> WAIT_VALIDATION : Send time to PC
    SEND_TIME --> SEND_TIME : RTC Error (retry)
    
    WAIT_VALIDATION --> RUNNING : "TIME_VALID"
    WAIT_VALIDATION --> WAIT_SYNC : "RESYNC" 
    WAIT_VALIDATION --> SEND_TIME : Timeout
    
    WAIT_SYNC --> SEND_TIME : Update RTC
    
    RUNNING --> RUNNING : "CHECK" command
    RUNNING --> SEND_TIME : "RESYNC" command
    
    note right of RTC_INIT
        Send "Protocol Ready"
    end note
    
    note right of SEND_TIME  
        Format: TIME: YYYY-MM-DD HH:MM:SS
    end note
    
    note right of RUNNING
        Non-blocking operation
    end note