
:run.bat

start "" "Debug/Client1Executive.exe" "UPLOAD" "Messages.exe" 5051 9080 "test.txt" 8080

start "" "Debug/Client2Executive.exe" "UPLOAD" "UnitTest.h" 5050 9080

start "" "Debug/Server1Executive.exe" 9080

start "" "Debug/Server2Executive.exe" 8080