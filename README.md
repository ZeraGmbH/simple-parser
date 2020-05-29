# simple-parser
Tiny qt helper classes to implement servers for a command/response protocol. The protocol was
designed for simple implementation and logs readable by humans.

Protocol rules:

* All commands expect a response
* No timeout handling within the protocol
* Allow one pending request only at a time. Sending a second request while another is pending causes an error
* Commands have a name field and optional parameter fields. Fields are separated by `,`
* A command executed successfully responds with: `<command-name>,OK,<optional-data>`
* A failed command responds with `<command-name>,ERROR,<optional-but-recommended-error-description>`
* Command names are not case sensitive
* Commands and responses are terminated by linefeed `\n`
* Carriage return characters `\r` are ignored
* UTF8 supported since version 0.2.0
* For parameters the following data types are supported:
  * BOOL: 1/0
  * INT: signed Integer
  * FLOAT: Float point numbers with `.` as decimal separator
    Exponential representations as `1234.56e-02` are accepted
  * STRING: A printable string
* Parameters can be expanded by macros that can be nested e.g `@num_to_hex(@date_time(yyyy);4`sets parameter to `07e4` at the time of writing. Currently the following macros are available:
  * `@date_time(QDateTime-format-string)`: expands to formatted system date/time
  * `@ascii_to_hex(string)`: expands to hexadecimal representation of `string`
  * `@hex_to_ascii(hex_string)`: expands to`hex_string` to ASCII
  * `@num_to_hex(integer;min_num_hex_digits)`: expands `integer` to hexadecimal string


To create a server the following is required:

* A class derived from `CmdParser`. This decodes incoming commands and checks parameters for proper values. Note
  that all possible validation shoud be done in `PlausiCheck(SimpleCmdData *pCmd, const QVariantList &params)` so
  the command handler can rely on the paramters passed to it.
* A class derived from `CmdHandler`. This is the worker receiving command and parameter information in
  `StartCmd(SimpleCmdData *pCmd, QVariantList params)`

Check [example project](src/simpleexample) for more details.

Example I/O:
```
--> PreparePLC,1,AbandWaveform.txt,0.872
<-- PreparePLC,OK
--> StartPLC,PLC.ini
<-- StartPLC,ERROR: Wrong number of parameters
--> StartPLC,1,PLC.ini
<-- StartPLC,OK,1.223
```

simple-parser consists of the following sub projects

* [simplecmdiobase](src/simplecmdiobase): The library implementation itself
* [simpleexample](src/simpleexample): An example application. To test
  * run the application
  * start `telnet localhost 5000`
  * enter `help` to show the command available
* [simpleautorun](src/simpleautorun): A runner application for endurance tests. It runs from a command file 
  based on the protocol decribed above plus few extensions (e.g loops). For more information see
  [simpleautorun documentation](src/simpleautorun/doc/simpleautorun.md)

Usage:
* QT += simplecmdiobase in <your-project>.pro 
