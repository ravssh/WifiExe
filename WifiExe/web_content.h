// web_content.h
#ifndef WEB_CONTENT_H
#define WEB_CONTENT_H

// Minimal placeholder HTML templates used by wifi_server.cpp

static const char *cssStyles = "<style>body{font-family:Arial;}</style>";
static const char *folderViewHTML = "<html><head>%s</head><body><h2>Folder</h2>%s</body></html>";
static const char *fileViewHTML = "<html><head>%s</head><body><h2>File</h2><pre>%s</pre></body></html>";
static const char *rootViewHTML = "<html><head>%s</head><body><h2>WifiExe</h2><ul><li><a href=\"/folder?path=/sd\">Browse SD</a></li></ul></body></html>";
static const char *redirectToRoot = "<html><head>%s</head><body><p>Done. <a href=\"/\">Home</a></p></body></html>";

#endif
