Person Segmentation
====================

Person segmentation is a function to detect persons and segment them from images by giving a score map and label map.
It can be used to build an AI Bokeh application for front-camera as well as rear-camera with one or multiple persons.

API Reference
-------------

.. code:: cpp

    #include <macekit/person_segmentation.h>


Summary
^^^^^^^

.. list-table::
    :header-rows: 1

    * - Classes
      -
    * - mace_kit::PersonSegmentationContext
      - context of person segmentation runtime
    * - mace_kit::PersonSegmentation
      - person segmentation main class
    * - mace_kit::PersonSegmentationResult
      - score map and result map of a person in the image


mace_kit::PersonSegmentationContext
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This class is inherited from mace_kit::Context.



mace_kit::PersonSegmentation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Methods include:

mace_kit::PersonSegmentation::Create
""""""""""""""""""""""""""""""""""""

Static method to create a `PersonSegmentation` object.

Arguments:

    * `context`: A `PersonSegmentationContext` object

    * `person_segmentation_ptr`: a pointer of `PersonSegmentation` object pointer

Returns:

    * `mace_kit::Status`


mace_kit::PersonSegmentation::Segment
"""""""""""""""""""""""""""""""""""""

Method to segment persons from an image.

Arguments:

    * `mat`: A `Mat` object decoded from an image

    * `result`: pointer of `PersonSegmentationResult`


Returns:
    `mace_kit::Status`



mace_kit::PersonSegmentationResult
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Members include:

* score_map: type `map`, front image with score.

* label_map: type `map`, front image with label, 0 for background, 1 for person


Library
-------

.. list-table::
    :header-rows: 1

    * - library name
    * - libperson_segmentation.so
    * - libmodel_person_segmentation.so
    * - libmace.so



