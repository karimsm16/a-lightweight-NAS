# DEVLOG (3)

This week I focused on improving both the backend architecture and the web interface of the ESP32 NAS.

I finished refactoring the `FileService` and `HttpServer` classes . I also implemented the first HTTP API endpoints that will allow the frontend to communicate with the ESP32, I really struggled to make the HTTP server work .

On the frontend, I redesigned the HTML layout, cleaned up the structure, fixed some dumb problems , and introduced dedicated pages for **Files**, **Network**, and **Settings**. for the actual moment the backend is ready , and the same thing for the frontend (I will improve it later) , now I have to connect them all through JavaScript and the REST API.

well I come along a hard way to this , but it deserves it 

# DEVLOG (4)

since the last time , I tried to turn the interface to a multi-page interface that contains the 3 main sections 
**files**
**Network**
**Settings**
and I tried to integrate java script navigation , between pages and itms , and during this I got some javascript issues ,because the javascript file refused  loadeding , but I fixed it later , after completing the navigation process I created an /api/network endpoint that returns live Wi-Fi information from the ESP32, and connected the Network page,I also implemented the /api/files API in the HTTP server, which converts the FileService output into JSON, then I started laying the foundation for folder navigation , which will allow the browser to navigate directories just like a desktop file explorer.


**important note :**the data in the screenshot is fake because I run UI files in the live server , not yet to the esp32    