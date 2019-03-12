set(SOURCE_FILES
      HttpRequest.cpp
	  HttpRequestHandler.cpp
	  HttpResponse.cpp
      HttpServer.cpp
	  HttpUriPatternProcessor.cpp
      TcpSocket.cpp
)

add_library(network STATIC ${SOURCE_FILES})
if(WIN32)
  target_link_libraries(network wsock32 ws2_32)
endif()
