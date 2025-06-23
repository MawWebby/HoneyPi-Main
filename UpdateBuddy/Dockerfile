# From Base Image
FROM debian:bookworm-slim

# Install Needed Packages
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y docker.io g++

# Set Working Directory
COPY . /usr/src/honeypi
WORKDIR /usr/src/honeypi

# This command compiles your app using GCC, adjust for your source code
RUN g++ -o honeypi updatescript.cpp

# REMOVE BUILD FILES
RUN mkdir /usr/src/temphoney
RUN mv /usr/src/honeypi/honeypi /usr/src/temphoney/honeypi
RUN rm -r /usr/src/honeypi/*
RUN mv /usr/src/temphoney/honeypi /usr/src/honeypi/honeypi
RUN rm -r /usr/src/temphoney

# This command runs your application, comment out this line to compile only
ENTRYPOINT ["./honeypi"]

# Versions and Labels
LABEL Name=HoneyPi-UpdateHelper Version=1
