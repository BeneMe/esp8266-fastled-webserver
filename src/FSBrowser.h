#ifndef FS_BROWSER_H
#define FS_BROWSER_H

#include <ESP8266WebServer.h>
#include <FS.h>

class FSBrowser {
  

  public:
  explicit FSBrowser();
  ~FSBrowser() = default;


//holds the current upload
  File fsUploadFile;

  String formatBytes(size_t bytes);
  String getContentType(String filename, ESP8266WebServer &webServer);
  bool handleFileRead(String path, ESP8266WebServer &webServer);
  void handleFileUpload(ESP8266WebServer &webServer);
  void handleFileDelete(ESP8266WebServer &webServer);
  void handleFileCreate(ESP8266WebServer &webServer);
  void handleFileList(ESP8266WebServer &webServer);
  boolean existsOrZipped(String path);
};

#endif