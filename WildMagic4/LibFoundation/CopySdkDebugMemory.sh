#!/bin/tcsh
mkdir -p ../SDK/Include
mkdir -p ../SDK/Library
mkdir -p ../SDK/Library/DebugMemory

set DIRS = `ls`
foreach dir (${DIRS})
    if (-d $dir && $dir != LibFoundation.xcodeproj && $dir != build) then
        echo $dir
        cd $dir
        set CURDIR = `pwd`
        set HEADERS = `ls *.h *.inl`
        foreach header (${HEADERS})
            cp -fp "${CURDIR}"/$header ../../SDK/Include
        end
        cd ..
    endif
end

set RHEADERS = `ls *.h`
foreach rheader (${RHEADERS})
    cp -fp "${SRCROOT}"/$rheader ../SDK/Include
end

cp -fp "${SRCROOT}"/build/Default/libWm4Foundationmd.a ../SDK/Library/DebugMemory/libWm4Foundationd.a
ranlib ../SDK/Library/DebugMemory/libWm4Foundationd.a
