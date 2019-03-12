set(SOURCE_FILES
      DataManager.cpp
	  List.cpp
	  ListItem.cpp
	  ListSerializer.cpp
)

add_library(data STATIC ${SOURCE_FILES})