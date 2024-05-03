# Use a base image with your desired Linux distribution
FROM debian:bullseye

# Set environment variables for WeeChat
ENV WEECHAT_HOME /weechat

# Install WeeChat
RUN apt-get update && apt-get install -y weechat && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Expose WeeChat port (should be equal to the port used by the IRC SERVER)
# EXPOSE 8888

# Set up a non-root user to run WeeChat
RUN groupadd -r weechat && useradd -r -g weechat -d $WEECHAT_HOME -s /sbin/nologin -c "WeeChat user" weechat && \
    mkdir -p $WEECHAT_HOME && chown -R weechat:weechat $WEECHAT_HOME

# Set the working directory
WORKDIR $WEECHAT_HOME

# Switch to the non-root user
USER weechat

# Run WeeChat
CMD ["weechat"]
# Run Weechat in the port 8888
#CMD ["weechat", "--config", "irc.server.default.addresses=localhost/8888"]
