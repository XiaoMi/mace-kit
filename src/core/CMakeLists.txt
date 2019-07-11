set(CORE_SRCS
        mat.cc
        ../../include/macekit/type.h ../../include/macekit/status.h ../../include/macekit/export.h ../face_recognition/face_recognition_impl.cc ../face_recognition/face_recognition_impl.h ../../include/macekit/face_recognition.h ../../include/macekit/context.h)

add_library(core STATIC ${CORE_SRCS})

