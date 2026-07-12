TARGET = visualizer.exe

CXX = cl
CXXFLAGS = /nologo /O2 /MD /W3 /I. /I imgui /I imgui/backends /I glfw/include /D "NDEBUG"

LIBS = user32.lib gdi32.lib shell32.lib ws2_32.lib opengl32.lib glfw/glfw3.lib

IMGUI_DIR = imgui
IMGUI_OBJ = \
	imgui.obj \
	imgui_draw.obj \
	imgui_tables.obj \
	imgui_widgets.obj \
	imgui_impl_glfw.obj \
	imgui_impl_opengl3.obj

MAIN_OBJ = main.obj

all: $(TARGET)

imgui.obj: $(IMGUI_DIR)/imgui.cpp
	$(CXX) $(CXXFLAGS) /c $< /Fo$@

imgui_draw.obj: $(IMGUI_DIR)/imgui_draw.cpp
	$(CXX) $(CXXFLAGS) /c $< /Fo$@

imgui_tables.obj: $(IMGUI_DIR)/imgui_tables.cpp
	$(CXX) $(CXXFLAGS) /c $< /Fo$@

imgui_widgets.obj: $(IMGUI_DIR)/imgui_widgets.cpp
	$(CXX) $(CXXFLAGS) /c $< /Fo$@

imgui_impl_glfw.obj: $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp
	$(CXX) $(CXXFLAGS) /c $< /Fo$@

imgui_impl_opengl3.obj: $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
	$(CXX) $(CXXFLAGS) /c $< /Fo$@

main.obj: main.cpp
	$(CXX) $(CXXFLAGS) /c $< /Fo$@

$(TARGET): $(MAIN_OBJ) $(IMGUI_OBJ)
	$(CXX) $(CXXFLAGS) $(MAIN_OBJ) $(IMGUI_OBJ) /Fe$(TARGET) /link $(LIBS)

clean:
	del *.obj $(TARGET)
