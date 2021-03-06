project(lugaru)

cmake_minimum_required(VERSION 2.6)

set(SRCDIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")
set(DEPDIR "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel"
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

if(NOT LUGARU_INSTALL_PREFIX)
  if(WIN32)
     set(LUGARU_INSTALL_PREFIX "C:/Lugaru" CACHE PATH
     "LUGARU_INSTALL_PREFIX: Install path prefix, prepended onto install directories."
      FORCE)
  else(WIN32)
     set(LUGARU_INSTALL_PREFIX "/usr/local/lugaru" CACHE PATH
     "CMAKE_INSTALL_PREFIX: Install path prefix, prepended onto install directories."
      FORCE)
  endif(WIN32)
endif(NOT LUGARU_INSTALL_PREFIX)

set(CMAKE_INSTALL_PREFIX "${LUGARU_INSTALL_PREFIX}" CACHE INTERNAL "Prefix prepended to install directories" FORCE)

if(MINGW)
	set(OPENGL_gl_LIBRARY "-lopengl32" CACHE STRING "OpenGL library for Win32" FORCE)
	set(OPENGL_glu_LIBRARY "-lglu32" CACHE STRING "GLU library for Win32" FORCE)
endif(MINGW)

if(APPLE)
	set(CMAKE_OSX_ARCHITECTURES "i386;x86_64;ppc" CACHE STRING "Build architectures for OSX")
	set(CMAKE_OSX_DEPLOYMENT_TARGET "10.5" CACHE STRING 
		"Minimum OS X version to target for deployment (at runtime); newer APIs weak linked. Set to empty string for default value")
	set(CMAKE_OSX_SYSROOT "/Developer/SDKs/MacOSX10.5.sdk" CACHE PATH 
		"The product will be built against the headers and libraries located inside the indicated SDK.")
endif(APPLE)

if(APPLE)
	# Save our sanity; Set all available libraries to internal by default
	set(LUGARU_FORCE_INTERNAL_SDL True)
	set(LUGARU_FORCE_INTERNAL_GLU True)
	set(LUGARU_FORCE_INTERNAL_JPEG True)
	set(LUGARU_FORCE_INTERNAL_PNG True)
	set(LUGARU_FORCE_INTERNAL_VORBIS True)
	set(LUGARU_FORCE_INTERNAL_ZLIB True)
endif(APPLE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(LUGARU_SRCS
	${SRCDIR}/Frustum.cpp
	${SRCDIR}/GameDraw.cpp
	${SRCDIR}/GameInitDispose.cpp
	${SRCDIR}/GameTick.cpp
	${SRCDIR}/Globals.cpp
	${SRCDIR}/Lights.cpp
	${SRCDIR}/Models.cpp
	${SRCDIR}/Objects.cpp
	${SRCDIR}/pack.c
	${SRCDIR}/pack_private.c
	${SRCDIR}/Person.cpp
	${SRCDIR}/private.c
	${SRCDIR}/Quaternions.cpp
	${SRCDIR}/Random.c
	${SRCDIR}/Skeleton.cpp
	${SRCDIR}/Skybox.cpp
	${SRCDIR}/Sprites.cpp
	${SRCDIR}/Terrain.cpp
	${SRCDIR}/Text.cpp
	${SRCDIR}/TGALoader.cpp
	${SRCDIR}/unpack.c
	${SRCDIR}/unpack_private.c
	${SRCDIR}/Weapons.cpp
	${SRCDIR}/OpenGL_Windows.cpp
	${SRCDIR}/openal_wrapper.cpp
	${SRCDIR}/WinInput.cpp
)

set(LUGARU_H
	${SRCDIR}/Constants.h
	${SRCDIR}/Frustum.h
	${SRCDIR}/Game.h
	${SRCDIR}/Lights.h
	${SRCDIR}/LinkedList.h
	${SRCDIR}/Models.h
	${SRCDIR}/Objects.h
	${SRCDIR}/Person.h
	${SRCDIR}/PhysicsMath.h
	${SRCDIR}/Pointer.h
	${SRCDIR}/Quaternions.h
	${SRCDIR}/Random.h
	${SRCDIR}/Skeleton.h
	${SRCDIR}/Skybox.h
	${SRCDIR}/Sprites.h
	${SRCDIR}/TGALoader.h
	${SRCDIR}/Terrain.h
	${SRCDIR}/Text.h
	${SRCDIR}/Weapons.h
	${SRCDIR}/WinInput.h
	${SRCDIR}/alstubs.h
	${SRCDIR}/binio.h
	${SRCDIR}/openal_wrapper.h
	${SRCDIR}/gamegl.h
	${SRCDIR}/glstubs.h
	${SRCDIR}/mmgr.h
	${SRCDIR}/nommgr.h
	${SRCDIR}/pack_private.h
	${SRCDIR}/private.h
	${SRCDIR}/unpack_private.h
)

if(UNIX)
	set(LUGARU_SRCS
		${LUGARU_SRCS}
		${SRCDIR}/MacCompatibility.cpp
	)
	set(LUGARU_H
		${LUGARU_H}
		${SRCDIR}/MacCompatibility.h
	)
endif(UNIX)

if(MSVC) # MSVC non-C99 support biting us hard
	set(LUGARU_H
		${LUGARU_H}
		${DEPDIR}/msinttypes/stdint.h
		${DEPDIR}/msinttypes/inttypes.h
	)
endif(MSVC)

if(WIN32)
  if(CMAKE_CROSSCOMPILING)
    ADD_CUSTOM_COMMAND( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lugaru_resource.obj
						  COMMAND i686-pc-mingw32-windres
							  -I${SRCDIR}/win-res
							  -o ${CMAKE_CURRENT_BINARY_DIR}/lugaru_resource.obj
							  -i${SRCDIR}/win-res/Lugaru.rc
					      DEPENDS ${SRCDIR}/win-res/Lugaru.rc
						  )
  endif(CMAKE_CROSSCOMPILING)
  if(NOT CMAKE_CROSSCOMPILING)
   if(MSVC)
    ADD_CUSTOM_COMMAND( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lugaru_resource.obj
						  COMMAND rc
							  -I${SRCDIR}/win-res
							  -fo${CMAKE_CURRENT_BINARY_DIR}/lugaru_resource.obj
							  ${SRCDIR}/win-res/Lugaru.rc
					      DEPENDS ${SRCDIR}/win-res/Lugaru.rc
						  )
   endif(MSVC)
   if(MINGW)
    ADD_CUSTOM_COMMAND( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lugaru_resource.obj
						  COMMAND windres
							  -I${SRCDIR}/win-res
							  -o ${CMAKE_CURRENT_BINARY_DIR}/lugaru_resource.obj
							  -i${SRCDIR}/win-res/Lugaru.rc
					      DEPENDS ${SRCDIR}/win-res/Lugaru.rc
						  )
   endif(MINGW)
  endif(NOT CMAKE_CROSSCOMPILING)

  # !!! FIXME: get rid of this.
  set(LUGARU_SRCS
	${LUGARU_SRCS}
	${SRCDIR}/WinDefs.cpp)

  set(LUGARU_H
	${LUGARU_H}
	${SRCDIR}/WinDefs.h
	${SRCDIR}/win-res/resource.h)
endif(WIN32)

if (APPLE)
    set(PLATFORM_LIBS "-framework Carbon -framework Cocoa -framework OpenGL -framework OpenAL")
endif (APPLE)

# Deal with dependencies...
find_package(OpenGL REQUIRED)
# force this include dir no matter what on Windows, so we get sane headers.
if(NOT WIN32)
	option (LUGARU_FORCE_INTERNAL_OPENGL "Force internal OpenGL headers, even if there's a system version" True)
	if (LUGARU_FORCE_INTERNAL_OPENGL)
		include_directories("${DEPDIR}/OpenGL")
	endif (LUGARU_FORCE_INTERNAL_OPENGL)
endif(NOT WIN32)

if(WIN32)
	include_directories("${DEPDIR}/OpenGL")
endif(WIN32)

if(MSVC) # More Visual Studio annoyances
	include_directories("${DEPDIR}/msinttypes")
endif(MSVC)

option (LUGARU_FORCE_INTERNAL_OPENAL "Force internal libOpenAL, even if there's a system version" False)
if (NOT LUGARU_FORCE_INTERNAL_OPENAL)
	find_package(OpenAL REQUIRED)
else(NOT LUGARU_FORCE_INTERNAL_OPENAL)
	set(OPENAL_FOUND False)
endif (NOT LUGARU_FORCE_INTERNAL_OPENAL)

if (NOT OPENAL_FOUND OR LUGARU_FORCE_INTERNAL_OPENAL)
    message(STATUS "Using internal copy of OpenAL")
    set(LUGARU_MISSING_DEPS "${LUGARU_MISSING_DEPS} OpenAL")
    set(OPENALDIR "${DEPDIR}/OpenAL")
    set(OPENAL_INCLUDE_DIR "${OPENALDIR}/include/AL")
    set(OPENAL_LIBRARY "")

    if (WIN32)
        set(LUGARU_HAS_INTERNAL_OPENAL True)
        if (MSVC80)
            set(OPENAL_LIBRARY
                ${OPENALDIR}/lib/win32/msvc2005/OpenAL32.lib
            )
        endif (MSVC80)
        if (MSVC90)
            set(OPENAL_LIBRARY
                ${OPENALDIR}/lib/win32/msvc2008/OpenAL32.lib
            )
        endif (MSVC90)
        if (MINGW)
            set(OPENAL_LIBRARY
                ${OPENALDIR}/lib/win32/mingw/libOpenAL32.dll.a
            )
        endif (MINGW)
     endif(WIN32)

    if (NOT LUGARU_HAS_INTERNAL_OPENAL)
        message(ERROR "We don't have a prebuilt OpenAL for this platform.")
    endif (NOT LUGARU_HAS_INTERNAL_OPENAL)
endif (NOT OPENAL_FOUND OR LUGARU_FORCE_INTERNAL_OPENAL)

option (LUGARU_FORCE_INTERNAL_SDL "Force internal libSDL, even if there's a system version" False)
if (NOT LUGARU_FORCE_INTERNAL_SDL)
    find_package(SDL)
else(NOT LUGARU_FORCE_INTERNAL_SDL)
	set(SDL_FOUND False)
endif (NOT LUGARU_FORCE_INTERNAL_SDL)

if (NOT SDL_FOUND)
    message(STATUS "Using internal copy of SDL")
    set(LUGARU_MISSING_DEPS "${LUGARU_MISSING_DEPS} SDL")
    set(SDLDIR "${DEPDIR}/SDL12")
    set(SDL_INCLUDE_DIR "${SDLDIR}/include")
    set(SDL_LIBRARY "")
    if (APPLE)
        set(LUGARU_HAS_INTERNAL_SDL True)
        set(SDL_LIBRARY
            ${SDLDIR}/lib/macosx/libSDL-1.2.0.dylib
            ${SDLDIR}/lib/macosx/libSDLmain-osx.a
        )
    endif (APPLE)

    if (WIN32)
        set(LUGARU_HAS_INTERNAL_SDL True)
        if (MSVC80)
          set(SDL_LIBRARY
              ${SDLDIR}/lib/win32/msvc2005/SDLmain.lib
              ${SDLDIR}/lib/win32/msvc2005/SDL.lib
          )
        endif (MSVC80)
        if (MSVC90)
          set(SDL_LIBRARY
              ${SDLDIR}/lib/win32/msvc2008/SDLmain.lib
              ${SDLDIR}/lib/win32/msvc2008/SDL.lib
          )
        endif (MSVC90)
        if (MINGW)
          set(SDL_LIBRARY
              "mingw32"
              ${SDLDIR}/lib/win32/mingw/libSDLmain.a
              ${SDLDIR}/lib/win32/mingw/libSDL.dll.a
              "-mwindows"
          )
        endif (MINGW)
    endif (WIN32)

    if (NOT LUGARU_HAS_INTERNAL_SDL)
        message(ERROR "We don't have a prebuilt SDL for this platform.")
    endif (NOT LUGARU_HAS_INTERNAL_SDL)
endif (NOT SDL_FOUND)

option (LUGARU_FORCE_INTERNAL_PNG "Force internal libPNG, even if there's a system version" False)
if (NOT LUGARU_FORCE_INTERNAL_PNG)
    find_package(PNG)
else(NOT LUGARU_FORCE_INTERNAL_PNG)
	set(PNG_FOUND False)
endif (NOT LUGARU_FORCE_INTERNAL_PNG)

if (NOT PNG_FOUND)
    message(STATUS "Using internal copy of libpng")
    set(LUGARU_MISSING_DEPS "${LUGARU_MISSING_DEPS} PNG")
    set(PNGDIR "${DEPDIR}/libpng")
    set(PNG_INCLUDE_DIR "${PNGDIR}")
    set(PNG_LIBRARY "")
    set(LUGARU_SRCS
        ${LUGARU_SRCS}
        ${PNGDIR}/png.c
        ${PNGDIR}/pngerror.c
        ${PNGDIR}/pngget.c
        ${PNGDIR}/pngmem.c
        ${PNGDIR}/pngpread.c
        ${PNGDIR}/pngread.c
        ${PNGDIR}/pngrio.c
        ${PNGDIR}/pngrtran.c
        ${PNGDIR}/pngrutil.c
        ${PNGDIR}/pngset.c
        ${PNGDIR}/pngtrans.c
        ${PNGDIR}/pngwio.c
        ${PNGDIR}/pngwrite.c
        ${PNGDIR}/pngwtran.c
        ${PNGDIR}/pngwutil.c
    )
endif (NOT PNG_FOUND)

option (LUGARU_FORCE_INTERNAL_JPEG "Force internal libJPEG, even if there's a system version" False)
if (NOT LUGARU_FORCE_INTERNAL_JPEG)
    find_package(JPEG)
else(NOT LUGARU_FORCE_INTERNAL_JPEG)
	set(JPEG_FOUND False)
endif (NOT LUGARU_FORCE_INTERNAL_JPEG)

if (NOT JPEG_FOUND)
    message(STATUS "Using internal copy of libjpeg")
    set(LUGARU_MISSING_DEPS "${LUGARU_MISSING_DEPS} JPEG")
    set(JPEGDIR "${DEPDIR}/libjpeg")
    set(JPEG_INCLUDE_DIR "${JPEGDIR}")
    set(JPEG_LIBRARY "")
    set(LUGARU_SRCS
        ${LUGARU_SRCS}
        ${JPEGDIR}/jdapistd.c
        ${JPEGDIR}/jdmaster.c
        ${JPEGDIR}/jdapimin.c
        ${JPEGDIR}/jcapimin.c
        ${JPEGDIR}/jdmerge.c
        ${JPEGDIR}/jdatasrc.c
        ${JPEGDIR}/jdatadst.c
        ${JPEGDIR}/jdcoefct.c
        ${JPEGDIR}/jdcolor.c
        ${JPEGDIR}/jddctmgr.c
        ${JPEGDIR}/jdhuff.c
        ${JPEGDIR}/jdinput.c
        ${JPEGDIR}/jdmainct.c
        ${JPEGDIR}/jdmarker.c
        ${JPEGDIR}/jdpostct.c
        ${JPEGDIR}/jdsample.c
        ${JPEGDIR}/jdtrans.c
        ${JPEGDIR}/jerror.c
        ${JPEGDIR}/jidctflt.c
        ${JPEGDIR}/jidctfst.c
        ${JPEGDIR}/jidctint.c
        ${JPEGDIR}/jmemmgr.c
        ${JPEGDIR}/jutils.c
        ${JPEGDIR}/jmemnobs.c
        ${JPEGDIR}/jquant1.c
        ${JPEGDIR}/jquant2.c
        ${JPEGDIR}/jcomapi.c
        ${JPEGDIR}/jcmarker.c
        ${JPEGDIR}/jcapistd.c
        ${JPEGDIR}/jcparam.c
        ${JPEGDIR}/jcinit.c
        ${JPEGDIR}/jcdctmgr.c
        ${JPEGDIR}/jccoefct.c
        ${JPEGDIR}/jcmainct.c
        ${JPEGDIR}/jfdctflt.c
        ${JPEGDIR}/jfdctint.c
        ${JPEGDIR}/jfdctfst.c
        ${JPEGDIR}/jchuff.c
        ${JPEGDIR}/jcsample.c
        ${JPEGDIR}/jcmaster.c
        ${JPEGDIR}/jccolor.c
        ${JPEGDIR}/jcprepct.c
        ${JPEGDIR}/jcarith.c
        ${JPEGDIR}/jdarith.c
        ${JPEGDIR}/jaricom.c
    )
endif (NOT JPEG_FOUND)

option (LUGARU_FORCE_INTERNAL_ZLIB "Force internal zlib, even if there's a system version" False)
if (NOT LUGARU_FORCE_INTERNAL_ZLIB)
    find_package(ZLIB)
else(NOT LUGARU_FORCE_INTERNAL_ZLIB)
	set(ZLIB_FOUND False)
endif (NOT LUGARU_FORCE_INTERNAL_ZLIB)

if (NOT ZLIB_FOUND)
    message(STATUS "Using internal copy of zlib")
    set(LUGARU_MISSING_DEPS "${LUGARU_MISSING_DEPS} ZLIB")
    set(ZLIBDIR "${DEPDIR}/zlib")
    set(ZLIB_INCLUDE_DIR "${ZLIBDIR}")
    set(ZLIB_LIBRARIES "")
    set(LUGARU_SRCS
        ${LUGARU_SRCS}
        ${ZLIBDIR}/adler32.c
        ${ZLIBDIR}/compress.c
        ${ZLIBDIR}/crc32.c
        ${ZLIBDIR}/deflate.c
        ${ZLIBDIR}/infback.c
        ${ZLIBDIR}/inffast.c
        ${ZLIBDIR}/inflate.c
        ${ZLIBDIR}/inftrees.c
        ${ZLIBDIR}/trees.c
        ${ZLIBDIR}/uncompr.c
        ${ZLIBDIR}/zutil.c
    )
endif (NOT ZLIB_FOUND)

option (LUGARU_FORCE_INTERNAL_GLU "Force internal libGLU, even if there's a system version" False)
if (LUGARU_FORCE_INTERNAL_GLU)
	set(OPENGL_GLU_FOUND False)
endif (LUGARU_FORCE_INTERNAL_GLU)

if (NOT OPENGL_GLU_FOUND)
    message(STATUS "Using internal copy of libGLU")
    set(LUGARU_MISSING_DEPS "${LUGARU_MISSING_DEPS} GLU")
    set(GLUDIR "${DEPDIR}/GLU")
    set(GLU_INCLUDE_DIR "${GLUDIR}")
    set(GLU_LIBRARY "")
    set(LUGARU_SRCS
        ${LUGARU_SRCS}
        ${GLUDIR}/dict.c
        ${GLUDIR}/geom.c
        ${GLUDIR}/memalloc.c
        ${GLUDIR}/mesh.c
        ${GLUDIR}/mipmap.c
        ${GLUDIR}/normal.c
        ${GLUDIR}/priorityq.c
        ${GLUDIR}/render.c
        ${GLUDIR}/sweep.c
        ${GLUDIR}/tess.c
        ${GLUDIR}/tessmono.c
        ${GLUDIR}/util.c
    )
endif (NOT OPENGL_GLU_FOUND)

option (LUGARU_FORCE_INTERNAL_VORBIS "Force internal Vorbis, even if there's a system version" False)
if (NOT LUGARU_FORCE_INTERNAL_VORBIS)
    find_package(OggVorbis)
else(NOT LUGARU_FORCE_INTERNAL_VORBIS)
	set(OGGVORBIS_FOUND False)
endif (NOT LUGARU_FORCE_INTERNAL_VORBIS)

if (NOT OGGVORBIS_FOUND)
    message(STATUS "Using internal copy of Ogg Vorbis")
    set(LUGARU_MISSING_DEPS "${LUGARU_MISSING_DEPS} OggVorbis")
    set(OGGDIR "${DEPDIR}/libogg")
    set(OGG_INCLUDE_DIR "${OGGDIR}/include")
    set(OGG_LIBRARY "")
    set(VORBISDIR "${DEPDIR}/libvorbis")
    set(VORBISFILE_INCLUDE_DIR "${VORBISDIR}/include")
    set(VORBISFILE_LIBRARY "")
    set(LUGARU_SRCS
        ${LUGARU_SRCS}
        ${OGGDIR}/src/bitwise.c
        ${OGGDIR}/src/framing.c
        ${VORBISDIR}/lib/analysis.c
        ${VORBISDIR}/lib/bitrate.c
        ${VORBISDIR}/lib/block.c
        ${VORBISDIR}/lib/codebook.c
        ${VORBISDIR}/lib/envelope.c
        ${VORBISDIR}/lib/floor0.c
        ${VORBISDIR}/lib/floor1.c
        ${VORBISDIR}/lib/info.c
        ${VORBISDIR}/lib/lpc.c
        ${VORBISDIR}/lib/lsp.c
        ${VORBISDIR}/lib/mapping0.c
        ${VORBISDIR}/lib/mdct.c
        ${VORBISDIR}/lib/psy.c
        ${VORBISDIR}/lib/registry.c
        ${VORBISDIR}/lib/res0.c
        ${VORBISDIR}/lib/sharedbook.c
        ${VORBISDIR}/lib/smallft.c
        ${VORBISDIR}/lib/synthesis.c
        ${VORBISDIR}/lib/vorbisfile.c
        ${VORBISDIR}/lib/window.c
    )
endif (NOT OGGVORBIS_FOUND)

include_directories(
    ${OPENAL_INCLUDE_DIR}
    ${JPEG_INCLUDE_DIR}
    ${PNG_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${GLU_INCLUDE_DIR}
    ${SDL_INCLUDE_DIR}
    ${VORBISFILE_INCLUDE_DIR}
    ${OGG_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/Source
)

set(LUGARU_LIBS ${OPENAL_LIBRARY} ${PNG_LIBRARY} ${JPEG_LIBRARY} ${ZLIB_LIBRARIES} ${SDL_LIBRARY} ${GLU_LIBRARY} ${OPENGL_LIBRARY} ${VORBISFILE_LIBRARY} ${OGG_LIBRARY} ${PLATFORM_LIBS})


if(WIN32)
    add_executable(lugaru ${LUGARU_SRCS} ${LUGARU_H} lugaru_resource.obj)
else(WIN32)
    add_executable(lugaru ${LUGARU_SRCS} ${LUGARU_H})
endif(WIN32)

target_link_libraries(lugaru ${LUGARU_LIBS})

if(WIN32)
	add_definitions(-DUSE_OPENAL=1 -DUSE_SDL=1 -DBinIO_STDINT_HEADER=<stdint.h>)
else(WIN32)
	add_definitions(-DPLATFORM_LINUX=1 -DPLATFORM_UNIX=1 -DUSE_OPENAL=1 -DUSE_SDL=1 -DBinIO_STDINT_HEADER=<stdint.h>)
endif(WIN32)

# Install target
if(APPLE)
	set(APPS_ROOT "${CMAKE_INSTALL_PREFIX}/Lugaru.app")
	set(APPS_BIN "${APPS_ROOT}/Contents/MacOS")
	set(APPS_DATA "${APPS_ROOT}/Contents/Resources")
endif(APPLE)

if(WIN32)
	install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/lugaru.exe DESTINATION ${CMAKE_INSTALL_PREFIX})
	if(MSVC80)
		install(FILES ${SDLDIR}/lib/win32/msvc2005/SDL.dll DESTINATION ${CMAKE_INSTALL_PREFIX})
		install(FILES ${OPENALDIR}/lib/win32/msvc2005/OpenAL32.dll DESTINATION ${CMAKE_INSTALL_PREFIX})
	endif(MSVC80)
	if(MSVC90)
		install(FILES ${SDLDIR}/lib/win32/msvc2008/SDL.dll DESTINATION ${CMAKE_INSTALL_PREFIX})
		install(FILES ${OPENALDIR}/lib/win32/msvc2008/OpenAL32.dll DESTINATION ${CMAKE_INSTALL_PREFIX})
	endif(MSVC90)
	if(MINGW)
		install(FILES ${SDLDIR}/lib/win32/mingw/SDL.dll DESTINATION ${CMAKE_INSTALL_PREFIX})
		install(FILES ${OPENALDIR}/lib/win32/mingw/OpenAL32.dll DESTINATION ${CMAKE_INSTALL_PREFIX})
	endif(MINGW)
else(WIN32)
	if(APPLE)
		set(CMAKE_INSTALL_PREFIX "${APPS_BIN}")
		install(FILES ${SRCDIR}/mac-res/lugaru.icns DESTINATION ${APPS_DATA})
		install(FILES ${SRCDIR}/mac-res/Info.plist DESTINATION ${APPS_ROOT}/Contents)
	endif(APPLE)
	install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/lugaru DESTINATION ${CMAKE_INSTALL_PREFIX})
	if(APPLE AND LUGARU_HAS_INTERNAL_SDL)
		install(FILES ${SDLDIR}/lib/macosx/libSDL-1.2.0.dylib DESTINATION ${CMAKE_INSTALL_PREFIX})
	endif(APPLE AND LUGARU_HAS_INTERNAL_SDL)
endif(WIN32)

if(NOT APPLE)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/Data DESTINATION ${CMAKE_INSTALL_PREFIX})
endif(NOT APPLE)

if(APPLE)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/Data DESTINATION ${APPS_ROOT})
endif(APPLE)

if (LUGARU_MISSING_DEPS)
    message(STATUS "Using our copy of these libs: ${LUGARU_MISSING_DEPS}")
endif (LUGARU_MISSING_DEPS)

