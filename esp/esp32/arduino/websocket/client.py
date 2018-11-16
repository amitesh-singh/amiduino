#/bin/python
import websocket
import time

ws = websocket.WebSocket()
ws.connect("ws://192.168.0.106/")
time.sleep(5);
i = 0
nrOfMessages = 200
print("going to send msgs")
while i<nrOfMessages:
    ws.send("message nr: " + str(i))
    print("sent msg..gonna recv");
    result = ws.recv()
    print(result)
    i=i+1
    time.sleep(1)

ws.close()
