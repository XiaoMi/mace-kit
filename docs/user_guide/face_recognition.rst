Face Recognition
====================

Face recognition is a function to compute enbedding of face feature.
It can be used to recognize front-camera face images with only one face in it.

API Reference
--------------

.. code:: cpp

    #include <macekit/face_recognition.h>


Summary
^^^^^^^^^

.. list-table::
    :header-rows: 1

    * - Classes
      -
    * - mace_kit::FaceRecognitionContext
      - context of face recognition runtime
    * - mace_kit::FaceRecognition
      - face recognition main class
    * - mace_kit::Face
      - score and localization of a face
    * - mace_kit::FaceResult
      - list of detected faces


mace_kit::FaceRecognitionContext
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This class is inherited from mace_kit::Context.


mace_kit::FaceRecognition
^^^^^^^^^^^^^^^^^^^^^^^^^^

Methods include:

mace_kit::FaceRecognition::Create
""""""""""""""""""""""""""""""""""

Static method to create a `FaceRecognition` object.

Arguments:

    * `context`: A `FaceRecognitionContext` object

    * `face_recognition_ptr`: a pointer of `FaceRecognition` object pointer

Returns:

    * `mace_kit::Status`


mace_kit::FaceRecognition::ComputeEmbedding
"""""""""""""""""""""""""""""""""""""""""""""

Method to compute embedding of a face.

Arguments:

    * `mat`: A `Mat` object decoded from an image

    * `embed`: a vector of float embedding with size of 128


Returns:
    `mace_kit::Status`



mace_kit::Compare
^^^^^^^^^^^^^^^^^^^^

Method to compare two faces to see if they are of the same person.

Arguments:

    * `lhs_mat`: A `Mat` object decoded from an image

    * `rhs_mat`: A `Mat` object decoded from another image

    * `similarity`: pointer to a float number to specify the similarity of two faces

Returns:
    `mace_kit::Status`




Library
-------------------

.. list-table::
    :header-rows: 1

    * - library name
    * - libface_recognition.so
    * - libmodel_face_recognition.so
    * - libmace.so



