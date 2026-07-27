TARGET = target/visualizer.exe

CXX = cl
CXXFLAGS = /nologo /O2 /MD /W3 \
		   /I. /I imgui /I imgui/backends /I glfw/include /I implot \
		   /D "NDEBUG" /std:c++20
		   # /fsanitize=address /Zi

LIBS = user32.lib gdi32.lib shell32.lib ws2_32.lib opengl32.lib glfw/glfw3.lib

SRCS = main.cpp \
	   imgui/imgui.cpp \
	   imgui/imgui_draw.cpp \
	   imgui/imgui_tables.cpp \
	   imgui/imgui_widgets.cpp \
	   imgui/backends/imgui_impl_glfw.cpp \
	   imgui/backends/imgui_impl_opengl3.cpp \
	   implot/implot.cpp \
	   implot/implot_items.cpp
       # \
	   # imgui/imgui_demo.cpp \
	   # implot/implot_demo.cpp

OBJS = $(addprefix target/, $(SRCS:.cpp=.obj))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) /Fe$(TARGET) /link $(LIBS)

target/%.obj: %.cpp
	@if not exist "$(@D)" mkdir "$(@D)"
	$(CXX) $(CXXFLAGS) /c $< /Fo$@

run: $(TARGET)
	./$(TARGET)

clean:
	@if exist target rmdir /s /q target

.PHONY: all run clean
