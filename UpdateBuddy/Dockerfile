# From Base Image
#
FROM debian:bookworm-slim
#FROM alpine:latest

# Install Needed Packages
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y docker.io

# Set Working Directory
COPY . /home/honeypi
WORKDIR /home/honeypi

# REMOVE BUILD FILES
#RUN mkdir /usr/src/temphoney
#RUN mv /usr/src/honeypi/honeypi /usr/src/temphoney/honeypi
#RUN mv /usr/src/honeypi/defaultconfig.txt /usr/src/temphoney/defaultconfig.txt
#RUN rm -r /usr/src/honeypi/*
#RUN mv /usr/src/temphoney/honeypi /usr/src/honeypi/honeypi
#RUN mv /usr/src/temphoney/defaultconfig.txt /usr/src/honeypi/defaultconfig.txt
#RUN rm -r /usr/src/temphoney
#RUN ls

# This command runs your application, comment out this line to compile only
ENTRYPOINT ["./honeypi"]
#ENTRYPOINT honeypi

# Versions and Labels
LABEL Name=HoneyPi-UpdateHelper Version=1
