add_library(TracyClientStub INTERFACE)
add_library(Tracy::TracyClient ALIAS TracyClientStub)
set(Tracy_FOUND TRUE)