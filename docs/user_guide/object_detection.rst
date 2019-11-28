Object Detection
====================

Object detection is a function to detect objects in images by creating rectangles with coordinates.
It can be used to detect front-camera images as well as rear-camera images with one or multiple
objects.

API Reference
--------------

.. code:: cpp

    #include <macekit/object_detection.h>


Summary
^^^^^^^^^

.. list-table::
    :header-rows: 1

    * - Classes
      -
    * - mace_kit::ObjectDetectionContext
      - context of object detection runtime
    * - mace_kit::ObjectDetection
      - object detection main class
    * - mace_kit::Object
      - label, confidence and coordinates of a object
    * - mace_kit::ObjectResult
      - list of detected objects


mace_kit::ObjectDetectionContext
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This class is inherited from mace_kit::Context.



mace_kit::ObjectDetection
^^^^^^^^^^^^^^^^^^^^^^^^^

Methods include:

mace_kit::ObjectDetection::Create
"""""""""""""""""""""""""""""""""

Static method to create a `ObjectDetection` object.

Arguments:

    * `context`: A `ObjectDetectionContext` object

    * `object_detection_ptr`: a pointer of `ObjectDetection` object pointer

Returns:

    * `mace_kit::Status`


mace_kit::ObjectDetection::Detect
""""""""""""""""""""""""""""""""""

Method to detect objects from an image.

Arguments:

    * `mat`: A `Mat` object decoded from an image

    * `max_object_count`: limit max number of objects to detect

    * `result`: pointer of `ObjectResult`


Returns:
    `mace_kit::Status`



mace_kit::Object
^^^^^^^^^^^^^^^

Members include:

* score: type `float`

The detection score to recognize the result as a object.

* localization: type `std::vector<float>`

The 4 coordinates from top to bottom and from left to right. (normalized to the range from 0 to 1


mace_kit::ObjectResult
^^^^^^^^^^^^^^^^^^^^^^

Members include:

* objects: type `vector<Object>`


Library
-------------------

.. list-table::
    :header-rows: 1

    * - library name
    * - libobject_detection.so
    * - libmodel_object_detection.so
    * - libmace.so



