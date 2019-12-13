Speech Recognition
==================

Speech recognition is a function to automatically recognize speech.

It can be used to recognize speech wav files and gives the text results.

API Reference
-------------

.. code:: cpp

    #include <macekit/speech_recognition.h>


kaldi::MaceNnet3WavDecode
"""""""""""""""""""""""""

Method to recognition speech from a wav file.

Arguments:

    * `configure_file`: A `string` object indicates the configuration file of the decoding model

    * `fst_file`: A `string` object indicates the fst file fo the decoding model

    * `wav_file`: A `string` object indicates the wav file to be decoded.

    * `prediction_file`: A `string` object indicates the file to save output prediction results.

    * `log_level`: A `string` of number (0~5), which indicates the debug log level.

Returns:
    `std::string`: the decoding result texts.


Library
-------

.. list-table::
    :header-rows: 1

    * - library name
    * - libkaldi-mace.so
    * - libmace.so