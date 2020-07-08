type t

@bs.val external process: t = "process"

@bs.send
external onSIGINT: (t, @bs.as("SIGINT") _, unit => unit) => unit = "on"
@bs.send
external onSIGTERM: (t, @bs.as("SIGTERM") _, unit => unit) => unit = "on"
@bs.send
external onSIGKILL: (t, @bs.as("SIGKILL") _, unit => unit) => unit = "on"
@bs.send
external onSIGSTOP: (t, @bs.as("SIGSTOP") _, unit => unit) => unit = "on"

@bs.send external exit: t => unit = "exit"

@bs.val @bs.scope("process") external argv: array<string> = "argv"
