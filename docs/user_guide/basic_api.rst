Basic APIs
====================

Basic Classes
--------------------


mace_kit::Context
^^^^^^^^^^^^^^^^^^^^

.. code:: cpp

    #include <macekit/context.h>

Context describes the runtime context and is used as an input to create each
service.

Members include:

* device_type: type `mace_kit::DeviceType`

    `mace_kit::CPU`

    `mace_kit::GPU`

* thread_count (only applicable if running on CPU): type `int`
    normally 1-8

* cpu_affinity_policy (only applicable if running on CPU): type `mace_kit::CPUAffinityPolicy`

    `mace_kit::AFFINITY_NONE`

    `mace_kit::AFFINITY_BIG_ONLY`

    `mace_kit::AFFINITY_LITTLE_ONLY`

    `mace_kit::AFFINITY_HIGH_PERFORMANCE`

    `mace_kit::AFFINITY_POWER_SAVE`,



mace_kit::Status
^^^^^^^^^^^^^^^^^^^^

.. code:: cpp

    #include <macekit/status.h>

Status is returned by methods of each service to indicate if the service succeeds.

Methods include:

mace_kit::Status::ok
"""""""""""""""""""""""""""""""""

Returns: a boolean value to indicate if the service succeeds.


mace_kit::Status::IsInvalidArgument
"""""""""""""""""""""""""""""""""""""
Returns: a boolean value to indicate if the service fails because of invalid argument.

mace_kit::Status::IsRuntimeError
"""""""""""""""""""""""""""""""""""""
Returns: a boolean value to indicate if the service fails because of runtime error.


mace_kit::Mat
^^^^^^^^^^^^^^^^^^^^

.. code:: cpp

    #include <macekit/mat.h>

Mat is internally used as an representation of image tensor.



