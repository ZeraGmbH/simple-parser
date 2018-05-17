# simple-parser
Tiny qt helper classes for a simple command/response protocol. The protocol was
desingend for simple implemtation and readable logs.

Protocol rules:

* Commands expect response
* No timeout handling within the protocol
* Allow one pending request only. Sending a second request while
  another is pending is treated as error
* Commands have a name and optional parameters. Fields are separated by ','
* A command executed successfully responds with: \<command-name\>,OK,\<optional-data\>
* A failed command responds with \<command-name\>,ERROR,\<optional-error-description>
* Command names a not case sensitive
* Commands and responses are terminated by linefeed ("\n")
* Carriage return characters ("\r") are ignored
* For parameters the following data types are supported:
  * BOOL: 1/0
  * INT: Integer with sign
  * FLOAT: FLoat point numbers with '.' as decimal separator
    Exponential representations as '1234.56e-02 are accepted
  * STRING: A printable string


Example I/O:
```
--> PreparePLC,1,AbandWaveform.txt,0.872
<-- PreparePLC,OK
--> StartPLC,PLC.ini
<-- StartPLC,ERROR: Wrong number of parameters
--> StartPLC,1,PLC.ini
<-- StartPLC,OK,1.223
```

Usage:
* QT += simplecmdiobase in <your-project>.pro 
