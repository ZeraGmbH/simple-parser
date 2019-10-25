# simpleautorun
For test of devices supporting simple protocol via socket connection the application simpleautorun was implemented. To
get the available command line parameters enter on the shell:

```
simpleautorun --help
```

To start a test, the following command has to be entered in the shell:
```
simpleautorun -x <command-file>
```

The command-file is read line by line. Each line contains either a command for simpleautorun itself (internal commands) or
for devices connected (external commands). Empty lines or lines starting with ‘#’ are ignored. To study or test a simple
example, see example files [autorun-example](doc/autorun-example) and
[autorun-example-expected](doc/autorun-example-expected).

In case of error on internal command, the sequence stops except command [CheckLastResponse](#check_last_response)
which follows the setting made by [SetOnErrorStop](#set_on_error_stop).

The following internal commands are supported by simpleautorun (none of
the commends listed below return data):

---------------------------------------------------------------------------------------------------------------------------
### <a id="open_ip_port"></a>Command “OpenIPPort“

This command opens a connection to a socket supporting simple protocol. All connections opened are enumerated starting
with 0. 

The following parameters are expected:
* **IP-adress** (STRING)
* **IP-port** (INT)

---------------------------------------------------------------------------------------------------------------------------
### Command “SelectIPConnection“

This command selects the connection currently active for next commands.

The following parameter is expected:

* **Connection number** (INT): The connection number which was applied when the connection was opened (see
  Command [OpenIPPort](#open_ip_port)).

---------------------------------------------------------------------------------------------------------------------------
### Command “WaitMs“

To add delays in the sequence use WaitMs.

The following parameter is expected:

* **Ms** (INT): Delay in [ms]

---------------------------------------------------------------------------------------------------------------------------
### <a id="set_tag"></a>Command “SetTag“

With this command loop tags can be set.

The following parameter is expected:

* **Name** (STRING): Name / identifier of the loop tag

---------------------------------------------------------------------------------------------------------------------------
### Command “LoopTag“

This command was added to run loops.

The following parameters are expected:

* **Name of target tag** (STRING): Name / identifier of the loop tag (see Command [SetTag](#set_tag))
* **Number of loops** (INT): By setting 0  the loop will never be finished.

---------------------------------------------------------------------------------------------------------------------------
### <a id="set_on_error_stop"></a>Command “SetOnErrorStop“

With this command the behaviour can be set in case a command response notifies an error.

The following parameter is expected:

* **Abort on error** (BOOL): By setting to 1 the sequence is aborted in case a command to a connected device return an error.
  Default: 0 (errors returned are ignored).

---------------------------------------------------------------------------------------------------------------------------
### <a id="check_last_response"></a>Command “CheckLastResponse“

With this command the response of the last command can be checked.

The following parameters are expected:

* **Expected data** (STRING): The data returned from previous command expected.
* **Ignore case** (BOOL): Deactivate case sensitivity ‘a’ is same as ‘A’

---------------------------------------------------------------------------------------------------------------------------
### Command “MsgAndWait“

This command displays a message waits until the user presses <RETURN>.

The following parameter is expected:

* **Message String** (STRING): The string displayed in the box.

