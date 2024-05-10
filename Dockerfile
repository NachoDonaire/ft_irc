# Use an official Ubuntu as a parent image
FROM ubuntu:latest

# Update the package lists
RUN apt-get update

# Install HexChat
RUN apt-get install -y hexchat

# Set the default command to run when the container starts
CMD ["hexchat"]

