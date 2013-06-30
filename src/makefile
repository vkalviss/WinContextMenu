#NODEBUG = 1

!include <win32.mak>

PROJ = WinContextMenu

#!IFDEF NODEBUG
#OUTDIR = Release_$(CPU)
#!ELSE
#OUTDIR = Debug_$(CPU)
#!ENDIF

#OUTDIR = Output

EXTRA_LIBS = 
#odbccp32.lib odbc32.lib shell32.lib advapi32.lib

PROJ_OBJS = $(OUTDIR)\$(PROJ).obj $(OUTDIR)\stdafx.obj $(OUTDIR)\WindowsCounter.obj $(OUTDIR)\CmdLineParser.obj

#$(guilibs)
MY_C_FLAGS_STATIC = $(cflags) $(cdebug) $(cvars) -DUNICODE -D_UNICODE /EHsc
MY_LINK_FLAGS_STATIC = $(guilflags) $(linkdebug) $(olelibs)

MY_C_FLAGS_DLL = $(cflags) $(cdebug) $(cvarsdll) -DUNICODE -D_UNICODE /EHsc
MY_LINK_FLAGS_DLL = $(guilflags) $(linkdebug) $(olelibsdll)

MY_C_FLAGS = $(MY_C_FLAGS_DLL)
MY_LINK_FLAGS = $(MY_LINK_FLAGS_DLL)

all: $(OUTDIR) $(OUTDIR)\$(PROJ).exe

$(OUTDIR):
    if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)


$(OUTDIR)\$(PROJ).obj: $(PROJ).cpp
   $(cc) $(MY_C_FLAGS)                             \
   /Fo$(OUTDIR)\\                                 \
   /Fd$(OUTDIR)\\                                 \
   /I$(OUTDIR)                                    \
   $(PROJ).cpp

$(OUTDIR)\stdafx.obj: stdafx.cpp
   $(cc) $(MY_C_FLAGS)                             \
   /Fo$(OUTDIR)\\                                 \
   /Fd$(OUTDIR)\\                                 \
   /I$(OUTDIR)                                    \
   stdafx.cpp

$(OUTDIR)\WindowsCounter.obj: WindowsCounter.cpp
   $(cc) $(MY_C_FLAGS)                             \
   /Fo$(OUTDIR)\\                                 \
   /Fd$(OUTDIR)\\                                 \
   /I$(OUTDIR)                                    \
   WindowsCounter.cpp

$(OUTDIR)\CmdLineParser.obj: CmdLineParser.cpp
   $(cc) $(MY_C_FLAGS)                             \
   /Fo$(OUTDIR)\\                                 \
   /Fd$(OUTDIR)\\                                 \
   /I$(OUTDIR)                                    \
   CmdLineParser.cpp

# Update the resources if necessary

$(OUTDIR)\$(PROJ).res: $(PROJ).rc Resource.h
    $(rc) $(rcflags) $(rcvars) /i $(OUTDIR) /fo $(OUTDIR)\$(PROJ).res  $(PROJ).rc

# $(OUTDIR)\$(PROJ).res
#   $(OUTDIR)\$(PROJ).res             \


$(OUTDIR)\$(PROJ).exe: $(PROJ_OBJS) 
   $(link)  $(MY_LINK_FLAGS)         \
   $(PROJ_OBJS)						 \
   shell32.lib user32.lib \
   $(EXTRA_LIBS) $(baselibs) \
   -out:$(OUTDIR)\$(PROJ).exe 


clean:
	$(CLEANUP)

