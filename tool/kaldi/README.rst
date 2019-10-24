Build kaldi-mace for Android
####################


Install docker
==============

Please refer to https://docs.docker.com/engine/installation/.

We have a dockerfile here which provides the environment for
compiling kaldi-mace or just kaldi for Android.


Build the docker image
======================

.. code-block:: bash

  $ cd docker
  $ docker build -t mace-kit/docker-kaldi-android:latest .


Apply kaldi-mace path
=====================

First of all, clone Kaldi repository to somewhere in your machine.
Then apply the kaldi-mace patch into kaldi and copy the libmace.so to tools/libmace/lib/libmace.so.

.. code-block:: bash

  $ git clone https://github.com/kaldi-asr/kaldi
  $ git apply --check /path/to/mace-kit/tool/kaldi/kaldi-mace.diff
  $ git apply /path/to/mace-kit/tool/kaldi/kaldi-mace.diff


Build MACE
==========

Please refer to https://mace.readthedocs.io/en/latest/user_guide/basic_usage_cmake.html

.. code-block:: bash

  $ cd /path/to/mace
  $ RUNTIME=CPU bash tools/cmake/cmake-build-armeabi-v7a.sh
  $ cp -r build/cmake-build/armeabi-v7a/install/include/mace /path/to/kaldi/tools/libmace/include/mace
  $ cp build/cmake-build/armeabi-v7a/install/lib/libmace.so /path/to/kaldi/tools/libmace/libmace.so


Build kaldi-mace
================

Run a container, mounting the kaldi repository you just cloned to /opt/kaldi:

.. code-block:: bash

  $ docker run -v /path/to/kaldi:/opt/kaldi mace-kit/docker-kaldi-android:latest


When ``docker run`` finishes, the compiled libkaldi-mace.so will be located
in ``/path/to/kaldi/src/mace`` subdirectories.

Then you can copy the kaldi-mace library and header file to mace-kit.

.. code-block:: bash

  $ cp /path/to/kaldi/src/mace/libkaldi-mace.so path/to/mace-kit/third_party/armeabi-v7a/libkaldi-mace.so
  $ cp /path/to/kaldi/src/mace/kaldi-mace.h path/to/mace-kit/third_party/include/kaldi-mace.h