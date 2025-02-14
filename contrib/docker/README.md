ebusd Docker image
==================

An [ebusd](https://github.com/john30/ebusd/) Docker image is available on the
[Docker Hub](https://hub.docker.com/r/john30/ebusd/) and is able to download the latest released German
[configuration files](https://github.com/john30/ebusd-configuration/) from a dedicated webservice.

It allows you to run ebusd without actually installing (or even building) it on your system.
You might even be able to run it on a non-Linux operating system, which is at least known to
work well on a Synology Diskstation as well as Windows with Docker Desktop.


Getting the image
-----------------
To download the latest release image from the hub, use the following command:  
> docker pull john30/ebusd

The image is able to run on any of the following architectures and the right image will be picked automatically:
* amd64
* i386
* arm32v7
* arm64v8

Due to changes of docker hub policies, the development set of images with "devel" tag are currently not automatically
built with every commit to the git repository. Run the following command to use it: 

> docker pull john30/ebusd:devel


Running interactively
---------------------

To run an ebusd container interactively, e.g. on serial device /dev/ttyUSB1, use the following command:
> docker run --rm -it --device=/dev/ttyUSB1:/dev/ttyUSB0 -p 8888 john30/ebusd

This will show the ebusd output directly in the terminal.


Running in background
---------------------

To start an ebusd container and have it run in the background, e.g. on serial device /dev/ttyUSB1, use the following command:
> docker run -d --name=ebusd --device=/dev/ttyUSB1:/dev/ttyUSB0 -p 8888 john30/ebusd

The container has the name "ebusd", so you can use that when querying docker about the container.

In order to get the log output from ebusd, use the following command:
> docker logs ebusd


Using a network device
----------------------

When using a network device, the "--device" argument to docker can be omitted, but the device information has to be
passed on to ebusd:
> docker run --rm -it -p 8888 john30/ebusd -f --scanconfig -d 192.168.178.123:10000 --latency=20

Note: the default "-f" and "--scanconfig" arguments are only passed to ebusd if it is called without any additional arguments.
So when passing further arguments, these two usually need to be added as well.


Running with MQTT broker
------------------------
To start an ebusd container in the background and have it connect to your MQTT broker, use the following command while
replacing "BROKERHOST" with your MQTT broker host name or IP address:
> docker run -d --name=ebusd --device=/dev/ttyUSB0 -p 8888 john30/ebusd -f --scanconfig -d /dev/ttyUSB0 --mqttport=1883 --mqtthost=BROKERHOST
