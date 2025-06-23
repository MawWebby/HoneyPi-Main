# From Base Image
FROM debian:bookworm-slim

# Install Needed Packages
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y docker.io
RUN apt-get install -y sudo
RUN apt-get install -y iputils-ping curl bash
RUN apt-get install -y g++
RUN apt-get remove openssh-server -y

# Expose Server Needed Ports
EXPOSE 63599
EXPOSE 11535
EXPOSE 9090

# Expose Port Blocking Needed Ports (9000-9010)
#EXPOSE 9000
#EXPOSE 9001
#EXPOSE 9002
#EXPOSE 9003
#EXPOSE 9004
#EXPOSE 9005
#EXPOSE 9006
#EXPOSE 9007
#EXPOSE 9008
#EXPOSE 9009
#EXPOSE 9010

# CREATE NEW HOME DIRECTORY
RUN mkdir /home/honeypi
RUN mkdir /home/honeypi/cogs
RUN mkdir /home/honeypi/log
RUN touch /home/honeypi/log/log.txt
RUN mkdir /home/honeypi/config
RUN touch /home/honeypi/log/log.txt

# Set Working Directory
COPY . /usr/src/honeypi
WORKDIR /usr/src/honeypi

# This command compiles your app using GCC, adjust for your source code
RUN g++ -o honeypi \
home.cpp \
adminconsole.cpp \
standardloops.cpp \ 
encryption.cpp \
network.cpp 
RUN g++ -o beta beta.cpp

# FINAL VERSION - SPEED BUILDING
#RUN apt-get remove g++ -y
#RUN apt-get autoremove -y

# REMOVE BUILD FILES
RUN mkdir /usr/src/temphoney
RUN mv /usr/src/honeypi/honeypi /usr/src/temphoney/honeypi
RUN mv /usr/src/honeypi/beta /usr/src/temphoney/beta
RUN rm -r /usr/src/honeypi/*
RUN mv /usr/src/temphoney/honeypi /usr/src/honeypi/honeypi
RUN mv /usr/src/temphoney/beta /usr/src/honeypi/beta
RUN rm -r /usr/src/temphoney

# This command runs your application, comment out this line to compile only
ENTRYPOINT ["./honeypi"]

# Versions and Labels
LABEL Name=HoneyPiMain Version=0.6.0
