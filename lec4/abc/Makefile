# Makefile

DEL=del /f
COPY=copy /y
MT=mt -nologo
CC=cl /nologo
RC=rc /nologo
LINK=link /nologo

CFLAGS=/MD /O2 /GA /Zi
LDFLAGS=/DEBUG /MACHINE:x86 /OPT:REF /OPT:ICF
RCFLAGS=
DEFS_COMMON=/D WIN32 /D UNICODE /D _UNICODE
DEFS_CONSOLE=$(DEFS_COMMON) /D CONSOLE /D _CONSOLE
DEFS_WINDOWS=$(DEFS_COMMON) /D WINDOWS /D _WINDOWS
DEFS=$(DEFS_WINDOWS)
#DEFS=$(DEFS_CONSOLE)
LIBS=
INCLUDES=
TARGET=abc.exe

all: $(TARGET)

test: $(TARGET)
	.\$(TARGET)

clean:
	-$(DEL) $(TARGET)
	-$(DEL) *.lib *.exp *.obj *.res *.ilk *.pdb *.manifest

$(TARGET): abc.obj abc.res
	$(LINK) $(LDFLAGS) /manifest /out:$@ $** $(LIBS)
	$(MT) -manifest $@.manifest -outputresource:$@;1

.cpp.obj:
	$(CC) $(CFLAGS) /Fo$@ /c $< $(DEFS) $(INCLUDES)
.rc.res:
	$(RC) $(RCFLAGS) $<
