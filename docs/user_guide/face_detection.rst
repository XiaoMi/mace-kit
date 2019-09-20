Face Detection
====================

Face detection is a function to detect faces in images by creating rectangles with coordinates.
It can be used to detect front-camera images as well as rear-camera images with one or multiple
human faces.

API Reference
--------------

.. code:: cpp

    #include <macekit/face_detection.h>


Summary
^^^^^^^^^

.. list-table::
    :header-rows: 1

    * - Classes
      -
    * - mace_kit::FaceDetectionContext
      - context of face detection runtime
    * - mace_kit::FaceDetection
      - face detection main class
    * - mace_kit::Face
      - score and localization of a face
    * - mace_kit::FaceResult
      - list of detected faces


mace_kit::FaceDetectionContext
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This class is inherited from mace_kit::Context.



mace_kit::FaceDetection
^^^^^^^^^^^^^^^^^^^^^^^^^

Methods include:

mace_kit::FaceDetection::Create
"""""""""""""""""""""""""""""""""

Static method to create a `FaceDetection` object.

Arguments:

    * `context`: A `FaceDetectionContext` object

    * `face_detection_ptr`: a pointer of `FaceDetection` object pointer

Returns:

    * `mace_kit::Status`


mace_kit::FaceDetection::Detect
""""""""""""""""""""""""""""""""""

Method to detect faces from an image.

Arguments:

    * `mat`: A `Mat` object decoded from an image

    * `max_face_count`: limit max number of faces to detect

    * `result`: pointer of `FaceResult`


Returns:
    `mace_kit::Status`



mace_kit::Face
^^^^^^^^^^^^^^^

Members include:

* score: type `float`

The detection score to recognize the result as a face.

* localization: type `std::vector<float>`

The 4 coordinates from top to bottom and from left to right. (normalized to the range from 0 to 1


mace_kit::FaceResult
^^^^^^^^^^^^^^^^^^^^^^

Members include:

* faces: type `vector<Face>`


Library
-------------------

.. list-table::
    :header-rows: 1

    * - library name
    * - libface_detection.so
    * - libmodel_face_detection.so
    * - libmace.so



